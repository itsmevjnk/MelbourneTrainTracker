#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Craigieburn line */
static const station_t kCraigieburnCGB = { LMAT_CRAIGIEBURN_CGB, LMAT_CRAIGIEBURN_CGB_ALT };
static const station_t kCraigieburnRXP = { LMAT_CRAIGIEBURN_RXP, LMAT_CRAIGIEBURN_RXP_ALT };
static const station_t kCraigieburnCLO = { LMAT_CRAIGIEBURN_CLO, LMAT_CRAIGIEBURN_CLO_ALT };
static const station_t kCraigieburnBMS = { LMAT_CRAIGIEBURN_BMS, LMAT_CRAIGIEBURN_BMS_ALT };
static const station_t kCraigieburnJAC = { LMAT_CRAIGIEBURN_JAC, LMAT_CRAIGIEBURN_JAC_ALT };
static const station_t kCraigieburnGRY = { LMAT_CRAIGIEBURN_GRY, LMAT_CRAIGIEBURN_GRY_ALT };
static const station_t kCraigieburnOKP = { LMAT_CRAIGIEBURN_OKP, LMAT_CRAIGIEBURN_OKP_ALT };
static const station_t kCraigieburnPVL = { LMAT_CRAIGIEBURN_PVL, LMAT_CRAIGIEBURN_PVL_ALT };
static const station_t kCraigieburnSME = { LMAT_CRAIGIEBURN_SME, LMAT_CRAIGIEBURN_SME_ALT };
static const station_t kCraigieburnGBV = { LMAT_CRAIGIEBURN_GBV, LMAT_CRAIGIEBURN_GBV_ALT };
static const station_t kCraigieburnESD = { LMAT_CRAIGIEBURN_ESD, LMAT_CRAIGIEBURN_ESD_ALT };
static const station_t kCraigieburnMPD = { LMAT_CRAIGIEBURN_MPD, LMAT_CRAIGIEBURN_MPD_ALT };
static const station_t kCraigieburnASV = { LMAT_CRAIGIEBURN_ASV, LMAT_CRAIGIEBURN_ASV_ALT };
static const station_t kCraigieburnNKT = { LMAT_CRAIGIEBURN_NKT, LMAT_CRAIGIEBURN_NKT_ALT };
static const station_t kCraigieburnKEN = { LMAT_CRAIGIEBURN_KEN, LMAT_CRAIGIEBURN_KEN_ALT };

/* Flemington Racecourse line */
static const station_t kFlemingtonRCE = { LMAT_FLEMINGTON_RCE, LMAT_FLEMINGTON_RCE_ALT };
static const station_t kFlemingtonSGS = { LMAT_FLEMINGTON_SGS, LMAT_FLEMINGTON_SGS_ALT };

/* North Melbourne Station */
static const station_t kNorthernNME = { LMAT_NORTHERN_NME, LMAT_NULL };

/* Sunbury line */
static const station_t kSunburySUY = { LMAT_SUNBURY_SUY, LMAT_SUNBURY_SUY_ALT };
static const station_t kSunburyDRT = { LMAT_SUNBURY_DRT, LMAT_SUNBURY_DRT_ALT };
static const station_t kSunburyWGS = { LMAT_SUNBURY_WGS, LMAT_SUNBURY_WGS_ALT };
static const station_t kSunburyKPL = { LMAT_SUNBURY_KPL, LMAT_SUNBURY_KPL_ALT };
static const station_t kSunburySAB = { LMAT_SUNBURY_SAB, LMAT_SUNBURY_SAB_ALT };
static const station_t kSunburyGIN = { LMAT_SUNBURY_GIN, LMAT_SUNBURY_GIN_ALT };
static const station_t kSunburyALB = { LMAT_SUNBURY_ALB, LMAT_SUNBURY_ALB_ALT };
static const station_t kSunburySUN = { LMAT_SUNBURY_SUN, LMAT_SUNBURY_SUN_ALT };
static const station_t kSunburyTOT = { LMAT_SUNBURY_TOT, LMAT_SUNBURY_TOT_ALT };
static const station_t kSunburyWFY = { LMAT_SUNBURY_WFY, LMAT_SUNBURY_WFY_ALT };
static const station_t kSunburyMFY = { LMAT_SUNBURY_MFY, LMAT_SUNBURY_MFY_ALT };
static const station_t kSunburyFSY = { LMAT_SUNBURY_FSY, LMAT_SUNBURY_FSY_ALT };

