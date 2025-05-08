/* MQTT client */

#pragma once

#include "esp_err.h"
#include "mqtt_client.h"

class MQTT {
public:
    static esp_err_t init(const char* brokerURI); // initialise MQTT

private:
    static void eventHandler(void* handlerArgs, esp_event_base_t eventBase, int32_t eventID, void* eventData); // MQTT event handler

    static const char* kTag; // for logging
};