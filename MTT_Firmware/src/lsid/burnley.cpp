#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Belgrave line */
static const station_t kBelgraveBEG = { LMAT_BELGRAVE_BEG, LMAT_BELGRAVE_BEG_ALT };
static const station_t kBelgraveTCM = { LMAT_BELGRAVE_TCM, LMAT_BELGRAVE_TCM_ALT };
static const station_t kBelgraveUPW = { LMAT_BELGRAVE_UPW, LMAT_BELGRAVE_UPW_ALT };
static const station_t kBelgraveUFG = { LMAT_BELGRAVE_UFG, LMAT_BELGRAVE_UFG_ALT };
static const station_t kBelgraveFTG = { LMAT_BELGRAVE_FTG, LMAT_BELGRAVE_FTG_ALT };
static const station_t kBelgraveBOR = { LMAT_BELGRAVE_BOR, LMAT_BELGRAVE_BOR_ALT };
static const station_t kBelgraveBAY = { LMAT_BELGRAVE_BAY, LMAT_BELGRAVE_BAY_ALT };
static const station_t kBelgraveHMT = { LMAT_BELGRAVE_HMT, LMAT_BELGRAVE_HMT_ALT };

/* Lilydale line */
static const station_t kLilydaleLIL = { LMAT_LILYDALE_LIL, LMAT_LILYDALE_LIL_ALT };
static const station_t kLilydaleMLK = { LMAT_LILYDALE_MLK, LMAT_LILYDALE_MLK_ALT };
static const station_t kLilydaleCDN = { LMAT_LILYDALE_CDN, LMAT_LILYDALE_CDN_ALT };
static const station_t kLilydaleRWE = { LMAT_LILYDALE_RWE, LMAT_LILYDALE_RWE_ALT };

/* Ringwood shared line */
static const station_t kRingwoodRWD = { LMAT_RINGWOOD_RWD, LMAT_RINGWOOD_RWD_ALT };
static const station_t kRingwoodHTD = { LMAT_RINGWOOD_HTD, LMAT_RINGWOOD_HTD_ALT };
static const station_t kRingwoodMCH = { LMAT_RINGWOOD_MCH, LMAT_RINGWOOD_MCH_ALT };
static const station_t kRingwoodNWG = { LMAT_RINGWOOD_NWG, LMAT_RINGWOOD_NWG_ALT };
static const station_t kRingwoodBBN = { LMAT_RINGWOOD_BBN, LMAT_RINGWOOD_BBN_ALT };
static const station_t kRingwoodLAB = { LMAT_RINGWOOD_LAB, LMAT_RINGWOOD_LAB_ALT };
static const station_t kRingwoodBOX = { LMAT_RINGWOOD_BOX, LMAT_RINGWOOD_BOX_ALT };
static const station_t kRingwoodUNN = { LMAT_RINGWOOD_UNN, LMAT_RINGWOOD_UNN_ALT };
static const station_t kRingwoodCHM = { LMAT_RINGWOOD_CHM, LMAT_RINGWOOD_CHM_ALT };
static const station_t kRingwoodCBY = { LMAT_RINGWOOD_CBY, LMAT_RINGWOOD_CBY_ALT };
static const station_t kRingwoodECM = { LMAT_RINGWOOD_ECM, LMAT_RINGWOOD_ECM_ALT };

/* Alamein line */
static const station_t kAlameinALM = { LMAT_ALAMEIN_ALM, LMAT_ALAMEIN_ALM_ALT };
static const station_t kAlameinASH = { LMAT_ALAMEIN_ASH, LMAT_ALAMEIN_ASH_ALT };
static const station_t kAlameinBWD = { LMAT_ALAMEIN_BWD, LMAT_ALAMEIN_BWD_ALT };
static const station_t kAlameinHWL = { LMAT_ALAMEIN_HWL, LMAT_ALAMEIN_HWL_ALT };
static const station_t kAlameinWSN = { LMAT_ALAMEIN_WSN, LMAT_ALAMEIN_WSN_ALT };
static const station_t kAlameinRIV = { LMAT_ALAMEIN_RIV, LMAT_ALAMEIN_RIV_ALT };

