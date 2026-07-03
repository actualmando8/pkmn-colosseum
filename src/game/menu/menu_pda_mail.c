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
    u8 pad00;
    s8 phase;
    s8 guard;
    u8 pad03;
    s32 msgObj;
    u8 pad08[0x58];
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

/* fn_800FF730 (gs_floor_data.c): floor-transition trigger; fn_8011288C
 * (gs_field_colquery.c, GSfield_IsTransitioning): floor resource-alloc
 * helper; _threadSwitch: cooperative thread yield. */
extern void fn_800FF730(s32 floorId);
extern void fn_8011288C(s32 a, u32 b);
extern void _threadSwitch(void);

#if 0
asm void menuPdaOpen(void) {
#include "src/game/menu/menu_pda_mail_menuPdaOpen.inc"
}
#else
#pragma scheduling off
void menuPdaOpen(void)
{
    fn_800FF730(0x392);
    fn_8011288C(0, 0);
    _threadSwitch();
}
#pragma scheduling reset
#endif

/* fn_801D1F7C (battle_waza.c): Waza-party active-effect count getter. */
extern u16 fn_801D1F7C(void);

/* Mail-ID lookup table (halfword mail IDs), indexed by receive-order
 * slot; sda21-addressed pointer variable (matches XD's pdaMailGetMailID
 * exactly per file header). */
extern u16* lbl_8047A500;

#if 0
asm s32 pdaMailGetMailID(s32 index) {
#include "src/game/menu/menu_pda_mail_pdaMailGetMailID.inc"
}
#else
#pragma peephole off
s32 pdaMailGetMailID(s32 index)
{
    extern s32 fn_801D1F7C(void);
    if (index < 0 || index >= fn_801D1F7C()) {
        return -1;
    }
    return lbl_8047A500[index];
}
#pragma peephole reset
#endif

/* mailGetReceiveNumber (XD-named, same address/size): returns the
 * receive-order slot for a given mail ID, or -1 if not found. */
extern s32 mailGetReceiveNumber(s32 mailId);

/* GScharCmp (menuCB_Battle.c): compares two rendered-message buffers. */
extern s32 GScharCmp(void* a, void* b);

#if 0
asm s32 fn_8004BE90(u16* a, u16* b) {
#include "src/game/menu/menu_pda_mail_fn_8004BE90.inc"
}
#else
/* WALL: W1 register-letter (idA/idB swap r31<->r30 vs target regardless
 * of local decl order/type -- u16 default order, swapped decl order,
 * s32 widen all tried); everything else (control flow, call order,
 * scheduling, subf formula) matches. Parked at 99.2% after 3 attempts. */
s32 fn_8004BE90(u16* a, u16* b)
{
    s32 idA = *a;
    s32 idB = *b;
    s32 cmp;
    void* msgA = fn_800FA280(fn_801D1ACC(idA));
    void* msgB = fn_800FA280(fn_801D1ACC(idB));
    cmp = GScharCmp(msgA, msgB);
    if (cmp != 0) {
        return cmp;
    }
    return mailGetReceiveNumber(idB) - mailGetReceiveNumber(idA);
}
#endif

#if 0
asm s32 fn_8004BF20(u16* a, u16* b) {
#include "src/game/menu/menu_pda_mail_fn_8004BF20.inc"
}
#else
/* WALL: same class as fn_8004BE90 (W1 register-letter idA/idB swap).
 * Parked at 99.2% (see fn_8004BE90 for attempts). */
s32 fn_8004BF20(u16* a, u16* b)
{
    s32 idA = *a;
    s32 idB = *b;
    s32 cmp;
    void* msgA = fn_800FA280(fn_801D1A88(idA));
    void* msgB = fn_800FA280(fn_801D1A88(idB));
    cmp = GScharCmp(msgA, msgB);
    if (cmp != 0) {
        return cmp;
    }
    return mailGetReceiveNumber(idB) - mailGetReceiveNumber(idA);
}
#endif

/* fn_801080CC (gs_event_exec.c): fires a scripted SE/event by (ctx, id). */
extern void fn_801080CC(s32 ctx, s32 id);

/* Small widget/state-machine record shared by the phase-triggered SE
 * callbacks below: phase drives a switch (only phases 0 and 3 do
 * anything), guard is a one-shot latch, msgObj is passed straight
 * through to fn_801080CC as its first (context) argument. */
typedef struct PdaMailPhaseWidget {
    u8 pad00;
    s8 phase;
    s8 guard;
    u8 pad03;
    s32 msgObj;
} PdaMailPhaseWidget;

#if 0
asm s32 fn_8004D928(PdaMailPhaseWidget* w) {
#include "src/game/menu/menu_pda_mail_fn_8004D928.inc"
}
#else
#pragma peephole off
s32 fn_8004D928(PdaMailPhaseWidget* w)
{
    switch (w->phase) {
    case 0:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c2);
            w->guard = 1;
        }
        break;
    case 3:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c6);
            w->guard = 1;
        }
        break;
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset
#endif

