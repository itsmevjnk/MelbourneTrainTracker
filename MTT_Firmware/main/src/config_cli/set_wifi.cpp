#include "config.h"

#include "subsystems/nvs.h"
#include "esp_check.h"

#include <string.h>
#include <stdio.h>

esp_console_cmd_t Config::kSetWiFiCommand = {
    .command = "set_wifi",
    .help = "configure Wi-Fi credentials",
    .hint = NULL,
    .func = &cliSetWiFi
};

#define WIFI_CERT_SIZE                              8192

esp_err_t Config::cliSetWiFi(int argc, char** argv) {
    printf("Enter the SSID (max %u characters): ", sizeof(m_wifiSSID) - 1); readString(m_wifiSSID, sizeof(m_wifiSSID));
    printf("Is this a WPA2-Enterprise network? (y/n) "); m_wifiEnterprise = readYN();
    if (m_wifiEnterprise) {
        printf("Enter the EAP identity (max %u characters): ", sizeof(m_wifiIdentity) - 1); readString(m_wifiIdentity, sizeof(m_wifiIdentity));
        printf("Enter the EAP username (max %u characters): ", sizeof(m_wifiUsername) - 1); readString(m_wifiUsername, sizeof(m_wifiUsername));
        printf("Enter the EAP password (max %u characters): ", sizeof(m_wifiPassword) - 1); readString(m_wifiPassword, sizeof(m_wifiPassword));
        printf("Configure server certificate? (y/n) "); bool cert = readYN();
        if (cert) {
            size_t maxsize = WIFI_CERT_SIZE;

            if (m_wifiCert) {
                char* newCert = (char*) realloc(m_wifiCert, WIFI_CERT_SIZE);
                if (!newCert) {
                    printf("Cannot allocate memory for Wi-Fi certificate - maximum permitted size is limited to current buffer size.\r\n");
                    maxsize = m_wifiCertLength;
                }
                else m_wifiCert = newCert;
            } else {
                m_wifiCert = (char*) malloc(WIFI_CERT_SIZE);
                if (!m_wifiCert) {
                    printf("Cannot allocate memory for Wi-Fi certificate - skipping certificate configuration.\r\n");
                    m_wifiCertLength = 0;
                    goto write;
                }
            }

            m_wifiCertLength = 0;
            printf("Enter the certificate PEM contents below.\r\n");

            bool end = false;
            while (m_wifiCertLength < maxsize) {
                char* line = &m_wifiCert[m_wifiCertLength];
                putchar('>'); size_t lineLength = readString(line, maxsize - m_wifiCertLength - 2); // minus 2 for CRLF at the end of the line
                m_wifiCertLength += lineLength; m_wifiCert[m_wifiCertLength++] = '\r'; m_wifiCert[m_wifiCertLength++] = '\n';
                static const char* endCertLine = "-----END CERTIFICATE-----";
                if (!memcmp(line, endCertLine, strlen(endCertLine))) {
                    end = true;
                    break; // last line encountered
                }
            }

            if (!end) {
                printf("The provided certificate PEM is too long, and is thus not accepted.\r\n");
                m_wifiCertLength = 0;
                free(m_wifiCert);
                m_wifiCert = nullptr;
                goto write;
            }
        } else {
            m_wifiCertLength = 0;
            if (m_wifiCert) {
                free(m_wifiCert);
                m_wifiCert = nullptr;
            }
        }
    } else {
        printf("Enter the passphrase if applicable (max %u characters): ", sizeof(m_wifiPassword) - 1); readString(m_wifiPassword, sizeof(m_wifiPassword));
    }

write:
    printf("Writing to NVS...\r\n");
    NVSHandle handle = NVS::open("wifi", NVS_READWRITE);
    if (handle.isClosed()) {
        printf("Cannot open handle!\r\n");
        return ESP_FAIL;
    }

    ESP_RETURN_ON_ERROR(handle.setString("ssid", m_wifiSSID), kTag, "cannot write SSID");
    ESP_RETURN_ON_ERROR(handle.setString("pw", m_wifiPassword), kTag, "cannot write password");
    ESP_RETURN_ON_ERROR(handle.setU8("ent", (m_wifiEnterprise) ? 1 : 0), kTag, "cannot write WPA-Enterprise flag");
    if (m_wifiEnterprise) {
        ESP_RETURN_ON_ERROR(handle.setString("id", m_wifiIdentity), kTag, "cannot write EAP identity");
        ESP_RETURN_ON_ERROR(handle.setString("user", m_wifiUsername), kTag, "cannot write EAP username");
        ESP_RETURN_ON_ERROR(handle.setBlob("cert", m_wifiCert, m_wifiCertLength), kTag, "cannot write server certificate");
    }

    printf("Configuration has been written to NVS.\r\n");
    return ESP_OK;
}

