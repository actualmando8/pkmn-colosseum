#include "trk/trk.h"

extern void MWTRACE(s32 level, const char* format, ...);
extern u8 lbl_803FE7B8[];

TRKResult TRKInitializeSerialHandler(void)
{
    u8* state = lbl_803FE7B8;

    ((s32*)state)[0] = -1;
    ((s32*)state)[2] = 0;
    ((s32*)state)[3] = 0;

    MWTRACE(1, "TRK_Packet_Header \t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReadMemory     %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_WriteMemory    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_Connect \t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReplyAck\t    %ld bytes\n", 0x40);
    MWTRACE(1, "TRK_CMD_ReadRegisters\t%ld bytes\n", 0x40);

    return kTRKSuccess;
}
