/* HTTP server for config & status */

#pragma once

#include "esp_http_server.h"
#include "mdns.h"

class WebServer {
public:
    static esp_err_t init(const char* hostname, const char* instance = nullptr); // in normal mode
    static esp_err_t initConfig(const char* hostname, const char* instance = nullptr); // in configuration mode
    static esp_err_t initConfig(); // in configuration mode, no mDNS

    static esp_err_t init(const char* hostname, const httpd_uri_t** handlers, size_t handlersCount, const char* instance = nullptr);

    static esp_err_t sendLEDBufferAsync(); // trigger sending LED buffer over WebSockets (asynchronously)
private:
    struct StaticData {
        const void* start;
        size_t length;
        const char* mimeType;
        bool gzip;
    };

    /* HTTP endpoints */
    static const httpd_uri_t kGetDriverState;
    static esp_err_t getDriverState(httpd_req_t* req); 

    static const httpd_uri_t kSetDriverState;
    static esp_err_t setDriverState(httpd_req_t* req);

    static const httpd_uri_t kGetLEDBuffer;
    static esp_err_t getLEDBuffer(httpd_req_t* req);

    static const httpd_uri_t kGetReverseLUT;
    static esp_err_t getReverseLUT(httpd_req_t* req);

    static const httpd_uri_t kConfigGetMAC_STA;
    static esp_err_t configGetMAC_STA(httpd_req_t* req);

    static const httpd_uri_t kConfigGetMAC_AP;
    static esp_err_t configGetMAC_AP(httpd_req_t* req);

    static esp_err_t configGetMAC(httpd_req_t* req, bool ap); // stub

    static const httpd_uri_t kConfigGetNTP;
    static esp_err_t configGetNTP(httpd_req_t* req);

    static const httpd_uri_t kConfigSetNTP;
    static esp_err_t configSetNTP(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetMQTT;
    static esp_err_t configGetMQTT(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetMQTT;
    static esp_err_t configSetMQTT(httpd_req_t* req);

    static const httpd_uri_t kConfigGetMDNSHost;
    static esp_err_t configGetMDNSHost(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetMDNSHost;
    static esp_err_t configSetMDNSHost(httpd_req_t* req);

    static const httpd_uri_t kConfigGetMDNSInst;
    static esp_err_t configGetMDNSInst(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetMDNSInst;
    static esp_err_t configSetMDNSInst(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetWiFiSSID;
    static esp_err_t configGetWiFiSSID(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetWiFiIdentity;
    static esp_err_t configGetWiFiIdentity(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetWiFiUsername;
    static esp_err_t configGetWiFiUsername(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetWiFiPassword;
    static esp_err_t configGetWiFiPassword(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetWiFiCertificate;
    static esp_err_t configGetWiFiCertificate(httpd_req_t* req);

    static const httpd_uri_t kConfigWiFiScan;
    static esp_err_t configWiFiScan(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetWiFiUnauth;
    static esp_err_t configSetWiFiUnauth(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetWiFiPass;
    static esp_err_t configSetWiFiPass(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetWiFiEnt;
    static esp_err_t configSetWiFiEnt(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetWiFiEntCert;
    static esp_err_t configSetWiFiEntCert(httpd_req_t* req);
    
    static const httpd_uri_t kConfigReset;
    static esp_err_t configReset(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetBuild;
    static esp_err_t configGetBuild(httpd_req_t* req);
    
    static const httpd_uri_t kConfigGetUptime;
    static esp_err_t configGetUptime(httpd_req_t* req);

    static esp_err_t readRequestBody(httpd_req_t* req, uint8_t* buf, size_t len, size_t* readLen);

    static const httpd_uri_t kLEDBufferWS;
    static esp_err_t wsLEDBuffer(httpd_req_t* req);
    static void sendLEDBufferWorker(void* arg); // worker function called by httpd

    static size_t getLinesString(char* result, size_t resultLen);

    static const httpd_uri_t kGetLines;
    static esp_err_t getLines(httpd_req_t* req);

    static const httpd_uri_t kEnableLines;
    static const httpd_uri_t kDisableLines;
    static esp_err_t setLines(httpd_req_t* req);

    static const httpd_uri_t kConfigGetBrightnessMode;
    static esp_err_t configGetBrightnessMode(httpd_req_t* req);
    
    static const httpd_uri_t kConfigSetBrightnessMode;
    static esp_err_t configSetBrightnessMode(httpd_req_t* req);

    static const httpd_uri_t kGetBrightness;
    static esp_err_t getBrightness(httpd_req_t* req);

    static const httpd_uri_t kSetBrightness;
    static esp_err_t setBrightness(httpd_req_t* req);

    static esp_err_t serveStaticData(httpd_req_t* req);

    static const StaticData kBoardImage;
    static const httpd_uri_t kGetBoardImage;
    static const StaticData kBoardView;
    static const httpd_uri_t kGetBoardView;
    static const StaticData kIndex;
    static const httpd_uri_t kGetIndex;
    static const httpd_uri_t kGetRoot;
    static const StaticData kBootstrapCSS;
    static const httpd_uri_t kGetBootstrapCSS;

    static const StaticData kConfigIndex;
    static const httpd_uri_t kGetConfigIndex;
    static const httpd_uri_t kGetConfigRoot;

    static httpd_handle_t m_server;

    static const httpd_uri_t* kHandlers[];
    static const httpd_uri_t* kConfigHandlers[];

    static mdns_txt_item_t kMDNSServiceTXT[];

    static const char* kTag;
};