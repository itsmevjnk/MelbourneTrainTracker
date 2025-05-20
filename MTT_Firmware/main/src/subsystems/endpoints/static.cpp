#include "subsystems/webserver.h"

#include "esp_check.h"
#include "esp_err.h"

esp_err_t WebServer::serveStaticData(httpd_req_t* req) {
    StaticData* data = (StaticData*)req->user_ctx;
    ESP_RETURN_ON_FALSE(data, ESP_ERR_INVALID_ARG, kTag, "user_ctx is null");

    if (data->mimeType) ESP_RETURN_ON_ERROR(httpd_resp_set_hdr(req, "Content-Type", data->mimeType), kTag, "cannot set Content-Type header");
    ESP_RETURN_ON_ERROR(httpd_resp_send(req, (const char*)data->start, data->length), kTag, "cannot send response");
    
    return ESP_OK;
}

extern const uint8_t board_jpg_start[] asm("_binary_board_jpg_start");
extern const uint8_t board_jpg_end[] asm("_binary_board_jpg_end");
const WebServer::StaticData WebServer::kBoardImage = {
    board_jpg_start,
    (uintptr_t)board_jpg_end - (uintptr_t)board_jpg_start,
    "image/jpeg"
};
const httpd_uri_t WebServer::kGetBoardImage = {
    .uri = "/board.jpg",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kBoardImage
};

extern const uint8_t board_htm_start[] asm("_binary_board_min_htm_start");
extern const uint8_t board_htm_end[] asm("_binary_board_min_htm_end");
const WebServer::StaticData WebServer::kBoardView = {
    board_htm_start,
    (uintptr_t)board_htm_end - (uintptr_t)board_htm_start,
    "text/html"
};
const httpd_uri_t WebServer::kGetBoardView = {
    .uri = "/board.htm",
    .method = HTTP_GET,
    .handler = serveStaticData,
    .user_ctx = (void*)&kBoardView
};

extern const uint8_t index_htm_start[] asm("_binary_index_min_htm_start");
extern const uint8_t index_htm_end[] asm("_binary_index_min_htm_end");
const WebServer::StaticData WebServer::kIndex = {
    index_htm_start,
    (uintptr_t)index_htm_end - (uintptr_t)index_htm_start,
    "text/html"
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