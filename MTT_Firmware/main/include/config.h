/* configuration */

#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_console.h"

enum BrightnessMode {
    kManualTemporary = 0,
    kManualPersistent,
    kAutoManualTime,
    kAutoSunTime // automatic sunset/sunrise time
};

class Config {
public:
    static esp_err_t init(); // load configuration if possible; otherwise, drop into config CLI
    static esp_err_t cli(); // run configuration CLI - note that this does not block, and the caller must exit from the task to yield to the CLI!

    static bool isInitialised(); // return true if configuration is loaded

    /* get Wi-Fi config */
    static bool isWiFiEnterprise(); // return true if WPA2-Enterprise is to be used
    static const char* getWiFiSSID();
    static const char* getWiFiPassword(); // WPA2 passphrase or WPA2-Enterprise EAP password
    static const char* getWiFiIdentity(); // null if isWiFiEnterprise() return false
    static const char* getWiFiUsername(); // null if isWiFiEnterprise() return false
    static const char* getWiFiCertificate(); // null if isWiFiEnterprise() return false
    static size_t getWiFiCertLength(); // 0 if isWiFiEnterprise() return false

    /* get address config */
    static const char* getTimeServer();
    static const char* getMQTTBroker();

    /* get mDNS config */
    static const char* getMDNSHostname();
    static const char* getMDNSInstanceName();

    static esp_err_t setTimeServer(const char* value);
    static esp_err_t setMQTTBroker(const char* value);
    static esp_err_t setMDNSHostname(const char* value);
    static esp_err_t setMDNSInstanceName(const char* value);

    static esp_err_t setWiFiCredentials(const char* ssid); // no authentication
    static esp_err_t setWiFiCredentials(const char* ssid, const char* password); // WPA2
    static esp_err_t setWiFiCredentials(const char* ssid, const char* identity, const char* username, const char* password); // WPA2-Enterprise
    static esp_err_t setWiFiCredentials(const char* ssid, const char* identity, const char* username, const char* password, const char* cert, size_t certLength); // WPA2-Enterprise with certificate

    static BrightnessMode getBrightnessMode();
    static uint8_t getMinBrightness();
    static uint8_t getMaxBrightness();
    static uint32_t getBrightnessSunsetTime();
    static uint32_t getBrightnessSunriseTime();
    static float getBrightnessLatitude();
    static float getBrightnessLongitude();

    static void overrideBrightness(uint8_t val);
    static esp_err_t setBrightnessMode(uint8_t init, bool persist); // manual mode
    static esp_err_t setBrightnessMode(uint8_t minValue, uint8_t maxValue, uint32_t sunsetTime, uint32_t sunriseTime); // auto mode with manual time
    static esp_err_t setBrightnessMode(uint8_t minValue, uint8_t maxValue, float latitude, float longitude); // auto mode with auto time

private:
    static bool m_initialised;

    /* Wi-Fi configuration */
    static bool m_wifiEnterprise;
    static char m_wifiSSID[32];
    static char m_wifiUsername[64];
    static char m_wifiPassword[64];
    static char m_wifiIdentity[64];
    static char* m_wifiCert;
    static size_t m_wifiCertLength;

    /* brightness configuration */
    static BrightnessMode m_brightMode;
    static uint8_t m_brightMin;
    static uint8_t m_brightMax; // also initial brightness
    static uint32_t m_brightSunsetTime; // seconds since midnight
    static uint32_t m_brightSunriseTime; // seconds since midnight of previous day
    static float m_brightLatitude;
    static float m_brightLongitude;

    /* network address configuration */
    static char m_timeServer[64];
    static char m_mqttBroker[64];

    /* mDNS configuration */
    static char m_mdnsHostname[32];
    static char m_mdnsInstanceName[64];

    static const char* kTag; // for logging

    /* verify that config is initialised */
    static inline void verifyInit() {
        if (!m_initialised) {
            ESP_LOGE(kTag, "config must be initialised before calling method");
            abort();
        }
    }

    /* CLI */
    static esp_console_repl_t* m_repl; // repl object

    static const esp_console_cmd_t* kCommands[];
    static esp_err_t cliRegisterCommands();

    static esp_console_cmd_t kExitCommand;
    static esp_err_t cliExit(int argc, char** argv);

    static esp_console_cmd_t kShowConfigCommand;
    static esp_err_t cliShowConfig(int argc, char** argv);

    static esp_console_cmd_t kShowCertificateCommand;
    static esp_err_t cliShowCertificate(int argc, char** argv);

    static esp_console_cmd_t kSetWiFiCommand;
    static esp_err_t cliSetWiFi(int argc, char** argv);

    static esp_console_cmd_t kSetNTPCommand;
    static esp_err_t cliSetNTP(int argc, char** argv);

    static esp_console_cmd_t kSetMQTTCommand;
    static esp_err_t cliSetMQTT(int argc, char** argv);

    static esp_console_cmd_t kSetMDNSCommand;
    static esp_err_t cliSetMDNS(int argc, char** argv);

    /* utilities for CLI */
    static size_t readString(char* str, size_t maxSize);
    static bool readYN(); // yes/no prompt
};