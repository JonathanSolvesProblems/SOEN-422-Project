/* TestDumpMemory.c - Test Dump Memory Function for Host and Arduino Nano.
//
// Copyright (C) 1985-2021 by Michel de Champlain
//
*/

#include "DumpMemory.h"

#if !defined(Host)
#include "bsl_Uart.h"
#endif

/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {
    static uint8_t mem[] = {
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31
    };

#if !defined(Host)
    bsl_Uart_Init();
    DumpMemory((uint8_t*)0, 32); // 32 registers on Nano.
#endif

    DumpMemory(mem, 32); // Based 'mem' address (0x0100) on SRAM data.

    return 0;
}
