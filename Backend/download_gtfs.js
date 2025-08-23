const temp = require('temp');
const fs = require('fs');
const stream = require('stream');
const progress = require('progress-stream');
const yauzl = require('yauzl');
const csv = require('csv-parse');
const schedule = require('node-schedule');

temp.track(); // automatically remove file on completion

const GTFS_DATASET_URL = process.env.GTFS_DATASET_URL || 'https://opendata.transport.vic.gov.au/dataset/3f4e292e-7f8a-4ffe-831f-1953be0fe448/resource/e4966d78-dc64-4a1d-a751-2470c9eaf034/download/gtfs.zip';
// const GTFS_DATASET_URL = 'http://192.168.42.69/gtfs.zip';

const createProgressStream = (length) => {
    const progressStream = progress({
        length: length,
        time: 1000
    });
    const lengthMB = length / 1048576;
    const lengthStr = lengthMB.toFixed(1);
    progressStream.on('progress', (progress) => {
        const transferMB = progress.transferred / 1048576;
        const speedMB = progress.speed / 1048576;
        console.log(`${progress.percentage.toFixed(0).padStart(3)}% | ${transferMB.toFixed(1).padStart(lengthStr.length)}/${lengthStr} MiB transferred (${speedMB.toFixed(1)} MiB/s)`);
    });

    return progressStream;
};

const convertDate = (dateString) => { // convert YYYYMMDD date string to Date object
    return new Date(dateString.slice(0,4), dateString.slice(4,6) - 1, dateString.slice(6));
};

const parseCalendar = (stream) => {
    return new Promise((resolve, reject) => {
        const parser = csv.parse({ delimiter: ',', columns: true, bom: true });
        parser.on('error', (err) => {
            reject(err);
        });
        const entries = {};
        parser.on('readable', () => {
            let record;
            while ((record = parser.read()) !== null) {
                entries[record.service_id] = {
                    startDate: convertDate(record.start_date),
                    endDate: convertDate(record.end_date),
                    monday: record.monday == '1',
                    tuesday: record.tuesday == '1',
                    wednesday: record.wednesday == '1',
                    thursday: record.thursday == '1',
                    friday: record.friday == '1',
                    saturday: record.saturday == '1',
                    sunday: record.sunday == '1',
                };
            }
        });
        parser.on('end', () => {
            resolve({ calendar: entries });
        });
        stream.pipe(parser);
    });
};

const getStationName = (name) => {
    name = name.split('Railway')[0];
    name = name.replace('Station', '');
    name = name.replace('Rail Replacement Bus Stop', '');
    name = name.trim();
    return name;
}

