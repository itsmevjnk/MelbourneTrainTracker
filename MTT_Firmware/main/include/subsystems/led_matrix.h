/* aggregation of LED matrices driven by AW20216S chips */

#pragma once

#include "subsystems/aw20216s.h"

#include "hwconfig/pindefs.h"
#include "hwconfig/led_matrix.h"

#include "driver/gpio.h"

#include "colours.h"

#include <array>
#include <cmath>

// #define CONFIG_LMAT_STRICT_COLOUR_CHECK // TODO: remove this in prod

/* LED calibration parameters */
// TODO: rev1 calibration
#define LMAT_SCALE_GLOBAL                               0.5f // global brightness scaling
// per-colour brightness scaling
#if CONFIG_IDF_TARGET_ESP32S3
#define LMAT_SCALE_RED                                  0.985f
#define LMAT_SCALE_GREEN                                1.0f
#define LMAT_SCALE_BLUE                                 0.65f
#else
#define LMAT_SCALE_RED                                  0.9143f
#define LMAT_SCALE_GREEN                                1.0f
#define LMAT_SCALE_BLUE                                 0.8421f
#endif

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

    static uint8_t getBrightness();
    static void setBrightness(uint8_t value, bool rescale = false);

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
    static float m_brightness;

    static AW20216S* m_drivers[NUM_DRIVERS]; // LED driver objects - initialised in init()
    static const size_t kBufferOffsets[NUM_DRIVERS]; // offsets into the buffer for each LED driver

    static bool m_driverState;

    static StaticSemaphore_t m_bufferMutexBuf;
    static SemaphoreHandle_t m_bufferMutex;

#ifndef CONFIG_LMAT_GAMMA
#define CONFIG_LMAT_GAMMA                           2200
#endif

    static constexpr std::array<std::array<uint8_t, 256>, 3> m_forwardLUT = [] {
        std::array<std::array<uint8_t, 256>, 3> table{};

        float gamma = CONFIG_LMAT_GAMMA / 1000.0f;
        for (int i = 0; i < 256; i++) {
            float globalVal = LMAT_SCALE_GLOBAL * std::pow(i / 255.0, gamma) * 255.0;
            table[0][i] = globalVal * LMAT_SCALE_RED;
            table[1][i] = globalVal * LMAT_SCALE_GREEN;
            table[2][i] = globalVal * LMAT_SCALE_BLUE;
        }
        
        return table;
    }(); // [0] = red, [1] = green, [2] = blue

    static constexpr std::array<uint8_t, 768> m_reverseLUT = [] {
        std::array<uint8_t, 768> table{};

        for (int col = 0; col < 3; col++) {
            for (int i = 0; i < 256; i++) {
                table[col * 256 + m_forwardLUT[col][i]] = i;
            }
        }

        return table;
    }();

    static uint32_t applyCorrection(colour_t colour);

    static esp_err_t setRaw(size_t offset, uint32_t colour);

#ifdef CONFIG_LMAT_STRICT_COLOUR_CHECK
    static const uint8_t* m_expectedColours; // expected colours for each LED - basically a copy of m_buffer with all LEDs set to line colours
#endif

    static const char* kTag;

public:
    static constexpr const uint8_t* getReverseCorrectionLUT() { // get the 256 * 3 (256*R-256*G-256*B) reverse colour correction LUT - used for web interface
        return m_reverseLUT.data();
    }
};
