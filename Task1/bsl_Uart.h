#ifndef __bsl_Usart_h
#define __bsl_Usart_h

#include <stdint.h>
#include <stdbool.h>

void bsl_Uart_Init(void);
char bsl_Uart_RxChar(void);
void bsl_Uart_TxChar(char c);

#endif