#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

/* Werribee line */
static const station_t kWerribeeWER = { LMAT_WERRIBEE_WER, LMAT_WERRIBEE_WER_ALT };
static const station_t kWerribeeHCG = { LMAT_WERRIBEE_HCG, LMAT_WERRIBEE_HCG_ALT };
static const station_t kWerribeeWLD = { LMAT_WERRIBEE_WLD, LMAT_WERRIBEE_WLD_ALT };
static const station_t kWerribeeACF = { LMAT_WERRIBEE_ACF, LMAT_WERRIBEE_ACF_ALT };
static const station_t kWerribeeLAV = { LMAT_WERRIBEE_LAV, LMAT_WERRIBEE_LAV_ALT }; // via Altona Loop
static const station_t kWerribeeWTO = { LMAT_WERRIBEE_WTO, LMAT_WERRIBEE_WTO_ALT };
static const station_t kWerribeeALT = { LMAT_WERRIBEE_ALT, LMAT_WERRIBEE_ALT_ALT };
static const station_t kWerribeeSHE = { LMAT_WERRIBEE_SHE, LMAT_WERRIBEE_SHE_ALT };

/* Williamstown line */
static const station_t kWilliamstownWIL = { LMAT_WILLIAMSTOWN_WIL, LMAT_WILLIAMSTOWN_WIL_ALT };
static const station_t kWilliamstownWBH = { LMAT_WILLIAMSTOWN_WBH, LMAT_WILLIAMSTOWN_WBH_ALT };
static const station_t kWilliamstownNWN = { LMAT_WILLIAMSTOWN_NWN, LMAT_WILLIAMSTOWN_NWN_ALT };

/* Newport group shared track */
static const station_t kNewportNPT = { LMAT_NEWPORT_NPT, LMAT_NEWPORT_NPT_ALT };
static const station_t kNewportSPT = { LMAT_NEWPORT_SPT, LMAT_NEWPORT_SPT_ALT };
static const station_t kNewportYVE = { LMAT_NEWPORT_YVE, LMAT_NEWPORT_YVE_ALT };
static const station_t kNewportSEN = { LMAT_NEWPORT_SEN, LMAT_NEWPORT_SEN_ALT };
static const station_t kNewportFSY = { LMAT_NEWPORT_FSY, LMAT_NEWPORT_FSY_ALT };
static const station_t kNewportSKN = { LMAT_NEWPORT_SKN, LMAT_NEWPORT_SKN_ALT };
static const station_t kNewportNME = { LMAT_NEWPORT_NME, LMAT_NEWPORT_NME_ALT };
static const station_t kNewportSSS = { LMAT_NEWPORT_SSS, LMAT_NEWPORT_SSS_ALT };

/* Flinders Street Station */
static const station_t kCrossCityFSS = { LMAT_FRANKSTON_FSS, LMAT_NULL };

/* Frankston line */
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

// NOTE: cross city services change line at Flinders St (according to GTFS timetable)

static const station_t* kWerribeeStations[] = {
    &kWerribeeWER, &kWerribeeHCG, &kWerribeeWLD, &kWerribeeACF, &kWerribeeLAV,
    &kWerribeeWTO, &kWerribeeALT, &kWerribeeSHE, // Altona Loop - NOTE: special case for LAV and NPT on WBE line
    &kNewportNPT, &kNewportSPT, &kNewportYVE, &kNewportSEN, &kNewportFSY, &kNewportSKN, &kNewportNME,
    &kNewportSSS, &kCrossCityFSS,
};
static const infraid_t kWerribeeCodes[] = {
    INFRAID_WER, INFRAID_HCG, INFRAID_WLD, INFRAID_ACF, INFRAID_LAV, INFRAID_WTO, INFRAID_ALT, INFRAID_SHE,
    INFRAID_NPT, INFRAID_SPT, INFRAID_YVE, INFRAID_SEN, INFRAID_FSY, INFRAID_SKN, INFRAID_NME,
    INFRAID_SSS, INFRAID_FSS,
};
static const size_t kWerribeeCount = sizeof(kWerribeeStations) / sizeof(station_t*);
static const size_t kWerribeeNPTIndex = 8; // index of Newport (NPT) in kWerribeeCodes

