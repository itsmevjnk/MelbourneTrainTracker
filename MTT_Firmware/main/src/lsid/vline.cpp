#include "colours.h"
#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include <esp_log.h>
#include <esp_check.h>

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

#if LMAT_VLINE_HAS_BENDIGO

#if LMAT_VLINE_HAS_REGIONAL
/* Swan Hill line */
static const station_t kSwanHillSWL = { LMAT_SWANHILL_SWL, LMAT_SWANHILL_SWL_ALT };
static const station_t kSwanHillKER = { LMAT_SWANHILL_KER, LMAT_SWANHILL_KER_ALT };
static const station_t kSwanHillPYD = { LMAT_SWANHILL_PYD, LMAT_SWANHILL_PYD_ALT };
static const station_t kSwanHillDGE = { LMAT_SWANHILL_DGE, LMAT_SWANHILL_DGE_ALT };
static const station_t kSwanHillRAY = { LMAT_SWANHILL_RAY, LMAT_SWANHILL_RAY_ALT };
static const station_t kSwanHillEAGV = { LMAT_SWANHILL_EAGV, LMAT_SWANHILL_EAGV_ALT };

/* Echuca line */
static const station_t kEchucaECH = { LMAT_ECHUCA_ECH, LMAT_ECHUCA_ECH_ALT };
static const station_t kEchucaROR = { LMAT_ECHUCA_ROR, LMAT_ECHUCA_ROR_ALT };
static const station_t kEchucaEME = { LMAT_ECHUCA_EME, LMAT_ECHUCA_EME_ALT };
static const station_t kEchucaGRG = { LMAT_ECHUCA_GRG, LMAT_ECHUCA_GRG_ALT };
static const station_t kEchucaHUY = { LMAT_ECHUCA_HUY, LMAT_ECHUCA_HUY_ALT };
static const station_t kEchucaEPM = { LMAT_ECHUCA_EPM, LMAT_ECHUCA_EPM_ALT };

/* Bendigo shared track */
static const station_t kBendigoBGO = { LMAT_BENDIGO_BGO, LMAT_BENDIGO_BGO_ALT };
static const station_t kBendigoKFT = { LMAT_BENDIGO_KFT, LMAT_BENDIGO_KFT_ALT };
static const station_t kBendigoCME = { LMAT_BENDIGO_CME, LMAT_BENDIGO_CME_ALT };
static const station_t kBendigoMMYV = { LMAT_BENDIGO_MMYV, LMAT_BENDIGO_MMYV_ALT };
static const station_t kBendigoKYN = { LMAT_BENDIGO_KYN, LMAT_BENDIGO_KYN_ALT };
static const station_t kBendigoWNO = { LMAT_BENDIGO_WNO, LMAT_BENDIGO_WNO_ALT };
static const station_t kBendigoMDN = { LMAT_BENDIGO_MDN, LMAT_BENDIGO_MDN_ALT };
static const station_t kBendigoGIS = { LMAT_BENDIGO_GIS, LMAT_BENDIGO_GIS_ALT };
static const station_t kBendigoRIK = { LMAT_BENDIGO_RIK, LMAT_BENDIGO_RIK_ALT };
static const station_t kBendigoCKF = { LMAT_BENDIGO_CKF, LMAT_BENDIGO_CKF_ALT };
#endif
static const station_t kBendigoSUY = { LMAT_BENDIGO_SUY, LMAT_BENDIGO_SUY_ALT };
static const station_t kBendigoWGS = { LMAT_BENDIGO_WGS, LMAT_BENDIGO_WGS_ALT };

static const station_t* kSwanHillStations[] = {
#if LMAT_VLINE_HAS_REGIONAL
    &kSwanHillSWL, &kSwanHillKER, &kSwanHillPYD, &kSwanHillDGE, &kSwanHillRAY, &kSwanHillEAGV,
    &kBendigoBGO, &kBendigoKFT, &kBendigoCME, &kBendigoMMYV, &kBendigoKYN, &kBendigoWNO, &kBendigoMDN, &kBendigoGIS, &kBendigoRIK, &kBendigoCKF,
#endif
    &kBendigoSUY, &kBendigoWGS, 
    &kVLineWestFSY, &kVLineSSS
};

