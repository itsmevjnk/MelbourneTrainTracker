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
        ESP_LOGE(kTag, "cannot open NVS handle for mdns");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("host", m_mdnsHostname), kTag, "cannot write mDNS hostname");
    ESP_RETURN_ON_ERROR(handle.setString("inst", m_mdnsInstanceName), kTag, "cannot write mDNS instance name");
    // NOTE: we do this since we're writing both mDNS fields anyway

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}

esp_err_t Config::setMDNSHostname(const char* value) {
    if (!value) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (value != m_mdnsHostname) strncpy(m_mdnsHostname, value, sizeof(m_mdnsHostname) - 1);

    NVSHandle handle = NVS::open("mdns", NVS_READWRITE);
    if (handle.isClosed()) {
        ESP_LOGE(kTag, "cannot open NVS handle for mdns");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("host", m_mdnsHostname), kTag, "cannot write mDNS hostname");
    return ESP_OK;
}

esp_err_t Config::setMDNSInstanceName(const char* value) {
    if (!value) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (value != m_mdnsInstanceName) strncpy(m_mdnsInstanceName, value, sizeof(m_mdnsInstanceName) - 1);

    NVSHandle handle = NVS::open("mdns", NVS_READWRITE);
    if (handle.isClosed()) {
        ESP_LOGE(kTag, "cannot open NVS handle for mdns");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("inst", m_mdnsInstanceName), kTag, "cannot write mDNS instance name");
    return ESP_OK;
}