/* Upfield line */
static const station_t kUpfieldUFD = { LMAT_UPFIELD_UFD, LMAT_UPFIELD_UFD_ALT };
static const station_t kUpfieldGOW = { LMAT_UPFIELD_GOW, LMAT_UPFIELD_GOW_ALT };
static const station_t kUpfieldFAK = { LMAT_UPFIELD_FAK, LMAT_UPFIELD_FAK_ALT };
static const station_t kUpfieldMYN = { LMAT_UPFIELD_MYN, LMAT_UPFIELD_MYN_ALT };
static const station_t kUpfieldBAT = { LMAT_UPFIELD_BAT, LMAT_UPFIELD_BAT_ALT };
static const station_t kUpfieldCOB = { LMAT_UPFIELD_COB, LMAT_UPFIELD_COB_ALT };
static const station_t kUpfieldMLD = { LMAT_UPFIELD_MLD, LMAT_UPFIELD_MLD_ALT };
static const station_t kUpfieldASY = { LMAT_UPFIELD_ASY, LMAT_UPFIELD_ASY_ALT };
static const station_t kUpfieldBWK = { LMAT_UPFIELD_BWK, LMAT_UPFIELD_BWK_ALT };
static const station_t kUpfieldJWL = { LMAT_UPFIELD_JWL, LMAT_UPFIELD_JWL_ALT };
static const station_t kUpfieldRPK = { LMAT_UPFIELD_RPK, LMAT_UPFIELD_RPK_ALT };
static const station_t kUpfieldFBD = { LMAT_UPFIELD_FBD, LMAT_UPFIELD_FBD_ALT };
static const station_t kUpfieldMAC = { LMAT_UPFIELD_MAC, LMAT_UPFIELD_MAC_ALT };

/* Northern city stations - counterclockwise (direct to FSS in the Up direction) */
static const station_t kNorthernSSS_CCW = { LMAT_NORTHERN_SSS, LMAT_NORTHERN_SSS_ALT };
static const station_t kNorthernFSS_CCW = { LMAT_NORTHERN_FSS, LMAT_NULL }; // no LED between Flinders St and Parliament - in hindsight we should have put one there
static const station_t kNorthernPAR_CCW = { LMAT_NORTHERN_PAR, LMAT_NORTHERN_PAR_ALT };
static const station_t kNorthernMCE_CCW = { LMAT_NORTHERN_MCE, LMAT_NORTHERN_MCE_ALT };
static const station_t kNorthernFGS_CCW = { LMAT_NORTHERN_FGS, LMAT_NORTHERN_FGS_ALT };

/* Northern city stations - clockwise (via City Loop in the Up direction) */
static const station_t kNorthernFGS_CW = { LMAT_NORTHERN_FGS, LMAT_NORTHERN_MCE_ALT };
static const station_t kNorthernMCE_CW = { LMAT_NORTHERN_MCE, LMAT_NORTHERN_PAR_ALT };
static const station_t kNorthernPAR_CW = { LMAT_NORTHERN_PAR, LMAT_NULL };
static const station_t kNorthernFSS_CW = { LMAT_NORTHERN_FSS, LMAT_NORTHERN_SSS_ALT };
static const station_t kNorthernSSS_CW = { LMAT_NORTHERN_SSS, LMAT_NULL };

static const station_t* kNorthernCityCCWStations[] = {
    &kNorthernSSS_CCW, &kNorthernFSS_CCW, &kNorthernPAR_CCW, &kNorthernMCE_CCW, &kNorthernFGS_CCW
};
static const infraid_t kNorthernCityCCWCodes[] = {
    INFRAID_SSS, INFRAID_FSS, INFRAID_PAR, INFRAID_MCE, INFRAID_FGS
};

static const station_t* kNorthernCityCWStations[] = {
    &kNorthernFGS_CW, &kNorthernMCE_CW, &kNorthernPAR_CW, &kNorthernFSS_CW, &kNorthernSSS_CW
};
// static const infraid_t kNorthernCityCWCodes[] = {
//     INFRAID_FGS, INFRAID_MCE, INFRAID_PAR, INFRAID_FSS, INFRAID_SSS
// };

uint16_t LSID::nmeCityGetLED(infraid_t code) {
    switch (code) {
        case INFRAID_FSS: return LMAT_NORTHERN_FSS;
        case INFRAID_SSS: return LMAT_NORTHERN_SSS;
        case INFRAID_FGS: return LMAT_NORTHERN_FGS;
        case INFRAID_MCE: return LMAT_NORTHERN_MCE;
        case INFRAID_PAR: return LMAT_NORTHERN_PAR;
        default:
            ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
            return LMAT_NULL;
    }
}

