#include "dolphin/types.h"

/*
 * TRKSerial.c - TRK serial handler initialization and input processing.
 *
 * Sets up the serial communication state used by the debugger nub
 * for receiving messages from the host debugger.
 */

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void fn_800BE464(void* event, s32 type);
extern s32  fn_800BE47C(void* event);
extern s32  fn_800BF1FC(void);  /* TRKProcessInput - returns buffer index or -1 */
extern void* TRKGetBuffer(s32 index);

/* Serial handler state at lbl_803FE7B8 */
extern u8 lbl_803FE7B8[];

/* String table for serial handler at lbl_8026F688 */
extern char lbl_8026F688[];

/*
 * TRKInitializeSerialHandler - Set up the serial handler state.
 *
 * Clears the serial handler fields and logs initialization messages
 * via MWTRACE. The serial handler state is a small structure with:
 *   offset 0x00: current buffer index (set to -1)
 *   offset 0x08: read position (0)
 *   offset 0x0C: write position (0)
 */
s32 TRKInitializeSerialHandler(void) {
    u8* state = lbl_803FE7B8;
    char* strings = lbl_8026F688;

    /* Initialize serial state */
    ((s32*)state)[0] = -1; /* buffer index = none */
    ((s32*)state)[2] = 0;  /* offset 0x08: read pos */
    ((s32*)state)[3] = 0;  /* offset 0x0C: write pos */

    MWTRACE(1, strings + 0x00, 0x40);  /* string at +0x00 */
    MWTRACE(1, strings + 0x24, 0x40);  /* string at +0x24 */
    MWTRACE(1, strings + 0x48, 0x40);  /* string at +0x48 */
    MWTRACE(1, strings + 0x6C, 0x40);  /* string at +0x6C */
    MWTRACE(1, strings + 0x8C, 0x40);  /* string at +0x8C */
    MWTRACE(1, strings + 0xAC, 0x40);  /* string at +0xAC */

    return 0;
}

/*
 * TRKGetInput - Check for and process incoming debugger messages.
 *
 * Calls the low-level input processor. If a complete message was
 * received (buffer index != -1), constructs a message event and
 * posts it to the event queue.
 */
void TRKGetInput(void) {
    s32 bufIdx;

    bufIdx = fn_800BF1FC();

    if (bufIdx != -1) {
        u8 eventBuf[0x10];
        u8* state = lbl_803FE7B8;

        TRKGetBuffer(bufIdx);

        fn_800BE464((void*)eventBuf, 2); /* construct message event */

        /* Store buffer index in event at offset 0x08 */
        ((s32*)eventBuf)[2] = bufIdx;

        /* Reset serial state */
        ((s32*)state)[0] = -1;

        fn_800BE47C((void*)eventBuf); /* post event */
    }
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800BF14C - 0x800BF14C | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800BF14C(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r4 = 0x2;
    r31 = r3;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_800BE464)();
    r3 = (u32)&lbl_803FE7B8;
    r0 = -0x1;
    r4 = (u32)&lbl_803FE7B8;
    r3 = r1 + 0x8;
    *(u32*)((u8*)r4 + 0x0) = r0;
    ((void(*)(void))fn_800BE47C)();
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* fn_800BF33C - 0x800BF33C | size: 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800BF33C(void) {
    extern void fn_800C04E8();
    extern void fn_800C04F4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    r29 = r3;
    r3 = 0x0;
    goto L_800BF390;
L_800BF364: ;
    fn_800C04F4();
    *(u8*)(sp + 0x8) = r30;
    r30 = r3;
    r3 = 0x0;
    *(u8*)(sp + 0x9) = r31;
    fn_800C04E8();
    r3 = r1 + 0x8;
    OSReport();
    r3 = r30;
    fn_800C04E8();
    r3 = 0x0;
L_800BF390: ;
    if ((s32)r3 != (s32)0x0) goto L_800BF3A8;
    r0 = *(u8*)((u8*)r29 + 0x0);
    r29 = r29 + 0x1;
    r30 = (s8)r0;
    if ((s32)r3 != (s32)0x0) goto L_800BF364;
L_800BF3A8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

