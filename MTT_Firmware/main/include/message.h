/* message parsing */

#pragma once

#include <bits/stdc++.h>

#include <sys/time.h>
#include "colours.h"
#include "lsid.h"

#include <freertos/FreeRTOS.h>

#include "subsystems/led_matrix.h"

struct MessageEntry {
    uint32_t tripHash; // 0
    infraid_t line : 24; // 4
    infraid_t station : 24; // 7
    int64_t timestamp; // 10
    struct Flags {
        uint8_t isDeparture : 1;
        uint8_t hasAdjacent : 1;
        uint8_t reserved : 6;
    } __attribute__((packed)) flags; // 18
    // 19 bytes

    /* only valid if hasAdjacent is set */
    infraid_t adjStation : 24; // 19
    int64_t adjTimestamp; // 22
    // 30 bytes
} __attribute__((packed));

class Message {
public:
    static void parseFragment(const char* data, int length, bool first); // start/continue parsing message fragment
    static void finish(); // finish parsing entire message

private:
    static const char* kTag; // for logging

    static MessageEntry m_entryFragment; // for holding fragmented entry
    static int m_entryFragmentSize; // size of the first half of the fragment

    static size_t m_expectedEntries; // number of expected (from header) and received entries
    static size_t m_receivedEntries;
    static bool m_started;
};