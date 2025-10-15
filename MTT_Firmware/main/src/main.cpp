/* subsystems */
#include "subsystems/status_led.h"
#include "subsystems/led_matrix.h"
#include "subsystems/wifi.h"
#include "subsystems/uart.h"
#include "subsystems/ntp.h"
#include "subsystems/mqtt.h"
#include "subsystems/webserver.h"
#include "subsystems/gtfsr.h"

#include "message.h"
#include "services.h"

#include "config.h"

#include "esp_log.h"
static const char* kTag = "main"; // for logging

/* update LED states */
void update() {
#ifdef CONFIG_UPDATE_FLASH_LED
    StatusLED::actyOn();
#endif
    Services::acquire();
    time_t now; time(&now);
    if (Services::updateStates(now)) { // update available
        ESP_ERROR_CHECK(LEDMatrix::fill(kOff)); // clear
        LEDMatrix::acquireBuffer();
        Services::showAllStates(now);
        LEDMatrix::update();
        LEDMatrix::releaseBuffer();
        ESP_ERROR_CHECK(WebServer::sendLEDBufferAsync());
        ESP_LOGI(kTag, "updated LED matrix, minimum free heap size: %lu bytes", esp_get_minimum_free_heap_size()); // log to detect excessive RAM usage
    }
    Services::release();
#ifdef CONFIG_UPDATE_FLASH_LED
    StatusLED::actyOff();
#endif
}

#ifndef CONFIG_UPDATE_INTERVAL
#define CONFIG_UPDATE_INTERVAL                 1000
#endif

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

    // ESP_ERROR_CHECK(MQTT::init(Config::getMQTTBroker()));

    ESP_ERROR_CHECK(WebServer::init(Config::getMDNSHostname(), Config::getMDNSInstanceName()));

    ESP_ERROR_CHECK(GTFSR::init(Config::getAPIKey()));

    ESP_LOGI(kTag, "init end"); ESP_ERROR_CHECK(StatusLED::actyOff());

#ifdef CONFIG_SPI3_ONLY
    ESP_LOGW(kTag, "CONFIG_SPI3_ONLY is set - all LED controllers are expected to be connected to SPI3 (which is NOT the stock configuration)");
#endif

    while (true) {
        GTFSR::getMetroTripUpdates([](){});
        vTaskDelay(pdMS_TO_TICKS(30000));
    }

    // while (true) {
    //     update();
    //     vTaskDelay(CONFIG_UPDATE_INTERVAL / portTICK_PERIOD_MS);
    // }
}