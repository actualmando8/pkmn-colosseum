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
extern void* lbl_8047A480;
extern s8 lbl_8047A490;
extern f32 lbl_8047A478;
extern f32 lbl_8047A484;
extern f32 lbl_8047A494;
extern s32 lbl_8047A4A8;
extern s32 lbl_8047A4B8;
extern s32 lbl_8047A4B4;
extern s32 lbl_8047A4B0;

extern f32 lbl_8047BA58;
extern f32 lbl_8047BA60;
extern f32 lbl_8047BA74;
extern f32 lbl_8047BA78;
extern f32 lbl_8047BAC0;

typedef struct PdaModelWindow {
    u8 pad00[0x8];
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    u8 pad14[0x4];
    f32 field_18;
    u8 pad1C[0xC];
    f32 field_28;
    u8 pad2C[0x18];
    f32 alphaScale;
} PdaModelWindow;

typedef struct PdaSprite {
    u8 pad00[0x4];
    s8 flags;
    u8 pad05[0x47];
    s32 messageId;
    s16 field_50;
    s16 field_52;
    s16 x;
    s16 y;
    u8 pad58[0xc];
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 alpha;
    u8 pad68[0x8];
    f32 value;
    u8 pad74[0x17];
    u8 alphaByte;
    u8 pad8c[9];
    s8 selectedIndex;
} PdaSprite;

typedef struct PdaSceneWork {
    u8 pad00[0x10];
    s32 field_10;
    u8 pad14[0x14];
    s32 field_28;
} PdaSceneWork;

typedef struct PdaEvent {
    u8 pad00[0x6];
    s16 messageId;
} PdaEvent;

typedef struct PdaMenuState {
    u8 pad00;
    s8 mode;
    s8 menuSet;
} PdaMenuState;

typedef struct PdaSelectionWork {
    u8 pad00[4];
    void* menu;
    u8 pad08[0x8d];
    s8 selectedIndex;
} PdaSelectionWork;

typedef struct PdaKeyInfo {
    u8 pad00[6];
    u16 buttons;
} PdaKeyInfo;

typedef struct PdaListEntry {
    u16 field_00;
    u16 battleId;
} PdaListEntry;

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
extern void fn_800FB680(s32 arg0, s32 arg1, s32 arg2, void* data);

void fn_80037158(void)
{
}

