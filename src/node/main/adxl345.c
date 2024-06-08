#include <esp_err.h>
#include <esp_log.h>

#include <driver/i2c_master.h>
#include <stdint.h>

#include "adxl345.h"
#include "i2c.h"
#include "utils.h"

#define ADXL345_ADDRESS    0x53
#define ADXL345_DEVID      0xE5
// Accelerometer data rate is limited by I2C at SCL*2 / 1000 Hz
#define ADXL345_I2C_SCL_RATE_HZ 400000 // Fast mode

#define ADXL345_G_RANGE     4.0 	// +/- 2g
#define ADXL345_RESOLUTION  1024	// 10 bits resolution
#define mG_COEFF ADXL345_G_RANGE / ADXL345_RESOLUTION

static const char *TAG = "node_adxl345";
static i2c_master_dev_handle_t acc_handle;

esp_err_t adxl345_init(void) {

	esp_err_t ret;

	// Add accelerometer to i2c bus
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

	// Set resolution to +/- 2g
	uint8_t data_format_val = 0x0;
	ret = n_i2c_write(acc_handle, ADXL345_REG_DATA_FORMAT, &data_format_val, sizeof(uint8_t), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Cannot write to DATA_FORMAT register");
		return ret;
	}

	// Set output data rate to 400Hz
	uint8_t bw_rate_val = ADXL345_OUTPUT_DATA_RATE_400;
	ret = n_i2c_write(acc_handle, ADXL345_REG_BW_RATE, &bw_rate_val , sizeof(uint8_t), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Cannot write to BW_RATE register");
		return ret;
	}

	return ESP_OK;
}

esp_err_t adxl345_read_data(adxl345_g_data_t *value) {

	esp_err_t ret;
	uint8_t raw_data[6];

	// Reading raw data
	ret = n_i2c_read(acc_handle, ADXL345_REG_DATA, raw_data, sizeof(raw_data), -1);
	if (ret != ESP_OK) {
		ESP_LOGE(TAG, "Error reading from ADXL345, rc = %x", ret);
		return ret;
	}

	// Computing g values
	value->x = ((int)(raw_data[1] << 8 | raw_data[0])) * mG_COEFF;
	value->y = ((int)(raw_data[3] << 8 | raw_data[2])) * mG_COEFF;
	value->z = ((int)(raw_data[5] << 8 | raw_data[4])) * mG_COEFF;

	return ESP_OK;
}
