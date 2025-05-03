/* daily database initialisation */

const { pgp, db } = require('./database');
const { TableName, ColumnSet, insert, select } = pgp.helpers;

const daily_init = () => {
    const query = `
        INSERT INTO daily.timetable (trip_id, line, seq, station, arrival, departure, next_station)
        SELECT
            A.trip_id AS trip_id,
            (REGEXP_MATCHES(A.trip_id, '[A-Z]{3}'))[1] AS line,
            A.seq AS seq,
            B.station AS station,
            CURRENT_DATE + A.arrival AS arrival,
            CURRENT_DATE + A.departure AS departure,
            B2.station AS next_station
        FROM gtfs.timetable A
        INNER JOIN gtfs.stops B
            ON A.stop_id = B.id
        LEFT JOIN gtfs.timetable A2
            ON A.trip_id = A2.trip_id AND A.seq + 1 = A2.seq
        LEFT JOIN gtfs.stops B2
            ON A2.stop_id = B2.id
        WHERE A.trip_id IN (
            SELECT id
            FROM (
                SELECT id, (REGEXP_MATCHES(id, '[A-Z]{3}'))[1] AS line
                FROM gtfs.trips
                WHERE calendar IN (
                    SELECT id FROM gtfs.calendar
                    WHERE
                        CASE TRIM(TO_CHAR(CURRENT_DATE, 'Day'))
                            WHEN 'Sunday' THEN sunday
                            WHEN 'Monday' THEN monday
                            WHEN 'Tuesday' THEN tuesday
                            WHEN 'Wednesday' THEN wednesday
                            WHEN 'Thursday' THEN thursday
                            WHEN 'Friday' THEN friday
                            WHEN 'Saturday' THEN saturday
                        END = TRUE
                        AND start_date <= CURRENT_DATE
                        AND end_date >= CURRENT_DATE
                )
            ) filtered_trips
            WHERE line IN (
                'ALM', 'BEG', 'CBE', 'CCL', 'CGB', 'FKN', 'GWY', 'HBE', 'LIL', 'MDD', 'PKM', 'RCE', 'SHM', 'STY', 'SUY', 'UFD', 'WER', 'WIL',
                'ART', 'BAT', 'BDE', 'GEL', 'MBY', 'TRN', 'vPK', 'WBL'
            )
        )
    `;
    return db.none('TRUNCATE TABLE daily.timetable CASCADE')
        .then(() => db.none(query))
        .then(() => console.log('Daily timetable has been updated'));
};

module.exports = { daily_init };

if (require.main === module) {
    daily_init();
}

