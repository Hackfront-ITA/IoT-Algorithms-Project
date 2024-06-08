#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "protocol.h"

static const char *TAG = "Task time sync";

void task_time_sync(task_args_t *task_args) {
	while (1) {
		vTaskDelay(1);

		// ...

		c_proto_set_epoch(12345);
	}
}
