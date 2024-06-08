#include <stddef.h>
#include <stdint.h>

#include <esp_log.h>

#include "tasks/tasks.h"
#include "config.h"
#include "lora.h"
#include "mqtt.h"
#include "protocol.h"

static const char *TAG = "Task data receive";

static c_pkt_payload_t pkt_payload;
static char m_topic[128];
static char m_payload[128];

void task_data_receive(task_args_t *task_args) {
	esp_err_t err;

	uint16_t dev_id;
	c_pkt_type_t type;
	uint32_t epoch;

	while (1) {
		if (!c_lora_initialized) {
			return;
		}

		err = c_proto_receive(&dev_id, &type, &epoch, (uint8_t *)(&pkt_payload));
		if (err != ESP_OK) {
			ESP_LOGW(TAG, "Cannot parse packet");
			continue;
		}

		uint16_t point_id = dev_id;

		switch (type) {
			case C_PKT_EMPTY:
				break;
			case C_PKT_EVENT:
				if (c_mqtt_connected) {
					snprintf(m_topic, sizeof(m_topic),
						"/buildings/" S_ESCAPE(BUILDING_ID) "/point/%d/event",
						point_id
					);

					snprintf(m_payload, sizeof(m_payload),
						S_ESCAPE({"axis": "%c", "frequency": "%.05f", "value": "%.05f"}),
						pkt_payload.event.axis, pkt_payload.event.frequency,
						pkt_payload.event.value
					);

					c_mqtt_publish(m_topic, m_payload);
				}
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
