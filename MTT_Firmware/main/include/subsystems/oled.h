#pragma once

#include "esp_err.h"

#include "u8g2_esp32_hal.h"
#include "qrcode.h"

#define OLED_WIDTH                          128
#define OLED_HEIGHT                         64

class OLED {
public:
    static esp_err_t init();

    static esp_err_t clear();
    static esp_err_t update();

    static esp_err_t setInverted(bool invert);
    static esp_err_t drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    static esp_err_t drawString(uint8_t x, uint8_t y, const char* str, bool large = false);
    static esp_err_t drawCenteredString(uint8_t y, uint8_t width, const char* str, bool large = false);
    static esp_err_t drawCenteredString(uint8_t y, const char* str, bool large = false);

    static esp_err_t setSleep(bool state);
    static esp_err_t setBrightness(uint8_t percent);

    static uint8_t getFontWidth();
    static uint8_t getFontHeight();
    static uint16_t getStringWidth(const char* str);

    static esp_err_t drawQRCode(uint8_t x, uint8_t y, const char* payload, uint32_t* sizeOut, uint8_t maxSize = OLED_HEIGHT);
    static esp_err_t drawQRCode(uint8_t x, uint8_t y, const char* payload, uint8_t maxSize = OLED_HEIGHT);

    static esp_err_t initClockTask();
private:
    static const char* kTag;
    static u8g2_t m_u8g2;
    static bool m_initialised;

    static uint8_t m_fontWidth;
    static uint8_t m_fontHeight;

    static void displayQRCode(esp_qrcode_handle_t qrcode, void* userData); // callback for QR code drawing
    static void IRAM_ATTR buttonHandler(void* arg);
    static TaskHandle_t m_clockTask;
    static void clockTask(void* pvParameters);
};