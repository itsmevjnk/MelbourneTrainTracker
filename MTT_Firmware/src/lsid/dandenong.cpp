#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Pakenham line */
static const station_t kPakenhamEPH = { LMAT_PAKENHAM_EPH, LMAT_PAKENHAM_EPH_ALT };
static const station_t kPakenhamPKM = { LMAT_PAKENHAM_PKM, LMAT_PAKENHAM_PKM_ALT };
static const station_t kPakenhamCDA = { LMAT_PAKENHAM_CDA, LMAT_PAKENHAM_CDA_ALT };
static const station_t kPakenhamOFC = { LMAT_PAKENHAM_OFC, LMAT_PAKENHAM_OFC_ALT };
static const station_t kPakenhamBFD = { LMAT_PAKENHAM_BFD, LMAT_PAKENHAM_BFD_ALT };
static const station_t kPakenhamBEW = { LMAT_PAKENHAM_BEW, LMAT_PAKENHAM_BEW_ALT };
static const station_t kPakenhamNWA = { LMAT_PAKENHAM_NWA, LMAT_PAKENHAM_NWA_ALT };
static const station_t kPakenhamHLM = { LMAT_PAKENHAM_HLM, LMAT_PAKENHAM_HLM_ALT };

/* Cranbourne line */
static const station_t kCranbourneCBE = { LMAT_CRANBOURNE_CBE, LMAT_CRANBOURNE_CBE_ALT };
static const station_t kCranbourneMPK = { LMAT_CRANBOURNE_MPK, LMAT_CRANBOURNE_MPK_ALT };
static const station_t kCranbourneLBK = { LMAT_CRANBOURNE_LBK, LMAT_CRANBOURNE_LBK_ALT };

/* Dandenong shared track */
static const station_t kDandenongDNG = { LMAT_DANDENONG_DNG, LMAT_DANDENONG_DNG_ALT };
static const station_t kDandenongYMN = { LMAT_DANDENONG_YMN, LMAT_DANDENONG_YMN_ALT };
static const station_t kDandenongNPK = { LMAT_DANDENONG_NPK, LMAT_DANDENONG_NPK_ALT };
static const station_t kDandenongSNP = { LMAT_DANDENONG_SNP, LMAT_DANDENONG_SNP_ALT };
static const station_t kDandenongSPG = { LMAT_DANDENONG_SPG, LMAT_DANDENONG_SPG_ALT };
static const station_t kDandenongWTL = { LMAT_DANDENONG_WTL, LMAT_DANDENONG_WTL_ALT };
static const station_t kDandenongCLA = { LMAT_DANDENONG_CLA, LMAT_DANDENONG_CLA_ALT };
static const station_t kDandenongHUN = { LMAT_DANDENONG_HUN, LMAT_DANDENONG_HUN_ALT };
static const station_t kDandenongOAK = { LMAT_DANDENONG_OAK, LMAT_DANDENONG_OAK_ALT };
static const station_t kDandenongHUG = { LMAT_DANDENONG_HUG, LMAT_DANDENONG_HUG_ALT };
static const station_t kDandenongMRB = { LMAT_DANDENONG_MRB, LMAT_DANDENONG_MRB_ALT };
static const station_t kDandenongCNE = { LMAT_DANDENONG_CNE, LMAT_DANDENONG_CNE_ALT };
static const station_t kDandenongCFD = { LMAT_DANDENONG_CFD, LMAT_DANDENONG_CFD_ALT };
// NOTE: apparently some services terminate at Malvern???????
static const station_t kDandenongSYR = { LMAT_DANDENONG_SYR, LMAT_DANDENONG_SYR_ALT };
static const station_t kDandenongRMD = { LMAT_DANDENONG_RMD, LMAT_DANDENONG_RMD_ALT };

/* Dandenong city stations - counterclockwise (via City Loop in the Up direction) */
static const station_t kDandenongPAR_CCW = { LMAT_DANDENONG_PAR, LMAT_DANDENONG_PAR_ALT };
static const station_t kDandenongMCE_CCW = { LMAT_DANDENONG_MCE, LMAT_DANDENONG_MCE_ALT };
static const station_t kDandenongFGS_CCW = { LMAT_DANDENONG_FGS, LMAT_DANDENONG_FGS_ALT };
static const station_t kDandenongSSS_CCW = { LMAT_DANDENONG_SSS, LMAT_DANDENONG_SSS_ALT };
static const station_t kDandenongFSS_CCW = { LMAT_DANDENONG_FSS, LMAT_NULL };

