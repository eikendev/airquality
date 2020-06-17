#include "network.hpp"
#include "settings.hpp"
#include "secrets.hpp"

static const char wifi_ssid[] = WIFI_SSID;
static const char wifi_password[] = WIFI_PASSWORD;

static const char mqtt_host[] = MQTT_HOST;
static const char mqtt_client[] = MQTT_CLIENT;
static const char mqtt_username[] = MQTT_USERNAME;
static const char mqtt_password[] = MQTT_PASSWORD;

void Network::get_wifi_connection(void)
{
    Serial.print(F("Trying to establish WiFi connection..."));

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(F("."));
        delay(NET_RECONNECT_DELAY);
    }
    Serial.println();
}

Network::Network(void)
{
}

void Network::begin(void)
{
    Serial.println(F("Configuring WiFi connection."));

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    this->get_wifi_connection();

    // Check [0] for certificate validation.
    // [0] https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_Validation/BearSSL_Validation.ino
    this->wifi.setInsecure();

    const bool mfln = this->wifi.probeMaxFragmentLength(mqtt_host, MQTT_PORT, NET_TLS_MFLN);
    if (!mfln) {
        Serial.println(F("Server does not support small payloads!"));
    } else {
        this->wifi.setBufferSizes(NET_TLS_MFLN, NET_TLS_MFLN);
    }

    Serial.println(F("Configuring MQTT connection."));
    this->mqtt.begin(mqtt_host, MQTT_PORT, this->wifi);

    this->connect();
}

void Network::connect(void)
{
    this->get_wifi_connection();

    Serial.print(F("Trying to establish MQTT connection..."));

    while (!this->mqtt.connect(mqtt_client, mqtt_username, mqtt_password)) {
        Serial.print(F("."));
        delay(NET_RECONNECT_DELAY);
    }
    Serial.println();
}

bool Network::is_connected(void)
{
    return this->mqtt.connected();
}

void Network::publish(
    const unsigned int co2_ppm,
    const int8_t temp_celsius
) {
    char buffer[16];

    snprintf(buffer, sizeof(buffer), "%d", co2_ppm);
    this->mqtt.publish(F(CO2_TOPIC), buffer);

    snprintf(buffer, sizeof(buffer), "%d", temp_celsius);
    this->mqtt.publish(F(TEMPERATURE_TOPIC), buffer);
}

void Network::serve(void)
{
    this->mqtt.loop();
}
