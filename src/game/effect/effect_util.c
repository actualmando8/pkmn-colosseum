/**
 * @file effect_util.c
 * @brief Decompiled functions.
 *
 * Address range: 0x8013151C - 0x80137114
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"

/* ===================================================================
 * Generated: 59 pattern-matched + 148 stubs
 * Range: 0x8013151C - 0x80137114
 * =================================================================== */

extern u32 lbl_803635D8;

extern u32 lbl_8047ADC8;
extern u32 lbl_8047ADCC;
extern u32 lbl_8047ADD0;
extern u32 lbl_8047ADE4;
extern u32 lbl_8047ADE8;
extern u32 lbl_8047ADEC;
extern u32 lbl_8047ADF0;
extern u32 lbl_8047ADF4;
extern u32 lbl_8047ADF8;
extern u32 lbl_8047ADFC;
extern u32 lbl_8047AE00;
extern u32 lbl_8047AE04;
extern u32 lbl_8047AE08;
extern u32 lbl_8047AE0C;
extern u32 lbl_8047AE20;
extern u32 lbl_8047AE24;
extern u32 lbl_8047AE28;
extern u32 lbl_8047AE2C;
extern u32 lbl_8047AE30;
extern u32 lbl_8047AE34;
extern u32 lbl_8047AE38;
extern u32 lbl_8047AE3C;
extern u32 lbl_8047AE40;
extern u32 lbl_8047AE5C;
extern u32 lbl_8047AE60;
extern u32 lbl_8047AE64;
extern u32 lbl_8047AE70;
extern u32 lbl_8047AE74;
extern u32 lbl_8047AE78;
extern u32 lbl_8047AE88;
extern u32 lbl_8047AE8C;
extern u32 lbl_8047AE98;
extern u32 lbl_8047AE9C;
extern u16 lbl_8047AEA2;
extern u8 lbl_8047AED0;

typedef struct EffectLinkedStatusRow {
    u16 links[8];
} EffectLinkedStatusRow;

typedef struct EffectTraceFxEntry {
    u8 kind;
    u8 pad_01[7];
    u32 effectId;
    u8 pad_0C[0xC];
} EffectTraceFxEntry;

typedef struct EffectTraceEntry {
    u8 kind;
    u8 pad_01;
    u16 value0;
    u16 value1;
    u8 pad_06[6];
} EffectTraceEntry;

typedef union EffectParamFirstWord {
    struct {
        u8 field_00;
        u8 field_01;
        u8 field_02;
        u8 field_03;
    } byte;
    u32 word;
} EffectParamFirstWord;

typedef struct EffectParamBlock {
    EffectParamFirstWord word0;
    u32 field_04;
    u32 field_08;
    u32 field_0C;
    u32 field_10;
    u32 field_14;
    f32 field_18;
    u32 field_1C;
    u8 field_20;
    u8 field_21;
    u8 field_22;
} EffectParamBlock;

/* ===== Index lookup globals ===== */
extern GSEffectGlobals lbl_803635C0;  /* effect globals (BSS) */
extern EffectTraceFxEntry lbl_80363B88[];  /* trace fx table (BSS) */
extern EffectTraceEntry lbl_80363C00[];  /* trace table (BSS) */
extern u32 lbl_80478B98;  /* effect count (SDA) */
extern u32 lbl_80478BA0;  /* trace count (SDA) */

/* External function declarations */
extern u8   fn_80102620(u32 objID);
extern void fn_80102510(u32 arg1);

/* Forward declarations for converted functions */
void _msgctrlSideName__FP15FightOutPokemonUc(u32 arg1, u32 arg2);
s32 fn_80133E6C(void* obj, s32 offset);

/* 0x58 | fn_8013151C | leaf_multi_output */
void* fn_8013151C(u32 arg) {
    GSEffectInstance* entry;
    GSEffectGlobals* globals;
    if (arg == 0) goto _ret0;
    globals = &lbl_803635C0;
    if (arg > globals->maxEffects) goto _ret0;
    entry = &((GSEffectInstance*)globals->instanceTable)[arg - 1];
    if (entry->state == GSEFFECT_STATE_UNINIT) goto _ret0;
    goto _compute;
_ret0:
    entry = 0;
_compute:
    if (entry == 0) goto _null_ret;
    return entry->userData;
_null_ret:
    return 0;
}

/* 0x80131574 | 20 bytes | indexed_lookup */
u8 fn_80131574(u32 idx) {
    return ((u8*)lbl_803635D8)[idx];
}

/* 0x80131630 | 0x30 -- read byte from stream, store extsb to obj+0x43 if flag set */
#pragma push
#pragma optimization_level 2
u32 fn_80131630(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x43) = (u8)(s8)*stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}
#pragma pop

/* 0x80131660 | 0x30 -- read byte from stream, store extsb to obj+0x42 if flag set */
#pragma push
#pragma optimization_level 2
u32 fn_80131660(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x42) = (u8)(s8)*stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}
#pragma pop

/* 0x80131690 | 16 bytes | set_field_return */
u32 fn_80131690(void* obj) {
    *(u8*)((u8*)obj + 0x41) = 1;
    return 0;
}

/* 0x801316A0 | 0x8 | sda_getter */
u16 fn_801316A0(void) { return lbl_8047AEA2; }

/* 0x801316A8 | 0x28 -- calls fn_801FBD58 with lbl_8047AEA0 then fn_801FBD28 */
extern u16  lbl_8047AEA0;
extern void fn_801FBD58(u16 handle);
extern void fn_801FBD28(void);
extern u16  lbl_8047AEA0;
/* renamed symbols referenced by asm incs (symbolmap port) */
extern void gamedataAttestCreate();
extern void gamedataAttestInit();
extern void gamedataGetStatus();
extern void gamedataSetStatus();
extern void statusGetStatus();
extern void statusSetStatus();
/* Forward declarations for self-referencing asm blocks */
extern void* fn_80104704(s32 key);
extern void* fn_80134228(s32 offset);
/* _msgctrlSideName__FP15FightOutPokemonUc already declared at top with typed params */
extern s32 fn_80133E6C(void* obj, s32 offset);
extern s32 fn_80134258(void* obj);
extern void* fn_80135CD0(void* ptr);
extern u8 fn_80135BC8(void* ptr);
extern u32 fn_80135C28(void* ptr);
extern u32 fn_80135C40(void* ptr);
extern u8 fn_80135BE0(void* ptr);
extern u8 fn_80135BB0(void* ptr);
extern u32 fn_80135C78(void* ptr);
extern u32 fn_80135BF8(void* ptr);
extern u32 fn_80135B0C(void* ptr);
extern u8 fn_80135AB8(void* ptr);
extern u8 fn_80135AA0(void* ptr);
extern u8 fn_80135A88(void* ptr);
extern u8 fn_80135A70(void* ptr);
extern u32 fn_80135028();
extern u32 fn_80135F90(u32 index);
extern u32 fn_80136050(u32 index);
extern u32 fn_80135FF8(u32 index);
extern s32 fn_80135FBC(u32 index, u32 subIndex);
extern u32 fn_80136024(u32 index);
extern s32 fn_801026A4(u32, ...);
extern void* fn_80132834(void* table, u32 stride, u32 count, u32 type);
extern void fn_80132A38(u32 id, u32 value);
extern s32 fn_801338A4(s32 offset);
extern u32 fn_80133B50(u32 arg0, u32* outMax);
extern u32 fn_80133BE4(void* obj, s32 offset);
extern s32 fn_80133C3C(s32 key);
extern u32 fn_80133E1C(void* obj, s32 offset);
extern s32 _dbgMenuGetLink__Fl(s32 idx);
extern u32 fn_80134274(void);
extern s32 fn_801342B8(s32 idx);
extern u32 fn_80134304(void);
extern void fn_80135338(void*);
extern void fn_801353C0(void*, u8, u8, u8, u8);
extern void fn_80135708(void*);
extern void fn_80135A30(void* ptr, u8 val);
extern void fn_80135A40(void* ptr, u8 val);
extern void fn_80135A50(void* ptr, u8 val);
extern void fn_80135A60(void* ptr, u8 val);
extern void fn_80135AEC(u32* dst, u32* src);
extern void fn_80135B1C(void* ptr, u8 val);
extern void fn_80135B2C(void* ptr, u8 val);
extern void fn_80135B3C(void* ptr, u8 val);
extern void fn_80135B4C(void* ptr, u32 val);
extern void fn_80135B5C(void* ptr, f32 val);
extern void fn_80135B6C(void* ptr, u32 val);
extern void fn_80135B7C(void* ptr, u32 val);
extern void fn_80135BA0(void* ptr, u32 val);
extern f32 fn_80135C10(void* ptr);
extern void fn_80135C90(void* dst, void* src);
extern void fn_80135CE8(void*);
extern void _koukaOneExec__FUlPvPvPl(u32 index, void* arg1, void* arg2, s32* out);

typedef s32 (*EffectUtilCountFunc)(void);
typedef s32 (*EffectUtilEntryCallback)(s32, s32);

typedef struct EffectUtilEntry {
    u8 flags;
    u8 pad_01;
    s16 link;
    u32 value;
    EffectUtilEntryCallback callback;
} EffectUtilEntry;

typedef EffectUtilEntry* (*EffectUtilEntryFunc)(s32);

typedef struct EffectStatusTableEntry {
    u8 statusKind;
    u8 mode;
    u16 statusSub;
    u16 linkedIndex;
    s16 amount;
    s16 divisor;
} EffectStatusTableEntry;

#if 0
asm void fn_801316A8(void) {
#include "src/game/effect/effect_util_fn_801316A8.inc"
}
#else
void fn_801316A8(void) {
    fn_801FBD58(lbl_8047AEA0);
    fn_801FBD28();
}
#endif

/* 0x801316D0 | 0x8 | sda_getter */
u32 fn_801316D0(void) { return lbl_8047AE8C; }

/* 0x801316D8 | 0x8 | sda_getter */
u32 fn_801316D8(void) { return lbl_8047AE9C; }

/* 0x801316E0 | 0x8 | sda_getter */
u32 fn_801316E0(void) { return lbl_8047AE98; }

/* 0x801316E8 | 0x2C -- read byte from stream, store to obj+0x03 if flag clear */
u32 fn_801316E8(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) == 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x03) = *stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x80131768 | 0x2C -- read byte from stream, store to obj+0x02 if flag set */
extern void* fn_80132834(void* table, u32 stride, u32 count, u32 type);
u32 fn_80131768(void* obj) {
    u8* stream;
    if (*(u8*)((u8*)obj + 0x01) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        *(u8*)((u8*)obj + 0x02) = *stream;
    }
    stream = *(u8**)((u8*)obj + 0x30);
    *(u32*)((u8*)obj + 0x30) = (u32)(stream + 1);
    return 0;
}

/* 0x80131794 | 0x34 -- fn_80132834(lbl_80426FF0, 0x10, lbl_8047AE94, 4) */
extern u8  lbl_80426FF0[];
extern u32 lbl_8047AE94;
extern u32 lbl_8047AE94;
#if 0
asm void fn_80131794(void) {
#include "src/game/effect/effect_util_fn_80131794.inc"
}
#else
#pragma peephole off
void fn_80131794(void) {
    fn_80132834(lbl_80426FF0, 0x10, lbl_8047AE94, 4);
}
#pragma peephole on
#endif

/* 0x801317C8 | 0x34 -- fn_80132834(lbl_80427010, 0x10, lbl_8047AE68, 5) */
extern u8  lbl_80427010[];
extern u32 lbl_8047AE68;
extern u32 lbl_8047AE68;
#if 0
asm void fn_801317C8(void) {
#include "src/game/effect/effect_util_fn_801317C8.inc"
}
#else
#pragma peephole off
void fn_801317C8(void) {
    fn_80132834(lbl_80427010, 0x10, lbl_8047AE68, 5);
}
#pragma peephole on
#endif

/* 0x801317FC | 0x28 -- calls fn_8011E778(lbl_8047AE90) then fn_8011E760 */
extern u16  lbl_8047AE90;
extern void fn_8011E778(u16 handle);
extern void fn_8011E760(void);
extern u16  lbl_8047AE90;
#if 0
asm void fn_801317FC(void) {
#include "src/game/effect/effect_util_fn_801317FC.inc"
}
#else
void fn_801317FC(void) {
    fn_8011E778(lbl_8047AE90);
    fn_8011E760();
}
#endif

/* 0x80131824 | 0x8 | sda_getter */
u32 fn_80131824(void) { return lbl_8047AE88; }

/* 0x8013182C | 0x208 */
extern u32 lbl_8047AE84;
extern u8 lbl_80427030[];
extern u32 lbl_8047AEA8;
extern u8 lbl_80427050[];
#if 0
asm void fn_8013182C(void) {
#include "src/game/effect/effect_util_fn_8013182C.inc"
}
#else
void fn_8013182C(void) {
    u32 seconds = lbl_8047AE84 / 3600;
    u32 minutes = (lbl_8047AE84 - seconds * 3600) / 60;
    u32 outIndex = 0;
    u16* out = (u16*)lbl_80427050;
    u16* glyph;

    if (seconds >= 100) {
        glyph = (u16*)fn_80132834(lbl_80427030, 0x10, seconds / 100, 0);
        lbl_8047AEA8 = (u32)glyph;
        out[outIndex++] = glyph[0];
        seconds -= (seconds / 100) * 100;
    }

    glyph = (u16*)fn_80132834(lbl_80427030, 0x10, seconds / 10, 0);
    lbl_8047AEA8 = (u32)glyph;
    out[outIndex++] = glyph[0];

    glyph = (u16*)fn_80132834(lbl_80427030, 0x10, seconds - (seconds / 10) * 10, 0);
    lbl_8047AEA8 = (u32)glyph;
    out[outIndex++] = glyph[0];

    out[outIndex++] = 0x3A;

    glyph = (u16*)fn_80132834(lbl_80427030, 0x10, minutes / 10, 0);
    lbl_8047AEA8 = (u32)glyph;
    out[outIndex++] = glyph[0];

    glyph = (u16*)fn_80132834(lbl_80427030, 0x10, minutes - (minutes / 10) * 10, 0);
    lbl_8047AEA8 = (u32)glyph;
    out[outIndex++] = glyph[0];
    out[outIndex] = 0;
}
#endif

/* 0x80131A34 | 0x34 -- fn_80132834(lbl_80427070, 0x10, lbl_8047AE80, 4) */
extern u8 lbl_80427070[];
extern u32 lbl_8047AE80;
extern u32 lbl_8047AE80;
#if 0
asm void fn_80131A34(void) {
#include "src/game/effect/effect_util_fn_80131A34.inc"
}
#else
#pragma peephole off
void fn_80131A34(void) {
    fn_80132834(lbl_80427070, 0x10, lbl_8047AE80, 4);
}
#pragma peephole on
#endif

/* 0x80131A68 | 0x34 -- fn_80132834(lbl_80427090, 0x10, lbl_8047AE6C, 2) */
extern u8 lbl_80427090[];
extern u32 lbl_8047AE6C;
extern u32 lbl_8047AE6C;
#if 0
asm void fn_80131A68(void) {
#include "src/game/effect/effect_util_fn_80131A68.inc"
}
#else
#pragma peephole off
void fn_80131A68(void) {
    fn_80132834(lbl_80427090, 0x10, lbl_8047AE6C, 2);
}
#pragma peephole on
#endif

/* 0x80131A9C | 0x34 -- fn_80132834(lbl_804270B0, 0x10, lbl_8047AE68, 2) */
extern u8 lbl_804270B0[];
extern u32 lbl_8047AE68;
#if 0
asm void fn_80131A9C(void) {
#include "src/game/effect/effect_util_fn_80131A9C.inc"
}
#else
#pragma peephole off
void fn_80131A9C(void) {
    fn_80132834(lbl_804270B0, 0x10, lbl_8047AE68, 2);
}
#pragma peephole on
#endif

/* 0x80131AD0 | 0x34 -- fn_80132834(lbl_804270D0, 0x10, lbl_8047AE68, 3) */
extern u8 lbl_804270D0[];
extern u32 lbl_8047AE68;
#if 0
asm void fn_80131AD0(void) {
#include "src/game/effect/effect_util_fn_80131AD0.inc"
}
#else
#pragma peephole off
void fn_80131AD0(void) {
    fn_80132834(lbl_804270D0, 0x10, lbl_8047AE68, 3);
}
#pragma peephole on
#endif

/* 0x80131B04 | 0x34 -- fn_80132834(lbl_804270F0, 0x10, lbl_8047AE68, 3) */
extern u8 lbl_804270F0[];
extern u32 lbl_8047AE68;
#if 0
asm void fn_80131B04(void) {
#include "src/game/effect/effect_util_fn_80131B04.inc"
}
#else
#pragma peephole off
void fn_80131B04(void) {
    fn_80132834(lbl_804270F0, 0x10, lbl_8047AE68, 3);
}
#pragma peephole on
#endif

/* 0x80131B38 | 0x34 -- fn_80132834(lbl_80427110, 0x10, lbl_8047AE6C, 1) */
extern u8 lbl_80427110[];
extern u32 lbl_8047AE6C;
#if 0
asm void fn_80131B38(void) {
#include "src/game/effect/effect_util_fn_80131B38.inc"
}
#else
#pragma peephole off
void fn_80131B38(void) {
    fn_80132834(lbl_80427110, 0x10, lbl_8047AE6C, 1);
}
#pragma peephole on
#endif

/* 0x80131B6C | 0x34 -- fn_80132834(lbl_80427130, 0x10, lbl_8047AE68, 1) */
extern u8 lbl_80427130[];
extern u32 lbl_8047AE68;
#if 0
asm void fn_80131B6C(void) {
#include "src/game/effect/effect_util_fn_80131B6C.inc"
}
#else
#pragma peephole off
void fn_80131B6C(void) {
    fn_80132834(lbl_80427130, 0x10, lbl_8047AE68, 1);
}
#pragma peephole on
#endif

/* 0x80131BA0 | 0x8 | return_const */
u32 fn_80131BA0(void) { return 0; }

/* 0x80131BA8 | 0x8 | return_const */
u32 fn_80131BA8(void) { return 0; }

/* 0x80131BB0 | 0x8 | sda_getter */
u32 fn_80131BB0(void) { return lbl_8047AE40; }

/* 0x80131BB8 | 0x8 | sda_getter */
u32 fn_80131BB8(void) { return lbl_8047AE3C; }

/* 0x80131BC0 | 0x8 | sda_getter */
u32 fn_80131BC0(void) { return lbl_8047AE38; }

/* 0x80131BC8 | 0x8 | sda_getter */
u32 fn_80131BC8(void) { return lbl_8047AE34; }

/* 0x80131BD0 | 0x8 | sda_getter */
u32 fn_80131BD0(void) { return lbl_8047AE30; }

/* 0x80131BD8 | 0x8 | sda_getter */
u32 fn_80131BD8(void) { return lbl_8047AE2C; }

/* 0x80131BE0 | 0x8 | sda_getter */
u32 fn_80131BE0(void) { return lbl_8047AE28; }

/* 0x80131BE8 | 0x8 | sda_getter */
u32 fn_80131BE8(void) { return lbl_8047AE24; }

/* 0x80131BF0 | 0x8 | sda_getter */
u32 fn_80131BF0(void) { return lbl_8047AE20; }

