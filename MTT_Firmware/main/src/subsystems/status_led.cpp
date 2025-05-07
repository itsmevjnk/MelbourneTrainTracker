#include "subsystems/status_led.h"

/* for logging */
#include "esp_log.h"
const char* StatusLED::kTag = "status_led";

#include "esp_check.h" // error checking

#define STRING(x)                          #x // stringify operation

esp_err_t StatusLED::init() {
    ESP_RETURN_ON_ERROR(gpio_set_direction(LED_ACTY, GPIO_MODE_OUTPUT), kTag, "gpio_set_direction on pin " STRING(LED_ACTY) " failed");
    ESP_RETURN_ON_ERROR(actyOff(), kTag, "cannot turn off LED on pin " STRING(LED_ACTY));
    ESP_RETURN_ON_ERROR(gpio_set_direction(LED_ERROR, GPIO_MODE_OUTPUT), kTag, "gpio_set_direction on pin " STRING(LED_ERROR) " failed");
    ESP_RETURN_ON_ERROR(errorOff(), kTag, "cannot turn off LED on pin " STRING(LED_ERROR));

    return ESP_OK; // all good
}