#ifndef C_MQTT_H
#define C_MQTT_H

#include <esp_err.h>

extern bool c_mqtt_connected;

esp_err_t c_mqtt_start(void);
esp_err_t c_mqtt_stop(void);
esp_err_t c_mqtt_publish(const char *topic, const char *data);

#endif /* end of include guard: C_MQTT_H */
