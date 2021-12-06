/* TestDumpMemory.c - Test Dump Memory Function for Host and Arduino Nano. */

#include "DumpMemory.h"
#include "bsl_Uart.h"

/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {

    bsl_Uart_Init();

    #if !defined(Host)
        DumpMemory((uint8_t*)0, 32); // 32 registers on Nano.
    #else
        static uint8_t mem[] = {
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
            30, 31
        };
        DumpMemory(mem, 32); // Based 'mem' address (0x0100) on SRAM data.
    #endif
    
    return 0;
}
