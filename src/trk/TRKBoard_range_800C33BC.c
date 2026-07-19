#include "dolphin/types.h"

extern void OSReport(const char* fmt, ...);

/* Comm table */
extern u32 gDBCommTable[];

/* "%s\n" */
extern char lbl_8026FB94[];

#if defined(TRK_BOARD_800C33BC_800C3414)

static s32 TRK_mainError;

/* TRK_main - 0x800C33BC | size 0x58 | scope global */
void TRK_main(void) {
    s32 error;
    extern void MWTRACE(s32 level, const char* fmt, ...);
    extern s32 TRKInitializeNub(void);
    extern void TRKNubWelcome(void);
    extern void TRKNubMainLoop(void);
    extern s32 TRKTerminateNub(void);

    MWTRACE(1, "TRK_Main \n");
    error = TRKInitializeNub();
    TRK_mainError = error;
    if (error == 0) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }
    error = TRKTerminateNub();
    TRK_mainError = error;
}

#endif

#if defined(TRK_BOARD_800C3414_800C349C)

/*
 * TRKLoadContext restores privileged processor state and remains target-owned
 * until its context-switch implementation can be represented authentically.
 */

#endif

#if defined(TRK_BOARD_800C349C_800C3588)

/* MetroTRK program-end marker copied immediately after PPCHalt. */
const u32 EndofProgramInstruction[] = { 0x00454E44 };

/* TRKUARTInterruptHandler - 0x800C349C | size 0x4 | scope global (empty, returns void) */
void TRKUARTInterruptHandler(void) {
}

/* InitializeProgramEndTrap - 0x800C34A0 | size 0x58 | scope global */
void InitializeProgramEndTrap(void) {
    extern void fn_80003488(void* dst, const void* src, u32 size);
    extern void ICInvalidateRange(void* addr, u32 size);
    extern void DCFlushRange(void* addr, u32 size);
    extern void PPCHalt(void);
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

#endif
