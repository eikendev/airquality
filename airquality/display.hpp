#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#include "settings.hpp"

class Display {
    private:
        Adafruit_SSD1306 ssd1306;

        void print_value(
            const __FlashStringHelper *name,
            const int value,
            const bool is_unknown
        );

    public:
        Display(void);

        void begin(
        );

        void update(
            const int co2_ppm,
            const int8_t temp_celsius,
            const bool is_unknown
        );
};

#endif
