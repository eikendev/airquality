#include "display.hpp"

#include "settings.hpp"

#define PANIC          \
    {                  \
        while (true) { \
        };             \
    }

void Display::print_value(const __FlashStringHelper *name, const int value, const bool is_valid) {
    this->ssd1306.print(name);

    if (is_valid)
        this->ssd1306.println(value);
    else
        this->ssd1306.println(F("-"));
}

void Display::print_values(const unsigned int co2_ppm, const bool co2_ppm_valid, const int8_t temp_celsius,
                           const bool temp_celsius_valid) {
    this->ssd1306.clearDisplay();
    this->ssd1306.setCursor(0, 10);

    this->print_value(F(" CO2 (ppm): "), co2_ppm, co2_ppm_valid);
    this->print_value(F(" Temp. (C): "), temp_celsius, temp_celsius_valid);

    this->ssd1306.display();
}

Display::Display(void) { this->ssd1306 = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); }

void Display::begin(void) {
    Serial.println(F("Initializing display."));

    while (!this->ssd1306.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Allocation for screen buffer failed!"));
        PANIC;
    }

    Serial.println(F("Resetting display."));

    this->ssd1306.clearDisplay();
    this->ssd1306.setTextSize(1);
    this->ssd1306.setTextColor(SSD1306_WHITE);
    this->ssd1306.display();

    Serial.println(F("Populating display..."));
    this->reset();
}

void Display::reset(void) { this->print_values(0, false, 0, false); }

void Display::update(const unsigned int co2_ppm, const int8_t temp_celsius) {
    Serial.print(F("CO2 (ppm): "));
    Serial.println(co2_ppm);
    Serial.print(F("Temp. (C): "));
    Serial.println(temp_celsius);

    this->print_values(co2_ppm, true, temp_celsius, true);
}