s32 fn_8003715C(void)
{
    u8 flag;

    flag = lbl_8047A470;
    lbl_8047A470 = 1;
    return 1 - (u8)flag;
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

#pragma peephole off
void fn_80037300(void* window, PdaSprite* sprite)
{
    extern u8 lbl_80314E08[];
    extern void fn_800D5BA0(s32 index, u32 color);
    extern void fn_800D61E4(s32 x, s32 y);
    extern void fn_800D6728(void);
    extern void fn_800D67BC(s32 count);
    extern void fn_800D6A00(s32 primitive);
    extern void fn_800D7820(void* data);
    extern void fn_800D888C(s32 mode);
    extern void fn_800D88DC(s32 mode);

    (void)window;
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800D7820(lbl_80314E08);
    fn_800D6A00(6);
    fn_800D67BC(4);
    fn_800D61E4(0, 0);
    fn_800D5BA0(0, 0x003B6DFF);
    fn_800D61E4(sprite->x, 0);
    fn_800D5BA0(0, 0x489DECFF);
    fn_800D61E4(sprite->x, sprite->y);
    fn_800D5BA0(0, 0x489DECFF);
    fn_800D61E4(0, sprite->y);
    fn_800D5BA0(0, 0x003B6DFF);
    fn_800D6728();
}
#pragma peephole reset

#pragma peephole off
void fn_800373C8(PdaSprite* context, PdaSprite* sprite)
{
    extern u8 lbl_803A64EC[];
    extern u8 lbl_803A654C[];
    s32 value;

    value = (s32)*(f32*)(lbl_803A654C + 0x58);
    if ((s16)value < -0xff) {
        value = -0xff;
    }
    sprite->alpha = (u8)(value + 0xff);
    sprite->field_50 = (s16)(*(f32*)(lbl_803A64EC + 0x54) + *(f32*)(lbl_803A654C + 0x58));
    if (context->selectedIndex == 3) {
        sprite->colorR = 0xff;
        sprite->colorG = 0xff;
        sprite->colorB = 0xff;
    } else {
        sprite->colorR = 0x46;
        sprite->colorG = 0x8f;
        sprite->colorB = 0xb4;
    }
}
#pragma peephole reset

#pragma peephole off
void fn_80037468(PdaSprite* context, PdaSprite* sprite)
{
    extern u8 lbl_803A64EC[];
    extern u8 lbl_803A654C[];
    s32 value;

    value = (s32)*(f32*)(lbl_803A654C + 0x40);
    if ((s16)value < -0xff) {
        value = -0xff;
    }
    sprite->alpha = (u8)(value + 0xff);
    sprite->field_50 = (s16)(*(f32*)(lbl_803A64EC + 0x3c) + *(f32*)(lbl_803A654C + 0x40));
    if (context->selectedIndex == 2) {
        sprite->colorR = 0xff;
        sprite->colorG = 0xff;
        sprite->colorB = 0xff;
    } else {
        sprite->colorR = 0x46;
        sprite->colorG = 0x8f;
        sprite->colorB = 0xb4;
    }
}
#pragma peephole reset

#pragma peephole off
void fn_80037508(PdaSprite* context, PdaSprite* sprite)
{
    extern u8 lbl_803A64EC[];
    extern u8 lbl_803A654C[];
    s32 value;

    value = (s32)*(f32*)(lbl_803A654C + 0x28);
    if ((s16)value < -0xff) {
        value = -0xff;
    }
    sprite->alpha = (u8)(value + 0xff);
    sprite->field_50 = (s16)(*(f32*)(lbl_803A64EC + 0x24) + *(f32*)(lbl_803A654C + 0x28));
    if (context->selectedIndex == 1) {
        sprite->colorR = 0xff;
        sprite->colorG = 0xff;
        sprite->colorB = 0xff;
    } else {
        sprite->colorR = 0x46;
        sprite->colorG = 0x8f;
        sprite->colorB = 0xb4;
    }
}
#pragma peephole reset

#pragma peephole off
void fn_800375A8(PdaSprite* context, PdaSprite* sprite)
{
    extern u8 lbl_803A64EC[];
    extern u8 lbl_803A654C[];
    s32 value;

    value = (s32)*(f32*)(lbl_803A654C + 0x10);
    if ((s16)value < -0xff) {
        value = -0xff;
    }
    sprite->alpha = (u8)(value + 0xff);
    sprite->field_50 = (s16)(*(f32*)(lbl_803A64EC + 0xc) + *(f32*)(lbl_803A654C + 0x10));
    if (context->selectedIndex == 0) {
        sprite->colorR = 0xff;
        sprite->colorG = 0xff;
        sprite->colorB = 0xff;
    } else {
        sprite->colorR = 0x46;
        sprite->colorG = 0x8f;
        sprite->colorB = 0xb4;
    }
}
#pragma peephole reset

void fn_80037648(void)
{
    s32 messageId;

    switch (lbl_8047A490) {
    case 0:
        messageId = 0x1b5b;
        break;
    case 1:
        messageId = 0x1b5c;
        break;
    case 2:
        messageId = 0x1b5d;
        break;
    case 3:
        messageId = 0x1b5e;
        break;
    default:
        messageId = 0x1b59;
        break;
    }
    fn_800FB680(-4, 0, -1, (void*)messageId);
}

void fn_80038124(void* window, PdaSprite* sprite)
{
    sprite->value = lbl_8047BA74 - lbl_8047A478;
}

#pragma scheduling off
void fn_800376C8(void)
{
    fn_800FB680(0, 0, -1, lbl_8047A480);
}
#pragma scheduling reset

#pragma peephole off
void fn_800376F8(void* window, volatile PdaSprite* sprite)
{
    extern u8 lbl_803A654C[];
    f32 value;
    f32 next;
    f32 velocity;
    s32 alpha;

    (void)window;
    value = *(f32*)(lbl_803A654C + 0x58);
    if (value < lbl_8047BA58) {
        alpha = (s32)value;
        if ((s16)alpha < -0xff) {
            alpha = -0xff;
        }
        value = *(volatile f32*)&lbl_8047BA58;
        sprite->alpha = (u8)(alpha + 0xff);
        velocity = *(f32*)(lbl_803A654C + 0x50);
        next = *(f32*)(lbl_803A654C + 0x58) + velocity;
        *(f32*)(lbl_803A654C + 0x58) = next;
        *(f32*)(lbl_803A654C + 0x50) =
            lbl_8047A494 * *(f32*)(lbl_803A654C + 0x4c) + velocity;
        if (next > value) {
            *(f32*)(lbl_803A654C + 0x58) = value;
            sprite->field_50 = (s16)*(f32*)(lbl_803A654C + 0x54);
        }
        sprite->field_50 = (s16)(*(f32*)(lbl_803A654C + 0x54) +
                                  *(f32*)(lbl_803A654C + 0x58));
    }
}
#pragma peephole reset

#pragma peephole off
void fn_800377B4(void* window, volatile PdaSprite* sprite)
{
    extern u8 lbl_803A654C[];
    f32 value;
    f32 next;
    f32 velocity;
    s32 alpha;

    (void)window;
    value = *(f32*)(lbl_803A654C + 0x40);
    if (value < lbl_8047BA58) {
        alpha = (s32)value;
        if ((s16)alpha < -0xff) {
            alpha = -0xff;
        }
        value = *(volatile f32*)&lbl_8047BA58;
        sprite->alpha = (u8)(alpha + 0xff);
        velocity = *(f32*)(lbl_803A654C + 0x38);
        next = *(f32*)(lbl_803A654C + 0x40) + velocity;
        *(f32*)(lbl_803A654C + 0x40) = next;
        *(f32*)(lbl_803A654C + 0x38) =
            lbl_8047A494 * *(f32*)(lbl_803A654C + 0x34) + velocity;
        if (next > value) {
            *(f32*)(lbl_803A654C + 0x40) = value;
            sprite->field_50 = (s16)*(f32*)(lbl_803A654C + 0x3c);
        }
        sprite->field_50 = (s16)(*(f32*)(lbl_803A654C + 0x3c) +
                                  *(f32*)(lbl_803A654C + 0x40));
    }
}
#pragma peephole reset

#pragma peephole off
void fn_80037870(void* window, volatile PdaSprite* sprite)
{
    extern u8 lbl_803A654C[];
    f32 value;
    f32 next;
    f32 velocity;
    s32 alpha;

    (void)window;
    value = *(f32*)(lbl_803A654C + 0x28);
    if (value < lbl_8047BA58) {
        alpha = (s32)value;
        if ((s16)alpha < -0xff) {
            alpha = -0xff;
        }
        value = *(volatile f32*)&lbl_8047BA58;
        sprite->alpha = (u8)(alpha + 0xff);
        velocity = *(f32*)(lbl_803A654C + 0x20);
        next = *(f32*)(lbl_803A654C + 0x28) + velocity;
        *(f32*)(lbl_803A654C + 0x28) = next;
        *(f32*)(lbl_803A654C + 0x20) =
            lbl_8047A494 * *(f32*)(lbl_803A654C + 0x1c) + velocity;
        if (next > value) {
            *(f32*)(lbl_803A654C + 0x28) = value;
            sprite->field_50 = (s16)*(f32*)(lbl_803A654C + 0x24);
        }
        sprite->field_50 = (s16)(*(f32*)(lbl_803A654C + 0x24) +
                                  *(f32*)(lbl_803A654C + 0x28));
    }
}
#pragma peephole reset

#pragma peephole off
void fn_8003792C(void* window, volatile PdaSprite* sprite)
{
    extern u8 lbl_803A654C[];
    f32 value;
    f32 next;
    f32 velocity;
    s32 alpha;

    (void)window;
    value = *(f32*)(lbl_803A654C + 0x10);
    if (value < lbl_8047BA58) {
        alpha = (s32)value;
        if ((s16)alpha < -0xff) {
            alpha = -0xff;
        }
        value = *(volatile f32*)&lbl_8047BA58;
        sprite->alpha = (u8)(alpha + 0xff);
        velocity = *(f32*)(lbl_803A654C + 0x8);
        next = *(f32*)(lbl_803A654C + 0x10) + velocity;
        *(f32*)(lbl_803A654C + 0x10) = next;
        *(f32*)(lbl_803A654C + 0x8) =
            lbl_8047A494 * *(f32*)(lbl_803A654C + 0x4) + velocity;
        if (next > value) {
            *(f32*)(lbl_803A654C + 0x10) = value;
            sprite->field_50 = (s16)*(f32*)(lbl_803A654C + 0xc);
        }
        sprite->field_50 = (s16)(*(f32*)(lbl_803A654C + 0xc) +
                                  *(f32*)(lbl_803A654C + 0x10));
    }
}
#pragma peephole reset

/* The redundant expressions preserve MWCC's exact register/scheduling shape. */
void fn_80038138(void* window, PdaSprite* sprite)
{
    f32 new_var;
    int new_var4;
    f32 value;
    f32 new_var2;
    float new_var3;

    new_var2 = (0, lbl_8047A494);
    new_var3 = lbl_8047BA78 * new_var2;
    if (((!lbl_8047A478) && (!lbl_8047A478)) && (!lbl_8047A478)) {
    }
    value = lbl_8047A478 + new_var3;
    lbl_8047A478 = value;
    new_var = value;
    if (new_var4 = new_var > lbl_8047BA60) {
        lbl_8047A478 = lbl_8047BA60;
        lbl_8047A478 = value - lbl_8047A478;
    }
    sprite->value = lbl_8047A478;
}

#pragma peephole off
void fn_80038170(PdaSprite* context, PdaSprite* sprite)
{
    extern u8 lbl_803A654C[];
    extern f32 lbl_802E5288[][2];
    extern s8 lbl_8047A47C;
    f32* state;
    s8 index;
    u8 stopped;

    stopped = 0;
    state = (f32*)lbl_803A654C;
    if (lbl_8047BA58 == state[4]) {
        state += 6;
        if (lbl_8047BA58 == state[4]) {
            state += 6;
            if (lbl_8047BA58 == state[4]) {
                state += 6;
                if (lbl_8047BA58 == state[4]) {
                    stopped = 1;
                }
            }
        }
    }
    if (stopped) {
        sprite->flags |= 2;
    } else {
        sprite->flags &= ~2;
    }
    index = context->selectedIndex;
    lbl_8047A47C = index;
    sprite->field_50 = (s16)lbl_802E5288[index][0];
    *(s16*)((u8*)sprite + 0x52) = (s16)lbl_802E5288[index][1];
}
#pragma peephole reset

#pragma peephole off
s32 fn_80038250(PdaMenuState* state)
{
    extern void winSeqSetMenu(s32 sequence, s32 menu);

    switch (state->mode) {
    case 0:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x1b, 0xc4);
            state->menuSet = 1;
        }
        break;
    case 3:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x1b, 0xc8);
            state->menuSet = 1;
        }
        break;
    }
    return 0;
}
#pragma peephole reset

