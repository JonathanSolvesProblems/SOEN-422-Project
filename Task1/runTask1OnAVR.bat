avr-gcc -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p bsl_UsartByPolling.c COutForAUnit_task1_todo.c TestAUnit_task1_todo.c -o task1.o
avr-objcopy -O ihex -j .text -j .data task1.o task1.hex
avrdude -c arduino -p atmega328p -b 57600 -P COM3 -D -Uflash:w:task1.hex:i
pause
