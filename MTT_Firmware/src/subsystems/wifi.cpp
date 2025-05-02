#include "subsystems/wifi.h"
#include "esp_eap_client.h"

#include <string.h>

#include "esp_check.h"

#include "subsystems/nvs.h"

/* for logging */
#include "esp_log.h"
static const char* TAG = "wifi";

esp_err_t WiFi::init(const char* ssid, const char* password, size_t maxRetries) {
    /* check SSID and password */
    if (!ssid) {
        ESP_LOGE(TAG, "ssid cannot be null");
        return ESP_ERR_INVALID_ARG;
    }
    size_t ssidLength = strlen(ssid);
    if (ssidLength == 0 || ssidLength > 31) {
        ESP_LOGE(TAG, "invalid ssid length %u", ssidLength);
        return ESP_ERR_INVALID_ARG;
    }

    size_t pwLength = (!password) ? 0 : strlen(password);
    if (pwLength == 0) {
        ESP_LOGI(TAG, "password is null or empty, will connect to unencrypted Wi-Fi network");
        password = nullptr;
    } else if (pwLength > 63) {
        ESP_LOGE(TAG, "invalid password length %u", pwLength);
        return ESP_ERR_INVALID_ARG;
    }

    /* configure Wi-Fi */
    wifi_config_t config; memset(&config, 0, sizeof(config));
    memmove(config.sta.ssid, ssid, ssidLength + 1);
    if (password) memmove(config.sta.password, password, pwLength + 1);
    config.sta.threshold.rssi = -127; // accept any RSSI
    config.sta.threshold.authmode = (password) ? WIFI_AUTH_WEP : WIFI_AUTH_OPEN; // minimum auth mode is WEP if there's a password, or open if there's none
    
    ESP_RETURN_ON_ERROR(initStub(config, maxRetries), TAG, "Wi-Fi initialisation failed"); // run init stub
    ESP_RETURN_ON_ERROR(start(), TAG, "Wi-Fi driver start and connection failed");

    return ESP_OK;
}

esp_err_t WiFi::init(const char* ssid, const char* identity, const char* username, const char* password, const char* certificate, size_t certLength, size_t maxRetries) {
    /* check input */
    if (!ssid) {
        ESP_LOGE(TAG, "ssid cannot be null");
        return ESP_ERR_INVALID_ARG;
    }
    size_t ssidLength = strlen(ssid);
    if (ssidLength == 0 || ssidLength > 31) {
        ESP_LOGE(TAG, "invalid ssid length %u", ssidLength);
        return ESP_ERR_INVALID_ARG;
    }

    if (!identity) {
        ESP_LOGE(TAG, "identity cannot be null");
        return ESP_ERR_INVALID_ARG;
    }
    size_t idLength = strlen(identity);
    if (idLength == 0 || idLength > 127) {
        ESP_LOGE(TAG, "invalid identity length %u", idLength);
        return ESP_ERR_INVALID_ARG;
    }

    if (!username) {
        ESP_LOGE(TAG, "username cannot be null");
        return ESP_ERR_INVALID_ARG;
    }
    size_t userLength = strlen(username);
    if (userLength == 0 || userLength > 127) {
        ESP_LOGE(TAG, "invalid username length %u", userLength);
        return ESP_ERR_INVALID_ARG;
    }

    if (!password) {
        ESP_LOGE(TAG, "password cannot be null");
        return ESP_ERR_INVALID_ARG;
    }
    size_t pwLength = strlen(password);
    if (userLength == 0 || userLength > 127) {
        ESP_LOGE(TAG, "invalid password length %u", pwLength);
        return ESP_ERR_INVALID_ARG;
    }
    
    wifi_config_t config; memset(&config, 0, sizeof(config));
    memmove(config.sta.ssid, ssid, ssidLength + 1);
    // no password here since it'll be provided into EAP client
    config.sta.threshold.rssi = -127; // accept any RSSI
    config.sta.threshold.authmode = WIFI_AUTH_WPA2_ENTERPRISE; // must be WPA2-Enterprise

    ESP_RETURN_ON_ERROR(initStub(config, maxRetries), TAG, "Wi-Fi initialisation failed"); // run init stub

    /* configure EAP client */
    ESP_RETURN_ON_ERROR(
        esp_eap_client_set_identity((const uint8_t*)identity, idLength),
        TAG, "unable to set EAP identity"
    );
    ESP_RETURN_ON_ERROR(
        esp_eap_client_set_username((const uint8_t*)username, userLength),
        TAG, "unable to set EAP username"
    );
    ESP_RETURN_ON_ERROR(
        esp_eap_client_set_password((const uint8_t*)password, pwLength),
        TAG, "unable to set EAP password"
    );
    if (certificate && certLength > 0) {
        /* certificate provided */
        ESP_RETURN_ON_ERROR(
            esp_eap_client_set_ca_cert((const uint8_t*)certificate, certLength),
            TAG, "unable to set EAP CA certificate"
        );
    }
    ESP_RETURN_ON_ERROR(esp_wifi_sta_enterprise_enable(), TAG, "unable to enable enterprise authentication");

    ESP_RETURN_ON_ERROR(start(), TAG, "Wi-Fi driver start and connection failed");

    return ESP_OK;
}