/* Camberwell shared line */
static const station_t kCamberwellCAM = { LMAT_CAMBERWELL_CAM, LMAT_CAMBERWELL_CAM_ALT };
static const station_t kCamberwellAUB = { LMAT_CAMBERWELL_AUB, LMAT_CAMBERWELL_AUB_ALT };
static const station_t kCamberwellGFE = { LMAT_CAMBERWELL_GFE, LMAT_CAMBERWELL_GFE_ALT };
static const station_t kCamberwellHAW = { LMAT_CAMBERWELL_HAW, LMAT_CAMBERWELL_HAW_ALT };

/* Glen Waverley line */
static const station_t kWaverleyGWY = { LMAT_WAVERLEY_GWY, LMAT_WAVERLEY_GWY_ALT };
static const station_t kWaverleySYN = { LMAT_WAVERLEY_SYN, LMAT_WAVERLEY_SYN_ALT };
static const station_t kWaverleyMWY = { LMAT_WAVERLEY_MWY, LMAT_WAVERLEY_MWY_ALT };
static const station_t kWaverleyJOR = { LMAT_WAVERLEY_JOR, LMAT_WAVERLEY_JOR_ALT };
static const station_t kWaverleyHOL = { LMAT_WAVERLEY_HOL, LMAT_WAVERLEY_HOL_ALT };
static const station_t kWaverleyEMV = { LMAT_WAVERLEY_EMV, LMAT_WAVERLEY_EMV_ALT };
static const station_t kWaverleyDLG = { LMAT_WAVERLEY_DLG, LMAT_WAVERLEY_DLG_ALT };
static const station_t kWaverleyGIR = { LMAT_WAVERLEY_GIR, LMAT_WAVERLEY_GIR_ALT };
static const station_t kWaverleyGAR = { LMAT_WAVERLEY_GAR, LMAT_WAVERLEY_GAR_ALT };
static const station_t kWaverleyTGA = { LMAT_WAVERLEY_TGA, LMAT_WAVERLEY_TGA_ALT };
static const station_t kWaverleyKYG = { LMAT_WAVERLEY_KYG, LMAT_WAVERLEY_KYG_ALT };
static const station_t kWaverleyHEY = { LMAT_WAVERLEY_HEY, LMAT_WAVERLEY_HEY_ALT };

/* Burnley group shared line */
static const station_t kBurnleyBLY = { LMAT_BURNLEY_BLY, LMAT_BURNLEY_BLY_ALT };
static const station_t kBurnleyERM = { LMAT_BURNLEY_ERM, LMAT_BURNLEY_ERM_ALT };
static const station_t kBurnleyRMD = { LMAT_BURNLEY_RMD, LMAT_BURNLEY_RMD_ALT };

/* Burnley city stations - counterclockwise (via City Loop in the Up direction) */
static const station_t kBurnleyPAR_CCW = { LMAT_BURNLEY_PAR, LMAT_BURNLEY_PAR_ALT };
static const station_t kBurnleyMCE_CCW = { LMAT_BURNLEY_MCE, LMAT_BURNLEY_MCE_ALT };
static const station_t kBurnleyFGS_CCW = { LMAT_BURNLEY_FGS, LMAT_BURNLEY_FGS_ALT };
static const station_t kBurnleySSS_CCW = { LMAT_BURNLEY_SSS, LMAT_BURNLEY_SSS_ALT };
static const station_t kBurnleyFSS_CCW = { LMAT_BURNLEY_FSS, LMAT_NULL };

/* Burnley city stations - clockwise (direct to Flinders St in the Up direction) */
static const station_t kBurnleyFSS_CW = { LMAT_BURNLEY_FSS, LMAT_BURNLEY_SSS_ALT };
static const station_t kBurnleySSS_CW = { LMAT_BURNLEY_SSS, LMAT_BURNLEY_FGS_ALT };
static const station_t kBurnleyFGS_CW = { LMAT_BURNLEY_FGS, LMAT_BURNLEY_MCE_ALT };
static const station_t kBurnleyMCE_CW = { LMAT_BURNLEY_MCE, LMAT_BURNLEY_PAR_ALT };
static const station_t kBurnleyPAR_CW = { LMAT_BURNLEY_PAR, LMAT_NULL };

