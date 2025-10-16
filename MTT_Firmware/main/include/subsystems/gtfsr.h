/* GTFS-R client */

#pragma once

#include "esp_check.h"
#include "esp_err.h"
#include "esp_http_client.h"

#include "nanopb/gtfs-realtime.pb.h"
#include "nanopb/pb_decode.h"

#include <type_traits>

#ifndef CONFIG_GTFSR_ID_BUFFER_LEN
#define CONFIG_GTFSR_ID_BUFFER_LEN                  24
#endif

class GTFSR {
public:
    static esp_err_t init(const char *apiKey);

    typedef struct {
        const char *id; // trip ID
        // int seq; // stop sequence
        int stop; // stop ID
        int64_t arrival; // arrival timestamp (-1 if not available)
        int64_t departure; // departure timestamp (-1 if not available)
    } trip_update_item_t; // iterated trip update item
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

    struct feed_entity_ctx {
        void *func; // item callback function
        char id[CONFIG_GTFSR_ID_BUFFER_LEN]; // trip ID
    }; // feed entity context for decodeStopTimeUpdateCallback

    static bool readTripIDCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        char *idBuffer = (char *)*arg;
        size_t len = stream->bytes_left;
        if (len >= CONFIG_GTFSR_ID_BUFFER_LEN) {
            ESP_LOGW(kTag, "trip ID is %u characters long, beyond the maximum capacity of %u", len, CONFIG_GTFSR_ID_BUFFER_LEN - 1);
            len = CONFIG_GTFSR_ID_BUFFER_LEN - 1;
        }

        if (!pb_read(stream, (pb_byte_t *)idBuffer, len)) {
            ESP_LOGE(kTag, "cannot read trip ID (%s)", PB_GET_ERROR(stream));
            return false;
        }

        idBuffer[len] = '\0'; // null terminate
        return true;
    }

    static bool readStopIDCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        char idBuffer[11]; // cannot exceed 10 characters (in fact, the stop ID is only 5 digits)
        // NOTE: this will break if the stop ID is non-integer, but for TVOD this should be fine

        size_t len = stream->bytes_left;
        if (len >= sizeof(idBuffer)) {
            ESP_LOGW(kTag, "stop ID is %u characters long, beyond the maximum capacity of %u", len, sizeof(idBuffer) - 1);
            len = sizeof(idBuffer) - 1;
        }

        if (!pb_read(stream, (pb_byte_t *)idBuffer, len)) {
            ESP_LOGE(kTag, "cannot read stop ID (%s)", PB_GET_ERROR(stream));
            return false;
        }

        idBuffer[len] = '\0'; // null terminate
        int *id = (int *)*arg;
        *id = atoi(idBuffer); // NOTE: we null terminate ourselves so this should be ok

        return true;
    }

    template <typename F>
    static bool decodeStopTimeUpdateCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        transit_realtime_TripUpdate_StopTimeUpdate update = transit_realtime_TripUpdate_StopTimeUpdate_init_zero;
        struct feed_entity_ctx *ctx = (struct feed_entity_ctx *)*arg;

        trip_update_item_t item = { ctx->id }; // trip ID from context (guaranteed to be in stack)

        update.stop_id.arg = &item.stop;
        update.stop_id.funcs.decode = &GTFSR::readStopIDCallback;

        if (!pb_decode(stream, transit_realtime_TripUpdate_StopTimeUpdate_fields, &update)) {
            ESP_LOGE(kTag, "cannot decode StopTimeUpdate (%s)", PB_GET_ERROR(stream));
            return false;
        }

        item.arrival = (update.has_arrival && update.arrival.has_time) ? update.arrival.time : -1;
        item.departure = (update.has_departure && update.departure.has_time) ? update.departure.time : -1;

        auto *func = reinterpret_cast<F*>(ctx->func);
        (*func)(&item);

        return true;
    }
    
    template <typename F>
    static bool decodeFeedEntityCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        transit_realtime_FeedEntity entity = transit_realtime_FeedEntity_init_zero;

        struct feed_entity_ctx ctx = {
            *arg // callback function passed from getTripUpdates
            // ID will be read by readTripIDCallback
        };

        entity.trip_update.trip.trip_id.arg = ctx.id;
        entity.trip_update.trip.trip_id.funcs.decode = &GTFSR::readTripIDCallback;

        entity.trip_update.stop_time_update.arg = &ctx;
        entity.trip_update.stop_time_update.funcs.decode = &GTFSR::decodeStopTimeUpdateCallback<std::decay_t<F>>;

        if (!pb_decode(stream, transit_realtime_FeedEntity_fields, &entity)) {
            ESP_LOGE(kTag, "cannot decode FeedEntity (%s)", PB_GET_ERROR(stream));
            return false;
        }

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
            if (!pb_decode(&stream, transit_realtime_FeedMessage_fields, &message)) {
                ESP_LOGE(kTag, "cannot decode FeedMessage (%s)", PB_GET_ERROR(&stream));
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