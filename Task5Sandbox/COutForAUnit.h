// COutForAUnit.h - header for AUnit
#ifndef __CoutForAUnit_h
#define __CoutForAUnit_h

#include <stdbool.h>
#include <stdint.h>

bool Equals(void);

void ResetBuffer(void);

void PutC(char c);

void PutS(const char *s);

void PutN(void);

char GetC(void);

void PutI(uint16_t w);

void PutX4(uint8_t n);

void PutX8(uint8_t b);

void PutX16(uint16_t w);

#endif