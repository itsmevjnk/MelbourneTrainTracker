#include "subsystems/webserver.h"

#include "esp_app_desc.h"

#include "esp_log.h"
#include "esp_check.h"

const httpd_uri_t WebServer::kConfigGetBuild = {
    .uri = "/config/build",
    .method = HTTP_GET,
    .handler = configGetBuild,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetBuild(httpd_req_t* req) {
    const esp_app_desc_t* desc = esp_app_get_description();

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, desc->version, strlen(desc->version)),
        kTag, "GET %s: cannot send response", req->uri
    );
    return ESP_OK;
}
