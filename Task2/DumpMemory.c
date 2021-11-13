#include "DumpMemory.h"
#include <stdint.h>


void DumpMemory(uint8_t* from, uint16_t nBytes){
    uint8_t n;
    for(n = from; n < nBytes; n++){ // exclusive for nBytes
        puts(n);
    }
};
