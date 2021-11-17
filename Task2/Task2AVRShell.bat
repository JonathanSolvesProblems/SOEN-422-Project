avr-gcc -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p DumpMemory.c TestSmallShell.c ../Task1/bsl_UsartByPolling.c ../Task1/COutForAUnit_task1_todo.c -o task2.o
avr-objcopy -O ihex -j .text -j .data task2.o task2.hex
avrdude -c arduino -p atmega328p -b 57600 -P COM3 -D -Uflash:w:task2.hex:i
pause