#include "subsystems/gtfsr.h"
#include "lsid.h"
#include "xxhash32.h"
#include "services.h"

#include <sys/time.h>

const char *GTFSR::kTag = "gtfs";

esp_http_client_handle_t GTFSR::m_metroClient;
esp_http_client_handle_t GTFSR::m_vlineClient;

StaticTask_t GTFSR::m_taskBuffer;
TaskHandle_t GTFSR::m_task;
StackType_t GTFSR::m_taskStack[CONFIG_GTFSR_TASK_STACK_SIZE];

/* padding duration (in seconds) after service departure at stations */
#ifndef CONFIG_MSG_STATION_PAD
#define CONFIG_MSG_STATION_PAD                         10
#endif

esp_err_t GTFSR::init(const char *apiKey) {
    /* unitialise metro client */
    esp_http_client_config_t config = {
        .url = CONFIG_METRO_UPDATES_FEED,
        .method = HTTP_METHOD_GET,
        .disable_auto_redirect = false
    };
    m_metroClient = esp_http_client_init(&config);
    ESP_RETURN_ON_FALSE(m_metroClient != NULL, ESP_ERR_INVALID_STATE, kTag, "cannot initialise metro client");
    esp_http_client_set_header(m_metroClient, "Accept", "*/*");
    esp_http_client_set_header(m_metroClient, "KeyId", apiKey);

    /* initialise V/Line client */
    config.url = CONFIG_VLINE_UPDATES_FEED;
    m_vlineClient = esp_http_client_init(&config);
    ESP_RETURN_ON_FALSE(m_vlineClient != NULL, ESP_ERR_INVALID_STATE, kTag, "cannot initialise V/Line client");
    esp_http_client_set_header(m_vlineClient, "Accept", "*/*");
    esp_http_client_set_header(m_vlineClient, "KeyId", apiKey);

    /* initialise response buffer mutex */
    m_respMutex = xSemaphoreCreateMutexStatic(&m_respMutexBuf);

    /* create update task */
    m_task = xTaskCreateStatic(updateTask, "gtfs", CONFIG_GTFSR_TASK_STACK_SIZE, NULL, CONFIG_GTFSR_TASK_PRIORITY, m_taskStack, &m_taskBuffer);

    return ESP_OK;
}

uint8_t GTFSR::m_respFragmentBuffer[CONFIG_GTFSR_RESPBUF_LEN];
size_t GTFSR::m_respFragmentLength = 0;
size_t GTFSR::m_respFragmentOffset = 0;
// size_t GTFSR::m_respLength = 0;

SemaphoreHandle_t GTFSR::m_respMutex;
StaticSemaphore_t GTFSR::m_respMutexBuf;

bool GTFSR::readStringCallback(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    struct ReadStringContext *ctx = (struct ReadStringContext *)*arg;

    size_t len = stream->bytes_left;
    if (len >= ctx->buflen) {
        ESP_LOGW(kTag, "string is %u characters long, beyond the maximum capacity of %u", len, ctx->buflen - 1);
        len = ctx->buflen - 1;
    }

    if (!pb_read(stream, (pb_byte_t *)ctx->buf, len)) {
        ESP_LOGE(kTag, "cannot read string (%s)", PB_GET_ERROR(stream));
        return false;
    }

    ctx->buf[len] = '\0'; // null terminate
    if (ctx->bufHash) *ctx->bufHash = XXHash32::hash(ctx->buf, len, 0);

    return true;
}

bool GTFSR::httpReadCallback(pb_istream_t *stream, pb_byte_t *buf, size_t count) {
    /* NOTE: we assume that m_respMutex has been claimed by us */

    esp_http_client_handle_t client = (esp_http_client_handle_t)stream->state;

    size_t offset = 0;
    size_t remaining = count;
    while (remaining > 0) {
        if (m_respFragmentOffset == m_respFragmentLength) { // already caught up
            int read_len = esp_http_client_read(client, (char *)m_respFragmentBuffer, CONFIG_GTFSR_RESPBUF_LEN);
            ESP_LOGD(kTag, "read response fragment of %d bytes", read_len);
            if (read_len < 0) {
                ESP_LOGE(kTag, "HTTP read error (%d)", read_len);
                return false;
            } else if (read_len == 0) {
                // ESP_LOGW(kTag, "possibly premature EOF (read %u bytes, expecting %u bytes) - total length so far: %u", offset, count, m_respLength);
                stream->bytes_left = 0; // nanopb will be the judge on whether the EOF is early
                return false;
            }

            m_respFragmentOffset = 0;
            m_respFragmentLength = read_len;
            // m_respLength += read_len;
        }

        size_t bufRemaining = m_respFragmentLength - m_respFragmentOffset;
        size_t len = (remaining < bufRemaining) ? remaining : bufRemaining;
        memcpy(&buf[offset], &m_respFragmentBuffer[m_respFragmentOffset], len);
        offset += len; m_respFragmentOffset += len; remaining -= len;
    }

    return true;
}

