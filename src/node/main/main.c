#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_log.h>

#include "adxl345.h"
#include "utils.h"

#define TAG  "App main"

void app_main(void) {
  ESP_LOGI(TAG, "App start");

  ESP_ERROR_CHECK(adxl345_init());
  ESP_LOGI(TAG, "ADXL345 initialized successfully");

  while (1) {
    adxl345_data_t value;

    adxl345_read_data(&value);

    printf("value_x = %hd\n", value.x);
    printf("value_y = %hd\n", value.y);
    printf("value_z = %hd\n", value.z);

    vTaskDelay(C_DELAY_MS(1000));
  }

  return;
}
