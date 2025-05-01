/* subsystems */
#include "subsystems/status_led.h"
#include "subsystems/led_matrix.h"

#include "esp_log.h"
static const char* TAG = "main"; // for logging


static const size_t burnleyOffsets[] = { LMAT_BURNLEY };
static const size_t cliftonOffsets[] = { LMAT_CLIFTON };
static const size_t crosscityOffsets[] = { LMAT_CROSSCITY };
static const size_t dandenongOffsets[] = { LMAT_DANDENONG };
static const size_t munnelOffsets[] = { LMAT_MUNNEL };
static const size_t flemingtonOffsets[] = { LMAT_FLEMINGTON };
static const size_t northernOffsets[] = { LMAT_NORTHERN };
static const size_t sandringhamOffsets[] = { LMAT_SANDRINGHAM };
static const size_t vlineOffsets[] = { LMAT_VLINE };

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
        for (int i = 0; i < 8; i++) {
            ESP_ERROR_CHECK(LEDMatrix::fill(kOff));
            switch (i) {
                case 0:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(burnleyOffsets, sizeof(burnleyOffsets) / sizeof(size_t), kBurnley));
                    break;
                case 1:
                    ESP_ERROR_CHECK(LEDMatrix::set(LMAT_CLIFTON_FSS_CIRC, kSpecial));
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(cliftonOffsets, sizeof(cliftonOffsets) / sizeof(size_t), kClifton));
                    break;
                case 2:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(crosscityOffsets, sizeof(crosscityOffsets) / sizeof(size_t), kCrossCity));
                    break;
                case 3:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(dandenongOffsets, sizeof(dandenongOffsets) / sizeof(size_t), kDandenong));
                    break;
                case 4:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(munnelOffsets, sizeof(munnelOffsets) / sizeof(size_t), kDandenong));
                    break;
                case 5:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(flemingtonOffsets, sizeof(flemingtonOffsets) / sizeof(size_t), kFlemington));
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(northernOffsets, sizeof(northernOffsets) / sizeof(size_t), kNorthern));
                    break;
                case 6:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(sandringhamOffsets, sizeof(sandringhamOffsets) / sizeof(size_t), kSandringham));
                    break;
                case 7:
                    ESP_ERROR_CHECK(LEDMatrix::setMulti(vlineOffsets, sizeof(vlineOffsets) / sizeof(size_t), kVLine));
                    break;
                default:
                    ESP_LOGE(TAG, "invalid i=%d", i);
                    abort();
                    break;
            }

            ESP_ERROR_CHECK(LEDMatrix::update());
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}