#pragma peephole off
s32 fn_800382E8(PdaMenuState* state)
{
    extern void winSeqSetMenu(s32 sequence, s32 menu);

    switch (state->mode) {
    case 0:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x1a, 0xbc);
            state->menuSet = 1;
        }
        break;
    case 3:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x1a, 0xc0);
            state->menuSet = 1;
        }
        break;
    }
    return 0;
}
#pragma peephole reset

#pragma peephole off
#pragma scheduling off
s32 fn_80038380(PdaSelectionWork* work)
{
    extern PdaKeyInfo* windowGetKeyInfo(void);
    extern s32 menuGetSelectItemNum();
    PdaKeyInfo* keyInfo;
    u32 selectedIndex;
    s32 itemCount;

    keyInfo = windowGetKeyInfo();
    if (keyInfo->buttons & 2) {
        itemCount = menuGetSelectItemNum(work->menu);
        selectedIndex = (u8)work->selectedIndex;
        itemCount = (s8)itemCount;
        selectedIndex++;
        work->selectedIndex = selectedIndex;
        if ((s8)selectedIndex >= itemCount) {
            work->selectedIndex = itemCount - 1;
        }
    }
    if (keyInfo->buttons & 1) {
        selectedIndex = (u8)work->selectedIndex;
        selectedIndex--;
        work->selectedIndex = selectedIndex;
        if ((s8)selectedIndex < 0) {
            work->selectedIndex = 0;
        }
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset

#pragma peephole off
s32 fn_80039498(s32 value)
{
    extern u32 windowGetActiveID(void);
    extern s32 menuOpenCustom(s32 menuId, ...);
    extern void menuClose(s32 menuId);
    extern void menuCloseSync(s32 menuId, s32 wait);
    s32 parameter = value;
    s32 choices[4] = { 0, 1, 2, 3 };
    s32 choice;

    choice = menuOpenCustom(0x24, windowGetActiveID(), &parameter, 0, 1, 0);
    menuClose(0x24);
    menuCloseSync(0x24, 1);
    if (choice < 0 || choice >= 4) {
        return 4;
    }
    return choices[choice];
}
#pragma peephole reset

s32 fn_80039548(void* window, PdaSprite* sprite)
{
    (void)window;

    if (lbl_8047A4B0 == 0) {
        sprite->messageId = 0x1b6d;
    } else {
        sprite->messageId = 0x1b6e;
    }
    return 0;
}

#pragma peephole off
s32 fn_8003956C(void* window, void* sprite)
{
    extern u16 pcboxGetNbItemSlot(s32 box);
    extern void* pcboxGetItem(s32 box, s16 slot);
    extern u8 fn_801429E8(void* item);
    s32 count;
    s32 slot;
    s32 slotCount;
    u16 boundedSlotCount;
    s32 threshold;
    s32 display;

    (void)window;
    count = 0;
    threshold = lbl_8047A4A8 + 8;
    slotCount = pcboxGetNbItemSlot(0);
    boundedSlotCount = slotCount;
    for (slot = 0; slot < boundedSlotCount; slot++) {
        if (fn_801429E8(pcboxGetItem(0, slot))) {
            count++;
        }
    }
    if (threshold < count + 1) {
        display = 1;
    } else {
        display = 0;
    }
    winSpriteSetDisp(sprite, display);
    return 0;
}
#pragma peephole reset

#pragma scheduling off
void fn_800388C4(void)
{
    extern void menuCloseCustom(s32 slot, s32 arg1, s32 arg2);

    menuCloseCustom(0x19, 0, 1);
    menuCloseCustom(0x1a, 0, 1);
    menuCloseCustom(0x1b, 0, 1);
    menuCloseCustom(0x18, 0, 1);
    menuCloseCustom(0x1e, 0, 1);
    menuCloseCustom(0x1f, 0, 1);
    menuCloseCustom(0x20, 0, 1);
    menuCloseCustom(0x21, 0, 1);
    menuCloseCustom(0x22, 0, 1);
    menuCloseCustom(0x23, 0, 1);
    menuCloseCustom(0x1d, 0, 1);
}
#pragma scheduling reset

void fn_80038990(void)
{
    extern s32 fn_800D37CC(void);
    extern u32 fn_800D3088(void);
    extern void _threadSwitch(void);
    f32 divisor;

    while (1) {
        divisor = (f32)fn_800D37CC();
        lbl_8047A494 = (f32)fn_800D3088() / divisor;
        _threadSwitch();
    }
}

void fn_80038A00(void)
{
    lbl_8047A484 = lbl_8047BA58;
}

#pragma peephole off
s32 fn_80039004(PdaSprite* context, PdaSprite* sprite)
{
    extern f32 lbl_803A65B0[][3];
    s32 index;

    index = context->selectedIndex;
    if (index < 0 || index >= 8) {
        index = 0;
    }
    sprite->field_50 = (s16)lbl_803A65B0[index][0];
    sprite->field_52 = (s16)lbl_803A65B0[index][1];
    *(s8*)((u8*)sprite + 0x67) = lbl_803A65B0[index][2];
    return 0;
}
#pragma peephole reset

#pragma peephole off
#pragma scheduling off
s32 fn_8003907C(PdaSelectionWork* work)
{
    extern PdaKeyInfo* windowGetKeyInfo(void);
    extern s32 menuGetSelectItemNum();
    PdaKeyInfo* keyInfo;
    u32 selectedIndex;
    s32 itemCount;

    keyInfo = windowGetKeyInfo();
    if (keyInfo->buttons & 2) {
        itemCount = menuGetSelectItemNum(work->menu);
        selectedIndex = (u8)work->selectedIndex;
        itemCount = (s8)itemCount;
        selectedIndex++;
        work->selectedIndex = selectedIndex;
        if ((s8)selectedIndex >= itemCount) {
            work->selectedIndex = itemCount - 1;
        }
    }
    if (keyInfo->buttons & 1) {
        selectedIndex = (u8)work->selectedIndex;
        selectedIndex--;
        work->selectedIndex = selectedIndex;
        if ((s8)selectedIndex < 0) {
            work->selectedIndex = 0;
        }
    }
    return 0;
}
#pragma scheduling reset
#pragma peephole reset

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

#pragma peephole off
s32 fn_800398D4(void* work, PdaSprite* sprite)
{
    extern void fn_800FE38C(s32 x1, s32 y1, s32 x2, s32 y2);
    extern void fn_800FE35C(void);
    extern s32 fn_80039644(void* window, void* sprite);
    s32 x;
    s32 y;

    x = sprite->field_50;
    y = sprite->field_52;
    fn_800FE38C(0x118 - x, 0x8b - y, 0x150, 0x10d);
    fn_80039644((u8*)work + 0x94, *(void**)((u8*)work + 0x88));
    fn_800FE35C();
    return 0;
}
#pragma peephole reset

#pragma peephole off
s32 fn_8003992C(void* window, PdaSprite* sprite)
{
    extern s32 lbl_8047A4AC;
    extern s32 lbl_8047A4BC;
    extern f32 lbl_8047A4C0;
    s32 y;

    (void)window;
    y = lbl_8047A4AC * 31 + 0x9a;
    if (lbl_8047A4BC == 0) {
        y += (s32)lbl_8047A4C0;
    }
    sprite->field_52 = (s16)y;
    return 0;
}
#pragma peephole reset

#pragma peephole off
s32 fn_80039970(void* window, PdaSprite* sprite)
{
    extern s32 lbl_8047A4AC;
    extern s32 lbl_8047A4BC;
    extern f32 lbl_8047A4C0;
    s32 y;
    s32 disp;

    (void)window;
    if (lbl_8047A4B8 >= 0) {
        y = (lbl_8047A4B8 - lbl_8047A4A8) * 31 + 0x97;
        if (lbl_8047A4BC != 0) {
            y -= (s32)lbl_8047A4C0;
        }
        if (y + sprite->y < 0x97 || y >= 0x18f) {
            disp = 0;
        } else {
            disp = 1;
        }
        winSpriteSetDisp(sprite, disp);
    } else {
        y = lbl_8047A4AC * 31 + 0x97;
        if (lbl_8047A4BC == 0) {
            y += (s32)lbl_8047A4C0;
        }
        winSpriteSetDisp(sprite, 1);
    }
    sprite->field_52 = (s16)y;
    return 0;
}
#pragma peephole reset

#pragma scheduling off
s32 fn_80039A50(PdaSprite* sprite)
{
    fn_800FB680(0, 0, *(s32*)((u8*)sprite + 0x88), (void*)lbl_8047A4B4);
    return 0;
}
#pragma scheduling reset

void fn_80039F44(void* button)
{
    if (lbl_8047A4B8 < 0) {
        menuButtonNormal(button);
    }
}

#pragma peephole off
s32 fn_8003AC50(PdaMenuState* state)
{
    extern void winSeqSetMenu(s32 sequence, s32 menu);

    switch (state->mode) {
    case 0:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x26, 0xb4);
            state->menuSet = 1;
        }
        break;
    case 3:
        if (state->menuSet == 0) {
            winSeqSetMenu(0x26, 0xb8);
            state->menuSet = 1;
        }
        break;
    }
    return 0;
}
#pragma peephole reset

