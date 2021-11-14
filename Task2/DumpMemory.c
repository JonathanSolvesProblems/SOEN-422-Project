#include "DumpMemory.h"
#include <stdint.h>

 void PutC(char c)        { __putchar(c); }
    void PutS(const char* s) { while (*s) if (s != NULL) PutC(*s++); }

void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint8_t n;
    for(n = from; n < nBytes; n++){ // exclusive for nBytes
        PutS();

    }

    //  while (!(UCSR0A & (1 << UDRE0))); // wait for transmit buffer
    // UDR0 = c; //put data in buffer and transmit

   


};
