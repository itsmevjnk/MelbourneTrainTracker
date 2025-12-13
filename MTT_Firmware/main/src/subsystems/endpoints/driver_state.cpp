#include "subsystems/webserver.h"
#include "subsystems/led_matrix.h"

/* HTTP endpoints */
const httpd_uri_t WebServer::kGetDriverState = {
    .uri = "/driver",
    .method = HTTP_GET,
    .handler = getDriverState,
    .user_ctx = nullptr
};
esp_err_t WebServer::getDriverState(httpd_req_t* req) {
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, LEDMatrix::getState() ? "1" : "0", 1),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kSetDriverState = {
    .uri = "/driver",
    .method = HTTP_POST,
    .handler = setDriverState,
    .user_ctx = nullptr
};
esp_err_t WebServer::setDriverState(httpd_req_t* req) {
    /* get URL query string */
    size_t bufLength = httpd_req_get_url_query_len(req) + 1;
    if (bufLength == 1) { // must provide state
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }

    char* queryString = (char*)malloc(bufLength);
    ESP_RETURN_ON_FALSE(queryString, ESP_ERR_NO_MEM, kTag, "cannot allocate query string buffer (size %u)", bufLength);
    esp_err_t ret = httpd_req_get_url_query_str(req, queryString, bufLength);
    if (ret == ESP_OK) {
        char paramValue[2]; // reading one character (1 or 0)
        if (httpd_query_key_value(queryString, "s", paramValue, sizeof(paramValue)) != ESP_OK) {
            ESP_LOGE(kTag, "POST %s: cannot get query parameter for state", req->uri);
            ret = httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            goto done;
        }

        switch (paramValue[0]) {
            case '1':
                ESP_LOGI(kTag, "POST %s: enabling LED drivers", req->uri);
                ESP_ERROR_CHECK(LEDMatrix::enableDrivers());
                break;
            case '0':
                ESP_LOGI(kTag, "POST %s: disabling LED drivers", req->uri);
                ESP_ERROR_CHECK(LEDMatrix::disableDrivers());
                break;
            default:
                ESP_LOGE(kTag, "POST %s: invalid state %c", req->uri, paramValue[0]);
                ret = httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
                goto done;
        }
        ret = httpd_resp_send(req, NULL, 0);
    }

done:
    free(queryString);
    if (ret != ESP_OK)
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
        ESP_LOGE(kTag, "POST %s: error occurred executing handler (%s)", req->uri, esp_err_to_name(ret));
#else
        ESP_LOGE(kTag, "POST %s: error occurred executing handler (%d)", req->uri, ret);
#endif
    return ret;
}