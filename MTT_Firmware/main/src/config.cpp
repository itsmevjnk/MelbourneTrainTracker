#include "config.h"

#include "subsystems/fs.h"
#include "subsystems/nvs.h"

#include "esp_check.h"

const char* Config::kTag = "config";

esp_err_t Config::init() {
    m_initialised = false; // in case we fail
    
    ESP_RETURN_ON_ERROR(NVS::init(), kTag, "NVS initialisation failed, cannot continue");
    
    /* load Wi-Fi config */
    NVSHandle wifiHandle = NVS::open("wifi", NVS_READONLY);
    if (wifiHandle.isClosed()) {
        ESP_LOGE(kTag, "cannot open Wi-Fi configuration from NVS");
        return ESP_ERR_INVALID_STATE;
    }
    ESP_RETURN_ON_ERROR(wifiHandle.getString("ssid", m_wifiSSID, sizeof(m_wifiSSID)), kTag, "cannot read Wi-Fi SSID");
    ESP_RETURN_ON_ERROR(wifiHandle.getString("pw", m_wifiPassword, sizeof(m_wifiPassword)), kTag, "cannot read Wi-Fi password");
    uint8_t enterprise;
    ESP_RETURN_ON_ERROR(wifiHandle.getU8("ent", &enterprise), kTag, "cannot read WPA2-Enterprise flag");
    m_wifiEnterprise = (enterprise != 0);
    if (m_wifiEnterprise) {
        ESP_RETURN_ON_ERROR(wifiHandle.getString("id", m_wifiIdentity, sizeof(m_wifiIdentity)), kTag, "cannot read Wi-Fi EAP identity");
        ESP_RETURN_ON_ERROR(wifiHandle.getString("user", m_wifiUsername, sizeof(m_wifiUsername)), kTag, "cannot read Wi-Fi EAP username");
        ESP_RETURN_ON_ERROR(wifiHandle.getBlobLength("cert", &m_wifiCertLength), kTag, "cannot get Wi-Fi server certificate length");
        if (m_wifiCertLength > 0) {
            m_wifiCert = new char[m_wifiCertLength];
            ESP_RETURN_ON_ERROR(wifiHandle.getBlob("cert", m_wifiCert, m_wifiCertLength), kTag, "cannot read Wi-Fi server certificate");
        }
    }
    // wifiHandle.close(); // while it will be closed on destruction, it's best if we do that by ourselves

    /* load address config */
    NVSHandle addrHandle = NVS::open("addr", NVS_READONLY);
    if (addrHandle.isClosed()) {
        ESP_LOGW(kTag, "cannot open address configuration from NVS - using default values");
        // TODO: maybe we'll need to make this fatal
    } else {
        esp_err_t ret = addrHandle.getString("ntp", m_timeServer, sizeof(m_timeServer));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "time server not set in NVS, defaulting to %s", m_timeServer); break;
            default: ESP_LOGE(kTag, "cannot read time server address (%s)", esp_err_to_name(ret)); return ret;
        }

        ret = addrHandle.getString("mqtt", m_mqttBroker, sizeof(m_mqttBroker));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "MQTT broker not set in NVS, defaulting to %s", m_mqttBroker); break;
            default: ESP_LOGE(kTag, "cannot read MQTT server address (%s)", esp_err_to_name(ret)); return ret;
        }

        // addrHandle.close();
    }

    /* load mDNS config */
    NVSHandle mdnsHandle = NVS::open("mdns", NVS_READONLY);
    if (mdnsHandle.isClosed()) {
        ESP_LOGW(kTag, "cannot open mDNS configuration from NVS - using default values");
        // TODO: maybe we'll need to make this fatal
    } else {
        esp_err_t ret = mdnsHandle.getString("host", m_mdnsHostname, sizeof(m_mdnsHostname));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "mDNS hostname not set in NVS, defaulting to %s", m_mdnsHostname); break;
            default: ESP_LOGE(kTag, "cannot read mDNS hostname (%s)", esp_err_to_name(ret)); return ret;
        }

        ret = mdnsHandle.getString("inst", m_mdnsInstanceName, sizeof(m_mdnsInstanceName));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "mDNS instance name not set in NVS, defaulting to %s", m_mdnsInstanceName); break;
            default: ESP_LOGE(kTag, "cannot read mDNS instance name (%s)", esp_err_to_name(ret)); return ret;
        }

        // addrHandle.close();
    }

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

#ifndef CONFIG_DEFAULT_TIME_SERVER
#define CONFIG_DEFAULT_TIME_SERVER                          "pool.ntp.org"
#endif
char Config::m_timeServer[64] = CONFIG_DEFAULT_TIME_SERVER;
const char* Config::getTimeServer() {
    verifyInit();
    return m_timeServer;
}

#ifndef CONFIG_DEFAULT_MQTT_BROKER
#define CONFIG_DEFAULT_MQTT_BROKER                          "mqtt://melbtrains.mooo.com" // TODO: change this to an actual MQTT broker once we get the backend on the cloud
#endif
char Config::m_mqttBroker[64] = CONFIG_DEFAULT_MQTT_BROKER;
const char* Config::getMQTTBroker() {
    verifyInit();
    return m_mqttBroker;
}

#ifndef CONFIG_DEFAULT_MDNS_HOSTNAME
#define CONFIG_DEFAULT_MDNS_HOSTNAME                        "melbtrains"
#endif
char Config::m_mdnsHostname[32] = CONFIG_DEFAULT_MDNS_HOSTNAME;
const char* Config::getMDNSHostname() {
    verifyInit();
    return m_mdnsHostname;
}

#ifndef CONFIG_DEFAULT_MDNS_INSTANCE_NAME
#define CONFIG_DEFAULT_MDNS_INSTANCE_NAME                   "Melbourne Train Tracker"
#endif
char Config::m_mdnsInstanceName[64] = CONFIG_DEFAULT_MDNS_INSTANCE_NAME;
const char* Config::getMDNSInstanceName() {
    verifyInit();
    return m_mdnsInstanceName;
}
