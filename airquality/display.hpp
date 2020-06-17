#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void display_update(
    Adafruit_SSD1306 &display,
    const int co2_ppm,
    const int8_t temp_celsius,
    const bool is_unknown
);

#endif
