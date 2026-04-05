#include <utils/http_client.h>

#include <esp_crt_bundle.h>
#include <string.h>

HTTPClient::HTTPClient(const char* url, esp_http_client_method_t method) {
    memset(&m_config, 0, sizeof(m_config));
    m_config.url = url;
    m_config.method = method;
    m_config.event_handler = &HTTPClient::eventHandler;
    m_config.user_data = this;
    m_config.crt_bundle_attach = esp_crt_bundle_attach;
    m_config.user_agent = "VicTrains";
    m_client = esp_http_client_init(&m_config);
}

HTTPClient::~HTTPClient() {
    esp_http_client_cleanup(m_client);
}

esp_err_t HTTPClient::setURL(const char* url) {
    return esp_http_client_set_url(m_client, url);
}

esp_err_t HTTPClient::setHeader(const char* key, const char* value) {
    return esp_http_client_set_header(m_client, key, value);
}

esp_err_t HTTPClient::perform() {
    m_response.clear();
    return esp_http_client_perform(m_client);
}

int HTTPClient::getStatusCode() const {
    return esp_http_client_get_status_code(m_client);
}

const std::string& HTTPClient::getResponse() const {
    return m_response;
}

void HTTPClient::resetResponse() {
    m_response.clear();
    m_response.shrink_to_fit();
}

esp_err_t HTTPClient::eventHandler(esp_http_client_event_t* event) {
    if (event->event_id == HTTP_EVENT_ON_DATA) {
        HTTPClient* ctx = (HTTPClient*)event->user_data;
        ctx->m_response.append((char*)event->data, event->data_len);
    }
    return ESP_OK;
}

DeserializationError HTTPClient::deserialiseResponse(JsonDocument& doc) {
    return deserializeJson(doc, m_response);
}

DeserializationError HTTPClient::deserialiseResponse(JsonDocument& doc, JsonDocument& filter) {
    return deserializeJson(doc, m_response, DeserializationOption::Filter(filter));
}