#include "dolphin/types.h"

extern void OSEnableScheduler(void);
extern void TRKLoadContext(void* ctx, u32 exceptionID);

/* Target-continue helpers */
extern void TRKTargetSetStopped(s32 stopped);
extern void UnreserveEXI2Port(void);
extern void ReserveEXI2Port(void);
extern void TRKSwapAndGo(void);

/* TRKEXICallBack - 0x800C3934 | size 0x38 | scope global */
void TRKEXICallBack(s32 chan, void* ctx) {
    OSEnableScheduler();
    TRKLoadContext(ctx, 0x500);
}

/* TRKTargetContinue - 0x800C396C | size 0x34 | scope global (backup TRKTarget.c) */
s32 TRKTargetContinue(void) {
    TRKTargetSetStopped(0);
    UnreserveEXI2Port();
    TRKSwapAndGo();
    ReserveEXI2Port();
    return 0;
}
