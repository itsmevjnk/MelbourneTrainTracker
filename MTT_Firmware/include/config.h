/* configuration */

#pragma once

#include "esp_err.h"
#include "esp_log.h"
#include "esp_console.h"

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

    /* network address configuration */
    static char m_timeServer[64];

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

    /* utilities for CLI */
    static size_t readString(char* str, size_t maxSize);
    static bool readYN(); // yes/no prompt
};