/* current state of a service */

#pragma once

#include "lsid.h"
#include "colours.h"
#include <sys/time.h>

#include <bits/stdc++.h>
#include <freertos/FreeRTOS.h>

class ServiceState {
public:
    ServiceState() {}
    ServiceState(infraid_t line, time_t timestamp, infraid_t station)
        : m_line(line), m_station(station), m_time(timestamp), m_nextStation(0), m_nextTime(0) {} // stopping at station
    ServiceState(infraid_t line, time_t timestamp, infraid_t station, time_t nextTimestamp, infraid_t nextStation)
        : m_line(line), m_station(station), m_time(timestamp), m_nextStation(nextStation), m_nextTime(nextTimestamp) {} // in transit to another station        

    void copy(const ServiceState& other); // copy attributes from another ServiceState object

    void show(time_t now) const; // display state on LED matrix
    inline void show() const {
        time_t now; time(&now);
        show(now);
    }

    inline time_t getTimestamp() const { return m_time; }
    inline bool isInTransit() const { return m_nextStation != 0; }
    inline infraid_t getLine() const { return m_line; }

    void printInfo() const; // dump information

protected:
    infraid_t m_line; // train line
    infraid_t m_station; // current or departing station
    time_t m_time; // timestamp of this state
    infraid_t m_nextStation = 0; // next arriving station (0 if stopping)
    time_t m_nextTime = 0; // timestamp of reaching m_nextStation

private:
    static const char* kTag;
};

typedef size_t ServiceStateIndex; // index into m_allStates
typedef std::pair<uint32_t, ServiceStateIndex> TaggedServiceStateIndex;

class Services {
public:
    /* acquire/release access to m_states - to be used by renderer */
    static inline void acquire() {
        while (!xSemaphoreTakeRecursive(m_statesMutex, portMAX_DELAY));
    }
    static inline void release() { xSemaphoreGiveRecursive(m_statesMutex); }
    
    using StatesDict = std::map<uint32_t, ServiceStateIndex>;
    static StatesDict m_states; // current states of services

    static inline void acquireUpdates() { xSemaphoreTakeRecursive(m_updatesMutex, portMAX_DELAY); }
    static inline void releaseUpdates() { xSemaphoreGiveRecursive(m_updatesMutex); }

    static ServiceStateIndex insertUpdate(uint32_t tripHash, ServiceState&& state);

    static void clearAndReserve(size_t num);

    static bool updateStates(time_t now); // update service states for the given timestamp
    // returns false if there are no current states and there are updates but none were applied (as they are in the future)
    // when this is false, the caller should skip displaying as doing so will result in brief periods of no service
    static inline bool updateStates() {
        time_t now; time(&now);
        return updateStates(now);
    }

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

    static uint32_t getLineBitmask(infraid_t line);

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

    static ServiceState* m_allStates; // backing container for all states
    static ServiceStateIndex m_allStatesCount;
    static ServiceStateIndex m_allStatesCapacity;

    struct TimePriorityComparer {
        bool operator()(const TaggedServiceStateIndex& a, const TaggedServiceStateIndex& b) const {
            return m_allStates[a.second].getTimestamp() > m_allStates[b.second].getTimestamp(); // ascending timestamp
        }
    };

    using StateUpdateVector = std::vector<TaggedServiceStateIndex>;
    // using StateUpdateQueue = std::priority_queue<TaggedServiceStateIndex, StateUpdateVector, TimePriorityComparer>;
    class StateUpdateQueue : public std::priority_queue<TaggedServiceStateIndex, StateUpdateVector, TimePriorityComparer> {
    public:
        void clear() {
            size_t capacity = this->c.capacity();
            this->c.clear();
            this->c.reserve(capacity);
        }
    };
    static StateUpdateQueue m_updates;
    static StateUpdateVector m_updatesBacking; // backing container for m_updates; this should not shrink in size (and will thus get the aggressive reserve() treatment)
    static StaticSemaphore_t m_updatesMutexBuf; static SemaphoreHandle_t m_updatesMutex;

    static const uint32_t kAllLines;
    static uint32_t m_lines;
};