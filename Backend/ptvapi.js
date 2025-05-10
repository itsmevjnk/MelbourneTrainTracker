/* PTV Timetable API */

const crypto = require('crypto');
const secret = require('./secret');

const PTV_API_URL = process.env.PTV_API_URL || 'https://timetableapi.ptv.vic.gov.au';
const PTV_API_ID = secret.read(process.env.PTV_ID_FILE) || process.env.PTV_API_ID;
const PTV_API_KEY = secret.read(process.env.PTV_KEY_FILE) || process.env.PTV_API_KEY;

const generateURL = (query) => {
    const url = new URL(query, PTV_API_URL);
    url.searchParams.set('devid', PTV_API_ID);
    url.searchParams.set('signature', crypto.createHmac('sha1', PTV_API_KEY).update(url.pathname + url.search).digest('hex').toUpperCase());
    return url.href;
};

/* ping server */
if (require.main === module) {
    const url = generateURL('/v3/route_types');
    console.log('Fetching from ' + url);
    fetch(url)
        .then((resp) => resp.text())
        .then((respString) => {
            // console.log(respString);
            console.log(JSON.stringify(JSON.parse(respString), null, 2));
        });
}
