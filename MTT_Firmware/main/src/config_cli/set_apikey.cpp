#include "config.h"

#include "subsystems/nvs.h"
#include "esp_check.h"

#include <string.h>
#include <stdio.h>

esp_console_cmd_t Config::kSetAPIKeyCommand = {
    .command = "set_apikey",
    .help = "configure GTFS Realtime API key",
    .hint = NULL,
    .func = &cliSetAPIKey
};

esp_err_t Config::cliSetAPIKey(int argc, char** argv) {
    printf("To obtain the GTFS Realtime API key, register an account on https://opendata.transport.vic.gov.au, then go to Profile -> API Tokens and copy the \"API Key\" subscription key.\n");
    printf("Enter the GTFS Realtime API key (max %u characters): ", sizeof(m_apiKey) - 1); readString(m_apiKey, sizeof(m_apiKey));
    
    printf("Writing to NVS...\r\n");
    NVSHandle handle = NVS::open("api", NVS_READWRITE);
    if (handle.isClosed()) {
        printf("Cannot open handle!\r\n");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("key", m_apiKey), kTag, "cannot write GTFS-R API key");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}