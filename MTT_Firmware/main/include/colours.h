/* LED colours */

#pragma once

// from the PTV Visual Identity Toolkit 2.0: https://archive.org/details/ptv-vit
typedef enum {
    kSandringham = 0xF178AF,
    kFrankston = 0x028430,
    // Newport line colour will depend on whether it's connected to Sandringham or Frankston
    kDandenong = 0x279FD5,
    kBurnley = 0x1A3786, // brightness boosted by 25%, as it's predominantly blue which is significantly downscaled
    kNorthern = 0xFFBE00,
    kClifton = 0xBE1014,
    kFlemington = 0x95979A, 
    kSpecial = 0x95979A, // e.g. on the City Circle
    kVLine = 0x8F1A95,
    kDisruption = 0xFE5000,
    kOff = 0x000000, // LED off
} colour_t;