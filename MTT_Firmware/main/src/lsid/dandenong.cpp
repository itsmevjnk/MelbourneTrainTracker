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

#ifdef LMAT_DANDENONG_HAS_MAL
static const station_t kDandenongMAL = { LMAT_DANDENONG_MAL, LMAT_DANDENONG_MAL_ALT };
static const station_t kDandenongARM = { LMAT_DANDENONG_ARM, LMAT_DANDENONG_ARM_ALT };
static const station_t kDandenongTOR = { LMAT_DANDENONG_TOR, LMAT_DANDENONG_TOR_ALT };
static const station_t kDandenongHKN = { LMAT_DANDENONG_HKN, LMAT_DANDENONG_HKN_ALT };
#else
static const station_t kDandenongMAL = { LMAT_DANDENONG_CFD_ALT, LMAT_DANDENONG_CFD_ALT }; // use CFD alt (towards SYR) to represent MAL

#endif
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
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD,
    &kDandenongMAL,
#ifdef LMAT_DANDENONG_HAS_MAL
    &kDandenongARM, &kDandenongTOR, &kDandenongHKN,
#else
    &kDandenongMAL, &kDandenongMAL, &kDandenongMAL,
#endif
    &kDandenongSYR, &kDandenongRMD
};
static const infraid_t kPakenhamCodes[] = { 
    INFRAID_EPH, INFRAID_PKM, INFRAID_CDA, INFRAID_OFC, INFRAID_BFD, INFRAID_BEW, INFRAID_NWA, INFRAID_HLM,
    INFRAID_DNG, INFRAID_YMN, INFRAID_NPK, INFRAID_SNP, INFRAID_SPG, INFRAID_WTL, INFRAID_CLA, INFRAID_HUN, INFRAID_OAK, INFRAID_HUG, INFRAID_MRB, INFRAID_CNE, INFRAID_CFD,
    INFRAID_MAL, INFRAID_ARM, INFRAID_TOR, INFRAID_HKN,
    INFRAID_SYR, INFRAID_RMD
};
static const size_t kPakenhamCount = sizeof(kPakenhamStations) / sizeof(station_t*);

uint16_t LSID::pkmGetLED(infraid_t code) {
    if (isCityStation(code)) return dngCityGetLED(code);
    else if (isMunnelStation(code)) return mtGetLED(code);
    return getLEDStub(kPakenhamStations, kPakenhamCodes, kPakenhamCount, code);
}

size_t LSID::pkmGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return dngGetLEDsBetween(
        kPakenhamStations, kPakenhamCodes, kPakenhamCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kCranbourneStations[] = {
    &kCranbourneCBE, &kCranbourneMPK, &kCranbourneLBK,
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD,
    &kDandenongMAL,
#ifdef LMAT_DANDENONG_HAS_MAL
    &kDandenongARM, &kDandenongTOR, &kDandenongHKN,
#else
    &kDandenongMAL, &kDandenongMAL, &kDandenongMAL,
#endif
    &kDandenongSYR, &kDandenongRMD
};
static const infraid_t kCranbourneCodes[] = { 
    INFRAID_CBE, INFRAID_MPK, INFRAID_LBK,
    INFRAID_DNG, INFRAID_YMN, INFRAID_NPK, INFRAID_SNP, INFRAID_SPG, INFRAID_WTL, INFRAID_CLA, INFRAID_HUN, INFRAID_OAK, INFRAID_HUG, INFRAID_MRB, INFRAID_CNE, INFRAID_CFD,
    INFRAID_MAL, INFRAID_ARM, INFRAID_TOR, INFRAID_HKN,
    INFRAID_SYR, INFRAID_RMD
};
static const size_t kCranbourneCount = sizeof(kCranbourneStations) / sizeof(station_t*);

uint16_t LSID::cbeGetLED(infraid_t code) {
    if (isCityStation(code)) return dngCityGetLED(code);
    else if (isMunnelStation(code)) return mtGetLED(code);
    return getLEDStub(kCranbourneStations, kCranbourneCodes, kCranbourneCount, code);
}

size_t LSID::cbeGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return dngGetLEDsBetween(
        kCranbourneStations, kCranbourneCodes, kCranbourneCount,
        fromCode, toCode, buffer, maxLength
    );
}

