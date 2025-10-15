/* GTFS-R client */

#pragma once

#include "esp_check.h"
#include "esp_err.h"
#include "esp_http_client.h"

#include "nanopb/gtfs-realtime.pb.h"
#include "nanopb/pb_decode.h"

#include <type_traits>

class GTFSR {
public:
    static esp_err_t init(const char *apiKey);

    typedef struct {
        void *func; // only used internally for FeedEntity callback
        transit_realtime_FeedMessage *message;
        transit_realtime_TripDescriptor *trip;
    } item_cb_ctx_t; // item callback context

private:
    static const char *kTag; // for logging

    /* HTTP client handles */
    static esp_http_client_handle_t m_metroClient;
    static esp_http_client_handle_t m_vlineClient;

    static uint8_t *m_respBuffer;
    static size_t m_respBufferLen;
    static SemaphoreHandle_t m_respBufferMutex;
    static StaticSemaphore_t m_respBufferMutexBuffer;
    static size_t m_respLength;

    static esp_err_t performRequest(esp_http_client_handle_t client); // perform request and put result in m_respBuffer
    static esp_err_t clientEventHandler(esp_http_client_event_t *event);

    template <typename F>
    static bool decodeFeedEntityCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        transit_realtime_FeedEntity entity = transit_realtime_FeedEntity_init_zero;
        int status = pb_decode(stream, transit_realtime_FeedEntity_fields, &entity);
        if (!status) {
            ESP_LOGE(kTag, "cannot decode FeedEntity (%d - %s)", status, PB_GET_ERROR(&stream));
            return false;
        }

        entity.trip_update.stop_time_update.arg = *arg;

        return true;
    }

    template <typename F>
    static esp_err_t getTripUpdates(esp_http_client_handle_t client, F&& onItem) {
        while (!xSemaphoreTakeRecursive(m_respBufferMutex, portMAX_DELAY));

        esp_err_t err;
        if ((err = performRequest(m_metroClient)) != ESP_OK) {
            ESP_LOGE(kTag, "cannot make request: %s\n", esp_err_to_name(err));
            xSemaphoreGiveRecursive(m_respBufferMutex);
            return err;
        }

        transit_realtime_FeedMessage message = transit_realtime_FeedMessage_init_zero;
        message.entity.arg = &onItem;
        message.entity.funcs.decode = &GTFSR::decodeFeedEntityCallback<std::decay_t<F>>;

        pb_istream_t stream = pb_istream_from_buffer(m_respBuffer, m_respLength);
        while (stream.bytes_left > 0) {
            int status = pb_decode(&stream, transit_realtime_FeedMessage_fields, &message);
            if (!status) {
                ESP_LOGE(kTag, "cannot decode FeedMessage (%d - %s)", status, PB_GET_ERROR(&stream));
                xSemaphoreGiveRecursive(m_respBufferMutex);
                return ESP_ERR_INVALID_STATE;
            }

            
        }

        xSemaphoreGiveRecursive(m_respBufferMutex);
        return ESP_OK;
    }

public:
    template <typename F>
    static esp_err_t getMetroTripUpdates(F&& onItem) {
        return getTripUpdates(m_metroClient, onItem);
    }
};