#if 0
asm s32 fn_8004DB80(PdaMailPhaseWidget* w) {
#include "src/game/menu/menu_pda_mail_fn_8004DB80.inc"
}
#else
#pragma peephole off
s32 fn_8004DB80(PdaMailPhaseWidget* w)
{
    switch (w->phase) {
    case 0:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c2);
            w->guard = 1;
        }
        break;
    case 3:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c6);
            w->guard = 1;
        }
        break;
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset
#endif

#if 0
asm s32 fn_8004DF34(PdaMailPhaseWidget* w) {
#include "src/game/menu/menu_pda_mail_fn_8004DF34.inc"
}
#else
#pragma peephole off
s32 fn_8004DF34(PdaMailPhaseWidget* w)
{
    switch (w->phase) {
    case 0:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c2);
            w->guard = 1;
        }
        break;
    case 3:
        if (w->guard == 0) {
            fn_801080CC(w->msgObj, 0x1c6);
            w->guard = 1;
        }
        break;
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset
#endif

/* Angle-wrap constants for the two phase-2 float animations below
 * (distinct sdata2 float pair per callback; same idiom as fn_8004E144
 * but single-precision and accessed through window->field_0x60). */
extern f32 lbl_8047BE18;
extern f32 lbl_8047BE1C;
extern f32 lbl_8047BE4C;
extern f32 lbl_8047BE50;

#if 0
asm s32 fn_8004D26C(PdaMailWindowA* window) {
#include "src/game/menu/menu_pda_mail_fn_8004D26C.inc"
}
#else
/* WALL: W1 register-letter (f1/f2 swap between the float value temp
 * and the threshold local, same class as fn_8004E144) -- compound
 * assignment-in-condition, split statements, and swapping which
 * constant gets a named local all tried; switch dispatch/case layout,
 * SE-call bodies, and store-vs-compare ordering all byte-match.
 * Parked at 99.6% after 3 attempts. */
#pragma peephole off
s32 fn_8004D26C(PdaMailWindowA* window)
{
    s32** field = window->field_0x60;
    switch (window->phase) {
    case 0:
        if (window->guard == 0) {
            fn_801080CC(window->msgObj, 0x1c2);
            window->guard = 1;
        }
        break;
    case 2: {
        f32 thresh = lbl_8047BE1C;
        if ((*(f32*)*field += lbl_8047BE18) >= thresh) {
            *(f32*)*field -= thresh;
        }
        break;
    }
    case 3:
        if (window->guard == 0) {
            fn_801080CC(window->msgObj, 0x1c6);
            window->guard = 1;
        }
        break;
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset
#endif

#if 0
asm s32 fn_8004E8E0(PdaMailWindowA* window) {
#include "src/game/menu/menu_pda_mail_fn_8004E8E0.inc"
}
#else
/* WALL: same class as fn_8004D26C (W1 register-letter f1/f2 swap).
 * Parked at 99.6% (see fn_8004D26C for attempts). */
#pragma peephole off
s32 fn_8004E8E0(PdaMailWindowA* window)
{
    s32** field = window->field_0x60;
    switch (window->phase) {
    case 0:
        if (window->guard == 0) {
            fn_801080CC(0x77, 0x86);
            window->guard = 1;
        }
        break;
    case 2: {
        f32 thresh = lbl_8047BE4C;
        if ((*(f32*)*field += lbl_8047BE50) >= thresh) {
            *(f32*)*field -= thresh;
        }
        break;
    }
    case 3:
        if (window->guard == 0) {
            fn_801080CC(0x77, 0x8a);
            window->guard = 1;
        }
        break;
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset
#endif

/* fn_801046B8/fn_801026A4/fn_80102510/menuCloseSync (gs_event_exec.c):
 * modal list-menu open/poll/close idiom -- same call skeleton as the
 * gs_event_exec.c item-quantity-picker (menu_id, input-state,
 * &config, 0, 1, 1, &out), open by id, close by id. */
extern u32 fn_801046B8(void);
extern s32 fn_801026A4(s32 menuId, u32 inputState, s32* config, s32 zero,
                        s32 one1, s32 one2, s32* out, ...);
extern void fn_80102510(s32 menuId);
extern void menuCloseSync(s32 menuId, s32 flag);

#if 0
asm s32 fn_8004DC18(s32 a) {
#include "src/game/menu/menu_pda_mail_fn_8004DC18.inc"
}
#else
/* WALL: W3 scheduling (target hoists the "out=0" li r0,0x0 one slot
 * earlier, ahead of the param spill store); declaration order (for
 * correct stack-slot placement), statement-order, #pragma scheduling
 * off, and #pragma peephole off all tried without moving it. Parked
 * at 95.2% after 3 attempts -- everything else (call skeleton, vararg
 * setup, validity checks, clamp range) byte-matches. */
s32 fn_8004DC18(s32 a)
{
    s32 out = 0;
    s32 localA = a;
    s32 choice = fn_801026A4(0x75, fn_801046B8(), &localA, 0, 1, 1, &out);
    if (choice != -1 && choice != localA) {
        out = 1;
    }
    fn_80102510(0x75);
    menuCloseSync(0x75, 1);
    if (choice < 0 || choice >= 4) {
        return -1;
    }
    return choice;
}
#endif
