/* LED colours */

#pragma once

// inspired by the PTV Visual Identity Toolkit 2.0: https://archive.org/details/ptv-vit
typedef enum {
    kSandringham = 0xA0050A, // oddly enough the Clifton Hill colour works well for Sandringham (with some tweaks to reduce brightness)
    kCrossCity = 0x008000,
    kDandenong = 0x181880,
    kBurnley = 0x000080,
    kNorthern = 0x804000,
    kClifton = 0x800000,
    kFlemington = 0x606060, 
    kSpecial = 0x606060, // e.g. on the City Circle
    kVLine = 0x400080,
    kOff = 0x000000, // LED off
} colour_t;