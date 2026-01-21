#include "subsystems/ota.h"
#include "esp_app_desc.h"
#include "esp_crt_bundle.h"
#include "esp_flash_partitions.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "sdkconfig.h" // for config
#include "esp_check.h"

#include "soc/soc.h"
#include "subsystems/status_led.h"

#include "ArduinoJson.h"

#include <string.h>

/* for logging */
#include "esp_log.h"
const char* OTA::kTag = "ota";

#ifndef CONFIG_OTA_GITHUB_REPO
#define CONFIG_OTA_GITHUB_REPO "itsmevjnk/MelbourneTrainTracker"
#endif

#if CONFIG_IDF_TARGET_ESP32S3
#define OTA_FIRMWARE_NAME "fw_rev2.bin"
#else
#define OTA_FIRMWARE_NAME "fw_rev1.bin"
#endif

void OTA::updateLEDTask(void* pvParameters) {
    while (true) {
        ESP_ERROR_CHECK(StatusLED::actyOn()); ESP_ERROR_CHECK(StatusLED::errorOn()); vTaskDelay(500 / portTICK_PERIOD_MS);
        ESP_ERROR_CHECK(StatusLED::actyOff()); ESP_ERROR_CHECK(StatusLED::errorOff()); vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

esp_err_t OTA::doUpdate() {
    bool update = false;
    char updateURL[80 + sizeof(CONFIG_OTA_GITHUB_REPO)]; // assuming that the remainder of firmware URL is 80 chars long
    ESP_RETURN_ON_ERROR(checkForUpdates(&update, updateURL), kTag, "cannot check for updates");

    if (!update) {
        ESP_LOGI(kTag, "update is not needed");
        return ESP_OK;
    }

    TaskHandle_t ledTask;
    BaseType_t taskRet = xTaskCreatePinnedToCore(
        OTA::updateLEDTask,
        "update_led",
        2048,
        NULL,
        1,
        &ledTask,
        APP_CPU_NUM
    );
    if (taskRet != pdPASS) ESP_LOGE(kTag, "cannot create LED task (%d)", taskRet);

    esp_err_t ret = performUpdate(updateURL);

    if (taskRet == pdPASS) {
        vTaskDelete(ledTask);
        ESP_ERROR_CHECK(StatusLED::actyOff()); ESP_ERROR_CHECK(StatusLED::errorOff()); // reset LEDs
    }

    if (ret == ESP_OK) {
        ESP_LOGI(kTag, "rebooting");
        esp_restart();
    }    

    return ret;
}

std::string OTA::m_jsonResponse;
esp_err_t OTA::httpEventHandler(esp_http_client_event_t* event) {
    if (event->event_id == HTTP_EVENT_ON_DATA) {
        m_jsonResponse.append((char*)event->data, event->data_len);
    }
    return ESP_OK;
}

esp_err_t OTA::checkForUpdates(bool* update, char* url) {    
    m_jsonResponse.clear(); // in case it hasn't been cleared yet

    esp_http_client_config_t config = {
        .url = "https://api.github.com/repos/" CONFIG_OTA_GITHUB_REPO "/releases/latest",
        .method = HTTP_METHOD_GET,
        .event_handler = &OTA::httpEventHandler,
        .crt_bundle_attach = esp_crt_bundle_attach
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "User-Agent", "VicTrains");
    esp_http_client_set_header(client, "Accept", "application/vnd.github+json");
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        JsonDocument filter;
        filter["tag_name"] = true;
        filter["assets"][0]["name"] = true;
        filter["assets"][0]["browser_download_url"] = true;

        JsonDocument doc;
        DeserializationError jsonErr = deserializeJson(doc, m_jsonResponse, DeserializationOption::Filter(filter));

        if (!jsonErr) {
            const char* tagName = doc["tag_name"];
            const esp_app_desc_t* desc = esp_app_get_description();
            ESP_LOGI(kTag, "latest release tag on GitHub: %s, current build: %s", tagName, desc->version);

            /* check against current app version */
            if (strcmp(desc->version, tagName)) { // mismatch - possible update
                bool updateNeeded = true;
                size_t currentVersionLen = strlen(desc->version);
                if (currentVersionLen > 6 && !memcmp(&desc->version[currentVersionLen - 6], "-dirty", 6)) { // current app version is built from dirty tree - likely to be dev build, so no OTA
                    ESP_LOGI(kTag, "current build is development build, skipping update");
                    updateNeeded = false;
                }

                if (update) *update = updateNeeded;

                if (updateNeeded && url) { // retrieve firmware URL
                    *url = '\0'; // fall back to empty URL if a suitable one cannot be found
                    for (JsonVariant asset : doc["assets"].as<JsonArray>()) {
                        const char* name = asset["name"];
                        if (!strcmp(name, OTA_FIRMWARE_NAME)) {
                            const char* firmwareURL = asset["browser_download_url"];
                            ESP_LOGI(kTag, "found %s firmware at %s", OTA_FIRMWARE_NAME, firmwareURL);
                            strcpy(url, firmwareURL);
                            break;
                        }
                    }
                }                
            }
        } else {
            ESP_LOGE(kTag, "JSON parsing failed: %s", jsonErr.c_str());
        }
    }

    m_jsonResponse.clear(); m_jsonResponse.shrink_to_fit(); // shrink JSON response to minimum as we're done with it

    return err;
}

esp_err_t OTA::performUpdate(const char* url) {
    esp_http_client_config_t config = {
        .url = url,
        .buffer_size = CONFIG_MBEDTLS_SSL_IN_CONTENT_LEN,
        .buffer_size_tx = CONFIG_MBEDTLS_SSL_OUT_CONTENT_LEN,
        .crt_bundle_attach = esp_crt_bundle_attach
    };
    esp_https_ota_config_t ota_config = {
        .http_config = &config,
        .partial_http_download = true, // needed as otherwise we'd be running out of buffer
        .max_http_request_size = CONFIG_MBEDTLS_SSL_IN_CONTENT_LEN
    };

    esp_err_t ret = esp_https_ota(&ota_config);
    if (ret == ESP_OK) {
        ESP_LOGI(kTag, "OTA update succeeded, will boot into new firmware on next reset");
    } else {
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
        ESP_LOGE(kTag, "OTA update failed (%s)", esp_err_to_name(ret));
#else
        ESP_LOGE(kTag, "OTA update failed (%d)", ret);
#endif
    }

    return ret;
}

esp_err_t OTA::confirmUpdate() {
    const esp_partition_t* runningPartition = esp_ota_get_running_partition();
    esp_ota_img_states_t otaState;
    esp_err_t ret = esp_ota_get_state_partition(runningPartition, &otaState);
    if (ret == ESP_OK) {
        if (otaState == ESP_OTA_IMG_PENDING_VERIFY) {
            ESP_LOGI(kTag, "cancelling rollback to confirm update");
            esp_ota_mark_app_valid_cancel_rollback();
        }
    } else {
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
        ESP_LOGW(kTag, "cannot get OTA partition state (%s)", esp_err_to_name(ret));
#else
        ESP_LOGW(kTag, "cannot get OTA partition state (%d)", ret);
#endif
    }

    return ESP_OK;
}

#ifndef CONFIG_OTA_UPDATE_INTERVAL
#define CONFIG_OTA_UPDATE_INTERVAL              7
#endif

esp_err_t OTA::initUpdateTimer() {
    const TickType_t ticks = pdMS_TO_TICKS(CONFIG_OTA_UPDATE_INTERVAL * 86400000ULL);

    TimerHandle_t timer = xTimerCreate(
        "ota_update",
        ticks,
        pdTRUE,
        NULL,
        OTA::updateCallback
    );

    if (!timer) {
        ESP_LOGE(kTag, "cannot create update timer");
        return ESP_FAIL;
    }

    xTimerStart(timer, 0);
    return ESP_OK;
}

void OTA::updateCallback(TimerHandle_t xTimer) {
    ESP_LOGI(kTag, "executing update check");
    doUpdate();
}