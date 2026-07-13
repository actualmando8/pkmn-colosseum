/**
 * @file menu_poke_coupon.c
 * @brief Poke Coupon menu, 0x8007C260 - 0x8007C300.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"

extern s32 lbl_80478940;
extern void __assert(const char* file, u32 line, const char* msg);
extern const char lbl_80268B38[];
extern const char lbl_80268B4C[];

void fn_8007C260(void) {
    lbl_80478940 = -1;
}

void menuPokeCouponInit(void) {
    if (lbl_80478940 == -1) {
        __assert((const char*)lbl_80268B38, 0x3B, (const char*)lbl_80268B4C);
    }
}
