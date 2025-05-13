/* HTTP server for config & status */

#pragma once

#include "esp_http_server.h"

class WebServer {
public:
    static esp_err_t init();

private:
    /* HTTP endpoints */
    static const httpd_uri_t kGetDriverState;
    static esp_err_t getDriverState(httpd_req_t* req); 

    static const httpd_uri_t kSetDriverState;
    static esp_err_t setDriverState(httpd_req_t* req);

    static httpd_handle_t m_server;
    static const httpd_uri_t* kHandlers[];

    static const char* kTag;
};