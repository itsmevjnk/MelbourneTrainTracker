#include "lsid.h"

#include "esp_log.h"
#include "esp_check.h"

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
        case INFRAID_ART: return artGetLED(code); // Ararat
        case INFRAID_BAT: return artGetLED(code); // Ballarat
        case INFRAID_MBY: return mbyGetLED(code); // Maryborough

        case INFRAID_BDE: return gplGetLED(code); // Bairnsdale
        case INFRAID_TRN: return gplGetLED(code); // Traralgon

        case INFRAID_GEL: return gelGetLED(code); // Geelong
        case INFRAID_WBL: return gelGetLED(code); // Warrnambool

#if LMAT_VLINE_HAS_BENDIGO
        case INFRAID_BGO: return swlGetLED(code); // Bendigo
        case INFRAID_SWL: return swlGetLED(code); // Swan Hill
        case INFRAID_ECH: return echGetLED(code); // Echuca
#endif

#if LMAT_VLINE_HAS_SEYMOUR
        case INFRAID_SER: return snhGetLED(code); // Seymour
        case INFRAID_SNH: return snhGetLED(code); // Shepparton
        case INFRAID_ABY: return abyGetLED(code); // Albury
#endif

        /* Metro Tunnel lines */
#ifndef CONFIG_MUNNEL_COLOUR_NONE
        case INFRAID_SUYM: return suyGetLED(code);
        case INFRAID_CBEM: return cbeGetLED(code);
        case INFRAID_PKMM: return pkmGetLED(code);
#endif
#ifdef CONFIG_MUNNEL_COLOUR_DEST
        case INFRAID_SUYm: return suyGetLED(code);
        case INFRAID_CBEm: return cbeGetLED(code);
        case INFRAID_PKMm: return pkmGetLED(code);
#endif

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
        case INFRAID_ART: return artGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Ararat
        case INFRAID_BAT: return artGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Ballarat - we'll share the same code as Ararat, since we're just terminating short
        case INFRAID_MBY: return mbyGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Maryborough

        case INFRAID_BDE: return gplGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Bairnsdale
        case INFRAID_TRN: return gplGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Traralgon

        case INFRAID_GEL: return gelGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Geelong
        case INFRAID_WBL: return gelGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Warrnambool

#if LMAT_VLINE_HAS_BENDIGO
        case INFRAID_BGO: return swlGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Bendigo
        case INFRAID_SWL: return swlGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Swan Hill
        case INFRAID_ECH: return echGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Echuca
#endif

#if LMAT_VLINE_HAS_SEYMOUR
        case INFRAID_SER: return snhGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Seymour
        case INFRAID_SNH: return snhGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Shepparton
        case INFRAID_ABY: return abyGetLEDsBetween(fromCode, toCode, buffer, maxLength); // Albury
#endif

        /* Metro Tunnel lines */
#ifndef CONFIG_MUNNEL_COLOUR_NONE
        case INFRAID_SUYM: return suyGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_CBEM: return cbeMunnelGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_PKMM: return pkmMunnelGetLEDsBetween(fromCode, toCode, buffer, maxLength);
#endif
#ifdef CONFIG_MUNNEL_COLOUR_DEST
        case INFRAID_SUYm: return suyGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_CBEm: return cbeMunnelGetLEDsBetween(fromCode, toCode, buffer, maxLength);
        case INFRAID_PKMm: return pkmMunnelGetLEDsBetween(fromCode, toCode, buffer, maxLength);
#endif

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

        case INFRAID_STY: return kFrankston;
        case INFRAID_FKN: return kFrankston;
        case INFRAID_WIL: return kSandringham;
        case INFRAID_WER: return kSandringham;

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
        case INFRAID_GEL: return kVLine;
        case INFRAID_WBL: return kVLine;
#if LMAT_VLINE_HAS_BENDIGO
        case INFRAID_BGO: return kVLine;
        case INFRAID_SWL: return kVLine;
        case INFRAID_ECH: return kVLine;
#endif
#if LMAT_VLINE_HAS_SEYMOUR
        case INFRAID_SER: return kVLine;
        case INFRAID_SNH: return kVLine;
        case INFRAID_ABY: return kVLine;
#endif

#ifdef CONFIG_MUNNEL_COLOUR_BLUE
        case INFRAID_SUYM: return kDandenong;
        case INFRAID_CBEM: return kDandenong;
        case INFRAID_PKMM: return kDandenong;
#endif
#ifdef CONFIG_MUNNEL_COLOUR_DEST
        case INFRAID_SUYM: return kNorthern; // to Sunbury
        case INFRAID_SUYm: return kDandenong; // to Dandenong
        case INFRAID_CBEM: return kDandenong; // to Cranbourne
        case INFRAID_CBEm: return kNorthern; // to Sunbury
        case INFRAID_PKMM: return kDandenong; // to Pakenham
        case INFRAID_PKMm: return kNorthern; // to Sunbury
#endif

        default:
            ESP_LOGE(kTag, "unknown line code " INFRAID2STR_FMT, INFRAID2STR(line));
            return kSpecial;
    }
}