/**
 * @file synthdata.c
 * @brief MusyX runtime data tables (musyx/runtime/synthdata.c), 0x80150C78 - 0x801525E4.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/synthdata.c`. Boundary evidence: simindex
 * identifies dataInsertKeymap (0x80150C78) through dataInit (0x801524E0)
 * as synthdata.c at seq=1.0 vs the matched MP4/Prime/Strikers copies
 * (including the maccmp/curvecmp/layercmp/fxcmp comparator cluster);
 * fn_801525C4 (0x20) is dataExit (reference synthdata.c's final one-call
 * wrapper), ending at mcmdWait (0x801525E4), synthmacros.c's first fn.
 * The dataInsert/dataRemove half (0x80150C78 - 0x8015210C) is asm-only.
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void  DCFlushRange(void* ptr, u32 size);
extern u32   OSDisableInterrupts(void);
extern void  OSRestoreInterrupts(u32 level);

/* renamed symbols referenced by asm incs (symbolmap port) */
extern void ARQPostRequest();
extern void InitStreamBuffers();
extern void aramQueueCallback();
extern void aramUploadData();
extern u32 inpGetMidiCtrl(u32 ctrl, u32 bank, u32 channel);
extern void salCalcVolume(u32 volumeArg, f32* out, u32 voiceIndex, f32 a, f32 b, f32 c, u32 hasPan, u32 studioFlag);
extern void salCallback();
extern u8 jumptable_80369CB0[];
extern u8 jumptable_80369CD4[];
extern u8 jumptable_80369CF8[];
extern u8 lbl_80273448[];
extern u8 lbl_8036944C[];
extern u8 lbl_8036BF00[];
extern u8 lbl_80434C50[];
extern f32 lbl_8047D4D8;
extern f32 lbl_8047D4DC;
extern f32 lbl_8047D4E0;
extern f64 lbl_8047D4E8;
extern f32 lbl_8047D4F0;
extern f32 lbl_8047D4F4;
extern f32 lbl_8047D4F8;
extern f32 lbl_8047D4FC;
extern f32 lbl_8047D500;
extern f32 lbl_8047D504;
extern f64 lbl_8047D508;
extern f32 lbl_8047D510;
extern f32 lbl_8047D514;
extern f64 lbl_8047D518;
extern f32 lbl_8047D520;
extern f64 lbl_8047D528;
extern f32 lbl_8047D530;
extern f32 lbl_8047D534;
extern u32 lbl_8047B070;
extern u32 lbl_8047B078;
extern u32 lbl_8047B07C;

/* GSmem allocator */
extern u16   _toolentryAlloc__FUl(u32 size);
extern void* fn_800E27B0(u16 handle);

/* External functions referenced from asm wrappers */
extern u32 sndAuxCallbackUpdateSettingsReverbHI(u8* ptr);

/* Model system */
extern void  fn_800EE150(void* model, u32 param);
extern void  fn_800EE828(void* model, u32 param);
extern void  fn_800E24B0(void* model, u32 param);
extern void  fn_800E209C(void* model, u32 param);
extern void  fn_800E01F4(void* dst, void* src);
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800E019C(void* model, void* param);
extern void  fn_800E0BA0(void* param);
extern void  fn_800E0BE4(void* param);
extern void  fn_800E013C(void* param);
extern u32   __cvt_fp2unsigned(f64 val);

/* Floor/field system */
extern void* fn_800F9318(u16 group, u16 model, u16 param);

/* GX rendering */
extern void  GSmodelSetVisibility(void* param);

/* People data layer (people_data.c) */
extern void* fn_801440A0(u16 index);   /* peopleFieldGetByIndex */
extern void* fn_80142CF4(u32 a, u32 b, u32 c, u32 d);  /* peopleFieldAlloc */
extern void  fn_801429E8(void* entry);  /* peopleFieldGetEntry */
extern void  fn_80142984(u32 id);       /* peopleFieldGetByID */

/* Script system */

