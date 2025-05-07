const gtfs = require('./gtfs_rt');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, update, values } = pgp.helpers;

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
            const tripIDPattern = getTripIDPattern(tripID);
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
                    + ' WHERE t.trip_id LIKE v.trip_id AND t.seq = v.seq AND (t.arrival != v.arrival OR t.departure != v.departure)'
                    + ' RETURNING t.trip_id, t.seq, v.arrival, v.departure';
                return db.any(updateQuery).then((rows) => { // only update if there are rows available
                    const updatedRows = {};
                    for (const row of rows) {
                        if (!updatedRows.hasOwnProperty(row.trip_id)) updatedRows[row.trip_id] = {};
                        updatedRows[row.trip_id][row.seq] = {
                            arrival: row.arrival,
                            departure: row.departure
                        };
                    }
                    return {
                        timestamp: data.timestamp,
                        deletedCount: deleteResult.rowCount,
                        rows: updatedRows
                    };
                });
            } else {
                return {
                    timestamp: data.timestamp,
                    deletedCount: deleteResult.rowCount,
                    rows: {}
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

