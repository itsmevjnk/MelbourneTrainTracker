/* LED colours */

#pragma once

// taken from the PTV Visual Identity Toolkit 2.0: https://archive.org/details/ptv-vit
typedef enum {
    kSandringham = 0xF178AF,
    kCrossCity = 0x028430,
    kDandenong = 0x279FD5,
    kBurnley = 0x152C6B,
    kNorthern = 0xFFBE00,
    kClifton = 0xBE1014,
    kFlemington = 0x95979A,
    kSpecial = 0x95979A, // e.g. on the City Circle
    kVLine = 0x7F0D82,
    kOff = 0x000000, // LED off
} colour_t;