// NOTE: templated functions must be defined in header file

void GTFSR::updateTask(void *arg) {
    /* dry run to count number of entries */
    size_t initCount = 0;
    size_t initTotalCount = 0; // including skipped events

    time_t now; time(&now);
    time_t max_time = now + CONFIG_GTFSR_WINDOW; // timestamp window
    auto dryOnItem = [&](GTFSR::trip_update_item_t *item) { 
        initTotalCount++;
        if (item->arrival > max_time || (item->arrival < 0 && item->departure > max_time)) return; // skip event since it is beyond the window
        initCount++;
    };
    GTFSR::getMetroTripUpdates(dryOnItem);
    GTFSR::getVLineTripUpdates(dryOnItem);
    ESP_LOGI(kTag, "dry run complete, got %u trip updates (%u within window)", initTotalCount, initCount);
    Services::clearAndReserve(initCount * 2); // worst case scenario (1 arrival + 1 departure for each)

    while (true) {        
        Services::acquire(); Services::acquireUpdates();
        Services::clearAndReserve(0); // NOTE: services buffers are growth-only
        
        /* for storing last departing station to match with arrival */
        uint32_t lastIDHash = 0;
        infraid_t lastDepartingStation = 0;
        time_t lastDepartingTime = -1;

        size_t count = 0;
        time(&now);
        max_time = now + CONFIG_GTFSR_WINDOW;
        auto onItem = [&](GTFSR::trip_update_item_t *item) {
            infraid_t line = (item->id[0] == '0')
                ? INFRAID(&item->id[3]) // e.g. 02-BEG--19-T5-3637
                : INFRAID(&item->id[6]); // e.g. vic:02BEG:_:R:vpt._Belgrave_3420_20251016
            infraid_t stop = (item->stop[0] >= '0' && item->stop[1] <= '9')
                ? LSID::getStationFromStopID(atoi(item->stop)) // numeric stop ID
                : INFRAID(&item->stop[9]); // station ID given in stop ID (e.g. vic:rail:BOX)
            
            if (item->idHash != lastIDHash) {
                lastIDHash = item->idHash;
                lastDepartingStation = 0;
                lastDepartingTime = -1;
            }

            ESP_LOGD(kTag, "line " INFRAID2STR_FMT " trip 0x%08x stopping at " INFRAID2STR_FMT, INFRAID2STR(line), item->idHash, INFRAID2STR(stop));
            count++;

            if (item->arrival > max_time || (item->arrival < 0 && item->departure > max_time)) return; // skip event since it is beyond the window
            
            if (item->arrival >= 0) { // arriving at station
                Services::insertUpdate(item->idHash, ServiceState(line, item->arrival, stop)); // stopping
                if (lastDepartingStation) { // arriving from previous station
                    time_t departTime = lastDepartingTime + CONFIG_MSG_STATION_PAD;
                    Services::insertUpdate(item->idHash, ServiceState(line, departTime, lastDepartingStation, item->arrival, stop)); // in transit from prev. station to this one
                }
            }

            if (item->departure >= 0) { // departing from station
                lastDepartingStation = stop; lastDepartingTime = item->departure;
                // the connection to the next arriving station will be done when the next arrival event is received
            }
        };

        TickType_t t0 = xTaskGetTickCount();

        GTFSR::getMetroTripUpdates(onItem);
        // GTFSR::getVLineTripUpdates(onItem);

        TickType_t t1 = xTaskGetTickCount();

        ESP_LOGI(kTag, "read %u trip updates in %u ms", count, pdTICKS_TO_MS(t1 - t0));
        Services::release(); Services::releaseUpdates();
        vTaskDelay(pdMS_TO_TICKS(CONFIG_GTFSR_INTERVAL * 1000));
    }
}