const resolveStation = (name) => {
    return {
        'Melton': 'MEL',
        'Cobblebank': 'TLN',
        'Rockbank': 'RBK',
        'Caroline Springs': 'CPS',
        'Wyndham Vale': 'WVL',
        'West Tarneit': 'DAV',
        'Tarneit': 'TNT',
        'Deer Park': 'DEK',
        'Ardeer': 'ADR',
        'Sunshine': 'SUN',
        'Footscray': 'FSY',
        'Southern Cross': 'SSS',
        'Flinders Street': 'FSS',
        'Richmond': 'RMD',
        'Caulfield': 'CFD',
        'Clayton': 'CLA',
        'Dandenong': 'DNG',
        'Berwick': 'BEW',
        'Pakenham': 'PKM',
        'Jordanville': 'JOR',
        'Flagstaff': 'FGS',
        'Melbourne Central': 'MCE',
        'Parliament': 'PAR',
        'Belgrave': 'BEG',
        'Tecoma': 'TCM',
        'Upwey': 'UPW',
        'Alamein': 'ALM',
        'Ashburton': 'ASH',
        'Burwood': 'BWD',
        'Hartwell': 'HWL',
        'Willison': 'WSN',
        'Riversdale': 'RIV',
        'Camberwell': 'CAM',
        'Frankston': 'FKN',
        'Kananook': 'KAN',
        'Seaford': 'SEA',
        'Carrum': 'CAR',
        'Bonbeach': 'BON',
        'Chelsea': 'CSA',
        'Edithvale': 'EDI',
        'Aspendale': 'ASP',
        'Mordialloc': 'MOR',
        'Parkdale': 'PKD',
        'Mentone': 'MEN',
        'Cheltenham': 'CTM',
        'Upper Ferntree Gully': 'UFG',
        'Ferntree Gully': 'FTG',
        'Boronia': 'BOR',
        'Bayswater': 'BAY',
        'Heathmont': 'HMT',
        'Highett': 'HIG',
        'Glen Waverley': 'GWY',
        'Syndal': 'SYN',
        'Mount Waverley': 'MWY',
        'Lilydale': 'LIL',
        'Mooroolbark': 'MLK',
        'Croydon': 'CDN',
        'Ringwood East': 'RWE',
        'Officer': 'OFC',
        'Beaconsfield': 'BFD',
        'Narre Warren': 'NWA',
        'Hallam': 'HLM',
        'Cranbourne': 'CBE',
        'Merinda Park': 'MPK',
        'Yarraman': 'YMN',
        'Noble Park': 'NPK',
        'Sandown Park': 'SNP',
        'East Camberwell': 'ECM',
        'Canterbury': 'CBY',
        'Chatham': 'CHM',
        'Box Hill': 'BOX',
        'Laburnum': 'LAB',
        'Blackburn': 'BBN',
        'Nunawading': 'NWG',
        'Mitcham': 'MCH',
        'Heatherdale': 'HTD',
        'Ringwood': 'RWD',
        'Auburn': 'AUB',
        'Glenferrie': 'GFE',
        'Hawthorn': 'HAW',
        'Burnley': 'BLY',
        'East Richmond': 'ERM',
        'Heyington': 'HEY',
        'Kooyong': 'KYG',
        'Tooronga': 'TGA',
        'Gardiner': 'GAR',
        'Springvale': 'SPG',
        'Westall': 'WTL',
        'Huntingdale': 'HUN',
        'Oakleigh': 'OAK',
        'Hughesdale': 'HUG',
        'Murrumbeena': 'MRB',
        'Carnegie': 'CNE',
        'Werribee': 'WER',
        'Hoppers Crossing': 'HCG',
        'Laverton': 'LAV',
        'Aircraft': 'ACF',
        'Westona': 'WTO',
        'Altona': 'ALT',
        'Seaholme': 'SHE',
        'Westgarth': 'WTG',
        'Dennis': 'DEN',
        'Fairfield': 'FFD',
        'Alphington': 'ALP',
        'Darebin': 'DBN',
        'Ivanhoe': 'IVA',
        'Eaglemont': 'EAG',
        'Heidelberg': 'HDB',
        'Rosanna': 'ROS',
        'Moorabbin': 'MRN',
        'Patterson': 'PAT',
        'Bentleigh': 'BEN',
        'McKinnon': 'MCK',
        'Ormond': 'OMD',
        'Glen Huntly': 'GHY',
        'Malvern': 'MAL',
        'Armadale': 'ARM',
        'Toorak': 'TOR',
        'Hawksburn': 'HKN',
        'Sandringham': 'SHM',
        'Hampton': 'HAM',
        'Brighton Beach': 'BBH',
        'Middle Brighton': 'MBN',
        'North Brighton': 'NBN',
        'Gardenvale': 'GVE',
        'Elsternwick': 'ELS',
        'Ripponlea': 'RIP',
        'Balaclava': 'BCV',
        'Windsor': 'WIN',
        'Prahran': 'PRA',
        'South Yarra': 'SYR',
        'Upfield': 'UFD',
        'Gowrie': 'GOW',
        'Fawkner': 'FAK',
        'Merlynston': 'MYN',
        'Batman': 'BAT',
        'Coburg': 'COB',
        'Moreland': 'MLD',
        'Anstey': 'ASY',
        'Brunswick': 'BWK',
        'Jewell': 'JWL',
        'Royal Park': 'RPK',
        'Flemington Bridge': 'FBD',
        'Macaulay': 'MAC',
        'North Melbourne': 'NME',
        'Clifton Hill': 'CHL',
        'Victoria Park': 'VPK',
        'Collingwood': 'CWD',
        'North Richmond': 'NRM',
        'West Richmond': 'WRM',
        'Jolimont-MCG': 'JLI',
        'Macleod': 'MCD',
        'Watsonia': 'WAT',
        'Greensborough': 'GRN',
        'Montmorency': 'MMY',
        'Eltham': 'ELT',
        'Diamond Creek': 'DCK',
        'Wattle Glen': 'WTT',
        'Hurstbridge': 'HBE',
        'Williamstown': 'WIL',
        'Williamstown Beach': 'WBH',
        'North Williamstown': 'NWN',
        'Newport': 'NPT',
        'Spotswood': 'SPT',
        'Yarraville': 'YVE',
        'Seddon': 'SEN',
        'Sunbury': 'SUY',
        'Diggers Rest': 'DRT',
        'Watergardens': 'WGS',
        'Keilor Plains': 'KPL',
        'St Albans': 'SAB',
        'Ginifer': 'GIN',
        'Albion': 'ALB',
        'Epping': 'EPP',
        'Lalor': 'LAL',
        'Thomastown': 'TSN',
        'Keon Park': 'KPK',
        'Ruthven': 'RUT',
        'Reservoir': 'RES',
        'Regent': 'REG',
        'Preston': 'PRE',
        'Bell': 'BEL',
        'Thornbury': 'TBY',
        'Croxton': 'CXT',
        'Northcote': 'NCE',
        'Merri': 'MER',
        'Rushall': 'RUS',
        'Tottenham': 'TOT',
        'West Footscray': 'WFY',
        'Middle Footscray': 'MFY',
        'South Kensington': 'SKN',
        'Flemington Racecourse': 'RCE',
        'Showgrounds': 'SGS',
        'Craigieburn': 'CGB',
        'Broadmeadows': 'BMS',
        'Jacana': 'JAC',
        'Glenroy': 'GRY',
        'Oak Park': 'OKP',
        'Pascoe Vale': 'PVL',
        'Strathmore': 'SME',
        'Glenbervie': 'GBV',
        'Essendon': 'ESD',
        'Moonee Ponds': 'MPD',
        'Ascot Vale': 'ASV',
        'Newmarket': 'NKT',
        'Kensington': 'KEN',
        'Morradoo': 'MRO',
        'Darling': 'DLG',
        'East Malvern': 'EMV',
        'Glen Iris': 'GIR',
        'Holmesglen': 'HOL',
        'Williams Landing': 'WLD',
        'Union': 'UNN',
        'Cardinia Road': 'CDA',
        'Coolaroo': 'CLO',
        'East Pakenham': 'EPH',
        'Hawkstowe': 'HWS',
        'Lynbrook': 'LBK',
        'Mernda': 'MDD',
        'Middle Gorge': 'MMR',
        'South Morang': 'SMG',
        'Southland': 'SOU',
        'Bittern': 'BIT',
        'Hastings': 'HST',
        'Roxburgh Park': 'RXP',
        'Tyabb': 'TAB',
        'Somerville': 'SVE',
        'Baxter': 'BXR',
        'Crib Point': 'CPT',
        'Leawarra': 'LWA',
        'Stony Point': 'STY'
    }[name];
};