#pragma peephole off
s32 fn_8003ACE8(s32 arg0, s32 arg1, s32 arg2)
{
    extern s32 lbl_8047A4C8;
    extern s32 windowGetActiveID(void);
    extern s32 menuOpenCustom(s32 menu, ...);
    extern void menuClose(s32 menu);
    extern void menuCloseSync(s32 menu, s32 sync);
    s32 args[2];
    s32 result;
    s32 value;

    lbl_8047A4C8 = arg0;
    args[0] = arg2;
    args[1] = arg1;
    result = menuOpenCustom(0x26, windowGetActiveID(), 0, 0, 1, 1, args);
    menuClose(0x26);
    menuCloseSync(0x26, 1);
    if (result == -1) {
        value = -1;
    } else {
        value = lbl_8047A4C8;
    }
    return value;
}
#pragma peephole reset

#pragma peephole off
s32 fn_8003AE84(void)
{
    extern s32 menuOpen(s32 menu, s32 mode);
    extern void menuClose(s32 menu);
    extern void menuCloseSync(s32 menu, s32 sync);
    s32 result;
    s32 value;

    result = menuOpen(0x27, 1);
    menuClose(0x27);
    menuCloseSync(0x27, 1);
    if (result == -1) {
        value = 0;
    } else if (result == 0) {
        value = 1;
    } else {
        value = 0;
    }
    return value;
}
#pragma peephole reset

