#include "subsystems/led_matrix.h"

#include <string.h>

#include "esp_check.h"

/* for logging */
#include "esp_log.h"
const char* LEDMatrix::kTag = "led_matrix";

/* SPI configs */
static const spi_bus_config_t kSPI2BusConfig = {
    .mosi_io_num = SPI2_MOSI,
    .miso_io_num = SPI2_MISO,
    .sclk_io_num = SPI2_SCK,
    .data2_io_num = -1,
    .data3_io_num = -1,
    .data4_io_num = -1,
    .data5_io_num = -1,
    .data6_io_num = -1,
    .data7_io_num = -1,
    .max_transfer_sz = 216 + 2,
    
};
static const spi_bus_config_t kSPI3BusConfig = {
    .mosi_io_num = SPI3_MOSI,
    .miso_io_num = SPI3_MISO,
    .sclk_io_num = SPI3_SCK,
    .data2_io_num = -1,
    .data3_io_num = -1,
    .data4_io_num = -1,
    .data5_io_num = -1,
    .data6_io_num = -1,
    .data7_io_num = -1,
    .max_transfer_sz = 216 + 2
};

const size_t LEDMatrix::kBufferOffsets[8] = { L0_OFFSET, L1_OFFSET, L2_OFFSET, L3_OFFSET, L4_OFFSET, L5_OFFSET, L6_OFFSET, L7_OFFSET };

AW20216S* LEDMatrix::m_drivers[8];
uint8_t* LEDMatrix::m_buffer; // LED matrix frame buffer
const size_t LEDMatrix::kBufferSize = 12 * (L0_ROWS + L1_ROWS + L2_ROWS + L3_ROWS + L4_ROWS + L5_ROWS + L6_ROWS + L7_ROWS); // size of m_buffer

esp_err_t LEDMatrix::init() {
    /* allocate framebuffer */
    m_buffer = new uint8_t[kBufferSize];
    memset(m_buffer, 0, kBufferSize); // clear framebuffer here too
    ESP_LOGV(kTag, "%u byte framebuffer allocated at 0x%x", kBufferSize, (uintptr_t)m_buffer);

    /* initialise DRV_EN pin */
    ESP_RETURN_ON_ERROR(gpio_set_direction(DRV_EN, GPIO_MODE_OUTPUT), kTag, "cannot set pin %u direction", DRV_EN);
    ESP_RETURN_ON_ERROR(enableDrivers(), kTag, "cannot enable driver output");

    /* initialise SPI buses */
#if !defined(SPI3_ONLY)
    ESP_RETURN_ON_ERROR(spi_bus_initialize(
        SPI2_HOST,
        &kSPI2BusConfig,
        SPI_DMA_CH_AUTO
    ), kTag, "SPI2 bus initialisation failed");
    spi_device_handle_t spi2Handle;
    ESP_RETURN_ON_ERROR(spi_bus_add_device(
        SPI2_HOST,
        &AW20216S::kSPIDeviceConfig,
        &spi2Handle
    ), kTag, "SPI2 device add failed");
#endif

    ESP_RETURN_ON_ERROR(spi_bus_initialize(
        SPI3_HOST,
        &kSPI3BusConfig,
        SPI_DMA_CH_AUTO
    ), kTag, "SPI3 bus initialisation failed");
    spi_device_handle_t spi3Handle;
    ESP_RETURN_ON_ERROR(spi_bus_add_device(
        SPI3_HOST,
        &AW20216S::kSPIDeviceConfig,
        &spi3Handle
    ), kTag, "SPI3 device add failed");

    /* initialise LED drivers */
#if defined(SPI3_ONLY)
#define SPI_HANDLE(host)                spi3Handle
#else
#define SPI_HANDLE(host)                (((host) == SPI2_HOST) ? spi2Handle : spi3Handle) // macro to select SPI handle
#endif
    m_drivers[0] = new AW20216S("aw0", SPI_HANDLE(L0_SPI), L0_CS, &m_buffer[kBufferOffsets[0]], L0_ROWS); ESP_RETURN_ON_ERROR(m_drivers[0]->init(), kTag, "L0 init failed");
    m_drivers[1] = new AW20216S("aw1", SPI_HANDLE(L1_SPI), L1_CS, &m_buffer[kBufferOffsets[1]], L1_ROWS); ESP_RETURN_ON_ERROR(m_drivers[1]->init(), kTag, "L1 init failed");
    m_drivers[2] = new AW20216S("aw2", SPI_HANDLE(L2_SPI), L2_CS, &m_buffer[kBufferOffsets[2]], L2_ROWS); ESP_RETURN_ON_ERROR(m_drivers[2]->init(), kTag, "L2 init failed");
    m_drivers[3] = new AW20216S("aw3", SPI_HANDLE(L3_SPI), L3_CS, &m_buffer[kBufferOffsets[3]], L3_ROWS); ESP_RETURN_ON_ERROR(m_drivers[3]->init(), kTag, "L3 init failed");
    m_drivers[4] = new AW20216S("aw4", SPI_HANDLE(L4_SPI), L4_CS, &m_buffer[kBufferOffsets[4]], L4_ROWS); ESP_RETURN_ON_ERROR(m_drivers[4]->init(), kTag, "L4 init failed");
    m_drivers[5] = new AW20216S("aw5", SPI_HANDLE(L5_SPI), L5_CS, &m_buffer[kBufferOffsets[5]], L5_ROWS); ESP_RETURN_ON_ERROR(m_drivers[5]->init(), kTag, "L5 init failed");
    m_drivers[6] = new AW20216S("aw6", SPI_HANDLE(L6_SPI), L6_CS, &m_buffer[kBufferOffsets[6]], L6_ROWS); ESP_RETURN_ON_ERROR(m_drivers[6]->init(), kTag, "L6 init failed");
    m_drivers[7] = new AW20216S("aw7", SPI_HANDLE(L7_SPI), L7_CS, &m_buffer[kBufferOffsets[7]], L7_ROWS); ESP_RETURN_ON_ERROR(m_drivers[7]->init(), kTag, "L7 init failed");

    return ESP_OK;
}

