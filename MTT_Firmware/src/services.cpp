#include "services.h"

#include "subsystems/led_matrix.h"
#include "esp_check.h"

const char* ServiceState::kTag = "service_state";

// ServiceState& ServiceState::operator=(ServiceState&& other) noexcept {
//     if (this != &other) copy(other);
//     return *this;
// }

void ServiceState::copy(const ServiceState& other) {
    m_line = other.m_line;
    m_station = other.m_station;
    m_time = other.m_time;
    m_nextStation = other.m_nextStation;
    m_nextTime = other.m_nextTime;
}

static uint16_t midLEDs[75]; // LEDs in the middle - count might be overkill. this is so that we don't have to use the stack and risk overflowing it

void ServiceState::show(time_t now) {
    uint16_t stationLED = LSID::getLED(m_line, m_station);
    if (stationLED == LMAT_NULL) {
        ESP_LOGW(kTag, "invalid station code " INFRAID2STR_FMT " on line " INFRAID2STR_FMT, INFRAID2STR(m_station), INFRAID2STR(m_line));
        return;
    }

    colour_t lineColour = LSID::getLineColour(m_line);

    if (!m_nextStation) ESP_ERROR_CHECK(LEDMatrix::set(stationLED, lineColour));
    else {
        size_t numMidLEDs = LSID::getLEDsBetween(m_line, m_station, m_nextStation, midLEDs, sizeof(midLEDs) / sizeof(uint16_t));
        if (numMidLEDs > 0) {
            size_t ledIndex = (now - m_time) / ((m_nextTime - m_time) / numMidLEDs);
            ESP_ERROR_CHECK(LEDMatrix::set(midLEDs[ledIndex], lineColour));
        } else ESP_ERROR_CHECK(LEDMatrix::set(stationLED, lineColour)); // fallback, and also used in the FSS -> PAR on Northern group scenario
    }
}

StaticSemaphore_t Services::m_statesMutexBuf; SemaphoreHandle_t Services::m_statesMutex = xSemaphoreCreateRecursiveMutexStatic(&m_statesMutexBuf);
StaticSemaphore_t Services::m_updatesMutexBuf; SemaphoreHandle_t Services::m_updatesMutex = xSemaphoreCreateRecursiveMutexStatic(&m_updatesMutexBuf);

std::map<uint32_t, ServiceState> Services::m_states;
std::map<seq_hash_t, ServiceState, SeqHashComparer> Services::m_updatesMap;
std::priority_queue<tagged_state_t, std::vector<tagged_state_t>, TimeComparer> Services::m_updates;

size_t Services::commitUpdates() {
    acquireUpdates();

    std::vector<tagged_state_t> updates; updates.reserve(m_updatesMap.size()); // reserve to avoid heap fragmentation
    for (const auto& [key, value] : m_updatesMap) updates.push_back({key.second, value});
    m_updates = std::priority_queue<tagged_state_t, std::vector<tagged_state_t>, TimeComparer>(TimeComparer{}, std::move(updates));
    size_t count = m_updates.size();

    releaseUpdates();

    return count;
}

void Services::updateStates(time_t now) {
    acquire(); acquireUpdates();

    while (m_updates.size() > 0) {
        const tagged_state_t& update = m_updates.top();
        uint32_t updateTrip = update.first; const ServiceState& updateState = update.second;

        time_t updateTime = updateState.getTimestamp();
        if (updateTime > now) break; // future update - stop

        m_updates.pop(); // consume update

        if (updateState.isDeleteState()) m_states.erase(updateTrip);
        else {
            auto state = m_states.find(updateTrip);
            if (state == m_states.end()) m_states.insert({updateTrip, updateState}); // state does not exist yet
            else if (state->second.getTimestamp() < updateTime) state->second = updateState; // update existing state
        }
    }

    release(); releaseUpdates();
}

const char* Services::kTag = "services";

void Services::showAllStates(time_t now) {
    acquire();

    for (auto& [tripHash, state] : m_states) {
        state.show(now);
    }

    release();    
}
