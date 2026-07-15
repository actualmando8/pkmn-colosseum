/**
 * @file gs_range_80009178.c
 * @brief gs-engine, 0x80009178 - 0x800096B4.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

#pragma peephole off
s32 fn_800093D0(void)
{
    extern u32 fn_800F7BC4(s32 padId);
    extern s32 menuOpen(s32 menuId, s32 arg);
    extern u32 fn_801906A0(s32 flagId);
    extern u8 fn_8001E3E0(u32 value, u32* out);
    extern void _flagSet(s32 flagId, u32 value);
    extern void menuClose(s32 menuId);
    extern void fn_8018FE30(s32 value);
    s32 result;
    u32 selected;
    u32 value;

    if (fn_800F7BC4(1) & 0x20) {
        for (;;) {
            result = menuOpen(0xDD, 1);
            if (result >= 0) {
                selected = result;
                if (fn_8001E3E0(fn_801906A0(selected), &value) == 0) {
                    continue;
                }
                _flagSet(selected, value);
            } else {
                if (result == -1) {
                    menuClose(0xDD);
                    return -1;
                }
                break;
            }
        }
    } else {
        result = menuOpen(0xB, 1);
        menuClose(0xB);
        if (result >= 0) {
            fn_8018FE30(result);
        } else if (result == -1) {
            return -1;
        }
    }

    return 0;
}
#pragma peephole on
