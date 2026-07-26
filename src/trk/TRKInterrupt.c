#include "dolphin/types.h"

typedef struct TRKExceptionStatus {
    u32 words[3];
    u8 pad_0C;
    u8 exceptionDetected;
    u8 pad_0E[2];
} TRKExceptionStatus;

extern TRKExceptionStatus gTRKExceptionStatus_80313824;
extern void TRK__read_aram(void* data, u32 address, u32* length);
extern void TRK__write_aram(void* data, u32 address, u32* length);

s32 TRKTargetAccessARAM(void* data, u32 address, u32* length, BOOL read)
{
    s32 error = 0;
    TRKExceptionStatus saved = gTRKExceptionStatus_80313824;

    gTRKExceptionStatus_80313824.exceptionDetected = FALSE;
    if (read) {
        TRK__read_aram(data, address, length);
    } else {
        TRK__write_aram(data, address, length);
    }
    if (gTRKExceptionStatus_80313824.exceptionDetected) {
        *length = 0;
        error = 0x702;
    }
    gTRKExceptionStatus_80313824 = saved;
    return error;
}
