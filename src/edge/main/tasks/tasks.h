#ifndef E_TASKS_TASKS_H
#define E_TASKS_TASKS_H

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef struct {
} task_args_t;

void task_message_relay(task_args_t *task_args);
void task_time_sync(task_args_t *task_args);

extern TaskHandle_t th_message_relay;
extern TaskHandle_t th_time_sync;

#endif /* end of include guard: E_TASKS_TASKS_H */
