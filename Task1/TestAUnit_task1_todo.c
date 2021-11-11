#include "COutForAUnit.h"
#include "bsl_Uart.h"
#include <stdint.h>
#include <stdbool.h>

static void Test1(void) { // This test will be a success (.)
PutS("Test 1 - Test Number one\n");
PutS("1\n"); // Expected output
PutS("1\n"); // Current output
}

static void Test2(void) { // This test will be a failure (F)
PutS("Test 2 - Test Number two\n");
PutS("23\n"); // Expected output
PutS("24\n"); // Current output
}