uint16_t LSID::werGetLED(infraid_t code) {
    return getLEDStub(kWerribeeStations, kWerribeeCodes, kWerribeeCount, code);
}

// TODO: is there any trips that skip Newport or Laverton?
size_t LSID::werGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    if (fromCode == INFRAID_NPT && toCode == INFRAID_LAV) { // NPT direct to LAV
        ESP_RETURN_ON_FALSE(maxLength >= 2, 0, kTag, "not enough space to hold Newport - Laverton express LEDs");
        buffer[0] = LMAT_WERRIBEE_LAV_EX1;
        buffer[1] = LMAT_WERRIBEE_LAV_EX2;
        return 2;
    }

    if (fromCode == INFRAID_LAV && toCode == INFRAID_NPT) { // LAV direct to NPT
        ESP_RETURN_ON_FALSE(maxLength >= 2, 0, kTag, "not enough space to hold Newport - Laverton express LEDs");
        buffer[0] = LMAT_WERRIBEE_LAV_EX2;
        buffer[1] = LMAT_WERRIBEE_LAV_EX1;
        return 2;
    }

    return getLEDsBetweenCodes(kWerribeeStations, kWerribeeCodes, kWerribeeCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kWilliamstownStations[] = {
    &kWilliamstownWIL, &kWilliamstownWBH, &kWilliamstownNWN,
    &kNewportNPT, &kNewportSPT, &kNewportYVE, &kNewportSEN, &kNewportFSY, &kNewportSKN, &kNewportNME,
    &kNewportSSS, &kCrossCityFSS,
};
static const infraid_t kWilliamstownCodes[] = {
    INFRAID_WIL, INFRAID_WBH, INFRAID_NWN,
    INFRAID_NPT, INFRAID_SPT, INFRAID_YVE, INFRAID_SEN, INFRAID_FSY, INFRAID_SKN, INFRAID_NME,
    INFRAID_SSS, INFRAID_FSS,
};
static const size_t kWilliamstownCount = sizeof(kWilliamstownStations) / sizeof(station_t*);

uint16_t LSID::wilGetLED(infraid_t code) {
    return getLEDStub(kWilliamstownStations, kWilliamstownCodes, kWilliamstownCount, code);
}

size_t LSID::wilGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kWilliamstownStations, kWilliamstownCodes, kWilliamstownCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kFrankstonStations[] = {
    &kFrankstonFKN, &kFrankstonKAN, &kFrankstonSEA, &kFrankstonCAR, &kFrankstonBON, &kFrankstonCSA, &kFrankstonEDI, 
    &kFrankstonASP, &kFrankstonMOR, &kFrankstonPKD, &kFrankstonMEN, &kFrankstonCTM, &kFrankstonSOU, &kFrankstonHIG, 
    &kFrankstonMRN, &kFrankstonPAT, &kFrankstonBEN, &kFrankstonMCK, &kFrankstonOMD, &kFrankstonGHY, &kFrankstonCFD, 
    &kFrankstonMAL, &kFrankstonARM, &kFrankstonTOR, &kFrankstonHKN, &kFrankstonSYR, &kFrankstonRMD,
    &kCrossCityFSS,
};
static const infraid_t kFrankstonCodes[] = {
    INFRAID_FKN, INFRAID_KAN, INFRAID_SEA, INFRAID_CAR, INFRAID_BON, INFRAID_CSA, INFRAID_EDI, INFRAID_ASP, INFRAID_MOR, 
    INFRAID_PKD, INFRAID_MEN, INFRAID_CTM, INFRAID_SOU, INFRAID_HIG, INFRAID_MRN, INFRAID_PAT, INFRAID_BEN, INFRAID_MCK, 
    INFRAID_OMD, INFRAID_GHY, INFRAID_CFD, INFRAID_MAL, INFRAID_ARM, INFRAID_TOR, INFRAID_HKN, INFRAID_SYR, INFRAID_RMD, 
    INFRAID_FSS,
};
static const size_t kFrankstonCount = sizeof(kFrankstonStations) / sizeof(station_t*);

uint16_t LSID::fknGetLED(infraid_t code) {
    return getLEDStub(kFrankstonStations, kFrankstonCodes, kFrankstonCount, code);
}

size_t LSID::fknGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kFrankstonStations, kFrankstonCodes, kFrankstonCount, fromCode, toCode, buffer, maxLength);
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
