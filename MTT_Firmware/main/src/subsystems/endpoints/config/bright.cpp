#include <subsystems/webserver.h>
#include <subsystems/brightness.h>
#include <config.h>

#include <esp_check.h>

const httpd_uri_t WebServer::kConfigGetBrightnessMode = {
    .uri = "/config/bright",
    .method = HTTP_GET,
    .handler = configGetBrightnessMode
};
esp_err_t WebServer::configGetBrightnessMode(httpd_req_t* req) {
    char result[
        /* line 1: mode */ 1 + 2 /* \r\n */ +
        /* line 2: min brightness (0 - 100) */ 3 + 2 +
        /* line 3: max brightness (0 - 100) */ 3 + 2 +
        /* line 4: sunset time (0 - 86399) */ 5 + 2 +
        /* line 5: sunrise time (0 - 172799) */ 6 + 2 +
        /* line 6: latitude (sign + 3.6f) */ 1 + 3 + 1 + 6 + 2 +
        /* line 7: longitude (sign + 3.6f) */ 1 + 3 + 1 + 6 + 1 /* termination */
    ];
    size_t len = snprintf(
        result, sizeof(result),
        "%d\r\n%u\r\n%u\r\n%lu\r\n%lu\r\n%.6f\r\n%.6f",
        (uint8_t)Config::getBrightnessMode(),
        Config::getMinBrightness(), Config::getMaxBrightness(),
        Config::getBrightnessSunsetTime(), Config::getBrightnessSunriseTime(),
        Config::getBrightnessLatitude(), Config::getBrightnessLongitude()
    );
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, len),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kGetBrightness = {
    .uri = "/bright",
    .method = HTTP_GET,
    .handler = getBrightness
};
esp_err_t WebServer::getBrightness(httpd_req_t* req) {
    char result[4];
    size_t len = snprintf(result, sizeof(result), "%u", Brightness::getCurrentBrightness());
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, len),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kSetBrightness = {
    .uri = "/bright",
    .method = HTTP_POST,
    .handler = setBrightness
};
esp_err_t WebServer::setBrightness(httpd_req_t* req) {
    char content[4];
    size_t recv_size = req->content_len;
    if (recv_size <= 1 || recv_size >= sizeof(content)) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            ESP_RETURN_ON_ERROR(
                httpd_resp_send_err(req, HTTPD_408_REQ_TIMEOUT, NULL),
                kTag, "POST %s: cannot send error", req->uri
            );
            return ESP_OK;
        } else return ESP_FAIL;
    }

    content[ret] = '\0';
    uint32_t brightness;
    sscanf(content, "%lu", &brightness);
    if (brightness > 100) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }
    ESP_RETURN_ON_ERROR(
        Config::overrideBrightness(brightness),
        kTag, "POST %s: cannot override brightness", req->uri
    );
    
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST %s: cannot send response", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetBrightnessMode = {
    .uri = "/config/bright",
    .method = HTTP_POST,
    .handler = configSetBrightnessMode
};
esp_err_t WebServer::configSetBrightnessMode(httpd_req_t* req) {
    char content[
        /* line 1: mode */ 1 + 2 /* \r\n */ +
        /* line 2: min brightness (0 - 100) */ 3 + 2 +
        /* line 3: max brightness (0 - 100) */ 3 + 2 +
        // longest case: sending latitude + longitude (up to 11 chars per line)
        /* line 4: latitude (sign + 3.6f) */ 1 + 3 + 1 + 6 + 2 +
        /* line 5: longitude (sign + 3.6f) */ 1 + 3 + 1 + 6 + 1 /* termination */
    ];
    size_t recv_size = req->content_len;
    if (recv_size <= 1 || recv_size >= sizeof(content)) {
        ESP_LOGE(kTag, "POST %s: invalid content size %u", req->uri, recv_size);
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
    }

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            ESP_LOGE(kTag, "POST %s: request timed out (content size %u)", req->uri, recv_size);
            return httpd_resp_send_err(req, HTTPD_408_REQ_TIMEOUT, NULL);
        } else return ESP_FAIL;
    }

    content[ret] = '\0';
    char* saveptr = content;

    char* lineStr = strtok_r(content, "\r\n", &saveptr);
    if (!lineStr) {
        ESP_LOGE(kTag, "POST %s: brightness mode line cannot be retrieved", req->uri);
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
    }
    BrightnessMode mode; sscanf(lineStr, "%d", (int*)&mode);
    if (mode == kManualTemporary || mode == kManualPersistent) {
        lineStr = strtok_r(nullptr, "\r\n", &saveptr);
        if (!lineStr) {
            ESP_LOGE(kTag, "POST %s: brightness line cannot be retrieved", req->uri);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }
        uintptr_t brightness; sscanf(lineStr, "%u", &brightness);
        if (brightness > 100) {
            ESP_LOGE(kTag, "POST %s: invalid brightness %u received", req->uri, brightness);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }
        ESP_RETURN_ON_ERROR(
            Config::setBrightnessMode(brightness, (mode == kManualPersistent)),
            kTag, "failed to set brightness mode to %d", (int)mode
        );
    } else if (mode == kAutoManualTime || mode == kAutoSunTime) {
        lineStr = strtok_r(nullptr, "\r\n", &saveptr);
        if (!lineStr) {
            ESP_LOGE(kTag, "POST %s: min brightness line cannot be retrieved", req->uri);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }
        uintptr_t minBrightness; sscanf(lineStr, "%u", &minBrightness);
        if (minBrightness > 100) {
            ESP_LOGE(kTag, "POST %s: invalid min brightness %u received", req->uri, minBrightness);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }
        lineStr = strtok_r(nullptr, "\r\n", &saveptr);
        if (!lineStr) {
            ESP_LOGE(kTag, "POST %s: max brightness line cannot be retrieved", req->uri);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }
        uintptr_t maxBrightness; sscanf(lineStr, "%u", &maxBrightness);
        if (maxBrightness > 100 || maxBrightness < minBrightness) {
            ESP_LOGE(kTag, "POST %s: invalid max brightness %u received (min brightness: %u)", req->uri, maxBrightness, minBrightness);
            return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
        }

        if (mode == kAutoManualTime) {
            lineStr = strtok_r(nullptr, "\r\n", &saveptr);
            if (!lineStr) {
                ESP_LOGE(kTag, "POST %s: sunset time line cannot be retrieved", req->uri);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
            uint32_t sunsetTime; sscanf(lineStr, "%lu", &sunsetTime);
            lineStr = strtok_r(nullptr, "\r\n", &saveptr);
            if (!lineStr) {
                ESP_LOGE(kTag, "POST %s: sunrise time line cannot be retrieved", req->uri);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
            uint32_t sunriseTime; sscanf(lineStr, "%lu", &sunriseTime);

            ret = Config::setBrightnessMode(minBrightness, maxBrightness, sunsetTime, sunriseTime);
            if (ret == ESP_ERR_INVALID_ARG) {
                ESP_LOGE(kTag, "POST %s: invalid argument received (sunset: %u, sunrise: %u)", sunsetTime, sunriseTime);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
        } else {
            lineStr = strtok_r(nullptr, "\r\n", &saveptr);
            if (!lineStr) {
                ESP_LOGE(kTag, "POST %s: latitude line cannot be retrieved", req->uri);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
            float latitude; sscanf(lineStr, "%f", &latitude);
            lineStr = strtok_r(nullptr, "\r\n", &saveptr);
            if (!lineStr) {
                ESP_LOGE(kTag, "POST %s: longitude line cannot be retrieved", req->uri);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
            float longitude; sscanf(lineStr, "%f", &longitude);

            ret = Config::setBrightnessMode(minBrightness, maxBrightness, latitude, longitude);
            if (ret == ESP_ERR_INVALID_ARG) {
                ESP_LOGE(kTag, "POST %s: invalid argument received (latitude: %.6f, longitude: %.6f)", latitude, longitude);
                return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            }
        }
    } else {
        ESP_LOGE(kTag, "POST %s: invalid brightness mode %d", mode);
        return httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
    }
    
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST %s: cannot send response", req->uri
    );
    return ESP_OK;
}

