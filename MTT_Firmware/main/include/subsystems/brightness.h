#pragma once

#include <esp_err.h>
#include <sys/time.h>

#include <utils/http_client.h>

class Brightness {
public:
    static uint8_t getCurrentBrightness();

private:
    static time_t m_sunriseTime;
    static time_t m_sunsetTime;

    static esp_err_t getTimesFromAPI(HTTPClient& client, time_t now, time_t* sunrise, time_t* sunset); // stub for updateSunTimes
    static esp_err_t updateSunTimes();

    static const char* kTag;
};