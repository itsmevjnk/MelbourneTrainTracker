#include "subsystems/gtfsr.h"
#include "esp_check.h"

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
        .disable_auto_redirect = false,
        .event_handler = &GTFSR::clientEventHandler
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

    /* allocate initial response buffer */
    m_respBuffer = (uint8_t *)malloc(CONFIG_GTFSR_RESPBUF_INIT_LEN);
    ESP_RETURN_ON_FALSE(m_respBuffer != NULL, ESP_ERR_NO_MEM, kTag, "cannot allocate initial response buffer");

    /* initialise mutex */
    m_respBufferMutex = xSemaphoreCreateRecursiveMutexStatic(&m_respBufferMutexBuffer);

    return ESP_OK;
}

uint8_t *GTFSR::m_respBuffer;
size_t GTFSR::m_respBufferLen = CONFIG_GTFSR_RESPBUF_INIT_LEN;
SemaphoreHandle_t GTFSR::m_respBufferMutex;
StaticSemaphore_t GTFSR::m_respBufferMutexBuffer;
size_t GTFSR::m_respLength = 0;

esp_err_t GTFSR::clientEventHandler(esp_http_client_event_t *event) {
    static size_t offset = 0; // offset into m_respBuffer
    size_t newOffset;
    switch (event->event_id) {
        case HTTP_EVENT_HEADERS_SENT:
            offset = 0;
            break;
        case HTTP_EVENT_ON_DATA:
            // ESP_LOGI(kTag, "received data (%d bytes)", event->data_len);
            newOffset = offset + event->data_len;
            if (newOffset > m_respBufferLen) {
                size_t newBufferLen = newOffset;
#if CONFIG_GTFSR_RESPBUF_GROWTH > 1 // align to nearest growth increment
                newBufferLen = (m_respBufferLen + CONFIG_GTFSR_RESPBUF_GROWTH - 1) / CONFIG_GTFSR_RESPBUF_GROWTH;
                newBufferLen *= CONFIG_GTFSR_RESPBUF_GROWTH;
#endif
                void *newBuffer = realloc(m_respBuffer, newBufferLen);
                if (!newBuffer) {
                    ESP_LOGE(kTag, "insufficient memory to expand response buffer (was %u bytes, %u requested)", m_respBufferLen, newBufferLen);
                    // xSemaphoreGiveRecursive(m_respBufferMutex);
                    return ESP_ERR_NO_MEM;
                }

                m_respBuffer = (uint8_t *)newBuffer;
                m_respBufferLen = newBufferLen;
            }

            memcpy(&m_respBuffer[offset], event->data, event->data_len); // NOTE: we assume that the requester has already taken the mutex for us
            offset = newOffset;
            break;
        case HTTP_EVENT_ON_FINISH:
            m_respLength = offset;
            ESP_LOGI(kTag, "finished receiving %lu bytes, available memory: %lu bytes", offset, esp_get_minimum_free_heap_size());
            break;
        default:
            break;
    }
    return ESP_OK;
}

esp_err_t GTFSR::performRequest(esp_http_client_handle_t client) {
    ESP_RETURN_ON_ERROR(
        esp_http_client_flush_response(client, NULL),
        kTag, "cannot flush client response"
    );

    while (!xSemaphoreTakeRecursive(m_respBufferMutex, portMAX_DELAY));

    esp_err_t err;
    if ((err = esp_http_client_perform(client)) != ESP_OK) {
        ESP_LOGE(kTag, "HTTP request failed: %s\n", esp_err_to_name(err));
        xSemaphoreGiveRecursive(m_respBufferMutex);
        return err;
    }

    // ESP_LOGI(kTag, "HTTP status code: %d", esp_http_client_get_status_code(client));

    xSemaphoreGiveRecursive(m_respBufferMutex);
    return ESP_OK;
}

// NOTE: templated functions must be defined in header file

