/* current state of a service */

#pragma once

#include "lsid.h"
#include "colours.h"
#include <sys/time.h>

#include <bits/stdc++.h>
#include <freertos/FreeRTOS.h>

typedef size_t ServiceStateIndex; // index into m_allStates

struct ServiceState {
    infraid_t station;

    // zero if not valid
    time_t arrivalTime;
    time_t departureTime;

    // index to previous/next entry in the same trip - set to SIZE_MAX if not valid
    ServiceStateIndex prevIdx;
    ServiceStateIndex nextIdx;
};

struct ServiceStateOffsetEntry {
    /* key */
    infraid_t line;
    uint32_t tripHash;

    ServiceStateIndex firstIdx; // value
};

class Services {
public:
    /* acquire/release access to m_states - to be used by renderer */
    static inline void acquire() { xSemaphoreTakeRecursive(m_statesMutex, portMAX_DELAY); }
    static inline void release() { xSemaphoreGiveRecursive(m_statesMutex); }
    
    static ServiceStateIndex insertState(infraid_t line, uint32_t tripHash, infraid_t station, time_t arrivalTime, time_t departureTime);
    static void clearAndReserve(ServiceStateIndex count);

    static void showAllStates(time_t now, uint32_t lines);
    static inline void showAllStates(time_t now) {
        showAllStates(now, m_lines);
    }
    static inline void showAllStates() {
        time_t now; time(&now);
        showAllStates(now);
    }

    static void printInfo(); // dump all states and updates
    static void printInfoWithoutMutex(); // for use in crashes

    static uint64_t getLineBitmask(infraid_t line);

    static const infraid_t kLineIDs[]; // line IDs to match with m_lines flag
    static const size_t kNumLines;

    static inline uint32_t getEnabledLines() { return m_lines; }
    static esp_err_t enableLine(infraid_t line);
    static esp_err_t disableLine(infraid_t line);
    static inline void enableAllLines() { m_lines = kAllLines; }
    static inline void disableAllLines() { m_lines = 0; }
private:
    static const char* kTag;

    /* mutexes to protect m_states and m_updates */
    static StaticSemaphore_t m_statesMutexBuf; static SemaphoreHandle_t m_statesMutex;

    static std::vector<ServiceState> m_states; // service states
    static std::vector<ServiceStateOffsetEntry> m_trips; // lookup table into m_states

    static const uint64_t kAllLines;
    static uint64_t m_lines;
};