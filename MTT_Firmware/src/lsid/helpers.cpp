#include "lsid.h"
#include <assert.h>

#include "esp_log.h"
#include "esp_check.h"

// #include "esp_debug_helpers.h"

uint16_t LSID::getLEDStub(const station_t** stations, const infraid_t* codes, size_t count, infraid_t code) {
    for (size_t i = 0; i < count; i++) {
        if (codes[i] == code) return stations[i]->led;
    }
    ESP_LOGE(kTag, "cannot find station code " INFRAID2STR_FMT, INFRAID2STR(code));
    // esp_backtrace_print(10);
    return LMAT_NULL;
}

size_t LSID::getLEDsBetweenCodes(const station_t** stations, const infraid_t* codes, size_t count, infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    int fromIndex = -1, toIndex = -1;
    for (size_t i = 0; i < count && (fromIndex == -1 || toIndex == -1); i++) {
        if (codes[i] == fromCode) fromIndex = i;
        if (codes[i] == toCode) toIndex = i;
    }

    ESP_RETURN_ON_FALSE(fromIndex >= 0, 0, kTag, "cannot find fromCode " INFRAID2STR_FMT, INFRAID2STR(fromCode));
    ESP_RETURN_ON_FALSE(toIndex >= 0, 0, kTag, "cannot find toCode " INFRAID2STR_FMT, INFRAID2STR(toCode));

    return getLEDsBetweenIndices(stations, codes, count, fromIndex, toIndex, buffer, maxLength);
}

size_t LSID::getLEDsBetweenIndices(const station_t** stations, const infraid_t* codes, size_t count, int fromIndex, int toIndex, uint16_t* buffer, size_t maxLength) {
    if (fromIndex < 0 || fromIndex >= count || toIndex < 0 || toIndex >= count) {
        ESP_LOGE(kTag, "invalid indices (%d, %d)", fromIndex, toIndex);
        return 0;
    }

    size_t outIndex = 0;
    if (fromIndex < toIndex) { // Up direction
        for (int i = fromIndex; i < toIndex; i++) {
            buffer[outIndex++] = stations[i]->nextLED;
            if (outIndex == maxLength) break;
            if (i > fromIndex) {
                buffer[outIndex++] = stations[i]->led; // intermediary station
                if (outIndex == maxLength) break;
            }
        }            
    } else { // Down direction
        for (int i = fromIndex - 1; i >= toIndex; i--) {
            buffer[outIndex++] = stations[i]->nextLED;
            if (outIndex == maxLength) break;
            if (i > toIndex) {
                buffer[outIndex++] = stations[i]->led; // intermediary station
                if (outIndex == maxLength) break;
            }
        }
    }

    if (outIndex == 0 && fromIndex != toIndex) ESP_LOGW(kTag, "getLEDsBetweenIndices() returning 0 for index %d (" INFRAID2STR_FMT ") -> %d (" INFRAID2STR_FMT ")", fromIndex, INFRAID2STR(codes[fromIndex]), toIndex, INFRAID2STR(codes[toIndex]));

    return outIndex;
}