/* Dandenong city stations - clockwise (direct to Flinders St in the Up direction) */
static const station_t kDandenongFSS_CW = { LMAT_DANDENONG_FSS, LMAT_DANDENONG_SSS_ALT };
static const station_t kDandenongSSS_CW = { LMAT_DANDENONG_SSS, LMAT_DANDENONG_FGS_ALT };
static const station_t kDandenongFGS_CW = { LMAT_DANDENONG_FGS, LMAT_DANDENONG_MCE_ALT };
static const station_t kDandenongMCE_CW = { LMAT_DANDENONG_MCE, LMAT_DANDENONG_PAR_ALT };
static const station_t kDandenongPAR_CW = { LMAT_DANDENONG_PAR, LMAT_NULL };

static const station_t* kDandenongCityCCWStations[] = {
    &kDandenongPAR_CCW, &kDandenongMCE_CCW, &kDandenongFGS_CCW, &kDandenongSSS_CCW, &kDandenongFSS_CCW
};
static const infraid_t kDandenongCityCCWCodes[] = {
    INFRAID_PAR, INFRAID_MCE, INFRAID_FGS, INFRAID_SSS, INFRAID_FSS
};

static const station_t* kDandenongCityCWStations[] = {
    &kDandenongFSS_CW, &kDandenongSSS_CW, &kDandenongFGS_CW, &kDandenongMCE_CW, &kDandenongPAR_CW
};
// static const infraid_t kDandenongCityCWCodes[] = {
//     INFRAID_FSS, INFRAID_SSS, INFRAID_FGS, INFRAID_MCE, INFRAID_PAR
// };

uint16_t LSID::dngCityGetLED(infraid_t code) {
    switch (code) {
        case INFRAID_FSS: return LMAT_DANDENONG_FSS;
        case INFRAID_SSS: return LMAT_DANDENONG_SSS;
        case INFRAID_FGS: return LMAT_DANDENONG_FGS;
        case INFRAID_MCE: return LMAT_DANDENONG_MCE;
        case INFRAID_PAR: return LMAT_DANDENONG_PAR;
        default:
            ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
            return LMAT_NULL;
    }
}
static const station_t* kPakenhamStations[] = {
    &kPakenhamEPH, &kPakenhamPKM, &kPakenhamCDA, &kPakenhamOFC, &kPakenhamBFD, &kPakenhamBEW, &kPakenhamNWA, &kPakenhamHLM,
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD, &kDandenongSYR, &kDandenongRMD
};
static const infraid_t kPakenhamCodes[] = { 
    INFRAID_EPH, INFRAID_PKM, INFRAID_CDA, INFRAID_OFC, INFRAID_BFD, INFRAID_BEW, INFRAID_NWA, INFRAID_HLM,
    INFRAID_DNG, INFRAID_YMN, INFRAID_NPK, INFRAID_SNP, INFRAID_SPG, INFRAID_WTL, INFRAID_CLA, INFRAID_HUN, INFRAID_OAK, INFRAID_HUG, INFRAID_MRB, INFRAID_CNE, INFRAID_CFD, INFRAID_SYR, INFRAID_RMD
};
static const size_t kPakenhamCount = sizeof(kPakenhamStations) / sizeof(station_t*);

uint16_t LSID::pkmGetLED(infraid_t code) {
    if (isCityStation(code)) return dngCityGetLED(code);
    return getLEDStub(kPakenhamStations, kPakenhamCodes, kPakenhamCount, code);
}

size_t LSID::pkmGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations,
        kPakenhamStations, kPakenhamCodes, kPakenhamCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kCranbourneStations[] = {
    &kCranbourneCBE, &kCranbourneMPK, &kCranbourneLBK,
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD, &kDandenongSYR, &kDandenongRMD
};
static const infraid_t kCranbourneCodes[] = { 
    INFRAID_CBE, INFRAID_MPK, INFRAID_LBK,
    INFRAID_DNG, INFRAID_YMN, INFRAID_NPK, INFRAID_SNP, INFRAID_SPG, INFRAID_WTL, INFRAID_CLA, INFRAID_HUN, INFRAID_OAK, INFRAID_HUG, INFRAID_MRB, INFRAID_CNE, INFRAID_CFD, INFRAID_SYR, INFRAID_RMD
};
static const size_t kCranbourneCount = sizeof(kCranbourneStations) / sizeof(station_t*);

uint16_t LSID::cbeGetLED(infraid_t code) {
    if (isCityStation(code)) return dngCityGetLED(code);
    return getLEDStub(kCranbourneStations, kCranbourneCodes, kCranbourneCount, code);
}

size_t LSID::cbeGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations,
        kCranbourneStations, kCranbourneCodes, kCranbourneCount,
        fromCode, toCode, buffer, maxLength
    );
}