/* 0x80131BF8 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE4C, 2) */
extern u32 lbl_8047AE4C;
extern u32 lbl_8047AE4C;
#if 0
asm void fn_80131BF8(void) {
#include "src/game/effect/effect_util_fn_80131BF8.inc"
}
#else
#pragma peephole off
void fn_80131BF8(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE4C, 2);
}
#pragma peephole on
#endif

/* 0x80131C20 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE48, 1) */
extern u32 lbl_8047AE48;
extern u32 lbl_8047AE48;
#if 0
asm void fn_80131C20(void) {
#include "src/game/effect/effect_util_fn_80131C20.inc"
}
#else
#pragma peephole off
void fn_80131C20(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE48, 1);
}
#pragma peephole on
#endif

/* 0x80131C48 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE44, 0) */
extern u32 lbl_8047AE44;
extern u32 lbl_8047AE44;
#if 0
asm void fn_80131C48(void) {
#include "src/game/effect/effect_util_fn_80131C48.inc"
}
#else
#pragma peephole off
void fn_80131C48(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE44, 0);
}
#pragma peephole on
#endif

/* 0x80131C70 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE1C, 2) */
extern u32 lbl_8047AE1C;
extern u32 lbl_8047AE1C;
#if 0
asm void fn_80131C70(void) {
#include "src/game/effect/effect_util_fn_80131C70.inc"
}
#else
#pragma peephole off
void fn_80131C70(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE1C, 2);
}
#pragma peephole on
#endif

/* 0x80131C98 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE18, 1) */
extern u32 lbl_8047AE18;
extern u32 lbl_8047AE18;
#if 0
asm void fn_80131C98(void) {
#include "src/game/effect/effect_util_fn_80131C98.inc"
}
#else
#pragma peephole off
void fn_80131C98(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE18, 1);
}
#pragma peephole on
#endif

/* 0x80131CC0 | 0x28 -- _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE14, 0) */
extern u32 lbl_8047AE14;
extern u32 lbl_8047AE14;
#if 0
asm void fn_80131CC0(void) {
#include "src/game/effect/effect_util_fn_80131CC0.inc"
}
#else
#pragma peephole off
void fn_80131CC0(void) {
    _msgctrlSideName__FP15FightOutPokemonUc(lbl_8047AE14, 0);
}
#pragma peephole on
#endif

/* 0x80131CE8 | 0x21C */
#pragma push
#pragma optimization_level 1
void _msgctrlSideName__FP15FightOutPokemonUc(u32 arg1, u32 arg2) {
    extern void fn_800FA280();
    extern void fn_80132A38();
    extern void fn_801F0058();
    extern void fn_801F025C();
    extern void fn_801F18DC();
    extern void fn_801F4354();
    extern void fn_801F54A4();
    extern void fn_801F7258();
    extern void fn_801F8100();
    extern void fn_801FA524();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r24 = r3;
    r31 = r4;
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0x14;
    r6 = 0x0;
    fn_801F54A4();
    r30 = r3 & 0xFFFF;
    r4 = r24;
    r3 = 0x2;
    fn_801F025C();
    r29 = r3;
    r4 = r24;
    r3 = 0x0;
    fn_801F4354();
    r26 = 0x0;
    r28 = r3;
    r25 = 0x0;
    while (1) {
        r0 = r25 & 0xFFFF;
        if (r0 >= (u32)0x2) break;
        r3 = r29;
        r4 = r25;
        fn_801F7258();
        r27 = r3;
        do {
            if (r27 == (u32)0x0) break;
            fn_801FA524();
            r0 = r3 & 0xFF;
            if (r0 == (u32)0x0) break;
            r0 = r26 & 0xFFFF;
            if (r0 == (u32)0x0) {
                r3 = r27;
                fn_801F8100();
                r4 = r3;
                r3 = 0x4d;
                fn_80132A38();

            } else {
                if (r0 == (u32)0x1) {
                    r3 = r27;
                    fn_801F8100();
                    r4 = r3;
                    r3 = 0x57;
                    fn_80132A38();
                }
            }
            r26 = r26 + 0x1;
        } while (0);
        r25 = r25 + 0x1;

    }
    r3 = 0x0;
    fn_801F18DC();
    r0 = r3 & 0xFF;
    if ((r0 == (u32)0x1) && (r28 != (u32)0x0)) {

        r0 = r26 & 0xFFFF;
        if (r0 <= (u32)0x1) {
            r3 = r28;
            fn_801F8100();
            r4 = r3;
            r3 = 0x4d;
            fn_80132A38();
            r0 = r31 & 0xFF;
            if (r0 == (u32)0x0) {
                r3 = 0x7722;
                fn_800FA280();
                return;
            }
            if (r0 == (u32)0x1) {
                r3 = 0x7725;
                fn_800FA280();
                return;
            }
            r3 = 0x7727;
            fn_800FA280();
            return;
        }
        r0 = r31 & 0xFF;
        if (r0 == (u32)0x0) {
            r3 = 0x7724;
            fn_800FA280();
            return;
        }
        if (r0 == (u32)0x1) {
            r3 = 0x7726;
            fn_800FA280();
            return;
        }
        r3 = 0x7728;
        fn_800FA280();
        return;
    }
    r3 = r24;
    r4 = r30;
    fn_801F0058();
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = r31 & 0xFF;
        if (r0 == (u32)0x0) {
            r3 = 0x768a;
            fn_800FA280();
            return;
        }
        if (r0 == (u32)0x1) {
            r3 = 0x768c;
            fn_800FA280();
            return;
        }
        r3 = 0x7688;
        fn_800FA280();
        return;
    }
    r0 = r31 & 0xFF;
    if (r0 == (u32)0x0) {
        r3 = 0x7689;
        fn_800FA280();
        return;
    }
    if (r0 == (u32)0x1) {
        r3 = 0x768b;
        fn_800FA280();
        return;
    }
    r3 = 0x7687;
    fn_800FA280();

    return;
}
#pragma pop

