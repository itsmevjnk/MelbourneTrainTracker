#include "message.h"
#include "services.h"

#include <cJSON.h>

#include "esp_log.h"
#include "esp_check.h"
const char* Message::kTag = "msg";

// ===== XXHASH32 COPYRIGHT NOTICE =====
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <xxhash32.h>

static inline time_t getTimestamp(const cJSON* item) {
    double value = cJSON_GetNumberValue(item);
    return (value == NAN) ? INT64_MIN : static_cast<time_t>(std::floor(value)); // floor just in case
}

static inline int getInteger(const cJSON* item) {
    double value = cJSON_GetNumberValue(item);
    return (value == NAN) ? INT_MIN : static_cast<int>(std::floor(value)); // floor just in case
}

/* padding duration (in seconds) after service departure at stations */
#ifndef MSG_STATION_PAD
#define MSG_STATION_PAD                         10
#endif

void Message::parseMessage(const char* buffer, size_t bufferLength) {
    cJSON* json = cJSON_ParseWithLength(buffer, bufferLength);
    if (json == NULL) {
        const char* errorPtr = cJSON_GetErrorPtr();
        if (errorPtr != NULL) ESP_LOGE(kTag, "error parsing message (m_buffer=0x%x) before (0x%x) %s", (uintptr_t)buffer, (uintptr_t)errorPtr, errorPtr);
        else ESP_LOGE(kTag, "unspecified error parsing message (m_buffer=0x%x)", (uintptr_t)buffer);
        ESP_LOGE(kTag, "m_buffer contents: %s", buffer);
        ESP_LOGE(kTag, "available memory: %lu bytes", esp_get_minimum_free_heap_size());
        abort();
    }

    assert(cJSON_IsArray(json)); // make sure that we've got an array and not something else

    ESP_LOGD(kTag, "available memory: %lu bytes", esp_get_minimum_free_heap_size());

    /* iterate through events - this is effectively cJSON_ArrayForEach, but we remove each element after parsing */
    const cJSON* event = NULL;
    size_t numEvents = 0;
    cJSON_ArrayForEach(event, json) {
        assert(cJSON_IsObject(event));

        /* values we'll read from the object */
        const char* line = nullptr;
        const char* station = nullptr;
        time_t timestamp = INT64_MIN;
        bool isDeparture = false;
        const char* adjStation = nullptr;
        time_t adjTimestamp = INT64_MIN;
        const char* tripID = nullptr;
        int seq = 0;

        const cJSON* property = NULL;
        cJSON_ArrayForEach(property, event) {
            const char* key = property->string; // property key
            if (!strcmp(key, "seq")) {
                seq = getInteger(property);
            }
            else if (!strcmp(key, "line")) {
                line = cJSON_GetStringValue(property);
            }
            else if (!strcmp(key, "trip")) {
                tripID = cJSON_GetStringValue(property);
            }
            else if (!strcmp(key, "stn")) {
                station = cJSON_GetStringValue(property);
            }
            else if (!strcmp(key, "dep")) {
                isDeparture = true;
                timestamp = getTimestamp(property);
            }
            else if (!strcmp(key, "arr")) {
                isDeparture = false;
                timestamp = getTimestamp(property);
            }
            else if (!strcmp(key, "adj")) {
                adjStation = cJSON_GetStringValue(property);
            }
            else if (!strcmp(key, "adjt")) {
                adjTimestamp = getTimestamp(property);
            }
        }

        assert(seq >= 0 && line && station && tripID && timestamp >= 0); // make sure that the required properties are valid

        ESP_LOGV(kTag, "trip %s at %lld: %s %s event at %s", tripID, timestamp, line, (isDeparture) ? "departure" : "arrival", station);
        if (adjStation) ESP_LOGV(kTag, "next %s at %s on %lld", (isDeparture) ? "arrival" : "departure", adjStation, adjTimestamp);

        infraid_t lineID = INFRAID(line);
        infraid_t stationID = INFRAID(station);
        infraid_t adjStationID = (adjStation == NULL) ? 0 : INFRAID(adjStation);
        uint32_t tripHash = XXHash32::hash(tripID, strlen(tripID), 0);
        
        if (isDeparture) { // departing station
            time_t departTime = timestamp + MSG_STATION_PAD;
            if (adjStation == NULL) { // leaving into oblivion (i.e. end of service)
                Services::insertUpdate(tripHash, seq, ServiceState(lineID, departTime, stationID, departTime, 0)); // signal to Services::updateStates to delete service
            } else { // departing to another station
                Services::insertUpdate(tripHash, seq, ServiceState(lineID, departTime, stationID, adjTimestamp, adjStationID)); // in transit
            }
        } else { // arriving at station
            Services::insertUpdate(tripHash, seq, ServiceState(lineID, timestamp, stationID)); // stopping
            if (adjStation != NULL) { // arriving from another station
                Services::insertUpdate(tripHash, seq - 1, ServiceState(lineID, adjTimestamp + MSG_STATION_PAD, adjStationID, timestamp, stationID)); // in transit state from previous station to this one
            }
        }

        numEvents++;
    }

    size_t numUpdates = Services::commitUpdates();
    ESP_LOGI(kTag, "created %u state updates from %u received events", numUpdates, numEvents);

    cJSON_Delete(json);
}
