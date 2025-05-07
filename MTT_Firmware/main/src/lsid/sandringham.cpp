#include "lsid.h"

/* Sandringham line */
static const station_t kSandringhamSHM = { LMAT_SANDRINGHAM_SHM, LMAT_SANDRINGHAM_SHM_ALT };
static const station_t kSandringhamHAM = { LMAT_SANDRINGHAM_HAM, LMAT_SANDRINGHAM_HAM_ALT };
static const station_t kSandringhamBBH = { LMAT_SANDRINGHAM_BBH, LMAT_SANDRINGHAM_BBH_ALT };
static const station_t kSandringhamMBN = { LMAT_SANDRINGHAM_MBN, LMAT_SANDRINGHAM_MBN_ALT };
static const station_t kSandringhamNBN = { LMAT_SANDRINGHAM_NBN, LMAT_SANDRINGHAM_NBN_ALT };
static const station_t kSandringhamGVE = { LMAT_SANDRINGHAM_GVE, LMAT_SANDRINGHAM_GVE_ALT };
static const station_t kSandringhamELS = { LMAT_SANDRINGHAM_ELS, LMAT_SANDRINGHAM_ELS_ALT };
static const station_t kSandringhamRIP = { LMAT_SANDRINGHAM_RIP, LMAT_SANDRINGHAM_RIP_ALT };
static const station_t kSandringhamBCV = { LMAT_SANDRINGHAM_BCV, LMAT_SANDRINGHAM_BCV_ALT };
static const station_t kSandringhamWIN = { LMAT_SANDRINGHAM_WIN, LMAT_SANDRINGHAM_WIN_ALT };
static const station_t kSandringhamPRA = { LMAT_SANDRINGHAM_PRA, LMAT_SANDRINGHAM_PRA_ALT };
static const station_t kSandringhamSYR = { LMAT_SANDRINGHAM_SYR, LMAT_SANDRINGHAM_SYR_ALT };
static const station_t kSandringhamRMD = { LMAT_SANDRINGHAM_RMD, LMAT_SANDRINGHAM_RMD_ALT };
static const station_t kSandringhamFSS = { LMAT_SANDRINGHAM_FSS, LMAT_NULL };

static const station_t* kSandringhamStations[] = {
    &kSandringhamSHM, &kSandringhamHAM, &kSandringhamBBH, &kSandringhamMBN, &kSandringhamNBN, &kSandringhamGVE, &kSandringhamELS, &kSandringhamRIP, &kSandringhamBCV, &kSandringhamWIN, &kSandringhamPRA, &kSandringhamSYR, &kSandringhamRMD, &kSandringhamFSS
};
static const infraid_t kSandringhamCodes[] = {
    INFRAID_SHM, INFRAID_HAM, INFRAID_BBH, INFRAID_MBN, INFRAID_NBN, INFRAID_GVE, INFRAID_ELS, INFRAID_RIP, INFRAID_BCV, INFRAID_WIN, INFRAID_PRA, INFRAID_SYR, INFRAID_RMD, INFRAID_FSS
};

static const size_t kSandringhamCount = sizeof(kSandringhamStations) / sizeof(station_t*);

uint16_t LSID::shmGetLED(infraid_t code) {
    return getLEDStub(kSandringhamStations, kSandringhamCodes, kSandringhamCount, code);
}

size_t LSID::shmGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kSandringhamStations, kSandringhamCodes, kSandringhamCount, fromCode, toCode, buffer, maxLength);
}
