#include "subsystems/webserver.h"

#include "esp_check.h"
#include "esp_err.h"

esp_err_t WebServer::serveStaticData(httpd_req_t* req) {
    StaticData* data = (StaticData*)req->user_ctx;
    ESP_RETURN_ON_FALSE(data, ESP_ERR_INVALID_ARG, kTag, "user_ctx is null");

    if (data->mimeType) ESP_RETURN_ON_ERROR(httpd_resp_set_type(req, data->mimeType), kTag, "cannot set Content-Type header");
    if (data->gzip) ESP_RETURN_ON_ERROR(httpd_resp_set_hdr(req, "Content-Encoding", "gzip"), kTag, "cannot set Content-Encoding header");
    ESP_RETURN_ON_ERROR(httpd_resp_send(req, (const char*)data->start, data->length), kTag, "cannot send response");
    
    return ESP_OK;
}

#if CONFIG_IDF_TARGET_ESP32S3
#define REVISION "rev2"
#else
#define REVISION "rev1"
#endif

extern const uint8_t board_svg_start[] asm("_binary_board_" REVISION "_svg_gz_start");
extern const uint8_t board_svg_end[] asm("_binary_board_" REVISION "_svg_gz_end");
const WebServer::StaticData WebServer::kBoardImage = {
    board_svg_start,
    (uintptr_t)board_svg_end - (uintptr_t)board_svg_start,
    "image/svg+xml", true
};
const httpd_uri_t WebServer::kGetBoardImage = {
    .uri = "/board.svg",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kBoardImage
};

extern const uint8_t board_htm_start[] asm("_binary_board_" REVISION "_htm_gz_start");
extern const uint8_t board_htm_end[] asm("_binary_board_" REVISION "_htm_gz_end");
const WebServer::StaticData WebServer::kBoardView = {
    board_htm_start,
    (uintptr_t)board_htm_end - (uintptr_t)board_htm_start,
    "text/html", true
};
const httpd_uri_t WebServer::kGetBoardView = {
    .uri = "/board.htm",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kBoardView
};

extern const uint8_t index_htm_start[] asm("_binary_index_htm_gz_start");
extern const uint8_t index_htm_end[] asm("_binary_index_htm_gz_end");
const WebServer::StaticData WebServer::kIndex = {
    index_htm_start,
    (uintptr_t)index_htm_end - (uintptr_t)index_htm_start,
    "text/html", true
};
const httpd_uri_t WebServer::kGetIndex = {
    .uri = "/index.htm",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kIndex
};
const httpd_uri_t WebServer::kGetRoot = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kIndex
};

extern const uint8_t bootstrap_min_css_start[] asm("_binary_bootstrap_min_css_gz_start");
extern const uint8_t bootstrap_min_css_end[] asm("_binary_bootstrap_min_css_gz_end");
const WebServer::StaticData WebServer::kBootstrapCSS = {
    bootstrap_min_css_start,
    (uintptr_t)bootstrap_min_css_end - (uintptr_t)bootstrap_min_css_start,
    "text/css", true
};
const httpd_uri_t WebServer::kGetBootstrapCSS = {
    .uri = "/bootstrap.min.css",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kBootstrapCSS
};

extern const uint8_t config_htm_start[] asm("_binary_config_htm_gz_start");
extern const uint8_t config_htm_end[] asm("_binary_config_htm_gz_end");
const WebServer::StaticData WebServer::kConfigIndex = {
    config_htm_start,
    (uintptr_t)config_htm_end - (uintptr_t)config_htm_start,
    "text/html", true
};
const httpd_uri_t WebServer::kGetConfigIndex = {
    .uri = "/index.htm",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kConfigIndex
};
const httpd_uri_t WebServer::kGetConfigRoot = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kConfigIndex
};