#include <esp_err.h>
#include <esp_log.h>

#include <driver/i2c_master.h>

#include "adxl345.h"
#include "i2c.h"
#include "utils.h"

#define ADXL345_ADDRESS    0x53
#define ADXL345_DEVID      0xE5
#define ADXL345_I2C_SCL_RATE_HZ      100000 //100kHz

#define ADXL345_MAX_VALUE_G      2000.0
#define ADXL345_MAX_VALUE_RAW    0x8000

static const char *TAG = "node_adxl345";
static i2c_master_dev_handle_t acc_handle;

esp_err_t adxl345_init(void) {

	esp_err_t ret;

	// add device to i2c bus

	i2c_device_config_t acc_cfg = {
	    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
	    .device_address = ADXL345_ADDRESS,
	    .scl_speed_hz = ADXL345_I2C_SCL_RATE_HZ,
	};

	ret = n_i2c_add_device(&acc_cfg, &acc_handle, -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Couldn't add ADXL345 to I2C bus: %s (%d)", esp_err_to_name(ret), ret);
		return ret;
	}

	// Checking device id
	uint8_t devid_val;
	ret = n_i2c_read(acc_handle, ADXL345_REG_DEVID, &devid_val, sizeof(uint8_t), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Cannot read from device %02x", ADXL345_ADDRESS);
		return ret;
	}

	if (devid_val != ADXL345_DEVID) {
		ESP_LOGE(TAG, "Device at %02x has unexpected devid (%02x)",
			ADXL345_ADDRESS, devid_val);
		return ESP_FAIL;
	}

	// Enabling measurement
	uint8_t power_ctl_val = 0x08;
	ret = n_i2c_write(acc_handle, ADXL345_REG_POWER_CTL, &power_ctl_val, sizeof(uint8_t), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Cannot write to POWER_CTL register");
		return ret;
	}

	return ESP_OK;
}

esp_err_t adxl345_read_data(adxl345_norm_data_t *value) {

	esp_err_t ret;
	adxl345_raw_data_t raw_data;

	ret = n_i2c_read(acc_handle, ADXL345_REG_DATA,
		(uint8_t *)(&raw_data), sizeof(adxl345_raw_data_t), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Error reading from ADXL345, rc = %x", ret);
		return ret;
	}

	value->x = raw_data.x * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;
	value->y = raw_data.y * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;
	value->z = raw_data.z * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;

	return ESP_OK;
}
