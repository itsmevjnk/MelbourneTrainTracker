#include "subsystems/aw20216s.h"
#include "subsystems/aw20216s_regs.h"

#include "esp_private/spi_common_internal.h"
#include "esp_log.h"
#include "esp_check.h"

#include "hwconfig/led_matrix.h"

#include <string.h>

void AW20216S::preTransactionCallback(spi_transaction_t* trans) {
    gpio_num_t csPin = *(gpio_num_t*)trans->user;
    ESP_LOGV("aw20216s", "asserting CS pin %d", csPin);
    ESP_ERROR_CHECK(gpio_set_level(csPin, 0));
}

void AW20216S::postTransactionCallback(spi_transaction_t* trans) {
    gpio_num_t csPin = *(gpio_num_t*)trans->user;
    ESP_LOGV("aw20216s", "deasserting CS pin %d", csPin);
    ESP_ERROR_CHECK(gpio_set_level(csPin, 1));
}

const spi_device_interface_config_t AW20216S::kSPIDeviceConfig = {
    .command_bits = 0, // no command bits
    .address_bits = 16, // we'll use the address field for the device ID + page + register + R/W bit
    .mode = 3, // AW20216S uses SPI mode 3
    .clock_speed_hz = LMAT_SPI_CLOCK,
    .spics_io_num = -1, // no CS pin
    .queue_size = 7, // number of transactions to queue at a time (taken from https://github.com/espressif/esp-idf/blob/master/examples/peripherals/spi_master/lcd/main/spi_master_example_main.c) - might need tuning
    .pre_cb = AW20216S::preTransactionCallback,
    .post_cb = AW20216S::postTransactionCallback
    // the AW20216S uses MSB first for SPI
};

AW20216S::AW20216S(const char* tag, spi_device_handle_t spiHandle, gpio_num_t csPin, const uint8_t* buffer, uint8_t rows) 
    : m_tag(tag), m_spi(spiHandle), m_csPin(csPin), m_rows(rows), m_buffer(buffer) {
    assert(buffer != nullptr);
}

esp_err_t AW20216S::read(uint16_t address, size_t count, uint8_t* buf) {
    spi_transaction_t trans; // SPI transaction
    memset(&trans, 0, sizeof(trans)); // and zero out the transaction object
    trans.length = count << 3; // length is in bits
    trans.addr = 0xA000 | (1 << 8) | ((address & ~0xFF) << 1) | (address & 0xFF);
    trans.rx_buffer = buf; // specify receive buffer
    trans.user = &m_csPin; // supply CS pin number to transaction callbacks - NOTE: this is a very unsafe way to do it!
    esp_err_t ret = spi_device_polling_transmit(m_spi, &trans); // if we get an error, we'll still have to deassert CS anyway
    if (ret != ESP_OK) ESP_LOGE(m_tag, "SPI read transaction failed");
    return ret;
}

esp_err_t AW20216S::write(uint16_t address, size_t count, const uint8_t* buf) {
    spi_transaction_t trans; // SPI transaction
    memset(&trans, 0, sizeof(trans)); // and zero out the transaction object
    trans.length = count << 3; // length is in bits
    trans.addr = 0xA000 | ((address & ~0xFF) << 1) | (address & 0xFF);
    trans.tx_buffer = buf; // specify transmit buffer
    trans.user = &m_csPin; // supply CS pin number to transaction callbacks
    esp_err_t ret = spi_device_polling_transmit(m_spi, &trans); // if we get an error, we'll still have to deassert CS anyway
    if (ret != ESP_OK) ESP_LOGE(m_tag, "SPI read transaction failed");
    return ret;
}

esp_err_t AW20216S::init() {
    ESP_LOGI(m_tag, "%u rows, buffer at 0x%x", m_rows, (uintptr_t)m_buffer);

    /* initialise CS pin */
    ESP_RETURN_ON_ERROR(gpio_set_direction(m_csPin, GPIO_MODE_OUTPUT), m_tag, "cannot set pin %u direction", m_csPin);
    ESP_RETURN_ON_ERROR(gpio_set_level(m_csPin, 1), m_tag, "cannot set pin %u to high", m_csPin);
    vTaskDelay(1 / portTICK_PERIOD_MS); // give CS pin time to settle
    
    /* software reset */
    writeSingle(AW20216S_REG_RSTN, 0xAE); // will crash the system on failure (which is what we want)
    vTaskDelay(3 / portTICK_PERIOD_MS); // docs said we need to wait for at least 2 msec, but let's wait for 3
    
    /* configure chip - adapted from QMK AW20216S driver */
    writeSingle(AW20216S_REG_GCCR, 150); // set global maximum current
    uint8_t scaling[216]; memset(scaling, 150, sizeof(scaling)); ESP_RETURN_ON_ERROR(write(AW20216S_REG_SL(0), sizeof(scaling), scaling), m_tag, "cannot initialise SLx"); // set scaling regs
    writeSingle(AW20216S_REG_GCR, 0b10110001); // SW1-12 active, OSDE=0, CHIPEN=1 - enable chip
    writeSingle(AW20216S_REG_MIXCR, (1 << 1)); // enable auto low-power

    m_initialised = true;
    return ESP_OK;
}

esp_err_t AW20216S::update() {
    if (m_rows == 18) {
        /* updating the entire buffer in one go is possible */
        ESP_RETURN_ON_ERROR(
            write(AW20216S_REG_PWM(0), 216, m_buffer),
            m_tag, "complete buffer update failed"
        );
    } else {
        /* update one column at a time */
        for (int col = 0; col < 12; col++) {
            ESP_RETURN_ON_ERROR(
                write(AW20216S_REG_PWM(18 * col), m_rows, &m_buffer[m_rows * col]),
                m_tag, "updating buffer column %d failed", col
            );
        }
    }

    return ESP_OK;
}