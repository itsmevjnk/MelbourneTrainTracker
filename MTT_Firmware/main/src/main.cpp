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
#include "subsystems/brightness.h"

#include "driver/gpio.h"

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
    LEDMatrix::acquireBuffer();
    Services::showAllStates(now);
    LEDMatrix::update();
    LEDMatrix::releaseBuffer();
    ESP_ERROR_CHECK(WebServer::sendLEDBufferAsync());
    uint8_t brightness = Brightness::getCurrentBrightness();
    ESP_LOGI(kTag, "updated LED matrix (brightness: %u), minimum free heap size: %lu bytes", brightness, esp_get_minimum_free_heap_size()); // log to detect excessive RAM usage
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
    
    /* initialise boot pin */
    gpio_config_t bootIOConf = {
        .pin_bit_mask = (1ULL << BTN_BOOT),
        .mode = GPIO_MODE_INPUT,
        // .pull_up_en = GPIO_PULLUP_ENABLE,
        // .pull_down_en = GPIO_PULLDOWN_DISABLE,
        // .intr_type = GPIO_INTR_DISABLE
        // NOTE: the BOOT pin should have already been pulled up as it is a strapping pin
    };
    ESP_ERROR_CHECK(gpio_config(&bootIOConf));

#ifdef CONFIG_BATCH1_BODGE_TEST
    ESP_LOGI(kTag, "running 1st batch bodge test - press and release Boot key to continue booting");
    while (gpio_get_level(BTN_BOOT) == 1) {
        /* L2_K5 test */
        static const size_t ledsL2K5_1[] = { 
            LMAT_NEWPORT_NME, LMAT_NEWPORT_NME_ALT,
            LMAT_SEYMOUR_NME, LMAT_SEYMOUR_NME_ALT,
            LMAT_NORTHERN_NME, LMAT_NORTHERN_NME_ALT
        };
        LEDMatrix::fill(kOff); LEDMatrix::setMulti(ledsL2K5_1, sizeof(ledsL2K5_1) / sizeof(size_t), kNorthern); LEDMatrix::update();
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        static const size_t ledsL2K5_2[] = { 
            LMAT_FRANKSTON_ARM, LMAT_FRANKSTON_ARM_ALT,
            LMAT_FRANKSTON_HKN, LMAT_FRANKSTON_HKN_ALT,
            LMAT_FRANKSTON_TOR, LMAT_FRANKSTON_TOR_ALT
        };
        LEDMatrix::fill(kOff); LEDMatrix::setMulti(ledsL2K5_2, sizeof(ledsL2K5_2) / sizeof(size_t), kFrankston); LEDMatrix::update();
        vTaskDelay(pdMS_TO_TICKS(1000));

        /* L5_K4 test */
        static const size_t ledsL5K4_1[] = { 
            LMAT_SHEPPARTON_SNH, LMAT_SHEPPARTON_SNH_ALT, 
            LMAT_SHEPPARTON_MPA, LMAT_SHEPPARTON_MPA_ALT, 
            LMAT_SHEPPARTON_MST, LMAT_SHEPPARTON_MST_ALT
        };
        LEDMatrix::fill(kOff); LEDMatrix::setMulti(ledsL5K4_1, sizeof(ledsL5K4_1) / sizeof(size_t), kVLine); LEDMatrix::update();
        vTaskDelay(pdMS_TO_TICKS(1000));
        
        static const size_t ledsL5K4_2[] = { 
            LMAT_SHEPPARTON_NGE, LMAT_SHEPPARTON_NGE_ALT,
            LMAT_SEYMOUR_SER, LMAT_SEYMOUR_SER_ALT,
            LMAT_SEYMOUR_TOK, LMAT_SEYMOUR_TOK_ALT
        };
        LEDMatrix::fill(kOff); LEDMatrix::setMulti(ledsL5K4_2, sizeof(ledsL5K4_2) / sizeof(size_t), kVLine); LEDMatrix::update();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    LEDMatrix::fill(kOff); LEDMatrix::update();
    while (gpio_get_level(BTN_BOOT) == 0); // wait for release
#endif

    /* load config */
    esp_err_t ret = Config::init();
    bool runConfig = false;
    if (ret != ESP_OK) {
#ifdef CONFIG_ESP_ERR_TO_NME_LOOKUP
        ESP_LOGE(kTag, "configuration loading failed (%s), booting into configuration CLI", esp_err_to_name(ret));
#else  
        ESP_LOGE(kTag, "configuration loading failed (%d), booting into configuration CLI", ret);
#endif
        runConfig = true;
    } else {
        ESP_LOGI(kTag, "press any key or the BOOT button within 3 seconds to boot into configuration CLI");
        TickType_t startTick = xTaskGetTickCount();
        while (xTaskGetTickCount() - startTick < (3000 / portTICK_PERIOD_MS)) {
            uint8_t buf;
            if ((UART::read(&buf, 1, 0) > 0) || (gpio_get_level(BTN_BOOT) == 0)) {
                ESP_LOGI(kTag, "booting into configuration CLI");
                runConfig = true;
                break;
            }
        }
    }
    if (runConfig) {
        /* initialise web configuration interface access */
        ESP_ERROR_CHECK(WiFi::initAP());
        ESP_ERROR_CHECK(WebServer::initConfig()); // do not use mDNS settings here since Config might not be initialised yet

        ESP_ERROR_CHECK(Config::cli());
        while (true) { // while CLI is running on another task, we flash the two LEDs alternately
            ESP_ERROR_CHECK(StatusLED::actyOn()); ESP_ERROR_CHECK(StatusLED::errorOff()); vTaskDelay(500 / portTICK_PERIOD_MS);
            ESP_ERROR_CHECK(StatusLED::actyOff()); ESP_ERROR_CHECK(StatusLED::errorOn()); vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
    ESP_ERROR_CHECK(StatusLED::errorOff());

    if (Config::isWiFiEnterprise())
        ESP_ERROR_CHECK(WiFi::initSTA(Config::getWiFiSSID(), Config::getWiFiIdentity(), Config::getWiFiUsername(), Config::getWiFiPassword(), Config::getWiFiCertificate(), Config::getWiFiCertLength()));
    else
        ESP_ERROR_CHECK(WiFi::initSTA(Config::getWiFiSSID(), Config::getWiFiPassword()));

    ESP_ERROR_CHECK(OTA::confirmUpdate());

    ESP_ERROR_CHECK(NTP::init(Config::getTimeServer()));

    // ESP_ERROR_CHECK(OTA::doUpdate()); // should occur before setting up MQTT and web server
    ESP_ERROR_CHECK(OTA::init());

    ESP_ERROR_CHECK(MQTT::init(Config::getMQTTBroker()));

    ESP_ERROR_CHECK(WebServer::init(Config::getMDNSHostname(), Config::getMDNSInstanceName()));

    ESP_LOGI(kTag, "init end"); ESP_ERROR_CHECK(StatusLED::actyOff());

#ifdef CONFIG_SPI3_ONLY
    ESP_LOGW(kTag, "CONFIG_SPI3_ONLY is set - all LED controllers are expected to be connected to SPI3 (which is NOT the stock configuration)");
#endif

    while (true) {
        update();
        vTaskDelay(CONFIG_UPDATE_INTERVAL / portTICK_PERIOD_MS);
    }
}