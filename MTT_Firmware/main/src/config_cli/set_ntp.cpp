#include "config.h"

#include "subsystems/nvs.h"
#include "esp_check.h"

#include <string.h>
#include <stdio.h>

esp_console_cmd_t Config::kSetNTPCommand = {
    .command = "set_ntp",
    .help = "configure NTP time server",
    .hint = NULL,
    .func = &cliSetNTP
};

esp_err_t Config::cliSetNTP(int argc, char** argv) {
    printf("Enter the NTP time server (max %u characters): ", sizeof(m_timeServer) - 1); readString(m_timeServer, sizeof(m_timeServer));
    
    printf("Writing to NVS...\r\n");
    
    ESP_RETURN_ON_ERROR(setTimeServer(m_timeServer), kTag, "NVS writing failed");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}

esp_err_t Config::setTimeServer(const char* value) {
    if (!value) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (value != m_timeServer) strncpy(m_timeServer, value, sizeof(m_timeServer) - 1);

    NVSHandle handle = NVS::open("addr", NVS_READWRITE);
    if (handle.isClosed()) {
        ESP_LOGE(kTag, "cannot open NVS handle for addr");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("ntp", m_timeServer), kTag, "cannot write time server address");
    return ESP_OK;
}