/* 0x80131F04 | 0x98 */
extern void fn_801F4354(void);
extern void fn_801F18DC(void);
extern void fn_801F8100(void);
extern void fn_802037DC(void);
extern void fn_800FA280(void);
extern u32 lbl_8047AE10;
#if 0
asm void fn_80131F04(void) {
#include "src/game/effect/effect_util_fn_80131F04.inc"
}
#else
#pragma push
#pragma peephole off
#pragma push
#pragma optimization_level 1
void fn_80131F04(void) {
    extern u32 lbl_8047AE10;
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    extern void fn_80132A38(u32, u32);
    extern void fn_800FA280(u32);
    u32 val = lbl_8047AE10;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;
    if (flag == 1 && result != 0) {
        fn_80132A38(0x4D, fn_801F8100(result));
        fn_80132A38(0x57, fn_802037DC(val));
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}
#pragma pop
#pragma pop
#endif

/* 0x80131F9C | 0x8 | sda_getter */
u32 fn_80131F9C(void) { return lbl_8047AE0C; }

/* 0x80131FA4 | 0x8 | sda_getter */
u32 fn_80131FA4(void) { return lbl_8047AE08; }

/* 0x80131FAC | 0x8 | sda_getter */
u32 fn_80131FAC(void) { return lbl_8047AE04; }

/* 0x80131FB4 | 0x8 | sda_getter */
u32 fn_80131FB4(void) { return lbl_8047AE00; }

/* 0x80131FBC | 0x8 | sda_getter */
u32 fn_80131FBC(void) { return lbl_8047ADFC; }

/* 0x80131FC4 | 0x8 | sda_getter */
u32 fn_80131FC4(void) { return lbl_8047ADF8; }

/* 0x80131FCC | 0x8 | sda_getter */
u32 fn_80131FCC(void) { return lbl_8047ADF4; }

/* 0x80131FD4 | 0x8 | sda_getter */
u32 fn_80131FD4(void) { return lbl_8047ADF0; }

/* 0x80131FDC | 0x8 | sda_getter */
u32 fn_80131FDC(void) { return lbl_8047ADEC; }

/* 0x80131FE4 | 0x8 | sda_getter */
u32 fn_80131FE4(void) { return lbl_8047ADE8; }

/* 0x80131FEC | 0x8 | sda_getter */
u32 fn_80131FEC(void) { return lbl_8047ADE4; }

/* 0x80131FF4 | 0x98 */
extern u32 lbl_8047ADE0;
#if 0
asm void fn_80131FF4(void) {
#include "src/game/effect/effect_util_fn_80131FF4.inc"
}
#else
#pragma push
#pragma peephole off
#pragma push
#pragma optimization_level 1
void fn_80131FF4(void) {
    extern u32 lbl_8047ADE0;
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    extern void fn_80132A38(u32, u32);
    extern void fn_800FA280(u32);
    u32 val = lbl_8047ADE0;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;
    if (flag == 1 && result != 0) {
        fn_80132A38(0x4D, fn_801F8100(result));
        fn_80132A38(0x57, fn_802037DC(val));
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}
#pragma pop
#pragma pop
#endif

/* 0x8013208C | 0x98 */
extern u32 lbl_8047ADDC;
#if 0
asm void fn_8013208C(void) {
#include "src/game/effect/effect_util_fn_8013208C.inc"
}
#else
#pragma push
#pragma peephole off
#pragma push
#pragma optimization_level 1
void fn_8013208C(void) {
    extern u32 lbl_8047ADDC;
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    extern void fn_80132A38(u32, u32);
    extern void fn_800FA280(u32);
    u32 val = lbl_8047ADDC;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;
    if (flag == 1 && result != 0) {
        fn_80132A38(0x4D, fn_801F8100(result));
        fn_80132A38(0x57, fn_802037DC(val));
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}
#pragma pop
#pragma pop
#endif

/* 0x80132124 | 0x98 */
extern u32 lbl_8047ADD8;
#if 0
asm void fn_80132124(void) {
#include "src/game/effect/effect_util_fn_80132124.inc"
}
#else
#pragma push
#pragma peephole off
#pragma push
#pragma optimization_level 1
void fn_80132124(void) {
    extern u32 lbl_8047ADD8;
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    extern void fn_80132A38(u32, u32);
    extern void fn_800FA280(u32);
    u32 val = lbl_8047ADD8;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;
    if (flag == 1 && result != 0) {
        fn_80132A38(0x4D, fn_801F8100(result));
        fn_80132A38(0x57, fn_802037DC(val));
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}
#pragma pop
#pragma pop
#endif

/* 0x801321BC | 0x98 */
extern u32 lbl_8047ADD4;
#if 0
asm void fn_801321BC(void) {
#include "src/game/effect/effect_util_fn_801321BC.inc"
}
#else
#pragma push
#pragma peephole off
#pragma push
#pragma optimization_level 1
void fn_801321BC(void) {
    extern u32 lbl_8047ADD4;
    extern u32 fn_801F4354(u32, u32);
    extern u32 fn_801F18DC(u32);
    extern u32 fn_801F8100(u32);
    extern u32 fn_802037DC(u32);
    extern void fn_80132A38(u32, u32);
    extern void fn_800FA280(u32);
    u32 val = lbl_8047ADD4;
    u32 result = fn_801F4354(0, val);
    u32 flag = fn_801F18DC(0) & 0xFF;
    if (flag == 1 && result != 0) {
        fn_80132A38(0x4D, fn_801F8100(result));
        fn_80132A38(0x57, fn_802037DC(val));
        fn_800FA280(0x7721);
    } else {
        fn_802037DC(val);
    }
}
#pragma pop
#pragma pop
#endif

/* 0x80132254 | 0x8 | sda_getter */
u32 fn_80132254(void) { return lbl_8047ADD0; }

/* 0x8013225C | 0x8 | sda_getter */
u32 fn_8013225C(void) { return lbl_8047ADCC; }

/* 0x80132264 | 0x8 | sda_getter */
u32 fn_80132264(void) { return lbl_8047ADC8; }

/* 0x8013226C | 0x28 -- calls fn_8011CA34(lbl_8047AE7C) then fn_8011CA1C */
extern u16  lbl_8047AE7C;
extern void fn_8011CA34(u16 handle);
extern void fn_8011CA1C(void);
extern u16  lbl_8047AE7C;
#if 0
asm void fn_8013226C(void) {
#include "src/game/effect/effect_util_fn_8013226C.inc"
}
#else
#pragma peephole off
void fn_8013226C(void) {
    fn_8011CA34(lbl_8047AE7C);
    fn_8011CA1C();
}
#pragma peephole on
#endif

/* 0x80132294 | 0x8 | sda_getter */
u32 fn_80132294(void) { return lbl_8047AE78; }

/* 0x8013229C | 0x8 | sda_getter */
u32 fn_8013229C(void) { return lbl_8047AE74; }

/* 0x801322A4 | 0x8 | sda_getter */
u32 fn_801322A4(void) { return lbl_8047AE70; }

/* 0x801322AC | 0x34 -- fn_80132834(lbl_80427150, 0x10, lbl_8047AE6C, 0) */
extern u8 lbl_80427150[];
extern u32 lbl_8047AE6C;
#if 0
asm void fn_801322AC(void) {
#include "src/game/effect/effect_util_fn_801322AC.inc"
}
#else
#pragma peephole off
void fn_801322AC(void) {
    fn_80132834(lbl_80427150, 0x10, lbl_8047AE6C, 0);
}
#pragma peephole on
#endif

/* 0x801322E0 | 0x34 -- fn_80132834(lbl_80427170, 0x10, lbl_8047AE68, 0) */
extern u8 lbl_80427170[];
extern u32 lbl_8047AE68;
#if 0
asm void fn_801322E0(void) {
#include "src/game/effect/effect_util_fn_801322E0.inc"
}
#else
#pragma peephole off
void fn_801322E0(void) {
    fn_80132834(lbl_80427170, 0x10, lbl_8047AE68, 0);
}
#pragma peephole on
#endif

/* 0x80132314 | 0x8 | sda_getter */
u32 fn_80132314(void) { return lbl_8047AE64; }

/* 0x8013231C | 0x8 | sda_getter */
u32 fn_8013231C(void) { return lbl_8047AE60; }

/* 0x80132324 | 0x8 | sda_getter */
u32 fn_80132324(void) { return lbl_8047AE5C; }

/* 0x8013232C | 0x34 -- fn_80132834(lbl_80427190, 0x10, lbl_8047AE58, 0) */
extern u8  lbl_80427190[];
extern u32 lbl_8047AE58;
extern u32 lbl_8047AE58;
#if 0
asm void fn_8013232C(void) {
#include "src/game/effect/effect_util_fn_8013232C.inc"
}
#else
#pragma peephole off
void fn_8013232C(void) {
    fn_80132834(lbl_80427190, 0x10, lbl_8047AE58, 0);
}
#pragma peephole on
#endif

/* 0x80132360 | 0x34 -- fn_80132834(lbl_804271B0, 0x10, lbl_8047AE54, 0) */
extern u8  lbl_804271B0[];
extern u32 lbl_8047AE54;
extern u32 lbl_8047AE54;
#if 0
asm void fn_80132360(void) {
#include "src/game/effect/effect_util_fn_80132360.inc"
}
#else
#pragma peephole off
void fn_80132360(void) {
    fn_80132834(lbl_804271B0, 0x10, lbl_8047AE54, 0);
}
#pragma peephole on
#endif

/* 0x80132394 | 0x34 -- calls fn_801440A0(lbl_8047AE52) then fn_80144088, default to 0x2B6E */
extern u16  lbl_8047AE52;
extern void fn_801440A0(u16 handle);
extern u32  fn_80144088(void);
u32 fn_80132394(void) {
    u32 result;
    fn_801440A0(lbl_8047AE52);
    result = fn_80144088();
    if (result == 0) { result = 0x2B6E; }
    return result;
}

/* 0x801323C8 | 0x34 -- calls fn_801440A0(lbl_8047AE50) then fn_80144088, default to 0x2B6E */
extern u16 lbl_8047AE50;
u32 fn_801323C8(void) {
    u32 result;
    fn_801440A0(lbl_8047AE50);
    result = fn_80144088();
    if (result == 0) { result = 0x2B6E; }
    return result;
}

/* 0x801323FC | 0x2C -- fn_80129280(0, 2) then fn_8012A8D4 */
extern u32  fn_80129280(u32 side, u32 slotType);
extern u32  fn_8012A8D4(void);
#pragma peephole off
u32 fn_801323FC(void) {
    fn_80129280(0, 2);
    return fn_8012A8D4();
}
#pragma peephole on

/* 0x80132428 | 0x2C -- fn_80129280(0, 2) then fn_8012AC54 */
extern u32 fn_8012AC54(void);
#pragma peephole off
u32 fn_80132428(void) {
    fn_80129280(0, 2);
    return fn_8012AC54();
}
#pragma peephole on

/* 0x801324CC | 0xA4 -- read color index from stream, look up RGBA, apply */
void fn_801324CC(void* obj) {
    extern u32 lbl_80478E88;
    extern u32 lbl_80478E8C;
    extern void fn_800FA160(void*);
    u8* stream;
    u8 idx;
    u8* colorPtr;
    u32 maxIdx;

    if (*(u8*)((u8*)obj + 0x1) != 0) {
        stream = *(u8**)((u8*)obj + 0x30);
        idx = *stream;
        maxIdx = lbl_80478E88;
        if (idx >= maxIdx) {
            idx = 0;
        }
        colorPtr = (u8*)(lbl_80478E8C + (u32)idx * 4);
        *(u32*)((u8*)obj + 0x24) = ((u32)colorPtr[0] << 24) |
                                    ((u32)colorPtr[1] << 16) |
                                    ((u32)colorPtr[2] << 8) |
                                    (u32)colorPtr[3];
        fn_800FA160(obj);
    }
    /* Advance stream pointer */
    *(u32*)((u8*)obj + 0x30) = *(u32*)((u8*)obj + 0x30) + 1;
}

/* 0x68 | fn_801325C4 | two_call_arg_check */
void fn_801325C4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (arg1 != 0) { return; }
    fn_800FA1BC();
    /* store u16 to offset 0x20 */
    /* store u16 to offset 0x20 */
    /* store u32 to offset 0x30 */
    fn_800FA1BC();
}

/* 0x8013262C | 16 bytes | set_field_return */
u32 fn_8013262C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 0;
    return 0;
}

/* 0x8013263C | 16 bytes | set_field_return */
u32 fn_8013263C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 2;
    return 0;
}

/* 0x80132690 | 0xCC -- effect tick with flag-based logic */
#pragma push
#pragma peephole off
u32 fn_80132690(void* obj) {
    extern void fn_80166A28(u32);
    u8* p = (u8*)obj;

    /* Check flag bit 1 at offset 0x44 */
    if (p[0x44] & 0x02) {
        p[0x45] = 1;
    }
    /* If scene object 0xA is active, clear the flag */
    if ((u32)(fn_80102620(0x0A) & 0xFF) != 0) {
        p[0x45] = 0;
    }

    if (p[0x01] == 0) {
        /* Not active */
        if (p[0x45] != 0) {
            p[0x45] = 0;
            *(u32*)(p + 0x2C) = *(u32*)(p + 0x30);
            if ((p[0x44] & 0x02) == 0) {
                fn_80166A28(0x24);
            }
        } else {
            /* No trigger: rewind stream by 3, mark done */
            *(u32*)(p + 0x30) = *(u32*)(p + 0x30) - 3;
            *(u8*)(p + 0x46) = 1;
        }
    } else {
        /* Active: copy position floats */
        *(f32*)(p + 0x0C) = *(f32*)(p + 0x04);
        *(f32*)(p + 0x10) = *(f32*)(p + 0x08);
    }
    return 1;
}
#pragma pop

/* 0x8013275C | 0x84 */
#if 0
asm void fn_8013275C(void) {
#include "src/game/effect/effect_util_fn_8013275C.inc"
}
#else
#pragma peephole off
u32 fn_8013275C(void* obj) {
    u8* p = (u8*)obj;
    if (p[0x44] & 0x02) {
        p[0x45] = 1;
    }
    if ((u32)(fn_80102620(0x0A) & 0xFF) != 0) {
        p[0x45] = 0;
    }
    if (p[0x45] != 0) {
        p[0x45] = 0;
    } else {
        *(u32*)(p + 0x30) = *(u32*)(p + 0x30) - 3;
    }
    return 1;
}
#pragma peephole on
#endif

/* 0x80132834 | 0x204 */
#pragma push
#pragma optimization_level 1
void* fn_80132834(void* table, u32 stride, u32 count, u32 type) {
    extern u8 lbl_803635F0[];
    extern u8 lbl_80363610[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = (u32)table;
    u32 r4 = stride;
    u32 r5 = count;
    u32 r6 = type;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    *(u32*)(sp + 0x8) = r5;
    r9 = r4 - 1;
    r0 = r9 << 1;
    r4 = 0x0;
    *(u16*)(r3 + r0) = r4;
    r0 = 0x0;
    r4 = 0xa;
    r5 = 0x0;
    r7 = 0x1;
    if ((s32)r6 != (s32)0x3) {
        if ((s32)r6 < (s32)0x3) {
            if ((s32)r6 == (s32)0x1) goto L_801328BC;
            if ((s32)r6 < (s32)0x1) {
                if ((s32)r6 < (s32)0x0) {
                    goto L_801328BC;
                }
                if ((s32)r6 != (s32)0x5) {
                    goto L_801328BC;
                }
                }
            r8 = *(u32*)(sp + 0x8);
            if ((s32)r8 < (s32)0x0) {
                r8 = -r8;
                r0 = 0x1;
            }
            goto L_801328BC;
            }
        r7 = 0xa;

    } else {
        r4 = 0x10;
        r7 = 0x8;
    }
L_801328BC: ;
    if ((s32)r6 == (s32)0x5) {
        r8 = (u32)lbl_80363610;
        r8 = (u32)lbl_80363610;
    } else {
        r8 = (u32)lbl_803635F0;
        r8 = (u32)lbl_803635F0;
    }
    r10 = r9 << 1;
    while (1) {
        r11 = *(u32*)(sp + 0x8);
        if (r11 == (u32)0x0) break;
        if ((s32)r6 == (s32)0x4) {
            if ((s32)r5 != (s32)0x0) {
                r11 = (0x5555 << 16);
                r11 = r11 + 0x5556;
                r12 = (s32)((s64)r11 * (s64)r5 >> 32);
                r11 = (u32)r12 >> 31;
                r11 = r12 + r11;
                r11 = r11 * 0x3;
                r11 = r5 - r11;
                if ((s32)r11 == (s32)0x0) {
                    r11 = 0x2c;
                    *(u16*)(r3 + r10) = r11;
        }
        }
        }
        r31 = *(u32*)(sp + 0x8);
        r5 = r5 + 0x1;
        r12 = (u32)r31 / (u32)r4;
        r11 = r12 * r4;
        r11 = r31 - r11;
        r11 = r11 << 1;
        r11 = *(u16*)(r8 + r11);
        *(u16*)(r3 + r10) = r11;

    }
    r6 = r9 << 1;
    r5 = r7 - r5;
    do {
        if ((s32)r5 >= (s32)r7) break;
        r4 = (u32)r5 >> 3;
        ctr_fn = (void(*)(void))r4;
        if (r4 != (u32)0x0) {
            do {
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
                r4 = *(u16*)((u8*)r8 + 0x0);
                *(u16*)(r3 + r6) = r4;
            } while (--ctr != 0);
            r5 = r5 & 0x7;
            if (r4 == (u32)0x0) break;
        }
        ctr_fn = (void(*)(void))r5;
        do {
            r4 = *(u16*)((u8*)r8 + 0x0);
            *(u16*)(r3 + r6) = r4;
        } while (--ctr != 0);
    } while (0);
    r0 = r0 & 0xFF;
    if (r0 != (u32)0x0) {
        r4 = 0x2d;
        r0 = r9 << 1;
        *(u16*)(r3 + r0) = r4;
    }
    r0 = r9 << 1;
    r3 = r3 + r0;
    r31 = *(u32*)(sp + 0x1C);
    return (void*)r3;
}
#pragma pop

/* 0x80132A38 | 0x210 */
extern void jumptable_80363630();
extern u16 lbl_8047AE50;
extern u16  lbl_8047AE52;
extern u32 lbl_8047AE54;
extern u32 lbl_8047AE58;
extern u32 lbl_8047AE5C;
extern u32 lbl_8047AE60;
extern u32 lbl_8047AE64;
extern u32 lbl_8047AE68;
extern u32 lbl_8047AE6C;
extern u32 lbl_8047AE70;
extern u32 lbl_8047AE74;
extern u32 lbl_8047AE78;
extern u16  lbl_8047AE7C;
extern u32 lbl_8047AE88;
extern u32 lbl_8047AE8C;
extern u16  lbl_8047AE90;
extern u32 lbl_8047AE80;
extern u32 lbl_8047AE84;
extern u32 lbl_8047AE94;
extern u32 lbl_8047AE98;
extern u32 lbl_8047AE9C;
extern u16  lbl_8047AEA0;
extern u16 lbl_8047AEA2;
extern u16 lbl_8047AEA4;
extern u32 lbl_8047ADC8;
extern u32 lbl_8047ADCC;
extern u32 lbl_8047ADD0;
extern u32 lbl_8047ADD4;
extern u32 lbl_8047ADD8;
extern u32 lbl_8047ADDC;
extern u32 lbl_8047ADE0;
extern u32 lbl_8047ADE4;
extern u32 lbl_8047ADE8;
extern u32 lbl_8047ADEC;
extern u32 lbl_8047ADF0;
extern u32 lbl_8047ADF4;
extern u32 lbl_8047ADF8;
extern u32 lbl_8047ADFC;
extern u32 lbl_8047AE00;
extern u32 lbl_8047AE04;
extern u32 lbl_8047AE08;
extern u32 lbl_8047AE0C;
extern u32 lbl_8047AE10;
extern u32 lbl_8047AE14;
extern u32 lbl_8047AE18;
extern u32 lbl_8047AE1C;
extern u32 lbl_8047AE20;
extern u32 lbl_8047AE24;
extern u32 lbl_8047AE28;
extern u32 lbl_8047AE2C;
extern u32 lbl_8047AE30;
extern u32 lbl_8047AE34;
extern u32 lbl_8047AE38;
extern u32 lbl_8047AE3C;
extern u32 lbl_8047AE40;
extern u32 lbl_8047AE44;
extern u32 lbl_8047AE48;
extern u32 lbl_8047AE4C;
#if 0
asm void fn_80132A38(void) {
#include "src/game/effect/effect_util_fn_80132A38.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80132A38(u32 id, u32 value) {
    switch (id) {
    case 0x0D: lbl_8047AE50 = (u16)value; return;
    case 0x0E: lbl_8047AE52 = (u16)value; return;
    case 0x0F: lbl_8047AE54 = value; return;
    case 0x10: lbl_8047AE58 = value; return;
    case 0x11: lbl_8047AE5C = value; return;
    case 0x12: lbl_8047AE60 = value; return;
    case 0x13: lbl_8047AE64 = value; return;
    case 0x14: lbl_8047AE68 = value; return;
    case 0x15: lbl_8047AE6C = value; return;
    case 0x16: lbl_8047AE70 = value; return;
    case 0x17: lbl_8047AE74 = value; return;
    case 0x18: lbl_8047AE78 = value; return;
    case 0x19: lbl_8047AE7C = (u16)value; return;
    case 0x1A: lbl_8047AE88 = value; return;
    case 0x1B: lbl_8047AE8C = value; return;
    case 0x1C: lbl_8047AE90 = (u16)value; return;
    case 0x1D: lbl_8047AE80 = value; return;
    case 0x1E: lbl_8047AE84 = value; return;
    case 0x1F: lbl_8047AE94 = value; return;
    case 0x20: lbl_8047AE98 = value; return;
    case 0x21: lbl_8047AE9C = value; return;
    case 0x22: lbl_8047AEA0 = (u16)value; return;
    case 0x23: lbl_8047AEA2 = (u16)value; return;
    case 0x24: lbl_8047AEA4 = (u16)value; return;
    case 0x3C: lbl_8047ADC8 = value; return;
    case 0x3D: lbl_8047ADCC = value; return;
    case 0x3E: lbl_8047ADD0 = value; return;
    case 0x3F: lbl_8047ADD4 = value; return;
    case 0x40: lbl_8047ADD8 = value; return;
    case 0x41: lbl_8047ADDC = value; return;
    case 0x42: lbl_8047ADE0 = value; return;
    case 0x43: lbl_8047ADE4 = value; return;
    case 0x44: lbl_8047ADE8 = value; return;
    case 0x45: lbl_8047ADEC = value; return;
    case 0x46: lbl_8047ADF0 = value; return;
    case 0x47: lbl_8047ADF4 = value; return;
    case 0x48: lbl_8047ADF8 = value; return;
    case 0x49: lbl_8047ADFC = value; return;
    case 0x4A: lbl_8047AE00 = value; return;
    case 0x4B: lbl_8047AE04 = value; return;
    case 0x4C: lbl_8047AE08 = value; return;
    case 0x4D: lbl_8047AE0C = value; return;
    case 0x4E: lbl_8047AE10 = value; return;
    case 0x4F: lbl_8047AE14 = value; return;
    case 0x50: lbl_8047AE18 = value; return;
    case 0x51: lbl_8047AE1C = value; return;
    case 0x52: lbl_8047AE20 = value; return;
    case 0x53: lbl_8047AE24 = value; return;
    case 0x54: lbl_8047AE28 = value; return;
    case 0x55: lbl_8047AE2C = value; return;
    case 0x56: lbl_8047AE30 = value; return;
    case 0x57: lbl_8047AE34 = value; return;
    case 0x58: lbl_8047AE38 = value; return;
    case 0x59: lbl_8047AE3C = value; return;
    case 0x5A: lbl_8047AE40 = value; return;
    case 0x5B: lbl_8047AE44 = value; return;
    case 0x5C: lbl_8047AE48 = value; return;
    case 0x5D: lbl_8047AE4C = value; return;
    }
}
#pragma pop
#endif

/* 0x80132C48 | 36 bytes | multi_sda_store */
extern u32 lbl_8047AE70;
extern u32 lbl_8047AE74;
extern u32 lbl_8047AE78;
extern u32 lbl_8047AE60;
extern u32 lbl_8047AE64;
extern u32 lbl_8047AE88;
extern u32 lbl_8047AE8C;
#if 0
asm void fn_80132C48(void) {
#include "src/game/effect/effect_util_fn_80132C48.inc"
}
#else
void fn_80132C48(void) {
    lbl_8047AE70 = 0;
    lbl_8047AE74 = 0;
    lbl_8047AE78 = 0;
    lbl_8047AE60 = 0;
    lbl_8047AE64 = 0;
    lbl_8047AE88 = 0;
    lbl_8047AE8C = 0;
}
#endif

/* 0x80132C6C | 0x310 */
extern u32 fn_800E3534(u32 size);
extern u32 fn_800E27B0(u32 handle);
extern u32 lbl_8047AEB4;
extern u32 lbl_8047AEC0;
extern u32 lbl_8047AECC;
extern u32 lbl_8047AEC8;
extern u16 lbl_8047AEB8;
extern u32 lbl_8047AEB0;
extern u16 lbl_8047AEC4;
extern u32 lbl_8047AEBC;
#if 0
asm void fn_80132C6C(void) {
#include "src/game/effect/effect_util_fn_80132C6C.inc"
}
#else
void fn_80132C6C(u32 count, u32 maxPerSlot, u32 arg2, u32 arg3) {
    extern u32 lbl_8047AEB0;
    extern u32 lbl_8047AEB4;
    extern u16 lbl_8047AEB8;
    extern u32 lbl_8047AEBC;
    extern u32 lbl_8047AEC0;
    extern u16 lbl_8047AEC4;
    extern u32 lbl_8047AEC8;
    extern u32 lbl_8047AECC;
    u32 total;
    u32 i;
    u32 ofs;
    u8* e;

    if (count == 0 || maxPerSlot == 0) return;

    lbl_8047AEB4 = count;
    lbl_8047AEC0 = maxPerSlot;
    lbl_8047AECC = arg3;
    lbl_8047AEC8 = arg2;

    lbl_8047AEB8 = (u16)fn_800E3534(count * 0x18);
    if (lbl_8047AEB8 == 0) return;
    lbl_8047AEB0 = fn_800E27B0(lbl_8047AEB8);

    i = 0; ofs = 0;
    while (i < lbl_8047AEB4) {
        e = (u8*)lbl_8047AEB0 + ofs;
        i++; ofs += 0x18;
        *(u32*)(e + 0x00) = 0;
        *(u32*)(e + 0x04) = 0;
        *(u32*)(e + 0x08) = 0;
        *(u32*)(e + 0x0C) = 0;
        *(u32*)(e + 0x10) = 0;
        *(u8*)(e + 0x14) = 0;
        *(u8*)(e + 0x15) = 0;
    }

    total = lbl_8047AEB4 * lbl_8047AEC0;
    lbl_8047AEC4 = (u16)fn_800E3534(total << 5);
    if (lbl_8047AEC4 == 0) return;
    lbl_8047AEBC = fn_800E27B0(lbl_8047AEC4);
    if (total == 0) return;

    i = 0; ofs = 0;
    while (i < total) {
        e = (u8*)lbl_8047AEBC + ofs;
        i++; ofs += 0x20;
        *(u32*)(e + 0x00) = -1;
        *(u16*)(e + 0x04) = 0;
        *(u16*)(e + 0x06) = 0;
        *(u32*)(e + 0x08) = 0;
        *(u32*)(e + 0x0C) = 0;
        *(u32*)(e + 0x10) = -1;
        *(u8*)(e + 0x14) = 0;
        *(u32*)(e + 0x18) = 0;
        *(u32*)(e + 0x1C) = 0;
    }
}
#endif

/* 0x80133050 | 0x3C -- fn_800D37D4(3, 2, 0, 2, 0, 0), return 0 */
extern void fn_800D37D4(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
u32 fn_80133050(void) {
    fn_800D37D4(3, 2, 0, 2, 0, 0);
    return 0;
}

/* 0x8013308C | 0x3C -- fn_800D37D4(2, 2, 0, 2, 0, 0), return 0 */
u32 fn_8013308C(void) {
    fn_800D37D4(2, 2, 0, 2, 0, 0);
    return 0;
}

/* 0x801330C8 | 0x150 */
extern void fn_800D88DC(u32);
extern void fn_800D888C(u32);
extern void fn_800D9B58(f32, f32, f32, f32);
extern void fn_800DA4C4(u32, u32, u32);
extern void fn_800DA2BC(u32, u32, u32);
extern void fn_800DA1E8(u32, u32, u32);
extern void fn_800DA028(u32);
extern void fn_800D6A00(u32);
extern void fn_800D7820(u32);
extern void fn_800D67BC(u32);
extern void fn_800D6680(f32, f32, f32);
extern void fn_800D5CB8(u32, u32, u32, u32, u32);
extern void fn_800D6728(void);
extern f32 lbl_8047D0F0;
extern f32 lbl_8047D0F4;
extern f32 lbl_8047D0F8;
extern u8 lbl_80478AC0[];
extern f32 lbl_8047D0FC;
extern u8 lbl_80478AC4[];
extern f32 lbl_8047D100;
extern f32 lbl_8047D104;
#if 0
asm void fn_801330C8(void) {
#include "src/game/effect/effect_util_fn_801330C8.inc"
}
#else
u32 fn_801330C8(void) {
    fn_800D88DC(1);
    fn_800D888C(6);
    fn_800D9B58(lbl_8047D0F0, lbl_8047D0F0, lbl_8047D0F4, lbl_8047D0F8);
    fn_800DA4C4(0, 6, 7);
    fn_800DA2BC(1, 1, 0);
    fn_800DA1E8(0, 1, 1);
    fn_800DA028(0);
    fn_800D6A00(4);
    fn_800D7820(0);
    fn_800D67BC(3);
    fn_800D6680(lbl_8047D0FC, -*(f32*)lbl_80478AC0, lbl_8047D0F0);
    fn_800D5CB8(0, 0xFF, 0x80, 0x40, 0);
    fn_800D6680(*(f32*)lbl_80478AC4, lbl_8047D100, lbl_8047D0F0);
    fn_800D5CB8(0, 0x40, 0xFF, 0, 0);
    fn_800D6680(lbl_8047D104, *(f32*)lbl_80478AC4, lbl_8047D0F0);
    fn_800D5CB8(0, 0, 0x40, 0xFF, 0);
    fn_800D6680(lbl_8047D0FC, -*(f32*)lbl_80478AC0, lbl_8047D0F0);
    fn_800D5CB8(0, 0xFF, 0x80, 0x40, 0);
    fn_800D6728();
    return 0;
}
#endif

/* 0x80133218 | 0x38 -- fn_800E1544() then print result, return 0 */
extern u32  fn_800E1544(void);
extern void fn_800DD970(const char* fmt, ...);
extern const char lbl_80272AB8[];
u32 fn_80133218(void) {
    u32 val = fn_800E1544();
    fn_800DD970(lbl_80272AB8, val);
    return 0;
}

/* 0x80133250 | 0x2C -- fn_800E0E14(1, 1), return 0 */
extern u8 fn_800E0E14(u32 a, u32 b);
u32 fn_80133250(void) {
    fn_800E0E14(1, 1);
    return 0;
}

/* 0x5C | fn_8013327C -- fn_800E0E14(1,0) check then print */
extern const char lbl_80272AE0[];
extern const char lbl_80272AF0[];
/* 0x801332D8 | 0x28 -- fn_800D3074(2), return 0 */
extern void fn_800D3074(u32 mode);
u32 fn_801332D8(void) {
    fn_800D3074(2);
    return 0;
}

/* 0x80133300 | 0x28 -- fn_800D3074(1), return 0 */
u32 fn_80133300(void) {
    fn_800D3074(1);
    return 0;
}

/* 0x80133328 | 36 bytes | call_return_const2 */
u32 fn_80133328(void) {
    fn_801D216C();
    return 0;
}

/* 0x801333AC | 0xA4 */
extern void fn_800F9318(void);
#if 0
asm void fn_801333AC(void) {
#include "src/game/effect/effect_util_fn_801333AC.inc"
}
#else
#pragma optimization_level 4
u32 fn_801333AC(s32 arg) {
    extern void* fn_800F9318(u32, u32);
    u8* ptr;
    ptr = (u8*)fn_800F9318(0, 2);
    if (ptr == 0) return 0;
    switch (arg) {
    case 0xb8:
        if (ptr[0] != 0) {
            ptr[0] = 0;
        } else {
            ptr[0] = 1;
        }
        break;
    case 0xb9:
        if (ptr[1] != 0) {
            ptr[1] = 0;
        } else {
            ptr[1] = 1;
        }
        break;
    }
    return 0;
}
#endif

/* 0x58 | fn_80133450 | call_sequence */
extern u32 lbl_80478820;
#if 0
asm void fn_80133450(void) {
#include "src/game/effect/effect_util_fn_80133450.inc"
}
#else
u32 fn_80133450(void) {
    *(u8*)&lbl_80478820 = 0;
    fn_801026A4(5, 0, 0, 0, 1, 0);
    fn_80102510(5);
    *(u8*)&lbl_80478820 = 1;
    return 0;
}
#endif

/* 0x801334A8 | 0x34 -- toggle lbl_8047AED9 (cntlzw/extrwi), call fn_800E8F80 */
extern u8   lbl_8047AED9;
extern void fn_800E8F80(u32 val);
u32 fn_801334A8(void) {
    u32 clz = __cntlzw(lbl_8047AED9);
    u8 val = (u8)((clz >> 5) & 0xFF);
    lbl_8047AED9 = val;
    fn_800E8F80(val);
    return 0;
}

/* 0x801334DC | 0x34 -- toggle lbl_8047AED8 (cntlzw/extrwi), call fn_800D4610 */
extern u8   lbl_8047AED8;
extern void fn_800D4610(u32 val);
u32 fn_801334DC(void) {
    u32 clz = __cntlzw(lbl_8047AED8);
    u8 val = (u8)((clz >> 5) & 0xFF);
    lbl_8047AED8 = val;
    fn_800D4610(val);
    return 0;
}

/* 0x80133630 | 0x34 -- toggle lbl_8047AED4 (cntlzw>>5), call fn_80101B88 */
extern u32  lbl_8047AED4;
u32 fn_80133630(void) {
    u32 clz = __cntlzw(lbl_8047AED4);
    lbl_8047AED4 = clz >> 5;
    fn_80101B88(clz >> 5);
    return 0;
}

/* 0x80133664 | 0x13C */
extern void* fn_80105624(void);
extern u32 fn_8005D9E4(u32 arg);
#if 0
asm void fn_80133664(void) {
#include "src/game/effect/effect_util_fn_80133664.inc"
}
#else
void fn_80133664(void* obj) {
    u8 pair[2];
    s32 entryCount;
    s32 maxCount;
    u16 flags;
    s8 major;
    s8 minor;

    flags = *(u16*)((u8*)fn_80105624() + 0x6);
    entryCount = (s8)fn_80133B50((u32)obj, NULL);
    maxCount = (s8)fn_8005D9E4(*(u32*)((u8*)obj + 0x04));
    if (entryCount < maxCount) {
        maxCount = entryCount;
    }

    *(u16*)pair = *(u16*)((u8*)obj + 0x94);
    if (flags & 1) {
        pair[1]--;
    } else if (flags & 2) {
        pair[1]++;
    }

    major = (s8)pair[0];
    minor = (s8)pair[1];
    if (minor < 0) {
        pair[1] = 0;
        pair[0] = (u8)(major + minor);
        if ((s8)pair[0] < 0) {
            pair[1] = (u8)((s8)maxCount - 1);
            pair[0] = (u8)(entryCount - (s8)maxCount);
        }
    } else if (minor >= (s8)maxCount) {
        pair[1] = (u8)((s8)maxCount - 1);
        pair[0] = (u8)(major + (minor - ((s8)maxCount - 1)));
        if (((s8)pair[0] + (s8)pair[1]) >= entryCount) {
            pair[0] = 0;
            pair[1] = 0;
        }
    }

    *(u16*)((u8*)obj + 0x94) = *(u16*)pair;
}
#endif

/* 0x801337A0 | 0x8 | sda_getter */
u8 fn_801337A0(void) { return lbl_8047AED0; }

/* 0x801337A8 | 0x8 | sda_setter */
void fn_801337A8(u8 val) { lbl_8047AED0 = val; }

/* 0x801337B0 | 0x34 -- check fn_80102620(lbl_80478848) != 0, return 0 or 1 */
extern u32  lbl_80478848;
u32 fn_801337B0(void) {
    u8 result = fn_80102620(lbl_80478848);
    return (result != 0) ? 1 : 0;
}

/* 0x801337E4 | 0x2C -- set lbl_8047AED1 = 0, call fn_80102510(lbl_80478848) */
extern u8   lbl_8047AED1;
extern u32  lbl_80478848;
extern u8   lbl_8047AED1;
#if 0
asm void fn_801337E4(void) {
#include "src/game/effect/effect_util_fn_801337E4.inc"
}
#else
void fn_801337E4(void) {
    lbl_8047AED1 = 0;
    fn_80102510(lbl_80478848);
}
#endif

/* 0x80133810 | 0x94 */
extern u32 lbl_80478F88;
extern u8 lbl_8047AED0;
extern u32  lbl_80478848;
extern u8   lbl_8047AED1;
#if 0
asm void fn_80133810(void) {
#include "src/game/effect/effect_util_fn_80133810.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80133810(u8 flag) {
    extern s32 fn_801338A4(u32);
    u32 (*fp)(void);
    u32 result;
    fp = (u32 (*)(void))lbl_80478F88;
    if (fp == NULL) {
        result = 0;
    } else {
        result = fp();
    }
    if (result == 0) {
        return;
    }
    if (lbl_8047AED0 == 0) {
        return;
    }
    if (fn_80102620(lbl_80478848)) {
        return;
    }
    lbl_8047AED1 = flag;
    do {
        if (fn_801338A4(0) < 0) {
            return;
        }
    } while (lbl_8047AED1 == 1);
}
#pragma pop
#endif

/* 0x801338A4 | 0x2AC */
extern u32  lbl_80478848;
extern u32 lbl_8047AEDC;
extern u32 lbl_80478F88;
extern u32 lbl_80478F8C;
#if 0
asm void fn_801338A4(void) {
#include "src/game/effect/effect_util_fn_801338A4.inc"
}
#else
s32 fn_801338A4(s32 offset) {
    s32 prevOffset;
    s32 result;
    s32 sceneId;
    s32 key;
    s32 valueIndex;
    s32 link;
    s32 callbackResult;
    s32* outValue;
    u8* obj;
    EffectUtilEntry* entry;
    EffectUtilCountFunc countFunc;
    EffectUtilEntryFunc entryFunc;
    EffectUtilEntryCallback callback;

    prevOffset = offset - 1;
    result = 0;
    sceneId = (s32)lbl_80478848 + offset;

retry:
    if (offset < 0) {
        key = 0;
    } else {
        key = (s32)lbl_80478848 + prevOffset;
    }

    outValue = (s32*)(lbl_8047AEDC + fn_80133C3C(key) * 4);
    valueIndex = fn_801026A4(sceneId, key, outValue, 0, 1, 0);
    obj = (u8*)fn_80104704(sceneId);
    if (obj != NULL) {
        *outValue = (s8)obj[0x94] + (s8)obj[0x95];
    } else {
        *outValue = 0;
    }

    if (valueIndex == -1) {
        if (offset == 0) {
            result = -1;
        }
        fn_80102510(sceneId);
        return result;
    }

    valueIndex = fn_80133E6C(obj, (s8)obj[0x94] + (s8)obj[0x95]);
    link = 0;
    if (valueIndex > 0) {
        countFunc = (EffectUtilCountFunc)lbl_80478F88;
        if (countFunc != NULL && countFunc() > valueIndex) {
            entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
            entry = entryFunc != NULL ? entryFunc(valueIndex) : NULL;
            if (entry != NULL) {
                link = entry->link;
            }
            if (link > 0) {
                countFunc = (EffectUtilCountFunc)lbl_80478F88;
                if (countFunc == NULL || countFunc() <= link) {
                    link = 0;
                }
            } else {
                link = 0;
            }
        }
    }

    if ((s16)link != 0) {
        entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
        entry = entryFunc != NULL ? entryFunc(valueIndex) : NULL;
        callback = entry != NULL ? entry->callback : NULL;
        if (callback != NULL) {
            callbackResult = callback(valueIndex, (s8)obj[0x94] + (s8)obj[0x95]);
        } else {
            callbackResult = 1;
        }
        if (callbackResult == 0) {
            fn_80102510(lbl_80478848);
            return 1;
        }
        if (callbackResult == -1 || (s16)link == 1) {
            goto retry;
        }
        if (fn_801338A4(offset + 1) != 1) {
            goto retry;
        }
        return 1;
    }

    fn_80102510(lbl_80478848);
    entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
    entry = entryFunc != NULL ? entryFunc(valueIndex) : NULL;
    callback = entry != NULL ? entry->callback : NULL;
    if (callback != NULL) {
        callback(valueIndex, (s8)obj[0x94] + (s8)obj[0x95]);
    }
    return 1;
}
#endif

/* 0x80133B50 | 0x94 */
extern u32 fn_800FA444(u32 val);
#if 0
asm void fn_80133B50(void) {
#include "src/game/effect/effect_util_fn_80133B50.inc"
}
#else
u32 fn_80133B50(u32 arg0, u32* outMax) {
    u32 index;
    u32 value;
    u32 max;
    s32 done;

    if (outMax != NULL) {
        *outMax = 0;
    }
    index = 0;
    do {
        value = fn_80133E1C((void*)arg0, index);
        if (outMax != NULL) {
            value = fn_800FA444(value) >> 16;
            max = *outMax;
            if ((s32)max < (s32)value) {
                *outMax = value;
            }
        }
        done = fn_80133BE4((void*)arg0, index++);
    } while (done == 0);
    return index;
}
#endif

/* 0x80133C3C | 0x1E0 */
extern void fn_800057A8(void);
extern u32 lbl_80478F88;
extern u32 lbl_80478F8C;
#if 0
asm void fn_80133C3C(void) {
#include "src/game/effect/effect_util_fn_80133C3C.inc"
}
#else
s32 fn_80133C3C(s32 key) {
    s32 count;
    s32 index;
    s32 selected;
    u8* obj;
    EffectUtilEntry* entry;
    EffectUtilCountFunc countFunc;
    EffectUtilEntryFunc entryFunc;

    obj = (u8*)fn_80104704(key);
    if (obj != NULL) {
        selected = fn_80133E6C(obj, (s8)obj[0x94] + (s8)obj[0x95]);
        if (selected > 0) {
            countFunc = (EffectUtilCountFunc)lbl_80478F88;
            if (countFunc != NULL && countFunc() > selected) {
                entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
                entry = entryFunc != NULL ? entryFunc(selected) : NULL;
                selected = entry != NULL ? entry->link : 0;
                if (selected > 0) {
                    countFunc = (EffectUtilCountFunc)lbl_80478F88;
                    if (countFunc == NULL || countFunc() <= selected) {
                        selected = 0;
                    }
                } else {
                    selected = 0;
                }
            } else {
                selected = 0;
            }
        } else {
            selected = 0;
        }
        count = 0;
        for (index = 0; index < (s16)selected; index++) {
            entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
            entry = entryFunc != NULL ? entryFunc(index) : NULL;
            if (entry == NULL || (entry->flags & 0x80)) {
                count++;
            }
        }
    } else {
        count = 0;
        for (index = 0; index < (s32)fn_80134274(); index++) {
            entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
            entry = entryFunc != NULL ? entryFunc(index) : NULL;
            if (entry == NULL || (entry->flags & 0x80)) {
                count++;
            }
        }
    }
    return count;
}
#endif

/* 0x80133E6C | 0x2F8 */
extern u32  lbl_80478848;
extern u32 lbl_80478F88;
extern u32 lbl_80478F8C;
#if 0
asm void fn_80133E6C(void) {
#include "src/game/effect/effect_util_fn_80133E6C.inc"
}
#else
s32 fn_80133E6C(void* obj, s32 offset) {
    s32 rel;
    s32 value;
    s32 baseValue;
    s32 linked;
    u8* prev;
    u8* prior;
    EffectUtilCountFunc countFunc;
    EffectUtilEntryFunc entryFunc;
    EffectUtilEntry* entry;

    rel = fn_80134258(obj);
    if (rel < 0) {
        return 0;
    }
    if (rel == 0) {
        return (s32)fn_80134274() + offset;
    }

    prev = (u8*)fn_80134228(rel - 1);
    if (prev == NULL) {
        value = fn_80134274();
    } else {
        baseValue = (s8)prev[0x94] + (s8)prev[0x95];
        rel = fn_80134258(prev);
        if (rel < 0) {
            value = 0;
        } else if (rel == 0) {
            value = (s32)fn_80134274() + baseValue;
        } else {
            prior = (u8*)fn_80134228(rel - 1);
            if (prior == NULL) {
                linked = fn_80134274();
            } else {
                linked = (s8)prior[0x94] + (s8)prior[0x95];
                rel = fn_80134258(prior);
                if (rel < 0) {
                    linked = 0;
                } else if (rel == 0) {
                    linked += fn_80134274();
                } else {
                    u8* earlier = (u8*)fn_80134228(rel - 1);
                    if (earlier == NULL) {
                        linked += fn_80134274();
                    } else {
                        linked += (s16)_dbgMenuGetLink__Fl(fn_80133E6C(earlier, (s8)earlier[0x94] + (s8)earlier[0x95]));
                    }
                }
            }

            if (linked > 0 && (s32)fn_80134304() > linked) {
                linked = fn_801342B8(linked);
                if ((s16)linked <= 0 || (s32)fn_80134304() <= (s16)linked) {
                    linked = 0;
                }
            } else {
                linked = 0;
            }
            value = (s16)linked + baseValue;
        }
    }

    if (value > 0) {
        countFunc = (EffectUtilCountFunc)lbl_80478F88;
        if (countFunc != NULL && countFunc() > value) {
            entryFunc = (EffectUtilEntryFunc)lbl_80478F8C;
            entry = entryFunc != NULL ? entryFunc(value) : NULL;
            value = entry != NULL ? entry->link : 0;
            if ((s16)value > 0) {
                countFunc = (EffectUtilCountFunc)lbl_80478F88;
                if (countFunc != NULL && countFunc() > (s16)value) {
                    return (s16)value + offset;
                }
            }
        }
    }

    return offset;
}
#endif

/* 0x80134164 | 0xC4 */
extern u32 lbl_80478F88;
extern u32 lbl_80478F8C;
#if 0
asm void _dbgMenuGetLink__Fl(void) {
#include "src/game/effect/effect_util_fn_80134164.inc"
}
#else
s32 _dbgMenuGetLink__Fl(s32 idx) {
    s32 count;
    EffectUtilEntry* result;

    if (idx <= 0) { return 0; }

    {
        s32 (*fp)(void) = (s32 (*)(void))lbl_80478F88;
        if (fp == NULL) { count = 0; } else { count = fp(); }
    }
    if (count <= idx) { return 0; }

    {
        EffectUtilEntryFunc fp = (EffectUtilEntryFunc)lbl_80478F8C;
        if (fp == NULL) { result = NULL; } else { result = fp(idx); }
    }
    if (result == NULL) { idx = 0; } else { idx = result->link; }

    if ((s16)idx <= 0) {
        idx = 0;
    } else {
        s32 (*fp)(void) = (s32 (*)(void))lbl_80478F88;
        if (fp == NULL) { count = 0; } else { count = fp(); }
        if (count <= (s16)idx) { idx = 0; }
    }
    return idx;
}
#endif

/* 0x80134228 | 0x30 -- saturate add: max(0, lbl_80478848 + arg) then fn_80104704 */
/* extern void* fn_80104704(s32 key); -- forward-declared K&R style above */
#pragma push
#pragma optimization_level 1
void* fn_80134228(s32 offset) {
    s32 key;
    s32 mask;
    mask = offset >> 31;
    key = (s32)lbl_80478848 + offset;
    return fn_80104704(key & ~mask);
}
#pragma pop

/* 0x80134258 | 0x1C -- get relative key from obj->0x04, return (key - lbl_80478848), or -1 */
s32 fn_80134258(void* obj) {
    u32 val = *(u32*)((u8*)obj + 0x04);
    if ((s32)val < (s32)lbl_80478848) return -1;
    return (s32)(val - lbl_80478848);
}

/* 0x80134304 | 0x38 */
extern u32 lbl_80478F88;
#if 0
asm u32 fn_80134304(void) {
#include "src/game/effect/effect_util_fn_80134304.inc"
}
#else
u32 fn_80134304(void) {
    u32 (*fp)(void) = (u32 (*)(void))lbl_80478F88;
    if (fp == NULL) {
        return 0;
    }
    return fp();
}
#endif

/* 0x8013433C | 0xE4 */
extern void fn_80140A9C(void);
#if 0
asm void fn_8013433C(void) {
#include "src/game/effect/effect_util_fn_8013433C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 fn_8013433C(void* base, s16 idx1, s16 idx2) {
    extern void fn_80140A9C(void*, void*);
    void* b;
    void* b2;
    void* entry1;
    void* entry2;
    s16 i;
    b = base;
    if (b == 0) {
        b = (void*)fn_80129280(0, 3);
    }
    i = idx1;
    if (i < 0 || i >= 0xeb) {
        entry1 = 0;
    } else {
        entry1 = (u8*)b + 0x6dec + (s32)i * 4;
    }
    if (entry1 == 0) return 0;
    b2 = base;
    if (base == 0) {
        b2 = (void*)fn_80129280(0, 3);
    }
    i = idx2;
    if (i < 0 || i >= 0xeb) {
        entry2 = 0;
    } else {
        entry2 = (u8*)b2 + 0x6dec + (s32)i * 4;
    }
    if (entry2 == 0) return 0;
    fn_80140A9C(entry1, entry2);
    return 1;
}
#pragma scheduling off
#endif

/* 0x80134420 | 0x164 */
#if 0
asm void fn_80134420(void) {
#include "src/game/effect/effect_util_fn_80134420.inc"
}
#else
#pragma optimization_level 4
u16 fn_80134420(void* base, u16 effect_id) {
    extern u8 fn_801440A0(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    void* cur;
    void* entry;
    s16 idx;
    u16 val;
    s32 i;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    if (!fn_801440A0(effect_id)) return 0;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    if (idx < 0) return 0;
    entry = (idx >= 0 && idx < 0xeb) ? ((u8*)base + 0x6dec + (s32)idx * 4) : 0;
    if (entry == 0) {
        val = 0xFFFF;
    } else if (fn_801429E8(entry)) {
        val = itemGetStatus(entry, 0, 0x1c, 0) & 0xFFFF;
    } else {
        val = 0xFFFF;
    }
    if (val > 0x3e7) val = 0;
    return (u16)(0x3e7 - val);
}
#endif

/* 0x80134584 | 0xF8 */
#if 0
asm void fn_80134584(void) {
#include "src/game/effect/effect_util_fn_80134584.inc"
}
#else
#pragma scheduling on
u16 fn_80134584(void* base, u16 effect_id, u16 r5) {
    extern u8 fn_801440A0(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    extern u16 fn_80140ACC(void*, u16, u16, u16, s16, u16, u32);
    void* cur; s16 idx; s32 i;
    if (base == 0) { base = (void*)fn_80129280(0, 3); }
    if (r5 == 0) return r5;
    if (!fn_801440A0(effect_id)) return r5;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    if (idx < 0) return r5;
    return (u16)fn_80140ACC((u8*)base + 0x6dec, 0xeb, effect_id, r5, idx, 0x3e7, 0);
}
#pragma scheduling off
#endif

/* 0x8013467C | 0xEC */
#if 0
asm void fn_8013467C(void) {
#include "src/game/effect/effect_util_fn_8013467C.inc"
}
#else
#pragma scheduling on
u16 fn_8013467C(void* base, u16 effect_id, u16 r5) {
    extern u8 fn_801440A0(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    extern u16 fn_80141308(void*, u16, u16, u16, s16, u16, u32, u32);
    void* cur; s16 idx; s32 i;
    if (base == 0) { base = (void*)fn_80129280(0, 3); }
    if (r5 == 0) return r5;
    if (!fn_801440A0(effect_id)) return r5;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    return (u16)fn_80141308((u8*)base + 0x6dec, 0xeb, effect_id, r5, idx, 0x3e7, 0, 0);
}
#pragma scheduling off
#endif

/* 0x801347D0 | 0x8 | return_const */
u32 fn_801347D0(void) { return 235; }

/* 0x801347D8 | 0x8 | return_const */
u32 fn_801347D8(void) { return 30; }

/* 0x801347E0 | 0x8 | return_const */
u32 fn_801347E0(void) { return 3; }

/* 0x801347E8 | 0x104 */
extern void fn_80123FBC(void);
#if 0
asm void fn_801347E8(void) {
#include "src/game/effect/effect_util_fn_801347E8.inc"
}
#else
#pragma optimization_level 4
s8 fn_801347E8(void* base, s8 slot) {
    extern u8 fn_80123FBC(void*);
    u8* cur;
    s8 count;
    s8 i;
    count = 0;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    if (slot < 0 || slot >= 3) {
        count = -1;
    } else {
        cur = (u8*)base + (s32)slot * 0x24a4;
        i = 0;
        while (i < 0x1e) {
            if (fn_80123FBC(cur + 0x14)) {
                count++;
            }
            cur += 0x138;
            i++;
        }
    }
    if (count < 0) {
        return -1;
    }
    return (s8)(0x1e - count);
}
#endif

/* 0x801348EC | 0xF0 */
#if 0
asm void fn_801348EC(void) {
#include "src/game/effect/effect_util_fn_801348EC.inc"
}
#else
#pragma optimization_level 4
s32 fn_801348EC(void* base, void* src, s8 slot, s8 idx) {
    extern void fn_8012086C(void*);
    u8* entry;
    s8 s;
    s8 e;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    s = slot;
    e = idx;
    if (s < 0 || s >= 3 || e < 0 || e >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    fn_8012086C(entry);
    return 1;
}
#endif

/* 0x801349DC | 0xBC */
extern void fn_800F9E70(void);
#if 0
asm void fn_801349DC(void) {
#include "src/game/effect/effect_util_fn_801349DC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 fn_801349DC(void* base, s8 slot, u16* name) {
    extern void fn_800F9E70(void*, u16*);
    s32 len;
    u16* p;
    s8 s;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    s = slot;
    if (s < 0 || s >= 3) return 0;
    if (name == 0) return 0;
    p = name;
    len = 0;
    while (*p != 0) {
        p++;
        len++;
    }
    if (len > 8) return 0;
    fn_800F9E70((u8*)base + (s32)s * 0x24a4, name);
    return 1;
}
#pragma scheduling off
#endif

/* 0x80134AF8 | 0xC8 */
extern void fn_80124A60(void);
#if 0
asm void fn_80134AF8(void) {
#include "src/game/effect/effect_util_fn_80134AF8.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 fn_80134AF8(void* base, s8 slot, s8 idx) {
    extern u8 fn_80123FBC(void*);
    extern void fn_80124A60(void*);
    u8* entry;
    s8 s;
    s8 e;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    s = slot;
    if (s < 0 || s >= 3) {
        entry = 0;
    } else {
        e = idx;
        if (e < 0 || e >= 0x1e) {
            entry = 0;
        } else {
            entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
        }
    }
    if (entry == 0) return 0;
    if (!fn_80123FBC(entry)) return 0;
    fn_80124A60(entry);
    return 1;
}
#pragma scheduling off
#endif

/* 0x80134BC0 | 0x250 */
extern void fn_8012086C(void);
#if 0
asm void fn_80134BC0(void) {
#include "src/game/effect/effect_util_fn_80134BC0.inc"
}
#else
#pragma optimization_level 2
s32 fn_80134BC0(void* base, void* src, s8 slot) {
    extern u8 fn_80123FBC(void*);
    extern void fn_8012086C(void*);
    u8* slotbase;
    u8* entry;
    s8 found_slot;
    s8 found_entry;
    s8 si;
    s8 ei;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    if (slot < -1 || slot >= 3) return 0;
    if (src == 0) return 0;
    found_slot = slot;
    found_entry = -1;
    if (slot == -1) {
        /* Search all slots for first free entry */
        slotbase = (u8*)base;
        for (si = 0; si < 3; si++) {
            for (ei = 0; ei < 0x1e; ei++) {
                entry = (ei >= 0 && ei < 0x1e) ? (slotbase + (s32)ei * 0x138 + 0x14) : 0;
                if (entry != 0 && !fn_80123FBC(entry)) {
                    found_entry = ei;
                    break;
                }
            }
            if (found_entry >= 0) {
                found_slot = si;
                break;
            }
            slotbase += 0x24a4;
        }
        if (si >= 3) return 0;
    } else {
        /* Search specific slot for first free entry */
        slotbase = (u8*)base + (s32)slot * 0x24a4;
        for (ei = 0; ei < 0x1e; ei++) {
            entry = (ei >= 0 && ei < 0x1e) ? (slotbase + (s32)ei * 0x138 + 0x14) : 0;
            if (entry != 0 && !fn_80123FBC(entry)) {
                found_entry = ei;
                break;
            }
        }
        if (found_entry < 0) return 0;
    }
    /* Compute entry address */
    if (found_slot < 0 || found_slot >= 3 || found_entry < 0 || found_entry >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)found_slot * 0x24a4 + (s32)found_entry * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    fn_8012086C(entry);
    return 1;
}
#endif

/* 0x80134E10 | 0xE0 */
#if 0
asm void fn_80134E10(void) {
#include "src/game/effect/effect_util_fn_80134E10.inc"
}
#else
#pragma optimization_level 4
s32 fn_80134E10(void* base, void* src, s8 slot, s8 idx) {
    extern void fn_8012086C(void*);
    u8* entry;
    s8 s;
    s8 e;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    s = slot;
    e = idx;
    if (s < 0 || s >= 3 || e < 0 || e >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    fn_8012086C(entry);
    return 1;
}
#endif

/* 0x80134EF0 | 0x98 */
#if 0
asm void fn_80134EF0(void) {
#include "src/game/effect/effect_util_fn_80134EF0.inc"
}
#else
#pragma scheduling on
void* fn_80134EF0(void* base, s8 r4, s8 r5) {
    s8 slot;
    s8 entry;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    slot = r4;
    if (slot < 0 || slot >= 3) {
        return 0;
    }
    entry = r5;
    if (entry < 0 || entry >= 0x1e) {
        return 0;
    }
    return (u8*)base + (s32)slot * 0x24a4 + (s32)entry * 0x138 + 0x14;
}
#pragma scheduling off
#endif

/* 0x80134F88 | 0x9C */
extern void fn_800F96E4(void);
extern void fn_801249F8(void);
extern void fn_80142A88(void);
#if 0
asm void fn_80134F88(void) {
#include "src/game/effect/effect_util_fn_80134F88.inc"
}
#else
#pragma scheduling on
void fn_80134F88(void* base) {
    extern void fn_80132A38(u32, u32);
    extern void fn_800F96E4(void*, u32, u32);
    extern void fn_801249F8(void*, u32);
    extern void fn_80142A88(void*, u32);
    u8* cur;
    s32 i;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    i = 0;
    cur = (u8*)base;
    do {
        fn_80132A38(0x34, i + 1);
        fn_800F96E4(cur, 9, 0x32c9);
        fn_801249F8(cur + 0x14, 0x1e);
        i++;
        cur += 0x24a4;
    } while (i < 3);
    fn_80142A88((u8*)base + 0x6dec, 0xeb);
}
#pragma scheduling off
#endif

/* 0x80135024 | 0x4 | void_stub */
#if 0
asm void fn_80135024(void) {
#include "src/game/effect/effect_util_fn_80135024.inc"
}
#else
#pragma optimization_level 4
void fn_80135024(void) {
}
#endif

/* 0x80135028 | 0x8 | return_const */
u32 fn_80135028() { return 0; }

/* 0x80135030 | 0x138 */
extern f64 lbl_8047D108;
#if 0
asm void fn_80135030(void) {
#include "src/game/effect/effect_util_fn_80135030.inc"
}
#else
void fn_80135030(void* ptr, u16 kind, u32 value) {
    void* sub;

    if (kind == 0 || kind >= 0xB) {
        return;
    }

    if (ptr == NULL) {
        ptr = (void*)fn_80129280(0, 0);
        if (ptr == NULL) {
            return;
        }
        ptr = (void*)fn_80129280((u32)ptr, 1);
        if (ptr == NULL) {
            return;
        }
    }

    sub = fn_80135CD0(ptr);
    if (sub == NULL) {
        return;
    }

    switch (kind) {
    case 1:
        fn_80135C90(ptr, (void*)value);
        break;
    case 2:
        fn_80135BA0(sub, value);
        break;
    case 3:
        fn_80135B7C(sub, value);
        break;
    case 4:
        fn_80135B6C(sub, value);
        break;
    case 5:
        fn_80135B5C(sub, (f32)(s32)value);
        break;
    case 6:
        fn_80135B4C(sub, value);
        break;
    case 7:
        fn_80135B3C(sub, (u8)value);
        break;
    case 8:
        fn_80135B2C(sub, (u8)value);
        break;
    case 9:
        fn_80135B1C(sub, (u8)value);
        break;
    default:
        break;
    }
}
#endif

/* 0x80135168 | 0x124 */
extern void jumptable_80363A9C();
#if 0
asm void fn_80135168(void) {
#include "src/game/effect/effect_util_fn_80135168.inc"
}
#else
u32 fn_80135168(void* ptr, u16 kind) {
    void* base = NULL;
    void* sub;

    if (kind == 0 || kind >= 0xB) {
        return 0;
    }
    if (ptr == NULL) {
        base = (void*)fn_80129280(0, 0);
        if (base == NULL) {
            return 0;
        }
        ptr = (void*)fn_80129280((u32)base, 1);
        if (ptr == NULL) {
            return 0;
        }
    }
    sub = fn_80135CD0(ptr);
    if (sub == NULL) {
        return 0;
    }
    switch (kind) {
    case 0:
        return (u32)base;
    case 1:
        return fn_80135C78(sub);
    case 2:
        return fn_80135C40(sub);
    case 3:
        return fn_80135C28(sub);
    case 4:
        return (s32)fn_80135C10(sub);
    case 5:
        return fn_80135BF8(sub);
    case 6:
        return fn_80135BE0(sub);
    case 7:
        return fn_80135BC8(sub);
    case 8:
        return fn_80135BB0(sub);
    default:
        return 0;
    }
}
#endif

/* 0x8013528C | 0xAC */
#if 0
asm void fn_8013528C(void) {
#include "src/game/effect/effect_util_fn_8013528C.inc"
}
#else
#pragma optimization_level 4
void fn_8013528C(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    void* base;
    if (ptr == 0) return;
    fn_80135338(ptr);
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) return;
        base = (void*)fn_80129280((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)fn_80135B0C(base);
    if (base == 0) return;
    fn_801353C0(base, r4, r5, r6, r7);
}
#endif

/* 0x80135338 | 0x88 */
#if 0
asm void fn_80135338(void) {
#include "src/game/effect/effect_util_fn_80135338.inc"
}
#else
#pragma optimization_level 4
void fn_80135338(void* ptr) {
    void* base;
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) return;
        base = (void*)fn_80129280((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)fn_80135B0C(base);
    if (base == 0) return;
    fn_80135708(base);
    fn_80135CE8(ptr);
}
#endif

/* 0x801353C0 | 0x170 */
#if 0
asm void fn_801353C0(void) {
#include "src/game/effect/effect_util_fn_801353C0.inc"
}
#else
#pragma optimization_level 4
void fn_801353C0(void* ptr, u8 r4, u8 r5, u8 r6, u8 r7) {
    void* base;
    if (ptr == 0) return;
    if ((r4 & 0xFF) == 0) return;
    if ((r5 & 0xFF) == 0) return;
    if ((r6 & 0xFF) == 0) return;
    if ((r7 & 0xFF) == 0) return;
    fn_80135708(ptr);
    /* A60 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)fn_80129280((u32)base, 1); }
        base = (void*)fn_80135B0C(base);
        if (base != 0) { fn_80135A60(base, (u8)(r4 & 0xFF)); }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { fn_80135A60(base, (u8)(r4 & 0xFF)); }
    }
    /* A50 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)fn_80129280((u32)base, 1); }
        base = (void*)fn_80135B0C(base);
        if (base != 0) { fn_80135A50(base, (u8)(r5 & 0xFF)); }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { fn_80135A50(base, (u8)(r5 & 0xFF)); }
    }
    /* A40 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { base = 0; } else { base = (void*)fn_80129280((u32)base, 1); }
        base = (void*)fn_80135B0C(base);
        if (base != 0) { fn_80135A40(base, (u8)(r6 & 0xFF)); }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { fn_80135A40(base, (u8)(r6 & 0xFF)); }
    }
    /* A30 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) return;
        base = (void*)fn_80129280((u32)base, 1);
        if (base == 0) return;
    } else {
        base = ptr;
    }
    base = (void*)fn_80135B0C(base);
    if (base == 0) return;
    fn_80135A30(base, (u8)(r7 & 0xFF));
}
#endif

/* 0x80135530 | 0x1D8 */
#if 0
asm void fn_80135530(void) {
#include "src/game/effect/effect_util_fn_80135530.inc"
}
#else
#pragma optimization_level 4
u32 fn_80135530(void* ptr) {
    void* base; u32 r0; u32 r3;
    if (ptr == 0) return 0;
    /* AB8 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)fn_80129280((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)fn_80135B0C(base); r0 = base ? ((u32)fn_80135AB8(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { r0 = (u32)fn_80135AB8(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* AA0 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)fn_80129280((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)fn_80135B0C(base); r0 = base ? ((u32)fn_80135AA0(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { r0 = (u32)fn_80135AA0(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* A88 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { r0 = 0; } else {
            base = (void*)fn_80129280((u32)base, 1);
            if (base == 0) { r0 = 0; } else { base = (void*)fn_80135B0C(base); r0 = base ? ((u32)fn_80135A88(base) & 0xFF) : 0; }
        }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { r0 = (u32)fn_80135A88(base) & 0xFF; } else { r0 = 0; }
    }
    if (r0 == 0) return 0;
    /* A70 */
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) { r3 = 0; } else {
            base = (void*)fn_80129280((u32)base, 1);
            if (base == 0) { r3 = 0; } else { base = (void*)fn_80135B0C(base); if (base) { r3 = (u32)fn_80135A70(base) & 0xFF; } else { r3 = 0; } }
        }
    } else {
        base = (void*)fn_80135B0C(ptr);
        if (base != 0) { r3 = (u32)fn_80135A70(base) & 0xFF; } else { r3 = 0; }
    }
    return (r3 != 0) ? 1 : 0;
}
#endif

/* 0x80135708 | 0x134 */
#if 0
asm void fn_80135708(void) {
#include "src/game/effect/effect_util_fn_80135708.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
void fn_80135708(void* ptr) {
    void* r31 = ptr;
    void* base;
    if (r31 == 0) goto _end;
    /* A60 */
    base = r31;
    if (r31 != 0) goto _a60_handler;
    base = (void*)fn_80129280(0, 0);
    if (base == 0) goto _a50;
    base = (void*)fn_80129280((u32)base, 1);
    if (base == 0) goto _a50;
_a60_handler:
    base = (void*)fn_80135B0C(base);
    if (base == 0) goto _a50;
    fn_80135A60(base, 0);
_a50:
    /* A50 */
    base = r31;
    if (r31 != 0) goto _a50_handler;
    base = (void*)fn_80129280(0, 0);
    if (base == 0) goto _a40;
    base = (void*)fn_80129280((u32)base, 1);
    if (base == 0) goto _a40;
_a50_handler:
    base = (void*)fn_80135B0C(base);
    if (base == 0) goto _a40;
    fn_80135A50(base, 0);
_a40:
    /* A40 */
    base = r31;
    if (r31 != 0) goto _a40_handler;
    base = (void*)fn_80129280(0, 0);
    if (base == 0) goto _a30;
    base = (void*)fn_80129280((u32)base, 1);
    if (base == 0) goto _a30;
_a40_handler:
    base = (void*)fn_80135B0C(base);
    if (base == 0) goto _a30;
    fn_80135A40(base, 0);
_a30:
    /* A30 */
    base = r31;
    if (r31 != 0) goto _a30_handler;
    base = (void*)fn_80129280(0, 0);
    if (base == 0) goto _end;
    base = (void*)fn_80129280((u32)base, 1);
    if (base == 0) goto _end;
_a30_handler:
    base = (void*)fn_80135B0C(base);
    if (base == 0) goto _end;
    fn_80135A30(base, 0);
_end:;
}
#pragma scheduling off
#endif

/* 0x8013583C | 0xFC */
#if 0
asm void fn_8013583C(void) {
#include "src/game/effect/effect_util_fn_8013583C.inc"
}
#else
#pragma push
#pragma scheduling on
void fn_8013583C(void* ptr, u16 effect_type, u32 value) {
    void* base; u16 et;
    et = effect_type & 0xFFFF;
    if (et == 0 || et >= 7) return;
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base != 0) { base = (void*)fn_80129280((u32)base, 1); }
        ptr = base;
    }
    if (ptr == 0) return;
    base = (void*)fn_80135B0C(ptr);
    if (base == 0) return;
    switch (et) {
        case 1: fn_80135AEC((u32*)ptr, (u32*)value); break;
        case 2: fn_80135A60(base, (u8)(value & 0xFF)); break;
        case 3: fn_80135A50(base, (u8)(value & 0xFF)); break;
        case 4: fn_80135A40(base, (u8)(value & 0xFF)); break;
        case 5: fn_80135A30(base, (u8)(value & 0xFF)); break;
        default: break;
    }
}
#pragma pop
#endif

/* 0x80135938 | 0xF8 */
#if 0
asm void fn_80135938(void) {
#include "src/game/effect/effect_util_fn_80135938.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma optimization_level 1
u8 fn_80135938(void* ptr, u16 effect_type) {
    void* base; u16 et;
    et = effect_type & 0xFFFF;
    if (et == 0 || et >= 7) return 0;
    if (ptr == 0) {
        base = (void*)fn_80129280(0, 0);
        if (base == 0) return 0;
        base = (void*)fn_80129280((u32)base, 1);
        if (base == 0) return 0;
    } else {
        base = ptr;
    }
    base = (void*)fn_80135B0C(base);
    if (base == 0) return 0;
    switch (et) {
        case 1: return 0;
        case 2: return (u8)fn_80135AB8(base);
        case 3: return (u8)fn_80135AA0(base);
        case 4: return (u8)fn_80135A88(base);
        case 5: return (u8)fn_80135A70(base);
        default: return 0;
    }
}
#pragma pop
#endif

/* 0x80135A30 | 0x10 | nc_setter */
void fn_80135A30(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_03 = val;
}

/* 0x80135A40 | 0x10 | nc_setter */
void fn_80135A40(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_02 = val;
}

/* 0x80135A50 | 0x10 | nc_setter */
void fn_80135A50(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_01 = val;
}

/* 0x80135A60 | 0x10 | nc_setter */
void fn_80135A60(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_00 = val;
}

/* 0x80135A70 | 0x18 | nc_getter */
u8 fn_80135A70(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_03;
}

/* 0x80135A88 | 0x18 | nc_getter */
u8 fn_80135A88(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_02;
}

/* 0x80135AA0 | 0x18 | nc_getter */
u8 fn_80135AA0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_01;
}

/* 0x80135AB8 | 0x18 | nc_getter */
u8 fn_80135AB8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_00;
}

/* 0x80135AD0 | 0x1C */
#if 0
asm void fn_80135AD0(void) {
#include "src/game/effect/effect_util_fn_80135AD0.inc"
}
#else
#pragma optimization_level 4
void fn_80135AD0(void* dst, void* src) {
    if (dst == 0) return;
    if (src == 0) return;
    *(u32*)dst = *(u32*)src;
}
#endif

/* 0x80135AEC | 0x20 */
/* Copy the first u32 from src to dst, if both are non-NULL. */
void fn_80135AEC(u32* dst, u32* src) {
    if (dst == NULL || src == NULL) {
        return;
    }
    *dst = *src;
}

/* 0x80135B0C | 16 bytes | nc_bnelr */
u32 fn_80135B0C(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}

/* 0x80135B1C | 0x10 | nc_setter */
void fn_80135B1C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_22 = val;
}

/* 0x80135B2C | 0x10 | nc_setter */
void fn_80135B2C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_21 = val;
}

/* 0x80135B3C | 0x10 | nc_setter */
void fn_80135B3C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_20 = val;
}

/* 0x80135B4C | 0x10 | nc_setter */
void fn_80135B4C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_1C = val;
}

/* 0x80135B5C | 0x10 | nc_setter_f */
void fn_80135B5C(void* ptr, f32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_18 = val;
}

/* 0x80135B6C | 0x10 | nc_setter */
void fn_80135B6C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_14 = val;
}

