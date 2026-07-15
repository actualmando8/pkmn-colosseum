/**
 * @file menuCB_BattleStart.c
 * @brief menuCB_BattleStart.cpp, 0x8005DFC8 - 0x80062948.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. XD has __sinit_menuCB_BattleStart_cpp at
 * 0x800477F4; includes menuCBBattleStart* + menuCB_BattleResult* locals
 * (XD 0x80046594-0x800477F8). SMOKING GUN: fn_8005DFC8 takes the address of
 * local symbol _menuCBBattleStartDispTrainerTexCallBack__FlPvl (0x800626CC),
 * proving same-TU membership. All functions asm-only.
 */
#include "dolphin/types.h"

typedef struct MenuCBBattleStartState {
    void* menu;
    s32 status;
    u16 menuId;
    u8 padA[0x2E];
    s32 timer;
    u8 pad3C[0x330];
    u8 model[0x74];
} MenuCBBattleStartState;

typedef struct MenuCBBattleStartButton {
    u8 pad0[0x98];
    u8 finished;
} MenuCBBattleStartButton;

typedef struct MenuCBBattleStartParams {
    u8 pad0[4];
    s32 mode;
} MenuCBBattleStartParams;

extern MenuCBBattleStartState lbl_803A9A60;

extern void fn_8005DFC8(void* arg);

#pragma push
#pragma peephole off
void fn_8005E690(MenuCBBattleStartButton* button) {
    extern void menuButtonNormal(void* button);

    switch (lbl_803A9A60.status) {
    case 0:
        if (lbl_803A9A60.timer >= 3) {
            menuButtonNormal(button);
        }
        if (lbl_803A9A60.timer == 100) {
            button->finished = 1;
        }
        break;
    case 1:
        if (lbl_803A9A60.timer >= 7) {
            menuButtonNormal(button);
        }
        if (lbl_803A9A60.timer == 9) {
            button->finished = 1;
        }
        break;
    }
}
#pragma pop

void fn_8005E730(void* arg) {
    fn_8005DFC8(arg);
}

#pragma push
#pragma peephole off
s32 fn_8005E750(MenuCBBattleStartParams* params) {
    extern void menuCBBattleStartInit(void* params, s32 mode);
    extern void menuSetEnablePort(s32 enabled);
    extern void menuOpen(s32 menuId, s32 mode);
    extern void menuCloseCustom(s32 menuId, s32 arg1, s32 arg2);

    menuCBBattleStartInit(params, 0);
    lbl_803A9A60.menuId = 0xBA;
    menuSetEnablePort(0);
    menuOpen(0xDF, 0);
    menuOpen(0xBA, 1);
    menuSetEnablePort(1);
    menuCloseCustom(0xBA, 0, 1);
    lbl_803A9A60.status = 0;

    switch (params->mode) {
    default:
        return 0xC4;
    case 2:
        return 0xC6;
    }
}
#pragma pop

s32 menuCBBattleStartGetStatus(void) {
    MenuCBBattleStartState* state = &lbl_803A9A60;
    return state->status;
}

#pragma push
#pragma peephole off
void fn_80061028(s32 status) {
    extern void menuCloseCustom(s32 menuId, s32 arg1, s32 arg2);

    menuCloseCustom(0xBA, 0, 1);
    lbl_803A9A60.status = status;
}
#pragma pop

typedef struct MenuCBBattleStartDrawParams {
    u8 pad0[0x54];
    s16 x;
    s16 y;
} MenuCBBattleStartDrawParams;

#pragma push
#pragma scheduling off
#pragma peephole off
void fn_800608C4(void* context, MenuCBBattleStartDrawParams* params) {
    extern void* menuModelRender(void* model);
    extern void fn_800D88DC(s32 mode);
    extern void fn_800D888C(s32 mode);
    extern void fn_800D6A00(s32 primitive);
    extern void fn_800D7820(void* format);
    extern void fn_800D85D4(s32 index, void* model);
    extern void fn_800D67BC(s32 count);
    extern void fn_800D61E4(s32 x, s32 y);
    extern void fn_800D5CB8(s32 index, s32 red, s32 green, s32 blue, s32 alpha);
    extern void fn_800D59B8(s32 index, f32 x, f32 y);
    extern void fn_800D6728(void);
    extern u8 lbl_80314F98[];
    extern f32 lbl_8047BF60;
    extern f32 lbl_8047BF90;
    void* model;

    model = menuModelRender(lbl_803A9A60.model);
    if (model != 0) {
#pragma scheduling on
        fn_800D88DC(3);
        fn_800D888C(4);
        fn_800D6A00(7);
        fn_800D7820(lbl_80314F98);
        fn_800D85D4(0, model);
        fn_800D67BC(2);
        fn_800D61E4(0, 0);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, 0xFF);
        fn_800D59B8(0, lbl_8047BF60, lbl_8047BF60);
        fn_800D61E4(params->x, params->y);
        fn_800D5CB8(0, 0xFF, 0xFF, 0xFF, 0xFF);
        fn_800D59B8(0, lbl_8047BF90, lbl_8047BF90);
        fn_800D6728();
    }
}
#pragma pop

typedef struct MenuCBBattleStartSpriteContext {
    u8 pad0[0x84];
    s16 x;
    s16 y;
} MenuCBBattleStartSpriteContext;

typedef struct MenuCBBattleStartSprite {
    u8 pad0[6];
    s16 tableIndex;
    u8 pad8[0x48];
    s16 x;
    s16 y;
} MenuCBBattleStartSprite;

typedef struct MenuCBBattleStartSpriteEntry {
    u8 pad0[2];
    s16 x;
    u8 pad4[0x18];
} MenuCBBattleStartSpriteEntry;

#pragma push
#pragma peephole off
void fn_800609B4(MenuCBBattleStartSpriteContext* context,
                 MenuCBBattleStartSprite* sprite, f32 xOffset) {
    extern MenuCBBattleStartSpriteEntry lbl_802EF0A8[];
    extern void fn_800FE6D0(s32 x, s32 y);
    extern void spriteSetEnv(void);

    sprite->x = (s16)(lbl_802EF0A8[sprite->tableIndex].x + (s32)xOffset);
    fn_800FE6D0((s16)(context->x + sprite->x),
                (s16)(context->y + sprite->y));
    spriteSetEnv();
}
#pragma pop

typedef struct MenuCBBattleStartMessage {
    u8 pad0[4];
    s8 flags;
} MenuCBBattleStartMessage;

#pragma push
#pragma peephole off
void fn_80061B74(void* context, MenuCBBattleStartMessage* message) {
    switch (lbl_803A9A60.status) {
    case 0:
        message->flags &= ~2;
        break;
    case 1:
        message->flags &= ~2;
        break;
    }
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80062284(s32 trainer) {
    extern u16 toolentryTaisenGetEntryPokemonNum(s32 trainer);
    extern void* toolentryTaisenGetEntryPokemonPtr(s32 trainer, s32 index);
    extern u8 pokemonCheckValid(void* pokemon);
    extern u8 pokemonGetStatus(void* pokemon, s32 index, s32 status, s32 subindex);
    void* pokemon;
    u16 count;
    s32 i;

    count = toolentryTaisenGetEntryPokemonNum(trainer);
    for (i = 0; i < count; i++) {
        pokemon = toolentryTaisenGetEntryPokemonPtr(trainer, i);
        if (pokemon != 0 && pokemonCheckValid(pokemon) &&
            pokemonGetStatus(pokemon, 0, 0x7B, 0) == 1) {
            return 0;
        }
    }
    return 1;
}
#pragma pop
