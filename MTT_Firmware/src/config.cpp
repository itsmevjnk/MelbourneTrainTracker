#include "config.h"

#include "subsystems/fs.h"
#include "subsystems/nvs.h"

#include "esp_check.h"

const char* Config::kTag = "config";

esp_err_t Config::init() {
    m_initialised = false; // in case we fail
    
    ESP_RETURN_ON_ERROR(NVS::init(), kTag, "NVS initialisation failed, cannot continue");
    
    /* load Wi-Fi config */
    NVSHandle handle = NVS::open("wifi", NVS_READONLY);
    if (handle.isClosed()) {
        ESP_LOGE(kTag, "cannot open Wi-Fi configuration from NVS");
        return ESP_ERR_INVALID_STATE;
    }
    ESP_RETURN_ON_ERROR(handle.getString("ssid", m_wifiSSID, sizeof(m_wifiSSID)), kTag, "cannot read Wi-Fi SSID");
    ESP_RETURN_ON_ERROR(handle.getString("pw", m_wifiPassword, sizeof(m_wifiPassword)), kTag, "cannot read Wi-Fi password");
    uint8_t enterprise;
    ESP_RETURN_ON_ERROR(handle.getU8("ent", &enterprise), kTag, "cannot read WPA2-Enterprise flag");
    m_wifiEnterprise = (enterprise != 0);
    if (m_wifiEnterprise) {
        ESP_RETURN_ON_ERROR(handle.getString("id", m_wifiIdentity, sizeof(m_wifiIdentity)), kTag, "cannot read Wi-Fi EAP identity");
        ESP_RETURN_ON_ERROR(handle.getString("user", m_wifiUsername, sizeof(m_wifiUsername)), kTag, "cannot read Wi-Fi EAP username");
        ESP_RETURN_ON_ERROR(handle.getBlobLength("cert", &m_wifiCertLength), kTag, "cannot get Wi-Fi server certificate length");
        if (m_wifiCertLength > 0) {
            m_wifiCert = new char[m_wifiCertLength];
            ESP_RETURN_ON_ERROR(handle.getBlob("cert", m_wifiCert, m_wifiCertLength), kTag, "cannot read Wi-Fi server certificate");
        }
    }
    handle.close(); // while it will be closed on destruction, it's best if we do that by ourselves

    /* success */
    ESP_LOGI(kTag, "configuration has been read successfully");
    m_initialised = true;
    return ESP_OK;
}

bool Config::m_initialised = false;
bool Config::isInitialised() {
    return m_initialised;
}

bool Config::m_wifiEnterprise = false;
bool Config::isWiFiEnterprise() {
    verifyInit();
    return m_wifiEnterprise;
}

char Config::m_wifiSSID[32] = "";
const char* Config::getWiFiSSID() {
    verifyInit();
    return m_wifiSSID;
}

char Config::m_wifiUsername[64] = "";
const char* Config::getWiFiUsername() {
    verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiUsername;
}

char Config::m_wifiPassword[64] = "";
const char* Config::getWiFiPassword() {
    verifyInit();
    return m_wifiPassword;
}

char Config::m_wifiIdentity[64] = "";
const char* Config::getWiFiIdentity() {
    verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiIdentity;
}

char* Config::m_wifiCert = nullptr;
const char* Config::getWiFiCertificate() {
    verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiCert;
}

size_t Config::m_wifiCertLength = 0;
size_t Config::getWiFiCertLength() {
    verifyInit();
    return (!m_wifiEnterprise) ? 0 : m_wifiCertLength;
}
