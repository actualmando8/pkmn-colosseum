/**
 * @file menuCB_range_8005344C.c
 * @brief colosseum-battle Pokemon-select-from-PC screens, 0x8005344C - 0x80055E38.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. PCBOX-centric (getPokemon/setPokemon/delPokemon,
 * pcboxGetNbPokemonBox), menuModelCheck/Render, itemDataBiosGetName,
 * winSpriteSetDisp disp-subs + windowGetKeyInfo ctrl fns. Identity SPECULATIVE
 * (0 XD anchors; structural-family evidence only). All functions asm-only.
 */
#include "dolphin/types.h"

typedef struct MenuCBPane MenuCBPane;
typedef struct MenuCBSlotInfo MenuCBSlotInfo;
typedef struct MenuCBState MenuCBState;
typedef struct MenuKeyInfo MenuKeyInfo;
typedef struct MenuModelWork MenuModelWork;
typedef struct MenuCBLayoutEntry MenuCBLayoutEntry;
typedef struct MenuCBStatusWork MenuCBStatusWork;
typedef struct MenuCursorItem MenuCursorItem;

struct MenuCBState {
    s32 markKind;
    s32 cursorKind;
};

struct MenuCBPane {
    u8 pad_00[0x4c];
    s32 textId;
    s16 x;
    s16 y;
    s16 width;
    s16 height;
    u8 pad_58[0x8];
    MenuCBState* state;
    u8 pad_64[0x3];
    u8 alpha;
    u8 pad_68[0x2d];
    s8 boxIndex;
    u8 pad_96;
    s8 previousBoxIndex;
    u8 flag98;
};

struct MenuCBSlotInfo {
    s32 itemId;
    s32 kind;
    s32 slot;
};

struct MenuKeyInfo {
    u16 buttons;
    u16 buttonsPrev;
    u16 buttonsDown;
    u16 buttonsRepeat;
};

struct MenuModelWork {
    u8 pad_00[0x48];
};

struct MenuCBLayoutEntry {
    s32 id;
    s16 offset;
    s16 pad_06;
};

struct MenuCBStatusWork {
    u8 pad_00;
    s8 state;
    s8 initialized;
    u8 pad_03;
    s32 windowId;
};

struct MenuCursorItem {
    u8 pad_00[2];
    s16 x;
    s16 y;
};

extern f32 lbl_8047A54C;
extern void* lbl_8047A548;
extern f32 lbl_8047A554;
extern f32 lbl_8047A558;
extern const f32 lbl_8047BE60;
extern const f32 lbl_8047BE68;
extern const f32 lbl_8047BE8C;
extern const f32 lbl_8047BE90;
extern const f32 lbl_8047BE94;
extern const f32 lbl_8047BE80;
extern const MenuCBLayoutEntry lbl_802E61E8[17];
extern const u32 lbl_80267350[18];
extern u8 lbl_802EF0A8[];
extern MenuCBSlotInfo lbl_80267398[0x20];
extern MenuModelWork lbl_803A9720;

extern void* fn_80057270(MenuCBPane* pane);
extern s32 fn_800573C0(void);
extern s32 fn_800566E8(void);
extern s32 fn_80057E40(MenuCBPane* pane);
extern void fn_800FB680(s32 x, s32 y, s32 color, u32 msgId);
extern u32 GSmsgGetGSchar(u32 msgId);
extern u32 GSmsgGetRect(u32 msgId);
extern void* itemDataBiosGetPtr(u16 itemId);
extern u32 itemDataBiosGetName(void* itemData);
extern s32 menuCloseCustom(s32 menuId, s32 mode, s32 wait);
extern s32 menuCloseSync(s32 menuId, s32 wait);
extern void menuClose(s32 menuId);
extern void menuOpen(s32 menuId, s32 wait);
extern s32 menuOpenCustom(s32 menuId, s32 owner, s32 arg2, s32 arg3, s32 arg4, s32 arg5, ...);
extern void menuButtonNormal(MenuCBPane* pane);
extern u8 menuSubGetPokemonSexForDisp(void* pokemon);
extern void msgctrlSetValue(s32 id, u32 value);
extern void _threadSwitch(void);
extern u8 pokemonBiosGetLevel(void* pokemon);
extern u8 pokemonBiosGetPcboxMark(void* pokemon);
extern u16 pokemonBiosGetPokemonDataId(void* pokemon);
extern void* pokemonBiosGetNicknamePtr(void* pokemon);
extern u32 pokemonDataBiosGetName(void* pokemonData);
extern void* pokemonDataBiosGetPtr(u16 dataId);
extern u16 pokemonGetSoubiItemDataId(void* pokemon);
extern void* pcboxGetPokemonBoxName(void* pcbox, s8 box);
extern s8 pcboxGetNbPokemonBox(void);
extern s8 getPokemonBoxNbUsedSlot__5PCBOXFSc(void* pcbox, s8 box);
extern void winSpriteSetDisp(MenuCBPane* pane, u8 enable);
extern MenuKeyInfo* windowGetKeyInfo(void);
extern void fn_8010A420(MenuModelWork* work);
extern void fn_80054760(s32 forward, s32 wait);
extern void fn_800558B8(void);
extern void fn_80056A80(void);
extern void fn_80056B74(MenuCBPane* pane, s32 enabled);
extern void fn_80057A38(void);
extern void fn_80057A64(void* pokemon, s32 arg1);
extern s8 fn_801347D8(void);
extern s32 fn_80055E38(MenuCBPane* pane);

