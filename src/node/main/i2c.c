#include <stdint.h>
#include <string.h>

#include <driver/i2c_master.h>
#include <esp_err.h>
#include <esp_log.h>

#include "i2c.h"
#include "utils.h"

#define N_I2C_PIN_SDA   47
#define N_I2C_PIN_SCL   48
#define N_I2C_CLK_FREQ  100000
#define N_I2C_MASTER_PORT 1

static const char *TAG = "I2C";
i2c_master_bus_handle_t bus_handle;

esp_err_t n_i2c_init(void) {

	i2c_master_bus_config_t master_bus_config = {
	    .clk_source = I2C_CLK_SRC_DEFAULT,
	    .i2c_port = N_I2C_MASTER_PORT,
	    .scl_io_num = N_I2C_PIN_SCL,
	    .sda_io_num = N_I2C_PIN_SDA,
	    .glitch_ignore_cnt = 7, // Suggested by docs
	    .flags.enable_internal_pullup = true,
	};

	esp_err_t ret;
	ESP_LOGI(TAG, "Allocating new i2c bus");
	ret = i2c_new_master_bus(&master_bus_config, &bus_handle);
	if(ret != ESP_OK) {
		ESP_LOGE(TAG, "Couldn't create I2C bus: %s (%d)", esp_err_to_name(ret), ret);
		return ret;
	}

	return ESP_OK;
}

esp_err_t n_i2c_add_device(i2c_device_config_t *dev_cfg, i2c_master_dev_handle_t *dev_handle, int timeout) {

	esp_err_t ret;
	uint8_t dev_address = dev_cfg->device_address;

	ESP_LOGI(TAG, "Adding device 0x%X to bus", dev_address);

	// Probe device
	ret = i2c_master_probe(bus_handle, dev_address, timeout);
	if(ret != ESP_OK) {
		ESP_LOGE(TAG, "Probing error: %s (%d)", esp_err_to_name(ret), ret);
		return ret;
	}

	// If everyting is OK then add to bus
	ret = i2c_master_bus_add_device(bus_handle, dev_cfg, dev_handle);
	if(ret != ESP_OK) {
		ESP_LOGE(TAG, "Couldn't add device on bus: %s (%d)", esp_err_to_name(ret), ret);
		return ret;
	}

	return ESP_OK;
}

esp_err_t n_i2c_read(i2c_master_dev_handle_t dev_handle, uint8_t reg, uint8_t *data, size_t len, int timeout) {

	return i2c_master_transmit_receive(dev_handle, &reg, 1, data, len, timeout);

}

esp_err_t n_i2c_write(i2c_master_dev_handle_t dev_handle, uint8_t reg, uint8_t *data, size_t len, int timeout) {

	uint8_t wbuf[len + 1];
	wbuf[0] = reg;
	memcpy(wbuf + 1, data, len);

	return i2c_master_transmit(dev_handle, wbuf, sizeof(wbuf), timeout);
}
