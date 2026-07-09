/**
 * @file pda_range_80037158.c
 * @brief PDA subsystem body, 0x80037158 - 0x8004B7EC.
 *
 * Colosseum's PDA: the People/party-select 3D screen (5 private
 * widget tables), a shared PDA scene/camera block, and the mail-fetch
 * helpers. A reduced/reordered subset of XD's PDA (ReliveHall and
 * PdaSearcher clusters were cut). Internal boundaries are fuzzy --
 * split further only with evidence. All functions asm-only until
 * matched.
 */
#include "dolphin/types.h"

extern u8 lbl_8047A470;
extern f32 lbl_8047A478;
extern f32 lbl_8047A484;
extern s32 lbl_8047A4A8;
extern s32 lbl_8047A4B8;

extern f32 lbl_8047BA58;
extern f32 lbl_8047BA74;
extern f32 lbl_8047BAC0;

typedef struct PdaModelWindow {
    u8 pad00[0x44];
    f32 alphaScale;
} PdaModelWindow;

typedef struct PdaSprite {
    u8 pad00[0x4c];
    s32 messageId;
    s16 x;
    s16 y;
    u8 pad54[0x13];
    u8 alpha;
    u8 pad68[0x8];
    f32 value;
    u8 pad74[0x17];
    u8 alphaByte;
    u8 pad8c[9];
    s8 selectedIndex;
} PdaSprite;

typedef struct PdaSceneWork {
    u8 pad00[0x28];
    s32 field_28;
} PdaSceneWork;

typedef struct PdaEvent {
    u8 pad00[0x6];
    s16 messageId;
} PdaEvent;

extern PdaModelWindow lbl_803A6748;
extern u8 lbl_803A67FC[];
extern PdaSceneWork lbl_803A6818;

extern void fn_8003B85C(void* window, s32 enabled);
extern void fn_8003C2B8(PdaSprite* sprite, PdaEvent* event);
extern void fn_80041E48(void* work, s32 mode);
extern void fn_80042658(void* work, s32 mode);
extern void fn_800439BC(void* scene);
extern void GSscene_SetMode(s32 mode);
extern void menuButtonNormal(void* button);
extern void winSpriteSetDisp(void* sprite, s32 disp);

void fn_80037158(void)
{
}

void fn_80037174(void)
{
    lbl_8047A470 = 0;
}

void fn_800372F0(void)
{
}

void fn_800372F4(void)
{
}

void fn_800372F8(void)
{
}

void fn_800372FC(void)
{
}

void fn_80038124(void* window, PdaSprite* sprite)
{
    sprite->value = lbl_8047BA74 - lbl_8047A478;
}

void fn_80038A00(void)
{
    lbl_8047A484 = lbl_8047BA58;
}

#pragma peephole off
s32 fn_80039604(void* window, void* sprite)
{
    s32 disp;

    if (lbl_8047A4A8 > 0) {
        disp = 1;
    } else {
        disp = 0;
    }
    winSpriteSetDisp(sprite, disp);
    return 0;
}
#pragma peephole reset

void fn_80039F44(void* button)
{
    if (lbl_8047A4B8 < 0) {
        menuButtonNormal(button);
    }
}

#pragma scheduling off
void fn_8003B814(void* window)
{
    fn_8003B85C(window, 1);
}
#pragma scheduling reset

#pragma scheduling off
void fn_8003B838(void* window)
{
    fn_8003B85C(window, 0);
}
#pragma scheduling reset

void fn_8003C24C(PdaSprite* sprite, PdaEvent* event)
{
    switch (event->messageId) {
    case 0x2e2:
    case 0x2e4:
        sprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
        break;
    default:
        fn_8003C2B8(sprite, event);
        break;
    }
}

void fn_80041114(void* work)
{
    GSscene_SetMode(4);
    fn_800439BC(lbl_803A67FC);
    fn_80041E48(work, 0);
}

void fn_8004115C(void* work)
{
    GSscene_SetMode(4);
    fn_800439BC(lbl_803A67FC);
    fn_80042658(work, 1);
}

void fn_800411A4(void* work)
{
    GSscene_SetMode(4);
    fn_800439BC(lbl_803A67FC);
    fn_80042658(work, 0);
}

s32 fn_800411EC(void)
{
    return lbl_803A6818.field_28;
}
