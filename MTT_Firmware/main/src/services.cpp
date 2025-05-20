#include "services.h"

#include "subsystems/led_matrix.h"
#include "esp_check.h"

const char* ServiceState::kTag = "service_state";

void ServiceState::copy(const ServiceState& other) {
    m_line = other.m_line;
    m_station = other.m_station;
    m_time = other.m_time;
    m_nextStation = other.m_nextStation;
    m_nextTime = other.m_nextTime;
}

static uint16_t midLEDs[75]; // LEDs in the middle - count might be overkill. this is so that we don't have to use the stack and risk overflowing it

void ServiceState::show(time_t now) const {
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
            int ledIndex = (now - m_time) * numMidLEDs / (m_nextTime - m_time);
            if (ledIndex >= numMidLEDs) ledIndex = numMidLEDs - 1;
            // ESP_LOGI(kTag, "ledIndex = %d", ledIndex);
            // assert(ledIndex >= 0 && ledIndex < numMidLEDs);
            ESP_ERROR_CHECK(LEDMatrix::set(midLEDs[ledIndex], lineColour));
        } else ESP_ERROR_CHECK(LEDMatrix::set(stationLED, lineColour)); // fallback, and also used in the FSS -> PAR on Northern group scenario
    }
}

void ServiceState::printInfo() const {
    if (isInTransit()) {
        esp_rom_printf(" - at %lld: " INFRAID2STR_FMT " line in transit from " INFRAID2STR_FMT " to " INFRAID2STR_FMT " (arriving at %lld)\r\n", m_time, INFRAID2STR(m_line), INFRAID2STR(m_station), INFRAID2STR(m_nextStation), m_nextTime);
    } else {
        esp_rom_printf(" - at %lld: " INFRAID2STR_FMT " line stopping at " INFRAID2STR_FMT "\r\n", m_time, INFRAID2STR(m_line), INFRAID2STR(m_station));
    }
}

StaticSemaphore_t Services::m_statesMutexBuf; SemaphoreHandle_t Services::m_statesMutex = xSemaphoreCreateRecursiveMutexStatic(&m_statesMutexBuf);
StaticSemaphore_t Services::m_updatesMutexBuf; SemaphoreHandle_t Services::m_updatesMutex = xSemaphoreCreateRecursiveMutexStatic(&m_updatesMutexBuf);

Services::StatesDict Services::m_states;
Services::StateUpdateQueue Services::m_updates;
Services::StateUpdateVector Services::m_updatesBacking;

bool Services::updateStates(time_t now) {
    acquire(); acquireUpdates();

    bool noStates = m_states.empty(); // set if there are no states (i.e. just been cleared)
    bool noUpdates = m_updates.empty(); // set if there are no updates (i.e. no services)
    
    bool updated = false; // set when any state update was popped
    while (!m_updates.empty()) {
        const TaggedServiceStateIndex& update = m_updates.top();
        uint32_t updateTrip = update.first; ServiceStateIndex updateState = update.second;

        time_t updateTime = m_allStates[updateState].getTimestamp();
        if (updateTime > now) break; // future update - stop

        m_updates.pop(); // consume update
        auto state = m_states.find(updateTrip);
        if (state == m_states.end() || m_allStates[state->second].getTimestamp() < updateTime)
            m_states[updateTrip] = updateState;

        updated = true;
    }

    ESP_LOGD(kTag, "available memory after updateStates(): %lu bytes", esp_get_minimum_free_heap_size());

    release(); releaseUpdates();

    return (noStates && !noUpdates && !updated) ? false : true;
}

const char* Services::kTag = "services";

void Services::showAllStates(time_t now, uint32_t lines) {
    acquire();

    for (auto& [tripHash, state] : m_states) {
        const ServiceState& stateObj = m_allStates[state];

        uint32_t mask = getLineBitmask(stateObj.getLine());
        assert(mask);

        if (lines & mask) stateObj.show(now);
    }

    release();    
}

ServiceState* Services::m_allStates = nullptr;
ServiceStateIndex Services::m_allStatesCount = 0;
ServiceStateIndex Services::m_allStatesCapacity = 0;

