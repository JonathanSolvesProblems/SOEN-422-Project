#include "hal_Loader.h"
#include "../Task1/COutForAUnit.h"
#include "../Task1/bsl_Uart.h"
#include "../Task3/hal_kernel.h"

#define DATA (3)
#define KERNEL_SIZE (10)


uint8_t hal_Loader(uint8_t* mem, uint8_t status) {
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
    uint8_t address = 0;

    

    switch(cmd) {
        case GetStatus:
            PutC((char)Ack);
            PutC((char)size);
            PutC((char)checksum);
            PutC((char)cmd);
            PutC((char)status);
            PutC((char)0);
            return status;
        case Download:
            // do not finish until all packets
            address = 0;
            return Success;
        case SendData:
            // PutC((char)Ack);
            // PutC((char)size);
            // PutC((char)checksum);
            // PutC((char)cmd);

            // PutC((char)0);

            return (cmd != checksum) ? ChecksumInvalid : Success;
        case Ping:
            PutC((char)Ack);
            PutC((char)0);
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

        case Reset:

            return Success;
        default:
            return InvalidCmd;
    }
}
