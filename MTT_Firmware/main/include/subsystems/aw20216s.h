/* AW20216S LED matrix driver */

#pragma once

#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "esp_err.h"
#include "esp_check.h"

class AW20216S {
public:
    static const spi_device_interface_config_t kSPIDeviceConfig; // SPI device config to provide to SPI master driver

    /* constructor */
    AW20216S(const char* tag, spi_device_handle_t spiBus, gpio_num_t csPin, const uint8_t* buffer, uint8_t rows = 18); // note that buffer must be (12 * rows) bytes, and rows are CS lines

    esp_err_t init(); // initialise the LED driver. note that m_buffer must be non-null
    esp_err_t update(); // write buffer to the chip

private:
    /* SPI transaction callbacks */
    static void preTransactionCallback(spi_transaction_t* trans);
    static void postTransactionCallback(spi_transaction_t* trans);

    /* low-level access functions */
    esp_err_t read(uint16_t address, size_t count, uint8_t* buf); // address is 8 bit register + 3 bit page
    esp_err_t write(uint16_t address, size_t count, const uint8_t* buf);

    inline uint8_t readSingle(uint16_t address) {
        uint8_t ret;
        ESP_ERROR_CHECK(read(address, 1, &ret));
        return ret;
    }

    inline void writeSingle(uint16_t address, uint8_t data) {
        ESP_ERROR_CHECK(write(address, 1, &data));
    }

    bool m_initialised = false;

    const char* m_tag; // log tag
    spi_device_handle_t m_spi; // SPI device handle to use
    gpio_num_t m_csPin; // CS pin
    uint8_t m_rows; // number of CS lines this driver has
    const uint8_t* m_buffer; // framebuffer pointer
};