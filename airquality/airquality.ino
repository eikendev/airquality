#include <Arduino.h>
#include <Wire.h>

#include "display.hpp"
#include "network.hpp"
#include "sensor.hpp"
#include "settings.hpp"

static Display display;

static Network network;

static Sensor sensor;

void setup(void)
{
    Serial.begin(SENSOR_BAUDRATE, SERIAL_8N1);

    Wire.begin(SCREEN_SDA_PIN, SCREEN_SCL_PIN);

    display.begin();

    network.begin();

    sensor.begin();
}

static unsigned long dataTimer = 0;

void loop(void)
{
    network.serve();

    if (millis() - dataTimer >= PUBLISH_INTERVAL) {
        const SensorData data = sensor.fetch();

        display.update(data.co2, data.temperature);

        network.publish(data.co2, data.temperature);

        dataTimer = millis();
    }

    delay(MAINLOOP_DELAY);
}
