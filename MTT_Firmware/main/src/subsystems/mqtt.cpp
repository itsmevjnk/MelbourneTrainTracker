#include "subsystems/mqtt.h"
#include "subsystems/wifi.h"
#include "subsystems/status_led.h"

#include "message.h"
#include "config.h"

#include "esp_check.h"
const char* MQTT::kTag = "mqtt";

esp_err_t MQTT::init(const char* brokerURI) {
    ESP_RETURN_ON_FALSE(WiFi::isConnected(), ESP_ERR_WIFI_NOT_CONNECT, kTag, "not connected to Wi-Fi yet");

    esp_mqtt_client_config_t config; memset(&config, 0, sizeof(config));
    config.broker.address.uri = brokerURI;
    config.network.disable_auto_reconnect = false; // auto reconnect on disconnection
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&config);
    ESP_RETURN_ON_ERROR(
        esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, &eventHandler, nullptr),
        kTag, "cannot register event handler"
    );
    ESP_RETURN_ON_ERROR(
        esp_mqtt_client_start(client),
        kTag, "cannot start MQTT client"
    );

    ESP_LOGI(kTag, "MQTT client initialised, awaiting successful connection");
    return ESP_OK;
}

void MQTT::eventHandler(void* handlerArgs, esp_event_base_t eventBase, int32_t eventID, void* eventData) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)eventData;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)eventID) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(kTag, "connected to MQTT broker, subscribing to topics");
            esp_mqtt_client_subscribe(client, "melbtrains/bin", 1); // QoS 1
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(kTag, "disconnected from MQTT broker");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(kTag, "subscribed to topic, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(kTag, "error event received, error type %d", event->error_handle->error_type);
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                if (event->error_handle->esp_tls_last_esp_err != 0)
                    ESP_LOGE(kTag, "last ESP-TLS error code 0x%x", event->error_handle->esp_tls_last_esp_err);
                if (event->error_handle->esp_tls_stack_err != 0)
                    ESP_LOGE(kTag, "last TLS stack error code 0x%x", event->error_handle->esp_tls_stack_err);
                if (event->error_handle->esp_transport_sock_errno != 0)
                    ESP_LOGE(kTag, "last socket error code 0x%x", event->error_handle->esp_transport_sock_errno);
                ESP_LOGE(kTag, "last errno string: %s", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        case MQTT_EVENT_DATA:
            ESP_ERROR_CHECK(StatusLED::actyOn());
            // ESP_LOGI(kTag, "received message (msg_id=%d) from topic %.*s (%d bytes) - offset %d, total length %d", event->msg_id, event->topic_len, event->topic, event->data_len, event->current_data_offset, event->total_data_len);
            Message::parseFragment(event->data, event->data_len, (event->current_data_offset == 0));
            if (event->current_data_offset + event->data_len >= event->total_data_len) Message::finish();
            ESP_ERROR_CHECK(StatusLED::actyOff());
            break;
        default:
            ESP_LOGW(kTag, "unhandled event %ld", eventID);
            break;
    }
}

