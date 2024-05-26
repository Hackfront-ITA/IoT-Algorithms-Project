#ifndef A_CONFIG_H
#define A_CONFIG_H

// ADXL345 LPM sampling frequencies: [ 12.5, 25, 50, 100, 200, 400 ]
#define DATA_SAMPLING_FREQ    50
#define DATA_NUM_SAMPLES      2048

#define NET_WIFI_SSID       "<wifi ssid>"
#define NET_WIFI_PASSWORD   "<wifi password>"

// #define NET_WIFI_SECURITY WIFI_AUTH_OPEN
// #define NET_WIFI_SECURITY WIFI_AUTH_WEP
// #define NET_WIFI_SECURITY WIFI_AUTH_WPA_PSK
// #define NET_WIFI_SECURITY WIFI_AUTH_WPA2_PSK
#define NET_WIFI_SECURITY WIFI_AUTH_WPA_WPA2_PSK
// #define NET_WIFI_SECURITY WIFI_AUTH_WPA3_PSK
// #define NET_WIFI_SECURITY WIFI_AUTH_WPA2_WPA3_PSK

#define MQTT_BROKER_URL  "mqtts://<hostname>:8883"
#define MQTT_BASE_TOPIC  "/tests/esp32"

#endif /* end of include guard: A_CONFIG_H */
