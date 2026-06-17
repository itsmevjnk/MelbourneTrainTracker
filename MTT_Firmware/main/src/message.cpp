#include "message.h"
#include "services.h"

#include "esp_log.h"
#include "esp_check.h"
const char* Message::kTag = "msg";

/* padding duration (in seconds) after service departure at stations */
#ifndef CONFIG_MSG_STATION_PAD
#define CONFIG_MSG_STATION_PAD                         10
#endif

MessageEntry Message::m_entryFragment; // for holding fragmented entry
int Message::m_entryFragmentSize = 0; // size of the first half of the fragment

size_t Message::m_expectedEntries = 0; // number of expected (from header) and received entries
size_t Message::m_receivedEntries = 0;
bool Message::m_started = false;

#define ENTRY_BASE_SIZE                     (sizeof(MessageEntry) - (4 + 8))
#define ENTRY_ADJ_SIZE                      sizeof(MessageEntry)

// TODO: omit this completely for Rev2 once all V/Line LEDs have been mapped
bool Message::checkVLine(infraid_t line, infraid_t station) {
    line &= ~(1UL << 31); // strip RRB flag

    /* ignore V/Line lines not represented on map */
#if LMAT_VLINE_HAS_BENDIGO
    bool isBendigo = false;
#else
    bool isBendigo = line == INFRAID_BGO || line == INFRAID_ECH || line == INFRAID_SWL;
#endif
#if LMAT_VLINE_HAS_SEYMOUR
    bool isSeymour = false;
#else
    bool isSeymour = line == INFRAID_SER || line == INFRAID_SNH || line == INFRAID_ABY;
#endif
    if (isBendigo || isSeymour) return false;

    /* check V/Line lines if stop is within bounds */
#ifndef LMAT_VLINE_HAS_REGIONAL
    if (line == INFRAID_ART || line == INFRAID_BAT || line== INFRAID_MBY) { // V/Line Melton
        if (
            station != INFRAID_MEL && station != INFRAID_TLN && station != INFRAID_RBK && station != INFRAID_RVH
            && station != INFRAID_DEK && station != INFRAID_ARR && station != INFRAID_SUN && station != INFRAID_FSY
            && station != INFRAID_SSS
        ) return false;
    } else if (line == INFRAID_BDE || line == INFRAID_TRN) { // V/Line Gippsland
        if (
            station != INFRAID_PKM && station != INFRAID_BEW && station != INFRAID_DNG && station != INFRAID_CLA
            && station != INFRAID_CFD && station != INFRAID_RMD && station != INFRAID_FSS && station != INFRAID_SSS
        ) return false;
    } else if (line == INFRAID_GEL || line == INFRAID_WBL) { // V/Line Wyndham Vale
        if (
            station != INFRAID_WVL && station != INFRAID_DAV && station != INFRAID_TNT && station != INFRAID_DEK
            && station != INFRAID_ARR && station != INFRAID_SUN && station != INFRAID_FSY && station != INFRAID_SSS
        ) return false;
    }
#if LMAT_VLINE_HAS_BENDIGO
    else if (line == INFRAID_BGO || line == INFRAID_ECH || line == INFRAID_SWL) { // V/Line Bendigo via Sunbury
        if (
            station != INFRAID_SUY && station != INFRAID_WGS && station != INFRAID_FSY && station != INFRAID_SSS
        ) return false;
    }
#endif
#if LMAT_VLINE_HAS_SEYMOUR
    else if (line == INFRAID_SER || line == INFRAID_SNH || line == INFRAID_ABY) { // V/Line Seymour via Craigieburn
        if (
            station != INFRAID_CGB && station != INFRAID_BMS && station != INFRAID_ESD && station != INFRAID_SSS
        ) return false;
    }
#endif
#endif

    return true;
}