#pragma peephole off
void fn_8003AEF0(PdaSprite* sprite)
{
    extern PdaListEntry* lbl_8047A4D4;
    extern void fn_801EED88(u16 id);
    extern void* fn_801EE544(u16 id, u8* variant);
    extern s32 fn_801EEF40(u16 id);
    extern s32 fn_8011396C(s32 floor);
    extern void msgctrlSetValue(s32 id, s32 value);
    u16 id;
    void* message;
    u32 value;
    s32 color;

    fn_801EED88(lbl_8047A4D4[(u16)*(u32*)&lbl_803A6748].battleId);
    id = lbl_8047A4D4[*(u32*)&lbl_803A6748].battleId;
    sprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
    message = fn_801EE544(id, (u8*)&lbl_803A6748 + 0x94);
    value = fn_8011396C(fn_801EEF40(id));
    if (value == 0) {
        msgctrlSetValue(0x31, 0x18d3);
    } else {
        msgctrlSetValue(0x31, value);
    }
    color = -0x100;
    color |= sprite->alphaByte;
    fn_800FB680(0, 0, color, message);
}
#pragma peephole reset

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

#pragma peephole off
void fn_8003BF54(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    extern f32 lbl_8047BAD8;
    f32 offset;

    alphaSprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
    if (lbl_803A6748.field_10 > 10) {
        sprite->flags = sprite->flags | 2;
    } else {
        sprite->flags = sprite->flags & ~2;
        return;
    }
    offset = lbl_803A6748.field_18 /
             -(lbl_8047BAD8 * (f32)(lbl_803A6748.field_10 - 10));
    sprite->field_52 = (s16)(*(s16*)(lbl_802EF0A8 + 0x5084) +
                              offset * *(s16*)(lbl_802EF0A8 + 0x5088));
}
#pragma peephole reset

