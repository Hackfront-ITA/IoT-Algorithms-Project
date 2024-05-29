#include <math.h>
#include <stdio.h>

#include <esp_dsp.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "config.h"
#include "utils.h"

#define FFT_DYN_ALLOC     0
#define FFT_STATIC_SIZE   DATA_NUM_SAMPLES
#define FFT_WINDOW_FUNC   dsps_wind_blackman_f32

static const char *TAG = "FFT";

static size_t fft_size = 0;

#if FFT_DYN_ALLOC == 1
static fc32_t *buffer = NULL;
static float *window = NULL;
#else
static fc32_t buffer[FFT_STATIC_SIZE];
static float window[FFT_STATIC_SIZE];
#endif

esp_err_t n_fft_init(void) {
	esp_err_t result;
	result = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "dsps_fft2r_init_fc32");
		return result;
	}

	return ESP_OK;
}

esp_err_t n_fft_set_size(size_t size) {
#if FFT_DYN_ALLOC == 1
	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}

	if (window != NULL) {
		free(window);
		window = NULL;
	}

	buffer = calloc(size, sizeof(fc32_t));
	if (buffer == NULL) {
		ESP_LOGE(TAG, "malloc buffer");
		return ESP_FAIL;
	}

	window = calloc(size, sizeof(float));
	if (window == NULL) {
		ESP_LOGE(TAG, "malloc window");

		free(buffer);
		buffer = NULL;

		return ESP_FAIL;
	}
#else
	if (size > FFT_STATIC_SIZE) {
		ESP_LOGE(TAG, "FFT size exceeding static buffer size");
		return ESP_FAIL;
	}
#endif

	fft_size = size;
	FFT_WINDOW_FUNC(window, fft_size);

	return ESP_OK;
}

esp_err_t n_fft_execute(float *input, float *output) {
	/*
	 * Array sizes
	 *  input: 2048
	 *    x0, x1, x2 ...
	 *  buffer before 3 FFT functions: 4096
	 *    x0, 0, x1, 0, x2, 0 ...
	 *  buffer after 3 FFT functions: 4096
	 *    Re(f0), Im(f0), Re(f1), Im(f1), ..., Re(g0), Im(g0), Re(g1), Im(g1), ...
	 *  output: 1024
	 *    abs(f0), abs(f1), ..., abs(fn)
	 */
	esp_err_t result;

#if FFT_DYN_ALLOC == 1
	if (buffer == NULL || window == NULL) {
		ESP_LOGE(TAG, "FFT not initialized");
		return ESP_FAIL;
	}
#endif

	for (int i = 0 ; i < fft_size; i++) {
		buffer[i].re = input[i] * window[i];
		buffer[i].im = 0;
	}

	result = dsps_fft2r_fc32((float *)(buffer), fft_size);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "dsps_fft2r_fc32");
		return result;
	}

	result = dsps_bit_rev_fc32((float *)(buffer), fft_size);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "dsps_bit_rev_fc32");
		return result;
	}

	result = dsps_cplx2reC_fc32((float *)(buffer), fft_size);
	if (result != ESP_OK) {
		ESP_LOGE(TAG, "dsps_cplx2reC_fc32");
		return result;
	}

	for (int i = 0; i < (fft_size / 2); i++) {
		float abs = C_ABS(buffer[i].re, buffer[i].im);
		float norm = abs / fft_size;

		output[i] = C_LIN_TO_DB(norm);
	}

	return ESP_OK;
}

void n_fft_get_outliers(float* input, float* output) {
	unsigned int samples_num = fft_size/2;

	unsigned int mean = 0;
	for (size_t i = 0; i < samples_num; i++) {
		mean += input[i] / samples_num;
	}

	int variance = 0;
	for (size_t i = 0; i < samples_num; i++) {
		variance += pow(input[i] - mean, 2) / samples_num;
	}

	// Standard deviation
	int std_dev = sqrt(variance);

	// Compute z score for each frequency
	for (size_t i = 0; i < samples_num; i++) {
		output[i] = (input[i] - mean) / std_dev;
	}
}

esp_err_t n_fft_destroy(void) {
#if FFT_DYN_ALLOC == 1
	if (buffer != NULL) {
		free(buffer);
	}

	if (window != NULL) {
		free(window);
	}
#endif

	return ESP_OK;
}
