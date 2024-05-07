#ifndef N_I2C_H
#define N_I2C_H

#include <stdint.h>

esp_err_t n_i2c_init(void);

esp_err_t n_i2c_read(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len);
esp_err_t n_i2c_write(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len);

uint8_t n_i2c_read_u8(uint8_t addr, uint8_t reg);
uint16_t n_i2c_read_u16(uint8_t addr, uint8_t reg);

void n_i2c_write_u8(uint8_t addr, uint8_t reg, uint8_t value);

#endif /* end of include guard: N_I2C_H */
