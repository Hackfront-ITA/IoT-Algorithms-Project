#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"

static const char *TAG = "Task time sync";

static uint8_t rx_buffer[128];
static c_pkt_header_t *rx_header = (c_pkt_header_t *)(&rx_buffer[0]);
static uint8_t *rx_payload = &rx_buffer[sizeof(c_pkt_header_t)];

void task_time_sync(task_args_t *task_args) {
	while (1) {
		vTaskDelay(1);

		// ...

		c_proto_set_epoch(12345);
	}
}
