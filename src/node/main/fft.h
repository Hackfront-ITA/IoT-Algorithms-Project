#ifndef A_FFT_H
#define A_FFT_H

#include <stddef.h>

#include <esp_err.h>

esp_err_t n_fft_init(void);
esp_err_t n_fft_set_size(size_t size);
// esp_err_t n_fft_prepare(float *input);
esp_err_t n_fft_execute(float *input, float *output);
esp_err_t n_fft_destroy(void);
esp_err_t n_fft_get_outliers(float* input, float* output);

#endif /* end of include guard: A_FFT_H */
