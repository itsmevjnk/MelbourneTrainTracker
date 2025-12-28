#include "subsystems/webserver.h"

#include "esp_timer.h"

#include "esp_log.h"
#include "esp_check.h"

#include <inttypes.h>

const httpd_uri_t WebServer::kConfigGetUptime = {
    .uri = "/config/uptime",
    .method = HTTP_GET,
    .handler = configGetUptime,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetUptime(httpd_req_t* req) {
    int64_t uptime = esp_timer_get_time() / 1000000LL;

    char result[21];
    size_t resultLen = snprintf(result, sizeof(result), "%" PRId64, uptime);

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, resultLen),
        kTag, "GET %s: cannot send response", req->uri
    );
    return ESP_OK;
}
