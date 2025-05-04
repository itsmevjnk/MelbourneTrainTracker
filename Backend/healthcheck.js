/* health check server */

const express = require('express');

const app = express();

app.get('/', (req, res) => {
    res.status(200).send(new Date());
});

const start = (port = null) => {
    port = port || 3000; // default HTTP port
    app.listen(port, () => {
        console.log(`Health check server listening on port ${port}`);
    });
};

module.exports = { start };

if (require.main === module) {
    start(process.env.PORT);
}
