const gtfs = require('./gtfs_rt');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, update } = pgp.helpers;

/* ColumnSet for updating - globally declared for caching */
const cs = new ColumnSet(
    [
        '?trip_id',
        '?line',
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

const updateTimetable = (lastTimestamp = null) => {
    return gtfs.fetchUpdate()
    .then((data) => {
        if (lastTimestamp != null && lastTimestamp.getTime() == data.timestamp.getTime()) return null; // indicate that the data has not been updated

        const updates = [];
        for (const [ tripID, tripUpdates ] of Object.entries(data.updates)) {
            const idParts = tripID.split('-');
            const tripIDPattern = '%' + idParts[4] + '-' + idParts[5];
            const line = idParts[1];
            for (const [ seq, seqUpdate ] of Object.entries(tripUpdates)) {
                updates.push({
                    trip_id: tripIDPattern,
                    line: line,
                    seq: seq,
                    arrival: seqUpdate.arrival,
                    departure: seqUpdate.departure,
                    last_updated: data.timestamp
                });
            }
        }

        const query = update(updates, cs)
            + ' WHERE t.line = v.line AND t.trip_id LIKE v.trip_id AND t.seq = v.seq AND (t.arrival != v.arrival OR t.departure != v.departure)'
            + ' RETURNING t.trip_id, t.seq, v.arrival, v.departure';
        return db.any(query).then((rows) => {
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
                rows: updatedRows
            };
        });
    });
};

module.exports = { updateTimetable };

if (require.main === module) {
    updateTimetable().then((result) => {
        console.log(JSON.stringify(result, null, 2));
    });
}

