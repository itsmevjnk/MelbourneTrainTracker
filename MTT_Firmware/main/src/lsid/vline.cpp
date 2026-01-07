#include "hwconfig/leds/rev2/vline.h"
#include "lsid.h"
#include "hwconfig/led_matrix.h"

/* Ararat line */
#if LMAT_VLINE_HAS_REGIONAL
static const station_t kAraratART = { LMAT_ARARAT_ART, LMAT_ARARAT_ART_ALT };
static const station_t kAraratBET = { LMAT_ARARAT_BET, LMAT_ARARAT_BET_ALT };
static const station_t kAraratWED = { LMAT_ARARAT_WED, LMAT_ARARAT_WED_ALT };
#endif

/* Maryborough line */
#if LMAT_VLINE_HAS_REGIONAL
static const station_t kMaryboroughMBY = { LMAT_MARYBOROUGH_MBY, LMAT_MARYBOROUGH_MBY_ALT };
static const station_t kMaryboroughTAT = { LMAT_MARYBOROUGH_TAT, LMAT_MARYBOROUGH_TAT_ALT };
static const station_t kMaryboroughCLU = { LMAT_MARYBOROUGH_CLU, LMAT_MARYBOROUGH_CLU_ALT };
static const station_t kMaryboroughCWK = { LMAT_MARYBOROUGH_CWK, LMAT_MARYBOROUGH_CWK_ALT };
#endif

/* Ballarat shared track */
#if LMAT_VLINE_HAS_REGIONAL
static const station_t kBallaratBATV = { LMAT_BALLARAT_BATV, LMAT_BALLARAT_BATV_ALT };
static const station_t kBallaratBLN = { LMAT_BALLARAT_BLN, LMAT_BALLARAT_BLN_ALT };
static const station_t kBallaratBAH = { LMAT_BALLARAT_BAH, LMAT_BALLARAT_BAH_ALT };
#endif
static const station_t kBallaratMEL = { LMAT_BALLARAT_MEL, LMAT_BALLARAT_MEL_ALT }; // metropolitan Melton line starts here
static const station_t kBallaratTLN = { LMAT_BALLARAT_TLN, LMAT_BALLARAT_TLN_ALT };
static const station_t kBallaratRBK = { LMAT_BALLARAT_RBK, LMAT_BALLARAT_RBK_ALT };
static const station_t kBallaratRVH = { LMAT_BALLARAT_RVH, LMAT_BALLARAT_RVH_ALT };

/* Geelong/Warrnambool line */
#if LMAT_VLINE_HAS_REGIONAL
static const station_t kGeelongWBL = { LMAT_GEELONG_WBL, LMAT_GEELONG_WBL_ALT };
static const station_t kGeelongSDP = { LMAT_GEELONG_SDP, LMAT_GEELONG_SDP_ALT };
static const station_t kGeelongTER = { LMAT_GEELONG_TER, LMAT_GEELONG_TER_ALT };
static const station_t kGeelongCPD = { LMAT_GEELONG_CPD, LMAT_GEELONG_CPD_ALT };
static const station_t kGeelongCOL = { LMAT_GEELONG_COL, LMAT_GEELONG_COL_ALT };
static const station_t kGeelongBGE = { LMAT_GEELONG_BGE, LMAT_GEELONG_BGE_ALT };
static const station_t kGeelongWIA = { LMAT_GEELONG_WIA, LMAT_GEELONG_WIA_ALT };
static const station_t kGeelongWPP = { LMAT_GEELONG_WPP, LMAT_GEELONG_WPP_ALT };
static const station_t kGeelongMAS = { LMAT_GEELONG_MAS, LMAT_GEELONG_MAS_ALT };
static const station_t kGeelongSOG = { LMAT_GEELONG_SOG, LMAT_GEELONG_SOG_ALT };
static const station_t kGeelongGEL = { LMAT_GEELONG_GEL, LMAT_GEELONG_GEL_ALT };
static const station_t kGeelongNGL = { LMAT_GEELONG_NGL, LMAT_GEELONG_NGL_ALT };
static const station_t kGeelongNSH = { LMAT_GEELONG_NSH, LMAT_GEELONG_NSH_ALT };
static const station_t kGeelongCOR = { LMAT_GEELONG_COR, LMAT_GEELONG_COR_ALT };
static const station_t kGeelongLRA = { LMAT_GEELONG_LRA, LMAT_GEELONG_LRA_ALT };
static const station_t kGeelongLRR = { LMAT_GEELONG_LRR, LMAT_GEELONG_LRR_ALT };
#endif
static const station_t kGeelongWVL = { LMAT_GEELONG_WVL, LMAT_GEELONG_WVL_ALT }; // metropolitan Wyndham Vale line starts here
static const station_t kGeelongDAV = { LMAT_GEELONG_DAV, LMAT_GEELONG_DAV_ALT };
static const station_t kGeelongTNT = { LMAT_GEELONG_TNT, LMAT_GEELONG_TNT_ALT };

