/* COutForAUnit.h - COut for AUnit Interface
//
// Copyright (C) 2021 by Michel de Champlain
//
*/

#ifndef __COutForAUnit_h
#define __COutForAUnit_h

#include <stdint.h>
#include <stdbool.h>

void ResetBuffer(void);
bool Equals(void);

char GetC(void);

void PutC(char c);
void PutS(const char* s);
void PutN(void);

void PutX4(uint8_t n);
void PutX8(uint8_t b);
void PutX16(uint16_t w);

// TESTING
void PrintBuffer(void);

#endif
