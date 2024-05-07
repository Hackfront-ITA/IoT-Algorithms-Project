#include <esp_err.h>

#include "adxl345.h"
#include "i2c.h"
#include "utils.h"

#define ADXL345_ADDRESS    0x53
#define ADXL345_DEVID      0xE5

static const char *TAG = "node_adxl345";

esp_err_t adxl345_init(void) {
  esp_err_t result;
  result = n_i2c_init();
  if (result != ESP_OK) {
    ESP_LOGE(TAG, "Error initalizing I2C driver");
    return result;
  }

  uint8_t devid = n_i2c_read_u8(ADXL345_ADDRESS, ADXL345_REG_DEVID);
  if (devid != ADXL345_DEVID) {
    ESP_LOGE(TAG, "Device at %02x has unexpected devid (%02x)", devid);
    return ESP_FAIL;
  }

  uint8_t power_ctl = 0x08;
  n_i2c_write_u8(ADXL345_ADDRESS, ADXL345_REG_POWER_CTL, power_ctl);

  return ESP_OK;
}

void adxl345_read_data(adxl345_data_t *value) {
  ESP_ERROR_CHECK(n_i2c_read(ADXL345_ADDRESS, ADXL345_REG_DATA, value, sizeof(adxl345_data_t)));
}
