/**
 * @file menuCB_range_80055E38.c
 * @brief colosseum-battle team/status display screens, 0x80055E38 - 0x80057B34.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. menuSetDisp/winSeq screens, pokemon status
 * drawing via windowDrawSprite2. Identity SPECULATIVE (0 XD anchors;
 * structural-family evidence only: distinct .data pool band and static bss
 * 0x803A9768 shared across this range).
 *
 * fn_80056A78 (0x80056A78): trivial sda_getter, ported from the previous
 * campaign's archive/previous_campaign/src/game/menu/menu_status.c.
 * Remainder of the range is asm-only.
 */
#include "dolphin/types.h"

/* ===== SDA globals ===== */
extern u32 lbl_8047A584;
extern f32 lbl_8047BEC0;
extern f32 lbl_8047BEC4;
extern f32 lbl_8047A570;
extern f32 lbl_8047A578;
extern f32 lbl_8047A588;
extern f32 lbl_8047BF00;
extern f32 lbl_8047BF04;
extern u32 lbl_8047A56C;
extern u8 lbl_803A9768[];

extern void fn_80056C54(u8*, u8*, u32);
extern s32 menuCloseCustom(s32 menuId, s32 mode, s32 wait);

/* ===== Function implementations ===== */

#pragma optimization_level 4
u32 fn_80055E38(s32 idx) {
    extern const u32 lbl_8026768C[];
    extern s32 winSeqCheckMove(s32);
    u32 tbl[3];
    s32 val;

    tbl[0] = lbl_8026768C[0];
    tbl[1] = lbl_8026768C[1];
    tbl[2] = lbl_8026768C[2];
    if (idx < 0 || idx >= 3) {
        val = -1;
    } else {
        val = (s32)tbl[idx];
    }
    if (val < 0) {
        return 1;
    }
    return (u8)winSeqCheckMove(val) == 0;
}

