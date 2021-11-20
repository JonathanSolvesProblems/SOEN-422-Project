avr-g++ -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p SmallEdisonVM_LoadTodo.cpp ../Task1/bsl_UsartByPolling.c ../Task1/COutForAUnit_task1_todo.c -o task3.o
avr-objcopy -O ihex -j .text -j .data task3.o task3.hex
avrdude -c arduino -p atmega328p -b 57600 -P COM3 -D -Uflash:w:task3.hex:i
pause