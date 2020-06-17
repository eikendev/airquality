#include <ESP8266WiFi.h>

#include "network.hpp"
#include "settings.hpp"
#include "secrets.hpp"

static const char mqtt_client[] = MQTT_CLIENT;
static const char mqtt_username[] = MQTT_USERNAME;
static const char mqtt_password[] = MQTT_PASSWORD;

void network_connect(
    MQTTClient &mqtt,
    WiFiClientSecure &net
) {
    Serial.println(F("Trying to establish WiFi connection..."));
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(F("."));
        delay(NET_RECONNECT_DELAY);
    }
    Serial.println();

    net.setInsecure();

    Serial.println(F("Trying to establish MQTT connection..."));
    while (!mqtt.connect(mqtt_client, mqtt_username, mqtt_password)) {
        Serial.print(F("."));
        delay(NET_RECONNECT_DELAY);
    }
    Serial.println();
}