size_t LSID::dngGetLEDsBetween(
    const station_t** stations, const infraid_t* codes, size_t count,
    infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength
) {
    bool fromMunnel = isMunnelStation(fromCode), toMunnel = isMunnelStation(toCode);
    if (fromMunnel && toMunnel) return mtGetLEDsBetween(fromCode, toCode, buffer, maxLength);
    
    if (fromMunnel) { // Metro Tunnel -> Dandenong
        size_t mtOffset = mtGetLEDsBetween(fromCode, INFRAID_AZC, buffer, maxLength); // to Anzac
        if (mtOffset == maxLength) return mtOffset;
        buffer[mtOffset++] = LMAT_MUNNEL_AZC;
        if (mtOffset == maxLength) return mtOffset;
        buffer[mtOffset++] = LMAT_MUNNEL_AZC_ALT;
        if (
            mtOffset == maxLength
            || toCode == INFRAID_HKN
#ifndef LMAT_DANDENONG_HAS_MAL
            || toCode == INFRAID_TOR || toCode == INFRAID_ARM || toCode == INFRAID_MAL || toCode == INFRAID_CFD
#endif
        ) return mtOffset;
        /* at this point we still have more space in the buffer, and we want to go beyond Caulfield/Hawksburn */
        buffer[mtOffset++] = LMAT_DANDENONG_LAST_BEFORE_MT;
        return mtOffset + rmdGetLEDsBetween(
            kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations,
            stations, codes, count,
            INFRAID_DANDENONG_LAST_BEFORE_MT, toCode, &buffer[mtOffset], maxLength - mtOffset
        );
    }
    
    if (toMunnel) { // Dandenong -> Metro Tunnel
        size_t mtOffset = getLEDsBetweenCodes(stations, codes, count, fromCode, INFRAID_DANDENONG_LAST_BEFORE_MT, buffer, maxLength); // get to Caulfield/Hawksburn
        if (mtOffset == maxLength) return mtOffset;
        buffer[mtOffset++] = LMAT_DANDENONG_LAST_BEFORE_MT;
        if (mtOffset == maxLength) return mtOffset;
        buffer[mtOffset++] = LMAT_MUNNEL_AZC_ALT;
        if (mtOffset == maxLength || toCode == INFRAID_AZC) return mtOffset;

        /* at this point we still have more space in the buffer, and we want to go beyond Anzac */
        buffer[mtOffset++] = LMAT_MUNNEL_AZC;
        return mtOffset + mtGetLEDsBetween(INFRAID_AZC, toCode, &buffer[mtOffset], maxLength - mtOffset);
    }

    /* normal handling */
#ifndef LMAT_DANDENONG_FSS_ALT
    return rmdGetLEDsBetween(
        kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations,
        stations, codes, count,
        fromCode, toCode, buffer, maxLength
    );
#else // forked rmdGetLEDsBetween - handle PAR-RMD connection
    bool fromCity = isCityStation(fromCode), toCity = isCityStation(toCode);
        
    assert(codes[count - 1] == INFRAID_RMD);

    // only handles entering/exiting Flinders St or City Loop stations as of now, and FSS and SSS are not skipped
    if (fromCity && toCity) { // intra-city
        return rmdCityGetLEDsBetween(kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations, fromCode, toCode, buffer, maxLength);
    } else if (fromCity) { // exiting from city
        size_t outIndex = 0;
        bool fromLoop = isCityLoopStation(fromCode);
        outIndex = rmdCityGetLEDsBetween(
            kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations,
            fromCode, (fromLoop) ? INFRAID_PAR : INFRAID_FSS, buffer, maxLength
        );

        if (fromLoop) { // PAR -> RMD
            ESP_RETURN_ON_FALSE(
                outIndex + 4 <= maxLength,
                outIndex,
                kTag, "not enough space to hold the PAR -> RMD leg"
            );
            buffer[outIndex + 0] = LMAT_DANDENONG_PAR;
            buffer[outIndex + 1] = LMAT_DANDENONG_FSS_ALT;
            buffer[outIndex + 2] = LMAT_DANDENONG_RMD_ALT;
            buffer[outIndex + 3] = LMAT_DANDENONG_RMD;
        } else {
            ESP_RETURN_ON_FALSE(
                outIndex + 3 <= maxLength,
                outIndex,
                kTag, "not enough space to hold the PAR -> RMD leg"
            );
            buffer[outIndex + 0] = LMAT_DANDENONG_FSS;
            buffer[outIndex + 1] = LMAT_DANDENONG_RMD_ALT;
            buffer[outIndex + 2] = LMAT_DANDENONG_RMD;
        }
        
        outIndex += 3;
        return outIndex + getLEDsBetweenCodes(stations, codes, count, INFRAID_RMD, toCode, &buffer[outIndex], maxLength - outIndex);
    } else if (toCity) { // entering city
        bool toLoop = isCityLoopStation(toCode);
        size_t outIndex = getLEDsBetweenCodes(stations, codes, count, fromCode, INFRAID_RMD, buffer, maxLength); // get way into RMD first
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold RMD alt"
        );
        buffer[outIndex++] = LMAT_DANDENONG_RMD_ALT;
        if (toCode == INFRAID_PAR || toCode == INFRAID_FSS) return outIndex; // nothing else to do

        /* either PAR or FSS gets skipped (which is unusual) */
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold %s", (toLoop) ? "PAR" : "FSS"
        );
        buffer[outIndex++] = (toLoop) ? LMAT_DANDENONG_PAR : LMAT_DANDENONG_FSS;
        return outIndex + rmdCityGetLEDsBetween(kDandenongCityCCWCodes, kDandenongCityCCWStations, kDandenongCityCWStations, (toLoop) ? INFRAID_PAR : INFRAID_FSS, toCode, &buffer[outIndex], maxLength - outIndex);
    }
    else return getLEDsBetweenCodes(stations, codes, count, fromCode, toCode, buffer, maxLength); // both source and destination are outside city