const parseStops = (stream) => {
    return new Promise((resolve, reject) => {
        const parser = csv.parse({ delimiter: ',', columns: true, bom: true });
        parser.on('error', (err) => {
            reject(err);
        });
        const entries = {};
        parser.on('readable', () => {
            let record;
            while ((record = parser.read()) !== null) {
                const stopID = record.stop_id * 1;
                if (isNaN(stopID)) continue; // non-numeric stop ID - skip (all stop IDs in stop_times.txt are numeric)
                const stationName = getStationName(record.stop_name);
                let stationCode;
                if (record.parent_station.includes('vic:rail:')) {
                    stationCode = record.parent_station.split(':')[2]; // retrievable from parent_station
                    stationCode = stationCode.slice(0, 3); // cut down to 3 letter
                }
                else {
                    stationCode = resolveStation(stationName);
                    if (!stationCode) continue; // station code not in resolvable list - probably not necessary anyway, skip
                }
                if (!entries.hasOwnProperty(stationCode)) entries[stationCode] = {
                    name: stationName,
                    ids: []
                }; // create array of stop IDs
                entries[stationCode].ids.push(stopID); // convert string to number for more compact storage
            }
        });
        parser.on('end', () => {
            resolve({ stops: entries });
        });
        stream.pipe(parser);
    });
};

