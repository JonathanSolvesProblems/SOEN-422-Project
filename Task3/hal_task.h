#ifndef __hal_task_h
#define __hal_task_h

#include "bsl_vm.h"

struct TaskDesc; // Forward reference
typedef struct TaskDesc* Task; // Opaque type

Task Task_New();
Task Task_New(uint8_t size);

#endif