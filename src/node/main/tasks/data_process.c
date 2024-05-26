#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "config.h"
#include "fft.h"
#include "mqtt.h"
#include "utils.h"

static const char *TAG = "Task data process";

static float fft_data[DATA_NUM_SAMPLES / 2];

static void process_axis_data(float *fft_data, char axis, float sampling_freq);

void task_data_process(task_args_t *task_args) {
	uint8_t active_slot = 0;

	float *accel_data_x = task_args->accel_data_x;
	float *accel_data_y = task_args->accel_data_y;
	float *accel_data_z = task_args->accel_data_z;

	size_t num_samples = task_args->num_samples;
	float sampling_freq = task_args->sampling_freq;

	while (1) {
		vTaskSuspend(NULL);

		ESP_LOGI(TAG, "Task data_process resumed");

		if (n_mqtt_connected) {
			ESP_ERROR_CHECK(n_mqtt_publish(MQTT_BASE_TOPIC "/heartbeat", "ALIVE"));
		}

		float *cur_data_x = &accel_data_x[active_slot * num_samples];
		float *cur_data_y = &accel_data_y[active_slot * num_samples];
		float *cur_data_z = &accel_data_z[active_slot * num_samples];

		ESP_ERROR_CHECK(n_fft_execute(cur_data_x, fft_data));
		process_axis_data(fft_data, 'x', sampling_freq);

		ESP_ERROR_CHECK(n_fft_execute(cur_data_y, fft_data));
		process_axis_data(fft_data, 'y', sampling_freq);

		ESP_ERROR_CHECK(n_fft_execute(cur_data_z, fft_data));
		process_axis_data(fft_data, 'z', sampling_freq);

		active_slot = (active_slot + 1) % 2;
	}
}

static void process_axis_data(float *fft_data, char axis, float sampling_freq) {
	if (n_mqtt_connected) {
		ESP_LOGI(TAG, "Sending results");

		char buffer[16];
		snprintf(buffer, sizeof(buffer), "%.05f", 1.23456789);

		ESP_ERROR_CHECK(n_mqtt_publish(MQTT_BASE_TOPIC "/average", buffer));
	}
}
