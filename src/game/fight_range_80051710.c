/**
 * @file fight_range_80051710.c
 * @brief fight, 0x80051710 - 0x8005344C.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

void dbgMenuFightTrainerDataStatusInputDigit(u32 trainerId, u32 field, u32 index,
                                             s32 maximum, s32 minimum) {
    extern u32 fightTrainerGetStatus(u32, u32, u32, u32);
    extern u8 fn_8001E224(u32, s32*, u32, u32, u32, u32);
    extern void fightTrainerSetStatus(u32, u32, u32, u32, s32);
    extern void menuSubCloseNumberInput(void);
    s32 value;
    u8 result;

    result = fn_8001E224(fightTrainerGetStatus(0, trainerId, field, index),
                         &value, 0, 0x32, 0x32, 0);
    if (result == 1) {
        if (value > maximum) {
            value = maximum;
        }
        if (value < minimum) {
            value = minimum;
        }
        fightTrainerSetStatus(0, trainerId, field, index, value);
    }
    menuSubCloseNumberInput();
}
