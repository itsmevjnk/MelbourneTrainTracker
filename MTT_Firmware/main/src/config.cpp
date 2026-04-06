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
            m_wifiCert = (char*) malloc(m_wifiCertLength);
            ESP_RETURN_ON_FALSE(m_wifiCert, ESP_ERR_NO_MEM, kTag, "Wi-Fi server certificate buffer allocation failed");
            ESP_RETURN_ON_ERROR(wifiHandle.getBlob("cert", m_wifiCert, m_wifiCertLength), kTag, "cannot read Wi-Fi server certificate");
        }
    }
    // wifiHandle.close(); // while it will be closed on destruction, it's best if we do that by ourselves

    /* load brightness config */
    NVSHandle brightHandle = NVS::open("bright", NVS_READONLY);
    if (brightHandle.isClosed()) {
        ESP_LOGE(kTag, "cannot open brightness configuration from NVS - using default values");
    } else {
        uint8_t mode;
        esp_err_t ret = brightHandle.getU8("mode", &mode);
        switch (ret) {
            case ESP_OK: m_brightMode = (BrightnessMode)mode; break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "brightness mode not set in NVS, defaulting to %d", (int)m_brightMode); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read brightness mode (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read brightness mode (%d)", ret);
#endif
                return ret;
        }
        
        ret = brightHandle.getU8("min", &m_brightMin);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "min brightness not set in NVS, defaulting to %u", m_brightMin); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read min brightness (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read min brightness (%d)", ret);
#endif
                return ret;
        }
        
        ret = brightHandle.getU8("max", &m_brightMax);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "max brightness not set in NVS, defaulting to %u", m_brightMax); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read max brightness (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read max brightness (%d)", ret);
#endif
                return ret;
        }

        ret = brightHandle.getU32("set", &m_brightSunsetTime);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "sunset time not set in NVS, defaulting to %u", m_brightSunsetTime); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read sunset time (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read sunset time (%d)", ret);
#endif
                return ret;
        }
        ret = brightHandle.getU32("rise", &m_brightSunriseTime);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "sunrise time not set in NVS, defaulting to %u", m_brightSunriseTime); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read sunrise time (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read sunrise time (%d)", ret);
#endif
                return ret;
        }

        ret = brightHandle.getU32("lat", (uint32_t*)&m_brightLatitude);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "latitude not set in NVS, defaulting to %f", m_brightLatitude); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read latitude (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read latitude (%d)", ret);
#endif
                return ret;
        }

        ret = brightHandle.getU32("long", (uint32_t*)&m_brightLongitude);
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "longitude not set in NVS, defaulting to %f", m_brightLongitude); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read longitude (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read longitude (%d)", ret);
#endif
                return ret;
        }
    }

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
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read time server address (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read time server address (%d)", ret);
#endif
                return ret;
        }

        ret = addrHandle.getString("mqtt", m_mqttBroker, sizeof(m_mqttBroker));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "MQTT broker not set in NVS, defaulting to %s", m_mqttBroker); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read MQTT broker address (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read MQTT broker address (%d)", ret);
#endif
                return ret;
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
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read mDNS hostname (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read mDNS hostname (%d)", ret);
#endif
                return ret;
        }

        ret = mdnsHandle.getString("inst", m_mdnsInstanceName, sizeof(m_mdnsInstanceName));
        switch (ret) {
            case ESP_OK: break;
            case ESP_ERR_NVS_NOT_FOUND: ESP_LOGW(kTag, "mDNS instance name not set in NVS, defaulting to %s", m_mdnsInstanceName); break;
            default:
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                ESP_LOGE(kTag, "cannot read mDNS instance name (%s)", esp_err_to_name(ret));
#else
                ESP_LOGE(kTag, "cannot read mDNS instance name (%d)", ret);
#endif
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
    // verifyInit();
    return m_wifiEnterprise;
}

char Config::m_wifiSSID[32] = "";
const char* Config::getWiFiSSID() {
    // verifyInit();
    return m_wifiSSID;
}

char Config::m_wifiUsername[64] = "";
const char* Config::getWiFiUsername() {
    // verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiUsername;
}

char Config::m_wifiPassword[64] = "";
const char* Config::getWiFiPassword() {
    // verifyInit();
    return m_wifiPassword;
}

char Config::m_wifiIdentity[64] = "";
const char* Config::getWiFiIdentity() {
    // verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiIdentity;
}

char* Config::m_wifiCert = nullptr;
const char* Config::getWiFiCertificate() {
    // verifyInit();
    return (!m_wifiEnterprise) ? nullptr : m_wifiCert;
}

size_t Config::m_wifiCertLength = 0;
size_t Config::getWiFiCertLength() {
    // verifyInit();
    return (!m_wifiEnterprise) ? 0 : m_wifiCertLength;
}

#ifndef CONFIG_DEFAULT_TIME_SERVER
#define CONFIG_DEFAULT_TIME_SERVER                          "time.google.com"
#endif
char Config::m_timeServer[64] = CONFIG_DEFAULT_TIME_SERVER;
const char* Config::getTimeServer() {
    // verifyInit();
    return m_timeServer;
}

#ifndef CONFIG_DEFAULT_MQTT_BROKER
#define CONFIG_DEFAULT_MQTT_BROKER                          "mqtt://melbtrains.mooo.com" // TODO: change this to an actual MQTT broker once we get the backend on the cloud
#endif
char Config::m_mqttBroker[64] = CONFIG_DEFAULT_MQTT_BROKER;
const char* Config::getMQTTBroker() {
    // verifyInit();
    return m_mqttBroker;
}

#ifndef CONFIG_DEFAULT_MDNS_HOSTNAME
#define CONFIG_DEFAULT_MDNS_HOSTNAME                        "victrains"
#endif
char Config::m_mdnsHostname[32] = CONFIG_DEFAULT_MDNS_HOSTNAME;
const char* Config::getMDNSHostname() {
    // verifyInit();
    return m_mdnsHostname;
}

#ifndef CONFIG_DEFAULT_MDNS_INSTANCE_NAME
#define CONFIG_DEFAULT_MDNS_INSTANCE_NAME                   "Victorian Train Tracker"
#endif
char Config::m_mdnsInstanceName[64] = CONFIG_DEFAULT_MDNS_INSTANCE_NAME;
const char* Config::getMDNSInstanceName() {
    // verifyInit();
    return m_mdnsInstanceName;
}

#ifndef CONFIG_DEFAULT_BRIGHTNESS_MODE
#define CONFIG_DEFAULT_BRIGHTNESS_MODE                      kManualTemporary
#endif
BrightnessMode Config::m_brightMode = CONFIG_DEFAULT_BRIGHTNESS_MODE;
BrightnessMode Config::getBrightnessMode() {
    return m_brightMode;
}

#ifndef CONFIG_DEFAULT_MIN_BRIGHTNESS
#define CONFIG_DEFAULT_MIN_BRIGHTNESS                       10
#endif
uint8_t Config::m_brightMin = CONFIG_DEFAULT_MIN_BRIGHTNESS;
uint8_t Config::getMinBrightness() {
    return m_brightMin;
}

#ifndef CONFIG_DEFAULT_MAX_BRIGHTNESS
#define CONFIG_DEFAULT_MAX_BRIGHTNESS                       100
#endif
uint8_t Config::m_brightMax = CONFIG_DEFAULT_MAX_BRIGHTNESS;
uint8_t Config::getMaxBrightness() {
    return m_brightMax;
}

#ifndef CONFIG_DEFAULT_SUNSET_TIME
#define CONFIG_DEFAULT_SUNSET_TIME                          (18 * 3600) // 6pm
#endif
uint32_t Config::m_brightSunsetTime = CONFIG_DEFAULT_SUNSET_TIME;
uint32_t Config::getBrightnessSunsetTime() {
    return m_brightSunsetTime;
}

#ifndef CONFIG_DEFAULT_SUNRISE_TIME
#define CONFIG_DEFAULT_SUNRISE_TIME                         ((24 + 6) * 3600) // 6am next day
#endif
uint32_t Config::m_brightSunriseTime = CONFIG_DEFAULT_SUNRISE_TIME;
uint32_t Config::getBrightnessSunriseTime() {
    return m_brightSunriseTime;
}

#ifndef CONFIG_DEFAULT_LATITUDE
#define CONFIG_DEFAULT_LATITUDE                             -37.81823723747155 // Flinders Street Station
#endif
float Config::m_brightLatitude = CONFIG_DEFAULT_LATITUDE;
float Config::getBrightnessLatitude() {
    return m_brightLatitude;
}

#ifndef CONFIG_DEFAULT_LONGITUDE
#define CONFIG_DEFAULT_LONGITUDE                            144.96720709868555
#endif
float Config::m_brightLongitude = CONFIG_DEFAULT_LONGITUDE;
float Config::getBrightnessLongitude() {
    return m_brightLongitude;
}

esp_err_t Config::setBrightnessMode(uint8_t init, bool persist) {
    if (init > 100) init = 100; // 0-100 only
    m_brightMode = (persist) ? kManualPersistent : kManualTemporary;
    m_brightMax = init;
    
    NVSHandle handle = NVS::open("bright", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for brightness");

    ESP_RETURN_ON_ERROR(handle.setU8("mode", (uint8_t)m_brightMode), kTag, "cannot write brightness mode to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("max", m_brightMax), kTag, "cannot write initial brightness to NVS");
    return ESP_OK;
}

esp_err_t Config::setBrightnessMode(uint8_t minValue, uint8_t maxValue, uint32_t sunsetTime, uint32_t sunriseTime) {
    if (maxValue > 100) maxValue = 100;
    if (minValue > 100) minValue = 100;
    if (minValue > maxValue) return ESP_ERR_INVALID_ARG;
    if (sunsetTime >= 86400 || sunriseTime >= 86400 * 2 || abs((int)sunsetTime - (int)sunriseTime) >= 86400) return ESP_ERR_INVALID_ARG;

    m_brightMode = kAutoManualTime;
    m_brightMin = minValue; m_brightMax = maxValue;
    m_brightSunsetTime = sunsetTime; m_brightSunriseTime = sunriseTime;

    NVSHandle handle = NVS::open("bright", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for brightness");

    ESP_RETURN_ON_ERROR(handle.setU8("mode", (uint8_t)m_brightMode), kTag, "cannot write brightness mode to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("min", m_brightMin), kTag, "cannot write min brightness to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("max", m_brightMax), kTag, "cannot write max brightness to NVS");
    ESP_RETURN_ON_ERROR(handle.setU32("set", m_brightSunsetTime), kTag, "cannot write sunset time to NVS");
    ESP_RETURN_ON_ERROR(handle.setU32("rise", m_brightSunriseTime), kTag, "cannot write sunrise time to NVS");
    return ESP_OK;
}

esp_err_t Config::setBrightnessMode(uint8_t minValue, uint8_t maxValue, float latitude, float longitude) {
    if (maxValue > 100) maxValue = 100;
    if (minValue > 100) minValue = 100;
    if (minValue > maxValue) return ESP_ERR_INVALID_ARG;
    if (latitude < -90 || latitude > 90 || longitude < -180 || latitude > 180) return ESP_ERR_INVALID_ARG;

    m_brightMode = kAutoSunTime;
    m_brightMin = minValue; m_brightMax = maxValue;
    m_brightLatitude = latitude; m_brightLongitude = longitude;

    NVSHandle handle = NVS::open("bright", NVS_READWRITE);
    ESP_RETURN_ON_FALSE(!handle.isClosed(), ESP_FAIL, kTag, "cannot open NVS handle for brightness");

    ESP_RETURN_ON_ERROR(handle.setU8("mode", (uint8_t)m_brightMode), kTag, "cannot write brightness mode to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("min", m_brightMin), kTag, "cannot write min brightness to NVS");
    ESP_RETURN_ON_ERROR(handle.setU8("max", m_brightMax), kTag, "cannot write max brightness to NVS");
    ESP_RETURN_ON_ERROR(handle.setU32("lat", *(uint32_t*)&m_brightLatitude), kTag, "cannot write latitude to NVS"); // since NVS doesn't support float which is also 32-bit
    ESP_RETURN_ON_ERROR(handle.setU32("long", *(uint32_t*)&m_brightLongitude), kTag, "cannot write longitude to NVS");
    return ESP_OK;
}

esp_err_t Config::overrideBrightness(uint8_t val) {
    m_brightMax = val;
    if (m_brightMode == kManualPersistent)
        return setBrightnessMode(val, true); // persist in NVS
    else {
        m_brightMode = kManualTemporary;
        return ESP_OK;
    }
}
