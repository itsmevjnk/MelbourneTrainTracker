const gtfs = require('./gtfs_rt');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, update, values } = pgp.helpers;
const fs = require('fs');

/* ColumnSet for updating - globally declared for caching */
const csUpdate = new ColumnSet(
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

const getTripIDPattern = (id) => {
    const idParts = id.split('-');
    return `${idParts[0]}-${idParts[1]}-%-${idParts[4]}-${idParts[5]}`;
};

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
        for (const [ tripID, tripUpdates ] of Object.entries(data.updates)) {
            const line = (tripID.startsWith('vic:')) ? tripID.split(':')[1].slice(2) : tripID.split('-')[1]; // 02-SUY or vic:02SUY
            for (const [ seq, seqUpdate ] of Object.entries(tripUpdates)) {
                updates.push({
                    trip_id: tripID,
                    line: line,
                    seq: seq,
                    arrival: seqUpdate.arrival,
                    departure: seqUpdate.departure,
                    last_updated: data.timestamp,
                    stop_id: isNaN(seqUpdate.stop) ? null : seqUpdate.stop,
                    station: isNaN(seqUpdate.stop) ? seqUpdate.stop : null
                });
            }
        }

        return db.result(deleteQuery).then((deleteResult) => {
            if (Object.keys(updates).length > 0) {
                const updateQuery = `
                    WITH
                        new_data AS (SELECT * FROM (VALUES ${values(updates, csUpdate)}) AS t(trip_id, line, seq, arrival, departure, stop_id, station)),
                        rsv_data AS (SELECT nd.trip_id, nd.line, nd.seq, nd.arrival, nd.departure, COALESCE(nd.station, s.station) AS station FROM new_data nd LEFT JOIN gtfs.stops s ON nd.stop_id = s.id)
                    INSERT INTO daily.timetable (trip_id, line, seq, arrival, departure, station)
                        SELECT trip_id, line, seq, arrival, departure, station FROM rsv_data
                    ON CONFLICT (trip_id, seq) DO
                        UPDATE SET line=EXCLUDED.line, arrival=EXCLUDED.arrival, departure=EXCLUDED.departure, station=EXCLUDED.station, last_updated=CURRENT_TIMESTAMP
                    RETURNING trip_id, seq
                `;
                return db.any(updateQuery).then((rows) => {
                    return {
                        timestamp: data.timestamp,
                        deletedCount: deleteResult.rowCount,
                        rowCount: rows.length
                    };
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

