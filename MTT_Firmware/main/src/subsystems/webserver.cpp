#include "subsystems/webserver.h"


#include "esp_check.h"
const char* WebServer::kTag = "webserver";

httpd_handle_t WebServer::m_server = NULL;

#ifndef CONFIG_HTTP_PORT
#define CONFIG_HTTP_PORT                           80
#endif

const httpd_uri_t* WebServer::kHandlers[] = {
    &kGetDriverState, &kSetDriverState,
    &kGetLEDBuffer,
    &kLEDBufferWS,
    &kGetLines, &kEnableLines, &kDisableLines,
    &kGetBoardImage, &kGetBoardView, &kGetIndex, &kGetRoot,
    &kConfigGetMAC_STA, &kConfigGetMAC_AP,
    &kConfigGetNTP, &kConfigGetMQTT,
    &kConfigGetMDNSHost, &kConfigGetMDNSInst,
    &kConfigGetWiFiSSID,
    &kGetBootstrapCSS
};

const httpd_uri_t* WebServer::kConfigHandlers[] = {
    &kConfigGetMAC_STA, &kConfigGetMAC_AP,
    &kConfigGetNTP, &kConfigSetNTP,
    &kConfigGetMQTT, &kConfigSetMQTT,
    &kConfigGetMDNSHost, &kConfigSetMDNSHost,
    &kConfigGetMDNSInst, &kConfigSetMDNSInst,
    &kConfigGetWiFiSSID, &kConfigGetWiFiIdentity, &kConfigGetWiFiUsername, &kConfigGetWiFiPassword, &kConfigGetWiFiCertificate,
    &kConfigSetWiFiUnauth, &kConfigSetWiFiPass, &kConfigSetWiFiEnt, &kConfigSetWiFiEntCert,
    &kConfigReset,
    &kGetBootstrapCSS
};

mdns_txt_item_t WebServer::kMDNSServiceTXT[] = {
    { "device", "melbtrains" },
    { "board", "esp32" }
};

esp_err_t WebServer::init(const char* hostname, const httpd_uri_t** handlers, size_t handlersCount, const char* instance) {
    ESP_RETURN_ON_ERROR(mdns_init(), kTag, "cannot initialise mDNS service");
    mdns_hostname_set(hostname);
    if (instance) mdns_instance_name_set(instance);
    ESP_RETURN_ON_ERROR(mdns_service_add(NULL, "_http", "_tcp", CONFIG_HTTP_PORT, kMDNSServiceTXT, sizeof(kMDNSServiceTXT) / sizeof(mdns_txt_item_t)), kTag, "cannot add HTTP service to mDNS");

    ESP_LOGI(kTag, "initialised mDNS service");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = CONFIG_HTTP_PORT;
    config.lru_purge_enable = true;
    config.max_uri_handlers = handlersCount;

    ESP_RETURN_ON_ERROR(httpd_start(&m_server, &config), kTag, "cannot initialise HTTP server");

    /* register URI handlers */
    for (size_t i = 0; i < config.max_uri_handlers; i++) {
        ESP_RETURN_ON_ERROR(
            httpd_register_uri_handler(m_server, handlers[i]),
            kTag, "cannot register URI handler for %s type %d", handlers[i]->uri, handlers[i]->method
        );
    }

    ESP_LOGI(kTag, "initialised HTTP server");
    return ESP_OK;
}

esp_err_t WebServer::init(const char* hostname, const char* instance) {
    return init(hostname, kHandlers, sizeof(kHandlers) / sizeof(httpd_uri_t*), instance);
}

esp_err_t WebServer::initConfig(const char* hostname, const char* instance) {
    return init(hostname, kConfigHandlers, sizeof(kConfigHandlers) / sizeof(httpd_uri_t*), instance);
}
