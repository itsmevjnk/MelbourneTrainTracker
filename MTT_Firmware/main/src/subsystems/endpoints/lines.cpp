#include "subsystems/webserver.h"
#include "services.h"
#include "esp_check.h"

size_t WebServer::getLinesString(char* result, size_t resultLen) {
    assert(result && resultLen);
    result[0] = '\0';
    
    size_t resultIndex = 0;
    uint32_t lines = Services::getEnabledLines();
    for (size_t i = 0; i < Services::kNumLines; i++) {
        if (lines & (1 << i)) {
            if (resultIndex + 4 > resultLen) break; // no more space

            if (resultIndex > 0) result[resultIndex - 1] = ','; // not first occurrence - change last line's termination to comma
            infraid_t id = Services::kLineIDs[i];
            // ESP_LOGD(kTag, INFRAID2STR_FMT, INFRAID2STR(id));
            for (size_t j = 0; j < 3; j++) result[resultIndex + j] = (char)(((id) >> (j << 3)) & 0x7F);
            result[resultIndex + 3] = '\0';
            resultIndex += 4;
        }
    }

    return (resultIndex > 0) ? (resultIndex - 1) : 0;
}

const httpd_uri_t WebServer::kGetLines = {
    .uri = "/lines",
    .method = HTTP_GET,
    .handler = getLines,
    .user_ctx = nullptr
};
esp_err_t WebServer::getLines(httpd_req_t* req) {
    char result[Services::kNumLines * 4]; // 3-character line code + , / \0
    size_t len = getLinesString(result, sizeof(result));

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, len),
        kTag, "GET /lines: cannot respond"
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kEnableLines = {
    .uri = "/lines",
    .method = HTTP_PUT,
    .handler = setLines,
    .user_ctx = (void*)1 // use user_ctx to specify if we want to enable or disable
};

const httpd_uri_t WebServer::kDisableLines = {
    .uri = "/lines",
    .method = HTTP_DELETE,
    .handler = setLines,
    .user_ctx = (void*)0 // use user_ctx to specify if we want to enable or disable
};

esp_err_t WebServer::setLines(httpd_req_t* req) {
    bool enable = (req->user_ctx);

    /* get URL query string */
    size_t bufLength = httpd_req_get_url_query_len(req) + 1;
    if (bufLength == 1) { // must provide lines
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL),
            kTag, "%s /lines: cannot respond with 400", (enable) ? "PUT" : "DELETE"
        );
        return ESP_OK;
    }

    char* queryString = (char*)malloc(bufLength);
    ESP_RETURN_ON_FALSE(queryString, ESP_ERR_NO_MEM, kTag, "cannot allocate query string buffer (size %u)", bufLength);
    esp_err_t ret = httpd_req_get_url_query_str(req, queryString, bufLength);
    if (ret == ESP_OK) {
        char paramValue[Services::kNumLines * 4]; memset(paramValue, 0, sizeof(paramValue));
        if (httpd_query_key_value(queryString, "l", paramValue, sizeof(paramValue)) != ESP_OK) {
            ESP_LOGE(kTag, "%s /lines: cannot get query parameter for lines", (enable) ? "PUT" : "DELETE");
badRequest:
            ret = httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, NULL);
            goto done;
        }

        if (!strcmp(paramValue, "all")) { // enable/disable all
            if (enable) Services::enableAllLines();
            else Services::disableAllLines();
        } else {
            char* token = strtok(paramValue, ",");
            if (!token) {
                ESP_LOGE(kTag, "%s /lines: query parameter for lines cannot be empty", (enable) ? "PUT" : "DELETE");
                goto badRequest;
            }
            while (token) {
                if (strlen(token) != 3) {
                    ESP_LOGE(kTag, "%s /lines: length of token %s is invalid", (enable) ? "PUT" : "DELETE", token);
                    goto badRequest;
                }
                ret = (enable) ? Services::enableLine(INFRAID(token)) : Services::disableLine(INFRAID(token));
                if (ret != ESP_OK) {
                    ESP_LOGE(kTag, "%s /lines: invalid line %s", (enable) ? "PUT" : "DELETE", token);
                    goto badRequest;
                }
                token = strtok(NULL, ",");
            }
        }

        size_t len = getLinesString(paramValue, sizeof(paramValue));
        ret = httpd_resp_send(req, paramValue, len); // return resulting lines
    }

done:
    free(queryString);
    if (ret != ESP_OK)
        ESP_LOGE(kTag, "%s /lines: error occurred executing handler (%s)", (enable) ? "PUT" : "DELETE", esp_err_to_name(ret));
    return ret;
}
