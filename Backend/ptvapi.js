/* PTV Timetable API */

const crypto = require('crypto');
const secret = require('./secret');
const schedule = require('node-schedule');

const PTV_API_URL = process.env.PTV_API_URL || 'https://timetableapi.ptv.vic.gov.au';
const PTV_API_ID = secret.read(process.env.PTV_ID_FILE) || process.env.PTV_API_ID;
const PTV_API_KEY = secret.read(process.env.PTV_KEY_FILE) || process.env.PTV_API_KEY;

const generateURL = (query) => {
    const url = new URL(query, PTV_API_URL);
    url.searchParams.set('devid', PTV_API_ID);
    url.searchParams.set('signature', crypto.createHmac('sha1', PTV_API_KEY).update(url.pathname + url.search).digest('hex').toUpperCase());
    return url.href;
};

/* list of lines considered by discoverDepartures */
const LINES = [
    'ALM', 'BEG', 'CBE', 'CCL', 'CGB', 'FKN', 'GWY', 'HBE', 'LIL', 'MDD', 'PKM', 'RCE', 'SHM', 'STY', 'SUY', 'UFD', 'WER', 'WIL',
    'ART', 'BAT', 'BDE', 'GEL', 'MBY', 'TRN', 'vPK', 'WBL'
];

/* list of V/Line stations considered by discoverDepartures */
const VLINE_STATIONS = [
    'Melton',
    'Cobblebank',
    'Rockbank',
    'Caroline Springs',
    'Wyndham Vale',
    'West Tarneit',
    'Tarneit',
    'Deer Park',
    'Ardeer',
    'Sunshine',
    'Footscray',
    'Southern Cross',
    'Flinders Street',
    'Richmond',
    'Caulfield',
    'Clayton',
    'Dandenong',
    'Berwick',
    'Pakenham'
];

/* gather all services and their departures from termini */
const discoverDepartures = async (routeType = 0) => { // 0 for metropolitan trains, 3 for V/Line
    const stations = [1071, 1181]; // list of stations to query (we'll start out with FSS and SSS)
    const queriedStations = []; // list of stations that have been queried
    const ret = {
        stations: {}, // stop ID to station name (for translating to GTFS)
        runs: [], // discovered runs
    };
    const routes = {}; // translation from route ID to line code
    while (stations.length > 0) {
        const stationID = stations.pop();
        console.log(`Querying station ID ${stationID}.`);
        const result = JSON.parse(
            await fetch(generateURL(`/v3/departures/route_type/${routeType}/stop/${stationID}?expand=All`)).then((resp) => resp.text())
        );
        queriedStations.push(stationID);

        /* save stations */
        let proceed = true;
        for (const stop of Object.values(result.stops)) {
            const name = stop.stop_name.replace('Railway Station', '').trim();
            if (routeType == 3 && !VLINE_STATIONS.includes(name)) {
                proceed = false;
                break;
            }
            ret.stations[stop.stop_id] = name;
        }
        
        if (!proceed) continue; // do not proceed with this station, since it is not in the allowed list (V/Line only)

        /* save route IDs */
        for (const route of Object.values(result.routes)) {
            routes[route.route_id] = route.route_gtfs_id.split('-')[1]; // e.g. 2-BEG
        }

        /* save departures */
        for (const departure of result.departures) {
            const runInfo = result.runs[departure.run_ref];
            const terminusID = runInfo.final_stop_id;

            const line = routes[departure.route_id];
            if (!LINES.includes(line)) continue;

            ret.runs.push({
                rtype: routeType,
                ref: departure.run_ref,
                line: line,
                stop: departure.stop_id, // should be the same as the station we're querying
                time: new Date(departure.scheduled_departure_utc),
                // terminus: terminusID // not sure if this is needed
            });
            if (!queriedStations.includes(terminusID) && !stations.includes(terminusID)) stations.push(terminusID);
            if (runInfo.interchange) {
                // add info gathered from interchange runs for maximum coverage
                if (runInfo.interchange.feeder && !queriedStations.includes(runInfo.interchange.feeder.stop_id) && !stations.includes(runInfo.interchange.feeder.stop_id))
                    stations.push(runInfo.interchange.feeder.stop_id);
                if (runInfo.interchange.distributor && !queriedStations.includes(runInfo.interchange.distributor.stop_id) && !stations.includes(runInfo.interchange.distributor.stop_id))
                    stations.push(runInfo.interchange.distributor.stop_id);
            }
        }
    }

    return ret;
};

const saveDepartures = () => {
    const { pgp, db } = require('./database');
    const { TableName, ColumnSet, insert, select } = pgp.helpers;
    return Promise.all([
        discoverDepartures(0),
        discoverDepartures(3)
    ]).then((results) => {
        const stations = { ...results[0].stations, ...results[1].stations };
        const stationRows = [];
        for (const [id, name] of Object.entries(stations)) {
            stationRows.push({
                id: id,
                name: name
            });
        }

        const runs = results[0].runs.concat(results[1].runs);

        // const duplicates = runs.reduce((acc, run) => {
        //     const key = `${run.line}-${run.stop}-${run.time.toISOString()}`;
        //     if (!acc[key]) {
        //     acc[key] = [];
        //     }
        //     acc[key].push(run);
        //     return acc;
        // }, {});

        // const duplicateRuns = Object.values(duplicates).filter(group => group.length > 1);

        // if (duplicateRuns.length > 0) {
        //     console.log('Duplicate runs found:');
        //     duplicateRuns.forEach(group => {
        //     console.log(group);
        //     });
        // } else {
        //     console.log('No duplicate runs found.');
        // }

        return db.none('TRUNCATE TABLE ptvapi.stops CASCADE')
            .then(() => db.none(insert(stationRows, new ColumnSet(
                ['id', 'name'],
                { 
                    table: new TableName({
                        schema: 'ptvapi',
                        table: 'stops'
                    }) 
                }
            ))))
            .then(() => db.none('TRUNCATE TABLE ptvapi.departures CASCADE'))
            .then(() => db.none(insert(runs, new ColumnSet(
                ['rtype', 'ref', 'line', 'stop', 'time'],
                { 
                    table: new TableName({
                        schema: 'ptvapi',
                        table: 'departures'
                    }) 
                }
            )) + ' ON CONFLICT DO NOTHING'));        
    });
};

module.exports = { generateURL, discoverDepartures, saveDepartures };

const HEALTHCHECK_PORT = process.env.HEALTHCHECK_PORT || 3000;

if (require.main === module) {
    saveDepartures().then(() => {
        /* schedule nightly departures update at 12:15am */
        schedule.scheduleJob('15 0 * * *', async () => {
            console.log('>>> Updating departures from PTV API.');
            await saveDepartures();
        });

        /* start healthcheck server */
        const healthCheck = require('./healthcheck');
        healthCheck.start(HEALTHCHECK_PORT);
    });
}
