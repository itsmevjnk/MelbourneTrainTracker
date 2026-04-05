#pragma once

#include <string>
#include <esp_http_client.h>
#include <ArduinoJson.h>

class HTTPClient {
public:
    HTTPClient(const char* url, esp_http_client_method_t method = HTTP_METHOD_GET);
    ~HTTPClient();

    esp_err_t setURL(const char* url);
    esp_err_t setHeader(const char* key, const char* value);

    esp_err_t perform();
    
    int getStatusCode() const;
    const std::string& getResponse() const;
    
    void resetResponse();

    DeserializationError deserialiseResponse(JsonDocument& doc);
    DeserializationError deserialiseResponse(JsonDocument& doc, JsonDocument& filter);

private:
    static esp_err_t eventHandler(esp_http_client_event_t* event);
    std::string m_response;

    esp_http_client_config_t m_config;
    esp_http_client_handle_t m_client;
};