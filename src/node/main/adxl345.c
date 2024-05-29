#include <esp_err.h>
#include <esp_log.h>

#include "adxl345.h"
#include "i2c.h"
#include "utils.h"

#define ADXL345_ADDRESS    0x53
#define ADXL345_DEVID      0xE5

#define ADXL345_MAX_VALUE_G      1000.0
#define ADXL345_MAX_VALUE_RAW    0x8000

static const char *TAG = "node_adxl345";

esp_err_t adxl345_init(void) {
	uint8_t devid = n_i2c_read_u8(ADXL345_ADDRESS, ADXL345_REG_DEVID);
	if (devid != ADXL345_DEVID) {
		ESP_LOGE(TAG, "Device at %02x has unexpected devid (%02x)",
			ADXL345_ADDRESS, devid);
		return ESP_FAIL;
	}

	uint8_t power_ctl = 0x08;
	n_i2c_write_u8(ADXL345_ADDRESS, ADXL345_REG_POWER_CTL, power_ctl);

	return ESP_OK;
}

esp_err_t adxl345_read_data(adxl345_norm_data_t *value) {
	esp_err_t result;

	adxl345_raw_data_t raw_data;

	result = n_i2c_read(ADXL345_ADDRESS, ADXL345_REG_DATA,
		(uint8_t *)(&raw_data), sizeof(adxl345_raw_data_t));

	if (result != ESP_OK) {
		ESP_LOGE(TAG, "Error reading from ADXL345, rc = %x", result);
		return result;
	}

	value->x = raw_data.x * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;
	value->y = raw_data.y * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;
	value->z = raw_data.z * ADXL345_MAX_VALUE_G / ADXL345_MAX_VALUE_RAW;

	return ESP_OK;
}
