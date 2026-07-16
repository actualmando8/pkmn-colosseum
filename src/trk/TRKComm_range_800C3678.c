#include "dolphin/types.h"

extern void OSEnableScheduler(void);
extern void TRKLoadContext(void* ctx, u32 exceptionID);
extern void OSReport(const char* fmt, ...);
extern s32 Hu_IsStub(void);
extern s32 AMC_IsStub(void);

extern void ddh_cc_initialize(void);
extern void ddh_cc_initinterrupts(void);
extern void ddh_cc_shutdown(void);
extern void ddh_cc_peek(void);
extern void ddh_cc_read(void);
extern void ddh_cc_write(void);
extern void ddh_cc_open(void);
extern void ddh_cc_close(void);
extern void ddh_cc_pre_continue(void);
extern void ddh_cc_post_stop(void);

extern void gdev_cc_initialize(void);
extern void gdev_cc_initinterrupts(void);
extern void gdev_cc_shutdown(void);
extern void gdev_cc_peek(void);
extern void gdev_cc_read(void);
extern void gdev_cc_write(void);
extern void gdev_cc_open(void);
extern void gdev_cc_close(void);
extern void gdev_cc_pre_continue(void);
extern void gdev_cc_post_stop(void);

extern void udp_cc_initialize(void);
extern void udp_cc_shutdown(void);
extern void udp_cc_peek(void);
extern void udp_cc_read(void);
extern void udp_cc_write(void);
extern void udp_cc_open(void);
extern void udp_cc_close(void);
extern void udp_cc_pre_continue(void);
extern void udp_cc_post_stop(void);

extern u32 gDBCommTable[];
extern u8 TRK_Use_BBA;
extern char EndofProgramInstruction[];

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

s32 InitMetroTRKCommTable(s32 channel) {
    s32 result = 1;
    char* strings = EndofProgramInstruction;

    OSReport(strings + 0x08, channel);
    TRK_Use_BBA = 0;

    if (channel == 2) {
        OSReport(strings + 0x20);
        TRK_Use_BBA = 1;
        gDBCommTable[0] = (u32)udp_cc_initialize;
        gDBCommTable[6] = (u32)udp_cc_open;
        gDBCommTable[7] = (u32)udp_cc_close;
        gDBCommTable[4] = (u32)udp_cc_read;
        gDBCommTable[5] = (u32)udp_cc_write;
        gDBCommTable[2] = (u32)udp_cc_shutdown;
        gDBCommTable[3] = (u32)udp_cc_peek;
        gDBCommTable[8] = (u32)udp_cc_pre_continue;
        gDBCommTable[9] = (u32)udp_cc_post_stop;
        gDBCommTable[1] = 0;
        return 0;
    } else if (channel == 1) {
        OSReport(strings + 0x38);
        result = Hu_IsStub();
        gDBCommTable[0] = (u32)gdev_cc_initialize;
        gDBCommTable[6] = (u32)gdev_cc_open;
        gDBCommTable[7] = (u32)gdev_cc_close;
        gDBCommTable[4] = (u32)gdev_cc_read;
        gDBCommTable[5] = (u32)gdev_cc_write;
        gDBCommTable[2] = (u32)gdev_cc_shutdown;
        gDBCommTable[3] = (u32)gdev_cc_peek;
        gDBCommTable[8] = (u32)gdev_cc_pre_continue;
        gDBCommTable[9] = (u32)gdev_cc_post_stop;
        gDBCommTable[1] = (u32)gdev_cc_initinterrupts;
    } else if (channel == 0) {
        OSReport(strings + 0x5C);
        result = AMC_IsStub();
        gDBCommTable[0] = (u32)ddh_cc_initialize;
        gDBCommTable[6] = (u32)ddh_cc_open;
        gDBCommTable[7] = (u32)ddh_cc_close;
        gDBCommTable[4] = (u32)ddh_cc_read;
        gDBCommTable[5] = (u32)ddh_cc_write;
        gDBCommTable[2] = (u32)ddh_cc_shutdown;
        gDBCommTable[3] = (u32)ddh_cc_peek;
        gDBCommTable[8] = (u32)ddh_cc_pre_continue;
        gDBCommTable[9] = (u32)ddh_cc_post_stop;
        gDBCommTable[1] = (u32)ddh_cc_initinterrupts;
    } else {
        OSReport(strings + 0x80, channel);
        OSReport(strings + 0xAC);
        OSReport(strings + 0xDC);
    }

    return result;
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
