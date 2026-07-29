/**
 * @file gs_range_8000D290.c
 * @brief gs-engine code, 0x8000D290 - 0x8000DAA8 (4 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/* 0x8000D290 */
u32 fn_8000D290(void) {
    return 1;
}

extern void winSpriteSetDisp(void*, s32);
extern u8* menuItemBiosGetPtr(s16);
extern void windowDrawSprite(s32, s32, void*, u16, s32);
extern s32 menuGetCursorItemID(void*);
extern u32 GSmsgGetRect(void*);
extern void fn_800FB680(s16, s16, u32, void*);

void fn_8000D298(u8* menu, u8* item)
{
    u8* item_data;
    void* text;
    u32 color;
    s16 x;
    s16 y;
    u32 rect;

    winSpriteSetDisp(item, 0);
    item_data = menuItemBiosGetPtr(*(s16*)(item + 6));
    if (item_data != NULL) {
        windowDrawSprite(0, 0, menu, *(u16*)(item_data + 0xA), 0);
    }

    text = *(void**)(item + 0x4C);
    if (text != NULL) {
        if (*(s16*)(item + 6) == menuGetCursorItemID(*(void**)(menu + 4))) {
            color = 0x1EF00000;
        } else {
            color = 0x1E525F00;
        }
        rect = GSmsgGetRect(text);
        x = (*(s16*)(item + 0x54) - (s16)(rect >> 16)) / 2;
        y = (*(s16*)(item + 0x56) - (s16)rect) / 2 - 2;
        fn_800FB680(x, y, color | menu[0x8B], text);
    }
}

typedef struct MenuSequenceMessageState {
    u8 pad0;
    s8 mode;
    s8 initialized;
    u8 pad3;
    s32 menuId;
    u8 pad8[0x8D];
    s8 selection;
    u8 pad96;
    s8 previousSelection;
} MenuSequenceMessageState;

static u32 menuSequenceMessageId(s32 menuId, s32 selection)
{
    if (menuId == 0x41) {
        switch (selection) {
        case 0:
            return 0x65;
        case 1:
            return 0xDB0;
        case 2:
            return 0xDB1;
        case 3:
            return 0xDB2;
        }
    } else if (menuId == 0x109) {
        switch (selection) {
        case 0:
            return 0x12A9;
        case 1:
            return 0x12AA;
        case 2:
            return 0x12AB;
        }
    }
    return 0;
}

s32 fn_8000D3AC(MenuSequenceMessageState* state)
{
    extern void winSeqSetMenu(s32 menuId, s32 state);
    extern void fn_801081F8(void* context, u32 messageId, s32 action);
    u32 messageId;

    switch (state->mode) {
    case 0:
        if (state->initialized == 0) {
            winSeqSetMenu(state->menuId, 1);
            messageId =
                menuSequenceMessageId(state->menuId, state->selection);
            if (messageId != 0) {
                fn_801081F8(state, messageId, 0x10);
            }
            state->initialized = 1;
        }
        break;
    case 2:
        if (state->previousSelection != state->selection) {
            messageId =
                menuSequenceMessageId(state->menuId, state->selection);
            if (messageId != 0) {
                fn_801081F8(state, messageId, 0x10);
            }
            messageId = menuSequenceMessageId(
                state->menuId, state->previousSelection);
            if (messageId != 0) {
                fn_801081F8(state, messageId, 0x15);
            }
        }
        break;
    case 3:
        if (state->initialized == 0) {
            winSeqSetMenu(state->menuId, 7);
            state->initialized = 1;
        }
        break;
    }
    return 0;
}

typedef struct MenuPendingEvent {
    u8 pad0[0x30];
    u16 eventId;
    u8 pad32[2];
} MenuPendingEvent;