const convertTime = (timeString) => { // convert hh:mm:ss time string to time offset from 00:00:00
    const ret = new Date(0);
    ret.setUTCHours(timeString.slice(0,2));
    ret.setUTCMinutes(timeString.slice(3,5));
    ret.setUTCSeconds(timeString.slice(6,9));
    return ret.getTime();
};

const parseStopTimes = (stream) => {
    return new Promise((resolve, reject) => {
        const parser = csv.parse({ delimiter: ',', columns: true, bom: true });
        parser.on('error', (err) => {
            reject(err);
        });
        const entries = {};
        parser.on('readable', () => {
            let record;
            while ((record = parser.read()) !== null) {
                if (!entries.hasOwnProperty(record.trip_id)) entries[record.trip_id] = {};
                entries[record.trip_id][record.stop_sequence * 1] = {
                    stopID: record.stop_id * 1,
                    arrivalTime: convertTime(record.arrival_time),
                    departureTime: convertTime(record.departure_time)
                };
            }
        });
        parser.on('end', () => {
            resolve({ times: entries });
        });
        stream.pipe(parser);
    });
};

const parseTrips = (stream) => {
    return new Promise((resolve, reject) => {
        const parser = csv.parse({ delimiter: ',', columns: true, bom: true });
        parser.on('error', (err) => {
            reject(err);
        });
        const entries = {};
        parser.on('readable', () => {
            let record;
            while ((record = parser.read()) !== null) {
                entries[record.trip_id] = record.service_id;
            }
        });
        parser.on('end', () => {
            resolve({ tripCalendar: entries });
        });
        stream.pipe(parser);
    });
};

const extractFeed = (readStream) => {
    const feed = temp.createWriteStream('gtfs_feed');
    return new Promise((resolve, reject) => {
        readStream.on('end', () => { // on extraction completion
            feed.close();
            yauzl.open(feed.path, { lazyEntries: true, autoClose: true }, (err, zipfile) => {
                if (err) return reject(err);

                const promises = [];
                zipfile.readEntry();
                zipfile.on('entry', (entry) => {
                    const entryPath = entry.fileName;
                    if (entryPath == 'calendar.txt') {
                        zipfile.openReadStream(entry, (err, fileStream) => {
                            if (err) return reject(err);
                            promises.push(parseCalendar(fileStream));
                        });
                    }
                    else if (entryPath == 'stops.txt') {
                        zipfile.openReadStream(entry, (err, fileStream) => {
                            if (err) return reject(err);
                            promises.push(parseStops(fileStream));
                        });
                    }
                    else if (entryPath == 'stop_times.txt') {
                        zipfile.openReadStream(entry, (err, fileStream) => {
                            if (err) return reject(err);
                            promises.push(parseStopTimes(fileStream));
                        });
                    }
                    else if (entryPath == 'trips.txt') {
                        zipfile.openReadStream(entry, (err, fileStream) => {
                            if (err) return reject(err);
                            promises.push(parseTrips(fileStream));
                        });
                    }
                    zipfile.readEntry();
                });

                zipfile.once('close', () => {
                    Promise.all(promises).then((results) => {
                        resolve(results.reduce(((r, c) => Object.assign(r, c)), {}));
                    });
                });
            });
        });
        readStream.pipe(feed);
    });
};

