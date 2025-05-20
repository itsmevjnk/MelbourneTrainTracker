#include "subsystems/webserver.h"


#include "esp_check.h"
const char* WebServer::kTag = "webserver";

httpd_handle_t WebServer::m_server = NULL;

#ifndef HTTP_PORT
#define HTTP_PORT                           80
#endif

const httpd_uri_t* WebServer::kHandlers[] = {
    &kGetDriverState, &kSetDriverState,
    &kGetLEDBuffer,
    &kLEDBufferWS,
    &kGetBoardImage, &kGetBoardView, &kGetIndex, &kGetRoot
};

mdns_txt_item_t WebServer::kMDNSServiceTXT[] = {
    { "device", "melbtrains" },
    { "board", "esp32" }
};

esp_err_t WebServer::init(const char* hostname, const char* instance) {
    ESP_RETURN_ON_ERROR(mdns_init(), kTag, "cannot initialise mDNS service");
    mdns_hostname_set(hostname);
    if (instance) mdns_instance_name_set(instance);
    ESP_RETURN_ON_ERROR(mdns_service_add(NULL, "_http", "_tcp", HTTP_PORT, kMDNSServiceTXT, sizeof(kMDNSServiceTXT) / sizeof(mdns_txt_item_t)), kTag, "cannot add HTTP service to mDNS");

    ESP_LOGI(kTag, "initialised mDNS service");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = HTTP_PORT;
    config.lru_purge_enable = true;

    ESP_RETURN_ON_ERROR(httpd_start(&m_server, &config), kTag, "cannot initialise HTTP server");

    /* register URI handlers */
    for (size_t i = 0; i < sizeof(kHandlers) / sizeof(httpd_uri_t*); i++) {
        ESP_RETURN_ON_ERROR(
            httpd_register_uri_handler(m_server, kHandlers[i]),
            kTag, "cannot register URI handler for %s type %d", kHandlers[i]->uri, kHandlers[i]->method
        );
    }

    ESP_LOGI(kTag, "initialised HTTP server");
    return ESP_OK;
}
