#ifndef N_LORA_H
#define N_LORA_H

#include <esp_err.h>

#define N_SPI_HOST_ID  SPI2_HOST
// #define N_SPI_HOST_ID  SPI3_HOST

#define N_SPI_PIN_MISO   11
#define N_SPI_PIN_MOSI   10
#define N_SPI_PIN_SCLK    9
#define N_SPI_PIN_NSS     8
#define N_SPI_PIN_RST    12
#define N_SPI_PIN_BUSY   13
#define N_SPI_PIN_TXEN   -1
#define N_SPI_PIN_RXEN   -1

esp_err_t n_lora_init(void);
esp_err_t n_lora_send(void);

#endif /* end of include guard: N_LORA_H */
