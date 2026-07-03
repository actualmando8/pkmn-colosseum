/**
 * @file menu_pda_mail.c
 * @brief PDA Mailbox reader UI, 0x8004B7EC - 0x8004EADC.
 *
 * XD-anchor-backed identity: menuPdaOpen (0x34) and pdaMailGetMailID
 * (0x50) byte-size-match XD's identically-named functions exactly;
 * calls mailGetMailIDInMailbox/mailGetReceiveNumber; owns 9 private
 * widget tables plus the 56-entry mail-list layout table. Positional
 * porting from XD's menuPdaMail* family failed (reordered subset) --
 * further naming needs byte-level comparison. All functions asm-only
 * until matched.
 */
#include "dolphin/types.h"

/* Small PDA-mail state byte pair, shared with pda_range_80037158.c
 * (the sibling PDA-body TU) and initialized by fn_8004B7EC. Only
 * bytes [0] and [1] are touched by this TU's accessors. */
extern u8 lbl_803A6A60[];

#if 0
asm u8 fn_8004BDEC(void) {
#include "src/game/menu/menu_pda_mail_fn_8004BDEC.inc"
}
#else
u8 fn_8004BDEC(void)
{
    return lbl_803A6A60[0];
}
#endif

#if 0
asm u8 fn_8004BDFC(void) {
#include "src/game/menu/menu_pda_mail_fn_8004BDFC.inc"
}
#else
u8 fn_8004BDFC(void)
{
    return lbl_803A6A60[1];
}
#endif

#if 0
asm void fn_8004BDB8(s8 a, s8 b) {
#include "src/game/menu/menu_pda_mail_fn_8004BDB8.inc"
}
#else
/* WALL: W2 extsb. vs extsb+cmpwi codegen idiom (see archive WALLS.md
 * fn_80039004 precedent) -- CW fuses the sign-extend+compare into a
 * record-form `extsb.` regardless of source shape (goto/if, cast to
 * s32, operand order all tried); target keeps them separate. Parked
 * at 83.8% after 3 attempts. */
void fn_8004BDB8(s8 a, s8 b)
{
    if (a >= 0) {
        lbl_803A6A60[1] = a;
    }
    if (b < 0) {
        return;
    }
    lbl_803A6A60[1] = b;
}
#endif

/* PI/6 and 2*PI -- rotation-angle wrap constants shared by the PDA
 * mail-icon spin/animation helpers. */
extern f64 lbl_8047BE28;
extern f64 lbl_8047BE30;

typedef struct PdaMailSpinWork {
    u8 pad00[0x70];
    f32 angle;
} PdaMailSpinWork;

#if 0
asm s32 fn_8004E144(void* window, PdaMailSpinWork* sprite) {
#include "src/game/menu/menu_pda_mail_fn_8004E144.inc"
}
#else
/* WALL: W1 register-letter (f1/f2 swap between the angle temp and the
 * wrap-constant local); 98.3% after 3 source-shape attempts (assignment-
 * in-condition, named-local wrap, reversed comparison operands). */
s32 fn_8004E144(void* window, PdaMailSpinWork* sprite)
{
    f64 wrap = lbl_8047BE30;
    if ((sprite->angle += lbl_8047BE28) >= wrap) {
        sprite->angle -= wrap;
    }
    return 0;
}
#endif

/* pdaMailGetMailID: byte-size-matches XD's pdaMailGetMailID (0x50)
 * exactly (see file header); not yet ported to C in this TU, but its
 * asm-linked symbol/signature is known from the XD reference (takes
 * a mailbox-slot index, returns the mail ID, or -1 out of range). */
extern s32 pdaMailGetMailID(s32 index);

/* fn_801D1A44 (battle_waza.c): "Waza entry get field 0x14 by index". */
extern u32 fn_801D1A44(s32 idx);

typedef struct PdaMailWindowA {
    u8 pad00[0x60];
    s32** field_0x60;
} PdaMailWindowA;

typedef struct PdaMailOutA {
    u8 pad00[0x4c];
    u32 field_0x4c;
} PdaMailOutA;

#if 0
asm s32 fn_8004D6AC(PdaMailWindowA* window, PdaMailOutA* out) {
#include "src/game/menu/menu_pda_mail_fn_8004D6AC.inc"
}
#else
s32 fn_8004D6AC(PdaMailWindowA* window, PdaMailOutA* out)
{
    out->field_0x4c = fn_801D1A44(pdaMailGetMailID(**window->field_0x60));
    return 0;
}
#endif

/* fn_80105624 (gs_event_exec.c): returns the current input-device
 * state pointer; menuButtonNormal (gs_model.c): resets a widget's
 * button sprite to its normal (unpressed) state. */
extern u8* fn_80105624(void);
extern void menuButtonNormal(void* p);

#if 0
asm void fn_8004E89C(void* widget) {
#include "src/game/menu/menu_pda_mail_fn_8004E89C.inc"
}
#else
#pragma peephole off
void fn_8004E89C(void* widget)
{
    u8* state = fn_80105624();
    if (!(*(u16*) state & 0x10)) {
        menuButtonNormal(widget);
    }
}
#pragma peephole reset
#endif

/* winSpriteSetDisp (gs_worldmap.c): set a window-sprite field-handle's
 * display/visibility value. */
extern void winSpriteSetDisp(void* fieldHandle, s32 value);

typedef struct PdaMailWindowB {
    u8 pad00[0x60];
    s32* field_0x60;
} PdaMailWindowB;

