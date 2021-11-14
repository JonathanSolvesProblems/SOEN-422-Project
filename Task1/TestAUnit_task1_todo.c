// TestAUnit_task1_todo.c - Test AUnit for Arduino Nano.

#include "COutForAUnit.h"

#include "bsl_Uart.h"

#include <stdint.h>
#include <stdbool.h>

#if defined(Host)
    #include <stdio.h>
#endif

static void Test1(void) {
    PutS("Test 1 - Test Number one\n");
    PutS("1\n"); // Expected output
    PutS("1\n"); // Current output
    /*printf("Test 1 - Test Number one\n");
    printf("1\n"); // Expected output
    printf("1\n"); // Current output*/
}
static void Test2(void) {
    PutS("Test 2 - Test Number two\n");
    PutS("23\n"); // Expected output
    PutS("24\n"); // Current output
    /*printf("Test 2 - Test Number two\n");
    printf("23\n"); // Expected output
    printf("24\n"); // Current output*/
}

typedef void (*TestEntry)(void);

#define TestMax  4  // Up to 9.

static TestEntry tests[TestMax] = {
    Test1,
    Test2,
//  Test3,
//  Test4,
//  ...
//  Test9
};

uint8_t charToU8(char c) {
    return (c - '0');
}

char intToChar(uint8_t u8) {
    return (u8 + '0');
}


/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {
    bsl_Uart_Init();

    bool testRun = true;

    /*PutS("Test AUnit on Arduino Nano v1.0\n");
    PutS("Usage: Enter <n> where n is the test number 1..");
    PutX4(TestMax); PutS(" or '0' (zero) to quit.\n");*/

    /*printf("Test AUnit on Arduino Nano v1.0\n");
    printf("Usage: Enter <n> where n is the test number 1..");
    printf("%d", TestMax); 
    printf(" or '0' (zero) to quit.\n");*/

    uint8_t reading = 0;

    while (testRun) {
        PutS("$ ");

        // ResetBuffer();
        // printf("$ ");
        
        char cmd = GetC();

        uint8_t cmdInt = charToU8(cmd);
        
        if (cmd == '0') {
            break;
        } else if (cmd == '1' || cmd == '2' || cmd == '3' || cmd == '4' || cmd == '5' || cmd == '6' || cmd == '7'
        || cmd == '8' || cmd == '9') {
            if (tests[cmdInt - 1] == NULL) {
                PutS("Test \"");
                PutS("cmd");
                PutS("\" not referred.\n");
                // printf("Test \"%d\" not referred.\n", cmdInt);
            } else {
                 tests[cmdInt - 1]();
                 if(Equals()) {
                     PutS(".\n");
                     // printf(".\n");
                 } else {
                     PutS("F\n");
                     // printf("F\n");
                 }
            }
        } else if (cmd > TestMax) {
            PutS("Invalid test number. It should be 1..");
            PutS("TestMax");
            PutS("or \"0\" (zero) to quit.\n");
            // printf("Invalid test number. It should be 1..%d or \"0\" (zero) to quit.\n", TestMax);
        }

        PrintBuffer();
        // break;
    }

    PutS("bye!\n");
    
    // printf("bye!\n");
    return 0;
}