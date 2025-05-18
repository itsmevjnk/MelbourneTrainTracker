/* HTTP server for config & status */

#pragma once

#include "esp_http_server.h"
#include "mdns.h"

class WebServer {
public:
    static esp_err_t init(const char* hostname, const char* instance = nullptr);

    static esp_err_t sendLEDBufferAsync(); // trigger sending LED buffer over WebSockets (asynchronously)
private:
    struct StaticData {
        const void* start;
        size_t length;
        const char* mimeType;
    };

    /* HTTP endpoints */
    static const httpd_uri_t kGetDriverState;
    static esp_err_t getDriverState(httpd_req_t* req); 

    static const httpd_uri_t kSetDriverState;
    static esp_err_t setDriverState(httpd_req_t* req);

    static const httpd_uri_t kGetLEDBuffer;
    static esp_err_t getLEDBuffer(httpd_req_t* req);

    static const httpd_uri_t kLEDBufferWS;
    static esp_err_t wsLEDBuffer(httpd_req_t* req);
    static void sendLEDBufferWorker(void* arg); // worker function called by httpd

    static esp_err_t serveStaticData(httpd_req_t* req);

    static const StaticData kBoardImage;
    static const httpd_uri_t kGetBoardImage;
    static const StaticData kBoardView;
    static const httpd_uri_t kGetBoardView;

    static httpd_handle_t m_server;
    static const httpd_uri_t* kHandlers[];

    static mdns_txt_item_t kMDNSServiceTXT[];

    static const char* kTag;
};