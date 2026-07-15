/**
 * @file menu_bios_range_8005D7F8.c
 * @brief menu core bios accessor farm, 0x8005D7F8 - 0x8005DA48.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. menuSeBios/menuSeqBios/menuSpriteBios/
 * menuItemBios/menuDataBios accessor farm; sits inside XD's menu-core TU
 * next to menuPanel-family / menuTool_imasugu-family (XD 0x8007C958-0x8007CD64). 8 XD
 * anchors, all monotonic. All functions asm-only.
 */
#include "dolphin/types.h"

extern u8 *lbl_80478E94;
extern u32 *lbl_80478E90;
extern u32 lbl_80478900;
extern u32 lbl_80478908;
extern u32 lbl_80478968;
extern u32 lbl_80478848;
extern u8 lbl_802E6428[];
extern u8 lbl_802E7CE8[];
extern u8 lbl_802EF0A8[];
extern u8 lbl_802E2DB8[];

void* menuSeBiosGetPtr(s32 idx) {
    if (lbl_80478E94 == NULL) {
        return NULL;
    }
    if ((u32)idx >= *lbl_80478E90) {
        return NULL;
    }
    return lbl_80478E94 + (idx * 0xA);
}

void* menuSeqBiosGetPtr(u32 idx) {
    if (idx >= lbl_80478900) {
        return NULL;
    }
    return lbl_802E6428 + (idx * 0xC);
}

void* menuSpriteBiosGetPtr(u32 idx) {
    if (idx >= lbl_80478908) {
        return NULL;
    }
    return lbl_802E7CE8 + (idx * 0x18);
}

void* menuItemBiosSetXY(s32 idx, s16 x, s16 y) {
    if ((u32)idx >= lbl_80478968) {
        return NULL;
    }
    {
        s16* item = (s16*)(lbl_802EF0A8 + (u32)idx * 0x1C);
        if (item == NULL) {
            return NULL;
        }
        item[1] = x;
        item[2] = y;
        return item;
    }
}

u32 menuItemBiosGetSelectFlag(u32 idx) {
    if (idx >= lbl_80478968) {
        return 0;
    }
    {
        u8* item = lbl_802EF0A8 + (u32)idx * 0x1C;
        if (item == NULL) {
            return 0;
        }
        return item[0] & 0x80 ? 1 : 0;
    }
}

void* menuItemBiosSetSelectFlag(u32 idx, u32 flag) {
    if (idx >= lbl_80478968) {
        return NULL;
    }
    {
        u8* item = lbl_802EF0A8 + (u32)idx * 0x1C;
        if (item == NULL) {
            return NULL;
        }
        item[0] = (item[0] & 0x7F) | (u8)((flag << 7) & 0x80);
        return item;
    }
}

void* menuItemBiosGetPtr(s32 idx) {
    if ((u32)idx >= lbl_80478968) {
        return NULL;
    }
    return lbl_802EF0A8 + idx * 0x1C;
}

void menuDataBiosGetXY(s32 idx, s16* x, s16* y) {
    s16* data;

    if (idx < 0) {
        idx = 0;
    }
    if ((u32)idx >= lbl_80478848) {
        idx = 1;
    }
    data = (s16*)(lbl_802E2DB8 + idx * 0x1C);
    if (x != NULL) {
        *x = data[3];
    }
    if (y != NULL) {
        *y = data[4];
    }
}

u32 menuDataBiosSetXY(s32 idx, s16 x, s16 y) {
    if (idx < 0) {
        idx = 0;
    } else {
        if ((u32)idx >= lbl_80478848) {
            idx = 1;
        }
    }
    {
        s16* data = (s16*)(lbl_802E2DB8 + idx * 0x1C);
        data[3] = x;
        data[4] = y;
    }
    return (u32)idx;
}

u32 menuDataBiosGetType(s32 idx) {
    if (idx < 0) {
        idx = 0;
    } else if ((u32)idx >= lbl_80478848) {
        idx = 1;
    }
    return ((u8*)(lbl_802E2DB8 + (u32)idx * 0x1C))[0x2];
}

void* menuDataBiosGetPtr(s32 idx) {
    if (idx < 0) {
        idx = 0;
    } else if ((u32)idx >= lbl_80478848) {
        idx = 1;
    }
    return (void*)(lbl_802E2DB8 + (u32)idx * 0x1C);
}
