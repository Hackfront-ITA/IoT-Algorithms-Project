#ifndef A_TASKS_TASKS_H
#define A_TASKS_TASKS_H

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "adxl345.h"

typedef struct {
	float *accel_data_x;
	float *accel_data_y;
	float *accel_data_z;

	float *fft_data;

	size_t num_samples;
	float sampling_freq;
} task_args_t;

void task_data_collect(task_args_t *task_args);
void task_data_process(task_args_t *task_args);
void task_time_sync(task_args_t *task_args);

extern TaskHandle_t th_data_collect;
extern TaskHandle_t th_data_process;
extern TaskHandle_t th_time_sync;

#endif /* end of include guard: A_TASKS_TASKS_H */
