/* MQTT communication */

const mqtt = require('mqtt');
const secret = require('./secret');
const { pgp, db } = require('./database');
const { TableName, ColumnSet, select } = pgp.helpers;

const MQTT_BROKER = process.env.MQTT_BROKER || 'mqtt://broker.hivemq.com';
const MQTT_USERNAME = process.env.MQTT_USERNAME || undefined;
const MQTT_PASSWORD = secret.read(process.env.MQTT_PASSWORD_FILE) || process.env.MQTT_PASSWORD || undefined;
const MQTT_TOPIC = process.env.MQTT_TOPIC || 'melbtrains';

const client = mqtt.connect(MQTT_BROKER, {
    username: MQTT_USERNAME,
    password: MQTT_PASSWORD
});

client.on('connect', () => {
    console.log(`Connect to MQTT broker ${MQTT_BROKER}`);
});

let lastMessage = null;

const publish = () => {
    return db.any(`
        SELECT * FROM daily.timetable
        WHERE NOT (
            (departure < CURRENT_TIMESTAMP - INTERVAL '0:00:15')
            OR (arrival > CURRENT_TIMESTAMP + INTERVAL '0:01:00')
        )
    `).then((rows) => {
        const message = [];
        for (const row of rows) {
            const entryBase = {
                seq: row.seq,
                line: row.line,
                trip: row.trip_id,
                stn: row.station
            };
            message.push({
                ...entryBase,
                arr: row.arrival.getTime() / 1000,
                adj: row.prev_station || undefined, // adjacent station
                adjt: (!row.prev_departure) ? undefined : (row.prev_departure.getTime() / 1000)
            }); // arrival entry
            message.push({
                ...entryBase,
                dep: row.departure.getTime() / 1000,
                adj: row.next_station || undefined,
                adjt: (!row.next_arrival) ? undefined : (row.next_arrival.getTime() / 1000)
            }); // departure entry
        }

        message.sort((a, b) => {
            const aTime = a.arr || a.dep;
            const bTime = b.arr || b.dep;
            return aTime - bTime;
        }); // sort by ascending timestamp

        const messageStr = JSON.stringify(message);
        if (messageStr != lastMessage) {
            console.log(`Publishing message with ${message.length} entries (${messageStr.length} bytes)`);
            lastMessage = messageStr;
            return client.publishAsync(MQTT_TOPIC, messageStr, { qos: 1, retain: true })
                .then((_) => message);
        } else {
            console.log('Duplicate message, ignoring');
            return null;
        }
    });
};

module.exports = { publish };

if (require.main === module) {
    client.on('connect', () => {
        publish().then((message) => {
            if (message == null) console.log('No messages sent');
            else console.log('Message has been sent');
    
            return client.endAsync();
        }).then(() => {
            console.log('Disconnected from MQTT broker');
        });
    });
}