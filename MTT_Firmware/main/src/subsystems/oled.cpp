#include <subsystems/oled.h>
#include <hwconfig/pindefs.h>

#include "driver/i2c_master.h"

#include "esp_check.h"

#ifndef CONFIG_OLED_ADDRESS
#define CONFIG_OLED_ADDRESS                     0x3C
#endif

u8g2_t OLED::m_u8g2;
bool OLED::m_initialised = false;
uint8_t OLED::m_fontWidth = 0;
uint8_t OLED::m_fontHeight = 0;

const char* OLED::kTag = "oled";

esp_err_t OLED::init() {
#if CONFIG_IDF_TARGET_ESP32S3
    // /* scan I2C bus to detect device */
    // const i2c_master_bus_config_t busConfig = {
    //     .i2c_port = -1, // auto select
    //     .sda_io_num = I2C_SDA,
    //     .scl_io_num = I2C_SCL,
    //     .clk_source = I2C_CLK_SRC_DEFAULT,
    //     .glitch_ignore_cnt = 7,
    // };
    // i2c_master_bus_handle_t busHandle;
    // ESP_RETURN_ON_ERROR(i2c_new_master_bus(&busConfig, &busHandle), kTag, "cannot create I2C bus for probing");
    // esp_err_t probeResult = i2c_master_probe(busHandle, CONFIG_OLED_ADDRESS, 50);
    // ESP_RETURN_ON_ERROR(i2c_del_master_bus(busHandle), kTag, "cannot uninstall I2C bus created for probing");

    // if (probeResult != ESP_OK) {
    //     ESP_LOGE(kTag, "cannot detect OLED display on address 0x%02X", CONFIG_OLED_ADDRESS);
    //     return ESP_OK;
    // } else ESP_LOGI(kTag, "found OLED display on address 0x%02X", CONFIG_OLED_ADDRESS);

    gpio_reset_pin(I2C_SCL);
    gpio_reset_pin(I2C_SDA);

    static u8g2_esp32_hal_t hal = U8G2_ESP32_HAL_DEFAULT;
    hal.bus.i2c.scl = I2C_SCL;
    hal.bus.i2c.sda = I2C_SDA;
    u8g2_esp32_hal_init(hal);

    u8g2_Setup_ssd1315_i2c_128x64_noname_f(&m_u8g2, U8G2_R0, u8g2_esp32_i2c_byte_cb, u8g2_esp32_gpio_and_delay_cb);
    u8x8_SetI2CAddress(&m_u8g2.u8x8, CONFIG_OLED_ADDRESS << 1);

    ESP_LOGI(kTag, "initialising OLED display on address 0x%02X", CONFIG_OLED_ADDRESS);
    u8g2_InitDisplay(&m_u8g2);
    ESP_LOGI(kTag, "initialisation completed");
    m_initialised = true;
    
    u8g2_SetFont(&m_u8g2, u8g2_font_busdisplay8x5_tr);
    
    setSleep(false); // wake screen up
    clear(); // clear framebuffer
    update(); // send cleared framebuffer to screen

    m_fontHeight = u8g2_GetMaxCharHeight(&m_u8g2);
    m_fontWidth = u8g2_GetMaxCharWidth(&m_u8g2);

    return ESP_OK;
#else
    ESP_LOGW(kTag, "Rev1 board does not have I2C bus");
    return ESP_OK;
#endif
}

esp_err_t OLED::clear() {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    u8g2_ClearBuffer(&m_u8g2);
#endif
    return ESP_OK;
}

esp_err_t OLED::drawString(uint8_t x, uint8_t y, const char* str, bool large) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    if (large) u8g2_DrawStrX2(&m_u8g2, x, y, str);
    else u8g2_DrawStr(&m_u8g2, x, y, str);
#endif
    return ESP_OK;
}

esp_err_t OLED::drawCenteredString(uint8_t y, uint8_t width, const char* str, bool large) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    uint16_t textWidth = getStringWidth(str); if (large) textWidth <<= 1;
    if (textWidth > width) return ESP_ERR_INVALID_ARG; // too large to fit
    return drawString((width - textWidth) / 2, y, str, large);
