#pragma once

#include "esp_err.h"

#include "u8g2_esp32_hal.h"

#define OLED_WIDTH                          128
#define OLED_HEIGHT                         64

class OLED {
public:
    static esp_err_t init();

    static esp_err_t clear();
    static esp_err_t update();

    static esp_err_t drawString(uint8_t x, uint8_t y, const char* str, bool large = false);
    static esp_err_t drawCenteredString(uint8_t y, uint8_t width, const char* str, bool large = false);
    static esp_err_t drawCenteredString(uint8_t y, const char* str, bool large = false);

    static esp_err_t setSleep(bool state);

    static uint8_t getFontWidth();
    static uint8_t getFontHeight();
    static uint16_t getStringWidth(const char* str);
private:
    static const char* kTag;
    static u8g2_t m_u8g2;
    static bool m_initialised;

    static uint8_t m_fontWidth;
    static uint8_t m_fontHeight;
};