const pgp = require('pg-promise')({
    capSQL: true
});

const CONNECT_STR = process.env.CONNECT_STR || 'postgres://postgres:postgres@localhost:5432/melbtrains';
const db = pgp(CONNECT_STR);

process.env.TZ = 'Australia/Melbourne';

module.exports = { pgp, db };
