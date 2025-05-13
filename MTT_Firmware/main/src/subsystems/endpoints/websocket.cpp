#include "subsystems/webserver.h"
#include "subsystems/led_matrix.h"

const httpd_uri_t WebServer::kLEDBufferWS = {
    .uri = "/ws/buffer",
    .method = HTTP_GET,
    .handler = wsLEDBuffer,
    .user_ctx = nullptr,
    .is_websocket = true
};

esp_err_t WebServer::wsLEDBuffer(httpd_req_t* req) {
    if (req->method == HTTP_GET) ESP_LOGI(kTag, "new WebSocket connection from client fd %d", httpd_req_to_sockfd(req));
    return ESP_OK; // TODO: anything else we can do here?
}

void WebServer::sendLEDBufferWorker(void* arg) {
    LEDMatrix::acquireBuffer();
    httpd_ws_frame_t frame; memset(&frame, 0, sizeof(httpd_ws_frame_t));
    frame.payload = LEDMatrix::m_buffer;
    frame.len = LMAT_SIZE;
    frame.type = HTTPD_WS_TYPE_BINARY;
    int fd = (int)arg; // fd is passed in as arg
    esp_err_t ret = httpd_ws_send_frame_async(m_server, fd, &frame);
    LEDMatrix::releaseBuffer();
    if (ret != ESP_OK) ESP_LOGE(kTag, "cannot send WebSocket data to client fd %d (%s)", fd, esp_err_to_name(ret));
    else ESP_LOGD(kTag, "sent WebSocket data to client %d", fd);
}

esp_err_t WebServer::sendLEDBufferAsync() {
    /* get all FDs of connected clients */
    int clientFDs[CONFIG_LWIP_MAX_LISTENING_TCP];
    size_t fds = CONFIG_LWIP_MAX_LISTENING_TCP;
    ESP_RETURN_ON_ERROR(httpd_get_client_list(m_server, &fds, clientFDs), kTag, "cannot obtain client list");

    for (size_t i = 0; i < fds; i++) {
        httpd_ws_client_info_t info = httpd_ws_get_fd_info(m_server, clientFDs[i]);
        if (info == HTTPD_WS_CLIENT_WEBSOCKET) {
            ESP_LOGD(kTag, "staging WebSocket transmission for client fd %d", clientFDs[i]);
            ESP_RETURN_ON_ERROR(httpd_queue_work(m_server, sendLEDBufferWorker, (void*)clientFDs[i]), kTag, "cannot stage WebSocket transmission for client fd %d", clientFDs[i]);
        }
    }

    return ESP_OK;
}

