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

void Message::parseFragment(const char* data, int length, bool first) {
    int offset = 0;
    if (first) {
        if (m_started) {
            ESP_LOGW(kTag, "message parsing restarted - was the previous message not received in full?");
        } else {
            Services::acquire(); Services::acquireUpdates();
        }

        m_expectedEntries = *((const uint32_t*)data); offset = 4; // read number of entries in message
        Services::clearAndReserve(m_expectedEntries * 2); // worst-case
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

        ESP_LOGV(
            kTag, "trip hash 0x%08lx at %lld: " INFRAID2STR_FMT " %s event at " INFRAID2STR_FMT,
            entry->tripHash, entry->timestamp, INFRAID2STR(entry->line), (entry->flags.isDeparture) ? "departure" : "arrival", INFRAID2STR(entry->station)
        );
        if (entry->flags.hasAdjacent)
            ESP_LOGV(
                kTag, "next %s at " INFRAID2STR_FMT " on %lld",
                (entry->flags.isDeparture) ? "arrival" : "departure", INFRAID2STR(entry->adjStation), entry->adjTimestamp
            );
        
        assert(LSID::isValidLine(entry->line));

        if (entry->flags.isDeparture) { // departing station
            time_t departTime = entry->timestamp + CONFIG_MSG_STATION_PAD;
            if (entry->flags.hasAdjacent) { // departing to another station
                Services::insertUpdate(entry->tripHash, ServiceState(entry->line, departTime, entry->station, entry->adjTimestamp, entry->adjStation)); // in transit
            }
        } else { // arriving at station
            Services::insertUpdate(entry->tripHash, ServiceState(entry->line, entry->timestamp, entry->station)); // stopping
            if (entry->flags.hasAdjacent) { // arriving from another station
                time_t departTime = entry->adjTimestamp + CONFIG_MSG_STATION_PAD;
                Services::insertUpdate(entry->tripHash, ServiceState(entry->line, departTime, entry->adjStation, entry->timestamp, entry->station)); // in transit state from previous station to this one
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

    Services::release(); Services::releaseUpdates();
    m_started = false;
}
