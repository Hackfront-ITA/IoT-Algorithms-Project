#include <math.h>
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

float calc_mean(float *data, size_t len) {
	float mean = 0.0;

	for (size_t i = 0; i < len; i++) {
		mean += data[i];
	}

	mean /= len;

	return mean;
}

void calc_z_score(float *input, float *output, size_t len) {
	float mean = 0;
	for (size_t i = 0; i < len; i++) {
		mean += input[i];
	}
	mean /= len;

	float variance = 0;
	for (size_t i = 0; i < len; i++) {
		variance += pow(input[i] - mean, 2);
	}
	variance /= len;

	// Standard deviation
	float std_dev = sqrt(variance);

	// Compute z score for each frequency
	for (size_t i = 0; i < len; i++) {
		output[i] = (input[i] - mean) / std_dev;
	}
}

void remove_mean(float *data, size_t len) {
	float mean = calc_mean(data, len);

	for (size_t i = 0; i < len; i++) {
		data[i] -= mean;
	}
}