/* 0x80135B7C | 0x10 | nc_setter */
void fn_80135B7C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_08 = val;
}

/* 0x80135B8C | 0x14 */
#if 0
asm void fn_80135B8C(void) {
#include "src/game/effect/effect_util_fn_80135B8C.inc"
}
#else
#pragma optimization_level 4
void fn_80135B8C(void* ptr, u32 unused, u32 a, u32 b) {
    if (ptr == 0) return;
    ((EffectParamBlock*)ptr)->field_04 = b;
    ((EffectParamBlock*)ptr)->word0.word = a;
}
#endif

/* 0x80135BA0 | 0x10 | nc_setter */
void fn_80135BA0(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_0C = val;
}

/* 0x80135BB0 | 24 bytes | beq_default_getter */
u8 fn_80135BB0(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_22;
_ret0:
    return 0;
}

/* 0x80135BC8 | 24 bytes | beq_default_getter */
u8 fn_80135BC8(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_21;
_ret0:
    return 0;
}

/* 0x80135BE0 | 24 bytes | beq_default_getter */
u8 fn_80135BE0(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_20;
_ret0:
    return 0;
}

/* 0x80135BF8 | 24 bytes | beq_default_getter */
u32 fn_80135BF8(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_1C;
_ret0:
    return 0;
}

