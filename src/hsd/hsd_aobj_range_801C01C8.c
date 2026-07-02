/**
 * @file hsd_aobj_range_801C01C8.c
 * @brief HSD library code split out of battle_grid.c (audit 2026-07-01).
 *
 * Address range: 0x801C01C8 - 0x801C0F20. Contains the real, variadic
 * HSD_ForeachAnim (0x801C028C, classic PPC varargs prologue; callers in
 * gs_render*.c) plus small helpers. fn_801C01C8 / fn_801C021C /
 * HSD_ForeachAnim are asm-only for now.
 */
#include "dolphin/types.h"

/**
 * _HSD_AObjForgetMemory - Address: 0x801C0270 | Size: 0xC
 */
s32 _HSD_AObjForgetMemory(void) {
    extern u32 lbl_8047B388;
    lbl_8047B388 = 0;
}

/**
 * HSD_AObjSetRate - Address: 0x801C027C | Size: 0x10
 */
void HSD_AObjSetRate(void* obj, f32 frame) {
    if (obj != NULL) {
        *(f32*)((u8*)obj + 0x10) = frame;
    }
}