#pragma peephole off
void fn_8003C03C(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    extern f32 lbl_8047BAF0;
    extern f32 lbl_8047BAF4;
    extern f64 sin(f64 angle);
    f32 f0;
    f32 f1;
    f32 f3;

    alphaSprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
    if (lbl_803A6748.field_0C >= 10) {
        if (lbl_803A6748.field_10 != lbl_803A6748.field_0C) {
            sprite->flags = sprite->flags | 2;
        } else {
            sprite->flags = sprite->flags & ~2;
            return;
        }
    } else {
        sprite->flags = sprite->flags & ~2;
        return;
    }
    f0 = lbl_8047BAF4 * lbl_803A6748.field_28;
    f0 = lbl_8047BAF4 * f0;
    f1 = lbl_8047BAF0 * f0;
    f1 = (f32)sin(f1);
    f3 = f1;
    sprite->field_52 = (s16)(lbl_8047BAF4 * f3 +
                              (f32)(s32)*(s16*)(lbl_802EF0A8 + 0x504C));
}
#pragma peephole reset

#pragma peephole off
void fn_8003C13C(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    extern f32 lbl_8047BAF0;
    extern f32 lbl_8047BAF4;
    extern f64 sin(f64 angle);
    f32 f0;
    f32 f1;
    f32 f3;

    alphaSprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
    if (lbl_803A6748.field_08 > 0) {
        sprite->flags = sprite->flags | 2;
    } else {
        sprite->flags = sprite->flags & ~2;
        return;
    }
    f0 = lbl_8047BAF4 * lbl_803A6748.field_28;
    f0 = lbl_8047BAF4 * f0;
    f1 = lbl_8047BAF0 * f0 + lbl_8047BAF0;
    f1 = (f32)sin(f1);
    f3 = f1;
    sprite->field_52 = (s16)(lbl_8047BAF4 * f3 +
                              (f32)(s32)*(s16*)(lbl_802EF0A8 + 0x5014));
}
#pragma peephole reset