/* 0x80135C10 | 0x18 */
extern f32 lbl_8047D110;
#if 0
asm f32 fn_80135C10(void* ptr) {
#include "src/game/effect/effect_util_fn_80135C10.inc"
}
#else
f32 fn_80135C10(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_18;
    }
    return lbl_8047D110;
}
#endif

/* 0x80135C28 | 24 bytes | beq_default_getter */
u32 fn_80135C28(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_14;
_ret0:
    return 0;
}

/* 0x80135C40 | 24 bytes | beq_default_getter */
u32 fn_80135C40(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_08;
_ret0:
    return 0;
}

/* 0x80135C58 | 0x20 */
#if 0
asm void fn_80135C58(void) {
#include "src/game/effect/effect_util_fn_80135C58.inc"
}
#else
/* Returns the r3:r4 pair (two words at *p) as a u64, or 0 if p is NULL.
 * byte-match verified: 8/8 instrs vs target; decomp.me scratch O3Iat = 100%. */
u64 fn_80135C58(u32* p) {
    if (p) return *(u64*)p;
    return 0;
}
#endif

/* 0x80135C78 | 24 bytes | beq_default_getter */
u32 fn_80135C78(void* ptr) {
    if (ptr == NULL) goto _ret0;
    return ((EffectParamBlock*)ptr)->field_0C;
_ret0:
    return 0;
}

