#include "subsystems/gtfsr.h"

#ifndef CONFIG_GTFSR_RESPBUF_INIT_LEN
#define CONFIG_GTFSR_RESPBUF_INIT_LEN               65536
#endif

#ifndef CONFIG_GTFSR_RESPBUF_GROWTH
#define CONFIG_GTFSR_RESPBUF_GROWTH                 1
#endif

#ifndef CONFIG_GTFSR_RESPBUF_TEMP_LEN
#define CONFIG_GTFSR_RESPBUF_TEMP_LEN               4096
#endif

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

    return ESP_OK;
}

// NOTE: templated functions must be defined in header file

