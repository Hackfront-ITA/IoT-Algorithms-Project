#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

#include "i2c.h"
#include "utils.h"

#define TAG  "App main"

#define ADXL_ADDRESS    0x1D
#define ADXL_REG_DEVID  0x00

void app_main(void) {
  ESP_LOGI(TAG, "App start");

  ESP_ERROR_CHECK(n_i2c_init());
  ESP_LOGI(TAG, "I2C initialized successfully");

  uint8_t devid = n_i2c_read_u8(ADXL_ADDRESS, ADXL_REG_DEVID);
  printf("devid = %x\n", devid);

  while (1) {
    vTaskDelay(C_DELAY_MS(1000));
  }

  return;
}
