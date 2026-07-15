/**
 * @file hsd_range_801B0158.c
 * @brief hsd code, 0x801B0158 - 0x801B019C (1 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

void fn_801B0158(void)
{
    extern u8 lbl_804656B4[];
    extern u8 lbl_80465688[];
    extern void HSD_ObjAllocInit(void*, u32, u32);

    HSD_ObjAllocInit(lbl_804656B4, 0x1C, 4);
    HSD_ObjAllocInit(lbl_80465688, 0xC, 4);
}
