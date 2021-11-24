#include "hal_Loader.h"
#include "bsl_Uart.h"
#include "COutForAUnit.h"

uint8_t hal_Loader(uint8_t* mem) {
    uint8_t size = bsl_Uart_RxChar();
    uint8_t checksum = bsl_Uart_RxChar();
    uint8_t command = bsl_Uart_RxChar();
    uint8_t sum = command;
    for(uint8_t i = 0; i < size - 3; i++) {
        mem[i] = bsl_Uart_RxChar();
        PutC((char) mem[i]);
        sum += mem[i];
    }
    bsl_Uart_RxChar();
    switch (command) {
        case Ping:
            return (command != checksum) ? ChecksumInvalid : Success;
        case SendData:
            return (sum != checksum) ? ChecksumInvalid : Success;
        case GetStatus:
            return (command != checksum) ? ChecksumInvalid : Success;
        case Run:
            if (command != checksum) return ChecksumInvalid;
            PutC((char)Ack);
            PutC((char)0);
            Kernel k = Kernel_New();
            loadMem(k, mem);
            ResetBuffer();
            run(k);
            Kernel_Delete(k);
            ResetBuffer();
            return Success;
        default:
            return InvalidCmd;
    }
}
