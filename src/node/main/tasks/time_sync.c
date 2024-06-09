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

	uint16_t dev_id;
	c_pkt_type_t type;
	uint32_t epoch;

	while (1) {
		vTaskDelay(1);

		if (!c_lora_initialized) {
			continue;
		}

		err = c_proto_receive(&dev_id, &type, &epoch, NULL);
		if (err != ESP_OK) {
			ESP_LOGW(TAG, "Cannot parse packet");
			continue;
		}

		if (dev_id != EDGE_DEV_ID) {
			ESP_LOGI(TAG, "Packet not from edge server, dev_id = %hu", dev_id);
			continue;
		}

		if (type != C_PKT_TIMESYNC) {
			ESP_LOGI(TAG, "Ignoring packet, type = %u", type);
			continue;
		}

		c_proto_set_epoch(epoch);
		ESP_LOGI(TAG, "Updated epoch to %lu", epoch);
	}
}
