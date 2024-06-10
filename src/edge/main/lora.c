#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_log.h>

#include "drivers/ra01s.h"
#include "lora.h"

// Standard frequencies: 169 MHz, 433 MHz, 470 MHz, 866 MHz, 915 MHz
#define C_LORA_FREQUENCY     868300000

// In dBm
#define C_LORA_TX_POWER      22

// TCXO yes:  VCC = 3.3, LDO = true
// TCXO no:   VCC = 0.0, LDO = false
#define C_LORA_TCXO_VCC       3.3
#define C_LORA_USE_LDO        true

#define C_LORA_SP_FACTOR      7
#define C_LORA_BANDWIDTH      4
#define C_LORA_CODING_RATE    1

bool c_lora_initialized = false;

static const char *TAG = "LoRa";

static SemaphoreHandle_t c_lora_busy = NULL;

esp_err_t c_lora_init(void) {
	esp_err_t err;

	ra01s_set_debug(true);

	err = ra01s_init();
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "RA01S init failed");
		return ESP_FAIL;
	}

	err = ra01s_begin(C_LORA_FREQUENCY, C_LORA_TX_POWER, C_LORA_TCXO_VCC, C_LORA_USE_LDO);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "RA01S begin failed");
		return ESP_FAIL;
	}

	err = ra01s_config(C_LORA_SP_FACTOR, C_LORA_BANDWIDTH, C_LORA_CODING_RATE, 8, 0, true, false);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "RA01S config failed");
		return ESP_FAIL;
	}

	c_lora_busy = xSemaphoreCreateBinary();
	if (c_lora_busy == NULL) {
		ESP_LOGE(TAG, "Cannot create semaphore");
		return ESP_FAIL;
	}
	xSemaphoreGive(c_lora_busy);

	c_lora_initialized = true;

	return ESP_OK;
}

esp_err_t c_lora_send(uint8_t *data, size_t len, bool async) {
	bool taken = xSemaphoreTake(c_lora_busy, C_DELAY_MS(10));
	if (!taken) {
		ESP_LOGE(TAG, "Error taking LoRa semaphore");
		return ESP_FAIL;
	}

	bool result = ra01s_send(data, len, async ? SX126x_TXMODE_ASYNC : SX126x_TXMODE_SYNC);

	if (!result) {
		ESP_LOGE(TAG, "Error in ra01s_send()");
		return ESP_FAIL;
	}

	xSemaphoreGive(c_lora_busy);

	return ESP_OK;
}

size_t c_lora_receive(uint8_t *buffer, size_t len) {
	bool taken = xSemaphoreTake(c_lora_busy, 0);
	if (!taken) {
		return 0;
	}

	size_t rsize = ra01s_receive(buffer, len);

	xSemaphoreGive(c_lora_busy);

	return rsize;
}
