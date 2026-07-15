#include "dolphin/types.h"

extern void OSEnableScheduler(void);
extern void TRKLoadContext(void* ctx, u32 exceptionID);

/* Target-continue helpers */
extern void TRKTargetSetStopped(s32 stopped);
extern void UnreserveEXI2Port(void);
extern void ReserveEXI2Port(void);
extern void TRKSwapAndGo(void);

/* TRKInitializeIntDrivenUART - 0x800C3678 | size 0x50 | scope global */
s32 TRKInitializeIntDrivenUART(s32 baud, s32 polarity, s32 pad, void* pendingPtr) {
    typedef s32 (*InitFunc)(void* pending, void* callback);
    typedef s32 (*InitIntFunc)(void);
    extern u32 gDBCommTable[];
    extern void TRKEXICallBack(s32 chan, void* ctx);
    InitFunc initFunc = (InitFunc)gDBCommTable[0];
    InitIntFunc initIntFunc;

    initFunc(pendingPtr, (void*)TRKEXICallBack);
    initIntFunc = (InitIntFunc)gDBCommTable[6];
    initIntFunc();
    return 0;
}

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
