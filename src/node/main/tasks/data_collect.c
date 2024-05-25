#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "utils.h"

static const char *TAG = "Task data collect";

void task_data_collect(task_args_t *task_args) {
	uint8_t active_slot = 0;

	while (1) {
		size_t num_samples = task_args->num_samples;
		adxl345_data_t *accel_data = task_args->accel_data;
		float sampling_freq = task_args->sampling_freq;

		adxl345_data_t *cur_data = &accel_data[active_slot * num_samples];

		while (1) {
			adxl345_data_t value;

			adxl345_read_data(&value);

			printf("value_x = %hd\n", value.x);
			printf("value_y = %hd\n", value.y);
			printf("value_z = %hd\n", value.z);

			vTaskDelay(C_DELAY_MS(1000));
		}

		active_slot = (active_slot + 1) % 2;

		vTaskResume(th_data_process);
	}
}
