#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Mernda line */
static const station_t kMerndaMDD = { LMAT_MERNDA_MDD, LMAT_MERNDA_MDD_ALT };
static const station_t kMerndaHWS = { LMAT_MERNDA_HWS, LMAT_MERNDA_HWS_ALT };
static const station_t kMerndaMMR = { LMAT_MERNDA_MMR, LMAT_MERNDA_MMR_ALT };
static const station_t kMerndaSMG = { LMAT_MERNDA_SMG, LMAT_MERNDA_SMG_ALT };
static const station_t kMerndaEPP = { LMAT_MERNDA_EPP, LMAT_MERNDA_EPP_ALT };
static const station_t kMerndaLAL = { LMAT_MERNDA_LAL, LMAT_MERNDA_LAL_ALT };
static const station_t kMerndaTSN = { LMAT_MERNDA_TSN, LMAT_MERNDA_TSN_ALT };
static const station_t kMerndaKPK = { LMAT_MERNDA_KPK, LMAT_MERNDA_KPK_ALT };
static const station_t kMerndaRUT = { LMAT_MERNDA_RUT, LMAT_MERNDA_RUT_ALT };
static const station_t kMerndaRES = { LMAT_MERNDA_RES, LMAT_MERNDA_RES_ALT };
static const station_t kMerndaREG = { LMAT_MERNDA_REG, LMAT_MERNDA_REG_ALT };
static const station_t kMerndaPRE = { LMAT_MERNDA_PRE, LMAT_MERNDA_PRE_ALT };
static const station_t kMerndaBEL = { LMAT_MERNDA_BEL, LMAT_MERNDA_BEL_ALT };
static const station_t kMerndaTBY = { LMAT_MERNDA_TBY, LMAT_MERNDA_TBY_ALT };
static const station_t kMerndaCXT = { LMAT_MERNDA_CXT, LMAT_MERNDA_CXT_ALT };
static const station_t kMerndaNCE = { LMAT_MERNDA_NCE, LMAT_MERNDA_NCE_ALT };
static const station_t kMerndaMER = { LMAT_MERNDA_MER, LMAT_MERNDA_MER_ALT };
static const station_t kMerndaRUS = { LMAT_MERNDA_RUS, LMAT_MERNDA_RUS_ALT };

/* Hurstbridge line */
static const station_t kHurstbridgeHBE = { LMAT_HURSTBRIDGE_HBE, LMAT_HURSTBRIDGE_HBE_ALT };
static const station_t kHurstbridgeWTT = { LMAT_HURSTBRIDGE_WTT, LMAT_HURSTBRIDGE_WTT_ALT };
static const station_t kHurstbridgeDCK = { LMAT_HURSTBRIDGE_DCK, LMAT_HURSTBRIDGE_DCK_ALT };
static const station_t kHurstbridgeELT = { LMAT_HURSTBRIDGE_ELT, LMAT_HURSTBRIDGE_ELT_ALT };
static const station_t kHurstbridgeMMY = { LMAT_HURSTBRIDGE_MMY, LMAT_HURSTBRIDGE_MMY_ALT };
static const station_t kHurstbridgeGRN = { LMAT_HURSTBRIDGE_GRN, LMAT_HURSTBRIDGE_GRN_ALT };
static const station_t kHurstbridgeWAT = { LMAT_HURSTBRIDGE_WAT, LMAT_HURSTBRIDGE_WAT_ALT };
static const station_t kHurstbridgeMCD = { LMAT_HURSTBRIDGE_MCD, LMAT_HURSTBRIDGE_MCD_ALT };
static const station_t kHurstbridgeROS = { LMAT_HURSTBRIDGE_ROS, LMAT_HURSTBRIDGE_ROS_ALT };
static const station_t kHurstbridgeHDB = { LMAT_HURSTBRIDGE_HDB, LMAT_HURSTBRIDGE_HDB_ALT };
static const station_t kHurstbridgeEAG = { LMAT_HURSTBRIDGE_EAG, LMAT_HURSTBRIDGE_EAG_ALT };
static const station_t kHurstbridgeIVA = { LMAT_HURSTBRIDGE_IVA, LMAT_HURSTBRIDGE_IVA_ALT };
static const station_t kHurstbridgeDBN = { LMAT_HURSTBRIDGE_DBN, LMAT_HURSTBRIDGE_DBN_ALT };
static const station_t kHurstbridgeALP = { LMAT_HURSTBRIDGE_ALP, LMAT_HURSTBRIDGE_ALP_ALT };
static const station_t kHurstbridgeFFD = { LMAT_HURSTBRIDGE_FFD, LMAT_HURSTBRIDGE_FFD_ALT };
static const station_t kHurstbridgeDEN = { LMAT_HURSTBRIDGE_DEN, LMAT_HURSTBRIDGE_DEN_ALT };
static const station_t kHurstbridgeWTG = { LMAT_HURSTBRIDGE_WTG, LMAT_HURSTBRIDGE_WTG_ALT };

