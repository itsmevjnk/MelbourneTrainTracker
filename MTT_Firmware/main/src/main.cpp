/* subsystems */
#include "esp_err.h"
#include "subsystems/status_led.h"
#include "subsystems/led_matrix.h"
#include "subsystems/wifi.h"
#include "subsystems/uart.h"
#include "subsystems/ntp.h"
#include "subsystems/mqtt.h"
#include "subsystems/webserver.h"
#include "subsystems/ota.h"

#include "driver/gpio.h"

#include "message.h"
#include "services.h"

#include "config.h"

#include "esp_log.h"
static const char* kTag = "main"; // for logging

#include "zlib.h"

extern const uint8_t badapple_start[] asm("_binary_badapple_dat_gz_start");
extern const uint8_t badapple_end[] asm("_binary_badapple_dat_gz_end");

/* firmware entry point */
extern "C" void app_main() {
    // ESP_LOGI(kTag, "waiting");
    // vTaskDelay(3000 / portTICK_PERIOD_MS);

    ESP_LOGI(kTag, "init begin");

    ESP_ERROR_CHECK(UART::init());
    ESP_ERROR_CHECK(StatusLED::init()); ESP_ERROR_CHECK(StatusLED::actyOn()); ESP_ERROR_CHECK(StatusLED::errorOn());
    ESP_ERROR_CHECK(LEDMatrix::init());

    /* colour calibration */
    // TODO: rev1 board calibration
    // float redScale = 0.6875, greenScale = 0.70, blueScale = 0.55;
    // float redScale = 0.985, greenScale = 1.0, blueScale = 0.65;
    // float globalScale = 0.5; // to prevent saturation/overcurrent
    // uint8_t red = 255.0 * globalScale * redScale, green = 255.0 * globalScale * greenScale, blue = 255.0 * globalScale * blueScale;
    
    // while(1) {
    //     /* white */
    //     LEDMatrix::fill((colour_t)((red << 16) | (green << 8) | blue)); LEDMatrix::update();
    //     vTaskDelay(pdMS_TO_TICKS(2000));
    //     /* yellow */
    //     LEDMatrix::fill((colour_t)((red << 16) | (green << 8))); LEDMatrix::update();
    //     vTaskDelay(pdMS_TO_TICKS(2000));
    //     /* cyan */
    //     LEDMatrix::fill((colour_t)((green << 8) | blue)); LEDMatrix::update();
    //     vTaskDelay(pdMS_TO_TICKS(2000));
    //     /* magenta */
    //     LEDMatrix::fill((colour_t)((red << 16) | blue)); LEDMatrix::update();
    //     vTaskDelay(pdMS_TO_TICKS(2000));
    // }
    
    static const size_t offsets[] = {LMAT_BASE(0, 0, 0), LMAT_BASE(0, 6, 1), LMAT_BASE(1, 6, 3), LMAT_BASE(1, 8, 3), LMAT_BASE(1, 10, 3), LMAT_BASE(1, 1, 3), LMAT_BASE(1, 3, 3), LMAT_BASE(1, 5, 3), LMAT_BASE(1, 7, 3), LMAT_BASE(1, 9, 3), LMAT_BASE(1, 11, 3), LMAT_BASE(1, 0, 5), LMAT_BASE(0, 8, 1), LMAT_BASE(1, 2, 5), LMAT_BASE(1, 4, 5), LMAT_BASE(1, 6, 5), LMAT_BASE(1, 10, 5), LMAT_BASE(1, 8, 5), LMAT_BASE(1, 11, 5), LMAT_BASE(1, 9, 5), LMAT_BASE(1, 7, 5), LMAT_BASE(1, 5, 5), LMAT_BASE(1, 3, 5), LMAT_BASE(0, 10, 1), LMAT_BASE(1, 1, 5), LMAT_BASE(1, 10, 0), LMAT_BASE(1, 10, 4), LMAT_BASE(1, 8, 0), LMAT_BASE(1, 8, 4), LMAT_BASE(1, 6, 0), LMAT_BASE(1, 6, 4), LMAT_BASE(1, 4, 0), LMAT_BASE(1, 4, 4), LMAT_BASE(1, 2, 0), LMAT_BASE(0, 1, 0), LMAT_BASE(1, 2, 4), LMAT_BASE(1, 0, 0), LMAT_BASE(1, 0, 4), LMAT_BASE(1, 1, 0), LMAT_BASE(1, 1, 4), LMAT_BASE(1, 3, 0), LMAT_BASE(1, 3, 4), LMAT_BASE(1, 5, 0), LMAT_BASE(1, 5, 4), LMAT_BASE(1, 7, 0), LMAT_BASE(0, 3, 0), LMAT_BASE(1, 7, 4), LMAT_BASE(1, 9, 0), LMAT_BASE(1, 9, 4), LMAT_BASE(1, 11, 0), LMAT_BASE(1, 11, 4), LMAT_BASE(2, 0, 2), LMAT_BASE(2, 2, 2), LMAT_BASE(2, 4, 2), LMAT_BASE(2, 6, 2), LMAT_BASE(2, 8, 2), LMAT_BASE(0, 5, 0), LMAT_BASE(2, 10, 2), LMAT_BASE(2, 1, 2), LMAT_BASE(2, 3, 2), LMAT_BASE(2, 5, 2), LMAT_BASE(2, 7, 2), LMAT_BASE(2, 9, 2), LMAT_BASE(2, 11, 2), LMAT_BASE(2, 1, 1), LMAT_BASE(2, 3, 1), LMAT_BASE(2, 5, 1), LMAT_BASE(0, 7, 0), LMAT_BASE(2, 7, 1), LMAT_BASE(2, 9, 1), LMAT_BASE(2, 11, 1), LMAT_BASE(2, 10, 1), LMAT_BASE(2, 8, 1), LMAT_BASE(2, 6, 1), LMAT_BASE(2, 4, 1), LMAT_BASE(2, 2, 1), LMAT_BASE(2, 0, 1), LMAT_BASE(2, 10, 0), LMAT_BASE(0, 9, 0), LMAT_BASE(2, 8, 0), LMAT_BASE(2, 6, 0), LMAT_BASE(2, 4, 0), LMAT_BASE(2, 2, 0), LMAT_BASE(2, 0, 0), LMAT_BASE(2, 1, 0), LMAT_BASE(2, 3, 0), LMAT_BASE(2, 5, 0), LMAT_BASE(2, 7, 0), LMAT_BASE(2, 9, 0), LMAT_BASE(0, 11, 0), LMAT_BASE(2, 11, 0), LMAT_BASE(2, 8, 3), LMAT_BASE(2, 10, 3), LMAT_BASE(2, 6, 3), LMAT_BASE(2, 4, 3), LMAT_BASE(2, 2, 3), LMAT_BASE(2, 0, 3), LMAT_BASE(2, 1, 3), LMAT_BASE(2, 3, 3), LMAT_BASE(2, 5, 3), LMAT_BASE(0, 1, 1), LMAT_BASE(2, 7, 3), LMAT_BASE(2, 9, 3), LMAT_BASE(2, 11, 3), LMAT_BASE(2, 10, 4), LMAT_BASE(2, 8, 4), LMAT_BASE(2, 6, 4), LMAT_BASE(2, 4, 4), LMAT_BASE(2, 2, 4), LMAT_BASE(2, 0, 4), LMAT_BASE(2, 1, 4), LMAT_BASE(0, 2, 0), LMAT_BASE(0, 3, 1), LMAT_BASE(2, 3, 4), LMAT_BASE(2, 5, 4), LMAT_BASE(2, 7, 4), LMAT_BASE(2, 9, 4), LMAT_BASE(2, 11, 4), LMAT_BASE(2, 11, 5), LMAT_BASE(2, 9, 5), LMAT_BASE(2, 7, 5), LMAT_BASE(2, 5, 5), LMAT_BASE(2, 3, 5), LMAT_BASE(0, 5, 1), LMAT_BASE(2, 1, 5), LMAT_BASE(2, 10, 5), LMAT_BASE(2, 8, 5), LMAT_BASE(2, 6, 5), LMAT_BASE(2, 4, 5), LMAT_BASE(2, 2, 5), LMAT_BASE(2, 0, 5), LMAT_BASE(3, 0, 1), LMAT_BASE(3, 2, 1), LMAT_BASE(3, 3, 1), LMAT_BASE(0, 7, 1), LMAT_BASE(3, 1, 1), LMAT_BASE(3, 5, 1), LMAT_BASE(3, 10, 1), LMAT_BASE(3, 7, 1), LMAT_BASE(3, 8, 1), LMAT_BASE(3, 9, 1), LMAT_BASE(3, 4, 1), LMAT_BASE(3, 11, 1), LMAT_BASE(3, 6, 1), LMAT_BASE(3, 0, 0), LMAT_BASE(0, 9, 1), LMAT_BASE(3, 1, 0), LMAT_BASE(3, 2, 0), LMAT_BASE(3, 3, 0), LMAT_BASE(3, 4, 0), LMAT_BASE(3, 5, 0), LMAT_BASE(3, 6, 0), LMAT_BASE(3, 7, 0), LMAT_BASE(3, 8, 0), LMAT_BASE(3, 9, 0), LMAT_BASE(3, 10, 0), LMAT_BASE(0, 11, 1), LMAT_BASE(3, 11, 0), LMAT_BASE(3, 2, 4), LMAT_BASE(3, 4, 4), LMAT_BASE(3, 0, 4), LMAT_BASE(3, 6, 4), LMAT_BASE(3, 1, 4), LMAT_BASE(3, 8, 4), LMAT_BASE(3, 3, 4), LMAT_BASE(3, 10, 4), LMAT_BASE(3, 11, 4), LMAT_BASE(0, 0, 2), LMAT_BASE(3, 5, 4), LMAT_BASE(3, 7, 4), LMAT_BASE(3, 9, 4), LMAT_BASE(3, 2, 5), LMAT_BASE(3, 10, 5), LMAT_BASE(3, 0, 5), LMAT_BASE(3, 8, 5), LMAT_BASE(3, 1, 5), LMAT_BASE(3, 4, 5), LMAT_BASE(3, 3, 5), LMAT_BASE(0, 2, 2), LMAT_BASE(3, 6, 5), LMAT_BASE(3, 11, 5), LMAT_BASE(3, 9, 5), LMAT_BASE(3, 7, 5), LMAT_BASE(3, 5, 5), LMAT_BASE(3, 10, 2), LMAT_BASE(3, 2, 2), LMAT_BASE(3, 8, 2), LMAT_BASE(3, 0, 2), LMAT_BASE(3, 4, 2), LMAT_BASE(0, 4, 2), LMAT_BASE(3, 1, 2), LMAT_BASE(3, 6, 2), LMAT_BASE(3, 3, 2), LMAT_BASE(3, 9, 2), LMAT_BASE(3, 11, 2), LMAT_BASE(3, 7, 2), LMAT_BASE(3, 5, 2), LMAT_BASE(3, 0, 3), LMAT_BASE(3, 2, 3), LMAT_BASE(3, 4, 3), LMAT_BASE(0, 6, 2), LMAT_BASE(3, 6, 3), LMAT_BASE(3, 8, 3), LMAT_BASE(3, 3, 3), LMAT_BASE(3, 1, 3), LMAT_BASE(3, 10, 3), LMAT_BASE(3, 5, 3), LMAT_BASE(3, 7, 3), LMAT_BASE(3, 9, 3), LMAT_BASE(3, 11, 3), LMAT_BASE(4, 7, 0), LMAT_BASE(0, 8, 2), LMAT_BASE(4, 5, 0), LMAT_BASE(4, 9, 0), LMAT_BASE(4, 11, 0), LMAT_BASE(4, 3, 0), LMAT_BASE(4, 1, 0), LMAT_BASE(4, 0, 0), LMAT_BASE(4, 2, 0), LMAT_BASE(4, 8, 0), LMAT_BASE(4, 10, 0), LMAT_BASE(4, 6, 0), LMAT_BASE(0, 4, 0), LMAT_BASE(0, 10, 2), LMAT_BASE(4, 4, 0), LMAT_BASE(4, 11, 1), LMAT_BASE(4, 9, 1), LMAT_BASE(4, 7, 1), LMAT_BASE(4, 5, 1), LMAT_BASE(4, 3, 1), LMAT_BASE(4, 1, 1), LMAT_BASE(4, 0, 1), LMAT_BASE(4, 2, 1), LMAT_BASE(4, 4, 1), LMAT_BASE(0, 3, 2), LMAT_BASE(4, 6, 1), LMAT_BASE(4, 8, 1), LMAT_BASE(4, 10, 1), LMAT_BASE(4, 0, 2), LMAT_BASE(4, 2, 2), LMAT_BASE(4, 4, 2), LMAT_BASE(4, 7, 2), LMAT_BASE(4, 8, 2), LMAT_BASE(4, 10, 2), LMAT_BASE(4, 6, 2), LMAT_BASE(0, 5, 2), LMAT_BASE(4, 11, 2), LMAT_BASE(4, 5, 2), LMAT_BASE(4, 3, 2), LMAT_BASE(4, 9, 2), LMAT_BASE(4, 1, 2), LMAT_BASE(4, 11, 5), LMAT_BASE(4, 10, 4), LMAT_BASE(4, 11, 3), LMAT_BASE(4, 9, 5), LMAT_BASE(4, 8, 4), LMAT_BASE(0, 7, 2), LMAT_BASE(4, 9, 3), LMAT_BASE(4, 7, 5), LMAT_BASE(4, 6, 4), LMAT_BASE(4, 1, 3), LMAT_BASE(4, 5, 5), LMAT_BASE(4, 4, 4), LMAT_BASE(4, 3, 3), LMAT_BASE(4, 3, 5), LMAT_BASE(4, 2, 4), LMAT_BASE(4, 5, 3), LMAT_BASE(0, 9, 2), LMAT_BASE(4, 1, 5), LMAT_BASE(4, 0, 4), LMAT_BASE(4, 7, 3), LMAT_BASE(4, 0, 5), LMAT_BASE(4, 1, 4), LMAT_BASE(4, 8, 3), LMAT_BASE(4, 2, 5), LMAT_BASE(4, 3, 4), LMAT_BASE(4, 10, 3), LMAT_BASE(4, 4, 5), LMAT_BASE(0, 11, 2), LMAT_BASE(4, 5, 4), LMAT_BASE(4, 4, 3), LMAT_BASE(4, 6, 5), LMAT_BASE(4, 7, 4), LMAT_BASE(4, 6, 3), LMAT_BASE(4, 8, 5), LMAT_BASE(4, 9, 4), LMAT_BASE(4, 0, 3), LMAT_BASE(4, 10, 5), LMAT_BASE(4, 11, 4), LMAT_BASE(0, 1, 2), LMAT_BASE(4, 2, 3), LMAT_BASE(5, 0, 0), LMAT_BASE(5, 1, 0), LMAT_BASE(5, 2, 0), LMAT_BASE(5, 3, 0), LMAT_BASE(5, 4, 0), LMAT_BASE(5, 5, 0), LMAT_BASE(5, 6, 0), LMAT_BASE(5, 7, 0), LMAT_BASE(5, 8, 0), LMAT_BASE(0, 1, 3), LMAT_BASE(5, 9, 0), LMAT_BASE(5, 10, 0), LMAT_BASE(5, 11, 0), LMAT_BASE(5, 0, 1), LMAT_BASE(5, 1, 1), LMAT_BASE(5, 2, 1), LMAT_BASE(5, 3, 1), LMAT_BASE(5, 4, 1), LMAT_BASE(5, 5, 1), LMAT_BASE(5, 6, 1), LMAT_BASE(0, 11, 3), LMAT_BASE(5, 7, 1), LMAT_BASE(5, 8, 1), LMAT_BASE(5, 9, 1), LMAT_BASE(5, 10, 1), LMAT_BASE(5, 11, 1), LMAT_BASE(5, 0, 5), LMAT_BASE(5, 1, 5), LMAT_BASE(5, 2, 5), LMAT_BASE(5, 3, 5), LMAT_BASE(5, 4, 5), LMAT_BASE(0, 9, 3), LMAT_BASE(5, 5, 5), LMAT_BASE(5, 6, 5), LMAT_BASE(5, 7, 5), LMAT_BASE(5, 8, 5), LMAT_BASE(5, 9, 5), LMAT_BASE(5, 10, 5), LMAT_BASE(5, 11, 5), LMAT_BASE(5, 11, 2), LMAT_BASE(5, 9, 2), LMAT_BASE(5, 7, 2), LMAT_BASE(0, 6, 0), LMAT_BASE(0, 7, 3), LMAT_BASE(5, 5, 2), LMAT_BASE(5, 3, 2), LMAT_BASE(5, 1, 2), LMAT_BASE(5, 0, 2), LMAT_BASE(5, 2, 2), LMAT_BASE(5, 4, 2), LMAT_BASE(5, 6, 2), LMAT_BASE(5, 8, 2), LMAT_BASE(5, 10, 2), LMAT_BASE(5, 1, 4), LMAT_BASE(0, 5, 3), LMAT_BASE(5, 5, 4), LMAT_BASE(5, 7, 4), LMAT_BASE(5, 9, 4), LMAT_BASE(5, 11, 4), LMAT_BASE(5, 3, 4), LMAT_BASE(5, 0, 4), LMAT_BASE(5, 2, 4), LMAT_BASE(5, 4, 4), LMAT_BASE(5, 6, 4), LMAT_BASE(5, 8, 4), LMAT_BASE(0, 3, 3), LMAT_BASE(5, 10, 4), LMAT_BASE(5, 0, 3), LMAT_BASE(5, 1, 3), LMAT_BASE(5, 2, 3), LMAT_BASE(5, 3, 3), LMAT_BASE(5, 4, 3), LMAT_BASE(5, 5, 3), LMAT_BASE(5, 6, 3), LMAT_BASE(5, 7, 3), LMAT_BASE(5, 8, 3), LMAT_BASE(0, 10, 3), LMAT_BASE(5, 9, 3), LMAT_BASE(5, 10, 3), LMAT_BASE(5, 11, 3), LMAT_BASE(6, 11, 0), LMAT_BASE(6, 10, 0), LMAT_BASE(6, 9, 0), LMAT_BASE(6, 8, 0), LMAT_BASE(6, 7, 0), LMAT_BASE(6, 6, 0), LMAT_BASE(6, 5, 0), LMAT_BASE(0, 8, 3), LMAT_BASE(6, 4, 0), LMAT_BASE(6, 3, 0), LMAT_BASE(6, 2, 0), LMAT_BASE(6, 1, 0), LMAT_BASE(6, 0, 0), LMAT_BASE(6, 11, 1), LMAT_BASE(6, 10, 1), LMAT_BASE(6, 9, 1), LMAT_BASE(6, 8, 1), LMAT_BASE(6, 7, 1), LMAT_BASE(0, 6, 3), LMAT_BASE(6, 6, 1), LMAT_BASE(6, 5, 1), LMAT_BASE(6, 4, 1), LMAT_BASE(6, 3, 1), LMAT_BASE(6, 2, 1), LMAT_BASE(6, 1, 1), LMAT_BASE(6, 0, 1), LMAT_BASE(6, 11, 2), LMAT_BASE(6, 10, 2), LMAT_BASE(6, 9, 2), LMAT_BASE(0, 2, 3), LMAT_BASE(6, 8, 2), LMAT_BASE(6, 7, 2), LMAT_BASE(6, 6, 2), LMAT_BASE(6, 5, 2), LMAT_BASE(6, 4, 2), LMAT_BASE(6, 3, 2), LMAT_BASE(6, 2, 2), LMAT_BASE(6, 1, 2), LMAT_BASE(6, 0, 2), LMAT_BASE(6, 0, 3), LMAT_BASE(0, 4, 3), LMAT_BASE(6, 2, 3), LMAT_BASE(6, 4, 3), LMAT_BASE(6, 6, 3), LMAT_BASE(6, 8, 3), LMAT_BASE(6, 10, 3), LMAT_BASE(6, 1, 3), LMAT_BASE(6, 3, 3), LMAT_BASE(6, 5, 3), LMAT_BASE(6, 7, 3), LMAT_BASE(6, 9, 3), LMAT_BASE(0, 0, 3), LMAT_BASE(6, 11, 3), LMAT_BASE(6, 0, 4), LMAT_BASE(6, 2, 4), LMAT_BASE(6, 4, 4), LMAT_BASE(6, 6, 4), LMAT_BASE(6, 8, 4), LMAT_BASE(6, 10, 4), LMAT_BASE(6, 1, 4), LMAT_BASE(6, 3, 4), LMAT_BASE(6, 5, 4), LMAT_BASE(0, 0, 4), LMAT_BASE(6, 7, 4), LMAT_BASE(6, 9, 4), LMAT_BASE(6, 11, 4), LMAT_BASE(6, 11, 5), LMAT_BASE(6, 9, 5), LMAT_BASE(6, 7, 5), LMAT_BASE(6, 5, 5), LMAT_BASE(6, 3, 5), LMAT_BASE(6, 1, 5), LMAT_BASE(6, 10, 5), LMAT_BASE(0, 8, 0), LMAT_BASE(0, 11, 4), LMAT_BASE(6, 8, 5), LMAT_BASE(6, 6, 5), LMAT_BASE(6, 4, 5), LMAT_BASE(6, 2, 5), LMAT_BASE(6, 0, 5), LMAT_BASE(7, 11, 5), LMAT_BASE(7, 11, 3), LMAT_BASE(10, 9, 1), LMAT_BASE(7, 9, 5), LMAT_BASE(7, 9, 3), LMAT_BASE(0, 2, 4), LMAT_BASE(10, 9, 0), LMAT_BASE(7, 7, 5), LMAT_BASE(7, 7, 3), LMAT_BASE(10, 9, 5), LMAT_BASE(7, 5, 5), LMAT_BASE(7, 5, 3), LMAT_BASE(10, 11, 1), LMAT_BASE(7, 3, 5), LMAT_BASE(7, 3, 3), LMAT_BASE(7, 3, 4), LMAT_BASE(0, 9, 4), LMAT_BASE(7, 1, 5), LMAT_BASE(7, 1, 3), LMAT_BASE(7, 1, 4), LMAT_BASE(7, 0, 5), LMAT_BASE(7, 0, 3), LMAT_BASE(7, 0, 4), LMAT_BASE(7, 2, 5), LMAT_BASE(7, 2, 3), LMAT_BASE(7, 2, 4), LMAT_BASE(7, 4, 5), LMAT_BASE(0, 4, 4), LMAT_BASE(7, 4, 3), LMAT_BASE(7, 4, 4), LMAT_BASE(7, 6, 5), LMAT_BASE(7, 6, 3), LMAT_BASE(7, 6, 4), LMAT_BASE(7, 8, 5), LMAT_BASE(7, 8, 3), LMAT_BASE(7, 8, 4), LMAT_BASE(7, 10, 5), LMAT_BASE(7, 10, 3), LMAT_BASE(0, 7, 4), LMAT_BASE(7, 10, 4), LMAT_BASE(7, 3, 2), LMAT_BASE(7, 11, 0), LMAT_BASE(7, 11, 1), LMAT_BASE(7, 1, 2), LMAT_BASE(7, 9, 0), LMAT_BASE(7, 9, 1), LMAT_BASE(7, 0, 2), LMAT_BASE(7, 7, 0), LMAT_BASE(7, 7, 1), LMAT_BASE(0, 6, 4), LMAT_BASE(7, 2, 2), LMAT_BASE(7, 5, 0), LMAT_BASE(7, 5, 1), LMAT_BASE(7, 4, 2), LMAT_BASE(7, 3, 0), LMAT_BASE(7, 3, 1), LMAT_BASE(7, 6, 2), LMAT_BASE(7, 1, 0), LMAT_BASE(7, 1, 1), LMAT_BASE(7, 8, 2), LMAT_BASE(0, 5, 4), LMAT_BASE(7, 0, 0), LMAT_BASE(7, 0, 1), LMAT_BASE(7, 10, 2), LMAT_BASE(7, 2, 0), LMAT_BASE(7, 2, 1), LMAT_BASE(7, 5, 2), LMAT_BASE(7, 4, 0), LMAT_BASE(7, 4, 1), LMAT_BASE(7, 7, 2), LMAT_BASE(7, 6, 0), LMAT_BASE(0, 8, 4), LMAT_BASE(7, 6, 1), LMAT_BASE(7, 9, 2), LMAT_BASE(7, 8, 0), LMAT_BASE(7, 8, 1), LMAT_BASE(7, 11, 2), LMAT_BASE(7, 10, 0), LMAT_BASE(7, 10, 1), LMAT_BASE(8, 9, 2), LMAT_BASE(8, 9, 1), LMAT_BASE(8, 9, 0), LMAT_BASE(0, 3, 4), LMAT_BASE(8, 2, 5), LMAT_BASE(8, 3, 4), LMAT_BASE(8, 8, 3), LMAT_BASE(8, 11, 2), LMAT_BASE(8, 11, 1), LMAT_BASE(8, 11, 0), LMAT_BASE(8, 0, 5), LMAT_BASE(8, 1, 4), LMAT_BASE(8, 10, 3), LMAT_BASE(8, 5, 2), LMAT_BASE(0, 10, 4), LMAT_BASE(8, 5, 1), LMAT_BASE(8, 5, 0), LMAT_BASE(8, 6, 5), LMAT_BASE(8, 7, 4), LMAT_BASE(8, 4, 3), LMAT_BASE(8, 7, 2), LMAT_BASE(8, 7, 1), LMAT_BASE(8, 7, 0), LMAT_BASE(8, 4, 5), LMAT_BASE(8, 5, 4), LMAT_BASE(0, 10, 0), LMAT_BASE(0, 1, 4), LMAT_BASE(8, 6, 3), LMAT_BASE(8, 1, 2), LMAT_BASE(8, 1, 1), LMAT_BASE(8, 1, 0), LMAT_BASE(8, 10, 5), LMAT_BASE(8, 11, 4), LMAT_BASE(8, 11, 3), LMAT_BASE(8, 3, 2), LMAT_BASE(8, 3, 1), LMAT_BASE(8, 3, 0), LMAT_BASE(0, 0, 5), LMAT_BASE(8, 8, 5), LMAT_BASE(8, 9, 4), LMAT_BASE(8, 5, 3), LMAT_BASE(8, 2, 2), LMAT_BASE(8, 2, 1), LMAT_BASE(8, 2, 0), LMAT_BASE(8, 3, 5), LMAT_BASE(8, 8, 4), LMAT_BASE(8, 0, 3), LMAT_BASE(8, 0, 2), LMAT_BASE(0, 2, 5), LMAT_BASE(8, 0, 1), LMAT_BASE(8, 0, 0), LMAT_BASE(8, 1, 5), LMAT_BASE(8, 10, 4), LMAT_BASE(8, 2, 3), LMAT_BASE(8, 6, 2), LMAT_BASE(8, 6, 1), LMAT_BASE(8, 6, 0), LMAT_BASE(8, 7, 5), LMAT_BASE(8, 4, 4), LMAT_BASE(0, 4, 5), LMAT_BASE(8, 3, 3), LMAT_BASE(8, 4, 2), LMAT_BASE(8, 4, 1), LMAT_BASE(8, 4, 0), LMAT_BASE(8, 5, 5), LMAT_BASE(8, 6, 4), LMAT_BASE(8, 1, 3), LMAT_BASE(8, 10, 2), LMAT_BASE(8, 10, 1), LMAT_BASE(8, 10, 0), LMAT_BASE(0, 6, 5), LMAT_BASE(8, 11, 5), LMAT_BASE(8, 0, 4), LMAT_BASE(8, 9, 3), LMAT_BASE(8, 8, 2), LMAT_BASE(8, 8, 1), LMAT_BASE(8, 8, 0), LMAT_BASE(8, 9, 5), LMAT_BASE(8, 2, 4), LMAT_BASE(8, 7, 3), LMAT_BASE(9, 9, 1), LMAT_BASE(0, 8, 5), LMAT_BASE(9, 9, 2), LMAT_BASE(9, 9, 3), LMAT_BASE(9, 6, 4), LMAT_BASE(9, 8, 5), LMAT_BASE(10, 11, 0), LMAT_BASE(9, 11, 1), LMAT_BASE(9, 11, 2), LMAT_BASE(9, 11, 3), LMAT_BASE(9, 8, 4), LMAT_BASE(9, 10, 5), LMAT_BASE(0, 10, 5), LMAT_BASE(10, 11, 5), LMAT_BASE(9, 5, 1), LMAT_BASE(9, 5, 2), LMAT_BASE(9, 5, 3), LMAT_BASE(9, 2, 4), LMAT_BASE(9, 4, 5), LMAT_BASE(10, 5, 1), LMAT_BASE(9, 7, 1), LMAT_BASE(9, 7, 2), LMAT_BASE(9, 7, 3), LMAT_BASE(0, 1, 5), LMAT_BASE(9, 4, 4), LMAT_BASE(9, 6, 5), LMAT_BASE(10, 5, 0), LMAT_BASE(9, 1, 1), LMAT_BASE(9, 1, 2), LMAT_BASE(9, 1, 3), LMAT_BASE(9, 0, 4), LMAT_BASE(9, 0, 5), LMAT_BASE(10, 5, 5), LMAT_BASE(9, 3, 1), LMAT_BASE(0, 3, 5), LMAT_BASE(9, 3, 2), LMAT_BASE(9, 3, 3), LMAT_BASE(10, 7, 1), LMAT_BASE(9, 2, 5), LMAT_BASE(10, 7, 0), LMAT_BASE(9, 2, 1), LMAT_BASE(9, 2, 2), LMAT_BASE(9, 2, 3), LMAT_BASE(9, 3, 4), LMAT_BASE(9, 3, 5), LMAT_BASE(0, 5, 5), LMAT_BASE(10, 7, 5), LMAT_BASE(9, 0, 1), LMAT_BASE(9, 0, 2), LMAT_BASE(9, 0, 3), LMAT_BASE(9, 1, 4), LMAT_BASE(9, 1, 5), LMAT_BASE(10, 1, 1), LMAT_BASE(9, 6, 1), LMAT_BASE(9, 6, 2), LMAT_BASE(9, 6, 3), LMAT_BASE(0, 0, 1), LMAT_BASE(0, 7, 5), LMAT_BASE(9, 7, 4), LMAT_BASE(9, 7, 5), LMAT_BASE(10, 1, 0), LMAT_BASE(9, 4, 1), LMAT_BASE(9, 4, 2), LMAT_BASE(9, 4, 3), LMAT_BASE(9, 5, 4), LMAT_BASE(9, 5, 5), LMAT_BASE(10, 1, 5), LMAT_BASE(9, 10, 1), LMAT_BASE(0, 9, 5), LMAT_BASE(9, 10, 2), LMAT_BASE(9, 10, 3), LMAT_BASE(9, 11, 4), LMAT_BASE(9, 11, 5), LMAT_BASE(10, 3, 1), LMAT_BASE(9, 8, 1), LMAT_BASE(9, 8, 2), LMAT_BASE(9, 8, 3), LMAT_BASE(9, 9, 4), LMAT_BASE(9, 9, 5), LMAT_BASE(0, 11, 5), LMAT_BASE(10, 3, 0), LMAT_BASE(10, 3, 5), LMAT_BASE(10, 2, 1), LMAT_BASE(10, 2, 0), LMAT_BASE(10, 2, 5), LMAT_BASE(10, 0, 1), LMAT_BASE(10, 0, 0), LMAT_BASE(10, 0, 5), LMAT_BASE(10, 8, 1), LMAT_BASE(10, 6, 0), LMAT_BASE(1, 0, 2), LMAT_BASE(10, 10, 1), LMAT_BASE(10, 4, 0), LMAT_BASE(10, 4, 1), LMAT_BASE(10, 10, 0), LMAT_BASE(10, 6, 1), LMAT_BASE(10, 8, 0), LMAT_BASE(1, 2, 2), LMAT_BASE(1, 4, 2), LMAT_BASE(1, 6, 2), LMAT_BASE(1, 8, 2), LMAT_BASE(1, 10, 2), LMAT_BASE(1, 1, 2), LMAT_BASE(0, 2, 1), LMAT_BASE(1, 3, 2), LMAT_BASE(1, 5, 2), LMAT_BASE(1, 7, 2), LMAT_BASE(1, 9, 2), LMAT_BASE(1, 11, 2), LMAT_BASE(1, 0, 1), LMAT_BASE(1, 2, 1), LMAT_BASE(1, 4, 1), LMAT_BASE(1, 6, 1), LMAT_BASE(1, 8, 1), LMAT_BASE(0, 4, 1), LMAT_BASE(1, 10, 1), LMAT_BASE(1, 1, 1), LMAT_BASE(1, 3, 1), LMAT_BASE(1, 5, 1), LMAT_BASE(1, 7, 1), LMAT_BASE(1, 9, 1), LMAT_BASE(1, 11, 1), LMAT_BASE(1, 0, 3), LMAT_BASE(1, 2, 3), LMAT_BASE(1, 4, 3)};

    StatusLED::errorOff();
    while (true) {
        z_stream stream{};
        stream.next_in = (Bytef*)badapple_start;
        stream.avail_in = (uintptr_t)badapple_end - (uintptr_t)badapple_start;
        int ret = inflateInit2(&stream, 16 + MAX_WBITS);
        if (ret != Z_OK) {
            ESP_LOGE(kTag, "inflateInit2 failed (%d)", ret);
            abort();
        }

        ESP_LOGI(kTag, "inflateInit2 finished");

        static uint8_t buffer[sizeof(offsets) / sizeof(size_t)]; // for decompression
        size_t frame = 1;
        size_t filled = 0;
        TickType_t lastWakeTime = xTaskGetTickCount();
        do {
            stream.next_out = &buffer[filled];
            stream.avail_out = sizeof(buffer) - filled;

            ret = inflate(&stream, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&stream);
                ESP_LOGE(kTag, "inflate failed at frame %u (%d)", frame, ret);
                abort();
            }
            
            filled = sizeof(buffer) - stream.avail_out;

            if (filled == sizeof(buffer)) { // frame decompressed
                ESP_LOGI(kTag, "frame %u", frame);
                
                for (size_t i = 0; i < sizeof(buffer); i++) {
                    uint32_t x = buffer[i]; // brightness
                    uint32_t colour = (x << 16) | (x << 8) | x;
                    LEDMatrix::set(offsets[i], (colour_t)colour);
                }

                StatusLED::actyOff();
                LEDMatrix::update();

                vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(1000 / 25)); // 25fps Bad Apple - less frames and more precise timing

                filled = 0;
            }

            frame++;
        } while (ret != Z_STREAM_END);

        inflateEnd(&stream);

        
        StatusLED::actyOn();
        ESP_LOGI(kTag, "end, will restart in 5 sec");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}