const download = () => {
    const file = temp.createWriteStream('gtfs');
    return fetch(GTFS_DATASET_URL)
        .then((response) => { // download GTFS dataset to temp file
            console.log(`Downloading GTFS dataset from ${GTFS_DATASET_URL}`);
            const progressStream = createProgressStream(response.headers.get('Content-Length') * 1);
            stream.Readable.fromWeb(response.body)
                .pipe(progressStream) // also pipe to progress stream
                .pipe(file); // download to temp file
            return new Promise((resolve, reject) => {
                file.on('error', reject);
                file.on('finish', () => {
                    file.close();
                    resolve(file.path);
                });
            });
        })
        .then((filePath) => { // extract zip file
            return new Promise((resolve, reject) => {
                console.log(`GTFS dataset has been downloaded to ${filePath}, extracting data`);
                yauzl.open(filePath, { lazyEntries: true, autoClose: false }, (err, zipfile) => {
                    if (err) throw err;
                    const promises = []; // feed extraction promises
                    zipfile.readEntry();
                    zipfile.on('entry', (entry) => {
                        const entryPath = entry.fileName;
                        // console.log(entryPath);
                        if (!/\/$/.test(entryPath)) { // file
                            const pathSegments = entryPath.match(/[^\/]+/g) || []; // get path segments
                            if (
                                pathSegments[pathSegments.length - 1] === 'google_transit.zip'
                                && (pathSegments[0] == '1' || pathSegments[0] == '2') // 1 = regional train, 2 = metro train
                            ) {
                                /* GTFS feed zip file found */
                                console.log(`Extracting GTFS feed file ${entryPath}`);
                                zipfile.openReadStream(entry, (err, readStream) => {
                                    if (err) throw err;
                                    promises.push(extractFeed(readStream));
                                });
                            }
                        }
                        zipfile.readEntry(); // read next entry
                    });

                    zipfile.once('end', () => {
                        zipfile.close();
                        Promise.all(promises).then((results) => { // wait for all extraction promises to finish
                            const ret = {
                                calendar: {},
                                stops: {},
                                times: {},
                                tripCalendar: {}
                            };

                            for (let i = 0; i < results.length; i++) {
                                for (const [key, value] of Object.entries(results[i].calendar)) {
                                    ret.calendar[`${i}_${key}`] = value;
                                }
                                for (const [key, value] of Object.entries(results[i].tripCalendar)) {
                                    ret.tripCalendar[key] = `${i}_${value}`;
                                }
                                for (const [key, value] of Object.entries(results[i].stops)) {
                                    if (ret.stops.hasOwnProperty(key)) ret.stops[key].ids = ret.stops[key].ids.concat(value.ids);
                                    else ret.stops[key] = value;
                                }
                                ret.times = { ...ret.times, ...results[i].times };
                            }

                            resolve(ret);
                        });
                    });
                });
            });
        })
        .catch((err) => {
            console.error(`Error encountered downloading GTFS dataset: ${err}`);
            throw err;
        })
        .finally(() => temp.cleanup()); // clean up after ourselves
};

