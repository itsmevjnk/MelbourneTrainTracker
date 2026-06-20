#include "subsystems/webserver.h"
#include "subsystems/wifi.h"
#include "config.h"

#include "esp_check.h"
#include "esp_log.h"

const httpd_uri_t WebServer::kConfigGetWiFiSSID = {
    .uri = "/config/wifi/ssid",
    .method = HTTP_GET,
    .handler = configGetWiFiSSID,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetWiFiSSID(httpd_req_t* req) {
    const char* result = Config::getWiFiSSID();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigGetWiFiIdentity = {
    .uri = "/config/wifi/eap/id",
    .method = HTTP_GET,
    .handler = configGetWiFiIdentity,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetWiFiIdentity(httpd_req_t* req) {
    if (!Config::isWiFiEnterprise()) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_404(req),
            kTag, "GET %s: cannot respond", req->uri
        );
    } else {
        const char* result = Config::getWiFiIdentity();
        ESP_RETURN_ON_ERROR(
            httpd_resp_send(req, result, strlen(result)),
            kTag, "GET %s: cannot respond", req->uri
        );
    }
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigGetWiFiUsername = {
    .uri = "/config/wifi/eap/user",
    .method = HTTP_GET,
    .handler = configGetWiFiUsername,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetWiFiUsername(httpd_req_t* req) {
    if (!Config::isWiFiEnterprise()) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_404(req),
            kTag, "GET %s: cannot respond", req->uri
        );
    } else {
        const char* result = Config::getWiFiUsername();
        ESP_RETURN_ON_ERROR(
            httpd_resp_send(req, result, strlen(result)),
            kTag, "GET %s: cannot respond", req->uri
        );
    }
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigGetWiFiPassword = {
    .uri = "/config/wifi/password",
    .method = HTTP_GET,
    .handler = configGetWiFiPassword,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetWiFiPassword(httpd_req_t* req) {
    const char* result = Config::getWiFiPassword();
    ESP_RETURN_ON_ERROR(
        httpd_resp_send(req, result, strlen(result)),
        kTag, "GET %s: cannot respond", req->uri
    );
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigGetWiFiCertificate = {
    .uri = "/config/wifi/eap/cert",
    .method = HTTP_GET,
    .handler = configGetWiFiCertificate,
    .user_ctx = nullptr
};

esp_err_t WebServer::configGetWiFiCertificate(httpd_req_t* req) {
    if (!Config::isWiFiEnterprise()) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_404(req),
            kTag, "GET %s: cannot respond", req->uri
        );
    } else {
        size_t resultLen = Config::getWiFiCertLength();
        const char* result = Config::getWiFiCertificate();
        ESP_RETURN_ON_ERROR(
            httpd_resp_send(req, result, resultLen),
            kTag, "GET %s: cannot respond", req->uri
        );
    }
    return ESP_OK;
}

const httpd_uri_t WebServer::kConfigSetWiFiUnauth = {
    .uri = "/config/wifi/unauth",
    .method = HTTP_POST,
    .handler = configSetWiFiUnauth,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetWiFiUnauth(httpd_req_t* req) {
    char ssid[32];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) ssid, sizeof(ssid), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    ssid[len] = '\0';

    esp_err_t err = Config::setWiFiCredentials(ssid);
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

const httpd_uri_t WebServer::kConfigSetWiFiPass = {
    .uri = "/config/wifi/psk",
    .method = HTTP_POST,
    .handler = configSetWiFiPass,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetWiFiPass(httpd_req_t* req) {
    char buf[32 + 64];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) buf, sizeof(buf), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    buf[len] = '\0';

    char* saveptr = buf;
    char* ssid = strtok_r(buf, "\r\n", &saveptr); // NOTE: HTTP uses \r\n for newline
    char* password = strtok_r(nullptr, "\r\n", &saveptr);

    if (!ssid || !password) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ""),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }

    esp_err_t err = Config::setWiFiCredentials(ssid, password);
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

const httpd_uri_t WebServer::kConfigSetWiFiEnt = {
    .uri = "/config/wifi/eap",
    .method = HTTP_POST,
    .handler = configSetWiFiEnt,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetWiFiEnt(httpd_req_t* req) {
    char buf[32 + 64 + 64 + 64];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) buf, sizeof(buf), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    buf[len] = '\0';

    char* saveptr = buf;
    char* ssid = strtok_r(buf, "\r\n", &saveptr);
    char* identity = strtok_r(nullptr, "\r\n", &saveptr);
    char* username = strtok_r(nullptr, "\r\n", &saveptr);
    char* password = strtok_r(nullptr, "\r\n", &saveptr);

    if (!ssid || !identity || !username || !password) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ""),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }

    esp_err_t err = Config::setWiFiCredentials(ssid, identity, username, password);
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

const httpd_uri_t WebServer::kConfigSetWiFiEntCert = {
    .uri = "/config/wifi/eap_cert",
    .method = HTTP_POST,
    .handler = configSetWiFiEntCert,
    .user_ctx = nullptr
};

esp_err_t WebServer::configSetWiFiEntCert(httpd_req_t* req) {
    char buf[32 + 64 + 64 + 64 + 8192];

    size_t len;
    ESP_RETURN_ON_ERROR(
        readRequestBody(req, (uint8_t*) buf, sizeof(buf), &len),
        kTag, "POST %s: error occurred reading request body", req->uri
    );
    buf[len] = '\0';

    char* saveptr = buf;
    char* ssid = strtok_r(buf, "\r\n", &saveptr);
    char* identity = strtok_r(nullptr, "\r\n", &saveptr);
    char* username = strtok_r(nullptr, "\r\n", &saveptr);
    char* password = strtok_r(nullptr, "\r\n", &saveptr);
    char* cert = strtok_r(nullptr, "", &saveptr);

    if (!ssid || !identity || !username || !password || !cert) {
        ESP_RETURN_ON_ERROR(
            httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ""),
            kTag, "POST %s: cannot send error", req->uri
        );
        return ESP_OK;
    }

    size_t certLen = len - ((uintptr_t) cert - (uintptr_t) buf);

    esp_err_t err = Config::setWiFiCredentials(ssid, identity, username, password, cert, certLen);
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

const httpd_uri_t WebServer::kConfigWiFiScan = {
    .uri = "/config/wifi/scan",
    .method = HTTP_GET,
    .handler = configWiFiScan,
    .user_ctx = nullptr
};

esp_err_t WebServer::configWiFiScan(httpd_req_t* req) {
    wifi_ap_record_t* records = nullptr;
    size_t recordCount;
    ESP_RETURN_ON_ERROR(WiFi::scan(&records, &recordCount), kTag, "cannot scan for Wi-Fi networks");

    for (size_t i = 0; i < recordCount; i++) {
        char buf[6 * 2 + 5 + 2];
        int len = sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x\t", records[i].bssid[0], records[i].bssid[1], records[i].bssid[2], records[i].bssid[3], records[i].bssid[4], records[i].bssid[5]);
        httpd_resp_send_chunk(req, buf, len); // send BSSID
        httpd_resp_sendstr_chunk(req, (char*)records[i].ssid); // send SSID
        len = sprintf(buf, "\t%d\t", records[i].rssi);
        httpd_resp_send_chunk(req, buf, len); // send RSSI
        switch (records[i].authmode) {
            case WIFI_AUTH_OPEN: httpd_resp_sendstr_chunk(req, "Open\n"); break;
            case WIFI_AUTH_WPA_PSK: httpd_resp_sendstr_chunk(req, "WPA-PSK\n"); break;
            case WIFI_AUTH_WPA2_PSK: httpd_resp_sendstr_chunk(req, "WPA2-PSK\n"); break;
            case WIFI_AUTH_WPA_WPA2_PSK: httpd_resp_sendstr_chunk(req, "WPA/WPA2-PSK\n"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: httpd_resp_sendstr_chunk(req, "WPA2-Enterprise\n"); break;
            case WIFI_AUTH_WPA2_WPA3_PSK: httpd_resp_sendstr_chunk(req, "WPA2/WPA3-PSK\n"); break;
            default: httpd_resp_sendstr_chunk(req, "\n"); break;
        }
    }
    httpd_resp_send_chunk(req, NULL, 0); // stop sending

    free(records);
    return ESP_OK;
}