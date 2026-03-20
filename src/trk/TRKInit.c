#include "dolphin/types.h"

/*
 * TRKInit.c - MetroTRK initialization and main entry.
 *
 * Contains the top-level initialization functions called from __start
 * (InitMetroTRK / InitMetroTRK_BBA) and the TRK main loop entry.
 * Also includes TRKInitializeTarget and TRKLoadContext which set up
 * exception vectors and resume target execution.
 */

extern void MWTRACE(s32 level, const char* fmt, ...);
extern void fn_800C0E60(void);     /* get current MSR value */
extern void fn_80003488(void* dst, const void* src, u32 size);
extern void fn_800C0D70(u32 addr, u32 size); /* flush/invalidate cache */
extern void ICInvalidateRange(void* addr, u32 size);
extern void DCFlushRange(void* addr, u32 size);
extern void PPCHalt(void);
extern void fn_800C3630(void);     /* EnableEXI2Interrupts */

extern void TRKSaveExtended1Block(void);
extern s32  InitMetroTRKCommTable(s32 channel);
extern s32  TRKInitializeNub(void);
extern void TRKNubWelcome(void);
extern void TRKNubMainLoop(void);
extern s32  TRKTerminateNub(void);
extern void TRKInterruptHandler(void);
extern void TRK_main(void);

/* Linker-defined debug stack */
extern u32 _db_stack_addr;

/* External data */
extern u8 gTRKCPUState[];
extern u8 gTRKState[];
extern s32 TRK_mainError;
extern u8 gTRKInterruptVectorTable[];
extern u8 gTRKInterruptVectorTableEnd[];
extern u32 lbl_803FED58;       /* exception table base address */
extern u8 lbl_80313848[];      /* interrupt vector offsets table */

/*
 * InitMetroTRK - Main TRK entry point (serial/EXI debug).
 *
 * Called from __start after basic system initialization.
 * Saves the entire CPU state, sets up the debug stack,
 * initializes the communication table, and enters TRK_main.
 *
 * This function is pure asm because it must save r0-r31 atomically
 * and manipulate SRR1/MSR for exception-safe initialization.
 */
asm void InitMetroTRK(register u32 debugArg) {
    nofralloc

    /* Save r3 on stack temporarily */
    subi    r1, r1, 4
    stw     r3, 0(r1)

    /* Save all GPRs to gTRKCPUState */
    lis     r3, gTRKCPUState@h
    ori     r3, r3, gTRKCPUState@l
    stmw    r0, 0(r3)

    /* Restore original r3 (debugArg) and fix stack */
    lwz     r4, 0(r1)
    addi    r1, r1, 4

    /* Save corrected SP and original r3 */
    stw     r1, 0x04(r3)       /* SP */
    stw     r4, 0x0C(r3)       /* original r3 = debugArg */

    /* Save LR as both CTR save and PC */
    mflr    r4
    stw     r4, 0x84(r3)       /* CTR slot */
    stw     r4, 0x80(r3)       /* SRR0/PC */

    /* Save CR */
    mfcr    r4
    stw     r4, 0x88(r3)

    /* Disable external interrupts in MSR */
    mfmsr   r4
    ori     r3, r4, 0x8000
    xori    r3, r3, 0x8000
    mtmsr   r3

    /* Save original MSR to SRR1 */
    mtsrr1  r4

    /* Save extended state */
    bl      TRKSaveExtended1Block

    /* Restore all GPRs from saved state */
    lis     r3, gTRKCPUState@h
    ori     r3, r3, gTRKCPUState@l
    opword  0xB8030000          /* lmw r0, 0(r3) - uses r0 destination */

    /* Clear IABR and DABR */
    li      r0, 0
    mtspr   IABR, r0
    mtspr   DABR, r0

    /* Switch to debug stack */
    lis     r1, _db_stack_addr@h
    ori     r1, r1, _db_stack_addr@l

    /* Initialize comm table (r5 = debugArg from __start) */
    mr      r3, r5
    bl      InitMetroTRKCommTable

    /* Check if init failed */
    cmpwi   r3, 1
    bne     @enter_main

    /* Failure: restore state and return to caller */
    lwz     r4, 0x84(r3)
    mtlr    r4
    opword  0xB8030000          /* lmw r0, 0(r3) */
    blr

@enter_main:
    b       TRK_main
}

/*
 * InitMetroTRK_BBA - TRK entry point for BBA (broadband adapter) debug.
 *
 * Identical to InitMetroTRK except it passes channel=2 to
 * InitMetroTRKCommTable and enables MSR[EE] instead of disabling it.
 */
