#include <stdint.h>

#include <esp_err.h>
#include <esp_log.h>

#include "drivers/ra01s.h"
#include "lora.h"
#include "utils.h"

// Standard frequencies: 169 MHz, 433 MHz, 470 MHz, 866 MHz, 915 MHz
#define C_LORA_FREQUENCY     866000000

// In dBm
#define C_LORA_TX_POWER      22

// TCXO yes:  VCC = 3.3, LDO = true
// TCXO no:   VCC = 0.0, LDO = false
#define C_LORA_TCXO_VCC       0.0
#define C_LORA_USE_LDO        false

#define C_LORA_SP_FACTOR      7
#define C_LORA_BANDWIDTH      4
#define C_LORA_CODING_RATE    1

static const char *TAG = "LoRa";

bool c_lora_initialized = false;

esp_err_t c_lora_init(void) {
	ra01s_init();

	uint16_t err = ra01s_begin(C_LORA_FREQUENCY, C_LORA_TX_POWER, C_LORA_TCXO_VCC, C_LORA_USE_LDO);
	if (err != ERR_NONE) {
		ESP_LOGE(TAG, "RA01S begin failed");
		return ESP_FAIL;
	}

	ra01s_config(C_LORA_SP_FACTOR, C_LORA_BANDWIDTH, C_LORA_CODING_RATE, 8, 0, true, false);
	ra01s_set_debug(true);

	dev_id = gen_dev_id();

	c_lora_initialized = true;

	return ESP_OK;
}

esp_err_t c_lora_send(uint8_t data, size_t len, bool async) {
	bool result = ra01s_send(data, len, async ? SX126x_TXMODE_ASYNC : SX126x_TXMODE_SYNC);

	if (!result) {
		return ESP_FAIL;
	}

	return ESP_OK;
}

size_t c_lora_receive(uint8_t buffer, size_t len) {
	size_t rsize = ra01s_receive(buffer, len);

	return rsize;
}
