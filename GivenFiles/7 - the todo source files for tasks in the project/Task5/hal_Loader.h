// hal_Loader.h - Loader Interface

#ifndef __hal_Loader_h
#define __hal_Loader_h

#include <stdint.h>

#define Ack ((uint8_t)0xCC)
#define Nak ((uint8_t)0x33)

#define PacketSizeMax ((uint8_t)11)

typedef enum {
    Success = 0x40,
    // Errors during CheckPacket():
    UnknownCmd,
    InvalidCmd,

    // Errors before command execution:
    InvalidAddr,

    // Errors after command execution:
    MemoryFailed,
    ChecksumInvalid
} Status;

typedef enum {
    CmdBegin = 0x20,
    Ping = CmdBegin,
    Download,
    Run,
    GetStatus,
    SendData,
    Reset,
    CmdEnd
} Cmd;

uint8_t hal_Loader(uint8_t* mem);

#endif
