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

bool GTFSR::readStringCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    struct read_string_ctx *ctx = (struct read_string_ctx *)*arg;

    size_t len = stream->bytes_left;
    if (len >= ctx->buflen) {
        ESP_LOGW(kTag, "string is %u characters long, beyond the maximum capacity of %u", len, ctx->buflen - 1);
        len = ctx->buflen - 1;
    }

    if (!pb_read(stream, (pb_byte_t *)ctx->buf, len)) {
        ESP_LOGE(kTag, "cannot read string (%s)", PB_GET_ERROR(stream));
        return false;
    }

    ctx->buf[len] = '\0'; // null terminate
    return true;
}

bool GTFSR::httpReadCallback(pb_istream_t *stream, pb_byte_t *buf, size_t count) {
    /* NOTE: we assume that m_respMutex has been claimed by us */

    esp_http_client_handle_t client = (esp_http_client_handle_t)stream->state;

    size_t offset = 0;
    size_t remaining = count;
    while (remaining > 0) {
        if (m_respFragmentOffset == m_respFragmentLength) { // already caught up
            int read_len = esp_http_client_read(client, (char *)m_respFragmentBuffer, CONFIG_GTFSR_RESPBUF_LEN);
            ESP_LOGD(kTag, "read response fragment of %d bytes", read_len);
            if (read_len < 0) {
                ESP_LOGE(kTag, "HTTP read error (%d)", read_len);
                return false;
            } else if (read_len == 0) {
                // ESP_LOGW(kTag, "possibly premature EOF (read %u bytes, expecting %u bytes) - total length so far: %u", offset, count, m_respLength);
                stream->bytes_left = 0; // nanopb will be the judge on whether the EOF is early
                return false;
            }

            m_respFragmentOffset = 0;
            m_respFragmentLength = read_len;
            // m_respLength += read_len;
        }

        size_t bufRemaining = m_respFragmentLength - m_respFragmentOffset;
        size_t len = (remaining < bufRemaining) ? remaining : bufRemaining;
        memcpy(&buf[offset], &m_respFragmentBuffer[m_respFragmentOffset], len);
        offset += len; m_respFragmentOffset += len; remaining -= len;
    }

    return true;
}

// NOTE: templated functions must be defined in header file

