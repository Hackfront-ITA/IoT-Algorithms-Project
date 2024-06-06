#ifndef C_PROTOCOL_H
#define C_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	C_PKT_EMPTY = 0x00,
	C_PKT_EVENT = 0x45,
	C_PKT_HEARTBEAT = 0x48,
} c_pkt_type_t;

typedef struct {
	float frequency;
	float value;
	char axis;
} c_pkt_event_t;

esp_err_t c_proto_init(void);
esp_err_t c_proto_send(c_pkt_type_t type, uint8_t *payload, size_t pl_len, bool async);
// esp_err_t c_proto_receive(void);

#endif /* end of include guard: C_PROTOCOL_H */