void fn_8000D710(u8 mode)
{
    extern s32 heroMoveCheckEvent(MenuPendingEvent* events);
    extern void fn_80116D30(s32 kind, u16 eventId);
    extern void heroMoveInitEvent(void);
    extern void mailMainReceiveTerminate(void);
    extern void _threadSwitch(void);
    extern void GSmodelAllPauseAnimation(void);
    extern void GSmodelAllUnpauseAnimation(void);
    extern s32 fn_800D37CC(void);
    extern void menuCreateOffScreen(f32 rate);
    extern u8 menuGetOffScreenFlag(void);
    extern void fn_801661D0(s32, s32, s32, s32);
    extern void fn_800D3074(s32);
    extern u32 fn_801906A0(u32 flag);
    extern void* fn_800FF560(void);
    extern u32 fn_80130CD8(void);
    extern void GSgappBlock(u32 task);
    extern void GSgappUnblock(u32 task);
    extern void GSthreadBlockGroup(void* stack);
    extern void GSthreadCreate(s32 priority, void* stack, u32 stackSize,
                               s32 usesFpu, s32 group, void* entry);
    extern void fn_8000DAB0(void);
    extern void menuClose(s32 menuId);
    extern void menuPokemonOpen(s32, s32, s32);
    extern void menuPdaOpen(void);
    extern u16 fn_80018F88(s32, u32* value, s32);
    extern s32 fn_80019070(u16 handle);
    extern void heroMoveAddAutoEvent(s32 eventId, u16 handle, u32 value,
                                     s32, s32);
    extern s32 fn_801CBAB8(void);
    extern void menuOffScreenRelease(void);
    extern void fn_801660D8(s32, s32, s32);
    extern void menuReleaseOffScreen(f32 rate);
    extern void menuCloseSync(s32 menuId, s32 wait);
    extern void heroMoveTermEvent(void);
    extern u8 lbl_8047A2A0;
    extern s32 lbl_8047A2A4;
    extern void* lbl_8047A2A8;
    extern void* lbl_8047A2AC;
    extern s32 lbl_8047A2B0;
    MenuPendingEvent events[4];
    u32 eventValue;
    u16 selectionHandle;
    s32 eventId;
    s32 eventCount;
    s32 menuId;
    s32 i;

    eventCount = heroMoveCheckEvent(events);
    for (i = 0; i < eventCount; i++) {
        fn_80116D30(4, events[i].eventId);
    }
    heroMoveInitEvent();
    mailMainReceiveTerminate();
    _threadSwitch();
    GSmodelAllPauseAnimation();
    lbl_8047A2A0 = 1;
    menuCreateOffScreen(1.0f / (f32)fn_800D37CC());
    while (!menuGetOffScreenFlag()) {
        _threadSwitch();
    }

    fn_801661D0(0x55, 0x1F4, 1, 1);
    fn_800D3074(1);
    menuId = fn_801906A0(0x8AE) == 0 ? 0x41 : 0x109;
    selectionHandle = 0;
    eventValue = 0;

    if (mode == 0) {
        for (;;) {
            fn_800D3074(1);
            lbl_8047A2A4 = menuId;
            lbl_8047A2A8 = fn_800FF560();
            GSgappBlock(fn_80130CD8());
            GSthreadBlockGroup(lbl_8047A2A8);
            lbl_8047A2AC = (u8*)lbl_8047A2A8 - 0x20;
            GSthreadCreate(0xF, lbl_8047A2AC, 0x4000, 1, 1,
                           fn_8000DAB0);
            _threadSwitch();
            GSgappUnblock(fn_80130CD8());

            if (fn_801906A0(0x8AE) == 0) {
                if (lbl_8047A2B0 == 0) {
                    menuClose(menuId);
                    GSmodelAllUnpauseAnimation();
                    menuPokemonOpen(1, 0, 0);
                    GSmodelAllPauseAnimation();
                    continue;
                }
                if (lbl_8047A2B0 == 1) {
                    menuClose(menuId);
                    GSmodelAllUnpauseAnimation();
                    menuPdaOpen();
                    GSmodelAllPauseAnimation();
                    continue;
                }
                if (lbl_8047A2B0 == 2) {
                    menuClose(menuId);
                    GSmodelAllUnpauseAnimation();
                    selectionHandle = fn_80018F88(0, &eventValue, 0);
                    GSmodelAllPauseAnimation();
                    if (selectionHandle == 0) {
                        continue;
                    }
                    eventId = fn_80019070(selectionHandle);
                    if (eventId != -1) {
                        heroMoveAddAutoEvent(eventId, selectionHandle,
                                             eventValue, 0, 0);
                    }
                }
                break;
            }

            if (lbl_8047A2B0 == 0) {
                menuClose(menuId);
                menuPokemonOpen(1, 0, 0);
                continue;
            }
            if (lbl_8047A2B0 == 1) {
                if (fn_801CBAB8() != 1) {
                    continue;
                }
                menuOffScreenRelease();
                lbl_8047A2A0 = 0;
                _threadSwitch();
                continue;
            }
            break;
        }
    } else if (mode == 1) {
        selectionHandle = fn_80018F88(0, &eventValue, 0);
        if (selectionHandle != 0) {
            eventId = fn_80019070(selectionHandle);
            if (eventId != -1) {
                heroMoveAddAutoEvent(eventId, selectionHandle, eventValue,
                                     0, 0);
            }
        }
    }

    fn_80019070(selectionHandle);
    fn_801660D8(0x1F4, 1, 1);
    menuClose(menuId);
    menuReleaseOffScreen(1.0f / (f32)fn_800D37CC());
    menuCloseSync(menuId, 1);
    fn_800D3074(2);
    heroMoveTermEvent();
    GSmodelAllUnpauseAnimation();
    lbl_8047A2A0 = 0;
}
