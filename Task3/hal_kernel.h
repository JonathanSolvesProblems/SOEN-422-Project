#ifndef __hal_kernel_h
#define __hal_kernel_h

#include "bsl_vm.h"


struct KernelDesc; // Forward reference
typedef struct KernelDesc* Kernel; // Opaque type

Kernel Kernel_New(uint8_t size);
void load(FILE *input);

#endif