size_t LSID::nmeCityGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    int fromIndex = -1, toIndex = -1;
    for (size_t i = 0; i < 5 && (fromIndex == -1 || toIndex == -1); i++) {
        if (kNorthernCityCCWCodes[i] == fromCode) fromIndex = i;
        if (kNorthernCityCCWCodes[i] == toCode) toIndex = i;
    }

    ESP_RETURN_ON_FALSE(fromIndex >= 0, 0, kTag, "cannot find fromCode " INFRAID2STR_FMT, INFRAID2STR(fromCode));
    ESP_RETURN_ON_FALSE(toIndex >= 0, 0, kTag, "cannot find toCode " INFRAID2STR_FMT, INFRAID2STR(toCode));

    const station_t** stations = kNorthernCityCCWStations; // assume counterclockwise
    if (fromIndex > toIndex) { // clockwise
        stations = kNorthernCityCWStations;
        fromIndex = 5 - 1 - fromIndex;
        toIndex = 5 - 1 - toIndex;
    }

    size_t outIndex = 0;
    for (size_t i = fromIndex; i < toIndex && outIndex < maxLength; i++) {
        if (i > fromIndex) {
            buffer[outIndex++] = stations[i]->led; // intermediary station
            if (outIndex == maxLength) break;
        }
        if (stations[i]->nextLED != LMAT_NULL)
            buffer[outIndex++] = stations[i]->nextLED;
    }

    return outIndex;
}

size_t LSID::nmeGetLEDsBetween(
    const station_t** stations, const infraid_t* codes, size_t count,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) {
    bool fromCity = isCityStation(fromCode), toCity = isCityStation(toCode);

    // only handles entering/exiting Flinders St or City Loop stations as of now, and FSS and SSS are not skipped
    if (fromCity && toCity) { // intra-city
        return nmeCityGetLEDsBetween(fromCode, toCode, buffer, maxLength);
    } else if (fromCity) { // exiting from city
        size_t outIndex = 0;
        bool fromLoop = isCityLoopStation(fromCode);
        outIndex = nmeCityGetLEDsBetween(fromCode, (fromLoop) ? INFRAID_FGS : INFRAID_SSS, buffer, maxLength);
        ESP_RETURN_ON_FALSE(
            outIndex + 3 <= maxLength,
            outIndex,
            kTag, "not enough space to hold the %s -> NME leg", (fromLoop) ? "FGS" : "SSS"
        );
        buffer[outIndex + 0] = (fromLoop) ? LMAT_NORTHERN_FGS : LMAT_NORTHERN_SSS;
        buffer[outIndex + 1] = (fromLoop) ? LMAT_NORTHERN_FGS_ALT : LMAT_NORTHERN_NME_ALT;
        buffer[outIndex + 2] = LMAT_NORTHERN_NME;
        outIndex += 3;
        return outIndex + getLEDsBetweenCodes(stations, codes, count, INFRAID_NME, toCode, &buffer[outIndex], maxLength - outIndex);
    } else if (toCity) { // entering city
        bool toLoop = isCityLoopStation(toCode);
        size_t outIndex = getLEDsBetweenCodes(stations, codes, count, fromCode, INFRAID_NME, buffer, maxLength); // get way into NME first
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold NME alt"
        );
        buffer[outIndex++] = (toLoop) ? LMAT_NORTHERN_FGS_ALT : LMAT_NORTHERN_NME_ALT; // enter Flagstaff or Sthn Cross
        if (toCode == INFRAID_FGS || toCode == INFRAID_SSS) return outIndex; // nothing else to do

        /* either FGS or SSS gets skipped (which is unusual) */
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold %s", (toLoop) ? "FGS" : "SSS"
        );
        buffer[outIndex++] = (toLoop) ? LMAT_NORTHERN_FGS : LMAT_NORTHERN_SSS;
        return outIndex + nmeCityGetLEDsBetween((toLoop) ? INFRAID_FGS : INFRAID_SSS, toCode, &buffer[outIndex], maxLength - outIndex);
    }
    else return getLEDsBetweenCodes(stations, codes, count, fromCode, toCode, buffer, maxLength); // both source and destination are outside city
}

