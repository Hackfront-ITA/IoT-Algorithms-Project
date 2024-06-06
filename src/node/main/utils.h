#ifndef N_UTILS_H
#define N_UTILS_H

#define C_ABS(a,b)      ((a * a) + (b * b))
#define C_DELAY_MS(v)   ((v) / portTICK_PERIOD_MS)
#define C_LIN_TO_DB(l)  (10 * log10f(l))

#define S_ESCAPE(...)   #__VA_ARGS__

uint16_t gen_dev_id(void);

#endif /* end of include guard: N_UTILS_H */