/* Clifton Hill shared track */
static const station_t kCliftonCHL = { LMAT_CLIFTON_CHL, LMAT_CLIFTON_CHL_ALT };
static const station_t kCliftonVPK = { LMAT_CLIFTON_VPK, LMAT_CLIFTON_VPK_ALT };
static const station_t kCliftonCWD = { LMAT_CLIFTON_CWD, LMAT_CLIFTON_CWD_ALT };
static const station_t kCliftonNRM = { LMAT_CLIFTON_NRM, LMAT_CLIFTON_NRM_ALT };
static const station_t kCliftonWRM = { LMAT_CLIFTON_WRM, LMAT_CLIFTON_WRM_ALT };
static const station_t kCliftonJLI = { LMAT_CLIFTON_JLI, LMAT_CLIFTON_JLI_ALT };

/* Clifton Hill city stations - clockwise (direct to Flinders St in the Up direction) */
static const station_t kCliftonFSS_CW = { LMAT_CLIFTON_FSS, LMAT_CLIFTON_SSS_ALT };
static const station_t kCliftonSSS_CW = { LMAT_CLIFTON_SSS, LMAT_CLIFTON_FGS_ALT };
static const station_t kCliftonFGS_CW = { LMAT_CLIFTON_FGS, LMAT_CLIFTON_MCE_ALT };
static const station_t kCliftonMCE_CW = { LMAT_CLIFTON_MCE, LMAT_CLIFTON_PAR_ALT };
static const station_t kCliftonPAR_CW = { LMAT_CLIFTON_PAR, LMAT_CLIFTON_FSS_CIRC };

// City Circle services run clockwise to maintain interoperability with Clifton Hill services
static const station_t* kCityCircleStations[] = {
    &kCliftonFSS_CW, &kCliftonSSS_CW, &kCliftonFGS_CW, &kCliftonMCE_CW, &kCliftonPAR_CW
};
static const infraid_t kCityCircleCodes[] = {
    INFRAID_FSS, INFRAID_SSS, INFRAID_FGS, INFRAID_MCE, INFRAID_PAR
};
static const size_t kCityCircleCount = sizeof(kCityCircleStations) / sizeof(station_t*);

uint16_t LSID::cclGetLED(infraid_t code) {
    switch (code) {
        case INFRAID_FSS: return LMAT_CLIFTON_FSS;
        case INFRAID_SSS: return LMAT_CLIFTON_SSS;
        case INFRAID_FGS: return LMAT_CLIFTON_FGS;
        case INFRAID_MCE: return LMAT_CLIFTON_MCE;
        case INFRAID_PAR: return LMAT_CLIFTON_PAR;
        default:
            ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
            return LMAT_NULL;
    }
}

size_t LSID::cclGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    int fromIndex = -1, toIndex = -1;

    /* find fromIndex */
    for (size_t i = 0; i < kCityCircleCount; i++) {
        if (kCityCircleCodes[i] == fromCode) {
            fromIndex = i;
            break;
        }
    }
    ESP_RETURN_ON_FALSE(fromIndex >= 0, 0, kTag, "cannot find fromCode " INFRAID2STR_FMT, INFRAID2STR(fromCode));

    /* find toIndex with loop back to the beginning at the end */
    for (size_t i = fromIndex, n = 0; n < kCityCircleCount; n++, i = (i + 1) % kCityCircleCount) {
        if (kCityCircleCodes[i] == toCode) {
            toIndex = i;
            break;
        }
    }
    ESP_RETURN_ON_FALSE(toIndex >= 0, 0, kTag, "cannot find toCode " INFRAID2STR_FMT, INFRAID2STR(toCode));

    size_t outIndex = 0;
    for (size_t i = fromIndex; i != toIndex && outIndex < maxLength; i = (i + 1) % kCityCircleCount, outIndex++)
        buffer[outIndex] = kCityCircleStations[i]->nextLED;

    return outIndex;
}


size_t LSID::chlGetLEDsBetween(
    const station_t** stations, const infraid_t* codes, size_t count,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) {
    bool fromCity = isCityStation(fromCode), toCity = isCityStation(toCode);

    if (fromCity && toCity) { // intra-city
        return cclGetLEDsBetween(fromCode, toCode, buffer, maxLength);
    } else if (fromCity) { // exiting from city (via PAR)
        size_t outIndex = 0;
        outIndex = cclGetLEDsBetween(fromCode, INFRAID_PAR, buffer, maxLength);
        ESP_RETURN_ON_FALSE(
            outIndex + 3 <= maxLength,
            outIndex,
            kTag, "not enough space to hold the PAR -> JLI leg"
        );
        buffer[outIndex + 0] = LMAT_CLIFTON_PAR;
        buffer[outIndex + 1] = LMAT_CLIFTON_JLI_ALT;
        buffer[outIndex + 2] = LMAT_CLIFTON_JLI;
        outIndex += 3;
        return outIndex + getLEDsBetweenCodes(stations, codes, count, INFRAID_JLI, toCode, &buffer[outIndex], maxLength - outIndex);
    } else if (toCity) { // entering city (via FSS)
        size_t outIndex = getLEDsBetweenCodes(stations, codes, count, fromCode, INFRAID_JLI, buffer, maxLength); // get way into JLI first
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold JLI alt"
        );
        buffer[outIndex++] = LMAT_CLIFTON_JLI_ALT;
        if (toCode == INFRAID_FSS) return outIndex; // nothing else to do

        /* FSS gets skipped (which is unusual) */
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold FSS"
        );
        buffer[outIndex++] = LMAT_CLIFTON_FSS;
        return outIndex + cclGetLEDsBetween(INFRAID_FSS, toCode, &buffer[outIndex], maxLength - outIndex);
    }
    else return getLEDsBetweenCodes(stations, codes, count, fromCode, toCode, buffer, maxLength); // both source and destination are outside city
}

