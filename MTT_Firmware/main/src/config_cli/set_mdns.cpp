#include "config.h"

#include "subsystems/nvs.h"
#include "esp_check.h"

#include <string.h>
#include <stdio.h>

esp_console_cmd_t Config::kSetMDNSCommand = {
    .command = "set_mdns",
    .help = "configure mDNS advertisement information",
    .hint = NULL,
    .func = &cliSetMDNS
};

esp_err_t Config::cliSetMDNS(int argc, char** argv) {
    printf("Enter the mDNS hostname (max %u characters): ", sizeof(m_mdnsHostname) - 1); readString(m_mdnsHostname, sizeof(m_mdnsHostname));
    printf("Enter the mDNS instance name (max %u characters): ", sizeof(m_mdnsInstanceName) - 1); readString(m_mdnsInstanceName, sizeof(m_mdnsInstanceName));
    
    printf("Writing to NVS...\r\n");
    NVSHandle handle = NVS::open("mdns", NVS_READWRITE);
    if (handle.isClosed()) {
        printf("Cannot open handle!\r\n");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("host", m_mdnsHostname), kTag, "cannot write mDNS hostname");
    ESP_RETURN_ON_ERROR(handle.setString("inst", m_mdnsInstanceName), kTag, "cannot write mDNS instance name");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}