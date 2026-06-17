#include "services.h"

#include "lsid.h"
#include "subsystems/led_matrix.h"
#include "esp_check.h"

StaticSemaphore_t Services::m_statesMutexBuf;
SemaphoreHandle_t Services::m_statesMutex = xSemaphoreCreateRecursiveMutexStatic(&m_statesMutexBuf);

std::vector<ServiceState> Services::m_states;
std::vector<ServiceStateOffsetEntry> Services::m_trips;

const char* Services::kTag = "services";

void Services::showAllStates(time_t now, uint32_t lines) {
    acquire();

    for (auto& trip : m_trips) {
        /* check if line is selected for display */
        uint64_t mask = getLineBitmask(trip.line);
        assert(mask);
        if (!(lines & mask)) continue;

        colour_t lineColour = LSID::getLineColour(trip.line);
        ESP_LOGD(kTag, "line " INFRAID2STR_FMT " trip 0x%08x:", INFRAID2STR(trip.line), trip.tripHash);
        for (size_t idx = trip.firstIdx; idx != SIZE_MAX; idx = m_states[idx].nextIdx) {
            ESP_LOGD(kTag, " - idx %lu: at " INFRAID2STR_FMT ": arrival %lld, departure %lld", idx, INFRAID2STR(m_states[idx].station), m_states[idx].arrivalTime, m_states[idx].departureTime);
            if (now < m_states[idx].arrivalTime) { // before arriving at this station
                ServiceStateIndex prevIdx = m_states[idx].prevIdx;
                if (prevIdx == SIZE_MAX) { // first entry
                    goto atStation;
                } else { // between previous entry and this one
                    static uint16_t midLEDs[75]; // LEDs in the middle - count might be overkill. this is so that we don't have to use the stack and risk overflowing it
                    size_t numMidLEDs = LSID::getLEDsBetween(
                        trip.line, m_states[prevIdx].station, m_states[idx].station,
                        midLEDs, sizeof(midLEDs) / sizeof(uint16_t)
                    );

                    if (numMidLEDs > 0) {
                        int ledIndex = 0;
                        time_t prevDepartureTime = m_states[prevIdx].departureTime;
                        time_t arrivalTime = m_states[idx].arrivalTime;
                        if (prevDepartureTime != arrivalTime) ledIndex = (now - prevDepartureTime) * numMidLEDs / (arrivalTime - prevDepartureTime);
                        if (ledIndex >= numMidLEDs) ledIndex = numMidLEDs - 1;
                        // ESP_LOGI(kTag, "ledIndex = %d", ledIndex);
                        // assert(ledIndex >= 0 && ledIndex < numMidLEDs);
                        ESP_ERROR_CHECK(LEDMatrix::set(midLEDs[ledIndex], lineColour));
                        break;
                    } else goto atStation; // fallback, and also used in the FSS -> PAR on Northern group scenario
                }
            } else if (now <= m_states[idx].departureTime) { // at this station already
atStation:
                uint16_t stationLED = LSID::getLED(trip.line, m_states[idx].station);
                if (stationLED == LMAT_NULL) {
                    ESP_LOGW(kTag, "invalid station code " INFRAID2STR_FMT " on line " INFRAID2STR_FMT, INFRAID2STR(m_states[idx].station), INFRAID2STR(trip.line));
                    break;
                }

                ESP_ERROR_CHECK(LEDMatrix::set(stationLED, lineColour));
                break;
            }
        }
    }

    release();    
}

void Services::clearAndReserve(ServiceStateIndex count) {
    acquire();
    ESP_LOGD(kTag, "available memory before clearAndReserve(): %lu bytes", esp_get_minimum_free_heap_size());

    m_states.clear(); m_states.reserve(count);
    m_trips.clear();

    ESP_LOGD(kTag, "available memory after clearAndReserve(): %lu bytes", esp_get_minimum_free_heap_size());
    release();
}