void Message::parseFragment(const char* data, int length, bool first) {
    int offset = 0;
    if (first) {
        if (m_started) {
            ESP_LOGW(kTag, "message parsing restarted - was the previous message not received in full?");
        } else {
            Services::acquire();
        }

        m_expectedEntries = *((const uint32_t*)data); offset = 4; // read number of entries in message
        Services::clearAndReserve(m_expectedEntries);
        m_started = true; m_receivedEntries = 0; m_entryFragmentSize = 0;

        ESP_LOGD(kTag, "available memory at the beginning of message parsing: %lu bytes", esp_get_minimum_free_heap_size());
    }

    while (m_entryFragmentSize + length - offset >= (int)ENTRY_BASE_SIZE) { // minimum size for an entry
        ESP_LOGV(kTag, "entry %u, offset %d/%d", m_receivedEntries, offset, length);
        const MessageEntry* entry = (const MessageEntry*) ((uintptr_t)data + offset);

        if (m_entryFragmentSize) { // there's already a fragment
            assert(m_entryFragmentSize < ENTRY_ADJ_SIZE);

            size_t bytesToRead = 0;
            if (m_entryFragmentSize >= ENTRY_BASE_SIZE) { // flags already read
                assert(m_entryFragment.flags.hasAdjacent);
                bytesToRead = ENTRY_ADJ_SIZE - m_entryFragmentSize;
            } else { // flags not read yet
                if (m_entryFragmentSize + length - offset < ENTRY_BASE_SIZE) break; // cannot reach flags from here
                const MessageEntry::Flags* flags = (const MessageEntry::Flags*) ((uintptr_t)entry + (ENTRY_BASE_SIZE - 1 - m_entryFragmentSize));
                bytesToRead = ((flags->hasAdjacent) ? ENTRY_ADJ_SIZE : ENTRY_BASE_SIZE) - m_entryFragmentSize;
            }
            assert(bytesToRead > 0);

            if (length - offset < bytesToRead) break; // we cannot process just yet
            ESP_LOGV(kTag, "fragment end: reading %u bytes (total fragment size: %u bytes)", bytesToRead, m_entryFragmentSize + bytesToRead);
            memcpy((void*)((uintptr_t)&m_entryFragment + m_entryFragmentSize), entry, bytesToRead);
            entry = &m_entryFragment;
            offset += bytesToRead;
        } else {
            if (entry->flags.hasAdjacent && length - offset < (int)ENTRY_ADJ_SIZE) break; // incomplete
        }

        infraid_t line = entry->line;
#ifndef CONFIG_MUNNEL_COLOUR_NONE
        if (entry->flags.viaMunnel) {
            switch (line) {
#ifdef CONFIG_MUNNEL_COLOUR_BLUE
                case INFRAID_SUY: line = INFRAID_SUYM; break;
                case INFRAID_CBE: line = INFRAID_CBEM; break;
                case INFRAID_PKM: line = INFRAID_PKMM; break;
#endif
#ifdef CONFIG_MUNNEL_COLOUR_DEST
                case INFRAID_SUY: line = (entry->flags.mtOriginating) ? INFRAID_SUYM : INFRAID_SUYm; break;
                case INFRAID_CBE: line = (entry->flags.mtOriginating) ? INFRAID_CBEM : INFRAID_CBEm; break;
                case INFRAID_PKM: line = (entry->flags.mtOriginating) ? INFRAID_PKMM : INFRAID_PKMm; break;
#endif
                default: ESP_LOGE(kTag, "viaMunnel flag set for non-MT line " INFRAID2STR_FMT, INFRAID2STR(line)); break;
            }
        }
#ifdef CONFIG_MUNNEL_COLOUR_BLUE
        else if (line == INFRAID_SUY && !entry->flags.viaCity) line = INFRAID_SUYM; // Sunbury short workings not via city stations
#endif
#endif
        if (entry->flags.isRRB) line |= (1UL << 31);

        ESP_LOGV(
            kTag, "trip hash 0x%08lx at %lld: " INFRAID2STR_FMT " %s event at " INFRAID2STR_FMT,
            entry->tripHash, entry->timestamp, INFRAID2STR(line), (entry->flags.isDeparture) ? "departure" : "arrival", INFRAID2STR(entry->station)
        );
        if (entry->flags.hasAdjacent)
            ESP_LOGV(
                kTag, "next %s at " INFRAID2STR_FMT " on %lld",
                (entry->flags.isDeparture) ? "arrival" : "departure", INFRAID2STR(entry->adjStation), entry->adjTimestamp
            );
        
        if (checkVLine(line, entry->station)) {
            assert(LSID::isValidLine(line));

            // if (entry->flags.isDeparture) { // departing station
            //     if (entry->flags.hasAdjacent && checkVLine(line, entry->adjStation)) { // departing to another (valid) station
            //         time_t departTime = entry->timestamp + CONFIG_MSG_STATION_PAD;
            //         Services::insertUpdate(entry->tripHash, ServiceState(line, departTime, entry->station, entry->adjTimestamp, entry->adjStation)); // in transit
            //     }
            // }
            // else { // arriving at station
            //     Services::insertUpdate(entry->tripHash, ServiceState(line, entry->timestamp, entry->station)); // stopping
            //     if (entry->flags.hasAdjacent && checkVLine(line, entry->adjStation)) { // arriving from another (valid) station
            //         time_t departTime = entry->adjTimestamp + CONFIG_MSG_STATION_PAD;
            //         Services::insertUpdate(entry->tripHash, ServiceState(line, departTime, entry->adjStation, entry->timestamp, entry->station)); // in transit state from previous station to this one
            //     }
            // }

            if (!entry->flags.isDeparture) { // arriving at station, departure time is in adjacent timestamp
                time_t departureTime = ((entry->flags.hasAdjacent) ? entry->adjTimestamp : entry->timestamp) + CONFIG_MSG_STATION_PAD;
                Services::insertState(line, entry->tripHash, entry->station, entry->timestamp, departureTime);
            }
        }

        m_receivedEntries++;

        if (m_entryFragmentSize) m_entryFragmentSize = 0; // entire fragment read (offset increment is done above)
        else offset += (entry->flags.hasAdjacent) ? ENTRY_ADJ_SIZE : ENTRY_BASE_SIZE; // increment offset
    }

    /* copy remains into fragment buffer */
    if (offset < length) {
        ESP_LOGV(kTag, "fragment start/continue: reading %d bytes", length - offset);
        // abort();
        int bytesToRead = length - offset;
        assert(m_entryFragmentSize + bytesToRead <= (int)sizeof(MessageEntry)); // to prevent corruption
        memcpy((void*)((uintptr_t)&m_entryFragment + m_entryFragmentSize), &data[offset], bytesToRead);    
        m_entryFragmentSize += bytesToRead;
    }
}
void Message::finish() {
    if (!m_started) {
        ESP_LOGE(kTag, "finish() called without an active message");
        return;
    }

    if (m_entryFragmentSize > 0) {
        ESP_LOGW(kTag, "there is still data fragment at the end of the message - was it not received in full?");
    }

    ESP_LOGD(kTag, "received %u/%u entries", m_receivedEntries, m_expectedEntries);
    assert(m_receivedEntries == m_expectedEntries);

    ESP_LOGD(kTag, "available memory at the end of message parsing: %lu bytes", esp_get_minimum_free_heap_size());

    Services::release();
    m_started = false;
}
