#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include "config.h"
#include "mqtt.h"
#include "network.h"
#include "utils.h"

#define N_TASK_STACK_SIZE  4096

static const char *TAG = "Main";

void app_main(void) {
	ESP_LOGI(TAG, "App start");

	ESP_LOGI(TAG, "NVS flash init");
	ESP_ERROR_CHECK(nvs_flash_init());

	ESP_LOGI(TAG, "ESP event loop start");
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	// ESP_LOGI(TAG, "Create data processing task");
	// xTaskCreate((TaskFunction_t)(task_data_process), "Data processing task",
	// 	N_TASK_STACK_SIZE, &task_args, 10, &th_data_process);

	ESP_LOGI(TAG, "Connect to network");
	ESP_ERROR_CHECK(c_network_connect());

	ESP_LOGI(TAG, "Connect to MQTT server");
	ESP_ERROR_CHECK(c_mqtt_start());

	return;
}
