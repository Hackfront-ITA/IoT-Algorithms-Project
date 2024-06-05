#include <stdint.h>

#include <driver/i2c.h>
#include <esp_err.h>
#include <esp_log.h>

#include "i2c.h"
#include "utils.h"

#define N_I2C_PIN_SDA   46
#define N_I2C_PIN_SCL   45
#define N_I2C_CLK_FREQ  100000
#define N_I2C_MASTER_PORT 1

static const char *TAG = "I2C";

esp_err_t n_i2c_init(void) {
	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = N_I2C_PIN_SDA,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_io_num = N_I2C_PIN_SCL,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = N_I2C_CLK_FREQ,
		// .clk_flags = 0,
	};

	esp_err_t result;

	result = i2c_param_config(N_I2C_MASTER_PORT, &conf);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Error applying configuration, rc = %x", result);
		return result;
	}

	result = i2c_driver_install(N_I2C_MASTER_PORT, conf.mode, 0, 0, 0);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Error installing driver, rc = %x", result);
		return result;
	}

	return ESP_OK;
}

uint8_t n_i2c_read_u8(uint8_t addr, uint8_t reg) {
	uint8_t value;
	ESP_ERROR_CHECK(n_i2c_read(addr, reg, &value, sizeof(uint8_t)));
	return value;
}

void n_i2c_write_u8(uint8_t addr, uint8_t reg, uint8_t value) {
	ESP_ERROR_CHECK(n_i2c_write(addr, reg, &value, sizeof(uint8_t)));
}

esp_err_t n_i2c_read(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg, true);

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_READ, true);
	i2c_master_read(cmd, msg, len, I2C_MASTER_LAST_NACK);
	i2c_master_stop(cmd);

	esp_err_t result;
	result = i2c_master_cmd_begin(N_I2C_MASTER_PORT, cmd, C_DELAY_MS(1000));
	i2c_cmd_link_delete(cmd);

	return result;
}

esp_err_t n_i2c_write(uint8_t addr, uint8_t reg, uint8_t *msg, size_t len) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, addr << 1 | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, reg, true);
	i2c_master_write(cmd, msg, len, true);
	i2c_master_stop(cmd);

	esp_err_t result;
	result = i2c_master_cmd_begin(N_I2C_MASTER_PORT, cmd, C_DELAY_MS(1000));
	i2c_cmd_link_delete(cmd);

	return result;
}
