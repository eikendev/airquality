#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <MQTT.h>

#include "settings.hpp"
#include "secrets.hpp"
#include "display.hpp"
#include "network.hpp"
#include "sensor.hpp"

static const char wifi_ssid[] = WIFI_SSID;
static const char wifi_password[] = WIFI_PASSWORD;

static const char mqtt_host[] = MQTT_HOST;

static WiFiClientSecure net;

static MQTTClient mqtt;

static Display display;

static Sensor sensor;

void setup(void)
{
    Serial.begin(SENSOR_BAUDRATE, SERIAL_8N1);

    Wire.begin(SCREEN_SDA_PIN, SCREEN_SCL_PIN);

    display.begin();

    Serial.println(F("Configuring WiFi connection..."));
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(NET_RECONNECT_DELAY);
    }

    // Check [0] for certificate validation.
    // [0] https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_Validation/BearSSL_Validation.ino
    net.setInsecure();

    const bool mfln = net.probeMaxFragmentLength(mqtt_host, MQTT_PORT, NET_TLS_MFLN);
    if (!mfln) {
        Serial.println(F("Server does not support small payloads!"));
    } else {
        net.setBufferSizes(NET_TLS_MFLN, NET_TLS_MFLN);
    }

    Serial.println(F("Configuring MQTT connection..."));
    mqtt.begin(mqtt_host, MQTT_PORT, net);

    network_connect(mqtt, net);

    sensor.begin();
}

static unsigned long dataTimer = 0;

void loop(void)
{
    mqtt.loop();
    delay(100);

    if (!mqtt.connected()) {
        network_connect(mqtt, net);
    }

    if (millis() - dataTimer >= SENSOR_INTERVAL) {
        const SensorData data = sensor.fetch();

        display.update(data.co2, data.temperature);

        char buffer[25];
        snprintf(buffer, sizeof(buffer), "%d", data.co2);
        mqtt.publish(F(CO2_TOPIC), buffer);
        snprintf(buffer, sizeof(buffer), "%d", data.temperature);
        mqtt.publish(F(TEMPERATURE_TOPIC), buffer);

        dataTimer = millis();
    }
}