asm void InitMetroTRK_BBA(void) {
    nofralloc

    subi    r1, r1, 4
    stw     r3, 0(r1)

    lis     r3, gTRKCPUState@h
    ori     r3, r3, gTRKCPUState@l
    stmw    r0, 0(r3)

    lwz     r4, 0(r1)
    addi    r1, r1, 4

    stw     r1, 0x04(r3)
    stw     r4, 0x0C(r3)

    mflr    r4
    stw     r4, 0x84(r3)
    stw     r4, 0x80(r3)

    mfcr    r4
    stw     r4, 0x88(r3)

    /* BBA variant: enable EE in MSR */
    mfmsr   r4
    ori     r3, r4, 0x8000
    mtmsr   r3

    mtsrr1  r4

    bl      TRKSaveExtended1Block

    lis     r3, gTRKCPUState@h
    ori     r3, r3, gTRKCPUState@l
    opword  0xB8030000          /* lmw r0, 0(r3) */

    li      r0, 0
    mtspr   IABR, r0
    mtspr   DABR, r0

    lis     r1, _db_stack_addr@h
    ori     r1, r1, _db_stack_addr@l

    /* BBA uses channel 2 */
    li      r3, 2
    bl      InitMetroTRKCommTable

    cmpwi   r3, 1
    bne     @enter_main_bba

    lwz     r4, 0x84(r3)
    mtlr    r4
    opword  0xB8030000          /* lmw r0, 0(r3) */
    blr

@enter_main_bba:
    b       TRK_main
    blr                         /* padding/unreachable */
}

/*
 * TRKInitializeTarget - Initialize target-specific state.
 *
 * Sets the stopped flag, reads the current MSR for the debugger's
 * saved state, and configures the exception table base address.
 */
s32 TRKInitializeTarget(void) {
    u32 msr;

    /* Mark target as stopped */
    *(s32*)&gTRKState[0x98] = 1;

    /* Get current MSR for debugger use */
    fn_800C0E60();
    msr = *(u32*)&gTRKState[0]; /* result returned in state */
    *(u32*)&gTRKState[0x8C] = msr;

    /* Set exception table base to 0xE0000000 (physical) */
    lbl_803FED58 = 0xE0000000;

    return 0;
}

/*
 * EnableMetroTRKInterrupts - Enable EXI2 interrupts for TRK communication.
 */
void EnableMetroTRKInterrupts(void) {
    fn_800C3630();
}

/*
 * TRK_main - TRK debugger main function.
 *
 * Initializes the nub, displays the welcome message, enters the
 * main event loop, then terminates when done.
 */
void TRK_main(void) {
    s32 err;

    MWTRACE(1, "TRK_main: starting\n");

    err = TRKInitializeNub();
    TRK_mainError = err;

    if (err == 0) {
        TRKNubWelcome();
        TRKNubMainLoop();
    }

    err = TRKTerminateNub();
    TRK_mainError = err;
}

/*
 * TRKLoadContext - Load a CPU context and enter the interrupt handler.
 *
 * Restores GPRs, SPRs, and special registers from the given context
 * structure, then branches to TRKInterruptHandler to resume execution
 * as if returning from an interrupt.
 *
 * This is pure asm because it restores the entire register file.
 */
asm void TRKLoadContext(register void* ctx, register u32 exceptionID) {
    nofralloc

    /* Load r0, r1, r2 from context */
    lwz     r0, 0x00(r3)
    lwz     r1, 0x04(r3)
    lwz     r2, 0x08(r3)

    /* Check bit 1 of status flags at offset 0x1A2 */
    lhz     r5, 0x1A2(r3)
    rlwinm. r6, r5, 0, 30, 30
    beq     @load_nonvolatile

    /* Full register restore: clear bit 1 and restore r5-r31 */
    rlwinm  r5, r5, 0, 31, 29
    sth     r5, 0x1A2(r3)
    lmw     r5, 0x14(r3)
    b       @restore_sprs

@load_nonvolatile:
    /* Partial restore: only r13-r31 */
    lmw     r13, 0x34(r3)

@restore_sprs:
    /* Save context pointer to r31 */
    mr      r31, r3
    mr      r3, r4              /* r3 = exceptionID */

    /* Restore CR from offset 0x80 */
    lwz     r4, 0x80(r31)
    mtcrf   255, r4

    /* Restore LR from offset 0x84 */
    lwz     r4, 0x84(r31)
    mtlr    r4

    /* Restore CTR from offset 0x88 */
    lwz     r4, 0x88(r31)
    mtctr   r4

    /* Restore XER from offset 0x8C */
    lwz     r4, 0x8C(r31)
    mtxer   r4

    /* Disable EE and RI in MSR */
    mfmsr   r4
    rlwinm  r4, r4, 0, 17, 15  /* clear EE (bit 16) */
    rlwinm  r4, r4, 0, 31, 29  /* clear RI (bit 30) */
    mtmsr   r4

    /* Save r2 to SPRG1 */
    mtsprg  1, r2

    /* Load SRR0 from offset 0x0C and save to SPRG2 */
    lwz     r4, 0x0C(r31)
    mtsprg  2, r4

    /* Load SRR1 from offset 0x10 and save to SPRG3 */
    lwz     r4, 0x10(r31)
    mtsprg  3, r4

    /* Load SRR0/SRR1 for rfi */
    lwz     r2, 0x198(r31)
    lwz     r4, 0x19C(r31)

    /* Restore r31 from context */
    lwz     r31, 0x7C(r31)

    /* Enter the interrupt handler */
    b       TRKInterruptHandler
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C2EAC - 0x800C2EAC | size: 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800C2EAC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800C3098 - 0x800C3098 | size: 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800C3098(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800C3218 - 0x800C3218 | size: 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800C3218(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800C3344 - 0x800C3344 | size: 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800C3344(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

