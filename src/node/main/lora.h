#ifndef C_LORA_H
#define C_LORA_H

#include <esp_err.h>

#define C_SPI_HOST_ID  SPI2_HOST
// #define C_SPI_HOST_ID  SPI3_HOST

#define C_SPI_PIN_MISO   11
#define C_SPI_PIN_MOSI   10
#define C_SPI_PIN_SCLK    9
#define C_SPI_PIN_NSS     8
#define C_SPI_PIN_RST    12
#define C_SPI_PIN_BUSY   13
#define C_SPI_PIN_TXEN   -1
#define C_SPI_PIN_RXEN   -1

typedef enum {
	PKT_EMPTY = 0x00,
	PKT_EVENT = 0x45,
	PKT_HEARTBEAT = 0x48,
} c_pkt_type_t;

esp_err_t c_lora_init(void);
esp_err_t c_lora_send(void);

#endif /* end of include guard: C_LORA_H */
