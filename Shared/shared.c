#include "shared.h"

uint8_t charToU8(char c) {
    return (c - '0');
}

uint16_t charToU16(char c) {
    return (c - '0');
}

char intToChar(uint8_t u8) {
    return (u8 + '0');
}

char intToChar16(int16_t u16) {
    return (u16 + '0');
}

// void PutX4(uint8_t n) { 
//     n &= 0xF; // first four bits

//     if (n >= 10) {
//         n = n - 10 + 'A';
//     } else {
//         n = n + '0';
//     }
    
//     PutC(n);
// }
// /*---------------------------------------------------------------------------
//  * PutX8 - PutHexByte - print a byte (uint8_t) as two hex digit characters.
//  *-------------------------------------------------------------------------*/
// void ReturnX8(uint8_t b) {
//     PutX4(b >> ShiftByFour);
//     PutX4(b);
// }
// /*---------------------------------------------------------------------------
//  * PutX16 - PutHexWord - print a word (uint16_t) as four hex digit characters.
//  *-------------------------------------------------------------------------*/
// uint16_t ReturnX16(uint16_t w) {
//     PutX8((uint8_t)(w >> 8));
//     PutX8((uint8_t)w);
// }
