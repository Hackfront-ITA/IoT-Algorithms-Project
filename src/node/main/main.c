#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "config.h"
#include "fft.h"
#include "i2c.h"
#include "lora.h"
#include "protocol.h"
#include "utils.h"

#define N_TASK_STACK_SIZE  4096

#define N_NUM_AXIS   3
#define N_NUM_SLOTS  2

TaskHandle_t th_data_collect = NULL;
TaskHandle_t th_data_process = NULL;
TaskHandle_t th_time_sync = NULL;

static float accel_data[N_NUM_AXIS * N_NUM_SLOTS * DATA_NUM_SAMPLES];
static float fft_data[DATA_NUM_SAMPLES];

static const char *TAG = "Main";

static task_args_t task_args;

void app_main(void) {
	ESP_LOGI(TAG, "App start");

	ESP_LOGI(TAG, "I2C init");
	ESP_ERROR_CHECK(n_i2c_init());

	ESP_LOGI(TAG, "ADXL345 init");
	if (adxl345_init() != ESP_OK) {
		ESP_LOGE(TAG, "Cannot initialize ADXL345. Is accelerometer connected?");
		return;
	}

	ESP_LOGI(TAG, "FFT init");
	ESP_ERROR_CHECK(n_fft_init());
	ESP_ERROR_CHECK(n_fft_set_size(DATA_NUM_SAMPLES));

	task_args.accel_data_x = &accel_data[0 * DATA_NUM_SAMPLES];
	task_args.accel_data_y = &accel_data[1 * DATA_NUM_SAMPLES];
	task_args.accel_data_z = &accel_data[2 * DATA_NUM_SAMPLES];

	task_args.fft_data = fft_data;

	task_args.num_samples = DATA_NUM_SAMPLES;
	task_args.sampling_freq = DATA_SAMPLING_FREQ;

	ESP_LOGI(TAG, "Create data processing task");
	xTaskCreatePinnedToCore((TaskFunction_t)(task_data_process), "Data processing task",
		N_TASK_STACK_SIZE, &task_args, 20, &th_data_process, 1);

	ESP_LOGI(TAG, "Create data collecting task");
	xTaskCreatePinnedToCore((TaskFunction_t)(task_data_collect), "Data collecting task",
		N_TASK_STACK_SIZE, &task_args, 24, &th_data_collect, 0);

	ESP_LOGI(TAG, "Create time sync task");
	xTaskCreatePinnedToCore((TaskFunction_t)(task_time_sync), "Time sync task",
		N_TASK_STACK_SIZE, &task_args, 24, &th_time_sync, 1);

	ESP_LOGI(TAG, "Protocol init");
	ESP_ERROR_CHECK(c_proto_init());

	ESP_LOGI(TAG, "LoRa module init");
	if (c_lora_init() != ESP_OK) {
		ESP_LOGE(TAG, "Cannot initialize LoRa module.");
		return;
	}

	return;
}
