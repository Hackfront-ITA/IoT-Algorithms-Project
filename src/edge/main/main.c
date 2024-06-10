#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include "tasks/tasks.h"
#include "config.h"
#include "lora.h"
#include "mqtt.h"
#include "network.h"
#include "protocol.h"
#include "utils.h"

#define N_TASK_STACK_SIZE  4096

TaskHandle_t th_message_relay = NULL;
TaskHandle_t th_time_sync = NULL;

static const char *TAG = "Main";

static task_args_t task_args = {
};

void app_main(void) {
	ESP_LOGI(TAG, "App start");

	ESP_LOGI(TAG, "NVS flash init");
	ESP_ERROR_CHECK(nvs_flash_init());

	ESP_LOGI(TAG, "ESP event loop start");
	ESP_ERROR_CHECK(esp_event_loop_create_default());

	ESP_LOGI(TAG, "Protocol init");
	ESP_ERROR_CHECK(c_proto_init());

	ESP_LOGI(TAG, "LoRa module init");
	ESP_ERROR_CHECK(c_lora_init());

	ESP_LOGI(TAG, "Create message relay task");
	xTaskCreate((TaskFunction_t)(task_message_relay), "Message relay task",
		N_TASK_STACK_SIZE, &task_args, 10, &th_message_relay);
	ESP_LOGI(TAG, "Create time sync task");
	xTaskCreate((TaskFunction_t)(task_time_sync), "Time sync task",
		N_TASK_STACK_SIZE, &task_args, 20, &th_time_sync);

	ESP_LOGI(TAG, "Connect to network");
	ESP_ERROR_CHECK(c_network_connect());

	ESP_LOGI(TAG, "Connect to MQTT server");
	ESP_ERROR_CHECK(c_mqtt_start());

	return;
}
