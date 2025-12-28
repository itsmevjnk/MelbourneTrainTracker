#include "subsystems/webserver.h"
#include "subsystems/led_matrix.h"

/* HTTP endpoints */
const httpd_uri_t WebServer::kGetLEDBuffer = {
    .uri = "/buffer",
    .method = HTTP_GET,
    .handler = getLEDBuffer,
    .user_ctx = nullptr
};

esp_err_t WebServer::getLEDBuffer(httpd_req_t* req) {
    LEDMatrix::acquireBuffer();
    esp_err_t ret = httpd_resp_send(req, (char*)LEDMatrix::m_buffer, LMAT_SIZE);
    LEDMatrix::releaseBuffer();

    if (ret != ESP_OK) 
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
        ESP_LOGE(kTag, "GET %s: cannot respond (%s)", req->uri, esp_err_to_name(ret));
#else
        ESP_LOGE(kTag, "GET %s: cannot respond (%d)", req->uri, ret);
#endif
    return ret;
}