/**
 * @file gs_range_800096B4.c
 * @brief gs-engine, 0x800096B4 - 0x8000BE74.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * fn_800096B4 (0x800096B4, size 0x23E0) remains asm-only; the ten small
 * debug-menu callback functions below are decompiled to real C.
 */
#include "dolphin/types.h"

extern void menuDbgItemCreate(void);
extern s32 menuOpen(u32 a, u32 b);
extern void heroAddPokecoupon(u32 a, s32 b);
extern void heroAddPokedoru(u32 a, s32 b);
extern u8 fn_801EF63C(void);
extern void pokemonInit(u8* a);
extern s32 fn_800096B4(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern void heroCatchPokemon(u32 a, u8* b, u32 c, u32 d, u32 e);
extern u32 heroGetStatus(u8* a, u32 b, u32 c);
extern u8 lbl_80478840;
extern u8 lbl_803A1A48[];

/* fn_8000BA94 - 0x8000BA94 | size: 0x24 */
#pragma scheduling off
s32 fn_8000BA94(void) {
    menuDbgItemCreate();
    return 0;
}
#pragma scheduling on

/* fn_8000BAB8 - 0x8000BAB8 | size: 0x48 */
#pragma peephole off
s32 fn_8000BAB8(void) {
    s32 val = menuOpen(2, 1);
    if (val == -1) { return 0; }
    heroAddPokecoupon(0, val);
    return 0;
}
#pragma peephole on

/* fn_8000BB00 - 0x8000BB00 | size: 0x48 */
#pragma peephole off
s32 fn_8000BB00(void) {
    s32 val = menuOpen(2, 1);
    if (val == -1) { return 0; }
    heroAddPokedoru(0, val);
    return 0;
}
#pragma peephole on

/* dbgMenuHeroPokemonAdd - 0x8000BB48 | size: 0xA4 */
s32 dbgMenuHeroPokemonAdd(void) {
    if ((u8)fn_801EF63C() == 1) { return -1; }
    if (lbl_80478840 != 0) {
        pokemonInit(lbl_803A1A48);
        lbl_80478840 = 0;
    }
    if (fn_800096B4((u32)lbl_803A1A48, 0, 0, 0, 0, 0) < 0) { return -1; }
    heroCatchPokemon(0, lbl_803A1A48, 0, 4, 1);
    return -1;
}

/* fn_8000BBEC - 0x8000BBEC | size: 0x6C */
s32 fn_8000BBEC(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 5);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}

/* fn_8000BC58 - 0x8000BC58 | size: 0x6C */
s32 fn_8000BC58(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 4);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}

/* fn_8000BCC4 - 0x8000BCC4 | size: 0x6C */
s32 fn_8000BCC4(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 3);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}

/* fn_8000BD30 - 0x8000BD30 | size: 0x6C */
s32 fn_8000BD30(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 2);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}

/* fn_8000BD9C - 0x8000BD9C | size: 0x6C */
s32 fn_8000BD9C(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 1);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}

/* fn_8000BE08 - 0x8000BE08 | size: 0x6C */
s32 fn_8000BE08(void) {
    u32 val;
    if ((u8)fn_801EF63C() == 1) { return -1; }
    val = heroGetStatus(NULL, 3, 0);
    if (val == 0) { return -1; }
    return fn_800096B4(val, 0, 0, 0, 0, 0);
}