/* Western V/Line shared track */
static const station_t kVLineWestDEK = { LMAT_VLWEST_DEK, LMAT_VLWEST_DEK_ALT };
static const station_t kVLineWestARR = { LMAT_VLWEST_ARR, LMAT_VLWEST_ARR_ALT };
static const station_t kVLineWestSUN = { LMAT_VLWEST_SUN, LMAT_VLWEST_SUN_ALT };
static const station_t kVLineWestFSY = { LMAT_VLWEST_FSY, LMAT_VLWEST_FSY_ALT };

static const station_t kVLineSSS = { LMAT_VLWEST_SSS, LMAT_NULL };

/* Gippsland line */
static const station_t kGippslandPKM = { LMAT_GIPPSLAND_PKM, LMAT_GIPPSLAND_PKM_ALT };
static const station_t kGippslandBEW = { LMAT_GIPPSLAND_BEW, LMAT_GIPPSLAND_BEW_ALT };
static const station_t kGippslandDNG = { LMAT_GIPPSLAND_DNG, LMAT_GIPPSLAND_DNG_ALT };
static const station_t kGippslandCLA = { LMAT_GIPPSLAND_CLA, LMAT_GIPPSLAND_CLA_ALT };
static const station_t kGippslandCFD = { LMAT_GIPPSLAND_CFD, LMAT_GIPPSLAND_CFD_ALT };
static const station_t kGippslandRMD = { LMAT_GIPPSLAND_RMD, LMAT_GIPPSLAND_RMD_ALT };
static const station_t kGippslandFSS = { LMAT_GIPPSLAND_FSS, LMAT_VLWEST_SSS_ALT };
#if LMAT_VLINE_HAS_REGIONAL
static const station_t kGippslandNNG = { LMAT_GIPPSLAND_NNG, LMAT_GIPPSLAND_NNG_ALT };
static const station_t kGippslandTYN = { LMAT_GIPPSLAND_TYN, LMAT_GIPPSLAND_TYN_ALT };
static const station_t kGippslandGARV = { LMAT_GIPPSLAND_GARV, LMAT_GIPPSLAND_GARV_ALT };
static const station_t kGippslandBYP = { LMAT_GIPPSLAND_BYP, LMAT_GIPPSLAND_BYP_ALT };
static const station_t kGippslandLWY = { LMAT_GIPPSLAND_LWY, LMAT_GIPPSLAND_LWY_ALT };
static const station_t kGippslandDRN = { LMAT_GIPPSLAND_DRN, LMAT_GIPPSLAND_DRN_ALT };
static const station_t kGippslandWGL = { LMAT_GIPPSLAND_WGL, LMAT_GIPPSLAND_WGL_ALT };
static const station_t kGippslandYON = { LMAT_GIPPSLAND_YON, LMAT_GIPPSLAND_YON_ALT };
static const station_t kGippslandTAR = { LMAT_GIPPSLAND_TAR, LMAT_GIPPSLAND_TAR_ALT };
static const station_t kGippslandMOE = { LMAT_GIPPSLAND_MOE, LMAT_GIPPSLAND_MOE_ALT };
static const station_t kGippslandMWL = { LMAT_GIPPSLAND_MWL, LMAT_GIPPSLAND_MWL_ALT };
static const station_t kGippslandTRN = { LMAT_GIPPSLAND_TRN, LMAT_GIPPSLAND_TRN_ALT };
static const station_t kGippslandROE = { LMAT_GIPPSLAND_ROE, LMAT_GIPPSLAND_ROE_ALT };
static const station_t kGippslandSAE = { LMAT_GIPPSLAND_SAE, LMAT_GIPPSLAND_SAE_ALT };
static const station_t kGippslandSTD = { LMAT_GIPPSLAND_STD, LMAT_GIPPSLAND_STD_ALT };
static const station_t kGippslandBDE = { LMAT_GIPPSLAND_BDE, LMAT_GIPPSLAND_BDE_ALT };
#endif

static const station_t* kAraratStations[] = {
#if LMAT_VLINE_HAS_REGIONAL
    &kAraratART, &kAraratBET, &kAraratWED,
    &kBallaratBATV, &kBallaratBLN, &kBallaratBAH,
#endif
    &kBallaratMEL, &kBallaratTLN, &kBallaratRBK, &kBallaratRVH,
    &kVLineWestDEK, &kVLineWestARR, &kVLineWestSUN, &kVLineWestFSY, &kVLineSSS
};
static const infraid_t kAraratCodes[] = {
#if LMAT_VLINE_HAS_REGIONAL
    INFRAID_ART, INFRAID_BET, INFRAID_WED,
    INFRAID_BATV, INFRAID_BLN, INFRAID_BAH,
#endif
    INFRAID_MEL, INFRAID_TLN, INFRAID_RBK, INFRAID_RVH,
    INFRAID_DEK, INFRAID_ARR, INFRAID_SUN, INFRAID_FSY, INFRAID_SSS
};
static const size_t kAraratCount = sizeof(kAraratStations) / sizeof(station_t*);

