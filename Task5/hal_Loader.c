#include "hal_Loader.h"
#include "../Task1/COutForAUnit.h"
#include "../Task1/bsl_Uart.h"
#include "../sandbox/hal_kernel.h"

#define DATA (3)
#define KERNEL_SIZE (10)


uint8_t hal_Loader(uint8_t* mem) {
    uint8_t size = bsl_Uart_RxChar();
    uint8_t checksum = bsl_Uart_RxChar();
    uint8_t cmd = bsl_Uart_RxChar();
    uint8_t sum = cmd;

    for(uint8_t i = 0; i < size - DATA; i++) {
        mem[i] = bsl_Uart_RxChar();
        PutC((char)mem[i]);
        sum += mem[i];
    }

    // bsl_Uart_RxChar();

    switch(cmd) {
        case GetStatus:
            return (cmd != checksum) ? ChecksumInvalid : Success;
        case SendData:
            return (sum != checksum) ? ChecksumInvalid : Success;
        case Ping:
            return (cmd != checksum) ? ChecksumInvalid : Success;
        case Run:
            if (cmd != checksum) return ChecksumInvalid;
            PutC((char)Ack);
            PutC((char)0);
            Kernel kInstance = createKernel(KERNEL_SIZE);
            loadInMemory(kInstance, mem);
            ResetBuffer();
            run(kInstance);
            kernelClearMemory(kInstance);
            ResetBuffer();
            return Success;
        default:
            return InvalidCmd;
    }
}