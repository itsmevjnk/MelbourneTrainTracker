#include "lsid.h"

#include "esp_log.h"
#include "esp_check.h"

size_t LSID::rmdCityGetLEDsBetween(
    const infraid_t* ccwCodes, const station_t** ccwStations, const station_t** cwStations,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) {
    int fromIndex = -1, toIndex = -1;
    for (size_t i = 0; i < 5 && (fromIndex == -1 || toIndex == -1); i++) {
        if (ccwCodes[i] == fromCode) fromIndex = i;
        if (ccwCodes[i] == toCode) toIndex = i;
    }

    ESP_RETURN_ON_FALSE(fromIndex >= 0, 0, kTag, "cannot find fromCode " INFRAID2STR_FMT, INFRAID2STR(fromCode));
    ESP_RETURN_ON_FALSE(toIndex >= 0, 0, kTag, "cannot find toCode " INFRAID2STR_FMT, INFRAID2STR(toCode));

    const station_t** stations = ccwStations; // assume counterclockwise
    if (fromIndex > toIndex) { // clockwise
        stations = cwStations;
        fromIndex = 5 - 1 - fromIndex;
        toIndex = 5 - 1 - toIndex;
    }

    size_t outIndex = 0;
    for (size_t i = fromIndex; i < toIndex && outIndex < maxLength; i++, outIndex++) {
        if (i > fromIndex) {
            buffer[outIndex++] = stations[i]->led; // intermediary station
            if (outIndex == maxLength) break;
        }
        buffer[outIndex] = stations[i]->nextLED;
    }

    return outIndex;
}

size_t LSID::rmdGetLEDsBetween(
    const infraid_t* cityCCWCodes, const station_t** cityCCWStations, const station_t** cityCWStations,
    const station_t** stations, const infraid_t* codes, size_t count,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) {
    bool fromCity = isCityStation(fromCode), toCity = isCityStation(toCode);
    
    assert(codes[count - 1] == INFRAID_RMD);
    const station_t* rmdStation = stations[count - 1]; // Richmond station

    // only handles entering/exiting Flinders St or City Loop stations as of now, and FSS and SSS are not skipped
    if (fromCity && toCity) { // intra-city
        return rmdCityGetLEDsBetween(cityCCWCodes, cityCCWStations, cityCWStations, fromCode, toCode, buffer, maxLength);
    } else if (fromCity) { // exiting from city
        size_t outIndex = 0;
        bool fromLoop = isCityLoopStation(fromCode);
        outIndex = rmdCityGetLEDsBetween(
            cityCCWCodes, cityCCWStations, cityCWStations,
            fromCode, (fromLoop) ? INFRAID_PAR : INFRAID_FSS, buffer, maxLength
        );
        ESP_RETURN_ON_FALSE(
            outIndex + 3 <= maxLength,
            outIndex,
            kTag, "not enough space to hold the %s -> RMD leg", (fromLoop) ? "FSS" : "PAR"
        );
        buffer[outIndex + 0] = cityCCWStations[(fromLoop) ? 0 : 4]->led; // FSS/PAR
        buffer[outIndex + 1] = rmdStation->nextLED; // RMD alt
        buffer[outIndex + 2] = rmdStation->led; // RMD
        outIndex += 3;
        return outIndex + getLEDsBetweenCodes(stations, codes, count, INFRAID_RMD, toCode, &buffer[outIndex], maxLength - outIndex);
    } else if (toCity) { // entering city
        bool toLoop = isCityLoopStation(toCode);
        size_t outIndex = getLEDsBetweenCodes(stations, codes, count, fromCode, INFRAID_RMD, buffer, maxLength); // get way into RMD first
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold RMD alt"
        );
        buffer[outIndex++] = rmdStation->nextLED; // RMD alt
        if (toCode == INFRAID_PAR || toCode == INFRAID_FSS) return outIndex; // nothing else to do

        /* either PAR or FSS gets skipped (which is unusual) */
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold %s", (toLoop) ? "PAR" : "FSS"
        );
        buffer[outIndex++] = cityCCWStations[(toLoop) ? 0 : 4]->led; // FSS/PAR
        return outIndex + rmdCityGetLEDsBetween(cityCCWCodes, cityCCWStations, cityCWStations, (toLoop) ? INFRAID_PAR : INFRAID_FSS, toCode, &buffer[outIndex], maxLength - outIndex);
    }
    else return getLEDsBetweenCodes(stations, codes, count, fromCode, toCode, buffer, maxLength); // both source and destination are outside city
}