s32 maccmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
typedef s32 (*PeopleCmpFn)(u8* a, u8* b);
extern void* sndBSearch(u8* key, u8* base, s32 count, u32 size, PeopleCmpFn cmp);
extern u8 lbl_8043D6F8[];
extern u32 lbl_8047AF98;
extern u8 lbl_8047AF90[8];  /* true .sbss size 0x8 -> @sda21 (was unsized [] => band mis-measured 96.57%) */
extern u8 lbl_8043DEF8[];
extern u32 lbl_8047AF9C;
extern u32 lbl_8047AF8C;
/* Early asm includes predate the symbol-map rename at 0x80162118. */
#define fn_80162118 sndBSearch
#if 0
asm void dataGetMacro(void) {
#include "src/game/people/people_field_fn_8015211C.inc"
}
#else
/* WIP decomp (jun17): functionally-correct REAL C, byte-match tops out at 70.43%.
 * Entire 2nd half (sth/slwi/sndBSearch/epilogue) matches once lbl_8047AF90 is
 * sized [8] (-> @sda21) and `sub` is u32 (-> slwi not clrlslwi). Residual WALL:
 * target loads `count` via indexed `lhzx r5,r4,r6` (base+offset separate); CW
 * here CSEs `lbl_8043D6F8 + idx*4` into one pointer -> `add`+plain `lhz`. The
 * lhzx-vs-add+lhz addressing choice resisted: precomputed-ptr, inline-twice, and
 * named-offset forms all CSE to the same code. Real C is active for coverage;
 * wrapper parked until the lhzx form is cracked. */
u32 dataGetMacro(u32 key) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    u8* p;
    u16 count;
    u32 sub;

    lbl_8047AF98 = (key >> 6) & 0x3FF;
    count = *(u16*)(lbl_8043D6F8 + ((key >> 6) & 0x3FF) * 4);
    if (count == 0) return 0;
    p = lbl_8043D6F8 + ((key >> 6) & 0x3FF) * 4;
    sub = *(u16*)(p + 2);
    *(u16*)(lbl_8047AF90 + 4) = (u16)key;
    lbl_8047AF9C = sub;
    result = sndBSearch(lbl_8047AF90, lbl_8043DEF8 + sub * 8, count, 8, maccmp);
    lbl_8047AF8C = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
#if 0
asm void fn_801521A8(void) {
#include "src/game/people/people_field_fn_801521A8.inc"
}
#else
s32 fn_801521A8(u16* a, u16* b) {
    return (s32)(a[0]) - (s32)(b[0]);
}
#endif
extern void _savegpr_20(void);
extern void _restgpr_20(void);
extern void _savegpr_23(void);
extern void _restgpr_23(void);
extern void _savegpr_24(void);
extern void _restgpr_24(void);
extern void _savegpr_25(void);
extern void _restgpr_25(void);
extern void _savegpr_27(void);
extern void _restgpr_27(void);
extern u8 lbl_80445EF8[];
extern u8 lbl_8043CCF8[];
extern u32 lbl_8047AF88;
extern u32 lbl_8047AF84;
extern u16 lbl_8047AFAA;
#if 0
asm void fn_801521B8(void) {
#include "src/game/people/people_field_fn_801521B8.inc"
}
#else
u32 fn_801521B8(u16 key, u32* out) {
    void* result;
    u8* header;
    u8* table;
    u32 i;

    *(u16*)lbl_80445EF8 = key;
    for (i = 0; i < lbl_8047AFAA; i++) {
        table = lbl_8043CCF8 + i * 0xC;
        result = sndBSearch(lbl_80445EF8, *(u8**)table, *(u16*)(table + 8), 0x20, (PeopleCmpFn)fn_801521A8);
        lbl_8047AF88 = (u32)result;
        if (result != NULL && *(u16*)((u8*)result + 2) != 0xFFFF) {
            header = (u8*)result + 0xC;
            lbl_8047AF84 = (u32)header;
            out[0] = *(u32*)header;
            out[1] = *(u32*)((u8*)result + 8);
            out[3] = 0;
            out[5] = *(u32*)((u8*)result + 0x14);
            out[4] = *(u32*)((u8*)result + 0x10) & 0x00FFFFFF;
            *(u8*)((u8*)out + 0x1C) = (u8)(*(u32*)((u8*)result + 0x10) >> 24);
            if (*(u32*)((u8*)result + 0x1C) != 0) {
                out[2] = *(u32*)table + *(u32*)((u8*)result + 0x1C);
            }
            return 0;
        }
    }
    return (u32)-1;
}
#endif
#if 0
asm void curvecmp(void) {
#include "src/game/people/people_field_curvecmp.inc"
}
#else
s32 curvecmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80438CF8[];
extern u8 lbl_8047AF7C[8];
extern u16 lbl_8047AFA8;
extern u32 lbl_8047AF78;
#if 0
asm void dataGetCurve(void) {
#include "src/game/people/people_field_dataGetCurve.inc"
}
#else
u32 dataGetCurve(u16 arg) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF7C + 4) = arg;
    result = sndBSearch(lbl_8047AF7C, lbl_80438CF8, lbl_8047AFA8, 8, curvecmp);
    lbl_8047AF78 = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
