#include <string.h>

#include <esp_err.h>
#include <esp_log.h>

#include "lora.h"
#include "protocol.h"
#include "utils.h"

#define PKT_MAGIC  0xd6d6

typedef struct {
	uint16_t magic;
	uint16_t dev_id;
	uint32_t epoch;
	c_pkt_type_t type;
} c_pkt_header_t;

static const char *TAG = "Protocol";

static uint16_t dev_id = 0xffff;
static uint32_t epoch = 1;

static uint8_t tx_buffer[128];
static c_pkt_header_t *pkt_header = (c_pkt_header_t *)(&tx_buffer[0]);
static uint8_t *pkt_payload = &tx_buffer[sizeof(c_pkt_header_t)];

esp_err_t c_proto_init(void) {
	dev_id = gen_dev_id();

	ESP_LOGI(TAG, "dev_id = %04x", dev_id);

	return ESP_OK;
}

esp_err_t c_proto_send(c_pkt_type_t type, uint8_t *payload, size_t pl_len, bool async) {
	size_t pkt_len = pl_len + sizeof(c_pkt_header_t);
	if (pkt_len > sizeof(tx_buffer)) {
		return ESP_FAIL;
	}

	pkt_header->magic = PKT_MAGIC;
	pkt_header->dev_id = dev_id;
	pkt_header->epoch = epoch;
	pkt_header->type = type;

	memcpy(pkt_payload, payload, pl_len);

	return c_lora_send(tx_buffer, pkt_len, async);
}

// esp_err_t c_proto_receive(void) {
// 	size_t rsize = c_lora_receive(rx_buffer, sizeof(rx_buffer));
//
// 	if (rsize < sizeof(c_pkt_header_t)) {
// 		return ESP_FAIL;
// 	}
//
// 	return ESP_OK;
// }

void c_proto_set_epoch(uint32_t new_epoch) {
	epoch = new_epoch;
}
