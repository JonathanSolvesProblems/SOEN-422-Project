/* TestSmallShell.c - Test Small Shell for Host and Arduino Nano. */

#include "DumpMemory.h"
#include "bsl_Uart.h"

/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {

    bsl_Uart_Init();

    static uint8_t mem[] = {
        0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
        30, 31
    };

    uint8_t testRun = 1;

    while(testRun){
        // Initial msg
        PutS("Shell Nano v1.0\nUsage: type 'm'(memory) and 'q' to quit.\n");

        // Get Input
        PutC('$');
        char cmd = GetC();
        PutC(cmd);
        PutN();

        if(cmd == 'm'){
            #if !defined(Host)
                // DumpMemory((uint8_t*)0, 32); // 32 registers on Nano.
                DumpMemory(mem, 32); // 32 registers on Nano.
            #else
                DumpMemory(mem, 32); // Based 'mem' address (0x0100) on SRAM data.
            #endif

        } else if(cmd == 'q'){
            PutS("Bye!");
            testRun = 0;
        } else {
            PutS("Invalid command.");
        }
        PutN();
        PutN();
    }
    return 0;
}
