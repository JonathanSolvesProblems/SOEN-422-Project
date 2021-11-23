#ifndef __hal_task_h
#define __hal_task_h

#include "bsl_vm.h"

struct TaskDesc; // Forward reference
typedef struct TaskDesc* Task; // Opaque type

Task createTask(uint16_t size);
Task **createDoubleArrayTask(uint16_t size);

uint16_t GetBp(Task t);
uint16_t GetSp(Task t);
uint16_t GetPe(Task t);
uint16_t GetIp(Task t);

void SetBp(Task t, uint16_t value);
void SetSp(Task t, uint16_t value);
void SetPe(Task t, uint16_t value);
void SetIp(Task t, uint16_t value);

#endif