/* 0x40 | fn_80135C90 | generic */
void fn_80135C90(void* dst, void* src) {
    if (dst == NULL) return;
    if (src != NULL) goto _copy;
    return;
_copy: {
    f64 f1 = *(f64*)((u8*)src + 0x00);
    f64 f0 = *(f64*)((u8*)src + 0x08);
    *(f64*)((u8*)dst + 0x08) = f1;
    *(f64*)((u8*)dst + 0x10) = f0;
    f1 = *(f64*)((u8*)src + 0x10);
    f0 = *(f64*)((u8*)src + 0x18);
    *(f64*)((u8*)dst + 0x18) = f1;
    *(f64*)((u8*)dst + 0x20) = f0;
    *(f64*)((u8*)dst + 0x28) = *(f64*)((u8*)src + 0x20);
    }
}

/* 0x80135CD0 | 24 bytes | nc_addi_ptr */
void* fn_80135CD0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}

/* 0x80135CE8 | 0x28 */
#if 0
asm void fn_80135CE8(void) {
#include "src/game/effect/effect_util_fn_80135CE8.inc"
}
#else
#pragma optimization_level 4
void fn_80135CE8(void* ptr) {
    void* sub;
    if (ptr == 0) {
        sub = 0;
    } else {
        sub = (u8*)ptr + 0x8;
    }
    if (sub == 0) return;
    *(u32*)((u8*)sub + 0x8) = 0;
}
#endif

/* 0x80135D10 | 0x134 */
extern u32 fn_8012A450();
extern u32 fn_80142B24();
extern u32 fn_801254B4();
extern u32 fn_8011BBD8();
extern u32 fn_801F4C14();
extern u32 fn_801F75F8();
extern u32 fn_801FAA58();
extern void jumptable_80363AC8();
#if 0
asm void fn_80135D10(void) {
#include "src/game/effect/effect_util_fn_80135D10.inc"
}
#else
#pragma scheduling on
#pragma push
#pragma optimization_level 2
#pragma peephole off
#pragma scheduling on
u32 fn_80135D10(u32 kind, u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    extern u32 fn_8011BBD8();
    extern u32 fn_801254B4();
    extern u32 fn_8012A450();
    extern u32 fn_80135024();
    extern u32 fn_8013583C();
    extern u32 fn_80142B24();
    extern u32 fn_801F4C14();
    extern u32 fn_801F75F8();
    extern u32 fn_801FAA58();
    u32 result = 0;

    switch ((u8)kind) {
    case 0:
        break;
    case 1:
        fn_8013583C(arg1, arg3, arg5);
        break;
    case 2:
        fn_80135024(arg1, arg3, arg5);
        break;
    case 3:
        fn_8012A450(arg1, arg3, arg5);
        break;
    case 4:
        fn_80142B24(arg1, arg2, arg3, arg4, arg5);
        break;
    case 5:
        fn_801254B4(arg1, arg2, arg3, arg4, arg5);
        break;
    case 6:
        fn_8011BBD8(arg1, arg2, arg3, arg4, arg5);
        break;
    case 7:
        result = fn_801F4C14(arg1, arg2, arg3, arg4, arg5);
        break;
    case 8:
        fn_801F75F8(arg1, arg2, arg3, arg4, arg5);
        break;
    case 9:
        fn_801FAA58(arg1, arg2, arg3, arg4, arg5);
        break;
    default:
        break;
    }
    return result;
}
#pragma pop
#pragma scheduling off
#endif

/* 0x80135E44 | 0x114 */
extern u32 itemGetStatus();
extern u32 fn_8012A5B0();
extern u32 fn_8012640C();
extern u32 fn_8011BEB4();
extern u32 fn_801F54A4();
extern u32 fightSideGetStatus();
extern u32 fn_801FB1C0();
extern void jumptable_80363AF0();
#if 0
asm void fn_80135E44(void) {
#include "src/game/effect/effect_util_fn_80135E44.inc"
}
#else
u32 fn_80135E44(u32 kind, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    switch ((u8)kind) {
    case 0:
        return 0;
    case 1:
        return fn_80135938((void*)arg1, (u16)arg3);
    case 2:
        return fn_80135028(arg1, arg3, arg4);
    case 3:
        return fn_8012A5B0(arg1, arg3, arg4);
    case 4:
        return itemGetStatus(arg1, arg2, arg3, arg4);
    case 5:
        return fn_8012640C(arg1, arg2, arg3, arg4);
    case 6:
        return fn_8011BEB4(arg1, arg2, arg3, arg4);
    case 7:
        return fn_801F54A4(arg1, arg2, arg3, arg4);
    case 8:
        return fightSideGetStatus(arg1, arg2, arg3, arg4);
    case 9:
        return fn_801FB1C0(arg1, arg2, arg3, arg4);
    default:
        return 0;
    }
}
#endif

/* 0x80135F58 | 0x38 */
extern u32 lbl_80478B90;
extern EffectLinkedStatusRow lbl_80363B78[];
#if 0
asm void fn_80135F58(void) {
#include "src/game/effect/effect_util_fn_80135F58.inc"
}
#else
#pragma scheduling on
u32 fn_80135F58(u32 index, u32 sub) {
    if (index > lbl_80478B90 || sub > 8) {
        return 0;
    }
    return lbl_80363B78[index].links[sub];
}
#pragma scheduling off
#endif

/* 0x80135F90 | 0x2C */
/* Get the linked effect index from the effect status table. */
#pragma scheduling on
u32 fn_80135F90(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].linkedIndex;
}
#pragma scheduling off

/* 0x80135FBC | 0x3C
 * Get a signed status parameter from the effect status table.
 */
#pragma scheduling on
#pragma push
#pragma scheduling on
#pragma fp_contract on
s32 fn_80135FBC(u32 index, u32 subIndex) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;
    s16* effectParams;

    if (index > lbl_80478B88 || subIndex > 2) {
        return 0;
    }
    /* subIndex 2 is valid in the original table and reads the next signed halfword. */
    effectParams = &lbl_80363B18[index].amount;
    return effectParams[subIndex];
}
#pragma pop
#pragma scheduling off

/* 0x80135FF8 | 0x2C */
/* Get the status update mode from the effect status table. */
#pragma scheduling on
u32 fn_80135FF8(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].mode;
}
#pragma scheduling off

/* 0x80136024 | 0x2C */
/* Get the status sub-type from the effect status table. */
#pragma scheduling on
u32 fn_80136024(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].statusSub;
}
#pragma scheduling off

/* 0x80136050 | 0x28 */
/* Get the status kind from the effect status table. */
#pragma scheduling on
u32 fn_80136050(u32 index) {
    extern EffectStatusTableEntry lbl_80363B18[];
    extern u32 lbl_80478B88;

    if (index > lbl_80478B88) {
        return 0;
    }
    return lbl_80363B18[index].statusKind;
}
#pragma scheduling off

/* 0x80136078 | 0xC4 */
#if 0
asm void fn_80136078(void) {
#include "src/game/effect/effect_util_fn_80136078.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80136078(u32 index, void* arg1, void* arg2, s32* out) {
    u32 linkedIndex;
    u32 sub;

    if (out != NULL) {
        _koukaOneExec__FUlPvPvPl(index, arg1, arg2, out);
    } else {
        _koukaOneExec__FUlPvPvPl(index, arg1, arg2, NULL);
    }

    linkedIndex = fn_80135F90(index) & 0xFFFF;
    if (linkedIndex == 0) {
        return;
    }

    for (sub = 0; sub < 8; sub++) {
        if ((fn_80135F58(linkedIndex, sub & 0xFFFF) & 0xFFFF) != 0) {
            if (out != NULL) {
                _koukaOneExec__FUlPvPvPl(index, arg1, arg2, out + ((sub & 0xFFFF) + 1));
            } else {
                _koukaOneExec__FUlPvPvPl(index, arg1, arg2, NULL);
            }
        }
    }
}
#pragma pop
#endif

/* 0x8013613C | 0x22C */
extern u32 fn_801F0134(void* arg, u16 handle);
#if 0
asm void _koukaOneExec__FUlPvPvPl(void) {
#include "src/game/effect/effect_util_fn_8013613C.inc"
}
#else
void _koukaOneExec__FUlPvPvPl(u32 index, void* arg1, void* arg2, s32* out) {
    u32 statusKind;
    u32 statusSub;
    s32 amount;
    s32 divisor;
    u32 mode;
    s32 current;
    u32 extra;
    u16 handle;

    if (index == 0) {
        return;
    }

    statusKind = fn_80136050(index);
    statusSub = fn_80136024(index);
    amount = (s16)fn_80135FBC(index, 0);
    divisor = (s16)fn_80135FBC(index, 1);
    mode = fn_80135FF8(index) & 0xFF;

    current = fn_80135E44(statusKind, (u32)arg1, 0, statusSub, amount & 0xFFFF);
    if (mode == 0 || mode == 2 || mode == 3) {
        if (amount == -1) {
            amount = fn_80135E44(statusKind, (u32)arg1, 0, divisor & 0xFFFF, 0) / 2;
        } else if (amount == -2) {
            amount = (s16)fn_80135E44(statusKind, (u32)arg1, 0, divisor & 0xFFFF, 0);
        } else if (amount < -2 || divisor < -2) {
            return;
        }
    }

    switch (mode) {
    case 0:
        break;
    case 1:
        amount = (current * amount) / divisor;
        break;
    case 2:
        amount = current + amount;
        break;
    case 3:
        amount = current - amount;
        break;
    case 4:
        amount = current + ((current * amount) / divisor);
        break;
    case 5:
        amount = current - ((current * amount) / divisor);
        break;
    default:
        return;
    }

    if (arg2 != NULL) {
        handle = (u16)fn_801F54A4(0, 0, 0x14, 0);
        extra = fn_801F0134(arg2, handle);
    } else {
        extra = 0;
    }

    fn_80135D10(statusKind, (u32)arg1, 0, statusSub, extra, amount);
    if (out != NULL) {
        *out = amount;
    }
}
#endif

