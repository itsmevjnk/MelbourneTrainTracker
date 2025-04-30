/* subsystems */
#include "subsystems/status_led.h"

#include "esp_log.h"
static const char* TAG = "main"; // for logging

/* firmware entry point */
extern "C" void app_main() {
    ESP_LOGI(TAG, "init begin");
    StatusLED::init();
    ESP_LOGI(TAG, "init end");

    /* placeholder */
    while (true) {
        ESP_LOGI(TAG, "spinning");
        StatusLED::actyOn(); StatusLED::errorOff(); vTaskDelay(500 / portTICK_PERIOD_MS); // delay for 500 msec
        StatusLED::actyOff(); StatusLED::errorOn(); vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}