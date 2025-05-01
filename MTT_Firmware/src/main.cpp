/* subsystems */
#include "subsystems/status_led.h"
#include "subsystems/led_matrix.h"

#include "esp_log.h"
static const char* TAG = "main"; // for logging

/* firmware entry point */
extern "C" void app_main() {
    // ESP_LOGI(TAG, "waiting");
    // vTaskDelay(3000 / portTICK_PERIOD_MS);

    ESP_LOGI(TAG, "init begin");
    ESP_ERROR_CHECK(StatusLED::init());
    ESP_ERROR_CHECK(LEDMatrix::init());
    ESP_LOGI(TAG, "init end");

    /* placeholder */
    while (true) {
        colour_t colours[9] = { kSandringham, kCrossCity, kDandenong, kBurnley, kNorthern, kClifton, kFlemington, kVLine, kOff };
        for (int i = 0; i < 9; i++) {
            colour_t colour = colours[i];
            ESP_LOGI(TAG, "displaying #%06X", colour);
            ESP_ERROR_CHECK(LEDMatrix::fill(colour));
            ESP_ERROR_CHECK(LEDMatrix::update());
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}