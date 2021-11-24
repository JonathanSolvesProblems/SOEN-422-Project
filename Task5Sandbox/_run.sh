#!/usr/bin/bash
avr-gcc -Os -Wall -DF_CPU=16000000UL -mmcu=atmega328p bsl_UartByPolling.c COutForAUnit.c hal_Loader.c KernelAdt.c TaskAdt.c hal_TestLoader.c -o task5.o
avr-objcopy -O ihex -j .text -j .data task5.o  task5.hex
avrdude -c arduino -p atmega328p -b 57600 -P /dev/ttyUSB0 -D -Uflash:w:task5.hex:i
