/* COutForAUnit_task1_todo.c - COut for AUnit
*/

#include "COutForAUnit.h"

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

    // Comparing expected and current outputs.
    return strcmp(buffer[1], buffer[2]) == 0;
}

void ResetBuffer(void) {
    memset(buffer, 0, sizeof(buffer) * n * bufferNum);
    bufferNum = 0;
    n = 0;
}

#if defined(Host)
    static void putBuffer(char c) {
        if (c == '\n') {
            if (bufferNum < 2) {
                bufferNum++;
                putchar('\n');
                n = 0;
            } else {
                putchar('\n');
                bufferNum = 0;
                n = 0;
            }
        } else {
            if (c == 'T') {
                ResetBuffer();
            }
            buffer[bufferNum][n] = c;
            putchar(buffer[bufferNum][n]);
            n++;
        }
    }
#else
    static void putBuffer(char c) {
        if (c == '\n') {
            if (bufferNum < 2) {
                bufferNum++;
                n = 0;
            } else {
                bufferNum = 0;
                n = 0;
            }
        } else {
            if (c == 'T') {
                ResetBuffer();
            }
            buffer[bufferNum][n] = c;
            n++;
        }
    }
#endif



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
