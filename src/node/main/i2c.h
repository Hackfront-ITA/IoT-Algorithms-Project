#ifndef N_I2C_H
#define N_I2C_H

#include <stdint.h>

esp_err_t n_i2c_init(void);

esp_err_t n_i2c_read(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len);
esp_err_t n_i2c_write(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len);

#endif /* end of include guard: N_I2C_H */
