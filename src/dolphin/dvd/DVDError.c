#include "dolphin/dvd/dvd.h"

extern u32 ErrorCode2Num(u32 error);
extern void* __OSLockSramEx(void);
extern BOOL __OSUnlockSramEx(u32 offset);

void __DVDStoreErrorCode(u32 error) {
    u32 code;
    u32 severity;
    u32 errNum;
    void* sram;

    if (error == 0x01234567) {
        code = 0xFF;
    } else if (error == 0x01234568) {
        code = 0xFE;
    } else {
        severity = error >> 24;
        errNum = ErrorCode2Num(error & 0x00FFFFFF);

        if (severity >= 6) {
            severity = 6;
        }

        code = severity * 30 + (u8)errNum;
    }

    sram = __OSLockSramEx();
    ((u8*)sram)[0x24] = (u8)code;
    __OSUnlockSramEx(1);
}
