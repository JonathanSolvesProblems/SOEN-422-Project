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

