/* WiFi station (client) */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"

class WiFi {
public:
    static esp_err_t init(
        const char* ssid,
        const char* password = nullptr, // null for open networks
        size_t maxRetries = 3
    ); // initialise Wi-Fi with the provided params
    static esp_err_t init(
        const char* ssid,
        const char* identity,
        const char* username,
        const char* password,
        const char* certificate = nullptr, // intermediate CA certificate (that issued certificate for the RADIUS server) - optional but recommended
        size_t certLength = 0,
        /* TODO: check if eduroam requires client certificate - shouldn't be necessary for Deakin? */
        size_t maxRetries = 3
    ); // initialise Wi-Fi with enterprise credentials

    static bool isConnected();

private:
    static esp_err_t initStub(wifi_config_t& config, size_t maxRetries = 3); // initialise Wi-Fi with prepared configuration, without starting the Wi-Fi driver. this allows the WPA2-Enterprise init method to inject addition setup
    static esp_err_t start(); // start Wi-Fi driver and wait for connection

    static void wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData); // Wi-Fi event handler
    static void ipEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData); // IP event handler

    static EventGroupHandle_t m_eventGroup; // Wi-Fi event group
    static size_t kMaxRetries;
    static size_t m_retries; // retry counter

    static bool m_connected;

    static const char* kTag;
};