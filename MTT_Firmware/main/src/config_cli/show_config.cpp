#include "config.h"

esp_console_cmd_t Config::kShowConfigCommand = {
    .command = "show_config",
    .help = "show current configuration",
    .hint = NULL,
    .func = &cliShowConfig
};

esp_err_t Config::cliShowConfig(int argc, char** argv) {
    if (!m_initialised) {
        printf("Configuration has not been loaded\r\n");
        return ESP_OK;
    }

    printf("Wi-Fi configuration:\r\n");
    printf(" - SSID: %s\r\n", m_wifiSSID);
    if (!m_wifiEnterprise) printf(" - Passphrase: %s\r\n", m_wifiPassword);
    else {
        printf(" - EAP identity: %s\r\n", m_wifiIdentity);
        printf(" - EAP username: %s\r\n", m_wifiUsername);
        printf(" - EAP password: %s\r\n", m_wifiPassword);
        printf(" - RADIUS server certificate: ");
        if (m_wifiCertLength) printf("available (%u bytes) - run show_cert for details\r\n", m_wifiCertLength);
        else printf("unavailable\r\n");
    }
    printf("=================\r\n\r\n");

    printf("Network address configuration:\r\n");
    printf(" - Time server: %s\r\n", m_timeServer);
    printf(" - MQTT broker: %s\r\n", m_mqttBroker);
    printf("=================\r\n\r\n");

    return ESP_OK;
}