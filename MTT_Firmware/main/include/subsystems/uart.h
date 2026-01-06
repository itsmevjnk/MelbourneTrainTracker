/* UART0 (mostly init for now since we can use VFS for much of our things) */

#pragma once

#include "esp_err.h"
#include <driver/uart.h>

#if CONFIG_IDF_TARGET_ESP32S3
#include <driver/usb_serial_jtag.h>
#else
#include <driver/uart.h>
#endif

class UART {
public:
    static esp_err_t init(); // initialise UART and set VFS to use UART driver
    static esp_err_t deinit(); // de-initialise UART

    static bool isInitialised();
    
    /* wrappers */
    static inline size_t read(uint8_t* buf, size_t length, TickType_t waitTicks = portMAX_DELAY) {
#if CONFIG_IDF_TARGET_ESP32S3
        return usb_serial_jtag_read_bytes(buf, length, waitTicks);
#else
        return uart_read_bytes(UART_NUM_0, buf, length, waitTicks);
#endif
    }

    static inline size_t write(const uint8_t* buf, size_t length) {
#if CONFIG_IDF_TARGET_ESP32S3
        /* simulate blocking behaviour */
        size_t totalWritten = 0;
        while (length > 0) {
            int written = usb_serial_jtag_write_bytes(&buf[totalWritten], length, portMAX_DELAY);
            length -= written; totalWritten += written;
        }
        return totalWritten;
#else
        return uart_write_bytes(UART_NUM_0, buf, length);
#endif
    }

private:
    static const char* kTag;

    static bool m_initialised;
};