esp_err_t LEDMatrix::set(size_t offset, colour_t colour) {
    if (offset == LMAT_NULL) return ESP_OK;
    
    if (offset % 3 != 0) {
        ESP_LOGE(kTag, "offset %u given to LEDMatrix::set() is invalid", offset);
        return ESP_ERR_INVALID_ARG;
    }

    m_buffer[LMAT_R(offset)] = (colour >> 16) & 0xFF; // red
    m_buffer[LMAT_G(offset)] = (colour >> 8) & 0xFF; // green
    m_buffer[LMAT_B(offset)] = (colour >> 0) & 0xFF; // blue

    return ESP_OK;
}

esp_err_t LEDMatrix::setMulti(const size_t* offsets, size_t leds, colour_t colour) {
    uint8_t r = (colour >> 16) & 0xFF, // red
            g = (colour >> 8) & 0xFF, // green
            b = (colour >> 0) & 0xFF; // blue
    
    for (size_t i = 0; i < leds; i++, offsets++) { // increment to next offset after each iteration
        size_t offset = *offsets;
        if (offset == LMAT_NULL) continue;
        if (offset % 3 != 0) {
            ESP_LOGE(kTag, "offset %u given to LEDMatrix::setMulti() is invalid", offset);
            return ESP_ERR_INVALID_ARG;
        }
        m_buffer[LMAT_R(offset)] = r;
        m_buffer[LMAT_G(offset)] = g;
        m_buffer[LMAT_B(offset)] = b;
    }

    return ESP_OK;
}

esp_err_t LEDMatrix::fill(colour_t colour) {
    uint8_t r = (colour >> 16) & 0xFF, // red
            g = (colour >> 8) & 0xFF, // green
            b = (colour >> 0) & 0xFF; // blue
            
    if (r == g && g == b) memset(m_buffer, r, kBufferSize); // use memset if possible because it's faster
    else {
        for (size_t offset = 0; offset < kBufferSize; offset += 3) {
            m_buffer[LMAT_R(offset)] = r;
            m_buffer[LMAT_G(offset)] = g;
            m_buffer[LMAT_B(offset)] = b;
        }
    }

    return ESP_OK;
}

esp_err_t LEDMatrix::update() {
    /* TODO: more intelligent updating */
    for (int i = 0; i < 8; i++) {
        ESP_RETURN_ON_ERROR(m_drivers[i]->update(), kTag, "updating L%d failed", i);
    }

    return ESP_OK;
}