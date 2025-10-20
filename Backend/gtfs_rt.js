/* fetch GTFS Realtime feed */

const gtfs = require('gtfs-realtime-bindings');
const secret = require('./secret');

const METRO_UPDATE_API_URL = process.env.METRO_UPDATE_API_URL || 'https://api.opendata.transport.vic.gov.au/opendata/public-transport/gtfs/realtime/v1/metro/trip-updates';
const METRO_ALERT_API_URL = process.env.METRO_ALERT_API_URL || 'https://api.opendata.transport.vic.gov.au/opendata/public-transport/gtfs/realtime/v1/metro/service-alerts';
const VLINE_UPDATE_API_URL = process.env.VLINE_UPDATE_API_URL || 'https://api.opendata.transport.vic.gov.au/opendata/public-transport/gtfs/realtime/v1/vline/trip-updates';
// NOTE: the V/Line service alert API seems to be active, but it's not documented - let's just assume that it doesn't exist yet for now.

const API_KEY = secret.read(process.env.API_KEY_FILE) || process.env.API_KEY;

const fetchUpdate = (tripUpdateURL, serviceAlertURL = null) => {
    const fetchPromises = [
        fetch(tripUpdateURL, {
            method: 'GET',
            headers: {
                // 'Cache-Control': 'no-cache',
                'accept': '*/*',
                'KeyId': API_KEY
            }
        })
    ];
    if (serviceAlertURL !== null) fetchPromises.push(
        fetch(serviceAlertURL, {
            method: 'GET',
            headers: {
                // 'Cache-Control': 'no-cache',
                'accept': '*/*',
                'KeyId': API_KEY
            }
        })
    );

    return Promise.all(fetchPromises)
    .then((responses) => {
        const promises = [];
        for (const response of responses) promises.push(response.arrayBuffer());
        return Promise.all(promises);
    })
    .then((buffers) => {
        const updateFeed = gtfs.transit_realtime.FeedMessage.decode(new Uint8Array(buffers[0]));
        const timestamp = new Date(updateFeed.header.timestamp.toNumber() * 1000);
        const updates = {};
        const trips = {};
        for (const entity of updateFeed.entity) {
            const update = entity.tripUpdate;
            if (update) {
                const tripID = update.trip.tripId;
                const entries = {};
                for (const entry of update.stopTimeUpdate) {
                    let stopID;
                    if (isNaN(entry.stopId)) { // 3-letter code
                        const idSplit = entry.stopId.split(':');
                        stopID = idSplit[idSplit.length - 1]; // e.g. vic:rail:SGS
                    } else stopID = entry.stopId * 1;
                    const outputEntry = { stop: stopID };
                    if (entry.arrival) {
                        outputEntry.arrival = new Date(entry.arrival.time.toNumber() * 1000);
                        if (!entry.departure) outputEntry.departure = outputEntry.arrival;
                    }
                    if (entry.departure) {
                        outputEntry.departure = new Date(entry.departure.time.toNumber() * 1000);
                        if (!entry.arrival) outputEntry.arrival = outputEntry.departure;
                    }
                    if (outputEntry.departure.getTime() < outputEntry.arrival.getTime()) {
                        /* swap departure and arrival - not sure why this happens though */
                        let t = outputEntry.departure;
                        outputEntry.departure = outputEntry.arrival;
                        outputEntry.arrival = t;
                    }
                    entries[entry.stopSequence] = outputEntry;
                }
                updates[tripID] = entries;
                
                const tripDesc = update.trip;
                const startDate = tripDesc.startDate;
                const startTime = tripDesc.startTime.split(':');
                trips[update.trip.tripId] = new Date(
                    startDate.slice(0, 4), startDate.slice(4, 6) - 1, startDate.slice(6, 8),
                    startTime[0], startTime[1], startTime[2]
                );
            }
        }

        const cancellations = [];
        if (buffers.length > 1) {
            const alertFeed = gtfs.transit_realtime.FeedMessage.decode(new Uint8Array(buffers[1]));
            for (const entity of alertFeed.entity) {
                const alert = entity.alert;
                if (alert) {
                    const effect = alert.effect;
                    if (effect == 1) { // no service - cancelled service
                        for (const informedEntity of alert.informedEntity) {
                            cancellations.push(informedEntity.trip.tripId);
                        }
                    }
                    // else {
                    //     console.warn(`Unknown alert type ${effect}: ${alert.descriptionText.translation[0].text}`);
                    // }
                }
            }
        }

        return {
            timestamp: timestamp,
            tripStart: trips,
            updates: updates,
            cancellations: cancellations
        };
    });
};

const fetchAllUpdates = () => {
    return Promise.all([
        fetchUpdate(METRO_UPDATE_API_URL, METRO_ALERT_API_URL),
        fetchUpdate(VLINE_UPDATE_API_URL)
    ]).then((results) => {
        const merged = {
            timestamp: new Date(), // doesn't matter TBH, since the GTFS-R API now returns the latest timestamp
            tripStart: {},
            updates: {},
            cancellations: []
        };

        for (const res of results) {
            Object.assign(merged.tripStart, res.tripStart);
            Object.assign(merged.updates, res.updates);
            merged.cancellations.concat(res.cancellations);
        }

        return merged;
    });
};

module.exports = { fetchUpdate, fetchAllUpdates };

if (require.main === module) {
    fetchAllUpdates().then((feed) => {
        console.log(JSON.stringify(feed, null, 2));
    });
}
