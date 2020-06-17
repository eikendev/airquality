#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#define CO2_TOPIC "/iot/sensor/airquality/co2"
#define TEMPERATURE_TOPIC "/iot/sensor/airquality/temp"

#define NET_RECONNECT_DELAY (2000)
#define NET_TLS_MFLN (1024)

#define SCREEN_ADDRESS (0x3C)
#define SCREEN_WIDTH (128)
#define SCREEN_HEIGHT (64)
#define SCREEN_SDA_PIN (4)
#define SCREEN_SCL_PIN (5)

#define SENSOR_RX_PIN (13) // RX pin which the MHZ19 TX pin is attached to
#define SENSOR_TX_PIN (15) // TX pin which the MHZ19 RX pin is attached to
#define SENSOR_BAUDRATE (9600)
#define SENSOR_INTERVAL (2000)

#endif