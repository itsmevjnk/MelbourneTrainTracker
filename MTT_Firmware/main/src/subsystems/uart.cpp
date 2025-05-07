#include "subsystems/uart.h"

#include "esp_vfs_dev.h"

#include "esp_check.h"
const char* UART::kTag = "uart";

bool UART::m_initialised = false;
bool UART::isInitialised() {
    return m_initialised;
}

esp_err_t UART::init() {
    if (m_initialised) {
        ESP_LOGW(kTag, "re-initialising UART driver");
        ESP_RETURN_ON_ERROR(deinit(), kTag, "cannot de-initialise UART");
    }

    ESP_RETURN_ON_ERROR(
        uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0), // 1 KiB receive buffer, unbuffered transmit
        kTag, "cannot install UART driver"
    );
    esp_vfs_dev_uart_use_driver(0);

    m_initialised = true;
    ESP_LOGI(kTag, "UART driver installed");
    return ESP_OK;
}

esp_err_t UART::deinit() {
    if (!m_initialised) {
        ESP_LOGW(kTag, "UART driver has not been initialised");
        return ESP_OK;
    }
    
    m_initialised = false;
    esp_vfs_dev_uart_use_nonblocking(0); // set VFS back to using non-blocking implementation
    ESP_RETURN_ON_ERROR(uart_driver_delete(UART_NUM_0), kTag, "cannot delete UART driver");
    ESP_LOGI(kTag, "UART driver uninstalled");
    return ESP_OK;
}
