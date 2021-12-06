avr-gcc -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p hal_kernel.c hal_task.c ../task1/COutForAUnit_task1_todo.c ../task1/bsl_UsartByPolling.c hal_TestLoader.c hal_Loader.c ../Shared/shared.c -o task5OnMicro.o
avr-objcopy -O ihex -j .text -j .data task5OnMicro.o task5OnMicro.hex
avrdude -c arduino -p atmega328p -b 57600 -P COM3 -D -Uflash:w:task5OnMicro.hex:i
pause
