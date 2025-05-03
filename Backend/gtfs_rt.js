/* fetch GTFS Realtime feed */

const gtfs = require('gtfs-realtime-bindings');

const API_URL = process.env.API_URL || 'https://data-exchange-api.vicroads.vic.gov.au/opendata/v1/gtfsr/metrotrain-tripupdates';
const API_KEY = process.env.API_KEY;

const fetchUpdate = () => {
    return fetch(API_URL, {
        method: 'GET',
        headers: {
            'Cache-Control': 'no-cache',
            'Ocp-Apim-Subscription-Key': API_KEY
        }
    })
    .then((response) => response.arrayBuffer())
    .then((buffer) => {
        const feed = gtfs.transit_realtime.FeedMessage.decode(new Uint8Array(buffer));
        const timestamp = new Date(feed.header.timestamp.toNumber() * 1000);
        const updates = {};
        for (const entity of feed.entity) {
            const update = entity.tripUpdate;
            if (update) {
                const tripID = update.trip.tripId;
                const entries = {};
                for (const entry of update.stopTimeUpdate) {
                    entries[entry.stopSequence] = {
                        arrival: new Date(entry.arrival.time.toNumber() * 1000),
                        departure: new Date(entry.departure.time.toNumber() * 1000)
                    }
                }
                updates[tripID] = entries;
            }
        }
        return {
            timestamp: timestamp,
            updates: updates
        };
    });
};

module.exports = { fetchUpdate };

if (require.main === module) {
    fetchUpdate().then((feed) => {
        console.log(JSON.stringify(feed, null, 2));
    });
}