#define WIFI_CONNECTED_BIT                              BIT0
#define WIFI_FAIL_BIT                                   BIT1

EventGroupHandle_t WiFi::m_eventGroup; // Wi-Fi event group
size_t WiFi::kMaxRetries;

esp_err_t WiFi::initStub(wifi_config_t& config, size_t maxRetries) {
    kMaxRetries = maxRetries;

    ESP_RETURN_ON_ERROR(NVS::init(), TAG, "NVS initialisation failed, cannot continue");

    ESP_RETURN_ON_ERROR(esp_netif_init(), TAG, "cannot initialise network interface");

    m_eventGroup = xEventGroupCreate(); // create event group for passing Wi-Fi events

    esp_err_t ret = esp_event_loop_create_default();
    if (ret != ESP_OK) {
        if (ret == ESP_ERR_INVALID_STATE) ESP_LOGW(TAG, "default event loop has already been created");
        else {
            ESP_LOGE(TAG, "cannot create default event loop");
            return ret;
        }
    }

    esp_netif_create_default_wifi_sta(); // attach event handlers for handling Wi-Fi connection (including DHCP)

    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandler, NULL, NULL), // NOTE: we might not have to store the instance
        TAG, "cannot register Wi-Fi event handler"
    );

    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ipEventHandler, NULL, NULL),
        TAG, "cannot register IP event handler"
    );

    wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(esp_wifi_init(&initConfig), TAG, "cannot initialise Wi-Fi driver");

    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), TAG, "cannot set mode to STA");
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &config), TAG, "cannot set configuration");

    return ESP_OK;
}

esp_err_t WiFi::start() {
    ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "cannot start Wi-Fi driver");

    /* wait for connection to finish/fail */
    EventBits_t bits = xEventGroupWaitBits(
        m_eventGroup,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE, pdFALSE, portMAX_DELAY
    );

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to Wi-Fi successfully");
        return ESP_OK;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "cannot connect to Wi-Fi - refer to above logs for details");
        return ESP_ERR_WIFI_BASE;
    } else {
        ESP_LOGE(TAG, "unexpected event 0x%lx", bits); // EventBits_t is uint32_t (aka long unsigned int)
        return ESP_ERR_INVALID_RESPONSE;
    }
}

size_t WiFi::m_retries = 0;
void WiFi::wifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
    if (eventID == WIFI_EVENT_STA_START || eventID == WIFI_EVENT_STA_DISCONNECTED) {
        if (eventID == WIFI_EVENT_STA_DISCONNECTED) {
            m_retries++;
            if (m_retries > kMaxRetries) {
                ESP_LOGE(TAG, "maximum reconnection attempts (%u) exceeded", kMaxRetries);
                xEventGroupSetBits(m_eventGroup, WIFI_FAIL_BIT);
            }

            ESP_LOGI(TAG, "disconnection event received, attempting to reconnect");
        }
        else ESP_LOGI(TAG, "Wi-Fi driver started, connecting to network");

        /* connect to Wi-Fi network */
        esp_err_t ret = esp_wifi_connect();
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_WIFI_SSID:
                ESP_LOGE(TAG, "provided SSID is invalid");
                xEventGroupSetBits(m_eventGroup, WIFI_FAIL_BIT);
                break;

            default:
                ESP_LOGE(TAG, "Wi-Fi connection failed with error code %d", ret);
                abort(); // any other type of error is fatal
                break;
        }
    }
}

void WiFi::ipEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
    if (eventID == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) eventData;
        ESP_LOGI(TAG, "acquired IP address: " IPSTR ", netmask " IPSTR ", gateway " IPSTR, IP2STR(&event->ip_info.ip), IP2STR(&event->ip_info.netmask), IP2STR(&event->ip_info.gw));
        xEventGroupSetBits(m_eventGroup, WIFI_CONNECTED_BIT);
        m_retries = 0; // reset counter
    }
}