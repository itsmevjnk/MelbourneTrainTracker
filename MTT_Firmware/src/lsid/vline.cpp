#include "lsid.h"
#include "hwconfig/led_matrix.h"

/* Melton line */
static const station_t kMeltonMEL = { LMAT_VLINE_MEL, LMAT_VLINE_MEL_ALT };
static const station_t kMeltonTLN = { LMAT_VLINE_TLN, LMAT_VLINE_TLN_ALT };
static const station_t kMeltonRBK = { LMAT_VLINE_RBK, LMAT_VLINE_RBK_ALT };
static const station_t kMeltonCPS = { LMAT_VLINE_CPS, LMAT_VLINE_CPS_ALT };

/* Wyndham Vale line */
static const station_t kWyndhamValeWVL = { LMAT_VLINE_WVL, LMAT_VLINE_WVL_ALT };
static const station_t kWyndhamValeDAV = { LMAT_VLINE_DAV, LMAT_VLINE_DAV_ALT };
static const station_t kWyndhamValeTNT = { LMAT_VLINE_TNT, LMAT_VLINE_TNT_ALT };

/* Western V/Line shared track */
static const station_t kVLineWestDEK = { LMAT_VLINE_DEK, LMAT_VLINE_DEK_ALT };
static const station_t kVLineWestADR = { LMAT_VLINE_ADR, LMAT_VLINE_ADR_ALT };
static const station_t kVLineWestSUN = { LMAT_VLINE_SUN, LMAT_VLINE_SUN_ALT };
static const station_t kVLineWestFSY = { LMAT_VLINE_FSY, LMAT_VLINE_FSY_ALT };

static const station_t kVLineSSS = { LMAT_VLINE_SSS, LMAT_NULL };

/* Gippsland line */
static const station_t kGippslandPKM = { LMAT_VLINE_PKM, LMAT_VLINE_PKM_ALT };
static const station_t kGippslandBEW = { LMAT_VLINE_BEW, LMAT_VLINE_BEW_ALT };
static const station_t kGippslandDNG = { LMAT_VLINE_DNG, LMAT_VLINE_DNG_ALT };
static const station_t kGippslandCLA = { LMAT_VLINE_CLA, LMAT_VLINE_CLA_ALT };
static const station_t kGippslandCFD = { LMAT_VLINE_CFD, LMAT_VLINE_CFD_ALT };
static const station_t kGippslandRMD = { LMAT_VLINE_RMD, LMAT_VLINE_RMD_ALT };
static const station_t kGippslandFSS = { LMAT_VLINE_FSS, LMAT_VLINE_SSS_ALT };

static const station_t* kMeltonStations[] = {
    &kMeltonMEL, &kMeltonTLN, &kMeltonRBK, &kMeltonCPS,
    &kVLineWestDEK, &kVLineWestADR, &kVLineWestSUN, &kVLineWestFSY, &kVLineSSS
};
static const infraid_t kMeltonCodes[] = {
    INFRAID_MEL, INFRAID_TLN, INFRAID_RBK, INFRAID_CPS,
    INFRAID_DEK, INFRAID_ADR, INFRAID_SUN, INFRAID_FSY, INFRAID_SSS
};
static const size_t kMeltonCount = sizeof(kMeltonStations) / sizeof(station_t*);

uint16_t LSID::melGetLED(infraid_t code) {
    return getLEDStub(kMeltonStations, kMeltonCodes, kMeltonCount, code);    
}

size_t LSID::melGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kMeltonStations, kMeltonCodes, kMeltonCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kWyndhamValeStations[] = {
    &kWyndhamValeWVL, &kWyndhamValeDAV, &kWyndhamValeTNT,
    &kVLineWestDEK, &kVLineWestADR, &kVLineWestSUN, &kVLineWestFSY, &kVLineSSS
};
static const infraid_t kWyndhamValeCodes[] = {
    INFRAID_WVL, INFRAID_DAV, INFRAID_TNT,
    INFRAID_DEK, INFRAID_ADR, INFRAID_SUN, INFRAID_FSY, INFRAID_SSS
};
static const size_t kWyndhamValeCount = sizeof(kWyndhamValeStations) / sizeof(station_t*);

uint16_t LSID::wvlGetLED(infraid_t code) {
    return getLEDStub(kWyndhamValeStations, kWyndhamValeCodes, kWyndhamValeCount, code);
}

size_t LSID::wvlGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kWyndhamValeStations, kWyndhamValeCodes, kWyndhamValeCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kGippslandStations[] = {
    &kGippslandPKM, &kGippslandBEW, &kGippslandDNG, &kGippslandCLA, 
    &kGippslandCFD, &kGippslandRMD, &kGippslandFSS, &kVLineSSS
};
static const infraid_t kGippslandCodes[] = {
    INFRAID_PKM, INFRAID_BEW, INFRAID_DNG, INFRAID_CLA, 
    INFRAID_CFD, INFRAID_RMD, INFRAID_FSS, INFRAID_SSS
};
static const size_t kGippslandCount = sizeof(kGippslandStations) / sizeof(station_t*);

uint16_t LSID::gplGetLED(infraid_t code) {
    return getLEDStub(kGippslandStations, kGippslandCodes, kGippslandCount, code);
}

size_t LSID::gplGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kGippslandStations, kGippslandCodes, kGippslandCount, fromCode, toCode, buffer, maxLength);
}
