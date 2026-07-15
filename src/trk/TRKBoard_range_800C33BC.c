#include "dolphin/types.h"
#include "dolphin/os/OSContext.h"

extern void OSReport(const char* fmt, ...);

/* Comm table */
extern u32 gDBCommTable[];

/* "%s\n" */
extern char lbl_8026FB94[];

/* TRK_main - 0x800C33BC | size 0x58 | scope global */
void TRK_main(void) {
    s32 error;
    extern void MWTRACE(s32 level, const char* fmt, ...);
    extern s32 TRKInitializeNub(void);
    extern void TRKNubWelcome(void);
    extern void TRKNubMainLoop(void);
    extern s32 TRKTerminateNub(void);
    extern s32 TRK_mainError[];

    MWTRACE(1, "TRK_Main \n");
    error = TRKInitializeNub();
    TRK_mainError[0] = error;
    if (error == 0) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }
    error = TRKTerminateNub();
    TRK_mainError[0] = error;
}

/* TRKLoadContext - 0x800C3414 | size 0x88 | scope global */
void TRKLoadContext(register OSContext* context, register u32 vector) {
    extern void TRKInterruptHandler(void);

    asm {
        lwz r0, 0x0(context)
        lwz r1, 0x4(context)
        lwz r2, 0x8(context)
        lhz r5, 0x1a2(context)
        rlwinm. r6, r5, 0, 30, 30
        beq lbl_1
        rlwinm r5, r5, 0, 31, 29
        sth r5, 0x1a2(context)
        lmw r5, 0x14(context)
        b lbl_2
    lbl_1:
        lmw r13, 0x34(context)
    lbl_2:
        mr r3, vector
        lwz r4, 0x80(context)
        mtcrf 255, r4
        lwz r4, 0x84(context)
        mtlr r4
        lwz r4, 0x88(context)
        mtctr r4
        lwz r4, 0x8c(context)
        mtxer r4
        mfmsr r4
        rlwinm r4, r4, 0, 17, 15
        rlwinm r4, r4, 0, 31, 29
        mtmsr r4
        mtsprg 1, r2
        lwz r4, 0xc(context)
        mtsprg 2, r4
        lwz r4, 0x10(context)
        mtsprg 3, r4
        lwz r2, 0x198(context)
        lwz r4, 0x19c(context)
        lwz r30, 0x7c(context)
        b TRKInterruptHandler
    }
}

/* TRKUARTInterruptHandler - 0x800C349C | size 0x4 | scope global (empty, returns void) */
void TRKUARTInterruptHandler(void) {
}

/* InitializeProgramEndTrap - 0x800C34A0 | size 0x58 | scope global */
void InitializeProgramEndTrap(void) {
    extern void fn_80003488(void* dst, const void* src, u32 size);
    extern void ICInvalidateRange(void* addr, u32 size);
    extern void DCFlushRange(void* addr, u32 size);
    extern void PPCHalt(void);
    extern u32 EndofProgramInstruction[];
    u32* halt = (u32*)PPCHalt;

    fn_80003488(halt + 1, EndofProgramInstruction, 4);
    ICInvalidateRange(halt + 1, 4);
    DCFlushRange(halt + 1, 4);
}

/* TRK_board_display - 0x800C34F8 | size 0x30 | scope global */
void TRK_board_display(const char* msg) {
    OSReport(lbl_8026FB94, msg);
}

/* UnreserveEXI2Port - 0x800C3528 | size 0x30 | scope global */
void UnreserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[8]; /* offset 0x20 */
    func();
}

/* ReserveEXI2Port - 0x800C3558 | size 0x30 | scope global */
void ReserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[9]; /* offset 0x24 */
    func();
}
