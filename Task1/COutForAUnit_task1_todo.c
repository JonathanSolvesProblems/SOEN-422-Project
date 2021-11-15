/* COutForAUnit_task1_todo.c - COut for AUnit
*/

#include <string.h>
#include "COutForAUnit.h"

#include "bsl_Uart.h"

#if defined(Host)
    #include <stdio.h>
#endif

#define nBuffers  (3) 
#define BufferMax (40)
#define ShiftByFour (4)
#define ShiftByEight (8)

// buffer[0] is the test name.
// buffer[1] is the expected output.
// buffer[2] is the current output.
static char buffer[nBuffers][BufferMax];

static uint8_t  n = 0;
static uint8_t  bufferNum = 0; // based on the newline detected.
static bool     overflow = false;

// TESTING
bool StartsWith(const char* line, const char* with) {
    int withLength = strlen(with);
    return strncmp(line, with, withLength) == 0;
}

bool Equals(void) {
    if (overflow) return false; // TODO: Add message if overflowed?

    // printf("Buffer 0: %s\n", buffer[0]);
    // printf("Buffer 1: %s\n", buffer[1]);
    // printf("Buffer 2: %s\n", buffer[2]);

    // Comparing expected and current outputs.
    return strcmp(buffer[1], buffer[2]) == 0;
}

void PrintBuffer(void) {
    for(int i = 0; i < nBuffers; i++) {
        //for(int j = 0; j < BufferMax; j++) {
            printf("%s\n", buffer[i]);
        //}
        // printf("");
    }
}

void ResetBuffer(void) {
    /*for (int i = 0; i < nBuffers; i++) {
            buffer[i][0] = '\0';
    }
    n = 0;
    bufferNum = 0;*/
    memset(buffer, 0, sizeof(buffer) * n * bufferNum);
    bufferNum = 0;
    n = 0;
}

static void putBuffer(char c) {
    if (c == '\n') {
        // printf("\nTEST putBuffer \n: %s", buffer[bufferNum]);
        if (bufferNum < 2) {
             bufferNum++;
             putchar('\n');
             n = 0;
        } else {
            // ResetBuffer();
            putchar('\n');
            bufferNum = 0;
            n = 0;
        }
    } else {
        if (c == 'T') {
            ResetBuffer();
        }
        buffer[bufferNum][n] = c;
        // printf("%c", buffer[bufferNum][n]);
        putchar(buffer[bufferNum][n]);
        n++;
    }
}

// AUnit's putchar to store output characters in AUnit's buffers.
static void __putchar(char c) {
    bsl_Uart_TxChar(c);
    putBuffer(c);
}

#define getchar()  bsl_Uart_RxChar()

void PutC(char c)        { __putchar(c); }
void PutS(const char* s) { while (*s) if (s != NULL) PutC(*s++); }
void PutN(void)          { PutC('\n'); }

char GetC(void)          { return getchar(); }

/*---------------------------------------------------------------------------
 * PutX4 - PutHexNibble - print a nibble as an hex digit character.
 *-------------------------------------------------------------------------*/
void PutX4(uint8_t n) { 
    n &= 0xF; // first four bits

    if (n >= 10) {
        n = n - 10 + 'A';
    } else {
        n = n + '0';
    }
    
    PutC(n);
}
/*---------------------------------------------------------------------------
 * PutX8 - PutHexByte - print a byte (uint8_t) as two hex digit characters.
 *-------------------------------------------------------------------------*/
void PutX8(uint8_t b) {
    PutX4(b >> ShiftByFour);
    PutX4(b);
}
/*---------------------------------------------------------------------------
 * PutX16 - PutHexWord - print a word (uint16_t) as four hex digit characters.
 *-------------------------------------------------------------------------*/
void PutX16(uint16_t w) {
    PutX8((uint8_t)(w >> ShiftByEight));
    PutX8((uint8_t)w);
}
