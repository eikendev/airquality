#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>

class Network {
  private:
    MQTTClient mqtt;
    WiFiClientSecure wifi;

    void get_wifi_connection(void);

  public:
    Network(void);

    void begin(void);

    void connect(void);

    bool is_connected(void);

    void publish(const unsigned int co2_ppm, const int8_t temp_celsius);

    void serve(void);
};

#endif
