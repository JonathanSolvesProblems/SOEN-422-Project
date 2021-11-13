#include "DumpMemory.h"
#include "../Task1/COutForAUnit.h"
#include <stdint.h>


void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint16_t n;
    for(n = (uint16_t) *from; n < nBytes; n+=1){ // exclusive for nBytes
        PutX16(n);
    }
};