static const infraid_t kSwanHillCodes[] = {
#if LMAT_VLINE_HAS_REGIONAL
    INFRAID_SWL, INFRAID_KER, INFRAID_PYD, INFRAID_DGE, INFRAID_RAY, INFRAID_EAGV,
    INFRAID_BGO, INFRAID_KFT, INFRAID_CME, INFRAID_MMYV, INFRAID_KYN, INFRAID_WNO, INFRAID_MDN, INFRAID_GIS, INFRAID_RIK, INFRAID_CKF,
#endif
    INFRAID_SUY, INFRAID_WGS, 
    INFRAID_FSY, INFRAID_SSS
};

static const size_t kSwanHillCount = sizeof(kSwanHillStations) / sizeof(station_t*);

uint16_t LSID::swlGetLED(infraid_t code) {
    return getLEDStub(kSwanHillStations, kSwanHillCodes, kSwanHillCount, code);
}

size_t LSID::bgoGetLEDsBetweenStub(
    const station_t** stations, const infraid_t* codes, size_t count,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) { // NOTE: this will assume that the last 3 stations in the stations/codes list are WGS, FSY and SSS (as they should be for Bendigo)
    int fromIndex = -1, toIndex = -1;
    for (size_t i = 0; i < count && (fromIndex == -1 || toIndex == -1); i++) {
        if (codes[i] == fromCode) fromIndex = i;
        if (codes[i] == toCode) toIndex = i;
    }

    ESP_RETURN_ON_FALSE(fromIndex >= 0, 0, kTag, "cannot find fromCode " INFRAID2STR_FMT, INFRAID2STR(fromCode));
    ESP_RETURN_ON_FALSE(toIndex >= 0, 0, kTag, "cannot find toCode " INFRAID2STR_FMT, INFRAID2STR(toCode));

    size_t outIndex = 0;
    if (fromIndex <= count - 3 && toIndex >= count - 2) { // travelling into city, crossing WGS and FSY
        if (fromIndex != count - 3) { // get to WGS first
            outIndex = getLEDsBetweenIndices(stations, codes, count, fromIndex, count - 3, buffer, maxLength);
            if (outIndex == maxLength) return outIndex;
            buffer[outIndex++] = LMAT_BENDIGO_WGS;
        }
        if (outIndex == maxLength) return outIndex;
        buffer[outIndex++] = LMAT_BENDIGO_WGS_ALT;
        if (outIndex == maxLength) return outIndex;
        buffer[outIndex++] = LMAT_VLWEST_SUN_ALT; // double LED between WGS and FSY
        if (outIndex != maxLength && toIndex != count - 2) { // head out from FSY
            buffer[outIndex++] = LMAT_VLWEST_FSY;
            return outIndex + getLEDsBetweenIndices(stations, codes, count, count - 2, toIndex, &buffer[outIndex], maxLength - outIndex);
        } else return outIndex;
    } else if (fromIndex >= count - 2 && toIndex <= count - 3) { // travelling out of city, crossing WGS and FSY
        if (fromIndex != count - 2) { // get to FSY first
            outIndex = getLEDsBetweenIndices(stations, codes, count, fromIndex, count - 2, buffer, maxLength);
            if (outIndex == maxLength) return outIndex;
            buffer[outIndex++] = LMAT_VLWEST_FSY;
        }
        if (outIndex == maxLength) return outIndex;
        buffer[outIndex++] = LMAT_VLWEST_SUN_ALT;
        if (outIndex == maxLength) return outIndex;
        buffer[outIndex++] = LMAT_BENDIGO_WGS_ALT;
        if (outIndex != maxLength && toIndex != count - 3) { // head out from WGS
            buffer[outIndex++] = LMAT_BENDIGO_WGS;
            return outIndex + getLEDsBetweenIndices(stations, codes, count, count - 3, toIndex, &buffer[outIndex], maxLength - outIndex);
        } else return outIndex;
    } else return getLEDsBetweenIndices(stations, codes, count, fromIndex, toIndex, buffer, maxLength);
}

size_t LSID::swlGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return bgoGetLEDsBetweenStub(kSwanHillStations, kSwanHillCodes, kSwanHillCount, fromCode, toCode, buffer, maxLength);
}

