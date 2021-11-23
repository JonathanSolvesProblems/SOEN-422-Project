#ifndef __hal_kernel_h
#define __hal_kernel_h

#include "bsl_vm.h"
#include <stdio.h> // for FILE


struct KernelDesc; // Forward reference
typedef struct KernelDesc* Kernel; // Opaque type

// Kernel createKernel(uint16_t size);
Kernel createKernel();
#if defined(Host)
void load(Kernel kInstance, FILE *input);
#else
void load(Kernel kInstance, int16_t input[]);
#endif
void run(Kernel kInstance);
void Kernel_Delete(Kernel k) {

#endif