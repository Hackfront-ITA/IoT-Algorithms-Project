#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"

static const char *TAG = "Task data collect";

void task_data_collect(task_args_t *task_args) {
	uint8_t active_slot = 0;

	float *accel_data_x = task_args->accel_data_x;
	float *accel_data_y = task_args->accel_data_y;
	float *accel_data_z = task_args->accel_data_z;

	size_t num_samples = task_args->num_samples;
	float sampling_freq = task_args->sampling_freq;

	uint16_t sampling_ticks = 1000 / (sampling_freq * portTICK_PERIOD_MS);

	while (1) {
		float *cur_data_x = &accel_data_x[active_slot * 3 * num_samples];
		float *cur_data_y = &accel_data_y[active_slot * 3 * num_samples];
		float *cur_data_z = &accel_data_z[active_slot * 3 * num_samples];

		adxl345_g_data_t value;

		for (size_t i = 0; i < num_samples; i++) {
			adxl345_read_data(&value);

			cur_data_x[i] = value.x;
			cur_data_y[i] = value.y;
			cur_data_z[i] = value.z;

			vTaskDelay(sampling_ticks);
		}

		active_slot = (active_slot + 1) % 2;

		vTaskResume(th_data_process);
	}
}