#pragma push
#pragma peephole off
/* 0x80055EB8 | 0xD0 */
u32 fn_80055EB8(s8* ctx, u8* p) {
    extern s32 fn_80057A08(void);
    extern void* windowSearchID(s32);
    extern s32 fn_80055194(u32*, s32);
    extern void fn_80057094(s16*, s16*);
    extern void winSpriteSetDisp(void*, u32);
    extern u8 lbl_802EF0A8[];
    u8 result;
    u32 out;
    s16 x;
    s16 y;

    result = 0;
    if (fn_80057A08() != 0) {
        ctx = (s8*)windowSearchID(0x93);
        if (ctx != 0) {
            if (fn_80055194(&out, ctx[0x95]) == 0) {
                result = 1;
                fn_80057094(&x, &y);
                *(s16*)(p + 0x50) =
                    (s16)(x + *(s16*)(lbl_802EF0A8 + *(s16*)(p + 6) * 0x1c + 2));
                *(s16*)(p + 0x52) =
                    (s16)(y + *(s16*)(lbl_802EF0A8 + *(s16*)(p + 6) * 0x1c + 4));
            }
        }
    }
    winSpriteSetDisp(p, result);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
u32 fn_80055F88(u8* ctx, u8* p) {
    extern const s32 lbl_80267680[];
    extern char* pcboxGetPokemonBoxName(s32, s8);
    extern void msgctrlSetValue(s32, char*);
    extern u32 GSmsgGetRect(s32);
    extern void fn_800FB680(s32, s32, s32, u32);
    char* name;
    const s32* table;
    u32 rect;
    s32 pageId;
    s32 page;

    table = lbl_80267680;
    pageId = *(s16*)(p + 6);
    if (pageId == *table) {
        page = 0;
    } else {
        table++;
        if (pageId == *table) {
            page = 1;
        } else {
            table++;
            if (pageId == *table) {
                page = 2;
            } else {
                page = -1;
            }
        }
    }
    if (page < 0) {
        return 0;
    }
    name = pcboxGetPokemonBoxName(0, (s8)page);
    if (name == 0) {
        return 0;
    }
    msgctrlSetValue(0x37, name);
    rect = GSmsgGetRect(0xce);
    fn_800FB680((s16)(*(s16*)(p + 0x54) / 2 - (s16)(rect >> 16) / 2), 0,
                 -1, 0xce);
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
u32 fn_80056610(u8* ctx) {
    extern s32 lbl_8047A568;
    extern void winSeqSetMenu(s32, u32);

    switch ((s8)ctx[1]) {
    case 0:
        if ((s8)ctx[2] == 0) {
            if (lbl_8047A568 != 0) {
                winSeqSetMenu(*(s32*)(ctx + 4), 0x107);
            }
            ctx[2] = 1;
        }
        break;
    case 3:
        if ((s8)ctx[2] == 0) {
            winSeqSetMenu(*(s32*)(ctx + 4), 0x10b);
            ctx[2] = 1;
        }
        break;
    }
    return 0;
}
#pragma pop

u32 fn_80056A78(void) {
    return lbl_8047A584;
}

void fn_80056A80(void) {
    extern u8 lbl_8026768C[];
    extern s32 menuCloseSync(s32, s32);
    s32 i;
    s32 val;
    u32 tbl[3];
    u32* p;

    p = tbl;
    i = 0;
    while (i < 3) {
        u32* src = (u32*)lbl_8026768C;

        tbl[0] = src[0];
        tbl[1] = src[1];
        tbl[2] = src[2];
        if (i < 0 || i >= 3) {
            val = -1;
        } else {
            val = (s32)*p;
        }
        if (val >= 0) {
            menuCloseCustom(val, 2, 0);
        }
        p++;
        i++;
    }
    {
        u32 tbl2[3];
        s32 idx;
        u32* src = (u32*)lbl_8026768C;

        tbl2[0] = src[0];
        tbl2[1] = src[1];
        tbl2[2] = src[2];
        idx = (s32)lbl_8047A584;
        if (idx < 0 || idx >= 3) {
            val = -1;
        } else {
            val = (s32)tbl2[idx];
        }
        if (val >= 0) {
            menuCloseSync(val, 1);
        }
    }
}

u32 fn_80056B74(s32 idx, s32 mode) {
    extern u8 lbl_8026768C[];
    extern u32 lbl_8047A568;
    extern u32 lbl_8047A580;
    extern f32 lbl_8047A57C;
    extern f32 lbl_8047A574;
    extern f32 lbl_8047BEB4;
    extern s32 menuOpenCustom(s32, ...);
    extern void menuSetDisp(s32, u32);
    s32 val;
    u32* p;
    s32 i;
    u32 tbl[3];

    lbl_8047A568 = (u32)mode;
    p = tbl;
    i = 0;
    while (i < 3) {
        tbl[0] = ((u32*)lbl_8026768C)[0];
        tbl[1] = ((u32*)lbl_8026768C)[1];
        tbl[2] = ((u32*)lbl_8026768C)[2];
        if (i < 0 || i >= 3) {
            val = -1;
        } else {
            val = (s32)*p;
        }
        if (val >= 0) {
            menuOpenCustom(val, 0x1f, 0, 0, 0, 0);
        }
        if (idx != i) {
            menuSetDisp(val, 0);
        }
        p++;
        i++;
    }
    lbl_8047A584 = idx;
    lbl_8047A580 = idx;
    lbl_8047A57C = lbl_8047BEC0;
    lbl_8047A578 = lbl_8047BEC0;
    lbl_8047A574 = lbl_8047BEC0;
    lbl_8047A570 = lbl_8047BEB4;
    return 1;
}

u32 fn_800566B4(void) {
    return !(lbl_8047A570 >= lbl_8047BEC4);
}

void fn_800566D8(u32 a) {
    lbl_8047A56C = a;
    lbl_8047A570 = lbl_8047BEC0;
}

u32 fn_800566E8(void) {
    return lbl_8047BEC0 != lbl_8047A578;
}

#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80056704(void) {
    extern const u32 lbl_8026768C[];
    extern u32 lbl_8047A580;
    extern f32 lbl_8047A57C;
    extern f32 lbl_8047BEC8;
    extern void menuSetDisp(s32, u32);
    u32 tbl[3];
    u32 cur;
    s32 val;

    lbl_8047A580 = lbl_8047A584;
    lbl_8047A584 = lbl_8047A584 - 1;
    if ((s32)lbl_8047A584 < 0) {
        lbl_8047A584 = 2;
    }
    cur = lbl_8047A584;
    tbl[0] = lbl_8026768C[0];
    tbl[1] = lbl_8026768C[1];
    tbl[2] = lbl_8026768C[2];
    if ((s32)cur < 0 || (s32)cur >= 3) {
        val = -1;
    } else {
        val = (s32)tbl[cur];
    }
    if (val >= 0) {
        menuSetDisp(val, 1);
    }
    lbl_8047A57C = lbl_8047BEC0;
    lbl_8047A578 = lbl_8047BEC8;
    return lbl_8047A584;
}
#pragma pop

#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_800567AC(void) {
    extern const u32 lbl_8026768C[];
    extern u32 lbl_8047A580;
    extern f32 lbl_8047A57C;
    extern f32 lbl_8047BECC;
    extern void menuSetDisp(s32, u32);
    u32 tbl[3];
    u32 cur;
    s32 val;

    lbl_8047A580 = lbl_8047A584;
    lbl_8047A584 = lbl_8047A584 + 1;
    if ((s32)lbl_8047A584 >= 3) {
        lbl_8047A584 = 0;
    }
    cur = lbl_8047A584;
    tbl[0] = lbl_8026768C[0];
    tbl[1] = lbl_8026768C[1];
    tbl[2] = lbl_8026768C[2];
    if ((s32)cur < 0 || (s32)cur >= 3) {
        val = -1;
    } else {
        val = (s32)tbl[cur];
    }
    if (val >= 0) {
        menuSetDisp(val, 1);
    }
    lbl_8047A57C = lbl_8047BEC0;
    lbl_8047A578 = lbl_8047BECC;
    return lbl_8047A584;
}
#pragma pop