uint16_t LSID::artGetLED(infraid_t code) {
    return getLEDStub(kAraratStations, kAraratCodes, kAraratCount, code);    
}

size_t LSID::artGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kAraratStations, kAraratCodes, kAraratCount, fromCode, toCode, buffer, maxLength);
}

#if LMAT_VLINE_HAS_REGIONAL
static const station_t* kMaryboroughStations[] = {
    &kMaryboroughMBY, &kMaryboroughTAT, &kMaryboroughCLU, &kMaryboroughCWK,
    &kBallaratBATV, &kBallaratBLN, &kBallaratBLN,
    &kBallaratMEL, &kBallaratTLN, &kBallaratRBK, &kBallaratRVH,
    &kVLineWestDEK, &kVLineWestARR, &kVLineWestSUN, &kVLineWestFSY, &kVLineSSS
};
static const infraid_t kMaryboroughCodes[] = {
    INFRAID_MBY, INFRAID_TAT, INFRAID_CLU, INFRAID_CWK,
    INFRAID_BATV, INFRAID_BLN, INFRAID_BAH,
    INFRAID_MEL, INFRAID_TLN, INFRAID_RBK, INFRAID_RVH,
    INFRAID_DEK, INFRAID_ARR, INFRAID_SUN, INFRAID_FSY, INFRAID_SSS
};
static const size_t kMaryboroughCount = sizeof(kMaryboroughStations) / sizeof(station_t*);

uint16_t LSID::mbyGetLED(infraid_t code) {
    return getLEDStub(kMaryboroughStations, kMaryboroughCodes, kMaryboroughCount, code);    
}

size_t LSID::mbyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kMaryboroughStations, kMaryboroughCodes, kMaryboroughCount, fromCode, toCode, buffer, maxLength);
}
#endif

static const station_t* kGeelongStations[] = {
#if LMAT_VLINE_HAS_REGIONAL
    &kGeelongWBL, &kGeelongSDP, &kGeelongTER, &kGeelongCPD, &kGeelongCOL, &kGeelongBGE, &kGeelongWIA, &kGeelongWPP,
    &kGeelongMAS, &kGeelongSOG, &kGeelongGEL, &kGeelongNGL, &kGeelongNSH, &kGeelongCOR, &kGeelongLRA, &kGeelongLRR,
#endif 
    &kGeelongWVL, &kGeelongDAV, &kGeelongTNT,
    &kVLineWestDEK, &kVLineWestARR, &kVLineWestSUN, &kVLineWestFSY, &kVLineSSS
};
static const infraid_t kGeelongCodes[] = {
#if LMAT_VLINE_HAS_REGIONAL
    INFRAID_WBL, INFRAID_SDP, INFRAID_TER, INFRAID_CPD, INFRAID_COL, INFRAID_BGE, INFRAID_WIA, INFRAID_WPP,
    INFRAID_MAS, INFRAID_SOG, INFRAID_GEL, INFRAID_NGL, INFRAID_NSH, INFRAID_COR, INFRAID_LRA, INFRAID_LRR,
#endif 
    INFRAID_WVL, INFRAID_DAV, INFRAID_TNT,
    INFRAID_DEK, INFRAID_ARR, INFRAID_SUN, INFRAID_FSY, INFRAID_SSS
};
static const size_t kGeelongCount = sizeof(kGeelongStations) / sizeof(station_t*);

uint16_t LSID::gelGetLED(infraid_t code) {
    return getLEDStub(kGeelongStations, kGeelongCodes, kGeelongCount, code);
}

size_t LSID::gelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kGeelongStations, kGeelongCodes, kGeelongCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kGippslandStations[] = {
#if LMAT_VLINE_HAS_REGIONAL
    &kGippslandBDE, &kGippslandSTD, &kGippslandSAE, &kGippslandROE, 
    &kGippslandTRN, &kGippslandMWL, &kGippslandMOE, &kGippslandTAR, 
    &kGippslandYON, &kGippslandWGL, &kGippslandDRN, &kGippslandLWY, 
    &kGippslandBYP, &kGippslandGARV, &kGippslandTYN, &kGippslandNNG,
#endif
    &kGippslandPKM, &kGippslandBEW, &kGippslandDNG, &kGippslandCLA, 
    &kGippslandCFD, &kGippslandRMD, &kGippslandFSS, &kVLineSSS
};
static const infraid_t kGippslandCodes[] = {
#if LMAT_VLINE_HAS_REGIONAL
    INFRAID_BDE, INFRAID_STD, INFRAID_SAE, INFRAID_ROE,
    INFRAID_TRN, INFRAID_MWL, INFRAID_MOE, INFRAID_TAR,
    INFRAID_YON, INFRAID_WGL, INFRAID_DRN, INFRAID_LWY,
    INFRAID_BYP, INFRAID_GARV, INFRAID_TYN, INFRAID_NNG,
#endif
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