void Services::clearAndReserve(ServiceStateIndex count) {
    acquire(); acquireUpdates();
    ESP_LOGD(kTag, "available memory before clearing: %lu bytes", esp_get_minimum_free_heap_size());

    m_states.clear();
    m_updates.clear();

    m_allStatesCount = 0;
    if (m_allStates) free(m_allStates);
    if (count > m_allStatesCapacity) {
        ESP_LOGD(kTag, "growing m_allStatesCapacity to %u", count);
        m_allStatesCapacity = count; // increase reserved capacity
    }
    size_t allocSize = m_allStatesCapacity * sizeof(ServiceState);
    ESP_LOGD(kTag, "allocating %u bytes for m_allStates", allocSize);
    m_allStates = (ServiceState*)malloc(allocSize); // here we can get away with not reallocating since there are no more states
    assert(!m_allStatesCapacity || m_allStates); // ensure that calloc doesn't die (but if there are no entries to begin with then that's okay)

    ESP_LOGD(kTag, "available memory after clearing: %lu bytes", esp_get_minimum_free_heap_size());
    release(); releaseUpdates();
}

#ifndef SERVICES_CAPACITY_GROWTH
#define SERVICES_CAPACITY_GROWTH                    10
#endif

ServiceStateIndex Services::insertUpdate(uint32_t tripHash, ServiceState&& state) {
    acquire(); acquireUpdates();
    ServiceStateIndex index = m_allStatesCount;
    if (index == m_allStatesCapacity) {
        m_allStatesCapacity += SERVICES_CAPACITY_GROWTH;
        ESP_LOGW(kTag, "growing m_allStatesCapacity outside of reserved capacity to %u bytes - wrong estimate?",  m_allStatesCapacity * sizeof(ServiceState));
        m_allStates = (ServiceState*)realloc(m_allStates, m_allStatesCapacity * sizeof(ServiceState)); // we have to realloc to keep all states
        assert(m_allStates);
    }
    m_allStates[m_allStatesCount++] = state;
    m_updates.push(std::make_pair(tripHash, index));
    release(); releaseUpdates();
    return index;
}

void Services::printInfo() {
    acquire();
    printInfoWithoutMutex();
    release(); 
}

void Services::printInfoWithoutMutex() {
    esp_rom_printf("current service states:\r\n");
    for (auto& [tripHash, state] : m_states) {
        m_allStates[state].printInfo();
    }
    // TODO: dump available updates
}

extern "C" void printServicesWithoutMutex() {
    Services::printInfoWithoutMutex();
}

const infraid_t Services::kLineIDs[] = {
    /* metropolitan lines */
    INFRAID_SHM,
    INFRAID_MDD,
    INFRAID_HBE,
    INFRAID_CCL,
    INFRAID_PKM,
    INFRAID_CBE,
    INFRAID_BEG,
    INFRAID_LIL,
    INFRAID_GWY,
    INFRAID_ALM,
    INFRAID_STY,
    INFRAID_FKN,
    INFRAID_WIL,
    INFRAID_WER,
    INFRAID_CGB,
    INFRAID_SUY,
    INFRAID_RCE,
    INFRAID_UFD,

    /* country lines */
    INFRAID_ART, // Ararat
    INFRAID_BAT, // Ballarat
    INFRAID_MBY, // Maryborough

    INFRAID_BDE, // Bairnsdale
    INFRAID_TRN, // Traralgon
    INFRAID_vPK, // Pakenham

    INFRAID_GEL, // Geelong
    INFRAID_WBL  // Warrnambool
}; // line IDs to match with m_lines flag
const size_t Services::kNumLines = (sizeof(kLineIDs) / sizeof(infraid_t));
const uint32_t Services::kAllLines = (1 << kNumLines) - 1;
uint32_t Services::m_lines = kAllLines; // all lines on by default

uint32_t Services::getLineBitmask(infraid_t line) {
    for (size_t i = 0; i < kNumLines; i++) {
        if (line == kLineIDs[i]) return (1 << i);
    }

    ESP_LOGE(kTag, "invalid line ID " INFRAID2STR_FMT, INFRAID2STR(line));
    return 0;
}

esp_err_t Services::enableLine(infraid_t line) {
    uint32_t mask = getLineBitmask(line);
    if (!mask) return ESP_ERR_INVALID_ARG;
    m_lines |= mask;
    return ESP_OK;
}

esp_err_t Services::disableLine(infraid_t line) {
    uint32_t mask = getLineBitmask(line);
    if (!mask) return ESP_ERR_INVALID_ARG;
    m_lines &= ~mask;
    return ESP_OK;
}