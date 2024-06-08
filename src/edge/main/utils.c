#include <stdint.h>

#include <esp_err.h>
#include <esp_mac.h>

uint16_t gen_dev_id(void) {
	esp_err_t err;
	uint16_t dev_id = 0;

	uint8_t mac[8];
	uint16_t *buf = (uint16_t *)(&mac[0]);

	err = esp_read_mac(mac, ESP_MAC_WIFI_STA);
	if (err != ESP_OK) {
		return 0xffff;
	}

	dev_id = buf[0] + buf[1] + buf[2];

	return dev_id;
}
