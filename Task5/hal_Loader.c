#include "hal_Loader.h"
#include "../Task1/COutForAUnit.h"
#include "../Task1/bsl_Uart.h"
#include "../Task3/hal_kernel.h"

#define DATA (3)
#define KERNEL_SIZE (10)


uint8_t hal_Loader(uint8_t* mem) {
    char rcdChar;
    uint8_t packIndex = 0;

    do {
        rcdChar = GetC();
        if (packIndex < PacketSizeMax) {
            packet[packIndex] = rcdChar;
            packIndex++;
        }
    } while (rcdChar != 0);

    uint8_t size = packet[0];
    uint8_t checksum = packet[1];
    uint8_t cmd = packet[2];
    uint8_t sum = cmd;
    uint8_t address = 0;


    ResetBuffer();

    switch(cmd) {
        case GetStatus:
            return (cmd != checksum) ? ChecksumInvalid : Success;
        case SendData:
            for(uint8_t i = DATA; i < size; i++) {
                mem[i] = GetC();
                PutC((char)mem[i]);
                sum += mem[i];
            }
            return (sum != checksum) ? ChecksumInvalid : Success;
        case Ping:
            return (cmd != checksum) ? ChecksumInvalid : Success;
        case Run:
            if (cmd != checksum) return ChecksumInvalid;
            PutC((char)Ack);
            PutC((char)0);
            Kernel kInstance = createKernel(KERNEL_SIZE);
            loadInMemory(kInstance, mem);
            run(kInstance);
            kernelClearMemory(kInstance);
            ResetBuffer();
            return Success;
        default:
            return InvalidCmd;
    }
}
