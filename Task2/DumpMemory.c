#include "DumpMemory.h"

void DumpMemory(uint8_t* from, uint16_t nBytes){
    #if !defined(Host)

    uint8_t n;
    for(n = 0; n < nBytes; n++){ // exclusive for nBytes
        if(n%16 == 0){
            PutN();
            PutX16((uint16_t) n);
            PutC(' ');
        }
        PutX8((uint8_t) from[n]);
        PutC(' ');
    }
    #endif
};
