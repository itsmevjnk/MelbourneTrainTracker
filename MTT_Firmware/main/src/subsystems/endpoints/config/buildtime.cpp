#include "subsystems/webserver.h"

#include "esp_app_desc.h"

#include "esp_log.h"
#include "esp_check.h"

const httpd_uri_t WebServer::kConfigGetBuildTime = {
    .uri = "/config/build",
    .method = HTTP_GET,
    .handler = configGetBuildTime,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetBuildTime(httpd_req_t* req) {
    const esp_app_desc_t* desc = esp_app_get_description();
    char result[16 + 1 + 16 + 1];
    size_t resultLen = snprintf(result, sizeof(result), "%s %s", desc->date, desc->time);

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, resultLen),
        kTag, "GET %s: cannot send response", req->uri
    );
    return ESP_OK;
}
