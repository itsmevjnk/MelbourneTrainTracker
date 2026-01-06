#include "subsystems/webserver.h"

#include "esp_check.h"
#include "esp_log.h"

esp_err_t WebServer::readRequestBody(httpd_req_t* req, uint8_t* buf, size_t len, size_t* readLen) {
    size_t remaining = req->content_len;
    if (remaining >= len) {
        ESP_LOGE(kTag, "%s: client supplied value of %u bytes, larger than the permitted size of %u bytes", req->uri, remaining, len);
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ""),
            kTag, "%s: cannot send error", req->uri
        );
    }

    size_t offset = 0;
    while (remaining > 0) {
        int received = httpd_req_recv(req, (char*) &buf[offset], remaining);
        if (received <= 0) {
            if (received == HTTPD_SOCK_ERR_TIMEOUT) continue;

            ESP_LOGE(kTag, "%s: unrecoverable error receiving request body", req->uri);
            ESP_RETURN_ON_ERROR(
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
                kTag, "%s: cannot send error", req->uri
            );
            return ESP_FAIL;
        }

        offset += received;
        remaining -= received;
    }

    if (readLen) *readLen = offset;
    return ESP_OK;
} 