extern u8 lbl_804378F8[];
extern u8 lbl_8047AF70[8];
extern u16 lbl_8047AFA6;
extern u32 lbl_8047AF6C;
#if 0
asm void dataGetKeymap(void) {
#include "src/game/people/people_field_dataGetKeymap.inc"
}
#else
u32 dataGetKeymap(u16 arg) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF70 + 4) = arg;
    result = sndBSearch(lbl_8047AF70, lbl_804378F8, lbl_8047AFA6, 8, curvecmp);
    lbl_8047AF6C = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
#if 0
asm void layercmp(void) {
#include "src/game/people/people_field_layercmp.inc"
}
#else
s32 layercmp(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80445F18[];
extern u8 lbl_804380F8[];
extern u16 lbl_8047AFA4;
extern u32 lbl_8047AF68;
#if 0
asm void dataGetLayer(void) {
#include "src/game/people/people_field_fn_801523B8.inc"
}
#else
u32 dataGetLayer(u16 arg, u16* out) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_80445F18 + 4) = arg;
    result = sndBSearch(lbl_80445F18, lbl_804380F8, lbl_8047AFA4, 0xc, layercmp);
    lbl_8047AF68 = (u32)result;
    if (result != NULL) {
        *out = *(u16*)((u8*)result + 6);
        return *(u32*)(u32)lbl_8047AF68;
    }
    return 0;
}
#endif

extern u16 lbl_8047AFA0;
extern u16 lbl_8047AFA2;
extern u8 lbl_8043D2F8[];
extern u8 lbl_80445F24[];
#if 0
asm void fxcmp(void) {
#include "src/game/people/people_field_fn_80152434.inc"
}
#else
s32 fxcmp(u16* a, u16* b) {
    return (s32)(a[0]) - (s32)(b[0]);
}
#endif
#if 0
asm void dataGetFX(void) {
#include "src/game/people/people_field_fn_80152444.inc"
}
#else
u32 dataGetFX(u16 key) {
    extern void* sndBSearch(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    u8* table;
    s32 i;

    *(u16*)lbl_80445F24 = key;
    for (i = 0; i < lbl_8047AFA0; i++) {
        table = lbl_8043D2F8 + i * 8;
        result = sndBSearch(lbl_80445F24, *(u8**)(table + 4), *(u16*)(table + 2), 0xA, fxcmp);
        if (result != NULL) { return (u32)result; }
    }
    return 0;
}
#endif
#if 0
asm void dataInit(void) {
#include "src/game/people/people_field_dataInit.inc"
}
#else
typedef struct { u16 num; u16 subTabIndex; } DataMacMainEntry;

void dataInit(u32 smpBase, u32 smpLength) {
    extern void fn_8016300C(u32 a, u32 b);
    s32 i;

    lbl_8047AFAA = 0;
    lbl_8047AFA8 = 0;
    lbl_8047AFA6 = 0;
    lbl_8047AFA4 = 0;
    lbl_8047AFA0 = 0;
    lbl_8047AFA2 = 0;
    for (i = 0; i < 0x200; i++) {
        ((DataMacMainEntry*)lbl_8043D6F8)[i].num = 0;
        ((DataMacMainEntry*)lbl_8043D6F8)[i].subTabIndex = 0;
    }
    fn_8016300C(smpBase, smpLength);
}
#endif

#undef fn_80162118
