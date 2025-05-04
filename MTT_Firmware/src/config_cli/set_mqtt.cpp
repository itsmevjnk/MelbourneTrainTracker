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
    NVSHandle handle = NVS::open("addr", NVS_READWRITE);
    if (handle.isClosed()) {
        printf("Cannot open handle!\r\n");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("mqtt", m_mqttBroker), kTag, "cannot write MQTT bokrer address");

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}