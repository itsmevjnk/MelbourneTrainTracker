/* functions to drive status LEDs */

#pragma once

#include <driver/gpio.h>
#include "hwconfig/pindefs.h"

class StatusLED {
    public:
        /* initialise status LED pins */
        static esp_err_t init();

        /* turn on the activity LED */
        static inline esp_err_t actyOn() {
            return gpio_set_level(LED_ACTY, 1);
        }
        
        /* turn off the activity LED */
        static inline esp_err_t actyOff() {
            return gpio_set_level(LED_ACTY, 0);
        }

        /* turn on the error LED */
        static inline esp_err_t errorOn() {
            return gpio_set_level(LED_ERROR, 1);
        }
        
        /* turn off the error LED */
        static inline esp_err_t errorOff() {
            return gpio_set_level(LED_ERROR, 0);
        }
};