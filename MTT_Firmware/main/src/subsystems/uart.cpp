#include "subsystems/uart.h"

#include "esp_vfs_dev.h"

#include "esp_check.h"
const char* UART::kTag = "uart";

#if CONFIG_IDF_TARGET_ESP32S3
#include <driver/usb_serial_jtag_vfs.h>
#endif

bool UART::m_initialised = false;
bool UART::isInitialised() {
    return m_initialised;
}

esp_err_t UART::init() {
    if (m_initialised) {
        ESP_LOGW(kTag, "re-initialising UART driver");
        ESP_RETURN_ON_ERROR(deinit(), kTag, "cannot de-initialise UART");
    }

#if CONFIG_IDF_TARGET_ESP32S3
    usb_serial_jtag_driver_config_t config = USB_SERIAL_JTAG_DRIVER_CONFIG_DEFAULT();
    ESP_RETURN_ON_ERROR(
        usb_serial_jtag_driver_install(&config),
        kTag, "cannot install UART driver"
    );
    usb_serial_jtag_vfs_use_driver();
#else
    ESP_RETURN_ON_ERROR(
        uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0), // 1 KiB receive buffer, unbuffered transmit
        kTag, "cannot install UART driver"
    );
    esp_vfs_dev_uart_use_driver(0);
#endif

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
#if CONFIG_IDF_TARGET_ESP32S3
    usb_serial_jtag_vfs_use_nonblocking();
    ESP_RETURN_ON_ERROR(usb_serial_jtag_driver_uninstall(), kTag, "cannot delete UART driver");
#else
    esp_vfs_dev_uart_use_nonblocking(0); // set VFS back to using non-blocking implementation
    ESP_RETURN_ON_ERROR(uart_driver_delete(UART_NUM_0), kTag, "cannot delete UART driver");
#endif

    ESP_LOGI(kTag, "UART driver uninstalled");
    return ESP_OK;
}
