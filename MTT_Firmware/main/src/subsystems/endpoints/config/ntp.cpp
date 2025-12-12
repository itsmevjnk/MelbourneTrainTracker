#include "subsystems/webserver.h"
#include "config.h"

#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigGetNTP = {
    .uri = "/config/ntp",
    .method = HTTP_GET,
    .handler = configGetNTP,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetNTP(httpd_req_t* req) {
    const char* result = Config::getTimeServer();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET /config/ntp: cannot respond"
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetNTP = {
    .uri = "/config/ntp",
    .method = HTTP_POST,
    .handler = configSetNTP,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetNTP(httpd_req_t* req) {
    char value[64];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) value, sizeof(value), &len),
        kTag, "POST /config/ntp: error occurred reading request body"
    );
    value[len] = '\0';

    esp_err_t err = Config::setTimeServer(value);
    if (err != ESP_OK) {
        ESP_LOGE(kTag, "POST /config/ntp: error occurred writing to NVS");
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
            kTag, "POST /config/ntp: cannot send error"
        );
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST /config/ntp: cannot send response"
    );

    return ESP_OK;
}
