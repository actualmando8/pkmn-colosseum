#include "dolphin/types.h"

extern void fn_80003458(void* dst, s32 val, u32 len);

/* fn_800BEE74 - 0x800BEE74 | size 0x40 | scope none (TRKResetBuffer) */
void fn_800BEE74(u8* buf, s32 keepData) {
    *(u32*)(buf + 0x8) = 0;
    *(u32*)(buf + 0xC) = 0;
    if (keepData == 0) {
        fn_80003458(buf + 0x10, 0, 0x880);
    }
}