static const station_t* kBurnleyCityCCWStations[] = {
    &kBurnleyPAR_CCW, &kBurnleyMCE_CCW, &kBurnleyFGS_CCW, &kBurnleySSS_CCW, &kBurnleyFSS_CCW
};
static const infraid_t kBurnleyCityCCWCodes[] = {
    INFRAID_PAR, INFRAID_MCE, INFRAID_FGS, INFRAID_SSS, INFRAID_FSS
};

static const station_t* kBurnleyCityCWStations[] = {
    &kBurnleyFSS_CW, &kBurnleySSS_CW, &kBurnleyFGS_CW, &kBurnleyMCE_CW, &kBurnleyPAR_CW
};
static const infraid_t kBurnleyCityCWCodes[] = {
    INFRAID_FSS, INFRAID_SSS, INFRAID_FGS, INFRAID_MCE, INFRAID_PAR
};

uint16_t LSID::blyCityGetLED(infraid_t code) {
    switch (code) {
        case INFRAID_FSS: return LMAT_BURNLEY_FSS;
        case INFRAID_SSS: return LMAT_BURNLEY_SSS;
        case INFRAID_FGS: return LMAT_BURNLEY_FGS;
        case INFRAID_MCE: return LMAT_BURNLEY_MCE;
        case INFRAID_PAR: return LMAT_BURNLEY_PAR;
        default:
            ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
            return LMAT_NULL;
    }
}

static const station_t* kBelgraveStations[] = {
    &kBelgraveBEG, &kBelgraveTCM, &kBelgraveUPW, &kBelgraveUFG, &kBelgraveFTG, &kBelgraveBOR, &kBelgraveBAY, &kBelgraveHMT,
    &kRingwoodRWD, &kRingwoodHTD, &kRingwoodMCH, &kRingwoodNWG, &kRingwoodBBN, &kRingwoodLAB, &kRingwoodBOX, &kRingwoodUNN, &kRingwoodCHM, &kRingwoodCBY, &kRingwoodECM,
    &kCamberwellCAM, &kCamberwellAUB, &kCamberwellGFE, &kCamberwellHAW,
    &kBurnleyBLY, &kBurnleyERM, &kBurnleyRMD
};
static const infraid_t kBelgraveCodes[] = { 
    INFRAID_BEG, INFRAID_TCM, INFRAID_UPW, INFRAID_UFG, INFRAID_FTG, INFRAID_BOR, INFRAID_BAY, INFRAID_HMT,
    INFRAID_RWD, INFRAID_HTD, INFRAID_MCH, INFRAID_NWG, INFRAID_BBN, INFRAID_LAB, INFRAID_BOX, INFRAID_UNN, INFRAID_CHM, INFRAID_CBY, INFRAID_ECM,
    INFRAID_CAM, INFRAID_AUB, INFRAID_GFE, INFRAID_HAW,
    INFRAID_BLY, INFRAID_ERM, INFRAID_RMD
};
static const size_t kBelgraveCount = sizeof(kBelgraveStations) / sizeof(station_t*);

uint16_t LSID::begGetLED(infraid_t code) {
    if (isCityStation(code)) return blyCityGetLED(code);
    return getLEDStub(kBelgraveStations, kBelgraveCodes, kBelgraveCount, code);
}

