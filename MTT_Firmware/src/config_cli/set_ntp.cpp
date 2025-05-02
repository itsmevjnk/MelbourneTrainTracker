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
    NVSHandle handle = NVS::open("addr", NVS_READWRITE);
    if (handle.isClosed()) {
        printf("Cannot open handle!\r\n");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("ntp", m_timeServer), kTag, "cannot write time server address");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}