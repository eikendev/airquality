#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <MHZ19.h>
#include <MQTT.h>

#include "settings.hpp"
#include "secrets.hpp"
#include "display.hpp"
#include "network.hpp"

#define UART_SWAP { Serial.flush(); Serial.swap(); }

static const char wifi_ssid[] = WIFI_SSID;
static const char wifi_password[] = WIFI_PASSWORD;

static const char mqtt_host[] = MQTT_HOST;

static WiFiClientSecure net;

static MQTTClient mqtt;

static MHZ19 mhz19;

static Display display;

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

    Serial.println(F("Populating display..."));
    display.update(0, 0, true);

    Serial.println(F("Initializing sensor..."));
    Serial.flush();

    UART_SWAP;
    mhz19.begin(Serial);
    mhz19.autoCalibration();
    UART_SWAP;
}

static unsigned long getDataTimer = 0;

void loop(void)
{
    mqtt.loop();
    delay(100);

    if (!mqtt.connected()) {
        network_connect(mqtt, net);
    }

    if (millis() - getDataTimer >= SENSOR_INTERVAL) {
        UART_SWAP;
        const int co2_ppm = mhz19.getCO2();
        const int8_t temp_celsius = mhz19.getTemperature();
        UART_SWAP;

        display.update(co2_ppm, temp_celsius, false);

        char buffer[25];
        snprintf(buffer, sizeof(buffer), "%d", co2_ppm);
        mqtt.publish(F(CO2_TOPIC), buffer);
        snprintf(buffer, sizeof(buffer), "%d", temp_celsius);
        mqtt.publish(F(TEMPERATURE_TOPIC), buffer);

        getDataTimer = millis();
    }
}
