const mqtt = require('./mqtt');

const base = { line: 'BEG', trip: 'TEST' };
const time = Math.floor(new Date().getTime() / 1000);
mqtt.publishMessage([
    { ...base, stn: 'MCE', arr: time + 0 },
    { ...base, stn: 'MCE', dep: time + 0, adj: 'PAR', adjt: time + 30 },
    { ...base, stn: 'PAR', arr: time + 30, adj: 'MCE', adjt: time + 0 },
    { ...base, stn: 'PAR', dep: time + 30, adj: 'RMD', adjt: time + 60 },
    { ...base, stn: 'RMD', arr: time + 60, adj: 'PAR', adjt: time + 30 },
    { ...base, stn: 'RMD', dep: time + 60, adj: 'CAM', adjt: time + 90 },
    { ...base, stn: 'CAM', arr: time + 90 },
    { ...base, stn: 'CAM', dep: time + 90 },
]);