#if 0
asm s32 fn_8004DB34(PdaMailWindowB* window, void* fieldHandle) {
#include "src/game/menu/menu_pda_mail_fn_8004DB34.inc"
}
#else
#pragma scheduling off
s32 fn_8004DB34(PdaMailWindowB* window, void* fieldHandle)
{
    if (*window->field_0x60 != 0) {
        winSpriteSetDisp(fieldHandle, 0);
    } else {
        winSpriteSetDisp(fieldHandle, 1);
    }
    return 0;
}
#pragma scheduling reset
#endif

/* fn_801D16F0 (battle_waza.c): "Waza entry get field 0x18 by index". */
extern u32 fn_801D16F0(s32 idx);

#if 0
asm s32 fn_8004D5EC(PdaMailWindowA* window, void* fieldHandle) {
#include "src/game/menu/menu_pda_mail_fn_8004D5EC.inc"
}
#else
#pragma peephole off
s32 fn_8004D5EC(PdaMailWindowA* window, void* fieldHandle)
{
    u8 flag;
    if (fn_801D16F0(pdaMailGetMailID(**window->field_0x60)) != 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    winSpriteSetDisp(fieldHandle, flag);
    return 0;
}
#pragma peephole reset
#endif

#if 0
asm s32 fn_8004D64C(PdaMailWindowA* window, void* fieldHandle) {
#include "src/game/menu/menu_pda_mail_fn_8004D64C.inc"
}
#else
#pragma peephole off
s32 fn_8004D64C(PdaMailWindowA* window, void* fieldHandle)
{
    u8 flag;
    if (fn_801D16F0(pdaMailGetMailID(**window->field_0x60)) != 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    winSpriteSetDisp(fieldHandle, flag);
    return 0;
}
#pragma peephole reset
#endif

#if 0
asm s32 fn_8004D590(PdaMailWindowA* window, PdaMailOutA* out) {
#include "src/game/menu/menu_pda_mail_fn_8004D590.inc"
}
#else
/* WALL: W2 epilogue-register-restore order (lwz r0,0x14(r1) vs lwz
 * r31,0xc(r1) swapped) after #pragma scheduling off fixed the earlier
 * branch/epilogue scheduling issue; optimize_for_size (alone or
 * combined) does not move it further. Parked at 99.5% after 3
 * attempts. */
#pragma scheduling off
s32 fn_8004D590(PdaMailWindowA* window, PdaMailOutA* out)
{
    if (fn_801D16F0(pdaMailGetMailID(**window->field_0x60)) != 0) {
        out->field_0x4c = 0x36B9;
    } else {
        out->field_0x4c = 0;
    }
    return 0;
}
#pragma scheduling reset
#endif

/* fn_801D1A88 (battle_waza.c): "Waza entry get field 0x0C by index".
 * fn_800FA280/fn_80132A38 (gs_title.c): message/window callbacks. */
extern u32 fn_801D1A88(s32 idx);
extern void* fn_800FA280(u32);
extern void fn_80132A38(s32, void*);

#if 0
asm s32 fn_8004D6F0(PdaMailWindowA* window, PdaMailOutA* out) {
#include "src/game/menu/menu_pda_mail_fn_8004D6F0.inc"
}
#else
/* WALL: W1 arg-shuffle register (target routes fn_800FA280's result
 * through r0 before moving to r4, ours keeps it in r3->r4 directly;
 * a plain and a volatile local both tried) + W2 epilogue-restore
 * order. 95.8% after 3 attempts. */
#pragma scheduling off
s32 fn_8004D6F0(PdaMailWindowA* window, PdaMailOutA* out)
{
    u32 val = fn_801D1A88(pdaMailGetMailID(**window->field_0x60));
    if (val != 0) {
        void* winPtr = fn_800FA280(val);
        fn_80132A38(0x37, winPtr);
        out->field_0x4c = 0xE7;
    } else {
        out->field_0x4c = 0;
    }
    return 0;
}
#pragma scheduling reset
#endif

/* fn_801D1ACC (battle_waza.c): "Waza entry get field 0x10 by index". */
extern u32 fn_801D1ACC(s32 idx);

#if 0
asm s32 fn_8004D760(PdaMailWindowA* window, PdaMailOutA* out) {
#include "src/game/menu/menu_pda_mail_fn_8004D760.inc"
}
#else
/* WALL: same class as fn_8004D6F0 (W1 arg-shuffle + W2 epilogue
 * order). 95.8% after re-using the same fix. */
#pragma scheduling off
s32 fn_8004D760(PdaMailWindowA* window, PdaMailOutA* out)
{
    u32 val = fn_801D1ACC(pdaMailGetMailID(**window->field_0x60));
    if (val != 0) {
        void* winPtr = fn_800FA280(val);
        fn_80132A38(0x37, winPtr);
        out->field_0x4c = 0xE7;
    } else {
        out->field_0x4c = 0;
    }
    return 0;
}
#pragma scheduling reset
#endif

#if 0
asm void fn_8004D8BC(PdaMailWindowA* window) {
#include "src/game/menu/menu_pda_mail_fn_8004D8BC.inc"
}
#else
#pragma peephole off
void fn_8004D8BC(PdaMailWindowA* window)
{
    u8* state = fn_80105624();
    if (fn_801D16F0(pdaMailGetMailID(**window->field_0x60)) != 0
        || (*(u16*) state & 0x10) == 0) {
        menuButtonNormal(window);
    }
}
#pragma peephole reset
#endif
