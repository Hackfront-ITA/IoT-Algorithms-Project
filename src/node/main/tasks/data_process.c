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

		if (c_lora_initialized) {
			c_proto_send(C_PKT_HEARTBEAT, NULL, 0, false);
		}

		float *cur_data_x = &accel_data_x[active_slot * 3 * num_samples];
		float *cur_data_y = &accel_data_y[active_slot * 3 * num_samples];
		float *cur_data_z = &accel_data_z[active_slot * 3 * num_samples];

		ESP_LOGI(TAG, "Processing axis data X");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_x, DATA_NUM_SAMPLES, 128, 10, -15, +15, '#');
		}

		ESP_ERROR_CHECK(n_fft_execute(cur_data_x, fft_data));
		process_axis_data(fft_data, 'x', sampling_freq);

		ESP_LOGI(TAG, "Processing axis data Y");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_y, DATA_NUM_SAMPLES, 128, 10, -15, +15, '#');
		}

		ESP_ERROR_CHECK(n_fft_execute(cur_data_y, fft_data));
		process_axis_data(fft_data, 'y', sampling_freq);

		ESP_LOGI(TAG, "Processing axis data Z");
		if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
			dsps_view(cur_data_z, DATA_NUM_SAMPLES, 128, 10, -15, +15, '#');
		}

		ESP_ERROR_CHECK(n_fft_execute(cur_data_z, fft_data));
		process_axis_data(fft_data, 'z', sampling_freq);

		//Getting outliers
		ESP_LOGI(TAG, "Outliers on X");
		n_fft_compute_z_score(cur_data_x, cur_data_x);
		for(int i = 0; i < num_samples/2; i++) {
			if (cur_data_x[i] > 3) {
				printf("%d @ %f: %f\n", i, i * sampling_freq/num_samples, cur_data_x[i]);
			}
		}

		ESP_LOGI(TAG, "Outliers on Y");
		n_fft_compute_z_score(cur_data_y, cur_data_y);
		for(int i = 0; i < num_samples/2; i++) {
			if (cur_data_y[i] > 3) {
				printf("%d @ %f: %f\n", i, i * sampling_freq/num_samples, cur_data_y[i]);
			}
		}

		ESP_LOGI(TAG, "Outliers on Z");
		n_fft_compute_z_score(cur_data_z, cur_data_z);
		for(int i = 0; i < num_samples/2; i++) {
			if (cur_data_z[i] > 3) {
				printf("%d @ %f: %f\n", i, i * sampling_freq/num_samples, cur_data_z[i]);
			}
		}

		active_slot = (active_slot + 1) % 2;
	}
}

static void process_axis_data(float *fft_data, char axis, float sampling_freq) {
	if (esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(fft_data, DATA_NUM_SAMPLES / 2, 128, 10, 0, +100, '*');
	}

	if (c_lora_initialized) {
		ESP_LOGI(TAG, "Sending results");

		size_t index = rand() % (DATA_NUM_SAMPLES / 2);
		float value = (rand() % 60000) / 1000.0;  // fft_data[index];

		c_pkt_event_t event = {
			.frequency = index * sampling_freq / DATA_NUM_SAMPLES,
			.value = value,
			.axis = axis
		};

		c_proto_send(C_PKT_EVENT, (uint8_t *)(&event), sizeof(c_pkt_event_t), false);
	}
}
