#include "subsystems/ntp.h"
#include "subsystems/wifi.h"

#include <time.h>

#include "esp_netif_sntp.h"
#include "esp_sntp.h"

#include "esp_check.h"

const char* NTP::kTag = "ntp";

esp_err_t NTP::init(const char* server) {
    ESP_RETURN_ON_FALSE(WiFi::isConnected(), ESP_ERR_WIFI_NOT_CONNECT, kTag, "not connected to Wi-Fi yet");

    setenv("TZ", "AEST-10AEDT,M10.1.0,M4.1.0/3", 1); // set timezone to Australia/Melbourne
    tzset();

    /* initialise SNTP */
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(server);
    config.sync_cb = &syncCallback;
    ESP_RETURN_ON_ERROR(esp_netif_sntp_init(&config), kTag, "cannot initialise SNTP");

    ESP_RETURN_ON_ERROR(
        esp_netif_sntp_sync_wait(pdMS_TO_TICKS(30000)),
        kTag, "timed out waiting for NTP synchronisation"
    );

    ESP_LOGI(kTag, "time synchronisation with %s finished", server);
    return ESP_OK;
}

void NTP::syncCallback(timeval* tv) {
    time_t now; time(&now); // read back time
    struct tm timeInfo; localtime_r(&now, &timeInfo);
    ESP_LOGI(
        kTag, "time synchronised, current time is %llu.%06ld sec (%04d-%02d-%02d %02d:%02d:%02d)",
        tv->tv_sec, tv->tv_usec,
        timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
        timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec
    );
}