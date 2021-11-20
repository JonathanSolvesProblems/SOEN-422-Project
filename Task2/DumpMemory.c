#include "DumpMemory.h"

void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint8_t n;
    for(n = 0; n < nBytes; n++){ // excluded for nBytes
        if(n%16 == 0){ // at the start of each register
            if(n == 16)
                PutN(); // indent between first and second register
            PutX16((uint16_t) n);
            PutC(' ');
        }
        // TODO: Implement offset.
        PutX8((uint8_t) &from[n]);
        PutC(' ');
    }
    PutN();
};
