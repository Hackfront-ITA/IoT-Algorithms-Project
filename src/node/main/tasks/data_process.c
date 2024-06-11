#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <esp_dsp.h>
#include <esp_log.h>
#include <esp_timer.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "config.h"
#include "fft.h"
#include "lora.h"
#include "protocol.h"
#include "utils.h"

#define DP_ENABLE_TIMING  0
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

#if DP_ENABLE_TIMING == 1
		int64_t start_time = esp_timer_get_time();
#endif

		if (c_lora_initialized) {
			c_proto_send(C_PKT_HEARTBEAT, NULL, 0, false);
		}

		float *cur_data_x = &accel_data_x[active_slot * 3 * num_samples];
		float *cur_data_y = &accel_data_y[active_slot * 3 * num_samples];
		float *cur_data_z = &accel_data_z[active_slot * 3 * num_samples];

		process_axis_data(cur_data_x, 'x', sampling_freq, num_samples);
		process_axis_data(cur_data_y, 'y', sampling_freq, num_samples);
		process_axis_data(cur_data_z, 'z', sampling_freq, num_samples);

		active_slot = (active_slot + 1) % 2;

#if DP_ENABLE_TIMING == 1
		int64_t end_time = esp_timer_get_time();
		int64_t time_delta_d = end_time - start_time;
		float time_delta_f = time_delta_d / 1000.0;

		ESP_LOGI(TAG, "Iteration took %.03f ms", time_delta_f);
#endif
	}
}

static void process_axis_data(float *cur_data, char axis, float sampling_freq,
		size_t num_samples)
{
	// View input data
#if DP_ENABLE_TIMING != 1
	ESP_LOGI(TAG, "Processing axis data %c", axis);
#endif
	if (!DP_ENABLE_TIMING && esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(cur_data, num_samples, 128, 10, -300, +300, '#');
	}

	// Compute FFT
	ESP_ERROR_CHECK(n_fft_execute(cur_data, fft_data));

	// View FFT data
	if (!DP_ENABLE_TIMING && esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(fft_data, num_samples / 2, 128, 10, 0, +100, '*');
	}

	// Compute z-score
	calc_z_score(fft_data, z_data, num_samples / 2);

	// View z-score data
	if (!DP_ENABLE_TIMING && esp_log_level_get(TAG) == ESP_LOG_INFO) {
		dsps_view(z_data, num_samples / 2, 128, 9, -4, +4, '=');
	}

	// Collapse group of adjacent outliers to median value. This is an attempt to reduce
	// the number of packages sent
	for (size_t i = 4; i < num_samples / 2; i++) {
		if (z_data[i] <= OUTLIERS_THRESH) {
			continue;
		}

		size_t head_index = i;
		size_t median_index;

		// If adjacent index is an outlier go forward until last occurrence
		while (
			((i + 1) < (num_samples / 2)) &&
			(z_data[i + 1] > OUTLIERS_THRESH)
		) {
			i++;
		}

		median_index = (i + head_index) / 2;

		c_pkt_event_t event = {
			.frequency = median_index * sampling_freq / num_samples,
			.value = fft_data[median_index],
			.axis = axis
		};

#if DP_ENABLE_TIMING != 1
		ESP_LOGI(TAG, "Outlier %3d -> %.3f: %.3f", i, event.frequency, event.value);
#endif

		if (c_lora_initialized) {
#if DP_ENABLE_TIMING == 1
			int64_t start_time = esp_timer_get_time();
#endif
			c_proto_send(C_PKT_EVENT, (uint8_t *)(&event), sizeof(c_pkt_event_t),
				false);

#if DP_ENABLE_TIMING == 1
			int64_t end_time = esp_timer_get_time();
			int64_t time_delta_d = end_time - start_time;
			float time_delta_f = time_delta_d / 1000.0;

			ESP_LOGI(TAG, "Message sending took %.03f ms", time_delta_f);
#endif
		}
	}
}
