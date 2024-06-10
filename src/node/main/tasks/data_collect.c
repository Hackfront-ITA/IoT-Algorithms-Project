#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "freertos/projdefs.h"
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

	uint16_t sampling_period_ticks = 1000 / (sampling_freq * portTICK_PERIOD_MS);
	uint16_t fifo_fill_time_ticks = pdMS_TO_TICKS(ADXL345_FIFO_SIZE * 1000 / sampling_freq);

	while (1) {
		float *cur_data_x = &accel_data_x[active_slot * 3 * num_samples];
		float *cur_data_y = &accel_data_y[active_slot * 3 * num_samples];
		float *cur_data_z = &accel_data_z[active_slot * 3 * num_samples];

		adxl345_g_data_t value;

		// for (size_t i = 0; i < num_samples; i++) {
		// 	adxl345_read_data(&value);
		//
		// 	cur_data_x[i] = value.x;
		// 	cur_data_y[i] = value.y;
		// 	cur_data_z[i] = value.z;
		//
		// 	vTaskDelay(sampling_period_ticks);
		// }


		// Read block by block until having all the samples
		int read_blocks = num_samples / ADXL345_FIFO_SIZE;
		for (size_t block = 0; block < read_blocks; block++) {
			for(size_t j = 1; j <= ADXL345_FIFO_SIZE; j++) {
				adxl345_read_data(&value);

				cur_data_x[j*block] = value.x;
				cur_data_y[j*block] = value.y;
				cur_data_z[j*block] = value.z;

			}
			// Wait until fifo is full again
			vTaskDelay(fifo_fill_time_ticks);
		}

		active_slot = (active_slot + 1) % 2;

		if (th_data_process != NULL) {
			vTaskResume(th_data_process);
		}
	}
}
