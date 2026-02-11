#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Frankston city stations - counterclockwise (via City Loop in the Up direction) */
static const station_t kFrankstonPAR_CCW = { LMAT_DANDENONG_PAR, LMAT_DANDENONG_PAR_ALT };
static const station_t kFrankstonMCE_CCW = { LMAT_DANDENONG_MCE, LMAT_DANDENONG_MCE_ALT };
static const station_t kFrankstonFGS_CCW = { LMAT_DANDENONG_FGS, LMAT_DANDENONG_FGS_ALT };
static const station_t kFrankstonSSS_CCW = { LMAT_DANDENONG_SSS, LMAT_NEWPORT_SSS_ALT };
static const station_t kFrankstonFSS_CCW = { LMAT_FRANKSTON_FSS, LMAT_NULL };

/* Frankston city stations - clockwise (direct to Flinders St in the Up direction) */
static const station_t kFrankstonFSS_CW = { LMAT_FRANKSTON_FSS, LMAT_NEWPORT_SSS_ALT };
static const station_t kFrankstonSSS_CW = { LMAT_DANDENONG_SSS, LMAT_DANDENONG_FGS_ALT };
static const station_t kFrankstonFGS_CW = { LMAT_DANDENONG_FGS, LMAT_DANDENONG_MCE_ALT };
static const station_t kFrankstonMCE_CW = { LMAT_DANDENONG_MCE, LMAT_DANDENONG_PAR_ALT };
static const station_t kFrankstonPAR_CW = { LMAT_DANDENONG_PAR, LMAT_NULL };

// NOTE: Frankston line now uses Caulfield loop tracks in the city
// NOTE (2): there are TWO LEDs between PAR and RMD: RMD_ALT and FSS_ALT (see Dandenong for more details)

static const station_t kFrankstonFSS = { LMAT_FRANKSTON_FSS, LMAT_NEWPORT_SSS_ALT }; // alt LED for Frankston services running up to SSS (as reported by PTV API?)
static const station_t kFrankstonFKN = { LMAT_FRANKSTON_FKN, LMAT_FRANKSTON_FKN_ALT };
static const station_t kFrankstonKAN = { LMAT_FRANKSTON_KAN, LMAT_FRANKSTON_KAN_ALT };
static const station_t kFrankstonSEA = { LMAT_FRANKSTON_SEA, LMAT_FRANKSTON_SEA_ALT };
static const station_t kFrankstonCAR = { LMAT_FRANKSTON_CAR, LMAT_FRANKSTON_CAR_ALT };
static const station_t kFrankstonBON = { LMAT_FRANKSTON_BON, LMAT_FRANKSTON_BON_ALT };
static const station_t kFrankstonCSA = { LMAT_FRANKSTON_CSA, LMAT_FRANKSTON_CSA_ALT };
static const station_t kFrankstonEDI = { LMAT_FRANKSTON_EDI, LMAT_FRANKSTON_EDI_ALT };
static const station_t kFrankstonASP = { LMAT_FRANKSTON_ASP, LMAT_FRANKSTON_ASP_ALT };
static const station_t kFrankstonMOR = { LMAT_FRANKSTON_MOR, LMAT_FRANKSTON_MOR_ALT };
static const station_t kFrankstonPKD = { LMAT_FRANKSTON_PKD, LMAT_FRANKSTON_PKD_ALT };
static const station_t kFrankstonMEN = { LMAT_FRANKSTON_MEN, LMAT_FRANKSTON_MEN_ALT };
static const station_t kFrankstonCTM = { LMAT_FRANKSTON_CTM, LMAT_FRANKSTON_CTM_ALT };
static const station_t kFrankstonSOU = { LMAT_FRANKSTON_SOU, LMAT_FRANKSTON_SOU_ALT };
static const station_t kFrankstonHIG = { LMAT_FRANKSTON_HIG, LMAT_FRANKSTON_HIG_ALT };
static const station_t kFrankstonMRN = { LMAT_FRANKSTON_MRN, LMAT_FRANKSTON_MRN_ALT };
static const station_t kFrankstonPAT = { LMAT_FRANKSTON_PAT, LMAT_FRANKSTON_PAT_ALT };
static const station_t kFrankstonBEN = { LMAT_FRANKSTON_BEN, LMAT_FRANKSTON_BEN_ALT };
static const station_t kFrankstonMCK = { LMAT_FRANKSTON_MCK, LMAT_FRANKSTON_MCK_ALT };
static const station_t kFrankstonOMD = { LMAT_FRANKSTON_OMD, LMAT_FRANKSTON_OMD_ALT };
static const station_t kFrankstonGHY = { LMAT_FRANKSTON_GHY, LMAT_FRANKSTON_GHY_ALT };
static const station_t kFrankstonCFD = { LMAT_FRANKSTON_CFD, LMAT_FRANKSTON_CFD_ALT };
static const station_t kFrankstonMAL = { LMAT_FRANKSTON_MAL, LMAT_FRANKSTON_MAL_ALT };
static const station_t kFrankstonARM = { LMAT_FRANKSTON_ARM, LMAT_FRANKSTON_ARM_ALT };
static const station_t kFrankstonTOR = { LMAT_FRANKSTON_TOR, LMAT_FRANKSTON_TOR_ALT };
static const station_t kFrankstonHKN = { LMAT_FRANKSTON_HKN, LMAT_FRANKSTON_HKN_ALT };
static const station_t kFrankstonSYR = { LMAT_FRANKSTON_SYR, LMAT_FRANKSTON_SYR_ALT };
static const station_t kFrankstonRMD = { LMAT_FRANKSTON_RMD, LMAT_FRANKSTON_RMD_ALT };

