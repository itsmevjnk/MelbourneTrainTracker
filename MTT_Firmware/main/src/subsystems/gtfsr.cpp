#include "subsystems/gtfsr.h"

const char *GTFSR::kTag = "gtfs";

esp_http_client_handle_t GTFSR::m_metroClient;
esp_http_client_handle_t GTFSR::m_vlineClient;

esp_err_t GTFSR::init(const char *apiKey) {
    /* unitialise metro client */
    esp_http_client_config_t config = {
        .url = CONFIG_METRO_UPDATES_FEED,
        .method = HTTP_METHOD_GET,
        .disable_auto_redirect = false
    };
    m_metroClient = esp_http_client_init(&config);
    ESP_RETURN_ON_FALSE(m_metroClient != NULL, ESP_ERR_INVALID_STATE, kTag, "cannot initialise metro client");
    esp_http_client_set_header(m_metroClient, "Accept", "*/*");
    esp_http_client_set_header(m_metroClient, "KeyId", apiKey);

    /* initialise V/Line client */
    config.url = CONFIG_VLINE_UPDATES_FEED;
    m_vlineClient = esp_http_client_init(&config);
    ESP_RETURN_ON_FALSE(m_vlineClient != NULL, ESP_ERR_INVALID_STATE, kTag, "cannot initialise V/Line client");
    esp_http_client_set_header(m_vlineClient, "Accept", "*/*");
    esp_http_client_set_header(m_vlineClient, "KeyId", apiKey);

    /* initialise response buffer mutex */
    m_respMutex = xSemaphoreCreateMutexStatic(&m_respMutexBuf);

    return ESP_OK;
}

uint8_t GTFSR::m_respFragmentBuffer[CONFIG_GTFSR_RESPBUF_LEN];
size_t GTFSR::m_respFragmentLength = 0;
size_t GTFSR::m_respFragmentOffset = 0;
// size_t GTFSR::m_respLength = 0;

SemaphoreHandle_t GTFSR::m_respMutex;
StaticSemaphore_t GTFSR::m_respMutexBuf;


// NOTE: templated functions must be defined in header file

