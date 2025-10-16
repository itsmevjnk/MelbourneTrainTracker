#include "lsid.h"

#include "esp_log.h"
#include "esp_check.h"

#include <algorithm>

const char* LSID::kTag = "lsid";

uint16_t LSID::getLED(infraid_t line, infraid_t code) {
    switch (line) {
        /* metropolitan lines */
        case INFRAID_SHM: return shmGetLED(code);
        case INFRAID_MDD: return mddGetLED(code);
        case INFRAID_HBE: return hbeGetLED(code);
        case INFRAID_CCL: return cclGetLED(code);
        case INFRAID_PKM: return pkmGetLED(code);
        case INFRAID_CBE: return cbeGetLED(code);
        case INFRAID_BEG: return begGetLED(code);
        case INFRAID_LIL: return lilGetLED(code);
        case INFRAID_GWY: return gwyGetLED(code);
        case INFRAID_ALM: return almGetLED(code);
        case INFRAID_STY: return styGetLED(code);
        case INFRAID_FKN: return fknGetLED(code);
        case INFRAID_WIL: return wilGetLED(code);
        case INFRAID_WER: return werGetLED(code);
        case INFRAID_CGB: return cgbGetLED(code);
        case INFRAID_SUY: return suyGetLED(code);
        case INFRAID_RCE: return rceGetLED(code);
        case INFRAID_UFD: return ufdGetLED(code);

        /* country lines */
        case INFRAID_ART: return melGetLED(code); // Ararat
        case INFRAID_BAT: return melGetLED(code); // Ballarat
        case INFRAID_MBY: return melGetLED(code); // Maryborough

        case INFRAID_BDE: return gplGetLED(code); // Bairnsdale
        case INFRAID_TRN: return gplGetLED(code); // Traralgon
        case INFRAID_vPK: return gplGetLED(code); // Pakenham

        case INFRAID_GEL: return wvlGetLED(code); // Geelong
        case INFRAID_WBL: return wvlGetLED(code); // Warrnambool

        default:
            ESP_LOGE(kTag, "invalid line code " INFRAID2STR_FMT, INFRAID2STR(line));
            return LMAT_NULL;
    }
}

size_t LSID::getLEDsBetween(infraid_t line, infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    switch (line) {
        /* metropolitan lines */
        case INFRAID_SHM: return shmGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_MDD: return mddGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_HBE: return hbeGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_CCL: return cclGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_PKM: return pkmGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_CBE: return cbeGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_BEG: return begGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_LIL: return lilGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_GWY: return gwyGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_ALM: return almGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_STY: return styGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_FKN: return fknGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_WIL: return wilGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_WER: return werGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_CGB: return cgbGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_SUY: return suyGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_RCE: return rceGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_UFD: return ufdGetLEDsBetween(fromCode, toCode, buffer, maxLength);

        /* country lines */
        case INFRAID_ART: return melGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Ararat
        case INFRAID_BAT: return melGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Ballarat
        case INFRAID_MBY: return melGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Maryborough

        case INFRAID_BDE: return gplGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Bairnsdale
        case INFRAID_TRN: return gplGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Traralgon
        case INFRAID_vPK: return gplGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Pakenham

        case INFRAID_GEL: return wvlGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Geelong
        case INFRAID_WBL: return wvlGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Warrnambool

        default:
            ESP_LOGE(kTag, "invalid line code " INFRAID2STR_FMT, INFRAID2STR(line));
            return 0;
    }
}

colour_t LSID::getLineColour(infraid_t line) {
    switch (line) {
        /* metropolitan lines */
        case INFRAID_SHM: return kSandringham;

        case INFRAID_MDD: return kClifton;
        case INFRAID_HBE: return kClifton;
        case INFRAID_CCL: return kSpecial;

        case INFRAID_PKM: return kDandenong;
        case INFRAID_CBE: return kDandenong;

        case INFRAID_BEG: return kBurnley;
        case INFRAID_LIL: return kBurnley;
        case INFRAID_GWY: return kBurnley;
        case INFRAID_ALM: return kBurnley;

        case INFRAID_STY: return kCrossCity;
        case INFRAID_FKN: return kCrossCity;
        case INFRAID_WIL: return kCrossCity;
        case INFRAID_WER: return kCrossCity;

        case INFRAID_CGB: return kNorthern;
        case INFRAID_SUY: return kNorthern;
        case INFRAID_RCE: return kFlemington;
        case INFRAID_UFD: return kNorthern;

        /* country lines */
        case INFRAID_ART: return kVLine;
        case INFRAID_BAT: return kVLine;
        case INFRAID_MBY: return kVLine;
        case INFRAID_BDE: return kVLine;
        case INFRAID_TRN: return kVLine;
        case INFRAID_vPK: return kVLine;
        case INFRAID_GEL: return kVLine;
        case INFRAID_WBL: return kVLine;

        default:
            ESP_LOGE(kTag, "unknown line code " INFRAID2STR_FMT, INFRAID2STR(line));
            return kSpecial;
    }
}

infraid_t LSID::getStationFromStopID(int id) {
    const struct StopIDTableEntry *begin = kStopIDTable;
    const struct StopIDTableEntry *end = &kStopIDTable[kStopIDTableLength];
    const struct StopIDTableEntry *it = std::lower_bound(begin, end, id, [](const StopIDTableEntry &a, int b) { return a.stopID < b; });
    return (it != end && it->stopID == id) ? it->infraID : 0;
}