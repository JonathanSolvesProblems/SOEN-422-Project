#include "bsl_Uart.h" 

#if !defined(Host)
    #include <avr/io.h>
#endif


#include <stdbool.h>

#define BaudRate9600 ((uint16_t)(103))

static bool initialized = 0;

#if defined(Host)

#include <stdio.h>

    static char character;

    char bsl_Uart_RxChar(void) {
        scanf("%c", &character); // character = getchar(); 
 
        return character;
        
        /*while (character == '\0');

        char temp = character;
        character = '\0';
        return temp;*/
    }

    void bsl_Uart_TxChar(char c) { 
        // Intentionally left blank
    }

    void bsl_Uart_Init(void) {
        character = '\0';
        // puts(" bsl_Uart_Init(void)");
    }

#else
    // Receive a character from UART.
    char bsl_Uart_RxChar(void) {
        // Wait for data to be received before processing
        while (!(UCSR0A & (1<<RXC0)));
        // gets and returns the received data in buffer.
        return UDR0;
    }

    // Transmits character to UART.
    // NOTE: Function not used, implemented anyways for practice.
    void bsl_Uart_TxChar(char c) { 
        while (!(UCSR0A & (1 << UDRE0))); // wait for transmit buffer
        // put data into buffer, sends the data
        UDR0 = c;
    }

    // Initialize UART for polling.
    void bsl_Uart_Init(void) {
        if (!initialized) { // To make sure it will be done only once.
            // Set baud rate
            UBRR0 = BaudRate9600;
            // Enable receiver and transmiter.
            UCSR0B = (1<<RXEN0) | (1 << TXEN0);
            // sets the frame format with 8 data bits and 1 stop bit.
            UCSR0C = (1<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
            
            initialized = 1;
        }
    }

#endif

