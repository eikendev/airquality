#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#include <Arduino.h>
#include <MHZ19.h>

typedef struct {
    unsigned int co2;
    int8_t temperature;
} SensorData;

class Sensor {
  private:
    MHZ19 mhz19;

  public:
    Sensor(void);

    void begin(void);

    SensorData fetch(void);
};

#endif