#pragma peephole off
void fn_8003C21C(PdaSprite* sprite, PdaEvent* event)
{
    (void)event;
    sprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
}
#pragma peephole reset

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

#pragma peephole off
void fn_8003C728(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    s32 row;
    s16 baseY;

    alphaSprite->alphaByte = lbl_8047BAC0 * lbl_803A6748.alphaScale;
    baseY = *(s16*)(lbl_802EF0A8 + 0x50a0);
    row = *(s32*)&lbl_803A6748 - lbl_803A6748.field_08;
    if (row >= 10) {
        row = 9;
    }
    *(f32*)((u8*)&lbl_803A6748 + 0x38) =
        (f32)(baseY + row * 0x18);
    sprite->field_52 = *(f32*)((u8*)&lbl_803A6748 + 0x30);
}
#pragma peephole reset

#pragma peephole off
void fn_8003D818(void)
{
    extern u16* lbl_8047A4E4;
    extern u16 lbl_8047A4E8;
    extern u16 memoDataGetPokemonID(u16* data, u32 index);
    extern u16 memoDataGetCount(u16* data);
    extern void fn_8003E394(void);
    u8* work;
    u32 count;
    u32 index;

    work = (u8*)&lbl_803A6818;
    count = 0;
    index = count;
    while ((u16)index < memoDataGetCount(0)) {
        lbl_8047A4E4[(u16)index] = memoDataGetPokemonID(0, index);
        count++;
        index++;
    }
    lbl_8047A4E8 = count;
    work[0x158] = 0;
    work[0x159] = 0;
    work[0x15b] = 0;
    work[0x15a] = 0;
    work[0x15c] = 0;
    fn_8003E394();
    lbl_803A6818.field_10 = lbl_8047A4E8;
}
#pragma peephole reset

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

