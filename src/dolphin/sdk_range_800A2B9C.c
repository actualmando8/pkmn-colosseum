/**
 * @file sdk_range_800A2B9C.c
 * @brief dolphin-sdk code, 0x800A2B9C - 0x800A2C30 (3 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

extern void __init_cpp(void);
extern void PPCHalt(void);

void __init_user(void) {
    __init_cpp();
}

#pragma scheduling off
#pragma peephole off
static void __init_cpp(void) {
    FuncPtr* p;

    for (p = _ctors; *p != NULL; p++) {
        (*p)();
    }
}
#pragma peephole reset
#pragma scheduling reset

void _ExitProcess(void) {
    PPCHalt();
}
