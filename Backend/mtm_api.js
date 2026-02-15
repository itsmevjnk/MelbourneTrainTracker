/* Metro Trains Melbourne AWS API */

const { pgp, db } = require('./database');

const METRO_AWS_API_URL = process.env.METRO_AWS_API_URL || 'https://747813379903-static-assets-production.s3-ap-southeast-2.amazonaws.com';

const generateURL = (path) => {
    return `${METRO_AWS_API_URL}/${path}?_=${Date.now()}`; // cache busting
};

const getTripID = (routeID, tripID) => {
    return `R-${routeID}-${tripID}`;
};

/* internal cache */
let routeLines = {}; // route ID to line mapping
let cacheDate = ''; // cache date (in PTV date, i.e. new day starts at 3am)

const getPTDate = () => {
    const formatter = new Intl.DateTimeFormat('en-CA', {
        timeZone: 'Australia/Melbourne',
        year: 'numeric',
        month: '2-digit',
        day: '2-digit'
    });
    return formatter.format(new Date(Date.now() - 3 * 60 * 60 * 1000));
};

const getReplacementBuses = () => {
    return Promise.all([
        fetch(generateURL('bus/index.json')), // contains stations and stopping patterns
        fetch(generateURL('bus/rt-updates.json')) // contains arrival/departure times at stations
    ]).then((responses) => {
        const promises = [];
        for (const response of responses) promises.push(response.json());
        return Promise.all(promises);
    }).then((results) => {
        const index = results[0];
        const updates = results[1];

        const currentDate = getPTDate();
        if (cacheDate != currentDate) { // reset cache
            routeLines = {};
            cacheDate = currentDate;
        }
        
        /* parse index.json */
        const stops = {};
        for (const stationList of Object.values(index.stations)) {
            for (const stop of stationList) {
                stops[stop.stop_id] = stop.station_code;
            }
        }
        const patterns = {};
        for (const pattern of index.stopping_patterns) {
            patterns[pattern.id] = pattern.stops;
        }

        /* parse rt-updates.json */
        const resUpdates = [];
        const routeIDs = {}; // list of unresolved route IDs encountered, as well as all stations within
        const routeTrips = {};
        for (const update of updates) {
            const routeID = update.route_id;
            const station = stops[update.stop_id];

            if (!routeTrips.hasOwnProperty(routeID)) routeTrips[routeID] = new Set();
            routeTrips[routeID].add(update.trip_id);

            const entry = {
                tripID: getTripID(update.route_id, update.trip_id),
                rawTripID: update.trip_id,
                routeID: routeID,
                station: station
            };
            
            let arrivalTime = (update.generation_date + update.time_until_arrival) * 1000;
            entry.arrival = new Date(arrivalTime);
            if (update.time_until_departure !== null) {
                let departureTime = (update.generation_date + update.time_until_departure) * 1000;
                if (departureTime < arrivalTime) departureTime = arrivalTime;
                entry.departure = new Date(departureTime);
            }

            if (!routeLines.hasOwnProperty(routeID)) { // stage route ID for resolution
                if (!routeIDs.hasOwnProperty(routeID)) routeIDs[routeID] = new Set();
                routeIDs[routeID].add(station);
            } else entry.line = routeLines[routeID];

            resUpdates.push(entry);
        }

        /* resolve stopping pattern and sequence number */
        const stationRoutes = {};
        for (const [route, stations] of Object.entries(routeIDs)) {
            for (const station of stations) {
                if (!stationRoutes.hasOwnProperty(station)) stationRoutes[station] = new Set();
                stationRoutes[station].add(route);
            }
        }
        const uncoveredRoutes = new Set(Object.keys(routeIDs));
        const queryStations = [];
        while (uncoveredRoutes.size > 0) {
            let bestStation = null;
            let bestCoverage = 0;
            for (const [station, routes] of Object.entries(stationRoutes)) {
                const coverage = [...routes].filter(k => uncoveredRoutes.has(k)).length;
                if (coverage > bestCoverage) {
                    bestCoverage = coverage;
                    bestStation = station;
                }
            }
            if (!bestStation) break;

            queryStations.push(bestStation);

            for (const route of stationRoutes[bestStation]) {
                uncoveredRoutes.delete(route);
            }
        }
        
        const fetchPromises = [];
        for (const station of queryStations) fetchPromises.push(fetch(generateURL(`bus/bus-${station}.json`)));
        return Promise.all(fetchPromises).then((responses) => {
            const promises = [];
            for (const response of responses) promises.push(response.json());
            return Promise.all(promises);
        }).then((results) => {
            const tripPatterns = {};
            for (const result of results) {
                for (const entry of result) {
                    const stopPattern = patterns[entry.stopping_pattern];
                    for (const trip of entry.trips) {
                        if (!tripPatterns.hasOwnProperty(trip) || tripPatterns[trip].length < stopPattern.length) tripPatterns[trip] = stopPattern;
                    }
                }
            }

            for (const update of resUpdates) {
                const stopPattern = tripPatterns[update.rawTripID];
                update.seq = stopPattern.indexOf(update.station);
                // if (update.seq == -1) {
                //     console.warn(stopPattern, update.station);
                // }
                if (routeIDs.hasOwnProperty(update.routeID))
                    stopPattern.forEach(station => routeIDs[update.routeID].add(station)); // populate with all stations that can exist on the route
            }

            /* resolve route IDs */
            const resolveIDs = [];
            const dbPromises = [];
            for (const [id, stations] of Object.entries(routeIDs)) {
                resolveIDs.push(id);
                const stationsArray = [...stations];
                dbPromises.push(
                    db.any(
                        `
                            WITH unique_stations AS (
                                SELECT station FROM gtfs.station_lines
                                GROUP BY station
                                HAVING COUNT(DISTINCT line) = 1
                            ),
                            primary_result AS (
                                SELECT sl.line, 1 AS priority
                                FROM gtfs.station_lines sl
                                JOIN unique_stations us ON sl.station = us.station
                                WHERE us.station = ANY($1)
                                GROUP BY sl.line
                                ORDER BY COUNT(*) DESC
                                LIMIT 1
                            ),
                            fallback_result AS (
                                SELECT line, 2 AS priority
                                FROM gtfs.station_lines
                                WHERE station = ANY($1)
                                GROUP BY line
                                HAVING COUNT(DISTINCT station) = $2
                                LIMIT 1
                            )
                            SELECT line
                            FROM (
                                SELECT * FROM primary_result
                                UNION ALL
                                SELECT * FROM fallback_result
                            ) combined_result
                            ORDER BY priority
                            LIMIT 1
                        `,
                        [stationsArray, stationsArray.length]
                    )
                );
            }

            return Promise.all(dbPromises).then((queryResults) => {
                for (let i = 0; i < resolveIDs.length; i++) {
                    let id = resolveIDs[i];
                    // console.log(id, routeIDs[id], queryResults[i]);
                    if (queryResults[i].length == 1) { // definitive answer found
                        delete routeIDs[id];
                        routeLines[id] = queryResults[i][0].line;
                    }
                }

                /* clean up */
                const ret = [];
                for (const update of resUpdates) {
                    if (update.hasOwnProperty('line')) ret.push(update); // pre-resolved
                    else if (routeLines.hasOwnProperty(update.routeID)) { // just resolved now
                        update.line = routeLines[update.routeID];
                        ret.push(update);
                    } else console.warn(`Unable to resolve route ID ${update.routeID} for trip ${update.tripID}`);
                }
                return ret;
            });
        });
    });
};

module.exports = { getReplacementBuses };

if (require.main === module) {
    getReplacementBuses().then((result) => {
        console.log(JSON.stringify(result, null, 2));
    });
}