#include "subsystems/nvs.h"

/* for logging */
#include "esp_log.h"
static const char* TAG = "nvs";

#include "esp_check.h"

bool NVS::m_initialised = false;
bool NVS::isInitialised() {
    return m_initialised;
}

esp_err_t NVS::init() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGI(TAG, "erasing NVS");
        ESP_RETURN_ON_ERROR(nvs_flash_erase(), TAG, "NVS erase failed");
        ret = nvs_flash_init();
    }

    ESP_RETURN_ON_ERROR(ret, TAG, "NVS initialisation failed");

    m_initialised = true;
    ESP_LOGI(TAG, "NVS initialised successfully");
    return ESP_OK;
}