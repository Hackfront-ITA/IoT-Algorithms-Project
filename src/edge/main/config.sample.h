#ifndef E_CONFIG_H
#define E_CONFIG_H

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

#define BUILDING_ID     123

// Time sync delay: 2 * DATA_NUM_SAMPLES / DATA_SAMPLING_FREQ
#define TIMESYNC_DELAY  (10 * 1000)

#endif /* end of include guard: E_CONFIG_H */
