#include "subsystems/webserver.h"
#include "config.h"

#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigGetMDNSHost = {
    .uri = "/config/mdns/host",
    .method = HTTP_GET,
    .handler = configGetMDNSHost,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetMDNSHost(httpd_req_t* req) {
    const char* result = Config::getMDNSHostname();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetMDNSHost = {
    .uri = "/config/mdns/host",
    .method = HTTP_POST,
    .handler = configSetMDNSHost,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetMDNSHost(httpd_req_t* req) {
    char value[32];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) value, sizeof(value), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    value[len] = '\0';

    esp_err_t err = Config::setMDNSHostname(value);
    if (err != ESP_OK) {
        ESP_LOGE(kTag, "POST %s: error occurred writing to NVS", req->uri);
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST %s: cannot send response", req->uri
    );

    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigGetMDNSInst = {
    .uri = "/config/mdns/inst",
    .method = HTTP_GET,
    .handler = configGetMDNSInst,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetMDNSInst(httpd_req_t* req) {
    const char* result = Config::getMDNSInstanceName();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetMDNSInst = {
    .uri = "/config/mdns/inst",
    .method = HTTP_POST,
    .handler = configSetMDNSInst,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetMDNSInst(httpd_req_t* req) {
    char value[64];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) value, sizeof(value), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    value[len] = '\0';

    esp_err_t err = Config::setMDNSInstanceName(value);
    if (err != ESP_OK) {
        ESP_LOGE(kTag, "POST %s: error occurred writing to NVS", req->uri);
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, NULL, 0),
        kTag, "POST %s: cannot send response", req->uri
    );

    return ESP_OK;
}
