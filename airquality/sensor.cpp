#include "sensor.hpp"
#include "settings.hpp"

#define UART_SWAP { Serial.flush(); Serial.swap(); }

Sensor::Sensor(void)
{
}

void Sensor::begin(void)
{
    Serial.println(F("Initializing sensor."));

    UART_SWAP;
    mhz19.begin(Serial);
    mhz19.autoCalibration();
    UART_SWAP;
}

SensorData Sensor::fetch(void)
{
    UART_SWAP;
    const unsigned int co2_ppm = mhz19.getCO2();
    const int8_t temp_celsius = mhz19.getTemperature();
    UART_SWAP;

    return {
        .co2 = co2_ppm,
        .temperature = temp_celsius
    };
}