#endif
}

#ifndef LMAT_DANDENONG_HAS_MAL
static const station_t kDandenongMAL_AZC = { LMAT_MUNNEL_AZC_ALT, LMAT_MUNNEL_AZC_ALT }; // use AZC alt to represent MAL for Dandenong services going through the Metro Tunnel

static const station_t* kPakenhamMunnelStations[] = {
    &kPakenhamEPH, &kPakenhamPKM, &kPakenhamCDA, &kPakenhamOFC, &kPakenhamBFD, &kPakenhamBEW, &kPakenhamNWA, &kPakenhamHLM,
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD,
    &kDandenongMAL_AZC, &kDandenongMAL_AZC, &kDandenongMAL_AZC, &kDandenongMAL_AZC, 
    &kDandenongSYR, &kDandenongRMD
};

size_t LSID::pkmMunnelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return dngGetLEDsBetween(
        kPakenhamMunnelStations, kPakenhamCodes, kPakenhamCount,
        fromCode, toCode, buffer, maxLength
    );
}

static const station_t* kCranbourneMunnelStations[] = {
    &kCranbourneCBE, &kCranbourneMPK, &kCranbourneLBK,
    &kDandenongDNG, &kDandenongYMN, &kDandenongNPK, &kDandenongSNP, &kDandenongSPG, &kDandenongWTL, &kDandenongCLA, &kDandenongHUN, &kDandenongOAK, &kDandenongHUG, &kDandenongMRB, &kDandenongCNE, &kDandenongCFD,
    &kDandenongMAL_AZC, &kDandenongMAL_AZC, &kDandenongMAL_AZC, &kDandenongMAL_AZC, 
    &kDandenongSYR, &kDandenongRMD
};

size_t LSID::cbeMunnelGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return dngGetLEDsBetween(
        kCranbourneMunnelStations, kCranbourneCodes, kCranbourneCount,
        fromCode, toCode, buffer, maxLength
    );
}
#endif
