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

#define MaxLineSize (40)
/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {
    bsl_Uart_Init();

    bool testRun = true;

    char temp[MaxLineSize][MaxLineSize];

    // Test AUnit
    /*PutS("AUnit on Arduino Nano v1.0\n");
    PutS("Usage: Enter <n> where n is the test number 1..");
    PutX4(TestMax); PutS(" or '0' (zero) to quit.\n");*/

    /*printf("Test AUnit on Arduino Nano v1.0\n");
    printf("Usage: Enter <n> where n is the test number 1..");
    printf("%d", TestMax); 
    printf(" or '0' (zero) to quit.\n");*/
    
    uint8_t dollarChecker = 1;

    while (testRun) {
        if (dollarChecker) {
            PutS("$ ");
            dollarChecker = 0;
        } else {
            dollarChecker = 1;
        }

        // ResetBuffer();
        // printf("$ ");
        uint8_t cmd = GetC();
        // uint8_t cmd = getchar();
        
        // printf("HERE: %d", charToU8(cmd));

        int cmdInt = charToU8(cmd);

        if (cmdInt == 218) {
            // PutS("\n");
            continue;
        }
        
        // printf("%d", cmdInt);
        
        if (cmd == '0') {
            // printf("HERE");
            break; // TODO: Will clean this up when it works.
        } else if (cmdInt >= 1 && cmdInt <= TestMax) {
            if (tests[cmdInt - 1] == NULL) {
                PutS("Test \"");
                // PutS(cmd + "");
                PutS(&cmd);
                PutS("\" not referred.\n");
                // printf("Test \"%d\" not referred.\n", cmdInt);
            } else {
                 // printf("HERE");
                 tests[cmdInt - 1]();
                 if(Equals()) {
                     // printf("DEBUG: In Equals");
                     PutS(".\n");
                     // printf(".\n");
                 } else {
                     PutS("F\n");
                     // printf("F\n");
                 }
            }
        } else if (cmdInt > TestMax) {
            // printf("HERE");
            PutS("Invalid test number. It should be 1..");
            // PutS(intToChar(TestMax) + "");
            // PutS(TestMax);
            PutS("or \"0\" (zero) to quit.\n");
            // printf("Invalid test number. It should be 1..%d or \"0\" (zero) to quit.\n", TestMax);
        }
        // printf("PRINTING...");
        // PrintBuffer();
    } 
    
    // PutS("bye!\n");
    
    // printf("bye!\n");
    return 0;
}