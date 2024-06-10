#ifndef N_UTILS_H
#define N_UTILS_H

#include <stdint.h>

#define C_ABS(a,b)      ((a * a) + (b * b))
#define C_DELAY_MS(v)   ((v) / portTICK_PERIOD_MS)
#define C_LIN_TO_DB(l)  (10 * log10f(l))

#define S_ESCAPE(...)   #__VA_ARGS__

uint16_t gen_dev_id(void);

float calc_mean(float *data, size_t len);
void calc_z_score(float *input, float *output, size_t len);
void remove_mean(float *data, size_t len);

#endif /* end of include guard: N_UTILS_H */
