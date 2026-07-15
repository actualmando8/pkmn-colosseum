/**
 * @file sdk_range_8009E7B0.c
 * @brief dolphin-sdk code, 0x8009E7B0 - 0x8009F1B8 (6 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct {
    u8 _unk_00[0x1C];
    u32 moduleType;
} OSModuleInfo;

extern BOOL Link(OSModuleInfo* module, void* data, BOOL isFixed);

BOOL fn_8009ED4C(OSModuleInfo* module, void* data) {
    return Link(module, data, 0);
}

BOOL OSLinkFixed(OSModuleInfo* module, void* data) {
    if (module->moduleType > 3 || module->moduleType < 3) {
        return FALSE;
    }
    return Link(module, data, 1);
}
