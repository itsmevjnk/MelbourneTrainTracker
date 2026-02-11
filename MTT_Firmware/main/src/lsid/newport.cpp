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
static const station_t kNewportSSS = {
    LMAT_NEWPORT_SSS,
#ifdef LMAT_NEWPORT_SSS_ALT_SHM // because Rev1 boards don't have this LED
    LMAT_NEWPORT_SSS_ALT_SHM
#else
    LMAT_NEWPORT_SSS_ALT
#endif
};
static const station_t kNewportFSS = { LMAT_SANDRINGHAM_FSS, LMAT_NULL }; // joins up to Sandringham line

// NOTE: cross city services change line at Flinders St (according to GTFS timetable)

static const station_t* kWerribeeStations[] = {
    &kWerribeeWER, &kWerribeeHCG, &kWerribeeWLD, &kWerribeeACF, &kWerribeeLAV,
    &kWerribeeWTO, &kWerribeeALT, &kWerribeeSHE, // Altona Loop - NOTE: special case for LAV and NPT on WBE line
    &kNewportNPT, &kNewportSPT, &kNewportYVE, &kNewportSEN, &kNewportFSY, &kNewportSKN, &kNewportNME,
    &kNewportSSS, &kNewportFSS,
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
        buffer[0] = LMAT_WERRIBEE_LAV_EX2;
        buffer[1] = LMAT_WERRIBEE_LAV_EX1;
        return 2;
    }

    if (fromCode == INFRAID_LAV && toCode == INFRAID_NPT) { // LAV direct to NPT
        ESP_RETURN_ON_FALSE(maxLength >= 2, 0, kTag, "not enough space to hold Newport - Laverton express LEDs");
        buffer[0] = LMAT_WERRIBEE_LAV_EX1;
        buffer[1] = LMAT_WERRIBEE_LAV_EX2;
        return 2;
    }

    return getLEDsBetweenCodes(kWerribeeStations, kWerribeeCodes, kWerribeeCount, fromCode, toCode, buffer, maxLength);
}

static const station_t* kWilliamstownStations[] = {
    &kWilliamstownWIL, &kWilliamstownWBH, &kWilliamstownNWN,
    &kNewportNPT, &kNewportSPT, &kNewportYVE, &kNewportSEN, &kNewportFSY, &kNewportSKN, &kNewportNME,
    &kNewportSSS, &kNewportFSS,
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
