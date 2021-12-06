#include "hal_Loader.h"
#include "../Task1/COutForAUnit.h"
#include "../Task1/bsl_Uart.h"
#include "../Task3/hal_kernel.h"

#define DATA (3)

// helper method to return the status
uint8_t getStatus(uint8_t cmd, uint8_t checksum, uint8_t size, uint8_t status){
    if (cmd == checksum){
        PutC((char)Ack);
        PutC((char)size);
        PutC((char)checksum);
        PutC((char)cmd);
        PutC((char)status);
        PutC((char)0);
        return status;
    } else {
        PutC((char)Nak);
        PutC((char)0);
        return ChecksumInvalid;
    }
}

// helper method to get the data that was sent
uint8_t getSendData(uint8_t cmd, uint8_t checksum, uint8_t size, uint8_t status, uint8_t address, uint8_t* mem){
    // To get the sum of cmd and data which will be compared with checksum
    uint8_t sum = 0;
    for(uint8_t i = (DATA - 1); i < PacketSizeMax; i++, address++){
        sum += packet[i];
    }
    if(sum == checksum){
        // To load data from packet buffer into target memory
        for(uint8_t index = DATA; index < PacketSizeMax; index++, address++){
            mem[address] = packet[index];
        }
        PutC((char)Ack);
        PutC((char)0);
        status = Success;
    } else {
        PutC((char)Nak);
        PutC((char)0);
        status = ChecksumInvalid;
    }
    return getStatus(cmd, checksum, size, status);
}


uint8_t hal_Loader(uint8_t* mem, uint8_t status, Kernel kInstance) {
    char rcdChar;
    uint8_t packIndex = 0;

    do {
        rcdChar = GetC(); // Recieved character from the serial port
        if (packIndex < PacketSizeMax) {
            packet[packIndex] = rcdChar; // Inserting into packet buffer
            packIndex++;
        }
    } while (rcdChar != 0);

    uint8_t size = packet[0];
    uint8_t checksum = packet[1];
    uint8_t cmd = packet[2];
    uint8_t address = 0;

    switch(cmd) {
        case GetStatus:
            return getStatus(cmd, checksum, size, status);
        case Download:
            // do not finish until all packets
            address = 0;
            return (cmd != checksum) ? ChecksumInvalid : Success;
        case SendData:
            return getSendData(cmd, checksum, size, status, address, mem);
        case Ping:
            if (cmd == checksum){
                PutC((char)Ack);
                PutC((char)0);
                return Success;
            } else {
                PutC((char)Nak);
                PutC((char)0);
                return ChecksumInvalid;
            }
        case Run:
            if (cmd != checksum) return ChecksumInvalid;
            PutC((char)Ack);
            PutC((char)0);
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
