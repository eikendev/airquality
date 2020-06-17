#include "display.hpp"

static void display_print_value(
    Adafruit_SSD1306 &display,
    const __FlashStringHelper *name,
    const int value,
    const bool is_unknown
) {
    display.print(name);

    if (is_unknown)
        display.println(F("-"));
    else
        display.println(value);
}

void display_update(
    Adafruit_SSD1306 &display,
    const int co2_ppm,
    const int8_t temp_celsius,
    const bool is_unknown
) {
    Serial.print(F("CO2 (ppm): "));
    Serial.println(co2_ppm);
    Serial.print(F("Temp. (C): "));
    Serial.println(temp_celsius);

    display.clearDisplay();
    display.setCursor(0, 10);
    display_print_value(display, F("CO2 (ppm): "), co2_ppm, is_unknown);
    display_print_value(display, F("Temp. (C): "), temp_celsius, is_unknown);
    display.display();
}
