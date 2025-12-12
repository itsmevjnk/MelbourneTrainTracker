#include "subsystems/webserver.h"
#include "config.h"

#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigGetMQTT = {
    .uri = "/config/mqtt",
    .method = HTTP_GET,
    .handler = configGetMQTT,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetMQTT(httpd_req_t* req) {
    const char* result = Config::getTimeServer();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET /config/mqtt: cannot respond"
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetMQTT = {
    .uri = "/config/mqtt",
    .method = HTTP_POST,
    .handler = configSetMQTT,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetMQTT(httpd_req_t* req) {
    char value[64];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) value, sizeof(value), &len),
        kTag, "POST /config/mqtt: error occurred reading request body"
    );
    value[len] = '\0';

    esp_err_t err = Config::setMQTTBroker(value);
    if (err != ESP_OK) {
        ESP_LOGE(kTag, "POST /config/mqtt: error occurred writing to NVS");
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
            kTag, "POST /config/mqtt: cannot send error"
        );
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST /config/mqtt: cannot send response"
    );

    return ESP_OK;
}