esp_err_t Config::setWiFiCredentials(const char* ssid) {
    m_wifiPassword[0] = '\0';
    m_wifiEnterprise = false;
    if (ssid != m_wifiSSID) strncpy(m_wifiSSID, ssid, sizeof(m_wifiSSID));
    
    NVSHandle handle = NVS::open("wifi", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for wifi");

    ESP_RETURN_ON_ERROR(handle.setString("ssid", m_wifiSSID), kTag, "cannot write SSID to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("pw", m_wifiPassword), kTag, "cannot write password to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("ent", 0), kTag, "cannot write WPA-Enterprise flag to NVS");
    return ESP_OK;
}

esp_err_t Config::setWiFiCredentials(const char* ssid, const char* password) {
    if (!password) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (ssid != m_wifiSSID) strncpy(m_wifiSSID, ssid, sizeof(m_wifiSSID));
    if (password != m_wifiPassword) strncpy(m_wifiPassword, password, sizeof(m_wifiPassword));
    m_wifiEnterprise = false;
    
    NVSHandle handle = NVS::open("wifi", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for wifi");

    ESP_RETURN_ON_ERROR(handle.setString("ssid", m_wifiSSID), kTag, "cannot write SSID to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("pw", m_wifiPassword), kTag, "cannot write password to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("ent", 0), kTag, "cannot write WPA-Enterprise flag to NVS");
    return ESP_OK;
}

esp_err_t Config::setWiFiCredentials(const char* ssid, const char* identity, const char* username, const char* password) {
    if (!identity || !username || !password) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }

    if (ssid != m_wifiSSID) strncpy(m_wifiSSID, ssid, sizeof(m_wifiSSID));
    if (identity != m_wifiIdentity) strncpy(m_wifiIdentity, identity, sizeof(m_wifiIdentity));
    if (identity != m_wifiUsername) strncpy(m_wifiUsername, username, sizeof(m_wifiUsername));
    if (password != m_wifiPassword) strncpy(m_wifiPassword, password, sizeof(m_wifiPassword));
    m_wifiEnterprise = true;
    
    NVSHandle handle = NVS::open("wifi", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for wifi");

    ESP_RETURN_ON_ERROR(handle.setString("ssid", m_wifiSSID), kTag, "cannot write SSID to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("ent", 1), kTag, "cannot write WPA-Enterprise flag to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("id", m_wifiIdentity), kTag, "cannot write EAP identity to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("user", m_wifiUsername), kTag, "cannot write EAP username to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("pw", m_wifiPassword), kTag, "cannot write EAP password to NVS");
    ESP_RETURN_ON_ERROR(handle.setBlob("cert", nullptr, 0), kTag, "cannot write server certificate to NVS");
    return ESP_OK;
}

esp_err_t Config::setWiFiCredentials(const char* ssid, const char* identity, const char* username, const char* password, const char* cert, size_t certLength) {
    if (!identity || !username || !password || !cert) {
        ESP_LOGE(kTag, "cannot accept null value");
        return ESP_ERR_INVALID_ARG;
    }
    if (!certLength) {
        if (m_wifiCert) {
            free(m_wifiCert);
            m_wifiCert = nullptr;
        }
    } else if (cert != m_wifiCert) {
        if (certLength > m_wifiCertLength) { // must expand    
            free(m_wifiCert);
            m_wifiCert = (char*) malloc(certLength);
            ESP_RETURN_ON_FALSE(m_wifiCert, ESP_ERR_NO_MEM, kTag, "cannot allocate Wi-Fi certificate buffer");
        }
        memmove(m_wifiCert, cert, certLength);
    }
    m_wifiCertLength = certLength;

    if (ssid != m_wifiSSID) strncpy(m_wifiSSID, ssid, sizeof(m_wifiSSID));
    if (identity != m_wifiIdentity) strncpy(m_wifiIdentity, identity, sizeof(m_wifiIdentity));
    if (identity != m_wifiUsername) strncpy(m_wifiUsername, username, sizeof(m_wifiUsername));
    if (password != m_wifiPassword) strncpy(m_wifiPassword, password, sizeof(m_wifiPassword));
    m_wifiEnterprise = true;
    
    NVSHandle handle = NVS::open("wifi", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for wifi");

    ESP_RETURN_ON_ERROR(handle.setString("ssid", m_wifiSSID), kTag, "cannot write SSID to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("ent", 1), kTag, "cannot write WPA-Enterprise flag to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("id", m_wifiIdentity), kTag, "cannot write EAP identity to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("user", m_wifiUsername), kTag, "cannot write EAP username to NVS");
    ESP_RETURN_ON_ERROR(handle.setString("pw", m_wifiPassword), kTag, "cannot write EAP password to NVS");
    ESP_RETURN_ON_ERROR(handle.setBlob("cert", m_wifiCert, m_wifiCertLength), kTag, "cannot write server certificate to NVS");

    return ESP_OK;

}