#include <esp_err.h>
#include <esp_log.h>

#include "drivers/ra01s.h"

static const char *TAG = "LoRa";

/*
int16_t  ra01s_begin(uint32_t frequencyInHz, int8_t txPowerInDbm, float tcxoVoltage, bool useRegulatorLDO);
void     ra01s_config(uint8_t spreadingFactor, uint8_t bandwidth, uint8_t codingRate, uint16_t preambleLength, uint8_t payloadLen, bool crcOn, bool invertIrq);
*/

esp_err_t n_lora_init(void) {
	ra01s_init();

	err = ra01s_begin();
	if (err != ERR_NONE) {
		return ESP_FAIL;
	}

	ra01s_config();
	ra01s_set_debug(true);

	return ESP_OK;
}
