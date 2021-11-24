avr-gcc -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p ../sandbox/hal_kernel.c ../Task3/hal_task.c ../Task1/COutForAUnit_task1_todo.c ../Task1/bsl_UsartByPolling.c  hal_TestLoader.c hal_Loader.c ../Shared/shared.c -o task3OnMicro.o
avr-objcopy -O ihex -j .text -j .data task3OnMicro.o task3OnMicro.hex
avrdude -c arduino -p atmega328p -b 57600 -P COM3 -D -Uflash:w:task3OnMicro.hex:i
pause
