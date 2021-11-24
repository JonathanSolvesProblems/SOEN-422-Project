#include "bsl_Uart.h"

#include <avr/io.h>
#include <stdbool.h>

#define BaudRate9600 ((uint16_t)(103))

static bool initialized;

// Receive a character from UART
char bsl_Uart_RxChar()
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));

    // Return data received from buffer
    return UDR0;
}

// Transmit a character to UART
void bsl_Uart_TxChar(char c)
{
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Put data into buffer and send the data
    UDR0 = c;
}

// Initialize UART for polling
void bsl_Uart_Init()
{
    if (!initialized)
    {
        // Set the baud rate
        UBRR0H = (unsigned char)(BaudRate9600 >> 8);
        UBRR0L = (unsigned char)BaudRate9600;

        // Enable receiver and transmitter
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);

        // Set frame format to 8 bit
        UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

        initialized = true;
    }
}