#pragma peephole off
void fn_80041B5C(PdaSprite* sprite, PdaEvent* event)
{
    extern f32 lbl_8047BCA0;
    extern void fn_800411FC(PdaSprite* sprite, PdaEvent* event);

    sprite->alphaByte = lbl_8047BCA0 * *(f32*)((u8*)&lbl_803A6818 + 0x4c);
    switch (event->messageId) {
    case 0x331:
    case 0x759:
    case 0x76a:
    case 0xfbe:
        break;
    default:
        fn_800411FC(sprite, event);
        break;
    }
}
#pragma peephole reset

#pragma peephole off
void fn_80043CD8(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    extern f32 lbl_8047BCA0;
    extern f32 lbl_8047BCF4;
    f32 f0;
    f32 f3;
    s32 count_m1;

    alphaSprite->alphaByte = lbl_8047BCA0 * *(f32*)((u8*)&lbl_803A6818 + 0x4c);
    if (lbl_803A6818.field_10 > 10) {
        sprite->flags = sprite->flags | 2;
    } else {
        sprite->flags = sprite->flags & ~2;
        return;
    }
    f0 = *(f32*)((u8*)&lbl_803A6818 + 0x30);
    count_m1 = lbl_803A6818.field_10 - 1;
    f3 = -f0 / (f32)count_m1;
    sprite->field_52 = (s16)(f3 / lbl_8047BCF4 *
                                  *(s16*)(lbl_802EF0A8 + 0x5788) +
                              *(s16*)(lbl_802EF0A8 + 0x5784));
}
#pragma peephole reset

#pragma peephole off
void fn_80043EC8(PdaSprite* alphaSprite, PdaSprite* sprite)
{
    extern u8 lbl_802EF0A8[];
    extern f32 lbl_8047BCA0;
    extern f32 lbl_8047BCA4;
    extern f32 lbl_8047BCA8;
    extern f32 lbl_8047BD0C;
    extern f64 sin(f64 angle);
    f32 f0;
    f32 f1;
    f32 f3;

    alphaSprite->alphaByte = lbl_8047BCA0 * *(f32*)((u8*)&lbl_803A6818 + 0x4c);
    if (*(s32*)&lbl_803A6818 != 0) {
        sprite->flags = sprite->flags | 2;
    } else {
        sprite->flags = sprite->flags & ~2;
        return;
    }
    f0 = lbl_8047BCA8 * *(f32*)((u8*)&lbl_803A6818 + 0x40);
    f0 = lbl_8047BCA8 * f0;
    f1 = lbl_8047BCA4 * f0 + lbl_8047BCA4;
    f1 = (f32)sin(f1);
    f3 = f1;
    sprite->field_52 = (s16)(lbl_8047BD0C * f3 +
                              (f32)(s32)*(s16*)(lbl_802EF0A8 + 0x57bc));
}
#pragma peephole reset
