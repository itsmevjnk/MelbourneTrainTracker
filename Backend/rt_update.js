const gtfs = require('./gtfs_rt');
const mtm = require('./mtm_api');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, update, values } = pgp.helpers;
const fs = require('fs');

/* ColumnSet for updating - globally declared for caching */
const csGTFSUpdate = new ColumnSet(
    [
        'trip_id',
        'line',
        'seq:value',
        {
            name: 'arrival',
            cast: 'timestamp with time zone'
        },
        {
            name: 'departure',
            cast: 'timestamp with time zone'
        },
        'stop_id',
        'station'
    ]
);
const csMTMUpdate = new ColumnSet(
    [
        'trip_id_pattern',
        'trip_id',
        'line',
        'seq:value',
        {
            name: 'stamp',
            cast: 'timestamp with time zone'
        },
        'stn_name'
    ]
);

const getTripIDPattern = (id) => {
    const idParts = id.split('-');
    return `${idParts[0]}-${idParts[1]}-%-${idParts[4]}-${idParts[5]}`;
};

/* lines whose timetable should be retrieved from Metro Trains */
// const mtmLines = Object.values(mtm.lineIDs); // get operational timetable for all lines
const mtmLines = [mtm.lineIDs.STY];

const updateTimetable = (lastTimestamp = null) => {
    return Promise.all([
        gtfs.fetchAllUpdates(),
        mtm.getReplacementBuses(),
        mtm.getOperationalTimetable(mtmLines)
    ]).then((results) => {
        const gtfsData = results[0];
        const rrbData = results[1];
        const mtmData = results[2];

        // if (lastTimestamp != null && lastTimestamp.getTime() == gtfsData.timestamp.getTime()) return null; // indicate that the data has not been updated

        const cancelPatterns = [];
        for (const id of gtfsData.cancellations) {
            cancelPatterns.push(getTripIDPattern(pgp.as.value(id)));
        }
        const deleteQuery = `DELETE FROM daily.timetable WHERE trip_id SIMILAR TO '${cancelPatterns.join('|')}'`;
        // console.log(deleteQuery);

        const gtfsUpdates = [];
        for (const [ tripID, tripUpdates ] of Object.entries(gtfsData.updates)) {
            const line = (tripID.startsWith('vic:')) ? tripID.split(':')[1].slice(2) : tripID.split('-')[1]; // 02-SUY or vic:02SUY
            for (const [ seq, seqUpdate ] of Object.entries(tripUpdates)) {
                gtfsUpdates.push({
                    trip_id: tripID,
                    line: line,
                    seq: seq,
                    arrival: seqUpdate.arrival,
                    departure: seqUpdate.departure,
                    last_updated: gtfsData.timestamp,
                    stop_id: isNaN(seqUpdate.stop) ? null : seqUpdate.stop,
                    station: isNaN(seqUpdate.stop) ? seqUpdate.stop : null
                });
            }
        }
        const now = new Date();
        for (const update of rrbData) {
            gtfsUpdates.push({
                trip_id: update.tripID,
                line: update.line,
                seq: update.seq,
                arrival: update.arrival,
                departure: (update.departure) ? update.departure : update.arrival,
                last_updated: now,
                stop_id: null,
                station: update.station
            });
        }

        const mtmUpdates = [];
        const ptDate = mtm.getPTDate().replaceAll('-', '');
        for (const [ line, entries ] of Object.entries(mtmData)) {
            for (const entry of entries) {
                mtmUpdates.push({
                    'trip_id_pattern': `02-${line}-%-${entry.trip_id}`,
                    'trip_id': `02-${line}-${ptDate}-${entry.trip_id}`,
                    'line': line,
                    'seq': entry.seq,
                    'stamp': entry.time,
                    'stn_name': entry.station
                })
            }
        }

        return db.result(deleteQuery).then(() => {
            if (gtfsUpdates.length > 0) {
                const gtfsQuery = `
                    WITH
                        new_data AS (SELECT * FROM (VALUES ${values(gtfsUpdates, csGTFSUpdate)}) AS t(trip_id, line, seq, arrival, departure, stop_id, station)),
                        rsv_data AS (SELECT nd.trip_id, nd.line, nd.seq, nd.arrival, nd.departure, COALESCE(nd.station, s.station) AS station FROM new_data nd LEFT JOIN gtfs.stops s ON nd.stop_id = s.id)
                    INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                        SELECT trip_id, line, seq, arrival, departure, station FROM rsv_data
                    ON CONFLICT (trip_id, seq) DO
                        UPDATE SET line=EXCLUDED.line, arrival=EXCLUDED.arrival, departure=EXCLUDED.departure, station=EXCLUDED.station, last_updated=CURRENT_TIMESTAMP
                `;
                return db.none(gtfsQuery);
            }
        }).then(() => {
            if (mtmUpdates.length > 0) {
                const mtmQuery = `
                    WITH
                        updates_in (trip_id_pattern, trip_id, line, seq, stamp, stn_name) AS (VALUES ${values(mtmUpdates, csMTMUpdate)}),
                        updated AS (
                            UPDATE daily.timetable t
                            SET
                                arrival = ui.stamp, departure = ui.stamp,
                                station = s.station
                            FROM updates_in ui
                            JOIN gtfs.stop_names s ON s.name = ui.stn_name
                            WHERE t.trip_id LIKE ui.trip_id_pattern AND t.seq = ui.seq
                            RETURNING ui.trip_id_pattern, ui.seq
                        )
                    INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                    SELECT ui.trip_id, ui.line, ui.seq, ui.stamp, ui.stamp, s.station
                    FROM updates_in ui
                    JOIN gtfs.stop_names s ON s.name = ui.stn_name
                    WHERE NOT EXISTS (
                        SELECT 1 FROM updated u WHERE u.trip_id_pattern = ui.trip_id_pattern AND u.seq = ui.seq
                    )
                `;
                // console.log(mtmQuery);
                return db.none(mtmQuery);
            }
        }).then(() => {
            return { timestamp: gtfsData.timestamp }; // for compatibility
        });
    });
};

module.exports = { updateTimetable };

if (require.main === module) {
    updateTimetable().then((result) => {
        console.log(JSON.stringify(result, null, 2));
    });
}

