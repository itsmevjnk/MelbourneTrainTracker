/*
 * Main server script
 * This script polls the GTFS Realtime feed every 30s, updates the daily database,
 * then publishes a 5 min window (+15s into the past) of events occurring on the
 * network.
 * During init, and also at 3am every day, the server clears its daily database and
 * updates it with data for the next day.
 */

const rtUpdate = require('./rt_update');
const mqtt = require('./mqtt');
const daily = require('./daily_init');
const schedule = require('node-schedule');

/* initialisation */
if (require.main === module) {
    daily.daily_init().then(() => {
        /* schedule daily database update at 3am every day */
        schedule.scheduleJob('0 3 * * *', async () => {
            console.log('>>> Reinitialising daily database.');
            await daily.daily_init();
        });
        
        const periodicUpdate = async () => {
            console.log('>>> Updating from GTFS Realtime.');
            await rtUpdate.updateTimetable().then((result) => {
                return mqtt.publish().then(() => {
                    const currentTime = new Date().getTime();
                    let nextTimestamp = currentTime + 30000; // next invocation timestamp according to our clock
                    if (currentTime - result.timestamp.getTime() < 60000) {
                        /* data is not old yet - so we can use its timestamp for the next invocation timestamp */
                        const dataNextTimestamp = result.timestamp.getTime() + 45000; // 30 sec update interval + 15 sec to account for inaccuracies and caching
                        if (dataNextTimestamp - currentTime >= 5000) nextTimestamp = dataNextTimestamp; // to make sure that the next timestamp will not result in a missed invocation
                    }
                    nextTimestamp = new Date(nextTimestamp);
                    console.log(`>>> Message published, next update at ${nextTimestamp}.`);
                    schedule.scheduleJob(nextTimestamp, periodicUpdate); // reschedule ourselves
                });
            }).catch((err) => {
                console.error(`Error encountered polling GTFS Realtime: ${err}`);
                schedule.scheduleJob(new Date().getTime() + 30000, periodicUpdate); // reschedule to run after 30 sec
            });
        };

        return periodicUpdate(); // this will auto reschedule
    });
}
