// hal_TestLoader.c - Test Loader implementation for HAL using bsl_Uart.c
#include <stdbool.h>
#include "bsl_Uart.h"
#include "hal_Loader.h"
#include "COutForAUnit.h"

#define Target      "(ATMega328P)"
#define VMName      "Small Edison Virtual Machine "
#define AppSuffix   ""
#define AppName     "se"
#define Version     " v1.0 "
#define Copyright   "Copyright (c) 2021  Michel de Champlain"

// Banner = VMname AppSuffix Version Copyright
static void DisplayBanner() {
    PutS(VMName); 
    PutS(AppSuffix); 
    PutS(Version); 
    PutS(Target); 
    PutN();
    PutS(Copyright); 
    PutN();
}

#define MemMax 420

static uint8_t mem[MemMax];

int main() {
    bsl_Uart_Init();

    while (true) {
        uint8_t status = Success;
        // PutS("hello1");
        if ( (status = hal_Loader(mem)) == Success ) {
            // PutS("hello2");
            DisplayBanner();

            // kernel->run(mem);

            // Send an Ack to tell the Host that program's execution is done.
            PutC((char)Ack);
            PutC((char)0);
        } else {
            // PutS("hello3");
            PutS("Loader error: "); 
            PutX4(status); 
            PutN();
            return -1;
        }
    }
    return 0;
}
