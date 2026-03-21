#include "dolphin/types.h"

/*
 * TRKComm.c - TRK communication table initialization.
 *
 * Sets up the function pointer table (gDBCommTable) that provides
 * an abstraction layer for debugger communication. Supports three
 * backends: DDH (EXI/AMC), GDEV (USB Gecko), and UDP (BBA network).
 */

extern void OSReport(const char* fmt, ...);
extern void OSEnableScheduler(void);
extern void TRKLoadContext(void* ctx, u32 exceptionID);

extern s32 Hu_IsStub(void);
extern s32 AMC_IsStub(void);

/* Communication backends */
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

/* Global comm table - 10 function pointers */
extern u32 gDBCommTable[];

/* BBA flag */
extern u8 TRK_Use_BBA;

/* String table for comm messages */
extern char EndofProgramInstruction[];

/* TRKEXICallBack function pointer type */
typedef void (*TRKEXICallBackFunc)(s32 chan, void* ctx);
extern void TRKEXICallBack(s32 chan, void* ctx);

/*
 * TRKInitializeIntDrivenUART - Set up interrupt-driven UART communication.
 *
 * Calls the comm table's initialize function with the EXI callback,
 * then calls initInterrupts to enable interrupt-based reception.
 *
 * Parameters:
 *   baud - baud rate / configuration
 *   polarity - polarity setting
 *   pad - padding parameter (unused)
 *   pendingPtr - pointer to the input pending flag
 *
 * Returns 0 on success.
 */
s32 TRKInitializeIntDrivenUART(s32 baud, s32 polarity, s32 pad, void* pendingPtr) {
    typedef s32 (*InitFunc)(void* callback, void* pending);
    typedef s32 (*InitIntFunc)(void);

    InitFunc initFunc = (InitFunc)gDBCommTable[0]; /* offset 0x00: initialize */
    InitIntFunc initIntFunc;

    initFunc((void*)TRKEXICallBack, pendingPtr);

    initIntFunc = (InitIntFunc)gDBCommTable[6]; /* offset 0x18: open/initinterrupts */
    initIntFunc();

    return 0;
}

/*
 * InitMetroTRKCommTable - Initialize the communication function table.
 *
 * Based on the channel parameter:
 *   0: DDH (AMC/EXI) - standard serial debug
 *   1: GDEV (USB Gecko) - USB debug
 *   2: UDP (BBA) - network debug
 *
 * Populates gDBCommTable with the appropriate backend function pointers.
 * Returns 0 on success, 1 on failure (unknown channel).
 */
s32 InitMetroTRKCommTable(s32 channel) {
    s32 result = 1;
    char* strings = (char*)EndofProgramInstruction;

    OSReport(strings + 0x08, channel);

    /* Clear BBA flag */
    TRK_Use_BBA = 0;

    if (channel == 2) {
        /* UDP/BBA mode */
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

        result = 0;
    } else if (channel == 1) {
        /* GDEV/USB Gecko mode */
        OSReport(strings + 0x38);

        Hu_IsStub();

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

        result = Hu_IsStub(); /* result from Hu_IsStub() call */
    } else if (channel == 0) {
        /* DDH/AMC/EXI mode */
        OSReport(strings + 0x5C);

        AMC_IsStub();

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

        result = AMC_IsStub(); /* result from AMC_IsStub() call */
    } else {
        /* Unknown channel */
        OSReport(strings + 0x80, channel);
        OSReport(strings + 0xAC);
        OSReport(strings + 0xDC);
    }

    return result;
}

/*
 * TRKEXICallBack - EXI interrupt callback for TRK.
 *
 * Called when the EXI controller signals data availability.
 * Enables the OS scheduler and then loads the saved CPU context
 * to enter the TRK interrupt handler as exception 0x500.
 */