#pragma optimization_level 4
void fn_80057094(s16* a, s16* b) {
    *a = (s16)(s32)*(f32*)(lbl_803A9768 + 0x27c);
    *b = (s16)(s32)*(f32*)(lbl_803A9768 + 0x280);
}

#pragma scheduling off
u32 fn_800570D0(u8* a, u8* b) {
    fn_80056C54(a, b, (*(s32*)(lbl_803A9768 + 0x278) + 1) % 2);
    return 0;
}

u32 fn_80057114(u8* a, u8* b) {
    fn_80056C54(a, b, *(u32*)(lbl_803A9768 + 0x278));
    return 0;
}

u32 fn_800573C0(void) {
    s32 state;

#pragma scheduling on
#pragma optimization_level 4
    if (*(f32*)(lbl_803A9768 + 0x288) <= lbl_8047BF00) {
        state = *(s32*)lbl_803A9768;
        if (state == 0 || state == 3) {
            return 0;
        }
    }
    return 1;
}

void fn_80057400(void) {
    s32 value;

    value = (s32)(*(u32*)(lbl_803A9768 + 0x278) + 1);
    *(u32*)(lbl_803A9768 + 0x278) = value;
    if (value > 1) {
        *(u32*)(lbl_803A9768 + 0x278) = 0;
    }
}

u32 fn_80057428(void) {
    return !(lbl_8047A588 >= lbl_8047BF04);
}

void fn_8005744C(void) {
    lbl_8047A588 = lbl_8047BF00;
}

typedef struct {
    u32 data[78];
} Tbl78;

void fn_80057458(u8* src) {
    Tbl78* dstState;
    u32 slot;

    slot = (*(s32*)(lbl_803A9768 + 0x278) + 1) % 2;
    dstState = (Tbl78*)(lbl_803A9768 + slot * 0x138 + 8);
    *dstState = *(Tbl78*)src;
}

#pragma push
#pragma scheduling off
#pragma optimization_level 4
void fn_800574A8(void) {
    pokemonInit((u8*)(lbl_803A9768 + *(u32*)(lbl_803A9768 + 0x278) * 0x138 + 8));
}
#pragma pop

