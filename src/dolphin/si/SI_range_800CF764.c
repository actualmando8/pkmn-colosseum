/**
 * @file SI_range_800CF764.c
 * @brief Dolphin SI (one TU proven by static GetTypeCallback linkage), 0x800CF764 - 0x800D0DF8.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

#pragma push
#pragma optimization_level 0
#pragma peephole off
#pragma scheduling off
typedef struct {
    u32 reg;
    u32 unk4;
    u32 unk8;
} SICommandQueueEntry;
#pragma scheduling reset
#pragma peephole reset
#pragma pop

void fn_800D0338(s32 chan, u32 command) {
    ((volatile SICommandQueueEntry*)0xCC006400)[chan].reg = command;
}

void fn_800D034C(void) {
    volatile u32* commandRegister;
    u32 command;

    command = 0x80000000u;
    *(commandRegister = (volatile u32*)0xCC006438) = command;
}
