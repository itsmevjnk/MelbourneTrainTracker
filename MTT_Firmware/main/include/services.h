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
    ServiceState(infraid_t line, time_t timestamp, infraid_t station, time_t delTimestamp)
        : m_line(line), m_station(station), m_time(timestamp), m_nextStation(0), m_nextTime(delTimestamp) {} // state deletion
    
    ServiceState(const ServiceState&) = default; 

    ServiceState& operator=(const ServiceState&) = default; 
    ServiceState& operator=(ServiceState&& other) = default;

    void copy(const ServiceState& other); // copy attributes from another ServiceState object

    void show(time_t now); // display state on LED matrix
    inline void show() {
        time_t now; time(&now);
        show(now);
    }

    inline time_t getTimestamp() const { return m_time; }
    inline bool isInTransit() const { return m_nextTime != 0 && m_nextStation != 0; }

    inline bool isDeleteState() const { return m_nextTime != 0 && m_nextStation == 0; }
    inline time_t getDeletionTimestamp() const { return m_nextTime; }

protected:
    infraid_t m_line; // train line
    infraid_t m_station; // current or departing station
    time_t m_time; // timestamp of this state
    infraid_t m_nextStation = 0; // next arriving station
    time_t m_nextTime = 0; // timestamp of reaching m_nextStation (if m_nextTime != 0, then m_nextStation will hold the next arriving station, or 0 if )

private:
    static const char* kTag;
};

typedef std::pair<int, uint32_t> seq_hash_t; // pair of sequence number and uint32_t hash of trip ID (generated from xxHash32)
struct SeqHashComparer {
    bool operator()(const seq_hash_t& a, const seq_hash_t& b) const {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first; // arbitrary in practice
    }
};

typedef std::pair<uint32_t, ServiceState> tagged_state_t; // service state object "tagged" with the service's trip ID hash
struct TimeComparer { // for use in Services::m_updates below
    bool operator()(const tagged_state_t& a, const tagged_state_t& b) const {
        return a.second.getTimestamp() > b.second.getTimestamp(); // ascending timestamp
    }
};

class Services {
public:
    /* acquire/release access to m_states - to be used by renderer */
    static inline void acquire() { xSemaphoreTakeRecursive(m_statesMutex, portMAX_DELAY); }
    static inline void release() { xSemaphoreGiveRecursive(m_statesMutex); }
    
    static std::map<uint32_t, ServiceState> m_states; // current states of services

    static inline void insertUpdate(uint32_t tripHash, int seq, const ServiceState& state) {
        m_updatesMap.insert_or_assign({(seq << 1) | (state.isInTransit() ? 1 : 0), tripHash}, state); // replace if already exists
        // seq also pulls double duty, as it is also used to differentiate stopping and in-transit states
    }

    static size_t commitUpdates(); // commit changes to the update queue to m_updates for updating

    static void updateStates(time_t now); // update service states for the given timestamp
    static inline void updateStates() {
        time_t now; time(&now);
        updateStates(now);
    }

    static void showAllStates(time_t now); 
    static inline void showAllStates() {
        time_t now; time(&now);
        showAllStates(now);
    }

private:
    static const char* kTag;

    /* mutexes to protect m_states and m_updates */
    static StaticSemaphore_t m_statesMutexBuf; static SemaphoreHandle_t m_statesMutex;

    static std::priority_queue<tagged_state_t, std::vector<tagged_state_t>, TimeComparer> m_updates;
    static StaticSemaphore_t m_updatesMutexBuf; static SemaphoreHandle_t m_updatesMutex;
    static inline void acquireUpdates() { xSemaphoreTakeRecursive(m_updatesMutex, portMAX_DELAY); }
    static inline void releaseUpdates() { xSemaphoreGiveRecursive(m_updatesMutex); }

    static std::map<seq_hash_t, ServiceState, SeqHashComparer> m_updatesMap; // updates to services' states, indexed by the trip ID hash and sequence number
};