#if LMAT_VLINE_HAS_REGIONAL
static const station_t* kEchucaStations[] = {
    &kEchucaECH, &kEchucaROR, &kEchucaEME, &kEchucaGRG, &kEchucaHUY, &kEchucaEPM,
    &kBendigoBGO, &kBendigoKFT, &kBendigoCME, &kBendigoMMYV, &kBendigoKYN, &kBendigoWNO, &kBendigoMDN, &kBendigoGIS, &kBendigoRIK, &kBendigoCKF,
    &kBendigoSUY, &kBendigoWGS,
    &kVLineWestFSY, &kVLineSSS
};

static const infraid_t kEchucaCodes[] = {
    INFRAID_ECH, INFRAID_ROR, INFRAID_EME, INFRAID_GRG, INFRAID_HUY, INFRAID_EPM,
    INFRAID_BGO, INFRAID_KFT, INFRAID_CME, INFRAID_MMYV, INFRAID_KYN, INFRAID_WNO, INFRAID_MDN, INFRAID_GIS, INFRAID_RIK, INFRAID_CKF,
    INFRAID_SUY, INFRAID_WGS,
    INFRAID_FSY, INFRAID_SSS
};

static const size_t kEchucaCount = sizeof(kEchucaStations) / sizeof(station_t*);

uint16_t LSID::echGetLED(infraid_t code) {
    return getLEDStub(kEchucaStations, kEchucaCodes, kEchucaCount, code);
}

size_t LSID::echGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return bgoGetLEDsBetweenStub(kEchucaStations, kEchucaCodes, kEchucaCount, fromCode, toCode, buffer, maxLength);
}
#endif

#endif

#if LMAT_VLINE_HAS_SEYMOUR

#if LMAT_VLINE_HAS_REGIONAL
/* Shepparton line */
static const station_t kSheppartonSNH = { LMAT_SHEPPARTON_SNH, LMAT_SHEPPARTON_SNH_ALT };
static const station_t kSheppartonMPA = { LMAT_SHEPPARTON_MPA, LMAT_SHEPPARTON_MPA_ALT };
static const station_t kSheppartonMST = { LMAT_SHEPPARTON_MST, LMAT_SHEPPARTON_MST_ALT };
static const station_t kSheppartonNGE = { LMAT_SHEPPARTON_NGE, LMAT_SHEPPARTON_NGE_ALT };

/* Albury line */
static const station_t kAlburyABY = { LMAT_ALBURY_ABY, LMAT_ALBURY_ABY_ALT };
static const station_t kAlburyWOD = { LMAT_ALBURY_WOD, LMAT_ALBURY_WOD_ALT };
static const station_t kAlburyCHI = { LMAT_ALBURY_CHI, LMAT_ALBURY_CHI_ALT };
static const station_t kAlburySPTV = { LMAT_ALBURY_SPTV, LMAT_ALBURY_SPTV_ALT };
static const station_t kAlburyWRT = { LMAT_ALBURY_WRT, LMAT_ALBURY_WRT_ALT };
static const station_t kAlburyBXA = { LMAT_ALBURY_BXA, LMAT_ALBURY_BXA_ALT };
static const station_t kAlburyVTN = { LMAT_ALBURY_VTN, LMAT_ALBURY_VTN_ALT };
static const station_t kAlburyEOA = { LMAT_ALBURY_EOA, LMAT_ALBURY_EOA_ALT };
static const station_t kAlburyAVL = { LMAT_ALBURY_AVL, LMAT_ALBURY_AVL_ALT };