const updateGTFS = () => {
    const database = require('./database'); // connect to database
    const { TableName, ColumnSet, insert } = database.pgp.helpers;
    const db = database.db;

    return download().then((data) => {
        const stops = [];
        const stopIDs = [];
        const stopNames = [];
        for (const [key, value] of Object.entries(data.stops)) {
            stopNames.push({ code: key, name: value.name });
            for (const id of value.ids) {
                stops.push({
                    id: id,
                    station: key
                });
                stopIDs.push(id);
            }
        }

        console.log('Updating database.');
        return Promise.all([
                (() => { // calendar 
                    const cs = new ColumnSet(
                        ['id', 'start_date', 'end_date', 'monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday'],
                        { 
                            table: new TableName({
                                schema: 'gtfs',
                                table: 'calendar'
                            }) 
                        }
                    );
                    const calendar = [];
                    for (const [key, value] of Object.entries(data.calendar)) {
                        calendar.push({
                            id: key,
                            start_date: value.startDate,
                            end_date: value.endDate,
                            monday: value.monday,
                            tuesday: value.tuesday,
                            wednesday: value.wednesday,
                            thursday: value.thursday,
                            friday: value.friday,
                            saturday: value.saturday,
                            sunday: value.sunday
                        });
                    }
                    return db.none('TRUNCATE TABLE gtfs.calendar CASCADE')
                        .then(() => db.none(insert(calendar, cs)))
                        .then(() => console.log('Calendar has been updated'));
                })(),
                (() => { // stop names
                    const cs = new ColumnSet(
                        ['code', 'name'],
                        { 
                            table: new TableName({
                                schema: 'gtfs',
                                table: 'stop_names'
                            }) 
                        }
                    );
                    /* stops iterated above */
                    return db.none('TRUNCATE TABLE gtfs.stop_names CASCADE')
                        .then(() => db.none(insert(stopNames, cs)))
                        .then(() => console.log('Stops have been updated'));
                })().then(() => { // stops
                    const cs = new ColumnSet(
                        ['id', 'station'],
                        { 
                            table: new TableName({
                                schema: 'gtfs',
                                table: 'stops'
                            }) 
                        }
                    );
                    /* stops iterated above */
                    return db.none('TRUNCATE TABLE gtfs.stops CASCADE')
                        .then(() => db.none(insert(stops, cs)))
                        .then(() => console.log('Stops have been updated'));
                })
            ]).then(() => { // trips
                const cs = new ColumnSet(
                    ['id', 'calendar'],
                    { 
                        table: new TableName({
                            schema: 'gtfs',
                            table: 'trips'
                        }) 
                    }
                );
                const trips = [];
                for (const [key, value] of Object.entries(data.tripCalendar)) {
                    trips.push({
                        id: key,
                        calendar: value
                    });
                }
                return db.none('TRUNCATE TABLE gtfs.trips CASCADE')
                    .then(() => db.none(insert(trips, cs)))
                    .then(() => console.log('Trips have been updated'));
            }).then(() => { // timetable
                const cs = new ColumnSet(
                    ['trip_id', 'seq', 'stop_id', 'arrival', 'departure'],
                    { 
                        table: new TableName({
                            schema: 'gtfs',
                            table: 'timetable'
                        }) 
                    }
                );
                const timetable = [];
                for (const [trip, times] of Object.entries(data.times)) {
                    for (const [seq, value] of Object.entries(times)) {
                        if (!stopIDs.includes(value.stopID)) continue; // skip entries with non-metro stops
                        timetable.push({
                            trip_id: trip,
                            seq: seq,
                            stop_id: value.stopID,
                            arrival: `${(value.arrivalTime / 1000).toFixed(0)}S`,
                            departure: `${(value.departureTime / 1000).toFixed(0)}S`
                        });
                    }
                }
                console.log(`${timetable.length} rows will be inserted!`);
                return db.none('TRUNCATE TABLE gtfs.timetable CASCADE')
                    .then(() => db.tx('timetable-insert', (t) => {
                        const processData = (data) => {
                            if (data) {
                                const query = insert(data, cs);
                                return t.none(query);
                            }
                        };
                        return t.sequence((page) => {
                            const index = page * 5000;
                            const maxIndex = Math.min(index + 1000, timetable.length);
                            console.log(`Inserting page ${page + 1} (${index}-${maxIndex-1}/${timetable.length}).`);
                            return processData((index >= timetable.length) ? undefined : timetable.slice(index, maxIndex));
                        });
                    }))
                    .then((data) => console.log(`Timetable has been updated (${data.total} batches, ${data.duration} msec)`));
            });
    }).then(() => {
        console.log("All operations completed");
    });
}

module.exports = { download, updateGTFS };

const HEALTHCHECK_PORT = process.env.HEALTHCHECK_PORT || 3000;

if (require.main === module) {
    updateGTFS().then(() => {
        /* schedule weekly schedule update at 9pm on the 15th day of every month - should give us plenty of clearance since each schedule has 90 days of data */
        schedule.scheduleJob('0 21 15 * *', async () => {
            console.log('>>> Updating GTFS schedule.');
            await updateGTFS();
        });

        /* start healthcheck server */
        const healthCheck = require('./healthcheck');
        healthCheck.start(HEALTHCHECK_PORT);
    });
}