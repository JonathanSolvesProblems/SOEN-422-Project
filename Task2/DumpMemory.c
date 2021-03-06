#include "DumpMemory.h"

void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint8_t n;
    for(n = 0; n < nBytes; n++){ // excluded for nBytes
        if(n%16 == 0){ // at the start of each register
            if(n == 16)
                PutN(); // indent between first and second register
            PutX16((uint16_t) *from);
            PutC(' ');
        }
        PutX8(*from++);
        PutC(' ');
    }
    PutN();
};
