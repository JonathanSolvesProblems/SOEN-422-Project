#include "DumpMemory.h"
#include <stdint.h>

void PutC(char c)        { __putchar(c); }
void PutS(const char* s) { while (*s) if (s != NULL) PutC(*s++); }

void DumpMemory(uint8_t* from, uint16_t nBytes){
    #if !defined(Host)

    uint8_t n;
    for(n = 0; n < nBytes; n++){ // exclusive for nBytes
        if(n%16 == 0){
            PutN();
            PutX16((uint16_t) n);
            PutC('\t');
        }
    }

    #endif

};
