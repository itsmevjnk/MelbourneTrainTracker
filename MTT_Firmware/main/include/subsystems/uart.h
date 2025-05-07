/* UART0 (mostly init for now since we can use VFS for much of our things) */

#pragma once

#include "esp_err.h"
#include <driver/uart.h>

class UART {
public:
    static esp_err_t init(); // initialise UART and set VFS to use UART driver
    static esp_err_t deinit(); // de-initialise UART

    static bool isInitialised();
    
    /* wrappers */
    static inline size_t read(uint8_t* buf, size_t length, TickType_t waitTicks = portMAX_DELAY) {
        return uart_read_bytes(UART_NUM_0, buf, length, waitTicks);
    }

    static inline size_t write(const uint8_t* buf, size_t length) {
        return uart_write_bytes(UART_NUM_0, buf, length);
    }

private:
    static const char* kTag;

    static bool m_initialised;
};