/* Stony Point line */
static const station_t kStonySTY = { LMAT_STONY_STY, LMAT_STONY_STY_ALT };
static const station_t kStonyCPT = { LMAT_STONY_CPT, LMAT_STONY_CPT_ALT };
static const station_t kStonyMRO = { LMAT_STONY_MRO, LMAT_STONY_MRO_ALT };
static const station_t kStonyBIT = { LMAT_STONY_BIT, LMAT_STONY_BIT_ALT };
static const station_t kStonyHST = { LMAT_STONY_HST, LMAT_STONY_HST_ALT };
static const station_t kStonyTAB = { LMAT_STONY_TAB, LMAT_STONY_TAB_ALT };
static const station_t kStonySVE = { LMAT_STONY_SVE, LMAT_STONY_SVE_ALT };
static const station_t kStonyBXR = { LMAT_STONY_BXR, LMAT_STONY_BXR_ALT };
static const station_t kStonyLWA = { LMAT_STONY_LWA, LMAT_STONY_LWA_ALT };
static const station_t kStonyFKN = { LMAT_STONY_FKN, LMAT_NULL };

static const station_t* kFrankstonCityCCWStations[] = {
    &kFrankstonPAR_CCW, &kFrankstonMCE_CCW, &kFrankstonFGS_CCW, &kFrankstonSSS_CCW, &kFrankstonFSS_CCW
};
static const infraid_t kFrankstonCityCCWCodes[] = {
    INFRAID_PAR, INFRAID_MCE, INFRAID_FGS, INFRAID_SSS, INFRAID_FSS
};

static const station_t* kFrankstonCityCWStations[] = {
    &kFrankstonFSS_CW, &kFrankstonSSS_CW, &kFrankstonFGS_CW, &kFrankstonMCE_CW, &kFrankstonPAR_CW
};

static const station_t* kFrankstonStations[] = {
    &kFrankstonFKN, &kFrankstonKAN, &kFrankstonSEA, &kFrankstonCAR, &kFrankstonBON, &kFrankstonCSA, &kFrankstonEDI, 
    &kFrankstonASP, &kFrankstonMOR, &kFrankstonPKD, &kFrankstonMEN, &kFrankstonCTM, &kFrankstonSOU, &kFrankstonHIG, 
    &kFrankstonMRN, &kFrankstonPAT, &kFrankstonBEN, &kFrankstonMCK, &kFrankstonOMD, &kFrankstonGHY, &kFrankstonCFD, 
    &kFrankstonMAL, &kFrankstonARM, &kFrankstonTOR, &kFrankstonHKN, &kFrankstonSYR, &kFrankstonRMD
};
static const infraid_t kFrankstonCodes[] = {
    INFRAID_FKN, INFRAID_KAN, INFRAID_SEA, INFRAID_CAR, INFRAID_BON, INFRAID_CSA, INFRAID_EDI, INFRAID_ASP, INFRAID_MOR, 
    INFRAID_PKD, INFRAID_MEN, INFRAID_CTM, INFRAID_SOU, INFRAID_HIG, INFRAID_MRN, INFRAID_PAT, INFRAID_BEN, INFRAID_MCK, 
    INFRAID_OMD, INFRAID_GHY, INFRAID_CFD, INFRAID_MAL, INFRAID_ARM, INFRAID_TOR, INFRAID_HKN, INFRAID_SYR, INFRAID_RMD
};
static const size_t kFrankstonCount = sizeof(kFrankstonStations) / sizeof(station_t*);

uint16_t LSID::fknGetLED(infraid_t code) {
    if (isCityStation(code)) {
        switch (code) {
            case INFRAID_FSS: return LMAT_FRANKSTON_FSS;
            case INFRAID_SSS: return LMAT_DANDENONG_SSS;
            case INFRAID_FGS: return LMAT_DANDENONG_FGS;
            case INFRAID_MCE: return LMAT_DANDENONG_MCE;
            case INFRAID_PAR: return LMAT_DANDENONG_PAR;
            default:
                ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
                return LMAT_NULL;
        }
    }
    else return getLEDStub(kFrankstonStations, kFrankstonCodes, kFrankstonCount, code);
}

