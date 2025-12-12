#include "subsystems/webserver.h"

#include "esp_mac.h"
#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigGetMAC_AP = {
    .uri = "/config/mac/ap",
    .method = HTTP_GET,
    .handler = configGetMAC_AP,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetMAC_AP(httpd_req_t* req) {
    return configGetMAC(req, true);
}

const httpd_uri_t WebServer::kConfigGetMAC_STA = {
    .uri = "/config/mac/sta",
    .method = HTTP_GET,
    .handler = configGetMAC_STA,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetMAC_STA(httpd_req_t* req) {
    return configGetMAC(req, false);
}

esp_err_t WebServer::configGetMAC(httpd_req_t* req, bool ap) {
    char result[2 * 6 + 5 + 1]; // ASCII representation: 01:23:45:67:89:ab \0
    size_t resultLen = 0;

    uint8_t mac[6];
    esp_err_t err = esp_read_mac(mac, (ap) ? ESP_MAC_WIFI_SOFTAP : ESP_MAC_WIFI_STA);
    if (err != ESP_OK) {
        ESP_LOGE(kTag, "GET %s: cannot read MAC address (%d)", req->uri);
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, ""),
            kTag, "GET %s: cannot send error", req->uri
        );
        return ESP_FAIL;
    } else {
        resultLen = sprintf(result, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, resultLen),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}