u8* fn_800574E0(void) {
    return lbl_803A9768 + *(u32*)(lbl_803A9768 + 0x278) * 0x138 + 8;
}

typedef struct {
    u32 data[78];
} Tbl78;

void fn_800574FC(u8* src) {
    Tbl78* dstState;
    Tbl78* srcState;
    u32 slot;

    slot = *(u32*)(lbl_803A9768 + 0x278);
    dstState = (Tbl78*)(lbl_803A9768 + slot * 0x138 + 8);
    srcState = (Tbl78*)src;
    *dstState = *srcState;
}

u32 fn_80057694(void) {
    return *(u32*)(lbl_803A9768 + 4);
}

void fn_800576A4(u32 a) {
    *(u32*)(lbl_803A9768 + 4) = a;
}

u32 fn_800576B4(void) {
    return *(u32*)(lbl_803A9768 + 0);
}

void fn_80057948(void) {
    extern f32 lbl_8047A58C;
    extern f32 lbl_8047BEF4;
    extern f32 lbl_8047BF0C;
    f32 progress;

    if (*(f32*)(lbl_803A9768 + 0x288) > lbl_8047BF00) {
        progress = *(f32*)(lbl_803A9768 + 0x284) +
                   *(f32*)(lbl_803A9768 + 0x288);
        *(f32*)(lbl_803A9768 + 0x284) = progress;
        if (progress >= lbl_8047BEF4) {
            *(f32*)(lbl_803A9768 + 0x284) = lbl_8047BEF4;
            *(f32*)(lbl_803A9768 + 0x288) = lbl_8047BF00;
        }
        *(f32*)(lbl_803A9768 + 0x27c) =
            *(f32*)(lbl_803A9768 + 0x28c) +
            *(f32*)(lbl_803A9768 + 0x284) *
                (*(f32*)(lbl_803A9768 + 0x294) -
                 *(f32*)(lbl_803A9768 + 0x28c));
        *(f32*)(lbl_803A9768 + 0x280) =
            *(f32*)(lbl_803A9768 + 0x290) +
            *(f32*)(lbl_803A9768 + 0x284) *
                (*(f32*)(lbl_803A9768 + 0x298) -
                 *(f32*)(lbl_803A9768 + 0x290));
    }

    lbl_8047A58C += lbl_8047BF0C;
    if (lbl_8047A58C > lbl_8047BEF4) {
        lbl_8047A58C -= lbl_8047BEF4;
    }
    if (lbl_8047A588 < lbl_8047BEF4) {
        lbl_8047A588 += lbl_8047BF0C;
        if (lbl_8047A588 > lbl_8047BEF4) {
            lbl_8047A588 = lbl_8047BEF4;
        }
    }
}

#pragma push
#pragma peephole off
#pragma optimization_level 4
s32 fn_80057A08(void) {
    return fn_80104704(0xa0) != 0;
}

void fn_80057A38(void) {
    menuCloseCustom(0xa0, 2, 1);
}
#pragma pop

#pragma push
#pragma peephole off
#pragma optimization_level 4
void fn_80057A64(u8* state, u32 mode) {
    extern f32 lbl_8047A58C;
    extern f32 lbl_8047BEF4;
    extern s32 menuOpenCustom(s32, ...);
    s32 i;
    u8* base;

    base = lbl_803A9768;
    i = 0;
    *(u32*)(base + 0x278) = i;
    *(u32*)(base + 4) = mode;
    while (i < 2) {
        pokemonInit(base + 8);
        base += 0x138;
        i++;
    }
    if (state != 0) {
        Tbl78* dstState;
        Tbl78* srcState;

        base = lbl_803A9768;
        *(u32*)base = 3;
        dstState = (Tbl78*)(base + 8);
        srcState = (Tbl78*)state;
        *dstState = *srcState;
    }
    lbl_8047A58C = lbl_8047BF00;
    lbl_8047A588 = lbl_8047BEF4;
    menuOpenCustom(0xa0, 0x1f, 0, 0, 0, 0);
}
#pragma pop
