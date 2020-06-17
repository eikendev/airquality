#ifndef __NETWORK_HPP__
#define __NETWORK_HPP__

#include <Arduino.h>
#include <MQTT.h>

void network_connect(
    MQTTClient &mqtt,
    WiFiClientSecure &net
);

#endif