/* Seymour shared track */
static const station_t kSeymourSER = { LMAT_SEYMOUR_SER, LMAT_SEYMOUR_SER_ALT };
static const station_t kSeymourTOK = { LMAT_SEYMOUR_TOK, LMAT_SEYMOUR_TOK_ALT };
static const station_t kSeymourBRF = { LMAT_SEYMOUR_BRF, LMAT_SEYMOUR_BRF_ALT };
static const station_t kSeymourKET = { LMAT_SEYMOUR_KET, LMAT_SEYMOUR_KET_ALT };
static const station_t kSeymourWDG = { LMAT_SEYMOUR_WDG, LMAT_SEYMOUR_WDG_ALT };
static const station_t kSeymourHCJ = { LMAT_SEYMOUR_HCJ, LMAT_SEYMOUR_HCJ_ALT };
static const station_t kSeymourWAN = { LMAT_SEYMOUR_WAN, LMAT_SEYMOUR_WAN_ALT };
static const station_t kSeymourDBK = { LMAT_SEYMOUR_DBK, LMAT_SEYMOUR_DBK_ALT };
#endif
static const station_t kSeymourCGB = { LMAT_SEYMOUR_CGB, LMAT_SEYMOUR_CGB_ALT };
static const station_t kSeymourBMS = { LMAT_SEYMOUR_BMS, LMAT_SEYMOUR_BMS_ALT };
static const station_t kSeymourESD = { LMAT_SEYMOUR_ESD, LMAT_SEYMOUR_ESD_ALT };
static const station_t kSeymourNME = { LMAT_SEYMOUR_NME, LMAT_SEYMOUR_NME_ALT };

static const station_t* kSheppartonStations[] = {
#if LMAT_VLINE_HAS_REGIONAL
    &kSheppartonSNH, &kSheppartonMPA, &kSheppartonMST, &kSheppartonNGE,
    &kSeymourSER, &kSeymourTOK, &kSeymourBRF, &kSeymourKET, &kSeymourWDG, &kSeymourHCJ, &kSeymourWAN, &kSeymourDBK,
#endif
    &kSeymourCGB, &kSeymourBMS, &kSeymourESD, &kSeymourNME,
    &kVLineSSS
};

static const infraid_t kSheppartonCodes[] = {
#if LMAT_VLINE_HAS_REGIONAL
    INFRAID_SNH, INFRAID_MPA, INFRAID_MST, INFRAID_NGE,
    INFRAID_SER, INFRAID_TOK, INFRAID_BRF, INFRAID_KET, INFRAID_WDG, INFRAID_HCJ, INFRAID_WAN, INFRAID_DBK,
#endif
    INFRAID_CGB, INFRAID_BMS, INFRAID_ESD, INFRAID_NME,
    INFRAID_SSS
};

static const size_t kSheppartonCount = sizeof(kSheppartonStations) / sizeof(station_t*);

uint16_t LSID::snhGetLED(infraid_t code) {
    return getLEDStub(kSheppartonStations, kSheppartonCodes, kSheppartonCount, code);
}

size_t LSID::snhGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kSheppartonStations, kSheppartonCodes, kSheppartonCount, fromCode, toCode, buffer, maxLength);
}

#if LMAT_VLINE_HAS_REGIONAL
static const station_t* kAlburyStations[] = {
    &kAlburyABY, &kAlburyWOD, &kAlburyCHI, &kAlburySPTV, &kAlburyWRT, &kAlburyBXA, &kAlburyVTN, &kAlburyEOA, &kAlburyAVL,
    &kSeymourSER, &kSeymourTOK, &kSeymourBRF, &kSeymourKET, &kSeymourWDG, &kSeymourHCJ, &kSeymourWAN, &kSeymourDBK,
    &kSeymourCGB, &kSeymourBMS, &kSeymourESD, &kSeymourNME,
    &kVLineSSS
};

static const infraid_t kAlburyCodes[] = {
    INFRAID_ABY, INFRAID_WOD, INFRAID_CHI, INFRAID_SPTV, INFRAID_WRT, INFRAID_BXA, INFRAID_VTN, INFRAID_EOA, INFRAID_AVL,
    INFRAID_SER, INFRAID_TOK, INFRAID_BRF, INFRAID_KET, INFRAID_WDG, INFRAID_HCJ, INFRAID_WAN, INFRAID_DBK,
    INFRAID_CGB, INFRAID_BMS, INFRAID_ESD, INFRAID_NME,
    INFRAID_SSS
};

static const size_t kAlburyCount = sizeof(kAlburyStations) / sizeof(station_t*);

uint16_t LSID::abyGetLED(infraid_t code) {
    return getLEDStub(kAlburyStations, kAlburyCodes, kAlburyCount, code);
}

size_t LSID::abyGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kAlburyStations, kAlburyCodes, kAlburyCount, fromCode, toCode, buffer, maxLength);
}
#endif

#endif