#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <Adafruit_SSD1306.h>
#include <Arduino.h>

class Display {
  private:
    Adafruit_SSD1306 ssd1306;

    void print_value(const __FlashStringHelper *name, const int value, const bool is_unknown);

    void print_values(const unsigned int co2_ppm, const bool co2_ppm_valid, const int8_t temp_celsius,
                      const bool temp_celsius_valid);

  public:
    Display(void);

    void begin(void);

    void reset(void);

    void update(const unsigned int co2_ppm, const int8_t temp_celsius);
};

#endif
