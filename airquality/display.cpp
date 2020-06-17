#include "display.hpp"
#include "settings.hpp"

#define PANIC { while (true) {}; }

void Display::print_value(
    const __FlashStringHelper *name,
    const int value,
    const bool is_unknown
) {
    this->ssd1306.print(name);

    if (is_unknown)
        this->ssd1306.println(F("-"));
    else
        this->ssd1306.println(value);
}

Display::Display(void)
{
    this->ssd1306 = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
}

void Display::begin(void)
{
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
}

void Display::update(
    const int co2_ppm,
    const int8_t temp_celsius,
    const bool is_unknown
) {
    Serial.print(F("CO2 (ppm): "));
    Serial.println(co2_ppm);
    Serial.print(F("Temp. (C): "));
    Serial.println(temp_celsius);

    this->ssd1306.clearDisplay();
    this->ssd1306.setCursor(0, 10);
    this->print_value(F(" CO2 (ppm): "), co2_ppm, is_unknown);
    this->print_value(F(" Temp. (C): "), temp_celsius, is_unknown);
    this->ssd1306.display();
}
