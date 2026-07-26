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
