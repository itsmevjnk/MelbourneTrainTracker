#include "config.h"

#include <stdio.h>

esp_console_cmd_t Config::kShowCertificateCommand = {
    .command = "show_cert",
    .help = "show WPA2-Enterprise RADIUS server certificate",
    .hint = NULL,
    .func = &cliShowCertificate
};

esp_err_t Config::cliShowCertificate(int argc, char** argv) {
    if (!m_initialised) {
        printf("Configuration has not been loaded\r\n");
        return ESP_OK;
    }

    if (!m_wifiCertLength) {
        printf("RADIUS server certificate has not been set\r\n");
        return ESP_OK;
    }

    printf("RADIUS server certificate:\r\n");
    fwrite(m_wifiCert, 1, m_wifiCertLength, stdout);
    return ESP_OK;
}