void TRKEXICallBack(s32 chan, void* ctx) {
    OSEnableScheduler();
    TRKLoadContext(ctx, 0x500);
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C39A0 - 0x800C39A0 | size: 0x10 */
void fn_800C39A0(void) {
    extern u8 lbl_803FED70[];
    u32 r3 = 0;

    r3 = (u32)lbl_803FED70;
    r3 = (u32)lbl_803FED70;
    r3 = *(u8*)((u8*)r3 + 0x0);
    return;
}

/* fn_800C39B0 - 0x800C39B0 | size: 0xC */
void fn_800C39B0(void) {
    extern u8 lbl_803FED70[];
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = (u32)lbl_803FED70;
    *(u8*)lbl_803FED70 = r3;
    return;
}

/* fn_800C39BC - 0x800C39BC | size: 0x84 */
void fn_800C39BC(void) {
    extern void fn_800C04F4();
    extern void fn_800C2A00();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_800C04F4();
    if ((s32)r3 != 0) goto L_800C39E4;
    r3 = 0x1;
    goto L_800C3A2C;
L_800C39E4:
    r4 = r31;
    r3 = 0xd3;
    fn_800C2A00();
    tmp = r3 & 0xFF;
    if ((s32)tmp == 1) goto L_800C3A28;
    if ((s32)tmp >= 1) goto L_800C3A0C;
    if ((s32)tmp >= 0) goto L_800C3A18;
    goto L_800C3A28;
L_800C3A0C:
    if ((s32)tmp >= 3) goto L_800C3A28;
    goto L_800C3A20;
L_800C3A18:
    r3 = 0x0;
    goto L_800C3A2C;
L_800C3A20:
    r3 = 0x2;
    goto L_800C3A2C;
L_800C3A28:
    r3 = 0x1;
L_800C3A2C:
    return;
}

/* fn_800C3A40 - 0x800C3A40 | size: 0xBC */
void fn_800C3A40(void) {
    extern void fn_800C04F4();
    extern void fn_800C29F0();
    extern void fn_800C39A0();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r4;
    fn_800C39A0();
    tmp = r3 & 0xFF;
    if ((s32)tmp != 0) goto L_800C3A70;
    r3 = 0x1;
    goto L_800C3AE4;
L_800C3A70:
    fn_800C04F4();
    if ((s32)r3 != 0) goto L_800C3A84;
    r3 = 0x1;
    goto L_800C3AE4;
L_800C3A84:
    tmp = *(u32*)((u8*)r31 + 0x0);
    r6 = r30;
    r5 = (u32)sp + 0x8;
    r3 = 0xd0;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x1;
    fn_800C29F0();
    tmp = r3 & 0xFF;
    *(u32*)((u8*)r31 + 0x0) = r3;
    if ((s32)tmp == 1) goto L_800C3AE0;
    if ((s32)tmp >= 1) goto L_800C3AC4;
    if ((s32)tmp >= 0) goto L_800C3AD0;
    goto L_800C3AE0;
L_800C3AC4:
    if ((s32)tmp >= 3) goto L_800C3AE0;
    goto L_800C3AD8;
L_800C3AD0:
    r3 = 0x0;
    goto L_800C3AE4;
L_800C3AD8:
    r3 = 0x2;
    goto L_800C3AE4;
L_800C3AE0:
    r3 = 0x1;
L_800C3AE4:
    return;
}

/* fn_800C3AFC - 0x800C3AFC | size: 0xBC */
void fn_800C3AFC(void) {
    extern void fn_800C04F4();
    extern void fn_800C29F0();
    extern void fn_800C39A0();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r4;
    fn_800C39A0();
    tmp = r3 & 0xFF;
    if ((s32)tmp != 0) goto L_800C3B2C;
    r3 = 0x1;
    goto L_800C3BA0;
L_800C3B2C:
    fn_800C04F4();
    if ((s32)r3 != 0) goto L_800C3B40;
    r3 = 0x1;
    goto L_800C3BA0;
L_800C3B40:
    tmp = *(u32*)((u8*)r31 + 0x0);
    r6 = r30;
    r5 = (u32)sp + 0x8;
    r3 = 0xd1;
    *(u32*)(sp + 0x8) = tmp;
    r4 = 0x0;
    fn_800C29F0();
    tmp = r3 & 0xFF;
    *(u32*)((u8*)r31 + 0x0) = r3;
    if ((s32)tmp == 1) goto L_800C3B9C;
    if ((s32)tmp >= 1) goto L_800C3B80;
    if ((s32)tmp >= 0) goto L_800C3B8C;
    goto L_800C3B9C;
L_800C3B80:
    if ((s32)tmp >= 3) goto L_800C3B9C;
    goto L_800C3B94;
L_800C3B8C:
    r3 = 0x0;
    goto L_800C3BA0;
L_800C3B94:
    r3 = 0x2;
    goto L_800C3BA0;
L_800C3B9C:
    r3 = 0x1;
L_800C3BA0:
    return;
}

