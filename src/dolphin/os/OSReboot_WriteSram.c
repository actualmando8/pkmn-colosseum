#include "dolphin/exi/EXI.h"
#include "dolphin/types.h"

extern BOOL fn_80098368(s32 chan, u8* buf, s32 len, s32 mode);
extern void WriteSramCallback(s32 chan, OSContext* context);

u32 WriteSram(u8* dst, u32 addr, u32 len) {
    u32 cmd;
    u32 err;

    if (!EXILock(0, 1, WriteSramCallback)) {
        return 0;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return 0;
    }

    addr <<= 6;
    cmd = (addr + 0x100) | 0xA0000000;

    err = !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !fn_80098368(0, dst, len, 1);
    err |= !EXIDeselect(0);
    EXIUnlock(0);

    return !err;
}
