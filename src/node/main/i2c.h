#ifndef N_I2C_H
#define N_I2C_H

#include <stdint.h>
#include <driver/i2c_master.h>

esp_err_t n_i2c_init(void);

esp_err_t n_i2c_add_device(i2c_device_config_t *dev_cfg, i2c_master_dev_handle_t *dev_handle, int timeout);

esp_err_t n_i2c_read(i2c_master_dev_handle_t dev_handle, uint8_t reg, uint8_t *data, size_t len, int timeout);
esp_err_t n_i2c_write(i2c_master_dev_handle_t dev_handle, uint8_t reg, uint8_t *data, size_t len, int timeout);

#endif /* end of include guard: N_I2C_H */
