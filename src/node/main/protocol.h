#ifndef C_PROTOCOL_H
#define C_PROTOCOL_H

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

#endif /* end of include guard: C_PROTOCOL_H */
