/* aggregation of LED matrices driven by AW20216S chips */

#pragma once

#include "subsystems/aw20216s.h"

#include "hwconfig/pindefs.h"
#include "hwconfig/led_matrix.h"

#include "driver/gpio.h"

#include "colours.h"

class LEDMatrix {
public:
    static esp_err_t init(); // initialise LED matrix drivers

    /* enable driver output */
    static inline esp_err_t enableDrivers() {
        return gpio_set_level(DRV_EN, 1);
    }
    
    /* disable driver output */
    static inline esp_err_t disableDrivers() {
        return gpio_set_level(DRV_EN, 0);
    }

    static esp_err_t update(); // update all changed LEDs to display on the board
    
    static esp_err_t set(size_t offset, colour_t colour); // set an LED's colour
    static esp_err_t setMulti(const size_t* offsets, size_t leds, colour_t colour); // set multiple LEDs to a colour
    static esp_err_t fill(colour_t colour); // fill the entire buffer with a colour

private:
    static AW20216S* m_drivers[8]; // LED driver objects - initialised in init()
    static uint8_t* m_buffer; // LED matrix frame buffer - within a driver: (CS1,SW1) .. (CSn,SW1), (CS1, SW2) .. (CSn, SWm)
    static const size_t kBufferSize; // size of m_buffer
    static const size_t kBufferOffsets[8]; // offsets into the buffer for each LED driver
    
    static const char* kTag;
};
