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
        kTag, "GET /driver: cannot respond"
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
            kTag, "POST /driver: cannot respond with 400"
        );
        return ESP_OK;
    }

    char* queryString = (char*)malloc(bufLength);
    ESP_RETURN_ON_FALSE(queryString, ESP_ERR_NO_MEM, kTag, "cannot allocate query string buffer (size %u)", bufLength);
    esp_err_t ret = httpd_req_get_url_query_str(req, queryString, bufLength);
    if (ret == ESP_OK) {
        char paramValue[2]; // reading one character (1 or 0)
        if (httpd_query_key_value(queryString, "s", paramValue, sizeof(paramValue)) != ESP_OK) {
            ESP_LOGE(kTag, "POST /driver: cannot get query parameter for state");
            ret = httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            goto done;
        }

        switch (paramValue[0]) {
            case '1':
                ESP_LOGI(kTag, "POST /driver: enabling LED drivers");
                ESP_ERROR_CHECK(LEDMatrix::enableDrivers());
                break;
            case '0':
                ESP_LOGI(kTag, "POST /driver: disabling LED drivers");
                ESP_ERROR_CHECK(LEDMatrix::disableDrivers());
                break;
            default:
                ESP_LOGE(kTag, "POST /driver: invalid state %c", paramValue[0]);
                ret = httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
                goto done;
        }
        ret = httpd_resp_send(req, NULL, 0);
    }

done:
    free(queryString);
    if (ret != ESP_OK)
        ESP_LOGE(kTag, "POST /driver: error occurred executing handler (%s)", esp_err_to_name(ret));
    return ret;
}