#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"
#include "config.h"
#include "lora.h"
#include "protocol.h"

static const char *TAG = "Task time sync";

void task_time_sync(task_args_t *task_args) {
	esp_err_t err;

	uint32_t epoch = 0;

	while (1) {
		vTaskDelay(C_DELAY_MS(TIMESYNC_DELAY));

		epoch++;
		c_proto_set_epoch(epoch);

		if (!c_lora_initialized) {
			continue;
		}

		err = c_proto_send(C_PKT_TIMESYNC, NULL, 0, true);
		if (err != ESP_OK) {
			ESP_LOGE(TAG, "Cannot send packet");
			continue;
		}

		ESP_LOGI(TAG, "Sent time sync signal, epoch = %lu", epoch);
	}
}