static const station_t* kMerndaStations[] = {
    &kMerndaMDD, &kMerndaHWS, &kMerndaMMR, &kMerndaSMG, &kMerndaEPP, &kMerndaLAL, &kMerndaTSN, &kMerndaKPK, &kMerndaRUT, 
    &kMerndaRES, &kMerndaREG, &kMerndaPRE, &kMerndaBEL, &kMerndaTBY, &kMerndaCXT, &kMerndaNCE, &kMerndaMER, &kMerndaRUS,
    &kCliftonCHL, &kCliftonVPK, &kCliftonCWD, &kCliftonNRM, &kCliftonWRM, &kCliftonJLI,
    // &kCliftonFSS_CW, &kCliftonSSS_CW, &kCliftonFGS_CW, &kCliftonMCE_CW, &kCliftonPAR_CW
};
static const infraid_t kMerndaCodes[] = { 
    INFRAID_MDD, INFRAID_HWS, INFRAID_MMR, INFRAID_SMG, INFRAID_EPP, INFRAID_LAL, INFRAID_TSN, INFRAID_KPK, INFRAID_RUT, 
    INFRAID_RES, INFRAID_REG, INFRAID_PRE, INFRAID_BEL, INFRAID_TBY, INFRAID_CXT, INFRAID_NCE, INFRAID_MER, INFRAID_RUS,
    INFRAID_CHL, INFRAID_VPK, INFRAID_CWD, INFRAID_NRM, INFRAID_WRM, INFRAID_JLI,
    // INFRAID_FSS, INFRAID_SSS, INFRAID_FGS, INFRAID_MCE, INFRAID_PAR
};
static const size_t kMerndaCount = sizeof(kMerndaStations) / sizeof(station_t*);

uint16_t LSID::mddGetLED(infraid_t code) {
    if (isCityStation(code)) return cclGetLED(code);
    return getLEDStub(kMerndaStations, kMerndaCodes, kMerndaCount, code);
}

size_t LSID::mddGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return chlGetLEDsBetween(kMerndaStations, kMerndaCodes, kMerndaCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kHurstbridgeStations[] = {
    &kHurstbridgeHBE, &kHurstbridgeWTT, &kHurstbridgeDCK, &kHurstbridgeELT, &kHurstbridgeMMY, &kHurstbridgeGRN, &kHurstbridgeWAT, 
    &kHurstbridgeMCD, &kHurstbridgeROS, &kHurstbridgeHDB, &kHurstbridgeEAG, &kHurstbridgeIVA, &kHurstbridgeDBN, &kHurstbridgeALP, 
    &kHurstbridgeFFD, &kHurstbridgeDEN, &kHurstbridgeWTG,
    &kCliftonCHL, &kCliftonVPK, &kCliftonCWD, &kCliftonNRM, &kCliftonWRM, &kCliftonJLI,
    // &kCliftonFSS_CW, &kCliftonSSS_CW, &kCliftonFGS_CW, &kCliftonMCE_CW, &kCliftonPAR_CW
};
static const infraid_t kHurstbridgeCodes[] = { 
    INFRAID_HBE, INFRAID_WTT, INFRAID_DCK, INFRAID_ELT, INFRAID_MMY, INFRAID_GRN, INFRAID_WAT, INFRAID_MCD, INFRAID_ROS, 
    INFRAID_HDB, INFRAID_EAG, INFRAID_IVA, INFRAID_DBN, INFRAID_ALP, INFRAID_FFD, INFRAID_DEN, INFRAID_WTG, 
    INFRAID_CHL, INFRAID_VPK, INFRAID_CWD, INFRAID_NRM, INFRAID_WRM, INFRAID_JLI,
    // INFRAID_FSS, INFRAID_SSS, INFRAID_FGS, INFRAID_MCE, INFRAID_PAR
};
static const size_t kHurstbridgeCount = sizeof(kHurstbridgeStations) / sizeof(station_t*);

uint16_t LSID::hbeGetLED(infraid_t code) {
    if (isCityStation(code)) return cclGetLED(code);
    return getLEDStub(kHurstbridgeStations, kHurstbridgeCodes, kHurstbridgeCount, code);
}

size_t LSID::hbeGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return chlGetLEDsBetween(kHurstbridgeStations, kHurstbridgeCodes, kHurstbridgeCount, fromCode, toCode, buffer, maxLength);
}
