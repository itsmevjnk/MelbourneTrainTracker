#include "config.h"

#include "subsystems/nvs.h"
#include "esp_check.h"

#include <string.h>
#include <stdio.h>

esp_console_cmd_t Config::kSetMQTTCommand = {
    .command = "set_mqtt",
    .help = "configure MQTT broker",
    .hint = NULL,
    .func = &cliSetMQTT
};

esp_err_t Config::cliSetMQTT(int argc, char** argv) {
    printf("Enter the MQTT broker address, including the protocol schema (max %u characters): ", sizeof(m_mqttBroker) - 1); readString(m_mqttBroker, sizeof(m_mqttBroker));
    
    printf("Writing to NVS...\r\n");

    ESP_RETURN_ON_ERROR(setTimeServer(m_mqttBroker), kTag, "NVS writing failed");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}

esp_err_t Config::setMQTTBroker(const char* value) {
    if (!value) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (value != m_mqttBroker) strncpy(m_mqttBroker, value, sizeof(m_mqttBroker) - 1);

    NVSHandle handle = NVS::open("addr", NVS_READWRITE);
    if (handle.isClosed()) {
        ESP_LOGE(kTag, "cannot open NVS handle for addr");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("mqtt", m_mqttBroker), kTag, "cannot write MQTT broker address");
    return ESP_OK;
}