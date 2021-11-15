#include "DumpMemory.h"

void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint8_t n;
    for(n = 0; n < nBytes; n++){ // exclusive for nBytes
        if(n%16 == 0){
            if(n == 16)
                PutN();
            PutX16((uint16_t) n);
            PutC(' ');
        }
        PutX8((uint8_t) from[n]);
        PutC(' ');
    }
    PutN();
};