#pragma push
#pragma peephole off
s32 fn_8005344C(MenuCBPane* pane, MenuCBPane* sprite) {
    s16 position;
    s16 x;
    s16 y;
    s32 visible;
    MenuCBPane* window;

    extern s32 fn_80057A08(MenuCBPane* pane);
    extern void* windowSearchID(s32 id);
    extern s32 fn_80058F08(s16* position, s32 box);
    extern void fn_80057094(s16* x, s16* y);

    visible = FALSE;
    if (fn_80057A08(pane) != 0) {
        window = windowSearchID(0x94);
        if (window != NULL && fn_80058F08(&position, window->boxIndex) == 0) {
            visible = TRUE;
            fn_80057094(&x, &y);
            sprite->x = x + *(s16*)(lbl_802EF0A8 + (*(s16*)((u8*)sprite + 6) * 0x1c) + 2);
            sprite->y = y + *(s16*)(lbl_802EF0A8 + (*(s16*)((u8*)sprite + 6) * 0x1c) + 4);
        }
    }
    winSpriteSetDisp(sprite, (u8)visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053728(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;

    if (fn_80057E40(pane) != 2) {
        visible = TRUE;
    } else {
        visible = FALSE;
    }
    winSpriteSetDisp(sprite, (u8)visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053A60(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;
    void* pokemon;

    visible = FALSE;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        u32 mark = (u8)pokemonBiosGetPcboxMark(pokemon);
        s32 masked = mark & 8;

        if (masked != 0) {
            visible = TRUE;
        }
    }
    winSpriteSetDisp(sprite, visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053AC8(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;
    void* pokemon;

    visible = FALSE;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        u32 mark = (u8)pokemonBiosGetPcboxMark(pokemon);
        s32 masked = mark & 4;

        if (masked != 0) {
            visible = TRUE;
        }
    }
    winSpriteSetDisp(sprite, visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053B30(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;
    void* pokemon;

    visible = FALSE;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        u32 mark = (u8)pokemonBiosGetPcboxMark(pokemon);
        s32 masked = mark & 2;

        if (masked != 0) {
            visible = TRUE;
        }
    }
    winSpriteSetDisp(sprite, visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053B98(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;
    void* pokemon;

    visible = FALSE;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        u32 mark = (u8)pokemonBiosGetPcboxMark(pokemon);
        s32 masked = mark & 1;

        if (masked != 0) {
            visible = TRUE;
        }
    }
    winSpriteSetDisp(sprite, visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053C00(MenuCBPane* pane, MenuCBPane* sprite) {
    void* pokemon;
    u16 itemId;
    void* itemData;
    s32 result;

    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        itemId = pokemonGetSoubiItemDataId(pokemon);
        if (itemId != 0) {
            itemData = itemDataBiosGetPtr(itemId);
            if (itemData != NULL) {
                msgctrlSetValue(0x37, GSmsgGetGSchar(itemDataBiosGetName(itemData)));
                fn_800FB680(0, 0, -1, 0xe7);
            }
        }
    }
    result = 0;
    sprite->textId = result;
    return result;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053C84(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 visible;
    void* pokemon;
    u32 itemId;

    visible = FALSE;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        itemId = (u16)pokemonGetSoubiItemDataId(pokemon);
        if (itemId != 0) {
            visible = TRUE;
        }
    }
    winSpriteSetDisp(sprite, visible);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053CE8(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 textId;
    void* pokemon;

    textId = 0;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        switch ((u8)menuSubGetPokemonSexForDisp(pokemon)) {
        case 0:
            textId = 0xd67;
            break;
        case 1:
            textId = 0xd68;
            break;
        case 2:
            break;
        }
    }
    sprite->textId = textId;
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053D64(MenuCBPane* pane, MenuCBPane* sprite) {
    void* pokemon;
    s32 result;

    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        msgctrlSetValue(0x37, GSmsgGetGSchar(pokemonDataBiosGetName(pokemonDataBiosGetPtr(pokemonBiosGetPokemonDataId(pokemon)))));
        fn_800FB680(0, 0, -1, 0xe7);
    }
    result = 0;
    sprite->textId = result;
    return result;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80053DD4(MenuCBPane* pane, MenuCBPane* sprite) {
    void* pokemon;
    u32 level;
    s32 textId;
    u32 rect;

    textId = 0;
    pokemon = fn_80057270(pane);
    if (pokemon != NULL) {
        level = pokemonBiosGetLevel(pokemon);
        if ((s32)level < 100) {
            textId = 2;
        } else {
            textId = 3;
        }
        rect = GSmsgGetRect(0x1b82);
        fn_800FB680(sprite->width - (textId * 15) - (rect >> 16), 0, -1, 0x1b82);
        msgctrlSetValue(0x34, level);
        textId = 0xde;
    }
    sprite->textId = textId;
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80054420(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 i;
    s32 scaled;
    s16 offset;

    for (i = 0; i < 17; i++) {
        if (*(s16*)((u8*)sprite + 6) == lbl_802E61E8[i].id) {
            break;
        }
    }
    if (i >= 17) {
        return 0;
    }
    offset = lbl_802E61E8[i].offset;
    scaled = (s32)(lbl_8047BE80 * lbl_8047A558);
    sprite->y = scaled + offset;
    return 0;
}
#pragma pop

s32 fn_80053E7C(MenuCBPane* pane) {
    void* pokemon;
    void* nickname;

    pokemon = fn_80057270(pane);
    if (pokemon == NULL) {
        return 0;
    }
    nickname = pokemonBiosGetNicknamePtr(pokemon);
    msgctrlSetValue(0x37, (u32)nickname);
    fn_800FB680(0, 0, -1, 0xe7);
    return 0;
}

s32 fn_8005464C(void) {
    return !(lbl_8047A54C >= lbl_8047BE8C);
}

void fn_80054670(void* ptr) {
    lbl_8047A548 = ptr;
    lbl_8047A54C = lbl_8047BE68;
}

s32 fn_80054680(void) {
    if (lbl_8047A554 > lbl_8047BE68) {
        return 3;
    }
    if (lbl_8047A554 < lbl_8047BE68) {
        return 2;
    }
    return lbl_8047A558 >= lbl_8047BE60;
}

void fn_800546C0(s32 forward) {
    if (forward != 0) {
        lbl_8047A558 = lbl_8047BE60;
        lbl_8047A554 = lbl_8047BE68;
        return;
    }

    lbl_8047A558 = lbl_8047BE68;
    lbl_8047A554 = lbl_8047BE90;
}

void fn_800546F0(s32 forward) {
    if (forward != 0) {
        lbl_8047A558 = lbl_8047BE68;
        lbl_8047A554 = lbl_8047BE68;
        return;
    }

    lbl_8047A558 = lbl_8047BE60;
    lbl_8047A554 = lbl_8047BE94;
}

#pragma push
#pragma scheduling off
#pragma dont_inline on
void fn_8005471C(void) {
    menuCloseCustom(0x8f, 2, 0);
    menuCloseSync(0x8f, 1);
    fn_8010A420(&lbl_803A9720);
}
#pragma dont_inline reset

#pragma push
#pragma peephole off
s32 fn_80054914(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 ids[6][3] = {
        {0x119d, 0x119e, 0x119f},
        {0x713, 0x714, 0x715},
        {0x716, 0x717, 0x718},
        {0x719, 0x71a, 0x71b},
        {0x71c, 0x71d, 0x71e},
        {0x71f, 0x720, 0x721}
    };
    s32 column;
    s32 row;
    s16 id;

    for (row = 0; row < 6; row++) {
        id = *(s16*)((u8*)sprite + 6);
        for (column = 0; column < 3; column++) {
            if (id == ids[row][column]) {
                break;
            }
        }
        if (column < 3) {
            break;
        }
    }
    if (row >= 6) {
        return 0;
    }
    if (pane->boxIndex == row) {
        winSpriteSetDisp(sprite, TRUE);
    } else {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}
#pragma pop

s32 fn_800549F0(MenuCBPane* pane, MenuCBPane* sprite) {
    MenuCBState* state = pane->state;

    if (state->cursorKind != 2) {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}

s32 fn_80054A2C(MenuCBPane* pane, MenuCBPane* sprite) {
    MenuCBState* state = pane->state;

    if (state->cursorKind != 1) {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}

s32 fn_80054A68(MenuCBPane* pane, MenuCBPane* sprite) {
    MenuCBState* state = pane->state;

    if (state->cursorKind != 0) {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}

s32 fn_80054AA4(MenuCBPane* pane, MenuCBPane* sprite) {
    MenuCBState* state = pane->state;

    if (state->markKind == 0) {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}

s32 fn_80054AE0(MenuCBPane* pane, MenuCBPane* sprite) {
    MenuCBState* state = pane->state;

    if (state->markKind != 0) {
        winSpriteSetDisp(sprite, FALSE);
    }
    return 0;
}
#pragma pop

void fn_80054E7C(MenuCBPane* pane) {
    windowGetKeyInfo();
    if (fn_800573C0() == 0) {
        if (fn_800566E8() == 0) {
            menuButtonNormal(pane);
        }
    }
}

#pragma push
#pragma peephole off
s32 fn_800550B4(MenuCBStatusWork* work) {
    MenuCursorItem* item;
    s32 selected;

    extern MenuCursorItem* windowGetCursorToItem(MenuCBStatusWork* work);
    extern void fn_80057830(s16 x, s16 y, s32 selected);
    extern void fn_80057948(void);
    extern void fn_80056854(void);
    extern s32 windowGetActiveID(void);
    extern void fn_80054EC8(MenuCBStatusWork* work);

    switch (work->state) {
    case 0:
        if (work->initialized == 0) {
            selected = TRUE;
        } else {
            selected = FALSE;
        }
        item = windowGetCursorToItem(work);
        fn_80057830(item->x, item->y, selected);
        work->initialized = 1;
        break;
    case 2:
        fn_80057948();
        fn_80056854();
        if (work->windowId == windowGetActiveID()) {
            fn_80054EC8(work);
        }
        break;
    case 3:
        if (work->initialized == 0) {
            work->initialized = 1;
        }
        break;
    }
    return 0;
}
#pragma pop

s32 fn_80055194(s32* outSlot, s32 index) {
    MenuCBSlotInfo* info;

    if (index < 0 || index >= 0x20) {
        return 3;
    }

    info = &lbl_80267398[index];
    *outSlot = info->slot;
    return info->kind;
}

#pragma push
#pragma peephole off
void fn_80055B98(MenuCBPane* pane) {
    menuOpenCustom(0x10e, 0x1f, 0, 0, 0, 0);
    fn_80056B74(pane, TRUE);

    while (fn_80055E38(pane) == 0) {
        _threadSwitch();
    }

    fn_80054760(FALSE, TRUE);
    fn_80057A64(NULL, 0);
    fn_800558B8();
    fn_80057A38();
    fn_8005471C();
    fn_80056A80();
}
#pragma pop

#pragma push
#pragma peephole off
s32 fn_80055C2C(MenuCBPane* pane, MenuCBPane* sprite) {
    s32 box;
    s32 capacity;
    s32 used;

    box = pane->boxIndex;
    if (box < 0 || box >= pcboxGetNbPokemonBox()) {
        return 0;
    }

    capacity = fn_801347D8();
    used = getPokemonBoxNbUsedSlot__5PCBOXFSc(NULL, box);
    if (used < 0) {
        return 0;
    }

    msgctrlSetValue(0x34, used);
    msgctrlSetValue(0x35, capacity);
    sprite->textId = 0x1b7f;
    return 0;
}
#pragma pop

s32 fn_80055CD4(MenuCBPane* pane, MenuCBPane* sprite) {
    u8 box;
    u32 name;
    s32 result;

    box = pane->boxIndex;
    name = (u32)pcboxGetPokemonBoxName(NULL, (s8)box);
    if (name == 0) {
        return 0;
    }

    msgctrlSetValue(0x37, name);
    result = 0;
    sprite->textId = 0xcf;
    return result;
}

#pragma push
#pragma peephole off
#pragma scheduling off
s32 fn_80055D34(MenuCBPane* pane) {
    MenuKeyInfo* keyInfo;
    s8 count;
    s32 box;

    keyInfo = windowGetKeyInfo();
    if ((keyInfo->buttonsRepeat & 8) != 0) {
        count = pcboxGetNbPokemonBox();
        box = (u8)pane->boxIndex + 1;
        pane->boxIndex = box;
        if ((s8)box >= count) {
            pane->boxIndex = 0;
        }
    }
    if ((keyInfo->buttonsRepeat & 4) != 0) {
        box = (u8)pane->boxIndex - 1;
        pane->boxIndex = box;
        if ((s8)box < 0) {
            pane->boxIndex = pcboxGetNbPokemonBox() - 1;
        }
    }
    return 0;
}
#pragma pop

#pragma push
#pragma scheduling off
void fn_80055DE0(void) {
    menuClose(0xa3);
    menuCloseSync(0xa3, 1);
}

void fn_80055E10(void) {
    menuOpen(0xa3, 1);
}
#pragma pop
