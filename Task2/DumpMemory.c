#include "DumpMemory.h"
#include "../Task1/COutForAUnit.h"
#include <stdint.h>

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
