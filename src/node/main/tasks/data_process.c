#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "config.h"
#include "fft.h"
#include "lora.h"
#include "protocol.h"
#include "utils.h"

#define OUTLIERS_THRESH   3

static const char *TAG = "Task data process";

static float fft_data[DATA_NUM_SAMPLES / 2];
static float z_data[DATA_NUM_SAMPLES / 2];

static void process_axis_data(float *fft_data, char axis, float sampling_freq,
	size_t num_samples);

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

		if (c_lora_initialized) {
			c_proto_send(C_PKT_HEARTBEAT, NULL, 0, false);
		}


		// Process axis data X
		float *cur_data_x = &accel_data_x[active_slot * 3 * num_samples];

		// View input data
		ESP_LOGI(TAG, "Processing axis data X");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_x, num_samples, 128, 10, -300, +300, '#');
		}

		// Compute fft and view data
		remove_mean(cur_data_x, num_samples);
		ESP_ERROR_CHECK(n_fft_execute(cur_data_x, fft_data));
		process_axis_data(fft_data, 'x', sampling_freq, num_samples);


		// Process axis data Y
		float *cur_data_y = &accel_data_y[active_slot * 3 * num_samples];

		ESP_LOGI(TAG, "Processing axis data Y");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_y, num_samples, 128, 10, -300, +300, '#');
		}

		remove_mean(cur_data_y, num_samples);
		ESP_ERROR_CHECK(n_fft_execute(cur_data_y, fft_data));
		process_axis_data(fft_data, 'y', sampling_freq, num_samples);


		// Process axis data Z
		float *cur_data_z = &accel_data_z[active_slot * 3 * num_samples];

		ESP_LOGI(TAG, "Processing axis data Z");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_z, num_samples, 128, 10, -300, +300, '#');
		}

		remove_mean(cur_data_z, num_samples);
		ESP_ERROR_CHECK(n_fft_execute(cur_data_z, fft_data));
		process_axis_data(fft_data, 'z', sampling_freq, num_samples);


		active_slot = (active_slot + 1) % 2;
	}
}

static void process_axis_data(float *fft_data, char axis, float sampling_freq, size_t num_samples) {
	if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(fft_data, num_samples / 2, 128, 10, 0, +100, '*');
	}

	calc_z_score(fft_data, z_data, num_samples / 2);

	if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(z_data, num_samples / 2, 128, 9, -4, +4, '=');
	}

	for (int i = 0; i < num_samples / 2; i++) {
		if (z_data[i] > OUTLIERS_THRESH) {
			c_pkt_event_t event = {
				.frequency = i * sampling_freq / num_samples,
				.value = fft_data[i],
				.axis = axis
			};

			printf("%3d -> %.3f: %.3f\n", i, event.frequency, event.value);

			if (c_lora_initialized) {
				c_proto_send(C_PKT_EVENT, (uint8_t *)(&event), sizeof(c_pkt_event_t), false);
			}
		}
	}
}
