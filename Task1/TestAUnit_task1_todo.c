// TestAUnit_task1_todo.c - Test AUnit for Arduino Nano.

#include "COutForAUnit.h"

#include "bsl_Uart.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

static void Test1(void) {
    PutS("Test 1 - Test Number one\n");
    PutS("1\n"); // Expected output
    PutS("1\n"); // Current output
}
static void Test2(void) {
    PutS("Test 2 - Test Number two\n");
    PutS("23\n"); // Expected output
    PutS("24\n"); // Current output
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

    uint8_t testRun = 1;

        // Test AUnit
    PutS("AUnit on Arduino Nano v1.0\n");
    PutS("Usage: Enter <n> where n is the test number 1..");
    PutX4(TestMax); PutS(" or '0' (zero) to quit.\n");

    uint8_t dollarChecker = 1;

        while (testRun) {
            if (dollarChecker) {
                PutS("$ ");
                dollarChecker = 0;
            } else {
                dollarChecker = 1;
            }

            uint8_t cmd = GetC();

            int cmdInt = charToU8(cmd);

            if (cmdInt == 218) {
                continue;
            }
            
            if (cmd == '0') {
                break; 
            } else if (cmdInt >= 1 && cmdInt <= TestMax) {
                if (tests[cmdInt - 1] == NULL) {
                    PutS("Test \"");
                    PutC(cmd);
                    PutS("\" not referred.\n");
                } else {
                    tests[cmdInt - 1]();
                    if(Equals()) {
                        PutS(".\n");
                    } else {
                        PutS("F\n");
                    }
                }
            } else if (cmdInt > TestMax) {
                PutS("Invalid test number. It should be 1..");
                PutC(intToChar(TestMax));
                PutS(" or \"0\" (zero) to quit.\n");
            }
        } 
        
        PutS("bye!\n");

    return 0;
}