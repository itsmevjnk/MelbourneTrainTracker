#include <subsystems/brightness.h>
#include <config.h>

#include <esp_check.h>
#include <math.h>

const char* Brightness::kTag = "bright";

time_t Brightness::m_sunsetTime = 0;
time_t Brightness::m_sunriseTime = 0;

esp_err_t Brightness::getTimesFromAPI(HTTPClient& client, time_t now, time_t* sunrise, time_t* sunset) {
    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    char url[160];
    sprintf(
        url, "https://api.sunrise-sunset.org/json?lat=%.6f&lng=%.6f&date=%04d-%02d-%02d&formatted=0&tzid=Australia/Melbourne",
        Config::getBrightnessLatitude(), Config::getBrightnessLongitude(),
        timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday
    );
    client.setURL(url);
    ESP_RETURN_ON_ERROR(
        client.perform(),
        kTag, "failed to call sunrise/sunset API"
    );
    int status = client.getStatusCode();
    if (status >= 400) {
        ESP_LOGE(kTag, "received invalid HTTP status code %d from sunrise/sunset API", status);
        return ESP_FAIL;
    }

    JsonDocument filter;
    filter["results"]["sunrise"] = true;
    filter["results"]["day_length"] = true; // to be added to sunrise timestamp

    JsonDocument doc;
    DeserializationError jsonErr = client.deserialiseResponse(doc, filter);
    if (jsonErr) {
        ESP_LOGE(kTag, "error parsing JSON from sunrise/sunset API: %s", jsonErr.c_str());
        return ESP_FAIL;
    }

    int offsetHour, offsetMin;
    sscanf(
        doc["results"]["sunrise"], "%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d",
        &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday,
        &timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec,
        &offsetHour, &offsetMin // ignored
    );
    timeinfo.tm_mon--;
    timeinfo.tm_year -= 1900;
    time_t sunriseTime = mktime(&timeinfo);

    if (sunrise) *sunrise = sunriseTime;
    if (sunset) *sunset = sunriseTime + int(doc["results"]["day_length"]);
 
    return ESP_OK;
}

#ifndef CONFIG_DIM_ALPHA
#define CONFIG_DIM_ALPHA                                    10
#endif

esp_err_t Brightness::updateSunTimes() {
    time_t now; time(&now);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);

    HTTPClient client("https://api.sunrise-sunset.org/json"); // placeholder URL
    client.setHeader("Accept", "application/json");

    /* probe using current day */
    time_t sunriseTime, sunsetTime;
    ESP_RETURN_ON_ERROR(
        getTimesFromAPI(client, now, &sunriseTime, &sunsetTime),
        kTag, "cannot get sunrise/sunset time for current day"
    );
    
    if (now < sunriseTime) { // next sunset is today
        ESP_RETURN_ON_ERROR(
            getTimesFromAPI(client, now - 86400, nullptr, &sunsetTime),
            kTag, "cannot get sunset time for previous day"
        );
    } else { // next sunrise is tomorrow
        ESP_RETURN_ON_ERROR(
            getTimesFromAPI(client, now + 86400, &sunriseTime, nullptr),
            kTag, "cannot get sunrise time for next day"
        );
    }

    ESP_LOGI(kTag, "sunset time: %lld, sunrise time: %lld", sunsetTime, sunriseTime);

    m_sunriseTime = sunriseTime;
    m_sunsetTime = sunsetTime;
    return ESP_OK;
}

uint8_t Brightness::getCurrentBrightness() {
    BrightnessMode mode = Config::getBrightnessMode();
    if (mode == kManualPersistent || mode == kManualTemporary)
        return Config::getMaxBrightness();
    else {
        time_t now; time(&now);
        time_t sunsetTime, sunriseTime;
        if (mode == kAutoManualTime) {
            sunsetTime = Config::getBrightnessSunsetTime();
            sunriseTime = Config::getBrightnessSunriseTime();
            
            /* add day offset */
            struct tm timeinfo;
            localtime_r(&now, &timeinfo);
            uint32_t tod = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec; // time of day since midnight
            time_t midnightTime = now - tod; // current day
            if (sunriseTime >= 86400) { // sunrise time is in the next day
                time_t sunriseTOD = sunriseTime % 86400; // sunrise time of day since midnight
                if (tod <= sunriseTOD) { // at midnight till sunrise
                    midnightTime -= 86400; // go back by 1 day
                }
            }
            sunsetTime += midnightTime;
            sunriseTime += midnightTime;
        } else { // auto time from sunset/sunrise API
            if (now > m_sunriseTime) { // time expired - fetch times for today
                esp_err_t err = updateSunTimes();
                if (err != ESP_OK) {
#ifdef CONFIG_ESP_ERR_TO_NAME_LOOKUP
                    ESP_LOGE(kTag, "failed to retrieve sunset and sunrise times from API (%s)", esp_err_to_name(err));
#else
                    ESP_LOGE(kTag, "failed to retrieve sunset and sunrise times from API (%d)", err);
#endif
                    return Config::getMaxBrightness(); // fallback
                }
            }

            sunsetTime = m_sunsetTime;
            sunriseTime = m_sunriseTime;
        }

        if (now < sunsetTime) return Config::getMaxBrightness(); // not sunset yet

        // raw brightness scale: 1 at sunset -> 0 at middle (midnight) -> 1 at sunrise (linear)
        time_t midnightTime = (sunsetTime + sunriseTime) / 2;
        float rawScale = fabs(((float)now - (float)midnightTime) / (float)midnightTime);
        
        // exponentially scale the raw scale
        float expScale = (pow(CONFIG_DIM_ALPHA, rawScale) - 1) / (float)(CONFIG_DIM_ALPHA - 1);

        // apply scaling on brightness
        float min = Config::getMinBrightness();
        float max = Config::getMaxBrightness();
        float scaledBrightness = min + (max - min) * expScale;
        return scaledBrightness;
    }
}