ServiceStateIndex Services::insertState(infraid_t line, uint32_t tripHash, infraid_t station, time_t arrivalTime, time_t departureTime) {
    if (arrivalTime > departureTime && departureTime > 0) departureTime = arrivalTime; // enforce arrivalTime <= departureTime

    acquire();

    /* search for trip in offset lookup table */
    size_t offsetIdx = 0;
    for (offsetIdx = 0; offsetIdx < m_trips.size(); offsetIdx++) {
        if (m_trips[offsetIdx].line == line && m_trips[offsetIdx].tripHash == tripHash) break;
    }
    if (offsetIdx >= m_trips.size()) {
        m_trips.emplace_back(line, tripHash, SIZE_MAX); // placeholder
        offsetIdx = m_trips.size() - 1; // index to placeholder
    }

    /* insert into service states */
    m_states.emplace_back(station, arrivalTime, departureTime, SIZE_MAX, SIZE_MAX);
    size_t stateIdx = m_states.size() - 1;
    
    /* search for position in trip to drop service state in */
    size_t prevStateIdx = m_trips[offsetIdx].firstIdx;
    if (prevStateIdx == SIZE_MAX) { // first and only entry
        m_trips[offsetIdx].firstIdx = stateIdx;
    } else if (m_states[prevStateIdx].arrivalTime >= departureTime) { // append before first entry
        m_trips[offsetIdx].firstIdx = stateIdx;
        m_states[stateIdx].nextIdx = prevStateIdx;
        m_states[prevStateIdx].prevIdx = stateIdx;
    } else {
        while (m_states[prevStateIdx].nextIdx != SIZE_MAX && m_states[m_states[prevStateIdx].nextIdx].arrivalTime < departureTime) {
            prevStateIdx = m_states[prevStateIdx].nextIdx;
        }
        // after this prevStateIdx points to the state before this one

        size_t nextStateIdx = m_states[prevStateIdx].nextIdx;

        m_states[stateIdx].prevIdx = prevStateIdx;
        m_states[stateIdx].nextIdx = nextStateIdx;

        m_states[prevStateIdx].nextIdx = stateIdx;
        if (nextStateIdx != SIZE_MAX) m_states[nextStateIdx].prevIdx = stateIdx;
    }

    release();
    return stateIdx;
}

void Services::printInfo() {
    acquire();
    printInfoWithoutMutex();
    release(); 
}

void Services::printInfoWithoutMutex() {
    esp_rom_printf("current service states:\r\n");
    for (auto& trip : m_trips) {
        esp_rom_printf(" - line " INFRAID2STR_FMT " trip 0x%08x:\r\n", INFRAID2STR(trip.line), trip.tripHash);
        for (size_t idx = trip.firstIdx; idx != SIZE_MAX; idx = m_states[idx].nextIdx) {
            esp_rom_printf("   - idx %lu: stop " INFRAID2STR_FMT " arrival %lld, departure %lld\r\n", idx, INFRAID2STR(m_states[idx].station), m_states[idx].arrivalTime, m_states[idx].departureTime);
        }
    }
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

    INFRAID_GEL, // Geelong
    INFRAID_WBL,  // Warrnambool

    INFRAID_BGO, // Bendigo
    INFRAID_SWL, // Swan Hill
    INFRAID_ECH, // Echuca

    INFRAID_SER, // Seymour
    INFRAID_SNH, // Shepparton
    INFRAID_ABY, // Albury

    /* lines via Metro Tunnel */
    INFRAID_SUYM,
    INFRAID_CBEM,
    INFRAID_PKMM,
    INFRAID_SUYm,
    INFRAID_CBEm,
    INFRAID_PKMm,
    // TODO: properly do this for web interface
}; // line IDs to match with m_lines flag
const size_t Services::kNumLines = (sizeof(kLineIDs) / sizeof(infraid_t));
const uint64_t Services::kAllLines = (1ULL << kNumLines) - 1;
uint64_t Services::m_lines = kAllLines; // all lines on by default

uint64_t Services::getLineBitmask(infraid_t line) {
    uint64_t mask = 0;
    line &= ~(1UL << 31); // strip RRB flag
    if (line == INFRAID_SUY || line == INFRAID_SUYM || line == INFRAID_SUYm) {
        for (size_t i = 0; i < kNumLines; i++) {
            if (kLineIDs[i] == INFRAID_SUY || kLineIDs[i] == INFRAID_SUYM || kLineIDs[i] == INFRAID_SUYm) mask |= (1ULL << i);
        }
    } else if (line == INFRAID_CBE || line == INFRAID_CBEM || line == INFRAID_CBEm) {
        for (size_t i = 0; i < kNumLines; i++) {
            if (kLineIDs[i] == INFRAID_CBE || kLineIDs[i] == INFRAID_CBEM || kLineIDs[i] == INFRAID_CBEm) mask |= (1ULL << i);
        }
    } else if (line == INFRAID_PKM || line == INFRAID_PKMM || line == INFRAID_PKMm) {
        for (size_t i = 0; i < kNumLines; i++) {
            if (kLineIDs[i] == INFRAID_PKM || kLineIDs[i] == INFRAID_PKMM || kLineIDs[i] == INFRAID_PKMm) mask |= (1ULL << i);
        }
    } else {
        for (size_t i = 0; i < kNumLines; i++) {
            if (line == kLineIDs[i]) {
                mask = (1ULL << i);
                break;
            }
        }
    }

    if (!mask) ESP_LOGE(kTag, "invalid line ID " INFRAID2STR_FMT, INFRAID2STR(line));
    return mask;
}

esp_err_t Services::enableLine(infraid_t line) {
    uint64_t mask = getLineBitmask(line);
    if (!mask) return ESP_ERR_INVALID_ARG;
    m_lines |= mask;
    return ESP_OK;
}

esp_err_t Services::disableLine(infraid_t line) {
    uint64_t mask = getLineBitmask(line);
    if (!mask) return ESP_ERR_INVALID_ARG;
    m_lines &= ~mask;
    return ESP_OK;
}