#ifndef __hal_kernel_h
#define __hal_kernel_h

#include "bsl_vm.h"
#include "hal_task.h"
#include <stdio.h> // for FILE
#include "../Task1/COutForAUnit.h"
#include "private_kernel_variables.h"


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

void kernelClearMemory(Kernel kInstance);

void loadInMemory(Kernel kInstance, uint8_t* memory);
// void Kernel_Delete(Kernel k) {

#endif