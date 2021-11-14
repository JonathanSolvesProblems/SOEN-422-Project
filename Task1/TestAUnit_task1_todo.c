// TestAUnit_task1_todo.c - Test AUnit for Arduino Nano.

#include "COutForAUnit.h"

#include "bsl_Uart.h"

#include <stdint.h>
#include <stdbool.h>

#if defined(Host)
    #include <stdio.h>
#endif

static void Test1(void) {
    /*PutS("Test 1 - Test Number one\n");
    PutS("1\n"); // Expected output
    PutS("1\n"); // Current output*/
    printf("Test 1 - Test Number one\n");
    printf("1\n"); // Expected output
    printf("1\n"); // Current output
}
static void Test2(void) {
    /*PutS("Test 2 - Test Number two\n");
    PutS("23\n"); // Expected output
    PutS("24\n"); // Current output*/
    printf("Test 2 - Test Number two\n");
    printf("23\n"); // Expected output
    printf("24\n"); // Current output
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

/*-------------------------------------------------------------------
 * main
 *-------------------------------------------------------------------*/
int main(void) {
    bsl_Uart_Init();

    bool testRun = true;

    /*PutS("Test AUnit on Arduino Nano v1.0\n");
    PutS("Usage: Enter <n> where n is the test number 1..");
    PutX4(TestMax); PutS(" or '0' (zero) to quit.\n");*/

    printf("Test AUnit on Arduino Nano v1.0\n");
    printf("Usage: Enter <n> where n is the test number 1..");
    printf("%d", TestMax); 
    printf(" or '0' (zero) to quit.\n");

    uint8_t value;

    while (testRun) {
        // PutS("$ ");
        // char cmd = GetC();
        printf("$ ");

        scanf("%d", &value);

        if (value == 0) {
            break;
        } else if (value == 1 || value == 2 || value == 3 || value == 4 || value == 5 || value == 6 || value == 7
                    || value == 8 || value == 9) {
            if (tests[value - 1] == NULL) {
                printf("Test \"%d\" not referred.\n", value);
            } else {
                 tests[value - 1]();
                 if(Equals()) {
                     printf(".\n");
                 } else {
                     printf("F\n");
                 }
            }
        } else if (value > TestMax) {
            printf("Invalid test number. It should be 1..%d or \"0\" (zero) to quit.\n", TestMax);
        }

        // putBuffer(cmd);

        // if(Equals()) {
        //     PutS("")
        // } 
        
        // your code...
        // check if buffer[1] and buffer[2]

        // 

        testRun = true;
    }

    // PutS("bye!\n");
    printf("bye!\n");
    return 0;
}