static const station_t* kCraigieburnStations[] = {
    &kCraigieburnCGB, &kCraigieburnRXP, &kCraigieburnCLO, &kCraigieburnBMS, &kCraigieburnJAC, &kCraigieburnGRY, &kCraigieburnOKP,
    &kCraigieburnPVL, &kCraigieburnSME, &kCraigieburnGBV, &kCraigieburnESD, &kCraigieburnMPD, &kCraigieburnASV, &kCraigieburnNKT, &kCraigieburnKEN,
    &kNorthernNME
};
static const infraid_t kCraigieburnCodes[] = { 
    INFRAID_CGB, INFRAID_RXP, INFRAID_CLO, INFRAID_BMS, INFRAID_JAC, INFRAID_GRY, INFRAID_OKP,
    INFRAID_PVL, INFRAID_SME, INFRAID_GBV, INFRAID_ESD, INFRAID_MPD, INFRAID_ASV, INFRAID_NKT, INFRAID_KEN,
    INFRAID_NME
};
static const size_t kCraigieburnCount = sizeof(kCraigieburnStations) / sizeof(station_t*);

uint16_t LSID::cgbGetLED(infraid_t code) {
    if (isCityStation(code)) return nmeCityGetLED(code);
    return getLEDStub(kCraigieburnStations, kCraigieburnCodes, kCraigieburnCount, code);
}

size_t LSID::cgbGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return nmeGetLEDsBetween(kCraigieburnStations, kCraigieburnCodes, kCraigieburnCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kFlemingtonStations[] = {
    &kFlemingtonRCE, &kFlemingtonSGS, &kCraigieburnNKT, &kCraigieburnKEN,
    &kNorthernNME
};
static const infraid_t kFlemingtonCodes[] = { 
    INFRAID_RCE, INFRAID_SGS, INFRAID_NKT, INFRAID_KEN,
    INFRAID_NME
};
static const size_t kFlemingtonCount = sizeof(kFlemingtonStations) / sizeof(station_t*);

uint16_t LSID::rceGetLED(infraid_t code) {
    if (isCityStation(code)) return nmeCityGetLED(code);
    return getLEDStub(kFlemingtonStations, kFlemingtonCodes, kFlemingtonCount, code);
}

size_t LSID::rceGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return nmeGetLEDsBetween(kFlemingtonStations, kFlemingtonCodes, kFlemingtonCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kSunburyStations[] = {
    &kSunburySUY, &kSunburyDRT, &kSunburyWGS, &kSunburyKPL, &kSunburySAB, &kSunburyGIN, &kSunburyALB,
    &kSunburySUN, &kSunburyTOT, &kSunburyWFY, &kSunburyMFY, &kSunburyFSY,
    &kNorthernNME
};
static const infraid_t kSunburyCodes[] = { 
    INFRAID_SUY, INFRAID_DRT, INFRAID_WGS, INFRAID_KPL, INFRAID_SAB, INFRAID_GIN, INFRAID_ALB,
    INFRAID_SUN, INFRAID_TOT, INFRAID_WFY, INFRAID_MFY, INFRAID_FSY,
    INFRAID_NME
};
static const size_t kSunburyCount = sizeof(kSunburyStations) / sizeof(station_t*);

uint16_t LSID::suyGetLED(infraid_t code) {
    if (isCityStation(code)) return nmeCityGetLED(code);
    return getLEDStub(kSunburyStations, kSunburyCodes, kSunburyCount, code);
}

size_t LSID::suyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return nmeGetLEDsBetween(kSunburyStations, kSunburyCodes, kSunburyCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kUpfieldStations[] = {
    &kUpfieldUFD, &kUpfieldGOW, &kUpfieldFAK, &kUpfieldMYN, &kUpfieldBAT, &kUpfieldCOB, &kUpfieldMLD,
    &kUpfieldASY, &kUpfieldBWK, &kUpfieldJWL, &kUpfieldRPK, &kUpfieldFBD, &kUpfieldMAC,
    &kNorthernNME
};
static const infraid_t kUpfieldCodes[] = { 
    INFRAID_UFD, INFRAID_GOW, INFRAID_FAK, INFRAID_MYN, INFRAID_BAT, INFRAID_COB, INFRAID_MLD,
    INFRAID_ASY, INFRAID_BWK, INFRAID_JWL, INFRAID_RPK, INFRAID_FBD, INFRAID_MAC,
    INFRAID_NME
};
static const size_t kUpfieldCount = sizeof(kUpfieldStations) / sizeof(station_t*);

uint16_t LSID::ufdGetLED(infraid_t code) {
    if (isCityStation(code)) return nmeCityGetLED(code);
    return getLEDStub(kUpfieldStations, kUpfieldCodes, kUpfieldCount, code);
}

size_t LSID::ufdGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return nmeGetLEDsBetween(kUpfieldStations, kUpfieldCodes, kUpfieldCount, fromCode, toCode, buffer, maxLength);
}