#else
    return ESP_OK;
#endif
}

esp_err_t OLED::drawCenteredString(uint8_t y, const char* str, bool large) {
    return drawCenteredString(y, OLED_WIDTH, str, large);
}

esp_err_t OLED::update() {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    u8g2_SendBuffer(&m_u8g2);
#endif
    return ESP_OK;
}

esp_err_t OLED::setSleep(bool state) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    u8g2_SetPowerSave(&m_u8g2, state ? 1 : 0);
#endif
    return ESP_OK;
}

uint8_t OLED::getFontHeight() {
    return m_fontHeight;
}

uint8_t OLED::getFontWidth() {
    return m_fontWidth;
}

uint16_t OLED::getStringWidth(const char* str) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return 0;
    return u8g2_GetStrWidth(&m_u8g2, str);
#else
    return 0;
#endif
}

esp_err_t OLED::setInverted(bool invert) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    u8g2_SetDrawColor(&m_u8g2, invert ? 0 : 1);
#endif
    return ESP_OK;
}

esp_err_t OLED::drawBox(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
#if CONFIG_IDF_TARGET_ESP32S3
    if (!m_initialised) return ESP_ERR_INVALID_STATE;
    u8g2_DrawBox(&m_u8g2, x, y, width, height);
#endif
    return ESP_OK;
}

#define QRCODE_MAX_VERSION                      3

typedef union {
    struct {
        uint8_t x;
        uint8_t y;
        uint8_t maxSize;
        uint8_t ones; // set to 0xFF to indicate validity; another alternative for passing params is to store it in *sizeOut (whose pointer is definitely not 0xFFxxxxxx)
    } __attribute__((packed)) params;
    uint32_t u32;
    uint32_t* u32Pointer;
    void* ptr;
} qrcode_user_data_t;

void OLED::displayQRCode(esp_qrcode_handle_t qrcode, void* userData) {
    qrcode_user_data_t decodedData = { .ptr = userData };
    uint32_t* sizeOut = nullptr;
    if (decodedData.params.ones != 0xFF) { // sizeOut pointer passed in
        sizeOut = decodedData.u32Pointer;
        decodedData.u32 = *decodedData.u32Pointer;
        *sizeOut = 0; // reset
    }
    assert(decodedData.params.ones == 0xFF);

    int size = esp_qrcode_get_size(qrcode);
    if (decodedData.params.y + size > OLED_HEIGHT || decodedData.params.x + size > OLED_WIDTH || size > decodedData.params.maxSize) return; // too big to draw
    if (sizeOut != nullptr) *sizeOut = size;

    uint8_t originalColour = u8g2_GetDrawColor(&m_u8g2);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            u8g2_SetDrawColor(&m_u8g2, esp_qrcode_get_module(qrcode, x, y) ? 0 : 1);
            u8g2_DrawPixel(&m_u8g2, decodedData.params.x + x, decodedData.params.y + y);
        }
    }
    u8g2_SetDrawColor(&m_u8g2, originalColour);
}

esp_err_t OLED::drawQRCode(uint8_t x, uint8_t y, const char* payload, uint32_t* sizeOut, uint8_t maxSize) {
    qrcode_user_data_t userData = {
        .params = {
            .x = x,
            .y = y,
            .maxSize = maxSize,
            .ones = 0xFF
        }
    };
    if (sizeOut != nullptr) *sizeOut = userData.u32; // use *sizeOut to store params

    esp_qrcode_config_t config = {
        .display_func_with_cb = &OLED::displayQRCode,
        .max_qrcode_version = QRCODE_MAX_VERSION,
        .qrcode_ecc_level = ESP_QRCODE_ECC_LOW,
        .user_data = (sizeOut != nullptr) ? sizeOut : userData.ptr
    };
    return esp_qrcode_generate(&config, payload);
}

esp_err_t OLED::drawQRCode(uint8_t x, uint8_t y, const char* payload, uint8_t maxSize) {
    return drawQRCode(x, y, payload, nullptr, maxSize);
}