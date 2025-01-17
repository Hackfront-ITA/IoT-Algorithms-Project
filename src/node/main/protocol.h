#ifndef C_PROTOCOL_H
#define C_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	C_PKT_EMPTY = 0x00,
	C_PKT_EVENT = 0x40,
	C_PKT_TIMESYNC = 0x41,
	C_PKT_HEARTBEAT = 0x42,
} c_pkt_type_t;

typedef struct {
	float frequency;
	float value;
	char axis;
} c_pkt_event_t;

typedef union {
	c_pkt_event_t event;
} c_pkt_payload_t;

esp_err_t c_proto_init(void);
esp_err_t c_proto_send(c_pkt_type_t type, uint8_t *payload, size_t pl_len,
	bool async);
esp_err_t c_proto_receive(uint16_t *dev_id, c_pkt_type_t *type, uint32_t *epoch,
	uint8_t *payload);
void c_proto_set_epoch(uint32_t new_epoch);
bool c_proto_is_valid_epoch(uint32_t epoch);

#endif /* end of include guard: C_PROTOCOL_H */
