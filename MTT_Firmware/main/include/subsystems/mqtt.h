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

    /* MQTT message reception - subsystems/mqtt_msgbuf.cpp */
    static char* m_buffer; // buffer for reassembling MQTT fragments
    static int m_msgID; // incoming message ID
    static size_t m_msgLength, m_receivedBytes;
    static inline bool isReceiveActive() {
        return (m_buffer != nullptr);
    };
    static bool receiveFragment(int msgID, size_t totalLength, void* fragment, size_t fragOffset, size_t fragLength); // will automatically start and stop data reception, and return true once the message has been fully assembled
    static void bufferFinish(); // must be called once we've done with the buffer
};