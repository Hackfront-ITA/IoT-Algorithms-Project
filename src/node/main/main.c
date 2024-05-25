#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "utils.h"

#define A_TASK_STACK_SIZE  1024

TaskHandle_t th_data_read = NULL;
TaskHandle_t th_data_process = NULL;

static const char *TAG = "App main";

static task_args_t task_args;

void app_main(void) {
	ESP_LOGI(TAG, "App start");

	ESP_ERROR_CHECK(adxl345_init());
	ESP_LOGI(TAG, "ADXL345 initialized successfully");


	ESP_LOGI(TAG, "calloc(%d, %d)", 3 * num_samples, sizeof(float));
	float *accel_data = calloc(3 * num_samples, sizeof(float));
	if (accel_data == NULL) {
		ESP_LOGE(TAG, "Error allocating accel_data");
		return;
	}

	task_args.sampling_freq = sampling_freq;
	task_args.num_samples = num_samples;
	task_args.accel_data = accel_data;

	ESP_LOGI(TAG, "Create data collecting task\n");
	xTaskCreatePinnedToCore(task_data_collect, "Data collecting task",
		A_TASK_STACK_SIZE, &task_args, 10, &th_data_read, 1);

	ESP_LOGI(TAG, "Create data processing task\n");
	xTaskCreate(task_data_process, "Data processing task",
		A_TASK_STACK_SIZE, &task_args, 10, &th_data_process);

	return;
}
