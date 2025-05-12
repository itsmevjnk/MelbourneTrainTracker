const gtfs = require('./gtfs_rt');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, update, values } = pgp.helpers;
const ptvapi = require('./ptvapi');

/* ColumnSet for updating - globally declared for caching */
const cs = new ColumnSet(
    [
        '?trip_id',
        '?seq:value',
        {
            name: 'arrival',
            cast: 'timestamp with time zone'
        },
        {
            name: 'departure',
            cast: 'timestamp with time zone'
        },
        {
            name: 'last_updated',
            cast: 'timestamp with time zone'
        }
    ],
    { 
        table: new TableName({
            schema: 'daily',
            table: 'timetable'
        })
    }
);

const getTripIDPattern = (id) => {
    const idParts = id.split('-');
    return `${idParts[0]}-${idParts[1]}-%-${idParts[4]}-${idParts[5]}`;
};

const csRefQuery = new ColumnSet(
    [
        'trip_id', 'line',
        {
            name: 'time',
            cast: 'timestamp with time zone'
        },
        'id_pattern'
    ]
);

const csTripInsert = new ColumnSet(
    [
        'trip_id', 'line', 'seq',
        {
            name: 'arrival',
            cast: 'timestamp with time zone'
        },
        {
            name: 'departure',
            cast: 'timestamp with time zone'
        },
        'station_name'
    ]
);

