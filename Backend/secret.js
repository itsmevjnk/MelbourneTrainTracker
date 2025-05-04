/* secret file reading */

const fs = require('fs');

const read = (path) => {
    if (!path) return undefined;
    
    if (!fs.existsSync(path)) {
        console.warn(`Secret file ${path} not found`);
        return undefined;
    }

    const stats = fs.statSync(path);
    if (stats.size === 0) {
        console.warn(`Secret file ${path} is empty`);
        return undefined;
    }

    const file = fs.openSync(path, 'r');
    const buffer = Buffer.alloc(stats.size + 1);
    const bytesRead = fs.readSync(file, buffer, 0, buffer.length, 0);
    fs.closeSync(file);
    return buffer.toString('utf8', 0, bytesRead).split('\n')[0].trim();
};

module.exports = { read };

