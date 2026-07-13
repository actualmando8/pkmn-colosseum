/**
 * @file gs_range_8017A5FC.c
 * @brief gs-engine code, 0x8017A5FC - 0x8017B07C (9 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

#include "game/fsys/fsys.h"

/* Address: 0x8017A5FC | size: 0x28 */
extern FSYSManager lbl_80453FEC;
#pragma optimization_level 0
void fn_8017A5FC(void)
{
    FSYSSlot* slot;

    slot = lbl_80453FEC.activeSlot;
    slot->status = 0x12f;
}
#pragma optimization_level reset

u32 fn_8017AC30(void)
{
    return lbl_80453FEC.field_28;
}
