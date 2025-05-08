/* MQTT communication */

const mqtt = require('mqtt');
const secret = require('./secret');
const { pgp, db } = require('./database');
const hash = require('js-xxhash');
const { TableName, ColumnSet, select } = pgp.helpers;

const MQTT_BROKER = process.env.MQTT_BROKER || 'mqtt://broker.hivemq.com';
const MQTT_USERNAME = process.env.MQTT_USERNAME || undefined;
const MQTT_PASSWORD = secret.read(process.env.MQTT_PASSWORD_FILE) || process.env.MQTT_PASSWORD || undefined;
const MQTT_JSON_TOPIC = process.env.MQTT_JSON_TOPIC || 'melbtrains';
const MQTT_BIN_TOPIC = process.env.MQTT_BIN_TOPIC || 'melbtrains/bin';
const WINDOW_PAST = process.env.WINDOW_PAST || '00:00:30';
const WINDOW_FUTURE = process.env.WINDOW_FUTURE || '00:03:00';

const client = mqtt.connect(MQTT_BROKER, {
    username: MQTT_USERNAME,
    password: MQTT_PASSWORD
});

client.on('connect', () => {
    console.log(`Connect to MQTT broker ${MQTT_BROKER}`);
});

let lastMessage = null;

/* NEW: serialise message to binary for compactness, suitable for the ESP32 */
const toInfraID = (code) => {
    const buf = new Uint8Array(3);
    for (let i = 0; i < buf.length; i++) buf[i] = code[i].charCodeAt(0);
    return buf;
};

const toInt64 = (num) => {
    const buf = Buffer.alloc(8);
    buf.writeBigInt64LE(BigInt(num));
    return buf;
};

const toUInt32 = (num) => {
    const buf = Buffer.alloc(4);
    buf.writeUInt32LE(num);
    return buf;
};

const toUInt8 = (num) => {
    const arr = new Uint8Array(1);
    arr[0] = num;
    return arr;
};

const binarySerialise = (message) => {
    const serialisedEntries = [];
    for (const entry of message) {
        const isDeparture = entry.hasOwnProperty('dep') && entry.dep;
        const hasAdjacent = entry.hasOwnProperty('adj') && entry.adj;

        // console.log(entry);

        let properties = [
            toUInt32(hash.xxHash32(entry.trip, 0)),
            toInfraID(entry.line),
            toInfraID(entry.stn),
            toInt64(entry.dep || entry.arr),
            toUInt8((hasAdjacent ? (1 << 1) : 0) | (isDeparture ? (1 << 0) : 0))
        ];
        if (hasAdjacent) {
            properties = properties.concat([
                toInfraID(entry.adj),
                toInt64(entry.adjt)
            ]);
        }
        serialisedEntries.push(Buffer.concat(properties));
    }

    return Buffer.concat([
        toUInt32(serialisedEntries.length),
        ...serialisedEntries
    ]);
};

const publishMessage = (message) => {
    const binMessage = binarySerialise(message);
    if (binMessage != lastMessage) {
        const messageStr = JSON.stringify(message);
        console.log(`Publishing message with ${message.length} entries (${messageStr.length} bytes in JSON, ${binMessage.length} bytes in binary)`);
        lastMessage = binMessage;
        return Promise.all([
            client.publishAsync(MQTT_JSON_TOPIC, messageStr, { qos: 1, retain: true }),
            client.publishAsync(MQTT_BIN_TOPIC, binMessage, { qos: 1, retain: true })
        ]).then((_) => message);
    } else {
        console.log('Duplicate message, ignoring');
        return null;
    }    
};

const publish = () => {
    return db.any(`
        SELECT
            t.trip_id AS trip_id,
            t.line AS line,
            t.seq AS seq,
            t.arrival AS arrival,
            t.departure AS departure,
            t.station AS station,
            next.station AS next_station,
            next.arrival AS next_arrival,
            prev.station AS prev_station,
            prev.departure AS prev_departure
        FROM daily.timetable t
        LEFT JOIN LATERAL (
            SELECT station, arrival
            FROM daily.timetable
            WHERE trip_id = t.trip_id AND seq = t.seq + 1
            LIMIT 1
        ) next ON true
        LEFT JOIN LATERAL (
            SELECT station, departure
            FROM daily.timetable
            WHERE trip_id = t.trip_id AND seq = t.seq - 1
            LIMIT 1
        ) prev ON true
        WHERE
            NOT (
                (t.departure < CURRENT_TIMESTAMP - INTERVAL '${WINDOW_PAST}')
                OR (t.arrival > CURRENT_TIMESTAMP + INTERVAL '${WINDOW_FUTURE}')
            )
            OR (
                (t.departure < CURRENT_TIMESTAMP - INTERVAL '${WINDOW_PAST}')
                AND (next.arrival > CURRENT_TIMESTAMP + INTERVAL '${WINDOW_FUTURE}')
            )
            OR (
                (t.arrival < CURRENT_TIMESTAMP - INTERVAL '${WINDOW_PAST}')
                AND (t.departure > CURRENT_TIMESTAMP + INTERVAL '${WINDOW_FUTURE}')
            )
    `).then((rows) => {
        const message = [];
        for (const row of rows) {
            const entryBase = {
                // seq: row.seq,
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

        return publishMessage(message);
    });
};

module.exports = { publish, publishMessage };

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