size_t LSID::fknGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
#ifndef LMAT_DANDENONG_FSS_ALT
    return rmdGetLEDsBetween(
        kFrankstonCityCCWCodes, kFrankstonCityCCWStations, kFrankstonCityCWStations,
        kFrankstonStations, kFrankstonCodes, kFrankstonCount,
        fromCode, toCode, buffer, maxLength
    );
#else // forked rmdGetLEDsBetween - handle PAR-RMD connection
    bool fromCity = isCityStation(fromCode), toCity = isCityStation(toCode);
        
    // assert(kFrankstonCodes[kFrankstonCount - 1] == INFRAID_RMD);

    // only handles entering/exiting Flinders St or City Loop stations as of now, and FSS and SSS are not skipped
    if (fromCity && toCity) { // intra-city
        return rmdCityGetLEDsBetween(kFrankstonCityCCWCodes, kFrankstonCityCCWStations, kFrankstonCityCWStations, fromCode, toCode, buffer, maxLength);
    } else if (fromCity) { // exiting from city
        size_t outIndex = 0;
        bool fromLoop = isCityLoopStation(fromCode);
        outIndex = rmdCityGetLEDsBetween(
            kFrankstonCityCCWCodes, kFrankstonCityCCWStations, kFrankstonCityCWStations,
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
            buffer[outIndex + 2] = LMAT_FRANKSTON_RMD_ALT;
            buffer[outIndex + 3] = LMAT_FRANKSTON_RMD;
        } else {
            ESP_RETURN_ON_FALSE(
                outIndex + 3 <= maxLength,
                outIndex,
                kTag, "not enough space to hold the PAR -> RMD leg"
            );
            buffer[outIndex + 0] = LMAT_FRANKSTON_FSS;
            buffer[outIndex + 1] = LMAT_FRANKSTON_RMD_ALT;
            buffer[outIndex + 2] = LMAT_FRANKSTON_RMD;
        }
        
        outIndex += 3;
        return outIndex + getLEDsBetweenCodes(kFrankstonStations, kFrankstonCodes, kFrankstonCount, INFRAID_RMD, toCode, &buffer[outIndex], maxLength - outIndex);
    } else if (toCity) { // entering city
        bool toLoop = isCityLoopStation(toCode);
        size_t outIndex = getLEDsBetweenCodes(kFrankstonStations, kFrankstonCodes, kFrankstonCount, fromCode, INFRAID_RMD, buffer, maxLength); // get way into RMD first
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold RMD alt"
        );
        buffer[outIndex++] = LMAT_FRANKSTON_RMD_ALT;
        if (toCode == INFRAID_PAR || toCode == INFRAID_FSS) return outIndex; // nothing else to do

        /* either PAR or FSS gets skipped (which is unusual) */
        ESP_RETURN_ON_FALSE(
            outIndex + 1 <= maxLength,
            outIndex,
            kTag, "not enough space to hold %s", (toLoop) ? "PAR" : "FSS"
        );
        buffer[outIndex++] = (toLoop) ? LMAT_DANDENONG_PAR : LMAT_DANDENONG_FSS;
        return outIndex + rmdCityGetLEDsBetween(kFrankstonCityCCWCodes, kFrankstonCityCCWStations, kFrankstonCityCWStations, (toLoop) ? INFRAID_PAR : INFRAID_FSS, toCode, &buffer[outIndex], maxLength - outIndex);
    }
    else return getLEDsBetweenCodes(kFrankstonStations, kFrankstonCodes, kFrankstonCount, fromCode, toCode, buffer, maxLength); // both source and destination are outside city
#endif
}

static const station_t* kStonyStations[] = {
    &kStonySTY, &kStonyCPT, &kStonyMRO, &kStonyBIT, &kStonyHST, &kStonyTAB, &kStonySVE, &kStonyBXR, &kStonyLWA, &kStonyFKN
};
static const infraid_t kStonyCodes[] = {
    INFRAID_STY, INFRAID_CPT, INFRAID_MRO, INFRAID_BIT, INFRAID_HST, INFRAID_TAB, INFRAID_SVE, INFRAID_BXR, INFRAID_LWA, INFRAID_FKN
};
static const size_t kStonyCount = sizeof(kStonyStations) / sizeof(station_t*);

uint16_t LSID::styGetLED(infraid_t code) {
    return getLEDStub(kStonyStations, kStonyCodes, kStonyCount, code);
}

size_t LSID::styGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kStonyStations, kStonyCodes, kStonyCount, fromCode, toCode, buffer, maxLength);
}
