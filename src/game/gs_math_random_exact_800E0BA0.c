/**
 * @file gs_math_random_exact_800E0BA0.c
 * @brief Exact floating-point random helpers.
 *
 * Address range: 0x800E0BA0 - 0x800E0C04.
 */

#include "dolphin/types.h"

extern f32 fn_801ADC7C(void);
extern f32 lbl_8047CB10;

f32 fn_800E0BA0(void)
{
    f32 a = fn_801ADC7C();
    f32 b = fn_801ADC7C();

    return b + a - lbl_8047CB10;
}

f32 fn_800E0BE4(void)
{
    return fn_801ADC7C();
}