/* 0x801364A8 | 0xC6C */
extern void fn_800D37CC(void);
extern void fn_80137780(void);
extern void fn_800E01D0(void);
extern void fn_800E2C04(void);
extern void fn_800EFD3C(void);
extern void fn_800EFD14(void);
extern void fn_8013757C(void);
extern void fn_800EF590(void);
extern void fn_801DB060(void);
extern void fn_8010147C(void);
extern void fn_801013A0(void);
extern void fn_800E4014(void);
extern void fn_8013735C(void);
extern void fn_8013D604(void);
extern void fn_80137114(void);
extern void* memset(void* dst, int val, u32 n);
extern u32 jumptable_80363C70[];
extern void* memcpy(void* dst, const void* src, u32 n);
extern f64 lbl_8047D128;
extern f32 lbl_8047D118;
extern u8 lbl_80314AE8[];
extern u8 lbl_80314638[];
extern f32 lbl_8047D11C;
extern f32 lbl_8047D120;
#if 0
asm void fn_801364A8(void) {
#include "src/game/effect/effect_util_fn_801364A8.inc"
}
#else
#pragma push
#pragma peephole off
void fn_801364A8(void) {
    extern u8 lbl_80314638[];
    extern u8 lbl_80314AE8[];
    extern f32 lbl_8047D118;
    extern f32 lbl_8047D11C;
    extern f32 lbl_8047D120;
    extern f64 lbl_8047D128;
    extern void fn_800D37CC();
    extern void fn_800E01D0();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800E4014();
    extern void fn_800EF590();
    extern void fn_800EFD14();
    extern void fn_800EFD3C();
    extern void fn_800F9318();
    extern void fn_801013A0();
    extern void fn_8010147C();
    extern void fn_80137114();
    extern void fn_8013735C();
    extern void fn_8013757C();
    extern void fn_80137780();
    extern void fn_8013D604();
    extern void fn_801DB060();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    *(f64*)(sp + 0x60) = f31;
    *(f64*)(sp + 0x50) = f30;
    r29 = r4;
    r28 = r3;
    r30 = *(u32*)((u8*)r29 + 0x4);
    r27 = r29;
    r4 = 0x0;
    r5 = 0xd8;
    r29 = r29 + 0xc;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)((u8*)r27 + 0x0);
    *(u32*)((u8*)r28 + 0x0) = r0;
    fn_800D37CC();
    r4 = (0x4330 << 16);
    f3 = lbl_8047D128;
    f0 = lbl_8047D118;
    f1 = *(f64*)(sp + 0x10);
    *(u32*)(sp + 0x1C) = r0;
    f2 = f1 - f3;
    f1 = *(f64*)(sp + 0x18);
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x20) = f0;
    r0 = *(u32*)(sp + 0x24);
    *(u32*)((u8*)r28 + 0x4) = r0;
    r0 = *(u32*)((u8*)r27 + 0x0);
    do {
    do {
        if (r0 > (u32)0xc) break;
        r3 = (u32)jumptable_80363C70;
        r0 = r0 << 2;
        r3 = (u32)jumptable_80363C70;
        r0 = *(u32*)(r3 + r0);
        ctr_fn = (void(*)(void))r0;
        /* indirect jump via ctr */;
        r4 = r29;
        r5 = r30;
        r3 = r28 + 0x8;
        fn_80137780();
        r29 = r3;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x60;
        memset((void*)r3, (int)r4, (u32)r5);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        f3 = lbl_8047D128;
        r4 = r29;
        r3 = r28 + 0x10;
        f0 = lbl_8047D118;
        f1 = *(f64*)(sp + 0x20);
        *(u32*)(sp + 0x1C) = r0;
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x18);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x10) = f0;
        r0 = *(u32*)(sp + 0x14);
        *(u16*)((u8*)r28 + 0x52) = r0;
        r0 = *(u32*)((u8*)r29 + 0x3C);
        *(u16*)((u8*)r28 + 0x4C) = r0;
        r0 = *(u32*)((u8*)r29 + 0x40);
        *(u16*)((u8*)r28 + 0x4E) = r0;
        f0 = *(f32*)((u8*)r29 + 0x38);
        *(f32*)((u8*)r28 + 0x48) = f0;
        r0 = *(u32*)((u8*)r29 + 0x44);
        r0 = (u32)r0 >> 24;
        *(u8*)((u8*)r28 + 0x67) = r0;
        r0 = *(u32*)((u8*)r29 + 0x44);
        *(u8*)((u8*)r28 + 0x66) = r0;
        r0 = *(u32*)((u8*)r29 + 0x44);
        *(u8*)((u8*)r28 + 0x65) = r0;
        r0 = *(u32*)((u8*)r29 + 0x44);
        *(u8*)((u8*)r28 + 0x64) = r0;
        r0 = *(u32*)((u8*)r29 + 0x48);
        *(u32*)((u8*)r28 + 0x58) = r0;
        f0 = *(f32*)((u8*)r29 + 0x34);
        *(f32*)((u8*)r28 + 0x44) = f0;
        f0 = *(f32*)((u8*)r29 + 0x30);
        *(f32*)((u8*)r28 + 0x40) = f0;
        fn_800E01D0();
        r3 = r28 + 0x34;
        r4 = r29 + 0x24;
        fn_800E01D0();
        r3 = r28 + 0x1c;
        r4 = r29 + 0xc;
        fn_800E01D0();
        r3 = r28 + 0x28;
        r4 = r29 + 0x18;
        fn_800E01D0();
        r3 = *(u32*)((u8*)r29 + 0x4C);
        r0 = r29 + 0x73;
        /* clrrwi r27, r0, 5 */;
        r4 = 0x20;
        r0 = r3 + 0x1f;
        /* clrrwi r3, r0, 5 */;
        fn_800E2C04();
        r0 = r3 & 0xFFFF;
        r31 = r3;
        if (r0 != (u32)0xc) {
            fn_800E27B0();
            r5 = *(u32*)((u8*)r29 + 0x4C);
            r30 = r3;
            r4 = r27;
            r0 = r5 + 0x1f;
            /* clrrwi r5, r0, 5 */;
            memcpy((void*)r3, (const void*)r4, (u32)r5);
            r3 = r30;
            fn_800EFD3C();
            *(u32*)((u8*)r28 + 0x60) = r3;
            r4 = r31;
            r3 = *(u32*)((u8*)r28 + 0x60);
            fn_800EFD14();
        } else {

            r0 = 0x0;
            *(u32*)((u8*)r28 + 0x60) = r0;
        }
        r4 = *(u32*)((u8*)r29 + 0x4C);
        r3 = (u32)lbl_80314AE8;
        r0 = (u32)lbl_80314AE8;
        r3 = r4 + 0x1f;
        *(u32*)((u8*)r28 + 0x5C) = r0;
        /* clrrwi r0, r3, 5 */;
        r27 = r27 + r0;
        r29 = r27;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x24;
        memset((void*)r3, (int)r4, (u32)r5);
        f0 = *(f32*)((u8*)r29 + 0x4);
        r4 = (0x4330 << 16);
        r3 = (u32)lbl_80314638;
        r0 = (u32)lbl_80314638;
        f1 = lbl_8047D128;
        *(f32*)((u8*)r28 + 0x18) = f0;
        f0 = *(f32*)((u8*)r29 + 0x8);
        *(f32*)((u8*)r28 + 0x1C) = f0;
        r3 = *(u32*)((u8*)r29 + 0xC);
        f0 = *(f64*)(sp + 0x20);
        f0 = f0 - f1;
        *(f32*)((u8*)r28 + 0x20) = f0;
        r3 = *(u32*)((u8*)r29 + 0x10);
        f0 = *(f64*)(sp + 0x18);
        f0 = f0 - f1;
        *(f32*)((u8*)r28 + 0x24) = f0;
        f0 = *(f32*)((u8*)r29 + 0x14);
        *(f32*)((u8*)r28 + 0x28) = f0;
        r3 = *(u32*)((u8*)r29 + 0x0);
        r3 = (u32)r3 >> 24;
        *(u8*)((u8*)r28 + 0xB) = r3;
        r3 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0xA) = r3;
        r3 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x9) = r3;
        r3 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x8) = r3;
        *(u32*)((u8*)r28 + 0xC) = r0;
        fn_800D37CC();
        r4 = (0x4330 << 16);
        f3 = lbl_8047D128;
        r29 = r29 + 0x1c;
        f0 = lbl_8047D118;
        f1 = *(f64*)(sp + 0x10);
        *(u32*)(sp + 0x2C) = r0;
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x30) = f0;
        r0 = *(u32*)(sp + 0x34);
        *(u16*)((u8*)r28 + 0x12) = r0;
        break;
        r4 = r29;
        r5 = r30;
        r3 = r28 + 0x8;
        fn_8013757C();
        r29 = r3;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x2c;
        memset((void*)r3, (int)r4, (u32)r5);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        r0 = r29 + 0x37;
        f3 = lbl_8047D128;
        /* clrrwi r31, r0, 5 */;
        f0 = lbl_8047D118;
        r4 = 0x20;
        f1 = *(f64*)(sp + 0x30);
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r0 = *(u32*)(sp + 0x24);
        *(u16*)((u8*)r28 + 0x28) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        r0 = (u32)r0 >> 24;
        *(u8*)((u8*)r28 + 0x23) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x22) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x21) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x20) = r0;
        r0 = *(u32*)((u8*)r29 + 0x4);
        *(u16*)((u8*)r28 + 0x2A) = r0;
        r0 = *(u32*)((u8*)r29 + 0x8);
        *(u16*)((u8*)r28 + 0x30) = r0;
        r0 = *(u32*)((u8*)r29 + 0xC);
        *(u16*)((u8*)r28 + 0x32) = r0;
        r3 = *(u32*)((u8*)r29 + 0x10);
        r0 = r3 + 0x1f;
        /* clrrwi r3, r0, 5 */;
        fn_800E2C04();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if (r0 != (u32)0xc) {
            fn_800E27B0();
            r5 = *(u32*)((u8*)r29 + 0x10);
            r27 = r3;
            r4 = r31;
            r0 = r5 + 0x1f;
            /* clrrwi r5, r0, 5 */;
            memcpy((void*)r3, (const void*)r4, (u32)r5);
            r3 = r27;
            fn_800EFD3C();
            *(u32*)((u8*)r28 + 0x1C) = r3;
            r4 = r30;
            r3 = *(u32*)((u8*)r28 + 0x1C);
            fn_800EFD14();
        } else {

            r0 = 0x0;
            *(u32*)((u8*)r28 + 0x1C) = r0;
        }
        r4 = *(u32*)((u8*)r29 + 0x10);
        r3 = *(u32*)((u8*)r28 + 0x1C);
        r0 = r4 + 0x1f;
        /* clrrwi r0, r0, 5 */;
        r31 = r31 + r0;
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            r5 = 0x0;
            fn_800EF590();
        }
        r3 = (u32)lbl_80314AE8;
        r29 = r31;
        r0 = (u32)lbl_80314AE8;
        *(u32*)((u8*)r28 + 0x18) = r0;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x4c;
        memset((void*)r3, (int)r4, (u32)r5);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        r3 = r29 + 0x5b;
        f3 = lbl_8047D128;
        /* clrrwi r27, r3, 5 */;
        f0 = lbl_8047D118;
        r0 = 0x4e20;
        f1 = *(f64*)(sp + 0x30);
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r3 = *(u32*)(sp + 0x24);
        *(u16*)((u8*)r28 + 0x46) = r3;
        f0 = *(f32*)((u8*)r29 + 0x0);
        *(f32*)((u8*)r28 + 0x18) = f0;
        f0 = *(f32*)((u8*)r29 + 0x4);
        *(f32*)((u8*)r28 + 0x1C) = f0;
        f0 = *(f32*)((u8*)r29 + 0x8);
        *(f32*)((u8*)r28 + 0x20) = f0;
        f0 = *(f32*)((u8*)r29 + 0xC);
        *(f32*)((u8*)r28 + 0x24) = f0;
        f0 = *(f32*)((u8*)r29 + 0x10);
        *(f32*)((u8*)r28 + 0x28) = f0;
        f0 = *(f32*)((u8*)r29 + 0x14);
        *(f32*)((u8*)r28 + 0x2C) = f0;
        f0 = *(f32*)((u8*)r29 + 0x18);
        *(f32*)((u8*)r28 + 0x30) = f0;
        f0 = *(f32*)((u8*)r29 + 0x1C);
        *(f32*)((u8*)r28 + 0x34) = f0;
        f0 = *(f32*)((u8*)r29 + 0x20);
        *(f32*)((u8*)r28 + 0x38) = f0;
        f0 = *(f32*)((u8*)r29 + 0x24);
        *(f32*)((u8*)r28 + 0x3C) = f0;
        r3 = *(u32*)((u8*)r29 + 0x28);
        *(u16*)((u8*)r28 + 0x40) = r3;
        r3 = *(u32*)((u8*)r29 + 0x2C);
        *(u16*)((u8*)r28 + 0x42) = r3;
        r3 = *(u32*)((u8*)r29 + 0x30);
        *(u16*)((u8*)r28 + 0x52) = r3;
        *(u16*)((u8*)r28 + 0x48) = r0;
        fn_801DB060();
        *(u16*)((u8*)r28 + 0x4A) = r3;
        fn_801DB060();
        *(u16*)((u8*)r28 + 0x4C) = r3;
        r3 = r27;
        r5 = 0x4e20;
        r4 = *(u32*)((u8*)r29 + 0x34);
        r6 = *(u16*)((u8*)r28 + 0x4A);
        fn_8010147C();
        r4 = *(u16*)((u8*)r28 + 0x4A);
        r3 = 0x4e20;
        fn_800F9318();
        r6 = *(u16*)((u8*)r28 + 0x4C);
        r4 = 0x4e20;
        r5 = 0x0;
        fn_801013A0();
        r4 = *(u16*)((u8*)r28 + 0x4C);
        r3 = 0x4e20;
        fn_800F9318();
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            fn_800E4014();
        }
        r3 = *(u32*)((u8*)r29 + 0x34);
        r0 = r3 + 0x1f;
        /* clrrwi r0, r0, 5 */;
        r0 = r27 + r0;
        r29 = r0;
        break;
        r4 = r29;
        r5 = r30;
        r3 = r28 + 0x8;
        fn_8013735C();
        r29 = r3;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0xd0;
        memset((void*)r3, (int)r4, (u32)r5);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        r3 = r29 + 0x53;
        f3 = lbl_8047D128;
        /* clrrwi r31, r3, 5 */;
        f0 = lbl_8047D118;
        r0 = 0x4e20;
        f1 = *(f64*)(sp + 0x30);
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r3 = *(u32*)(sp + 0x24);
        *(u16*)((u8*)r28 + 0xD6) = r3;
        f0 = *(f32*)((u8*)r29 + 0x0);
        *(f32*)((u8*)r28 + 0x2C) = f0;
        f0 = *(f32*)((u8*)r29 + 0x4);
        *(f32*)((u8*)r28 + 0x30) = f0;
        f0 = *(f32*)((u8*)r29 + 0x8);
        *(f32*)((u8*)r28 + 0x34) = f0;
        r3 = *(u32*)((u8*)r29 + 0xC);
        r3 = (s32)r3 >> 24;
        *(u8*)((u8*)r28 + 0x47) = r3;
        r3 = *(u32*)((u8*)r29 + 0xC);
        *(u8*)((u8*)r28 + 0x46) = r3;
        r3 = *(u32*)((u8*)r29 + 0xC);
        *(u8*)((u8*)r28 + 0x45) = r3;
        r3 = *(u32*)((u8*)r29 + 0xC);
        *(u8*)((u8*)r28 + 0x44) = r3;
        f0 = *(f32*)((u8*)r29 + 0x10);
        *(f32*)((u8*)r28 + 0x48) = f0;
        f0 = *(f32*)((u8*)r29 + 0x14);
        *(f32*)((u8*)r28 + 0x4C) = f0;
        f0 = *(f32*)((u8*)r29 + 0x18);
        *(f32*)((u8*)r28 + 0x50) = f0;
        r3 = *(u32*)((u8*)r29 + 0x1C);
        *(u16*)((u8*)r28 + 0x54) = r3;
        r3 = *(u32*)((u8*)r29 + 0x20);
        *(u16*)((u8*)r28 + 0x56) = r3;
        f0 = *(f32*)((u8*)r29 + 0x24);
        *(f32*)((u8*)r28 + 0xCC) = f0;
        f0 = *(f32*)((u8*)r29 + 0x28);
        *(f32*)((u8*)r28 + 0xD0) = f0;
        *(u32*)((u8*)r28 + 0x58) = r0;
        fn_801DB060();
        *(u32*)((u8*)r28 + 0x60) = r3;
        fn_801DB060();
        *(u32*)((u8*)r28 + 0x5C) = r3;
        r3 = r31;
        r5 = 0x4e20;
        r4 = *(u32*)((u8*)r29 + 0x2C);
        r6 = *(u32*)((u8*)r28 + 0x60);
        fn_8010147C();
        r4 = *(u32*)((u8*)r28 + 0x60);
        r3 = 0x4e20;
        fn_800F9318();
        r6 = *(u32*)((u8*)r28 + 0x5C);
        r4 = 0x4e20;
        r5 = 0x0;
        fn_801013A0();
        r4 = *(u32*)((u8*)r28 + 0x5C);
        r3 = 0x4e20;
        fn_800F9318();
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            fn_800E4014();
        }
        r3 = *(u32*)((u8*)r29 + 0x2C);
        r0 = r3 + 0x1f;
        /* clrrwi r0, r0, 5 */;
        r0 = r31 + r0;
        r29 = r0;
        break;
        r27 = *(u32*)((u8*)r29 + 0x8);
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x18;
        memset((void*)r3, (int)r4, (u32)r5);
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u32*)((u8*)r28 + 0xC) = r0;
        r0 = *(u32*)((u8*)r29 + 0x4);
        *(u32*)((u8*)r28 + 0x10) = r0;
        r0 = *(u32*)((u8*)r29 + 0xC);
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 >= (s32)0x2) break;
            if ((s32)r0 < (s32)0x1) {
                break;
            }
            f30 = lbl_8047D128;
            r31 = r29 + 0x10;
            f31 = lbl_8047D118;
            r30 = 0x0;
            r29 = (0x4330 << 16);
            while ((s32)r30 < (s32)r27) {

                fn_800D37CC();
                r0 = *(u32*)((u8*)r31 + 0x8);
                r3 = r28 + 0x8;
                f1 = *(f32*)((u8*)r31 + 0x0);
                f2 = *(f32*)((u8*)r31 + 0x4);
                f0 = *(f64*)(sp + 0x30);
                *(u32*)(sp + 0x2C) = r0;
                f3 = f0 - f30;
                f0 = *(f64*)(sp + 0x28);
                f0 = f0 - f30;
                f0 = f0 * f3;
                f0 = f0 / f31;
                f0 = (f64)(s32)f0;
                *(f64*)(sp + 0x20) = f0;
                r4 = *(u32*)(sp + 0x24);
                fn_8013D604();
                r30 = r30 + 0x1;
                r31 = r31 + 0x10;

            }
            break;
            }
        r0 = *(u32*)((u8*)r29 + 0x8);
        *(u32*)(sp + 0x8) = r0;
        f30 = *(f32*)(sp + 0x8);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        r3 = (0x5555 << 16);
        f5 = lbl_8047D128;
        r0 = r3 + 0x5556;
        f0 = lbl_8047D118;
        f2 = f30;
        f1 = *(f64*)(sp + 0x30);
        r3 = r28 + 0x8;
        f4 = f1 - f5;
        f1 = lbl_8047D11C;
        f3 = *(f64*)(sp + 0x28);
        f3 = f3 - f5;
        f3 = f3 * f4;
        f0 = f3 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r4 = *(u32*)(sp + 0x24);
        r4 = (s32)((s64)r0 * (s64)r4 >> 32);
        r0 = (u32)r4 >> 31;
        r27 = r4 + r0;
        r4 = r27;
        fn_8013D604();
        f1 = f30;
        r4 = r27;
        f2 = f30;
        r3 = r28 + 0x8;
        fn_8013D604();
        f1 = lbl_8047D11C;
        r4 = r27;
        r3 = r28 + 0x8;
        f2 = f1;
        fn_8013D604();
    } while (0);
    do {
        r29 = r31;
        break;
        r4 = r29;
        r5 = r30;
        r3 = r28 + 0x8;
        fn_80137114();
        r29 = r3;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x34;
        memset((void*)r3, (int)r4, (u32)r5);
        fn_800D37CC();
        r4 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        f3 = lbl_8047D128;
        r0 = 0x0;
        f0 = lbl_8047D118;
        f1 = *(f64*)(sp + 0x30);
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r3 = *(u32*)(sp + 0x24);
        *(u16*)((u8*)r28 + 0x3A) = r3;
        f0 = *(f32*)((u8*)r29 + 0x18);
        *(f32*)((u8*)r28 + 0x24) = f0;
        f0 = *(f32*)((u8*)r29 + 0x1C);
        *(f32*)((u8*)r28 + 0x28) = f0;
        f0 = *(f32*)((u8*)r29 + 0x20);
        *(f32*)((u8*)r28 + 0x2C) = f0;
        r3 = *(u32*)((u8*)r29 + 0x4);
        *(u8*)((u8*)r28 + 0x20) = r3;
        *(u8*)((u8*)r28 + 0x21) = r0;
        r0 = *(u32*)((u8*)r29 + 0xC);
        if ((s32)r0 != (s32)0x0) {
            r0 = *(u8*)((u8*)r28 + 0x21);
            r0 = r0 | 0x2;
            *(u8*)((u8*)r28 + 0x21) = r0;
        }
        r0 = *(u32*)((u8*)r29 + 0x14);
        if ((s32)r0 != (s32)0x0) {
            r0 = *(u8*)((u8*)r28 + 0x21);
            r0 = r0 | 0x8;
            *(u8*)((u8*)r28 + 0x21) = r0;
        }
        r0 = *(u32*)((u8*)r29 + 0x8);
        if ((s32)r0 != (s32)0x0) {
            r0 = *(u8*)((u8*)r28 + 0x21);
            r0 = r0 | 0x4;
            *(u8*)((u8*)r28 + 0x21) = r0;
        }
        r0 = *(u32*)((u8*)r29 + 0x10);
        if ((s32)r0 != (s32)0x0) {
            r0 = *(u8*)((u8*)r28 + 0x21);
            r0 = r0 | 0x1;
            *(u8*)((u8*)r28 + 0x21) = r0;
        }
        r0 = *(u32*)((u8*)r29 + 0x0);
        r0 = (s32)r0 >> 24;
        *(u8*)((u8*)r28 + 0x13) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x12) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u8*)((u8*)r28 + 0x11) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        r29 = r29 + 0x28;
        *(u8*)((u8*)r28 + 0x10) = r0;
        break;
        r3 = r28 + 0x8;
        r4 = 0x0;
        r5 = 0x24;
        memset((void*)r3, (int)r4, (u32)r5);
        r0 = *(u32*)((u8*)r29 + 0x8);
        *(u16*)((u8*)r28 + 0x24) = r0;
        r0 = *(u32*)((u8*)r29 + 0x10);
        if ((s32)r0 != (s32)0x1) {
            if ((s32)r0 >= (s32)0x1) break;
            if ((s32)r0 < (s32)0x0) {
                break;
            }
            r0 = 0x0;
            *(u32*)((u8*)r28 + 0x20) = r0;
            break;
        }
        r0 = 0x1;
        *(u32*)((u8*)r28 + 0x20) = r0;
    } while (0);
    do {
        r4 = *(u32*)((u8*)r29 + 0xC);
        r3 = r29 + 0x37;
        r0 = 0x4e20;
        *(u16*)((u8*)r28 + 0x26) = r4;
        /* clrrwi r27, r3, 5 */;
        *(u32*)((u8*)r28 + 0x8) = r0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        if ((s32)r0 != (s32)0x0) {
            fn_801DB060();
            *(u32*)((u8*)r28 + 0xC) = r3;
            fn_801DB060();
            *(u32*)((u8*)r28 + 0x10) = r3;
            r3 = r27;
            r5 = 0x4e20;
            r4 = *(u32*)((u8*)r29 + 0x0);
            r6 = *(u32*)((u8*)r28 + 0xC);
            fn_8010147C();
            r4 = *(u32*)((u8*)r28 + 0xC);
            r3 = 0x4e20;
            fn_800F9318();
            r6 = *(u32*)((u8*)r28 + 0x10);
            r4 = 0x4e20;
            r5 = 0x0;
            fn_801013A0();
            r4 = *(u32*)((u8*)r28 + 0x10);
            r3 = 0x4e20;
            fn_800F9318();
            if (r3 != (u32)0x0) {
                r4 = 0x0;
                fn_800E4014();
            }
            r3 = *(u32*)((u8*)r29 + 0x0);
            r0 = r3 + 0x1f;
            /* clrrwi r0, r0, 5 */;
            r27 = r27 + r0;
        } else {

            r0 = 0x0;
            *(u32*)((u8*)r28 + 0xC) = r0;
            *(u32*)((u8*)r28 + 0x10) = r0;
        }
        r0 = 0x0;
        r29 = r27;
        *(u32*)((u8*)r28 + 0x14) = r0;
        break;
        r3 = r28 + 0x8;
        r27 = 0x0;
        r4 = 0x0;
        r5 = 0xb4;
        memset((void*)r3, (int)r4, (u32)r5);
        r0 = *(u32*)((u8*)r29 + 0x14);
        if ((s32)r0 != (s32)0x2) {
            if ((s32)r0 >= (s32)0x2) break;
            if ((s32)r0 < (s32)0x1) {
                break;
            }
            f0 = lbl_8047D120;
            r27 = -0x4;
            *(f32*)((u8*)r28 + 0x24) = f0;
            break;
        }
        f0 = *(f32*)((u8*)r29 + 0x18);
        *(f32*)((u8*)r28 + 0x24) = f0;
    } while (0);
        fn_800D37CC();
        r5 = (0x4330 << 16);
        *(u32*)(sp + 0x34) = r0;
        r4 = r27 + r29;
        f3 = lbl_8047D128;
        r0 = r4 + 0x3b;
        f0 = lbl_8047D118;
        /* clrrwi r31, r0, 5 */;
        f1 = *(f64*)(sp + 0x30);
        r4 = 0x20;
        f2 = f1 - f3;
        f1 = *(f64*)(sp + 0x28);
        f1 = f1 - f3;
        f1 = f1 * f2;
        f0 = f1 / f0;
        f0 = (f64)(s32)f0;
        *(f64*)(sp + 0x20) = f0;
        r0 = *(u32*)(sp + 0x24);
        *(u16*)((u8*)r28 + 0xBA) = r0;
        f0 = *(f32*)((u8*)r29 + 0x8);
        *(f32*)((u8*)r28 + 0x20) = f0;
        f0 = *(f32*)((u8*)r28 + 0x20);
        *(f32*)((u8*)r28 + 0x1C) = f0;
        f0 = *(f32*)((u8*)r29 + 0xC);
        *(f32*)((u8*)r28 + 0x2C) = f0;
        r0 = *(u32*)((u8*)r29 + 0x0);
        *(u16*)((u8*)r28 + 0x30) = r0;
        r0 = *(u32*)((u8*)r29 + 0x4);
        *(u16*)((u8*)r28 + 0x32) = r0;
        r3 = *(u32*)((u8*)r29 + 0x10);
        r0 = r3 + 0x1f;
        /* clrrwi r3, r0, 5 */;
        fn_800E2C04();
        r0 = r3 & 0xFFFF;
        r30 = r3;
        if ((s32)r0 != (s32)0x1) {
            fn_800E27B0();
            r5 = *(u32*)((u8*)r29 + 0x10);
            r27 = r3;
            r4 = r31;
            r0 = r5 + 0x1f;
            /* clrrwi r5, r0, 5 */;
            memcpy((void*)r3, (const void*)r4, (u32)r5);
            r3 = r27;
            fn_800EFD3C();
            *(u32*)((u8*)r28 + 0xC) = r3;
            r4 = r30;
            r3 = *(u32*)((u8*)r28 + 0xC);
            fn_800EFD14();
        } else {

            r0 = 0x0;
            *(u32*)((u8*)r28 + 0xC) = r0;
        }
        r3 = *(u32*)((u8*)r29 + 0x10);
        r0 = r3 + 0x1f;
        /* clrrwi r0, r0, 5 */;
        r31 = r31 + r0;
        r29 = r31;
    } while (0);
    r3 = r29;
    f31 = *(f64*)(sp + 0x60);
    f30 = *(f64*)(sp + 0x50);
    return;
}
#pragma pop
#endif
extern s32 fn_801666BC(u16);
extern u16 lbl_8047AEA4;
#if 0
asm void fn_80131588(void) {
#include "src/game/effect/effect_util_fn_80131588.inc"
}
#else
#pragma peephole off
#pragma scheduling on
u32 fn_80131588(void* obj) {
    u8* p = (u8*)obj;
    if (p[0x01] == 0) {
        u16 handle = lbl_8047AEA4;
        if (handle == 0) {
            return 0;
        }
        if (fn_801666BC(handle) == 2) {
            *(u32*)(p + 0x30) = *(u32*)(p + 0x30) - 3;
        }
    }
    return 1;
}
#pragma scheduling off
#pragma peephole on
#endif
extern void fn_80165A20(u16, u32, u32);
extern u16 lbl_8047AEA4;
#if 0
asm void fn_801315EC(void) {
#include "src/game/effect/effect_util_fn_801315EC.inc"
}
#else
#pragma peephole off
u32 fn_801315EC(void* obj) {
    if (*(u8*)((u8*)obj + 0x01) == 0) {
        u16 handle = lbl_8047AEA4;
        if (handle != 0) {
            fn_80165A20(handle, 0, 0xFF);
        }
    }
    return 0;
}
#pragma peephole on
#endif
extern void GSmsgAdjustAlign(void);
#if 0
asm void fn_80131714(void) {
#include "src/game/effect/effect_util_fn_80131714.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
#pragma scheduling on
s32 fn_80131714(void* obj) {
    extern void GSmsgAdjustAlign(void*);
    u8* p = (u8*)obj;
    u8* stream;
    if (p[0x01] != 0) {
        stream = *(u8**)(p + 0x30);
        p[0x4a] = stream[0];
        GSmsgAdjustAlign(obj);
    }
    stream = *(u8**)(p + 0x30);
    *(u32*)(p + 0x30) = (u32)(stream + 1);
    return 0;
}
#pragma scheduling off
#pragma pop
#endif
#if 0
asm void fn_80132454(void) {
#include "src/game/effect/effect_util_fn_80132454.inc"
}
#else
#pragma optimization_level 4
s32 fn_80132454(void* obj) {
    u8* p = (u8*)obj;
    u8* stream;
    s16 counter;
    if (p[0x01] != 0) {
        stream = *(u8**)(p + 0x30);
        *(u32*)(p + 0x30) = (u32)(stream + 1);
        return 0;
    }
    counter = *(s16*)(p + 0x48);
    if (counter == 0) {
        stream = *(u8**)(p + 0x30);
        counter = (s16)((s16)stream[0] + 1);
        *(s16*)(p + 0x48) = counter;
    }
    counter = (s16)(counter - 1);
    *(s16*)(p + 0x48) = counter;
    if (counter <= 0) {
        *(s16*)(p + 0x48) = 0;
        stream = *(u8**)(p + 0x30);
        *(u32*)(p + 0x30) = (u32)(stream + 1);
        return 0;
    }
    stream = *(u8**)(p + 0x30);
    *(u32*)(p + 0x30) = (u32)(stream - 3);
    return 1;
}
#endif
extern void fn_800FA160(void);
#if 0
asm void fn_80132570(void) {
#include "src/game/effect/effect_util_fn_80132570.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
#pragma scheduling on
s32 fn_80132570(void* obj) {
    extern void fn_800FA160(void*);
    u8* p = (u8*)obj;
    u8* stream;
    if (p[0x01] != 0) {
        stream = *(u8**)(p + 0x30);
        *(u32*)(p + 0x24) = *(u32*)stream;
        fn_800FA160(obj);
    }
    stream = *(u8**)(p + 0x30);
    *(u32*)(p + 0x30) = (u32)(stream + 4);
    return 0;
}
#pragma scheduling off
#pragma pop
#endif
extern void fn_800FAA98(void);
#if 0
asm void fn_8013264C(void) {
#include "src/game/effect/effect_util_fn_8013264C.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
#pragma scheduling on
s32 fn_8013264C(void* obj) {
    extern void fn_800FAA98(void*);
    u8* p = (u8*)obj;
    if (p[0x01] != 0) {
        fn_800FAA98(obj);
    }
    p[0x4b] = 1;
    return 0;
}
#pragma scheduling off
#pragma pop
#endif
extern f64 lbl_8047D0E0;
#if 0
asm void fn_801327E0(void) {
#include "src/game/effect/effect_util_fn_801327E0.inc"
}
#else
u32 fn_801327E0(void* obj) {
    u8* p = (u8*)obj;
    *(f32*)(p + 0x0C) = *(f32*)(p + 0x04);
    *(f32*)(p + 0x10) += *(f32*)(p + 0x64) * (f32)((s32)(u8)p[0x23] + (s32)(s8)p[0x42]);
    return 0;
}
#endif
#if 0
asm void fn_80132F7C(void) {
#include "src/game/effect/effect_util_fn_80132F7C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_80132F7C(void) {
    if (fn_80102620(0xab) & 0xFF) {
        fn_80102510(0xab);
    } else {
        fn_801026A4(0xab, 0, 0, 0, 0, 0);
    }
    return 0;
}
#pragma scheduling off
#endif
extern void fn_801E1810(void);
extern void _threadSwitch(void);
extern u8 fn_801E1874(void);
extern s32 fn_8010264C(u32, u32);
extern void fn_800C8520(u8*, u8*, ...);
extern void fn_801E189C(u8*, u32);
extern u8 lbl_80272AA8[];
extern u8 lbl_8047D0E8[4];
#if 0
asm void fn_80132FD8(void) {
#include "src/game/effect/effect_util_fn_80132FD8.inc"
}
#else
#pragma scheduling on
u32 fn_80132FD8(void) {
    u8 buf[0x18];
    s32 id;
    while ((u32)(fn_801E1874() & 0xFF) == 1) {
        fn_801E1810();
        _threadSwitch();
    }
    id = fn_8010264C(2, 1);
    if (id != -1) {
        fn_800C8520(buf, lbl_80272AA8, lbl_8047D0E8, id);
        fn_801E189C(buf, 0);
    }
    return 0;
}
#pragma scheduling off
#endif
#if 0
asm void fn_8013327C(void) {
#include "src/game/effect/effect_util_fn_8013327C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_8013327C(void) {
    if ((u32)(fn_800E0E14(1, 0) & 0xFF) == 1) {
        fn_800DD970(lbl_80272AE0);
    } else {
        fn_800DD970(lbl_80272AF0);
    }
    return 0;
}
#pragma scheduling off
#endif
extern void fn_801D1CC4(void);
extern void fn_801D1D58(void);
extern void fn_801D268C(void);
#if 0
asm void fn_8013334C(void) {
#include "src/game/effect/effect_util_fn_8013334C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_8013334C(void) {
    extern s32 fn_8010264C(u32, u32);
    extern s32 fn_801D1CC4(s32);
    extern void fn_801D1D58(s32);
    s32 slot;
    while ((slot = fn_8010264C(2, 1)) != -1) {
        if (fn_801D1CC4(slot) == 0) {
            fn_801D1D58(slot);
            fn_801D268C();
        }
    }
    return 0;
}
#pragma scheduling off
#endif
#if 0
asm void fn_80133510(void) {
#include "src/game/effect/effect_util_fn_80133510.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_80133510(void) {
    if (fn_80102620(0x7) & 0xFF) {
        fn_80102510(0x7);
    } else {
        fn_801026A4(0x7, 0, 0, 0, 0, 0);
    }
    return 0;
}
#pragma scheduling off
#endif
extern void fn_8012F11C(void);
extern void fn_8012F150(void);
extern void fn_8012F1FC(void);
#if 0
asm void fn_8013356C(void) {
#include "src/game/effect/effect_util_fn_8013356C.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_8013356C(u32 arg1, s32 arg2) {
    extern u32 fn_8012F11C(s32);
    extern void fn_8012F150(s32);
    extern void fn_8012F1FC(s32);
    s32 r31 = (arg2 == 0) ? 1 : -1;
    if (r31 >= 0) {
        if (fn_8012F11C(r31) & 0xFF) {
            fn_8012F150(r31);
        } else {
            fn_8012F1FC(r31);
        }
    }
    return 0;
}
#pragma scheduling off
#endif
#if 0
asm void fn_801335D4(void) {
#include "src/game/effect/effect_util_fn_801335D4.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
u32 fn_801335D4(void) {
    if (fn_80102620(0x4) & 0xFF) {
        fn_80102510(0x4);
    } else {
        fn_801026A4(0x4, 0, 0, 0, 0, 0);
    }
    return 0;
}
#pragma scheduling off
#endif
extern u32 lbl_80478F8C;
#if 0
asm u8 fn_80133BE4(void) {
#include "src/game/effect/effect_util_fn_80133BE4.inc"
}
#else
u32 fn_80133BE4(void* obj, s32 offset) {
    EffectUtilEntry* result;
    u32 ret;
    s32 index = fn_80133E6C(obj, offset);
    {
        EffectUtilEntryFunc fp = (EffectUtilEntryFunc)lbl_80478F8C;
        if (fp == NULL) {
            result = NULL;
        } else {
            result = fp(index);
        }
    }
    if (result == NULL) {
        ret = 1;
    } else {
        ret = (result->flags >> 7) & 1;
    }
    return (u8)ret;
}
#endif
extern u32 lbl_80478F8C;
#if 0
asm void fn_80133E1C(void) {
#include "src/game/effect/effect_util_fn_80133E1C.inc"
}
#else
#pragma peephole off
u32 fn_80133E1C(void* obj, s32 offset) {
    EffectUtilEntry* result;
    s32 index = fn_80133E6C(obj, offset);
    {
        EffectUtilEntryFunc fp = (EffectUtilEntryFunc)lbl_80478F8C;
        if (fp == NULL) {
            result = NULL;
        } else {
            result = fp(index);
        }
    }
    if (result == NULL) {
        return 0;
    }
    return result->value;
}
#pragma peephole on
#endif
#if 0
asm void fn_80134274(void) {
#include "src/game/effect/effect_util_fn_80134274.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma scheduling on
u32 fn_80134274(void) {
    int new_var;
    extern u32 fn_800057A8(void);
    s32 val = (s32)fn_800057A8();
    if (val == 1) goto _ret2;
    if (val >= 1) goto _chk3;
    new_var = 2;
    goto _ret2;
_chk3:
    if (val >= 3) goto _ret2;
    return 0x115;
_ret2:
    return new_var;
}
#pragma pop
#endif
extern u32 lbl_80478F8C;
#if 0
asm void fn_801342B8(void) {
#include "src/game/effect/effect_util_fn_801342B8.inc"
}
#else
s32 fn_801342B8(s32 idx) {
    EffectUtilEntryFunc fp = (EffectUtilEntryFunc)lbl_80478F8C;
    EffectUtilEntry* result;
    if (fp == NULL) {
        result = NULL;
    } else {
        result = fp(idx);
    }
    if (result == NULL) {
        return 0;
    }
    return result->link;
}
#endif
#if 0
asm void fn_80134768(void) {
#include "src/game/effect/effect_util_fn_80134768.inc"
}
#else
#pragma optimization_level 4
void* fn_80134768(void* base, s16 index) {
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    if ((s16)index >= 0 && (s16)index < 0xeb) {
        return (u8*)base + 0x6dec + (s32)(s16)index * 4;
    }
    return 0;
}
#endif
#if 0
asm void fn_80134A98(void) {
#include "src/game/effect/effect_util_fn_80134A98.inc"
}
#else
#pragma scheduling on
void* fn_80134A98(void* base, s8 index) {
    s8 i;
    if (base == 0) {
        base = (void*)fn_80129280(0, 3);
    }
    i = index;
    if (i < 0) goto _ret0;
    i = index;
    if (i < 3) goto _compute;
_ret0:
    return 0;
_compute:
    return (u8*)base + (s32)i * 0x24a4;
}
#pragma scheduling off
#endif
extern u32 lbl_80478B98;  /* effect count (SDA) */
#if 0
asm void fn_80136368(void) {
#include "src/game/effect/effect_util_fn_80136368.inc"
}
#else
#pragma scheduling on
u32 fn_80136368(u16 index) {
    EffectTraceFxEntry* entry;
    if ((u32)index > lbl_80478B98) {
        entry = NULL;
    } else {
        entry = &lbl_80363B88[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->effectId;
}
#pragma scheduling off
#endif
extern u32 lbl_80478B98;  /* effect count (SDA) */
#if 0
asm void fn_801363A8(void) {
#include "src/game/effect/effect_util_fn_801363A8.inc"
}
#else
#pragma scheduling on
u32 fn_801363A8(u16 index) {
    EffectTraceFxEntry* entry;
    if ((u32)index > lbl_80478B98) {
        entry = NULL;
    } else {
        entry = &lbl_80363B88[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->kind;
}
#pragma scheduling off
#endif
extern u32 lbl_80478BA0;  /* trace count (SDA) */
#if 0
asm void fn_801363E8(void) {
#include "src/game/effect/effect_util_fn_801363E8.inc"
}
#else
#pragma scheduling on
u32 fn_801363E8(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->value1;
}
#pragma scheduling off
#endif
extern u32 lbl_80478BA0;  /* trace count (SDA) */
#if 0
asm void fn_80136428(void) {
#include "src/game/effect/effect_util_fn_80136428.inc"
}
#else
#pragma scheduling on
u32 fn_80136428(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->kind;
}
#pragma scheduling off
#endif
extern u32 lbl_80478BA0;  /* trace count (SDA) */
#if 0
asm void fn_80136468(void) {
#include "src/game/effect/effect_util_fn_80136468.inc"
}
#else
#pragma scheduling on
u32 fn_80136468(u16 index) {
    EffectTraceEntry* entry;
    if ((u32)index > lbl_80478BA0) {
        entry = NULL;
    } else {
        entry = &lbl_80363C00[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->value0;
}
#pragma scheduling off
#endif
