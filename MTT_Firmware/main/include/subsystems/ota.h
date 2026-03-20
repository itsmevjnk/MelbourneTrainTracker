#pragma once

#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "esp_http_client.h"
#include <string>

class OTA {
public:
    static esp_err_t doUpdate(bool forceUpdate = false); // check for updates, and update if needed
    static esp_err_t checkForUpdates(bool* update, char* url, bool forceCheck = false);
    static esp_err_t performUpdate(const char* url);
    static esp_err_t confirmUpdate();
    static esp_err_t init();

private:
    static const char* kTag;

    static std::string m_jsonResponse;
    static esp_err_t httpEventHandler(esp_http_client_event_t* event);

    static void updateTask(void* pvParameters);
    static void updateLEDTask(void* pvParameters);
};