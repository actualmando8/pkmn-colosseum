/**
 * @file menu_debug_range_8005DA48.c
 * @brief debug menu TU, 0x8005DA48 - 0x8005DFC8.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. dbgMenuLog-family / dbgMenuFieldCamera-family /
 * menuDbgItem-family cluster in XD's dbg menu source (XD 0x8000DEE8-0x8000E53C). 2 XD anchors
 * (dbgMenuFieldCameraChangeDisp, menuDbgItemCreate), monotonic with exact
 * size matches. All functions asm-only.
 */
#include "dolphin/types.h"

#pragma peephole off
s32 dbgMenuLogChangeDisp(void)
{
    extern u8 menuIsCheck(s32 menuId);
    extern void menuClose(s32 menuId);
    extern s32 menuOpenCustom(s32 menuId, ...);

    if (menuIsCheck(0xBB)) {
        menuClose(0xBB);
    } else {
        menuOpenCustom(0xBB, 0, 0, 0, 1, 0);
    }

    return 0;
}
#pragma peephole reset

#pragma peephole off
s32 dbgMenuFieldCameraChangeDisp(void)
{
    extern u8 menuIsCheck(s32 menuId);
    extern s32 menuOpenCustom(s32 menuId, ...);
    extern void menuSetPosition(s32 menuId, s32 x, s32 y);
    extern u32 fn_800FF56C(void);
    extern void fn_801176C8(u32 floorId);
    extern u32 fn_80117AD4(void);
    extern u32 lbl_8047A5B0;
    extern u32 lbl_8047A5B4;
    extern u32 lbl_8047A5B8;
    extern u8 lbl_8047A5BC;
    u32 floorId;

    if (!menuIsCheck(0xCA)) {
        floorId = fn_800FF56C();
        if (floorId != fn_80117AD4()) {
            fn_801176C8(floorId);
            lbl_8047A5B0 = 0;
            lbl_8047A5B4 = 0;
            lbl_8047A5B8 = 0;
        }

        lbl_8047A5BC = 0;
        menuOpenCustom(0xCA, 0, 0, 0, 1, 0);
        menuSetPosition(0xCA, 0xC, 0xA);
    }

    return 0;
}
#pragma peephole reset

#pragma peephole off
void menuDbgItemCreate(void)
{
    extern s32 menuOpen(s32 menuId, s32 mode);
    extern u8 fn_80142984(u16 itemId);
    extern void menuCloseCustom(s32 menuId, s32 mode, s32 wait);
    extern s32 heroItemAddItemDataId(void* hero, u16 itemId, u16 count,
                                     s32 slot);
    extern void menuClose(s32 menuId);
    extern void menuCloseSync(s32 menuId, s32 wait);
    extern s32 lbl_8047A5C0;
    extern s32 lbl_8047A5C4;
    extern s32 lbl_8047A5C8;
    s32 result;

    if (lbl_8047A5C0 == 0) {
        lbl_8047A5C8 = 1;
        lbl_8047A5C0 = 1;
    }

    lbl_8047A5C4 = 1;
    for (;;) {
        result = menuOpen(0xCB, 1);
        if (result == -1) {
            break;
        }
        if (fn_80142984((u16)lbl_8047A5C8) == 0) {
            continue;
        }
        if (lbl_8047A5C4 < 1 || lbl_8047A5C4 > 999) {
            continue;
        }

        result = menuOpen(0x44, 1);
        menuCloseCustom(0x44, 0, 1);
        if (result == 0) {
            heroItemAddItemDataId(0, (u16)lbl_8047A5C8,
                                  (u16)lbl_8047A5C4, -1);
        }
    }

    menuClose(0xCB);
    menuCloseSync(0xCB, 1);
}
#pragma peephole reset
