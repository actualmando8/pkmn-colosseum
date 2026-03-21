#include "dolphin/types.h"

/*
 * TRKBoard.c - TRK board-level support functions.
 *
 * Provides hardware-specific functions for the GameCube platform:
 * display output, EXI port reservation, program end trap, and
 * the UART interrupt handler stub.
 */

extern void OSReport(const char* fmt, ...);
extern void ICInvalidateRange(void* addr, u32 size);
extern void DCFlushRange(void* addr, u32 size);
extern void fn_80003488(void* dst, const void* src, u32 size);

/* Communication table */
extern u8 gDBCommTable[];

/* PPCHalt address - used for program end trap */
extern void PPCHalt(void);

/* EndofProgramInstruction - the trap instruction to install */
extern u32 EndofProgramInstruction;

/*
 * TRKUARTInterruptHandler - UART interrupt handler stub.
 * On GameCube, UART interrupts are handled via EXI callbacks,
 * so this is a no-op.
 */
void TRKUARTInterruptHandler(void) {
    /* empty - EXI callback handles this */
}

/*
 * InitializeProgramEndTrap - Install a trap instruction at PPCHalt+4.
 *
 * Copies the EndofProgramInstruction (a trap opcode) to the instruction
 * immediately after PPCHalt, then flushes the instruction and data caches
 * so the trap takes effect. This causes the debugger to catch program
 * termination.
 */
void InitializeProgramEndTrap(void) {
    void* trapAddr = (void*)((u32)PPCHalt + 4);

    fn_80003488(trapAddr, (void*)&EndofProgramInstruction, 4);
    ICInvalidateRange(trapAddr, 4);
    DCFlushRange(trapAddr, 4);
}

/*
 * TRK_board_display - Display a message via OSReport.
 * Used by the TRK welcome banner and diagnostic messages.
 */
void TRK_board_display(const char* msg) {
    OSReport("TRK: %s\n", msg);
}

/*
 * UnreserveEXI2Port - Release the EXI channel 2 port.
 * Calls the preContinue callback in the comm table, allowing
 * the target program to use EXI channel 2.
 */
void UnreserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[8]; /* offset 0x20 */
    func();
}

/*
 * ReserveEXI2Port - Reserve the EXI channel 2 port for TRK.
 * Calls the postStop callback in the comm table, taking control
 * of EXI channel 2 for debugger communication.
 */
void ReserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[9]; /* offset 0x24 */
    func();
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C3588 - 0x800C3588 | size: 0x3C */
void fn_800C3588(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r5 = (u32)gDBCommTable;
    r5 = (u32)gDBCommTable;
    r12 = *(u32*)((u8*)r5 + 0x14);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = -r3;
    tmp = tmp | r3;
    r3 = (s32)tmp >> 31;
    return;
}

/* fn_800C35C4 - 0x800C35C4 | size: 0x3C */
void fn_800C35C4(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r5 = (u32)gDBCommTable;
    r5 = (u32)gDBCommTable;
    r12 = *(u32*)((u8*)r5 + 0x10);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    tmp = -r3;
    tmp = tmp | r3;
    r3 = (s32)tmp >> 31;
    return;
}

/* fn_800C3600 - 0x800C3600 | size: 0x30 */
void fn_800C3600(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r12 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = (u32)gDBCommTable;
    r3 = (u32)gDBCommTable;
    r12 = *(u32*)((u8*)r3 + 0xC);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    return;
}

