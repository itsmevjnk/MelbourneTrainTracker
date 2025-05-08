/* subsystems */
#include "subsystems/status_led.h"
#include "subsystems/led_matrix.h"
#include "subsystems/wifi.h"
#include "subsystems/uart.h"
#include "subsystems/ntp.h"
#include "subsystems/mqtt.h"

#include "message.h"
#include "services.h"

#include "config.h"

#include "esp_log.h"
static const char* kTag = "main"; // for logging


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
    // ESP_LOGI(kTag, "waiting");
    // vTaskDelay(3000 / portTICK_PERIOD_MS);

    ESP_LOGI(kTag, "init begin");

    ESP_ERROR_CHECK(UART::init());
    ESP_ERROR_CHECK(StatusLED::init()); ESP_ERROR_CHECK(StatusLED::actyOn()); ESP_ERROR_CHECK(StatusLED::errorOn());
    ESP_ERROR_CHECK(LEDMatrix::init());
    
    /* load config */
    esp_err_t ret = Config::init();
    if (ret != ESP_OK) {
        ESP_LOGE(kTag, "configuration loading failed (%s), booting into configuration CLI", esp_err_to_name(ret));
        goto runCLI;
    } else {
        ESP_LOGI(kTag, "press any key within 3 seconds to boot into configuration CLI");
        uint8_t buf;
        if (UART::read(&buf, 1, 3000 / portTICK_PERIOD_MS) > 0) { // catch incoming byte, and if there's any, we boot into CLI
            ESP_LOGI(kTag, "booting into configuration CLI");
runCLI:
            ESP_ERROR_CHECK(Config::cli());
            while (true) { // while CLI is running on another task, we flash the two LEDs alternately
                ESP_ERROR_CHECK(StatusLED::actyOn()); ESP_ERROR_CHECK(StatusLED::errorOff()); vTaskDelay(500 / portTICK_PERIOD_MS);
                ESP_ERROR_CHECK(StatusLED::actyOff()); ESP_ERROR_CHECK(StatusLED::errorOn()); vTaskDelay(500 / portTICK_PERIOD_MS);
            }
        }
    }
    ESP_ERROR_CHECK(StatusLED::errorOff());

    if (Config::isWiFiEnterprise())
        ESP_ERROR_CHECK(WiFi::init(Config::getWiFiSSID(), Config::getWiFiIdentity(), Config::getWiFiUsername(), Config::getWiFiPassword(), Config::getWiFiCertificate(), Config::getWiFiCertLength()));
    else
        ESP_ERROR_CHECK(WiFi::init(Config::getWiFiSSID(), Config::getWiFiPassword()));

    ESP_ERROR_CHECK(NTP::init(Config::getTimeServer()));

    ESP_ERROR_CHECK(MQTT::init(Config::getMQTTBroker()));

    ESP_LOGI(kTag, "init end"); ESP_ERROR_CHECK(StatusLED::actyOff());

    /* placeholder */
    // while (true) {
    //     for (int i = 0; i < 8; i++) {
    //         ESP_ERROR_CHECK(LEDMatrix::fill(kOff));
    //         switch (i) {
    //             case 0:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(burnleyOffsets, sizeof(burnleyOffsets) / sizeof(size_t), kBurnley));
    //                 break;
    //             case 1:
    //                 ESP_ERROR_CHECK(LEDMatrix::set(LMAT_CLIFTON_FSS_CIRC, kSpecial));
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(cliftonOffsets, sizeof(cliftonOffsets) / sizeof(size_t), kClifton));
    //                 break;
    //             case 2:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(crosscityOffsets, sizeof(crosscityOffsets) / sizeof(size_t), kCrossCity));
    //                 break;
    //             case 3:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(dandenongOffsets, sizeof(dandenongOffsets) / sizeof(size_t), kDandenong));
    //                 break;
    //             case 4:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(munnelOffsets, sizeof(munnelOffsets) / sizeof(size_t), kDandenong));
    //                 break;
    //             case 5:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(flemingtonOffsets, sizeof(flemingtonOffsets) / sizeof(size_t), kFlemington));
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(northernOffsets, sizeof(northernOffsets) / sizeof(size_t), kNorthern));
    //                 break;
    //             case 6:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(sandringhamOffsets, sizeof(sandringhamOffsets) / sizeof(size_t), kSandringham));
    //                 break;
    //             case 7:
    //                 ESP_ERROR_CHECK(LEDMatrix::setMulti(vlineOffsets, sizeof(vlineOffsets) / sizeof(size_t), kVLine));
    //                 break;
    //             default:
    //                 ESP_LOGE(kTag, "invalid i=%d", i);
    //                 abort();
    //                 break;
    //         }

    //         ESP_ERROR_CHECK(LEDMatrix::update());
    //         vTaskDelay(1000 / portTICK_PERIOD_MS);
    //     }
    // }

    /* blinky blinky testy blinky */
    while (true) {
        // ESP_ERROR_CHECK(LEDMatrix::fill(kOff)); // clear
        // Services::acquire();
        ESP_ERROR_CHECK(LEDMatrix::fill(kOff)); // clear
        time_t now; time(&now);
        Services::updateStates(now);
        Services::showAllStates(now);
        // Services::release();
        LEDMatrix::update();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}