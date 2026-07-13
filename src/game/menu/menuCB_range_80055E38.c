/**
 * @file menuCB_range_80055E38.c
 * @brief colosseum-battle team/status display screens, 0x80055E38 - 0x80057B34.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. menuSetDisp/winSeq screens, pokemon status
 * drawing via windowDrawSprite2. Identity SPECULATIVE (0 XD anchors;
 * structural-family evidence only: distinct .data pool band and static bss
 * 0x803A9768 shared across this range).
 *
 * fn_80056A78 (0x80056A78): trivial sda_getter, ported from the previous
 * campaign's archive/previous_campaign/src/game/menu/menu_status.c.
 * Remainder of the range is asm-only.
 */
#include "dolphin/types.h"

/* ===== SDA globals ===== */
extern u32 lbl_8047A584;
extern f32 lbl_8047BEC0;
extern f32 lbl_8047BEC4;
extern f32 lbl_8047A570;
extern f32 lbl_8047A578;
extern f32 lbl_8047A588;
extern f32 lbl_8047BF00;
extern f32 lbl_8047BF04;
extern u32 lbl_8047A56C;
extern u8 lbl_803A9768[];

extern void fn_80056C54(u8*, u8*, u32);
extern s32 menuCloseCustom(s32 menuId, s32 mode, s32 wait);

/* ===== Function implementations ===== */

u32 fn_80056A78(void) {
    return lbl_8047A584;
}

u32 fn_800566B4(void) {
    return !(lbl_8047A570 >= lbl_8047BEC4);
}

void fn_800566D8(u32 a) {
    lbl_8047A56C = a;
    lbl_8047A570 = lbl_8047BEC0;
}

u32 fn_800566E8(void) {
    return lbl_8047BEC0 != lbl_8047A578;
}

#pragma optimization_level 4
void fn_80057094(s16* a, s16* b) {
    *a = (s16)(s32)*(f32*)(lbl_803A9768 + 0x27c);
    *b = (s16)(s32)*(f32*)(lbl_803A9768 + 0x280);
}

#pragma scheduling off
u32 fn_80057114(u8* a, u8* b) {
    fn_80056C54(a, b, *(u32*)(lbl_803A9768 + 0x278));
    return 0;
}

u32 fn_800573C0(void) {
    s32 state;

#pragma scheduling on
#pragma optimization_level 4
    if (*(f32*)(lbl_803A9768 + 0x288) <= lbl_8047BF00) {
        state = *(s32*)lbl_803A9768;
        if (state == 0 || state == 3) {
            return 0;
        }
    }
    return 1;
}

void fn_80057400(void) {
    s32 value;

    value = (s32)(*(u32*)(lbl_803A9768 + 0x278) + 1);
    *(u32*)(lbl_803A9768 + 0x278) = value;
    if (value > 1) {
        *(u32*)(lbl_803A9768 + 0x278) = 0;
    }
}

u32 fn_80057428(void) {
    return !(lbl_8047A588 >= lbl_8047BF04);
}

void fn_8005744C(void) {
    lbl_8047A588 = lbl_8047BF00;
}

#pragma push
#pragma scheduling off
#pragma optimization_level 4
void fn_800574A8(void) {
    pokemonInit((u8*)(lbl_803A9768 + *(u32*)(lbl_803A9768 + 0x278) * 0x138 + 8));
}
#pragma pop

u8* fn_800574E0(void) {
    return lbl_803A9768 + *(u32*)(lbl_803A9768 + 0x278) * 0x138 + 8;
}

typedef struct {
    u32 data[78];
} Tbl78;

void fn_800574FC(u8* src) {
    Tbl78* dstState;
    Tbl78* srcState;
    u32 slot;

    slot = *(u32*)(lbl_803A9768 + 0x278);
    dstState = (Tbl78*)(lbl_803A9768 + slot * 0x138 + 8);
    srcState = (Tbl78*)src;
    *dstState = *srcState;
}

u32 fn_80057694(void) {
    return *(u32*)(lbl_803A9768 + 4);
}

void fn_800576A4(u32 a) {
    *(u32*)(lbl_803A9768 + 4) = a;
}

u32 fn_800576B4(void) {
    return *(u32*)(lbl_803A9768 + 0);
}

#pragma push
#pragma peephole off
#pragma optimization_level 4
s32 fn_80057A08(void) {
    return fn_80104704(0xa0) != 0;
}

void fn_80057A38(void) {
    menuCloseCustom(0xa0, 2, 1);
}
#pragma pop
