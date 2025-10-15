/* aggregation of LED matrices driven by AW20216S chips */

#pragma once

#include "subsystems/aw20216s.h"

#include "hwconfig/pindefs.h"
#include "hwconfig/led_matrix.h"

#include "driver/gpio.h"

#include "colours.h"

// #define CONFIG_LMAT_STRICT_COLOUR_CHECK // TODO: remove this in prod

class LEDMatrix {
public:
    static esp_err_t init(); // initialise LED matrix drivers

    /* enable driver output */
    static inline esp_err_t enableDrivers() {
        m_driverState = true;
        return gpio_set_level(DRV_EN, 1);
    }
    
    /* disable driver output */
    static inline esp_err_t disableDrivers() {
        m_driverState = false;
        return gpio_set_level(DRV_EN, 0);
    }

    static inline bool getState() {
        return m_driverState;
    }

    static esp_err_t update(); // update all changed LEDs to display on the board
    
    static esp_err_t set(size_t offset, colour_t colour); // set an LED's colour
    static esp_err_t setMulti(const size_t* offsets, size_t leds, colour_t colour); // set multiple LEDs to a colour
    static esp_err_t fill(colour_t colour); // fill the entire buffer with a colour

    static const size_t kBurnleyOffsets[];
    static const size_t kCliftonOffsets[];
    static const size_t kCrossCityOffsets[];
    static const size_t kDandenongOffsets[];
    static const size_t kMunnelOffsets[];
    static const size_t kFlemingtonOffsets[];
    static const size_t kNorthernOffsets[];
    static const size_t kSandringhamOffsets[];
    static const size_t kVLineOffsets[];

    static uint8_t* m_buffer; // LED matrix frame buffer - within a driver: (CS1,SW1) .. (CSn,SW1), (CS1, SW2) .. (CSn, SWm)

    static inline void acquireBuffer() {
        xSemaphoreTakeRecursive(m_bufferMutex, portMAX_DELAY);
    }

    static inline void releaseBuffer() {
        xSemaphoreGiveRecursive(m_bufferMutex);
    }

private:
    static AW20216S* m_drivers[8]; // LED driver objects - initialised in init()
    static const size_t kBufferOffsets[8]; // offsets into the buffer for each LED driver

    static bool m_driverState;

    static StaticSemaphore_t m_bufferMutexBuf;
    static SemaphoreHandle_t m_bufferMutex;

#ifdef CONFIG_LMAT_STRICT_COLOUR_CHECK
    static const uint8_t* m_expectedColours; // expected colours for each LED - basically a copy of m_buffer with all LEDs set to line colours
#endif

    static const char* kTag;
};