const updateTimetable = (lastTimestamp = null) => {
    return gtfs.fetchUpdate()
    .then((data) => {
        if (lastTimestamp != null && lastTimestamp.getTime() == data.timestamp.getTime()) return null; // indicate that the data has not been updated

        const cancelPatterns = [];
        for (const id of data.cancellations) {
            cancelPatterns.push(getTripIDPattern(pgp.as.value(id)));
        }
        const deleteQuery = `DELETE FROM daily.timetable WHERE trip_id SIMILAR TO '${cancelPatterns.join('|')}'`;
        // console.log(deleteQuery);

        const updates = [];
        const ids = {};
        for (const [ tripID, tripUpdates ] of Object.entries(data.updates)) {
            const tripIDPattern = getTripIDPattern(tripID);
            ids[tripID] = tripIDPattern;
            for (const [ seq, seqUpdate ] of Object.entries(tripUpdates)) {
                updates.push({
                    trip_id: tripIDPattern,
                    seq: seq,
                    arrival: seqUpdate.arrival,
                    departure: seqUpdate.departure,
                    last_updated: data.timestamp
                });
            }
        }

        return db.result(deleteQuery).then((deleteResult) => {
            if (updates.length > 0) {
                const updateQuery = update(updates, cs)
                    + ' WHERE t.trip_id LIKE v.trip_id AND t.seq = v.seq'
                    + ' RETURNING t.trip_id';
                return db.any(updateQuery).then((rows) => {
                    for (const row of rows) {
                        delete ids[row.trip_id];
                    }

                    // let rowCount = rows.length;
                    if (Object.keys(ids).length == 0) return {
                        timestamp: data.timestamp,
                        deletedCount: deleteResult.rowCount,
                        rowCount: rows.length
                    }; // nothing else to do

                    console.log(`${Object.keys(ids).length} remaining IDs`);
                    const queryIDs = {};
                    for (const id of Object.keys(ids)) {
                        const line = id.match(/[A-Z]{3}/)[0];
                        queryIDs[id] = {
                            trip_id: id,
                            line: line,
                            time: data.tripStart[id],
                            id_pattern: '%' + id.slice(id.length - 3)
                        };
                    }
                    return db.any(`
                        WITH input(trip_id, line, time, id_pattern) AS (VALUES ${values(Object.values(queryIDs), csRefQuery)})
                        SELECT input.line, input.trip_id, t.rtype, t.ref FROM input
                        JOIN ptvapi.departures t ON t.time = input.time AND t.line = input.line AND true = ANY (ARRAY(SELECT unnest(t.ref) ILIKE input.id_pattern))
                    `).then(async (rows) => {
                        const prefixes = {}; // mapping from GTFS ID to PTV API trip ID prefix

                        console.log(`Pass 1 (run ref match): resolved ${rows.length} IDs to PTV refs`);
                        for (const row of rows) {
                            let ref;
                            if (row.ref.length == 1) ref = row.ref[0]; // only one ref to choose from
                            else {
                                for (const x of row.ref) {
                                    ref = x;
                                    if (ref.slice(ref.length - 3) == row.trip_id.slice(row.trip_id.length - 3)) break; // best match (last 3 digits in run ID match GTFS)
                                }
                            }

                            const pattern = await ptvapi.fetchPattern(row.rtype, ref, row.line); // NOTE: data from PTV API may have dubious timestamp!
                            if (pattern == null) {
                                console.error(`Run ${ref} does not correlate to GTFS ID ${row.trip_id}`);
                                continue;
                            }

                            if (ref.slice(ref.length - 3) == row.trip_id.slice(row.trip_id.length - 3)) {
                                /* log prefix into lookup table */
                                const gtfsPrefix = row.trip_id.slice(0, row.trip_id.length - 3);
                                const refPrefix = ref.slice(0, ref.length - 3);
                                if (!prefixes.hasOwnProperty(gtfsPrefix)) prefixes[gtfsPrefix] = [];
                                if (!prefixes[gtfsPrefix].includes(refPrefix)) prefixes[gtfsPrefix].push(refPrefix);
                            }

                            console.log(`Run ${ref} (${row.trip_id}) has ${pattern.length} stops`);

                            delete queryIDs[row.trip_id];
                            
                            if (pattern.length == 0) continue;

                            for (const item of pattern) {
                                item.trip_id = row.trip_id;
                                item.line = row.line;
                            }

                            await db.none(`
                                    INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                                    SELECT src.trip_id, src.line, src.seq, src.arrival, src.departure, sn.code AS station
                                    FROM (VALUES ${values(pattern, csTripInsert)}) AS src(trip_id, line, seq, arrival, departure, station_name)
                                    JOIN gtfs.stop_names AS sn ON src.station_name = sn.name
                                    ON CONFLICT DO NOTHING
                                `);

                            // console.log(pattern);

                            // rowCount++;
                        }

                        let p2Count = 0; // count for pass 2
                        for (const id of Object.keys(queryIDs)) {
                            const gtfsPrefix = id.slice(0, id.length - 3), suffix = id.slice(id.length - 3);
                            if (!prefixes.hasOwnProperty(gtfsPrefix)) continue;

                            const departureTime = queryIDs[id].time;
                            const line = id.match(/[A-Z]{3}/)[0];
                            for (const refPrefix of prefixes[gtfsPrefix]) {
                                const ref = refPrefix.concat(suffix);
                                const pattern = await ptvapi.fetchPattern(0, ref, line);
                                if (pattern == null) {
                                    console.error(`Run ${ref} does not correlate to GTFS ID ${id}`);
                                    continue;
                                }

                                console.log(`Run ${ref} (${id}) has ${pattern.length} stops`);

                                delete queryIDs[id];
                                
                                if (pattern.length == 0) continue;

                                for (const item of pattern) {
                                    item.trip_id = id;
                                    item.line = line;
                                }

                                await db.none(`
                                        INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                                        SELECT src.trip_id, src.line, src.seq, src.arrival, src.departure, sn.code AS station
                                        FROM (VALUES ${values(pattern, csTripInsert)}) AS src(trip_id, line, seq, arrival, departure, station_name)
                                        JOIN gtfs.stop_names AS sn ON src.station_name = sn.name
                                        ON CONFLICT DO NOTHING
                                    `);

                                p2Count++;
                                break;
                            }
                        }
                        console.log(`Pass 2 (ID translation): successfully resolved ${p2Count} IDs to PTV refs`);
                    })
                    .then(() => {
                        if (Object.values(queryIDs).length == 0) return; // done
                        return db.any(`
                            WITH input(trip_id, line, time, id_pattern) AS (VALUES ${values(Object.values(queryIDs), csRefQuery)})
                            SELECT input.line, input.trip_id, t.rtype, t.ref FROM input
                            JOIN ptvapi.departures t ON t.time = input.time AND t.line = input.line
                        `).then(async (rows) => {
                            console.log(`Pass 3 (non-matching): resolved ${rows.length} IDs to PTV refs`);
                            for (const row of rows) {
    
                                const ref = row.ref[0]; // TODO: what about multiple refs?
                                const pattern = await ptvapi.fetchPattern(row.rtype, ref, row.line);
                                if (pattern == null) {
                                    console.error(`Run ${ref} does not correlate to GTFS ID ${row.trip_id}`);
                                    continue;
                                }
    
                                delete queryIDs[row.trip_id];
    
                                console.log(`Run ${ref} (${row.trip_id}) has ${pattern.length} stops`);
                                
                                for (const item of pattern) {
                                    item.trip_id = row.trip_id;
                                    item.line = row.line;
                                }
    
                                if (pattern.length == 0) continue;
                                
                                await db.none(`
                                        INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                                        SELECT src.trip_id, src.line, src.seq, src.arrival, src.departure, sn.code AS station
                                        FROM (VALUES ${values(pattern, csTripInsert)}) AS src(trip_id, line, seq, arrival, departure, station_name)
                                        JOIN gtfs.stop_names AS sn ON src.station_name = sn.name 
                                        ON CONFLICT DO NOTHING
                                    `);
    
                                // console.log(pattern);
    
                                // rowCount++;
                            }
                        });
                    })
                    .then(() => {
                        if (Object.values(queryIDs).length > 0) console.warn(`Unresolvable trip IDs: ${Object.keys(queryIDs)}`);
                        return db.any(updateQuery).then((rows) => {
                            return {
                                timestamp: data.timestamp,
                                deletedCount: deleteResult.rowCount,
                                rowCount: rows.length
                            }; 
                        }); // get the latest updates in, besides the stuff from PTV API
                    });
                });
            } else {
                return {
                    timestamp: data.timestamp,
                    deletedCount: deleteResult.rowCount,
                    rowCount: 0
                };
            }
        });
    });
};

module.exports = { updateTimetable };

if (require.main === module) {
    updateTimetable().then((result) => {
        console.log(JSON.stringify(result, null, 2));
    });
}

