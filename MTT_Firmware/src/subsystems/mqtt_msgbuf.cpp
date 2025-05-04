#include "subsystems/mqtt.h"

#include "esp_check.h"

char* MQTT::m_buffer = nullptr; // buffer for reassembling MQTT fragments
int MQTT::m_msgID = -1; // incoming message ID
size_t MQTT::m_msgLength = 0, MQTT::m_receivedBytes = 0;

bool MQTT::receiveFragment(int msgID, size_t totalLength, void* fragment, size_t fragOffset, size_t fragLength) {
    if (!isReceiveActive()) { // new message
        ESP_LOGI(kTag, "beginning receiving message with ID %d", msgID);
        m_msgID = msgID; m_msgLength = totalLength; m_receivedBytes = 0;
        m_buffer = (char*)malloc(totalLength + 1); m_buffer[totalLength] = '\0'; // use malloc to be more explicit, and we'll null-terminate the message in advance so it can be safely passed into cJSON
    }
    else if (msgID != m_msgID) { // incoming message is different from the one we're receiving
        ESP_LOGW(kTag, "discarding message with ID %d while message with ID %d is still being processed", msgID, m_msgID);
        return false;
    }

    assert(m_receivedBytes + fragLength <= m_msgLength); // just to be sure

    memcpy(&m_buffer[fragOffset], fragment, fragLength);
    m_receivedBytes += fragLength;
    if (m_receivedBytes == m_msgLength) {
        ESP_LOGI(kTag, "finished receiving message with ID %d (%u bytes)", m_msgID, m_msgLength);
        return true;
    }
    else return false; // not done yet
}

void MQTT::bufferFinish() {
    if (!isReceiveActive()) {
        ESP_LOGW(kTag, "message receive is not active");
        return;
    }

    ESP_LOGI(kTag, "finished processing message with ID %d", m_msgID);
    free(m_buffer);
    m_buffer = nullptr;
}