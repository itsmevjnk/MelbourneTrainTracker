#include "lsid.h"
#include "hwconfig/led_matrix.h"

#include "esp_log.h"
#include "esp_check.h"

static const station_t kMunnelARN = { LMAT_MUNNEL_ARN, LMAT_MUNNEL_ARN_ALT };
static const station_t kMunnelPKV = { LMAT_MUNNEL_PKV, LMAT_MUNNEL_PKV_ALT };
static const station_t kMunnelSTL = { LMAT_MUNNEL_STL, LMAT_MUNNEL_STL_ALT };
static const station_t kMunnelTHL = { LMAT_MUNNEL_THL, LMAT_MUNNEL_THL_ALT };
static const station_t kMunnelAZC = { LMAT_MUNNEL_AZC, LMAT_MUNNEL_AZC_ALT };

static const station_t* kMunnelStations[] = {
    &kMunnelARN, &kMunnelPKV, &kMunnelSTL, &kMunnelTHL, &kMunnelAZC
};
static const infraid_t kMunnelCodes[] = {
    INFRAID_ARN, INFRAID_PKV, INFRAID_STL, INFRAID_THL, INFRAID_AZC
};
static const size_t kMunnelCount = sizeof(kMunnelStations) / sizeof(station_t*);

uint16_t LSID::mtGetLED(infraid_t code) {
    switch (code) {
        case INFRAID_ARN: return LMAT_MUNNEL_ARN;
        case INFRAID_PKV: return LMAT_MUNNEL_PKV;
        case INFRAID_STL: return LMAT_MUNNEL_STL;
        case INFRAID_THL: return LMAT_MUNNEL_THL;
        case INFRAID_AZC: return LMAT_MUNNEL_AZC;
        default:
            ESP_LOGE(kTag, "invalid station code " INFRAID2STR_FMT, INFRAID2STR(code));
            return LMAT_NULL;
    }
}

size_t LSID::mtGetLEDsBetween(infraid_t fromCode, infraid_t toCode, uint16_t* buffer, size_t maxLength) {
    return getLEDsBetweenCodes(kMunnelStations, kMunnelCodes, kMunnelCount, fromCode, toCode, buffer, maxLength);
}
