#include "subsystems/webserver.h"

#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigReset = {
    .uri = "/config/reset",
    .method = HTTP_POST,
    .handler = configReset,
    .user_ctx = nullptr
};

esp_err_t WebServer::configReset(httpd_req_t* req) {
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST /config/reset: cannot send response"
    );

    esp_restart();

    return ESP_OK;
}