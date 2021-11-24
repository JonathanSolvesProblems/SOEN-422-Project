// Task.h - ADT Queue of integers interface
#ifndef Task_h
#define Task_h

#include <stdbool.h>
#include <stdint.h>

struct TaskDesc; // Forward ref
typedef struct TaskDesc* Task; // Opaque type

Task Task_New(void);
void Task_Set(Task t, int8_t bp, int8_t sp, int8_t pe, int8_t ip);
int8_t Task_Get_bp(Task t);
int8_t Task_Get_sp(Task t);
int8_t Task_Get_pe(Task t);
int8_t Task_Get_ip(Task t);
void Task_Delete(Task t);
#endif /* Task_h */