size_t LSID::begGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kBurnleyCityCCWCodes, kBurnleyCityCCWStations, kBurnleyCityCWStations,
        kBelgraveStations, kBelgraveCodes, kBelgraveCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kLilydaleStations[] = {
    &kLilydaleLIL, &kLilydaleMLK, &kLilydaleCDN, &kLilydaleRWE,
    &kRingwoodRWD, &kRingwoodHTD, &kRingwoodMCH, &kRingwoodNWG, &kRingwoodBBN, &kRingwoodLAB, &kRingwoodBOX, &kRingwoodUNN, &kRingwoodCHM, &kRingwoodCBY, &kRingwoodECM,
    &kCamberwellCAM, &kCamberwellAUB, &kCamberwellGFE, &kCamberwellHAW,
    &kBurnleyBLY, &kBurnleyERM, &kBurnleyRMD
};
static const infraid_t kLilydaleCodes[] = {
    INFRAID_LIL, INFRAID_MLK, INFRAID_CDN, INFRAID_RWE,
    INFRAID_RWD, INFRAID_HTD, INFRAID_MCH, INFRAID_NWG, INFRAID_BBN, INFRAID_LAB, INFRAID_BOX, INFRAID_UNN, INFRAID_CHM, INFRAID_CBY, INFRAID_ECM,
    INFRAID_CAM, INFRAID_AUB, INFRAID_GFE, INFRAID_HAW,
    INFRAID_BLY, INFRAID_ERM, INFRAID_RMD
};
static const size_t kLilydaleCount = sizeof(kLilydaleStations) / sizeof(station_t*);

uint16_t LSID::lilGetLED(infraid_t code) {
    if (isCityStation(code)) return blyCityGetLED(code);
    return getLEDStub(kLilydaleStations, kLilydaleCodes, kLilydaleCount, code);
}

size_t LSID::lilGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kBurnleyCityCCWCodes, kBurnleyCityCCWStations, kBurnleyCityCWStations,
        kLilydaleStations, kLilydaleCodes, kLilydaleCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kAlameinStations[] = {
    &kAlameinALM, &kAlameinASH, &kAlameinBWD, &kAlameinHWL, &kAlameinWSN, &kAlameinRIV,
    &kCamberwellCAM, &kCamberwellAUB, &kCamberwellGFE, &kCamberwellHAW,
    &kBurnleyBLY, &kBurnleyERM, &kBurnleyRMD
};
static const infraid_t kAlameinCodes[] = {
    INFRAID_ALM, INFRAID_ASH, INFRAID_BWD, INFRAID_HWL, INFRAID_WSN, INFRAID_RIV,
    INFRAID_CAM, INFRAID_AUB, INFRAID_GFE, INFRAID_HAW,
    INFRAID_BLY, INFRAID_ERM, INFRAID_RMD
};
static const size_t kAlameinCount = sizeof(kAlameinStations) / sizeof(station_t*);

uint16_t LSID::almGetLED(infraid_t code) {
    if (isCityStation(code)) return blyCityGetLED(code);
    return getLEDStub(kAlameinStations, kAlameinCodes, kAlameinCount, code);
}

size_t LSID::almGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kBurnleyCityCCWCodes, kBurnleyCityCCWStations, kBurnleyCityCWStations,
        kAlameinStations, kAlameinCodes, kAlameinCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kWaverleyStations[] = {
    &kWaverleyGWY, &kWaverleySYN, &kWaverleyMWY, &kWaverleyJOR, &kWaverleyHOL, &kWaverleyEMV, &kWaverleyDLG, &kWaverleyGIR, &kWaverleyGAR, &kWaverleyTGA, &kWaverleyKYG, &kWaverleyHEY,
    &kBurnleyBLY, &kBurnleyERM, &kBurnleyRMD
};
static const infraid_t kWaverleyCodes[] = {
    INFRAID_GWY, INFRAID_SYN, INFRAID_MWY, INFRAID_JOR, INFRAID_HOL, INFRAID_EMV, INFRAID_DLG, INFRAID_GIR, INFRAID_GAR, INFRAID_TGA, INFRAID_KYG, INFRAID_HEY,
    INFRAID_BLY, INFRAID_ERM, INFRAID_RMD
};
static const size_t kWaverleyCount = sizeof(kWaverleyStations) / sizeof(station_t*);

uint16_t LSID::gwyGetLED(infraid_t code) {
    if (isCityStation(code)) return blyCityGetLED(code);
    return getLEDStub(kWaverleyStations, kWaverleyCodes, kWaverleyCount, code);
}

size_t LSID::gwyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return rmdGetLEDsBetween(
        kBurnleyCityCCWCodes, kBurnleyCityCCWStations, kBurnleyCityCWStations,
        kWaverleyStations, kWaverleyCodes, kWaverleyCount,
        fromCode, toCode, buffer, maxLength
    );
}
