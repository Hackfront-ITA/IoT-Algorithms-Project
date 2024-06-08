#ifndef C_LORA_H
#define C_LORA_H

#include <stdbool.h>
#include <stdint.h>

#include <esp_err.h>

#include "utils.h"

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

extern bool c_lora_initialized;

esp_err_t c_lora_init(void);
esp_err_t c_lora_send(uint8_t *data, size_t len, bool async);
size_t c_lora_receive(uint8_t *buffer, size_t len);

#endif /* end of include guard: C_LORA_H */
