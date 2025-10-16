/* GTFS-R client */

#pragma once

#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_check.h"

#include "nanopb/gtfs-realtime.pb.h"
#include "nanopb/pb_decode.h"

#include <type_traits>

#ifndef CONFIG_GTFSR_TRIPID_BUFFER_LEN
#define CONFIG_GTFSR_TRIPID_BUFFER_LEN                  48
#endif

#ifndef CONFIG_GTFSR_STOPID_BUFFER_LEN
#define CONFIG_GTFSR_STOPID_BUFFER_LEN                  20
#endif

#ifndef CONFIG_GTFSR_RESPBUF_LEN
#define CONFIG_GTFSR_RESPBUF_LEN                        1024
#endif

#ifndef CONFIG_GTFSR_TASK_STACK_SIZE
#define CONFIG_GTFSR_TASK_STACK_SIZE                    16384
#endif

#ifndef CONFIG_GTFSR_TASK_PRIORITY
#define CONFIG_GTFSR_TASK_PRIORITY                      8
#endif

#ifndef CONFIG_GTFSR_INTERVAL
#define CONFIG_GTFSR_INTERVAL                           30
#endif

class GTFSR {
public:
    static esp_err_t init(const char *apiKey);

    typedef struct {
        const char *id; // trip ID
        // int seq; // stop sequence
        char stop[CONFIG_GTFSR_STOPID_BUFFER_LEN];
        int64_t arrival; // arrival timestamp (-1 if not available)
        int64_t departure; // departure timestamp (-1 if not available)
    } trip_update_item_t; // iterated trip update item
private:
    static const char *kTag; // for logging

    /* HTTP client handles */
    static esp_http_client_handle_t m_metroClient;
    static esp_http_client_handle_t m_vlineClient;

    /* response fragment buffer */
    static uint8_t m_respFragmentBuffer[CONFIG_GTFSR_RESPBUF_LEN];
    static size_t m_respFragmentLength;
    static size_t m_respFragmentOffset;
    // static size_t m_respLength; // total response length - for verification
    static SemaphoreHandle_t m_respMutex;
    static StaticSemaphore_t m_respMutexBuf;

    struct FeedEntityContext {
        void *func; // item callback function
        char id[CONFIG_GTFSR_TRIPID_BUFFER_LEN]; // trip ID
    }; // feed entity context for decodeStopTimeUpdateCallback

    struct ReadStringContext {
        char *buf;
        size_t buflen; // incl. null termination
    }; // context for readStringCallbacl

    static bool readStringCallback(pb_istream_t *stream, const pb_field_t *field, void **arg);

    template <typename F>
    static bool decodeStopTimeUpdateCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
        transit_realtime_TripUpdate_StopTimeUpdate update = transit_realtime_TripUpdate_StopTimeUpdate_init_zero;
        struct FeedEntityContext *ctx = (struct FeedEntityContext *)*arg;

        trip_update_item_t item = { ctx->id }; // trip ID from context (guaranteed to be in stack)

        struct ReadStringContext stopid_ctx = { item.stop, CONFIG_GTFSR_STOPID_BUFFER_LEN };
        update.stop_id.arg = &stopid_ctx;
        update.stop_id.funcs.decode = &GTFSR::readStringCallback;

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

        struct FeedEntityContext ctx = {
            *arg // callback function passed from getTripUpdates
            // ID will be read by readTripIDCallback
        };

        struct ReadStringContext tripid_ctx = { ctx.id, CONFIG_GTFSR_TRIPID_BUFFER_LEN };
        entity.trip_update.trip.trip_id.arg = &tripid_ctx;
        entity.trip_update.trip.trip_id.funcs.decode = &GTFSR::readStringCallback;

        entity.trip_update.stop_time_update.arg = &ctx;
        entity.trip_update.stop_time_update.funcs.decode = &GTFSR::decodeStopTimeUpdateCallback<std::decay_t<F>>;

        if (!pb_decode(stream, transit_realtime_FeedEntity_fields, &entity)) {
            ESP_LOGE(kTag, "cannot decode FeedEntity (%s)", PB_GET_ERROR(stream));
            return false;
        }

        return true;
    }

    static bool httpReadCallback(pb_istream_t *stream, pb_byte_t *buf, size_t count);

    template <typename F>
    static esp_err_t getTripUpdates(esp_http_client_handle_t client, F&& onItem) {
        ESP_RETURN_ON_ERROR(
            esp_http_client_flush_response(client, NULL),
            kTag, "cannot flush client response"
        );

        while (!xSemaphoreTake(m_respMutex, portMAX_DELAY)); // wait until response buffer is available

        esp_err_t err = esp_http_client_open(client, 0);
        if (err != ESP_OK) {
            ESP_LOGE(kTag, "esp_http_client_open() failed (%s)", esp_err_to_name(err));
            xSemaphoreGive(m_respMutex);
            return err;
        }

        int status = esp_http_client_fetch_headers(client);
        if (status < 0) {
            ESP_LOGE(kTag, "esp_http_client_fetch_headers() failed (%d)", status);
            xSemaphoreGive(m_respMutex);
            return ESP_ERR_INVALID_RESPONSE;
        }


        transit_realtime_FeedMessage message = transit_realtime_FeedMessage_init_zero;
        message.entity.arg = &onItem;
        message.entity.funcs.decode = &GTFSR::decodeFeedEntityCallback<std::decay_t<F>>;

        pb_istream_t stream = { &GTFSR::httpReadCallback, client, SIZE_MAX }; // custom stream
        m_respFragmentOffset = m_respFragmentLength = 0; // reset fragment buffer
        // m_respLength = 0;
        if (!pb_decode(&stream, transit_realtime_FeedMessage_fields, &message)) {
            ESP_LOGE(kTag, "cannot decode FeedMessage (%s)", PB_GET_ERROR(&stream));
            esp_http_client_close(client);
            xSemaphoreGive(m_respMutex);
            return ESP_ERR_INVALID_STATE;
        }

        err = esp_http_client_close(client);
        if (err != ESP_OK) {
            ESP_LOGE(kTag, "esp_http_client_close() failed (%s)", esp_err_to_name(err));
            xSemaphoreGive(m_respMutex);
            return err;
        }

        xSemaphoreGive(m_respMutex);
        return ESP_OK;
    }

    /* update task */
    static StaticTask_t m_taskBuffer;
    static TaskHandle_t m_task;
    static StackType_t m_taskStack[CONFIG_GTFSR_TASK_STACK_SIZE];
    static void updateTask(void *arg);

public:
    template <typename F>
    static esp_err_t getMetroTripUpdates(F&& onItem) {
        return getTripUpdates(m_metroClient, onItem);
    }

    template <typename F>
    static esp_err_t getVLineTripUpdates(F&& onItem) {
        return getTripUpdates(m_vlineClient, onItem);
    }
};