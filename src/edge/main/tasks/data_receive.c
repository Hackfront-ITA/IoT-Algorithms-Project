#include <stddef.h>
#include <stdint.h>

#include <esp_dsp.h>
#include <esp_log.h>

#include "tasks/tasks.h"
#include "adxl345.h"

static const char *TAG = "Task data receive";

static uint8_t rx_buffer[128];
static c_pkt_header_t *pkt_header = (c_pkt_header_t *)(&rx_buffer[0]);
static uint8_t *pkt_payload = &rx_buffer[sizeof(c_pkt_header_t)];

void task_data_receive(task_args_t *task_args) {
	while (1) {
		size_t rsize = c_lora_receive(rx_buffer, sizeof(rx_buffer));

		if (rsize < sizeof(c_pkt_header_t)) {
			continue;
		}

		switch (pkt_header->type) {
			case C_PKT_EMPTY:
				break;
			case C_PKT_EVENT:
				break;
			case C_PKT_HEARTBEAT:
				break;
			default:
				break;
		}

		vTaskDelay(1);
	}
	return;
}
