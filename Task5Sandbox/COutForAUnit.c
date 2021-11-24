// COutForAUnit_task1_todo.c - COut for AUnit


#include <string.h>
#include "COutForAUnit.h"
#ifdef HOST
#include <stdio.h>
#else
#include "bsl_Uart.h"
#endif

#define nBuffers 3
#define BufferMax 40

// buffer[0] is the test name.
// buffer[1] is the expected output.
// buffer[2] is the current output.
static char buffer[nBuffers][BufferMax];

static uint8_t n = 0;         // index of where we are in buffer[bufferNum]
static uint8_t bufferNum = 0; // based on the newline detected. [0, 1, 2]
static bool overflow = false;

bool Equals(void)
{
    if (overflow) return false;
    // Comparing expected and current outputs.
    return strcmp(buffer[1], buffer[2]) == 0;
}

void ResetBuffer(void)
{
    for (uint8_t i = 0; i < nBuffers; i++)
    {
        memset(buffer[i], 0, BufferMax);
    }
    // Reset the overflow buffer
    overflow = false;
    bufferNum = 0;
    n = 0;
}

static void putBuffer(char c)
{
    // Check if c is a \n
    // If yes, bufferNum++, else proceed
    if (c == '\n')
    {
        bufferNum++;
        n = 0;
        return;
    }

    // Check if bufferNum is bigger than 3, then we loop back to 0 and reset buffers
    if (bufferNum >= nBuffers) ResetBuffer();

    // Check if n is reached to bufferMax, do nothing
    if (n >= BufferMax) {
        overflow = true;
        return;
    }

    // Everything is chill, we add c to the buffer
    buffer[bufferNum][n] = c;

    // Increment index for next character spot
    n++;
}

// AUnit's putchar to store output characters in AUnit's buffers.
static void __putchar(char c)
{
    putBuffer(c);
#ifdef HOST
    putchar(c);
#else
    bsl_Uart_TxChar(c);
#endif
}

#ifdef HOST
#define getchar() getchar()
#else
#define getchar() bsl_Uart_RxChar()
#endif

void PutC(char c)
{
    __putchar(c);
}
void PutS(const char *s)
{
    while (*s)
        PutC(*s++);
}
void PutN(void) { PutC('\n'); }

void PutI(uint16_t w){
    // 0000 0000 0000 0000
    uint8_t n = (w >> 12) & 0xF;
    n = n >= 10 ? n - 10 + 'A' : n + '0';
    if(n != '0') {
        PutC(n);
    }

    n = (w >> 8) & 0xF;
    n = n >= 10 ? n - 10 + 'A' : n + '0';
    if(n != '0') {
        PutC(n);
    }

    n = (w >> 4) & 0xF;
    n = n >= 10 ? n - 10 + 'A' : n + '0';
    if(n != '0') {
        PutC(n);
    }

    n = w & 0xF;
    n = n >= 10 ? n - 10 + 'A' : n + '0';
    PutC(n);

}

char GetC(void) { return getchar(); }

/*---------------------------------------------------------------------------
 * PutX4 - PutHexNibble - print a nibble as an hex digit character.
 *-------------------------------------------------------------------------*/
void PutX4(uint8_t n)
{
    PutC((n & 0xF) >= 10 ? (n & 0xF) - 10 + 'A' : (n & 0xF) + '0');
}
/*---------------------------------------------------------------------------
 * PutX8 - PutHexByte - print a byte (uint8_t) as two hex digit characters.
 *-------------------------------------------------------------------------*/
void PutX8(uint8_t b)
{
    PutX4((b >> 4) & 0xF);
    PutX4(b & 0xF);
}
/*---------------------------------------------------------------------------
 * PutX16 - PutHexWord - print a word (uint16_t) as four hex digit characters.
 *-------------------------------------------------------------------------*/
void PutX16(uint16_t w)
{
    PutX8((w >> 8) & 0xFF);
    PutX8(w & 0xFF);
}
