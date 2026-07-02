/**
 * @file gs_field_world.c
 * @brief GSfield world logic -- field camera, object management, world updates.
 *
 * This is the largest sub-module of GSfield, containing the field camera
 * system, world object management, and per-frame field update logic.
 *
 * Decompiled from (selected major functions):
 *   floorReadMapPreFunc - floorUpdateFieldCamera: Floor data parsing, scene setup
 *   floorUpdateFieldCamera (floorUpdateFieldCamera)
 *   floorUpdateFieldCamera - fn_80130CE0: World update, object management,
 *                               field rendering, transition logic
 *
 * The field camera (floorUpdateFieldCamera) is the most identifiable function via
 * its debug string: "floorUpdateFieldCamera: error updating field camera
 * - divide by zero!"
 *
 * This file covers approximately 600+ functions in the range
 * 0x80114CA8 - 0x80130CE0. The functions include:
 *   - Scene data loading and relocation
 *   - Field camera interpolation and cutscene cameras
 *   - Field object spawn/despawn/update
 *   - NPC placement and region triggers
 *   - Weather and ambient effect control
 *   - Door/warp transition logic
 *   - Field rendering pass management
 *   - Minimap integration
 *
 * Key patterns observed:
 *   - Many small accessor functions (0x18-0x34 bytes) that read
 *     struct fields at fixed offsets (0x8011E4A4-0x8011E800 area)
 *   - Iteration over object lists with 0x28 or 0x34 byte strides
 *   - Frequent calls to GSmem (fn_800E27B0, _toolentryAlloc__FUl)
 *   - Matrix operations for camera (PSMTXMultVec, fn_800A3A9C)
 *
 * Debug strings:
 *   "floorUpdateFieldCamera: error updating field camera - divide by zero!"
 *
 * Address range: 0x80114CA8 - 0x80130CE0
 *
 * Note: fn_8012640C (5456 bytes) and fn_801254B4 (3928 bytes) are
 * unmatched target slots that belong to this unit, but they are NOT
 * "field transition" / "field rendering" state machines -- that claim,
 * and fictional (void)-signature definitions named
 * GSfield_TransitionStateMachine / GSfield_RenderPass / GSfield_ObjectBatchUpdate /
 * GSfield_ProcessTriggers occupying their address comments, have been
 * removed. Cross-file call-site evidence (~1769 and ~350 call sites
 * respectively, consistently using a (context, slot, tableId, flags[,
 * value]) signature -- see trainer.c) shows fn_8012640C/fn_801254B4 are
 * the master data-table resolver/writer, still undecompiled. The other
 * two claimed addresses, fn_80124A60 and fn_8012CA84, are real and
 * already implemented further below in this file (100% and ~71%
 * matched respectively).
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* memcpy(void* dst, const void* src, u32 n);
extern void* memset(void* dst, int val, u32 size);
/* GSmem */
extern u16   _toolentryAlloc__FUl(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                   /* GSmemLock */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */
/* Matrix / vector */
extern void  PSMTXMultVec(void* mtx, void* vec, void* out); /* MTXMultVec3 */
extern void  fn_800A3A9C(void* out, void* in, f32 s);     /* VEC normalize */
/* GSgfx renderer */
extern void  fn_800D7868(void* handle, u32 a, u32 b, u32 c,
                          u32 d, u32 e, u32 f, u32 g);     /* GSgfx draw setup */
extern void* fn_800D7894(void);                             /* GSgfx create render obj */
/* Real named labels referenced by remaining inline-asm wrappers in this TU. */
extern void _modelSetRotateEulerToQuatAll__FP9_HSD_JObj();
extern void cos();
extern void gamedataAttestCheckValid();
extern void gamedataAttestCreate();
extern void gamedataAttestInit();
extern void gamedataCreate();
extern void gamedataGetStatus();
extern void GScharCmp();
extern void GScharLenCpy();
extern void GSflagClear();
extern void GSmsgFontClose();
extern void itemGetStatus();
extern void LCStoreData();
extern void memoDataSet();
extern void menuSubGetPokemonSexForFightDisp();
extern void OSGetTick();
extern void psInitAppSRT();
extern void psInitParticle();
extern void psSetGeneratorAngleRadiusScale();
extern void psSetParticleVisibility();
extern void sin();
extern void statusGetStatus();
/* GSfloor / GScolsys */
extern void* fn_800FF56C(void);                             /* GSfloor get active */
extern void  fn_8010C7BC(u32 triIdx, void* outFlag);        /* GScolsys query */
/* ===== Index lookup globals ===== */
extern u8 lbl_8035BBA8[];  /* NPC table (BSS) */
extern u8 lbl_8035C430[];  /* field obj table (BSS) */
extern u32 lbl_80478B48;  /* NPC count (SDA) */
extern u32 lbl_80478B50;  /* field obj count (SDA) */
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
extern u32 lbl_80478F94;  /* obj data base (SDA) */
/* Field subsystems -- forward declarations (defined below) */
u32  fn_80115094(void);
u32 fn_80115A38(u8* entry);
void fn_80117C84(void);
void fn_8012546C(void* ptr);
/* ===== String constants (rodata) ===== */
extern const char lbl_80272770[]; /* "floorUpdateFieldCamera: error updating..." */
extern const char lbl_802724E8[]; /* "floorReadMapPreFunc: can't alloc..." */
extern const char lbl_80272520[]; /* "floorReadScriptPreFunc(): can't alloc..." */
extern const char lbl_8027255C[]; /* "floorReadFontPreFunc(): can't alloc..." */
extern const char lbl_80272594[]; /* "floorReadMsgPreFunc(): can't alloc..." */
extern const char lbl_802725CC[]; /* "floorReadNormalPreFunc(): can't alloc..." */
/* ==================================================================
 * floorUpdateFieldCamera_Pseudocode -- floorUpdateFieldCamera notes
 *
 * Update the field camera each frame. Interpolates position, target,
 * and FOV toward their destination values. Includes safety check for
 * divide-by-zero when computing distance-based interpolation.
 *
 * This function is 0x1B4 bytes (436 bytes) and uses extensive float
 * math for smooth camera transitions.
 *
 * From disassembly:
 *   - Loads camera state from a BSS pointer
 *   - Computes direction vector from current to destination
 *   - Normalizes and scales by interpSpeed
 *   - If distance is near-zero, snaps to destination (avoids /0)
 *   - On divide-by-zero path: logs lbl_80272770
 * ================================================================== */
void floorUpdateFieldCamera_Pseudocode(void) {
    extern f32 lbl_8047CFD0;
    extern f32 lbl_8047CFDC;
    extern f32 lbl_8047CFE0;
    extern void fn_800E0020();
    extern void fn_800E01F4();
    extern u32 lbl_8047AD68;
    extern u32 lbl_8047AD6C;
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x70) = f29;
    /* psq_st f29, 0x78((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x60) = f28;
    /* psq_st f28, 0x68((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x50) = f27;
    /* psq_st f27, 0x58((u32)sp), 0, qr0 */;
    *(f64*)(sp + 0x40) = f26;
    /* psq_st f26, 0x48((u32)sp), 0, qr0 */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r28 = r6;
    r0 = lbl_8047AD68;
    if (r0 == (u32)0x1) {
        r4 = lbl_8047AD6C;
        r3 = 0x1;
        f0 = *(f32*)((u8*)r4 + 0x10);
        *(f32*)((u8*)r26 + 0x0) = f0;
        r4 = lbl_8047AD6C;
        f0 = *(f32*)((u8*)r4 + 0xC);
        *(f32*)((u8*)r27 + 0x0) = f0;
        r4 = lbl_8047AD6C;
        f0 = *(f32*)((u8*)r4 + 0x14);
        *(f32*)((u8*)r28 + 0x0) = f0;
    } else {
    f29 = lbl_8047CFD0;
    r30 = 0x0;
    r31 = 0x0;
    f28 = f29;
    f27 = f29;
    f26 = f29;
    f30 = lbl_8047CFDC;
    f31 = lbl_8047CFE0;
    while (r0 = lbl_8047AD68, r30 < r0) {

    r0 = lbl_8047AD6C;
    r3 = (u32)sp + 0x8;
    r29 = r0 + r31;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f2 = *(f32*)((u8*)r29 + 0x4);
    f3 = *(f32*)((u8*)r29 + 0x8);
    fn_800E01F4();
    r4 = r25;
    r3 = (u32)sp + 0x8;
    fn_800E0020();
    if (f1 > f30) {
        f3 = f31 / f1;
        f2 = *(f32*)((u8*)r29 + 0xC);
        f1 = *(f32*)((u8*)r29 + 0x10);
        f0 = *(f32*)((u8*)r29 + 0x14);
        f29 = f29 + f3;
        f28 = f2 * f3 + f28;
        f27 = f1 * f3 + f27;
        f26 = f0 * f3 + f26;
    r31 = r31 + 0x18;
    r30 = r30 + 0x1;
        continue;
    }
    f29 = f30;
    f27 = *(f32*)((u8*)r29 + 0x10);
    f28 = *(f32*)((u8*)r29 + 0xC);
    f26 = *(f32*)((u8*)r29 + 0x14);
    break;
    }

    f0 = lbl_8047CFD0;
    if (f0 == f29) {
        r3 = (u32)&lbl_80272770;
        r3 = (u32)&lbl_80272770;
        ((void(*)(void))fn_800DD970)();
        r3 = 0x0;
    } else {
    f0 = lbl_8047CFDC;
    r3 = 0x1;
    f0 = f0 / f29;
    f2 = f27 * f0;
    f1 = f28 * f0;
    f0 = f26 * f0;
    *(f32*)((u8*)r26 + 0x0) = f2;
    *(f32*)((u8*)r27 + 0x0) = f1;
    *(f32*)((u8*)r28 + 0x0) = f0;
    }
    }
    /* psq_l f31, 0x98((u32)sp), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88((u32)sp), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    /* psq_l f29, 0x78((u32)sp), 0, qr0 */;
    f29 = *(f64*)(sp + 0x70);
    /* psq_l f28, 0x68((u32)sp), 0, qr0 */;
    f28 = *(f64*)(sp + 0x60);
    /* psq_l f27, 0x58((u32)sp), 0, qr0 */;
    f27 = *(f64*)(sp + 0x50);
    /* psq_l f26, 0x48((u32)sp), 0, qr0 */;
    f26 = *(f64*)(sp + 0x40);
    return;
}
/* Note: fn_8012640C is the master data-table resolver called with a
 * (context, slot, tableId, flags) signature across ~1769 call sites
 * throughout the codebase (see trainer.c). It is not yet decompiled
 * (missing from this unit); a prior fictional (void)-signature
 * "GSfield_TransitionStateMachine" definition claiming this address
 * has been removed -- the real function takes arguments and its
 * body is unrelated to field transitions. */
/* Note: fn_801254B4 is the master data-table writer called with a
 * (context, slot, tableId, flags, value) signature across ~350 call
 * sites throughout the codebase (see trainer.c). It is not yet
 * decompiled (missing from this unit); a prior fictional
 * (void)-signature "GSfield_RenderPass" definition claiming this
 * address has been removed -- the real function takes arguments and
 * its body is unrelated to rendering. */
/* Note: fn_80124A60 (field-object init, 2008 bytes) is implemented and
 * matched at 100% further below in this file. A prior fictional
 * (void)-signature "GSfield_ObjectBatchUpdate" duplicate claiming
 * this address has been removed. */
/* Note: fn_8012CA84 (field movement/heading processor, 2104 bytes) is
 * implemented further below in this file. A prior fictional
 * (void)-signature "GSfield_ProcessTriggers" duplicate claiming this
 * address has been removed. */
/* ==================================================================
 * Small accessor functions -- field object getters
 *
 * The 0x8011E4A4-0x8011E800 range contains many tiny (0x18-0x34 byte)
 * functions that read specific fields from a field object struct.
 * Pattern: null check, then load from fixed offset and return.
 *
 * Example (fn_8011E4D8, 0x18 bytes):
 *   cmplwi r3, 0x0
 *   bne .ok
 *   li r3, 0x0
 *   blr
 *   .ok:
 *   lhz r3, 0x8(r3)    ; read u16 at offset 0x8
 *   blr
 * ================================================================== */
/* fn_8011E4A4 */ u16 GSfield_GetObjAttrU16(void* obj, u16 slot) {
    if (obj == NULL) { return 0; }
    if (slot >= 2) { return 0; }
    return *(u16*)((u8*)obj + slot * 2 + 0x70);
}
/* fn_8011E4D8 */ u16 GSfield_GetObjType(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x8);
}
/* fn_8011E4F0 */ u8 GSfield_GetObjSubtype(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x2);
}
/* fn_8011E508 */ u8 GSfield_GetObjFlags(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x1);
}
/* fn_8011E520 */ u8 GSfield_GetObjState(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u8*)((u8*)obj + 0x0);
}
/* fn_8011E538 */ u16 GSfield_GetObjGroupId(void* obj) {
    if (obj == NULL) { return 0; }
    return *(u16*)((u8*)obj + 0x6);
}
/* fn_8011E550 */ u16 GSfield_GetObjRegionId(void* obj) {
    u8* sub;
    if (obj == NULL) { return 0; }
    sub = (u8*)obj + 0x90;
    if (sub == NULL) { return 0; }
    return *(u16*)(sub + 0xA);
}
/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 208 functions matched
 * =================================================================== */
extern u8 lbl_8047AD71;
extern u32 lbl_8047ADC0;
/* Address: 0x801174EC | Size: 0x8 | Pattern: sda_getter */
u8 fn_801174EC(void) {
    return lbl_8047AD71;
}
/* Address: 0x80118068 | Size: 0x8 | Pattern: return_constant */
u32 fn_80118068(void) { return 0; }
/* Address: 0x80119F90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119F90(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x80119FA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FA0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}
/* Address: 0x80119FB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}
/* Address: 0x80119FC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FC0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}
/* Address: 0x80119FD0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FD0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x80119FE0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FE0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}
/* Address: 0x80119FF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_80119FF0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}
/* Address: 0x8011A000 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A000(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011A018 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011A018(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}
/* Address: 0x8011A030 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A030(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011A048 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A048(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011A060 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011A060(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011A078 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A078(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}
/* Address: 0x8011A090 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011A090(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
/* Address: 0x8011C420 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C420(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x13]) = val;
}
/* Address: 0x8011C570 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C570(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x13]);
}
/* Address: 0x8011C5B8 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5B8(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x12]) = val;
}
/* Address: 0x8011C5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x12]);
}
/* Address: 0x8011C5E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x15]) = val;
}
/* Address: 0x8011C5F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C5F0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x14]) = val;
}
/* Address: 0x8011C600 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C600(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8011C610 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C610(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x30]) = val;
}
/* Address: 0x8011C620 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C620(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x11]) = val;
}
/* Address: 0x8011C630 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C630(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}
/* Address: 0x8011C640 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C640(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}
/* Address: 0x8011C650 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C650(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}
/* Address: 0x8011C660 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C660(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD]) = val;
}
/* Address: 0x8011C670 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C670(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xC]) = val;
}
/* Address: 0x8011C680 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C680(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xB]) = val;
}
/* Address: 0x8011C690 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C690(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA]) = val;
}
/* Address: 0x8011C6A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x9]) = val;
}
/* Address: 0x8011C6B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6B0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x8]) = val;
}
/* Address: 0x8011C6C0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6C0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x7]) = val;
}
/* Address: 0x8011C6D0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6D0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x6]) = val;
}
/* Address: 0x8011C6E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6E0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x5]) = val;
}
/* Address: 0x8011C6F0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C6F0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x28]) = val;
}
/* Address: 0x8011C700 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C700(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x1A]) = val;
}
/* Address: 0x8011C710 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C710(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x18]) = val;
}
/* Address: 0x8011C720 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C720(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x16]) = val;
}
/* Address: 0x8011C730 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C730(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x8011C740 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C740(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}
/* Address: 0x8011C750 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C750(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}
/* Address: 0x8011C760 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C760(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}
/* Address: 0x8011C770 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C770(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}
/* Address: 0x8011C780 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011C780(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x20]) = val;
}
/* Address: 0x8011C790 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C790(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x15]);
}
/* Address: 0x8011C7A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C7A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x14]);
}
/* Address: 0x8011C7C0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C7C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x8011C7D8 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C7D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x30]);
}
/* Address: 0x8011C7F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C7F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}
/* Address: 0x8011C808 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011C820 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C820(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011C838 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C838(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011C850 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C850(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}
/* Address: 0x8011C868 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C868(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}
/* Address: 0x8011C880 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C880(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}
/* Address: 0x8011C898 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C898(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA]);
}
/* Address: 0x8011C8B0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}
/* Address: 0x8011C8C8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}
/* Address: 0x8011C8E0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8E0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}
/* Address: 0x8011C8F8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C8F8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011C910 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C910(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011C928 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C928(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x28]);
}
/* Address: 0x8011C940 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011C940(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x24]);
}
/* Address: 0x8011C958 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011C958(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x1A]);
}
/* Address: 0x8011C970 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011C970(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x18]);
}
/* Address: 0x8011C988 | Size: 0x18 | Pattern: nullcheck_getter */
s16 fn_8011C988(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(s16*)(&ptr[0x16]);
}
/* Address: 0x8011C9A0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011C9B8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9B8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}
/* Address: 0x8011C9D0 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011C9D0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011C9EC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011C9EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}
/* Address: 0x8011CA04 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CA04(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011CA1C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CA1C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}
/* Address: 0x8011CA9C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CA9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CB3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}
/* Address: 0x8011CB54 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CB54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* Address: 0x8011CB98 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CB98(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011CBB0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CBB0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011CC54 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC54(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x11]);
}
/* Address: 0x8011CC6C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011CC84 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC84(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011CC9C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CC9C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011CCB4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCB4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD]);
}
/* Address: 0x8011CCCC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCCC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xC]);
}
/* Address: 0x8011CCE4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CCE4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xB]);
}
/* Address: 0x8011CCFC | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CCFC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x4];
}
/* Address: 0x8011CD18 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD18(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x3];
}
/* Address: 0x8011CD34 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD34(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x2];
}
/* Address: 0x8011CD50 | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD50(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x1];
}
/* Address: 0x8011CD6C | Size: 0x1C | Pattern: nullcheck_getter_s8 */
s32 fn_8011CD6C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)ptr[0x0];
}
/* Address: 0x8011CD88 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CD88(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x9]);
}
/* Address: 0x8011CDA0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDA0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x8]);
}
/* Address: 0x8011CDB8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDB8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x7]);
}
/* Address: 0x8011CDD0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDD0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x6]);
}
/* Address: 0x8011CDE8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011CDE8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x5]);
}
/* Address: 0x8011CE00 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011CE00(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011CEA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x4]) = val;
}
/* Address: 0x8011CEB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x3]) = val;
}
/* Address: 0x8011CEC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CEC0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x16]) = val;
}
/* Address: 0x8011CF14 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF14(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8011CF24 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF24(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x12]) = val;
}
/* Address: 0x8011CF34 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF34(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x14]) = val;
}
/* Address: 0x8011CF9C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CF9C(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xE]) = val;
}
/* Address: 0x8011CFAC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFAC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x20]) = val;
}
/* Address: 0x8011CFBC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFBC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011CFCC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFCC(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA]) = val;
}
/* Address: 0x8011CFDC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011CFDC(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x1C]) = val;
}
/* Address: 0x8011D270 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D270(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x8]) = val;
}
/* Address: 0x8011D280 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D280(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x2]) = val;
}
/* Address: 0x8011D290 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D290(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x1]) = val;
}
/* Address: 0x8011D2A0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2A0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}
/* Address: 0x8011D2B0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D2B0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}
/* Address: 0x8011D470 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D470(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x18]) = val;
}
/* Address: 0x8011D494 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D494(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD6]) = val;
}
/* Address: 0x8011D4A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4A4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD2]) = val;
}
/* Address: 0x8011D4B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4B4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD4]) = val;
}
/* Address: 0x8011D4C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4C4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xD0]) = val;
}
/* Address: 0x8011D4D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4D4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCF]) = val;
}
/* Address: 0x8011D4E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4E4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCE]) = val;
}
/* Address: 0x8011D4F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D4F4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCD]) = val;
}
/* Address: 0x8011D56C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D56C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCB]) = val;
}
/* Address: 0x8011D57C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D57C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xCA]) = val;
}
/* Address: 0x8011D760 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D760(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xBC]) = val;
}
/* Address: 0x8011D8F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011D8F4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xD8]) = val;
}
/* Address: 0x8011DCB4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DCB4(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x88]) = val;
}
/* Address: 0x8011DE38 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE38(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x74]) = val;
}
/* Address: 0x8011DE88 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE88(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xE0]) = val;
}
/* Address: 0x8011DE98 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DE98(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x5C]) = val;
}
/* Address: 0x8011DF90 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DF90(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x14]) = val;
}
/* Address: 0x8011DFA0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFA0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x10]) = val;
}
/* Address: 0x8011DFB0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFB0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xF]) = val;
}
/* Address: 0x8011DFC0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFC0(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xE]) = val;
}
/* Address: 0x8011DFD0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFD0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xC]) = val;
}
/* Address: 0x8011DFE0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFE0(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}
/* Address: 0x8011DFF0 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8011DFF0(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}
/* Address: 0x8011E000 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E000(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x4]);
}
/* Address: 0x8011E018 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E018(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x3]);
}
/* Address: 0x8011E030 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E030(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x16]);
}
/* Address: 0x8011E0AC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E0AC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x8011E0C4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x12]);
}
/* Address: 0x8011E0DC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E0DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x14]);
}
/* Address: 0x8011E15C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E15C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE]);
}
/* Address: 0x8011E174 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E174(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x20]);
}
/* Address: 0x8011E18C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E18C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011E1A4 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E1A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA]);
}
/* Address: 0x8011E1BC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E1BC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x1C]);
}
/* Address: 0x8011E4D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E4D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8]);
}
/* Address: 0x8011E4F0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E4F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x2]);
}
/* Address: 0x8011E508 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E508(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x1]);
}
/* Address: 0x8011E520 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}
/* Address: 0x8011E538 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}
/* Address: 0x8011E760 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011E760(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x18]);
}
/* Address: 0x8011E7C0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD6]);
}
/* Address: 0x8011E7D8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7D8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD2]);
}
/* Address: 0x8011E7F0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011E7F0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD4]);
}
/* Address: 0x8011E808 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E808(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xD0]);
}
/* Address: 0x8011E820 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E820(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCF]);
}
/* Address: 0x8011E838 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E838(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCE]);
}
/* Address: 0x8011E850 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E850(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCD]);
}
/* Address: 0x8011E8DC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8DC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCB]);
}
/* Address: 0x8011E8F4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011E8F4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xCA]);
}
/* Address: 0x8011EB48 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011EB48(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xBC]);
}
/* Address: 0x8011EDF8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EDF8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xE4]);
}
/* Address: 0x8011EE10 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE10(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xE0]);
}
/* Address: 0x8011EE28 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011EE28(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xDC]);
}
/* Address: 0x8011EE40 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE40(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xD8]);
}
/* Address: 0x8011EE58 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011EE58(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xB0]);
}
/* Address: 0x8011F188 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F188(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x8A]);
}
/* Address: 0x8011F1A0 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F1A0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x88]);
}
/* Address: 0x8011F45C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F45C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x74]);
}
/* Address: 0x8011F4A8 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F4A8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x60]);
}
/* Address: 0x8011F4C0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F4C0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x5C]);
}
/* Address: 0x8011F520 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F520(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x14]);
}
/* Address: 0x8011F538 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F538(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x10]);
}
/* Address: 0x8011F550 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F550(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xF]);
}
/* Address: 0x8011F568 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8011F568(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xE]);
}
/* Address: 0x8011F580 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F580(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xC]);
}
/* Address: 0x8011F5B0 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8011F5B0(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}
/* Address: 0x8012A774 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A774(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA81]) = val;
}
/* Address: 0x8012A784 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A784(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA81]);
}
/* Address: 0x8012A79C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A79C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA98]);
}
/* Address: 0x8012A7B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A7B4(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA98]) = val;
}
/* Address: 0x8012A7C4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A7C4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA8C]);
}
/* Address: 0x8012A80C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A80C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA88]);
}
/* Address: 0x8012A854 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012A854(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA84]);
}
/* Address: 0x8012A8EC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A8EC(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA97]);
}
/* Address: 0x8012A904 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A904(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA96]);
}
/* Address: 0x8012A91C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A91C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA95]);
}
/* Address: 0x8012A934 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A934(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA94]);
}
/* Address: 0x8012A94C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A94C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA93]);
}
/* Address: 0x8012A964 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A964(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA92]);
}
/* Address: 0x8012A97C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A97C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA91]);
}
/* Address: 0x8012A994 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012A994(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA90]);
}
/* Address: 0x8012A9AC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9AC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA97]) = val;
}
/* Address: 0x8012A9BC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9BC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA96]) = val;
}
/* Address: 0x8012A9CC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9CC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA95]) = val;
}
/* Address: 0x8012A9DC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9DC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA94]) = val;
}
/* Address: 0x8012A9EC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9EC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA93]) = val;
}
/* Address: 0x8012A9FC | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012A9FC(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA92]) = val;
}
/* Address: 0x8012AA0C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA0C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA91]) = val;
}
/* Address: 0x8012AA1C | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA1C(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA90]) = val;
}
/* Address: 0x8012AA2C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fn_8012AA2C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA80]);
}
/* Address: 0x8012AA44 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA44(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA80]) = val;
}
/* Address: 0x8012AA54 | Size: 0x10 | Pattern: nullcheck_setter */
void fn_8012AA54(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x2C]) = val;
}
/* Address: 0x8012AC3C | Size: 0x18 | Pattern: nullcheck_getter */
u32 fn_8012AC3C(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x2C]);
}
/* Address: 0x80130CD8 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80130CD8(void) {
    return lbl_8047ADC0;
}
/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */
extern u32 lbl_8047ADB8;
/* Address: 0x8011F5C8 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fn_8011F5C8(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
/* Address: 0x80128E24 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80128E24(void) {
    return lbl_8047ADB8;
}
/* ===================================================================
 * Generated: 1 pattern-matched + 515 stubs
 * Range: 0x80114CA8 - 0x80130CD8
 * =================================================================== */
extern u8 lbl_804083D0[0x30];
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern void fn_801ED674(void);
/* Forward declarations for functions called before definition */
void fn_801193BC(void);
/* Forward declarations for converted functions */
s32 fn_80123E70(u8* ptr, u16 idx);
void fn_8011BEB4(void);
void fn_8011F260(void);
void fn_8012546C(void*);
void fn_8012795C(void);
void fn_80129BC8(void);
void fn_8012A450();
void fn_8012A5B0(void);
/* 0x70 | floorReadMapPreFunc | alloc_wrapper */
extern void* fn_800F9418();  /* K&R: called with 5 args, returns void* */
#pragma push
#pragma peephole off
void* floorReadMapPreFunc(void* owner, u32 param, u32 alloc_size) {
    u32 total = ((alloc_size + 0x1F) & ~0x1F) + 0x60;
    void* mem = (void*)fn_800F9418(total, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970(lbl_802724E8, total);
        return NULL;
    }
    return (u8*)mem + 0x60;
}
#pragma pop
/* 0x80114D6C | 0xA0 */
extern u8 fn_800FF548(void);
extern u32 fn_80115124();  /* K&R: asm void wrapper, used as function pointer */
extern u32 fn_80115170();  /* K&R: asm void wrapper, used as function pointer */
extern u32 fn_801151BC();  /* K&R: asm void wrapper, used as function pointer */
#pragma push
#pragma peephole off
void* floorReadScriptPreFunc(void* owner, u32 param, u32 alloc_size) {
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    alloc_size = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(alloc_size, 0x20, (u32)owner, param, (u32)fn_80115124);
    if (mem == NULL) {
        fn_800DD970(lbl_80272520, alloc_size);
    }
    return mem;
}
/* 0x80114E78 | 0xA0 */
void* floorReadFontPreFunc(void* owner, u32 param, u32 alloc_size) {
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    alloc_size = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(alloc_size, 0x20, (u32)owner, param, (u32)fn_80115170);
    if (mem == NULL) {
        fn_800DD970(lbl_8027255C, alloc_size);
    }
    return mem;
}
/* 0x80114F84 | 0xA0 */
void* floorReadMsgPreFunc(void* owner, u32 param, u32 alloc_size) {
    void* mem;
    if ((u8)fn_800FF548() != 0) { return NULL; }
    alloc_size = (alloc_size + 0x1F) & ~0x1F;
    mem = (void*)fn_800F9418(alloc_size, 0x20, (u32)owner, param, (u32)fn_801151BC);
    if (mem == NULL) {
        fn_800DD970(lbl_80272594, alloc_size);
    }
    return mem;
}
#pragma pop
/* 0x80115094 | 0x24 | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115094(void) {
    fn_801ED674();
    return 1;
}
#pragma pop
/* 0x801150B8 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_801150B8(void) {
    fn_801193BC();
    return 1;
}
#pragma pop
/* 0x801150DC | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_801150DC(void) {
    fn_800D2738();
    return 1;
}
#pragma pop
/* 0x80115100 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115100(void) {
    fn_800DCD98();
    return 1;
}
#pragma pop
/* 0x80115208 | 36 bytes | call_return_const */
#pragma push
#pragma scheduling off
u32 fn_80115208(void) {
    GScolsys2UnloadCCD();
    return 1;
}
#pragma pop
/* 0x8011522C | 36 bytes | call_return_const */
extern void fn_800EF5A4(void);
#pragma push
#pragma scheduling off
u32 fn_8011522C(void) {
    fn_800EF5A4();
    return 1;
}
#pragma pop
/* 0x80115250 | 0xC */
u32 fn_80115250(u32 val) {
    return (val & 0x7FFF0000U) | 0x1A00;
}
/* 0x8011525C | 0xC */
u32 fn_8011525C(u32 val) {
    return (val & 0x7FFF0000U) | 0x1800;
}
/* 0x80115268 | 0xC */
u32 fn_80115268(u32 val) {
    return (val & 0x7FFF0000U) | 0x1600;
}
/* 0x80115274 | 0xC */
u32 fn_80115274(u32 val) {
    return (val & 0x7FFF0000U) | 0x1000;
}
/* 0x80115280 | 0x10C */
#pragma push
#pragma peephole off
u32 fn_80115280(void* ptr) {
    extern const char lbl_80272608[];
    extern const char lbl_8027262C[];
    extern u8 lbl_8035BB50[];
    u32* data;
    u32 count = 0;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BB50);
        return 0;
    }
    data = *(u32**)((u8*)ptr + 0x10);
    if (data == NULL) {
        return 0;
    }
    data = *(u32**)data;
    if (data == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BB50);
        return 0;
    }
    if (data[2] != 0) { count++; }
    if (data[3] != 0) { count++; }
    if (data[4] != 0) { count++; }
    if (data[5] != 0) { count++; }
    if (data[6] != 0) { count++; }
    if (data[7] != 0) { count++; }
    if (data[8] != 0) { count++; }
    if (data[9] != 0) { count++; }
    return count;
}
#pragma pop
/* 0x8011538C | 0xA0 */
extern const char lbl_80272608[];
extern const char lbl_8027262C[];
extern u8 lbl_8035BB30[];
extern u8 lbl_8035BB50[];
#pragma push
#pragma peephole off
void* fn_8011538C(void* ptr, u32 idx) {
    void* p1;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BB30);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    if (idx >= 8) { return NULL; }
    p1 = *(void**)p1;
    if (p1 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BB30);
        return NULL;
    }
    (u8*)p1 += idx * 4;
    return *(void**)((u8*)p1 + 8);
}
#pragma pop
/* 0x8011542C | 0x88 */
extern const char lbl_80272608[];
extern const char lbl_8027262C[];
extern u8 lbl_8035BB10[];
#pragma push
#pragma scheduling on
#pragma peephole off
void* fn_8011542C(void* ptr) {
    void* p1;
    void* p2;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BB10);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    p2 = *(void**)p1;
    if (p2 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BB10);
        return NULL;
    }
    return *(void**)((u8*)p2 + 0x4);
}
#pragma pop
/* 0x801154B4 | 0x88 */
extern u8 lbl_8035BAF4[];
#pragma push
#pragma scheduling on
#pragma peephole off
void* fn_801154B4(void* ptr) {
    void* p1;
    void* p2;
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BAF4);
        return NULL;
    }
    p1 = *(void**)((u8*)ptr + 0x10);
    if (p1 == NULL) { return NULL; }
    p2 = *(void**)p1;
    if (p2 == NULL) {
        fn_800DD970(lbl_8027262C, lbl_8035BAF4);
        return NULL;
    }
    return *(void**)p2;
}
#pragma pop
/* 0x48 | fn_8011553C | nullcheck_store */
extern const char lbl_80272658[];
extern u8 lbl_8035BAD8[];
#pragma push
#pragma peephole off
void fn_8011553C(void* obj, u32 val) {
    if (obj == NULL) {
        fn_800DD970(lbl_80272658, lbl_8035BAD8, val);
        return;
    }
    *(u32*)((u8*)obj + 0x34) = val;
}
#pragma pop
/* 0x48 | fn_80115584 | nullcheck_store */
extern const char lbl_80272680[];
extern u8 lbl_8035BABC[];
#pragma push
#pragma peephole off
void fn_80115584(void* obj, u32 val) {
    if (obj == NULL) {
        fn_800DD970(lbl_80272680, lbl_8035BABC, val);
        return;
    }
    *(u32*)((u8*)obj + 0x8) = val;
}
#pragma pop
/* 0xfn_80115A38 | global_cond_call */
#pragma push
#pragma scheduling on
#pragma peephole off
u32 fn_80115A38(u8* entry) {
    extern u8 lbl_8035B91C[];
    if (entry == 0) {
        fn_800DD970(lbl_80272608, (const char*)lbl_8035B91C);
        return 0;
    }
    return *(u32*)(entry + 0x4);
}
#pragma pop
/* 0x70 | fn_80115BD8 | generic */
extern u32 lbl_80478FB8;
extern u32 lbl_80478FBC;
extern u8 lbl_802726D4[];
extern u8 lbl_8035B8A0[];
/* Forward declarations for self-referencing asm blocks */
extern void* fn_801155CC();
extern u32 fn_80115A80();
extern void* fn_80115C48(u32 key);
extern void fn_8011791C(void);
extern void fn_80119930(void);
extern void fn_80119BD0();
extern u8 fn_80119D90(u16 idx);
extern u8 fn_80119DD0(u16 idx);
extern u16 fn_80119E10(u16 idx);
extern u8 fn_80119E50(u16 idx);
extern u8 fn_80119E90(u16 idx);
extern u16 fn_80119ED0(u16 idx);
extern u8 fn_80119F10(u16 idx);
extern u32 fn_80119F50(u16 idx);
extern void fn_8011C1D0(u8* ptr, u32 val);
extern void fn_8011C220(u8* ptr, u32 val);
extern u32 fn_8011C270(u8* ptr);
extern u8 fn_8011C588(u8* ptr, u16 idx);
extern u32 fn_8011C2D0(u8* ptr);
extern void fn_8011C330(u8* ptr, u32 val);
extern void fn_8011C380(u8* ptr, u32 val);
extern void fn_8011C3D0(u8* ptr, u32 val);
extern void fn_8011C430(u8* ptr, u16 idx, u8 val);
extern u32 fn_8011C450(u8* ptr);
extern u32 fn_8011C4B0(u8* ptr);
extern u32 fn_8011C510(u8* ptr);
extern void fn_8011F77C(void);
extern u32  fn_80120C6C(u32 a, u16 key);
extern u8 fn_80121ADC(u8* ptr, u32 slot);
extern void fn_801237B8(void);
extern u32 fn_80123CD4(u8* ptr, u32 arg2);
extern void fn_80124A60(u8* ptr);
extern void fn_80128300(void);
extern void fn_80128524(void);
extern void fn_80129094(void);
extern void heroAddPokedoru(u8* ptr, u32 offset);
extern s32 fn_80129A78(u8* ptr, u32 arg2, u32 arg3, u32 arg4);
extern u32 fn_8012A08C(u8* ptr, void* arg2);
extern void fn_8012A1A4(u8* ptr, u32 arg2, u8 arg3);
extern void fn_8012A248();
extern void fn_8012A7DC();  /* K&R: typed impl or conflict */
extern void fn_8012A824();  /* K&R: typed impl or conflict */
extern void fn_8012A86C();  /* K&R: typed impl or conflict */
extern void heroBiosSetHizukiNamePtr();  /* K&R: typed impl or conflict */
extern void heroBiosSetNamePtr();  /* K&R: typed impl or conflict */
extern void fn_8012BBA8(void);
extern void fn_8012BCA4(void);
extern void fn_8012CA84();
extern void fn_80130A88(u32 arg1);
extern void fn_80130BB0(u32 arg1);
extern s32 psGetGeneratorChildMaxLife(u32);
extern void* wazaDataBiosGetPtr(u16 idx);
extern u32 fn_8012640C();
extern void fn_801254B4();
extern void fn_8011BEB4(void);
extern u16 fn_8011E36C(u8* ptr, u16 idx);
extern u16 fn_8011E3B4(u8* ptr, u16 idx);
extern u8 fn_8011E3FC(u8* ptr, u16 idx);
extern void fn_8011F260(void);
extern void fn_80129BC8(void);
extern u8 floorUpdateFieldCamera();
extern void fn_8012A450();
extern void fn_8012A5B0(void);
extern void fn_8012C660(void);
extern void* fn_8012AC08(u8* ptr, u16 idx);
extern void* fn_8012A8D4(void* ptr);
extern void* fn_8012AA9C(u8* ptr, u16 idx);
extern void* fn_8012AAD0(u8* ptr, u16 idx);
extern void* fn_8012AB04(u8* ptr, u16 idx);
extern void* fn_8012AB38(u8* ptr, u16 idx);
extern void* fn_8012AB6C(u8* ptr, u16 idx);
extern void* fn_8012ABA0(u8* ptr, u16 idx);
extern void* fn_8012ABD4(u8* ptr, u16 idx);
extern u32 fn_8012AC54(void* ptr);

#if 0
asm void fn_80115BD8(void) {
#include "src/game/gs_field_world_fn_80115BD8.inc"
}
#else
void* fn_80115BD8(void) {
    /* refs: lbl_802726D4, lbl_8035B8A0, lbl_80478FB8, lbl_80478FBC */
    void* floorId;
    u8* entry;
    u32 count;

    floorId = fn_800FF56C();
    entry = (u8*)lbl_80478FBC;
    for (count = *(u32*)lbl_80478FB8; count != 0; count--) {
        if (*(u32*)(entry + 0xC) == (u32)floorId) {
            return entry;
        }
        entry += 0x4C;
    }
    fn_800DD970((const char*)lbl_802726D4, (const char*)lbl_8035B8A0);
    entry = (u8*)0;
    return (void*)entry;
}
#endif
/* 0x80115CB4 | 0xB0 */
extern u32 lbl_80478EBC;
extern u32 lbl_80478EB8;
#if 0
asm void fn_80115CB4(void) {
#include "src/game/gs_field_world_fn_80115CB4.inc"
}
#else
#pragma push
#pragma peephole off
void* fn_80115CB4(u32 param) {
    extern u32 lbl_80478EB8;
    extern u32 lbl_80478EBC;
    u32 r4 = param & 0x7FFF0000;
    u32 r31;
    u8* r30 = (u8*)0;
    u32 r29;
    u32 r28 = 0;
    u32 r27;
    if ((u32)(r4 - 0x7FFF0000) != 0) {
        return (void*)0;
    }
    r27 = param & 0x1FF;
    r29 = 0;
    r31 = 0;
    while (r29 < *(u32*)lbl_80478EB8) {
        r30 = (u8*)lbl_80478EBC + r31;
        if (*(u16*)(r30 + 0x4) == (u32)fn_800FF56C()) {
            if (r27 == r28++) break;
        }
        r31 = r31 + 0x1c;
        r29 = r29 + 0x1;
    }
    if (r29 == *(u32*)lbl_80478EB8) {
        return (void*)0;
    }
    return (void*)r30;
}
#pragma pop
#endif
/* 0x80115D64 | 0xA0 */
extern void fn_80113F48(void);
extern void fn_8018C1E8(void);
extern u32 lbl_80478EBC;
extern u32 lbl_80478EB8;
#if 0
asm void fn_80115D64(void) {
#include "src/game/gs_field_world_fn_80115D64.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80115D64(u32 r25, u32 r26) {
    extern u32 lbl_80478EB8;
    extern u32 lbl_80478EBC;
    extern void* fn_80113F48();
    extern void fn_8018C1E8();
    u32 r27;
    u32 r28 = 0;
    u32 r29 = 0;
    u8* r30 = (u8*)0;
    u32 r31 = 0;
    while (r29 < *(u32*)lbl_80478EB8) {
        r30 = (u8*)lbl_80478EBC + r31;
        if (*(u16*)(r30 + 0x4) == (u32)fn_800FF56C()) {
            if (r29 == r25) {
                r27 = r28 | (0x7fff << 16);
                break;
            }
            r28 = r28 + 0x1;
        }
        r31 = r31 + 0x1c;
        r29 = r29 + 0x1;
    }
    if (r29 != *(u32*)lbl_80478EB8) {
        fn_8018C1E8(fn_80113F48(), r27, r26);
    }
}
#pragma pop
#endif
/* 0x80115E6C | 0x2F8 */
extern void fn_801653CC(void);
extern void fn_80132A38(void);
extern void fn_80106D3C(void);
extern void fn_801069FC(void);
extern void fn_8013467C(void);
extern void fn_8001E184(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80115E6C(void);
/* 0x80116164 | 0x30C */
extern void fn_8018B76C(void);
extern void fn_8018C7C8(void);
extern void fn_801902E0(void);
extern void fn_80166A28(void);
extern void peopleWaitSyncMotion(void);
extern void fn_80190528(void);
extern u32 lbl_80478EBC;
extern u32 lbl_80478EB8;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80116164(void);
/* 0x80116470 | 0x4E8 */
extern void floorGetResource(void);
extern void fn_800ECCA8(void);
extern void fn_800ECA78(void);
extern void fn_800EC9DC(void);
extern void fn_800ECB74(void);
extern void fn_800EC990(void);
extern void _threadSwitch(void);
extern void fn_800EC960(void);
extern void fn_800EE150(void);
extern void GSpartGetTransform(void);
extern void fn_800EE828();
extern void fn_8018AACC(void);
extern void peopleMoveCheck(u32 groupId, u32 index, u8 waitFlag);
extern void fn_8018805C(void);
extern void fn_80184470(void);
extern void fn_8018C0A8(void);
extern void fn_801669BC(void);
extern void fn_800EC1BC(void);
extern void fn_801845E4(void);
extern void fn_801860F8(void);
extern void GSmodelGetFrameCount(void);
extern void fn_800D37CC(void);
extern f32 lbl_8047CFA0;
extern u32 lbl_80478EC8;
extern u32 lbl_80478ECC;
extern f32 lbl_8047CFA4;
extern f32 lbl_8047CFA8;
extern f32 lbl_8047CFAC;
extern f64 lbl_8047CFB8;
extern f32 lbl_8047CFB4;
extern f32 lbl_8047CFB0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80116470(void);
/* 0x80116958 | 0x3D8 */
extern void fn_801CAA08(void);
extern void fn_801903B0(void);
extern void fn_8018C558(void);
extern void fn_8018C8F4(void);
extern f32 lbl_8047CFA0;
extern u32 lbl_80478EC8;
extern u32 lbl_80478ECC;
extern f32 lbl_8047CFAC;
extern f32 lbl_8047CFA4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80116958(void);
/* 0x80116D30 | 0x13C */
extern void EvlogSet__FScUl();
extern void fn_801CA9F8();
extern void fn_800F7434();
extern u8 lbl_80272708[];
extern u32 lbl_80478EC4;
extern u32 lbl_80478EC0;
#if 0
asm void fn_80116D30(void) {
#include "src/game/gs_field_world_fn_80116D30.inc"
}
#else
void fn_80116D30(u32 kind, u32 arg)
{
    s32 skind;
    u32 floor;
    u32 i;
    u32 offset;
    u8* rec;
    u8* text;

    text = lbl_80272708;
    EvlogSet__FScUl(kind, arg, text);
    skind = (s8)kind;

    switch (skind) {
    case 1:
        fn_800DD970((const char*)(text + 0x14), arg);
        break;
    case 2:
        fn_800DD970((const char*)(text + 0x28), arg);
        break;
    case 3:
        fn_800DD970((const char*)(text + 0x3C), arg);
        break;
    case 4:
        fn_800DD970((const char*)(text + 0x50), arg);
        break;
    }

    floor = (u32)fn_800FF56C();
    i = 0;
    offset = 0;
    while (i < *(u32*)lbl_80478EC0) {
        rec = (u8*)(lbl_80478EC4 + offset);
        if (*(u16*)(rec + 2) == floor &&
            (s32)*(u8*)rec == skind &&
            *(u32*)(rec + 4) == arg &&
            *(u32*)(rec + 8) != 0) {
            fn_801CA9F8(arg);
            fn_800F7434(*(u32*)(rec + 8), 4, *(u32*)(rec + 0xC),
                        *(u32*)(rec + 0x10), *(u32*)(rec + 0x14),
                        *(u32*)(rec + 0x18));
        }
        offset += 0x1C;
        i++;
    }
}
#endif
/* 0x80116E6C | 0x18 */
void fn_80116E6C(u8* ptr, u8 val) {
    u8 tmp;
    if (ptr == NULL) { return; }
    tmp = ptr[0];
    tmp = (u8)(((val & 1) << 7) | (tmp & ~0x80));
    ptr[0] = tmp;
}
/* 0x80116E84 | 0x2C */
void floorCharacterBiosSetPos(u8* dst, f32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *(f32*)(&dst[0x18]) = src[0];
    *(f32*)(&dst[0x1C]) = src[1];
    *(f32*)(&dst[0x20]) = src[2];
}
/* 0x80116EB0 | 24 bytes | beq_default_getter */
u32 fn_80116EB0(void* ptr) {
    if (ptr == NULL) goto _ret0_80116EB0;
    return *(u32*)((u8*)ptr + 0x14);
_ret0_80116EB0:
    return 0;
}
/* 0x80116EC8 | 24 bytes | beq_default_getter */
u32 fn_80116EC8(void* ptr) {
    if (ptr == NULL) goto _ret0_80116EC8;
    return *(u32*)((u8*)ptr + 0x10);
_ret0_80116EC8:
    return 0;
}
/* 0x80116EE0 | 24 bytes | beq_default_getter */
u16 fn_80116EE0(void* ptr) {
    if (ptr == NULL) goto _ret0_80116EE0;
    return *(u16*)((u8*)ptr + 0x8);
_ret0_80116EE0:
    return 0;
}
/* 0x80116EF8 | 0x1C */
u32 fn_80116EF8(u8* ptr) {
    if (ptr == NULL) goto _ret0_16EF8;
    return (u32)((ptr[0] >> 4) & 1);
_ret0_16EF8:
    return 0;
}
/* 0x80116F14 | 0x1C */
u32 fn_80116F14(u8* ptr) {
    if (ptr == NULL) goto _ret0_80116F14;
    return (u32)((ptr[1] >> 4) & 3);
_ret0_80116F14:
    return 0;
}
/* 0x80116F30 | 0x1C */
u32 fn_80116F30(u8* ptr) {
    if (ptr == NULL) goto _ret0_80116F30;
    return (u32)((ptr[1] >> 6) & 3);
_ret0_80116F30:
    return 0;
}
/* 0x80116F4C | 0x1C */
u32 fn_80116F4C(u8* ptr) {
    if (ptr == NULL) goto _ret0_80116F4C;
    return (u32)(ptr[0] & 7);
_ret0_80116F4C:
    return 0;
}
/* 0x80117038 | 0x1C */
u32 fn_80117038(u8* ptr) {
    if (ptr == NULL) goto _ret0_80117038;
    return (u32)((ptr[0] >> 6) & 1);
_ret0_80117038:
    return 0;
}
/* 0x80117054 | 0x1C */
u32 fn_80117054(u8* ptr) {
    if (ptr == NULL) goto _ret0_80117054;
    return (u32)((ptr[0] >> 7) & 1);
_ret0_80117054:
    return 0;
}
/* 0x80117070 | 0x34 */
extern void* peopleInfoBiosGetPtrFromIndex(u16);
#pragma push
#pragma scheduling off
void* floorCharacterBiosGetPeopleInfoPtr(u8* ptr) {
    void* sub;
    if (ptr == NULL) { goto ret_null; }
    sub = peopleInfoBiosGetPtrFromIndex(*(u16*)(&ptr[0x6]));
    return sub;
ret_null:
    return NULL;
}
#pragma pop
/* 0x8011711C | 0x38 */
void fn_8011711C(u32 arg) {
    extern void* fn_800FF56C(void);
    extern void* fn_80115C48(u32 key);
    extern void* fn_80115684(void* a, u32 b);
    fn_80115684(fn_80115C48((u32)fn_800FF56C()), arg);
}
/* 0x80117154 | 16 bytes | multi_sda_store */
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
#if 0
asm void fn_80117154(void) {
#include "src/game/gs_field_world_fn_80117154.inc"
}
#else
void fn_80117154(void) {
    lbl_8047AD68 = 0;
    lbl_8047AD6C = 0;
}
#endif
/* 0x64 | fn_80117164 | generic */
extern f32 lbl_8047CFD0;
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern u8 lbl_8047AD70;
extern u8 lbl_8047AD71;
extern f32 lbl_8047AD74;
extern f32 lbl_8047AD78;
extern f32 lbl_8047AD7C;
#if 0
asm void fn_80117164(void) {
#include "src/game/gs_field_world_fn_80117164.inc"
}
#else
void fn_80117164(void) {
    void* result;
    result = fn_801155CC();
    lbl_8047AD68 = 0;
    lbl_8047AD6C = 0;
    lbl_8047AD70 = 0;
    lbl_8047AD71 = 1;
    lbl_8047AD74 = lbl_8047CFD0;
    lbl_8047AD78 = lbl_8047CFD0;
    lbl_8047AD7C = lbl_8047CFD0;
    if (result != NULL) {
        lbl_8047AD68 = *(u32*)(*(u32*)result);
        lbl_8047AD6C = *(u32*)((u8*)result + 4);
    }
}
#endif
/* 0x801171C8 | 0x168 */
extern u8 GSscene_GetMode(void);
extern void fn_80176868(f32);
extern void fn_801767E0(f32);
extern void fn_80176758(f32);
extern void GSscene_GetCameraPositionVector(void*);
extern u8 lbl_8047AD71;
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern f32 lbl_8047AD74;
extern f32 lbl_8047AD78;
extern f32 lbl_8047AD7C;
extern u8 lbl_8047AD70;
extern f32 lbl_8047CFD8;
extern f32 lbl_8047CFD4;
#pragma push
#pragma fp_contract on
#pragma peephole off
#if 0
asm void fn_801171C8(void) {
#include "src/game/gs_field_world_fn_801171C8.inc"
}
#else
void fn_801171C8(void) {
    u8 pos[0xC];
    f32 y;
    f32 x;
    f32 z;

    if (lbl_8047AD71 == 0) { return; }
    if (GSscene_GetMode() != 0) { return; }
    if (lbl_8047AD68 == 0) { return; }

    if (lbl_8047AD68 == 1) {
        f32* ptr = (f32*)lbl_8047AD6C;
        f32 direct_x;
        f32 direct_z;
        direct_z = ptr[5];
        direct_x = ptr[3];
        fn_80176868(ptr[4]);
        fn_801767E0(direct_x);
        fn_80176758(direct_z);
        return;
    }

    GSscene_GetCameraPositionVector(pos);
    x = lbl_8047AD74;
    y = lbl_8047AD78;
    z = lbl_8047AD7C;
    if (floorUpdateFieldCamera(pos, &x, &y, &z) == 0) { return; }

    if (lbl_8047AD70 != 0) {
        f32 f3 = lbl_8047CFD8;
        f32 f2 = x;
        f32 f1 = y;
        f32 f0 = z;
        f32 f4 = f3 * f2;
        f2 = f3 * f1;
        {
            f32 f6 = lbl_8047CFD4;
            f32 f5 = lbl_8047AD74;
            f0 = f3 * f0;
            f3 = lbl_8047AD78;
            f1 = lbl_8047AD7C;
            f4 = f6 * f5 + f4;
            f2 = f6 * f3 + f2;
            f0 = f6 * f1 + f0;
        }
        lbl_8047AD74 = f4;
        lbl_8047AD78 = f2;
        lbl_8047AD7C = f0;
    } else {
        lbl_8047AD74 = x;
        lbl_8047AD78 = y;
        lbl_8047AD7C = z;
        lbl_8047AD70 = 1;
    }

    {
        f32 out_y = lbl_8047AD78;
        f32 out_z = lbl_8047AD7C;
        fn_80176868(lbl_8047AD74);
        fn_801767E0(out_y);
        fn_80176758(out_z);
    }
}
#endif
#pragma pop
/* 0x80117330 | 0x194 */
extern void* fn_800F9318();
extern void fn_800E3D98(void*, void*);
extern void GSscene_GetCameraViewVector(void*);
extern f32 fn_8017669C(void);
extern f32 fn_80176690(void);
extern f32 fn_80176684(void);
extern void fn_800E01F4(void* obj, f32 f1, f32 f2, f32 f3);
extern void fn_800E0518(void*, f32);
extern void fn_800DFF98(void*, void*, void*);
extern void fn_800E019C(void*, void*, void*);
extern f64 fn_800CE2D8(f32, f32);
extern void fn_801776E8(u32, void*, f32);
extern void fn_80177574(u32, void*, f32);
extern void fn_80177478(u32, void*, f32);
extern u8 lbl_8047AD71;
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern f32 lbl_8047CFD0;
#if 0
asm void fn_80117330(void) {
#include "src/game/gs_field_world_fn_80117330.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80117330(f32 arg) {
    extern u8 GSscene_GetMode(void);
    extern void GSscene_GetCameraPositionVector(void*);
    u8 tmp[0x30];
    u8 pos[0xC];
    u8 view[0xC];
    u8 rot[0xC];
    u8 mat[0x18];
    f32 y;
    f32 x;
    f32 z;
    void* obj;

    if (lbl_8047AD71 == 0) { return; }
    if (GSscene_GetMode() != 0) { return; }
    if (lbl_8047AD68 == 0) { return; }

    obj = fn_800F9318(0, 0x64);
    if (obj != NULL) {
        fn_800E3D98(obj, pos);
    } else {
        GSscene_GetCameraPositionVector(pos);
    }
    GSscene_GetCameraViewVector(view);

    if (lbl_8047AD68 == 1) {
        f32* ptr = (f32*)lbl_8047AD6C;
        x = ptr[4];
        y = ptr[3];
        z = ptr[5];
    } else {
        x = fn_8017669C();
        y = fn_80176690();
        z = fn_80176684();
        if (floorUpdateFieldCamera(pos, &x, &y, &z) == 0) { return; }
    }

    fn_800E01F4(mat, lbl_8047CFD0, x, y);
    fn_800E0518(tmp, z);
    fn_800DFF98(mat, tmp, mat);
    fn_800E019C(rot, pos, view);
    fn_800E019C(rot, rot, mat);
    *(f32*)(&mat[0x10]) = z;
    *(f32*)(&mat[0xC]) = -(f32)fn_800CE2D8(x, y);
    *(f32*)(&mat[0x14]) = lbl_8047CFD0;
    fn_801776E8(0, pos, arg);
    fn_80177574(0, rot, arg);
    fn_80177478(0, &mat[0xC], arg);
}
#pragma pop
#endif
/* 0x801174C4 | 0x28 */
u32 fn_801174C4(void) {
    u8 result = 0;
    if (lbl_8047AD68 != 0 && lbl_8047AD6C != 0) {
        result = 1;
    }
    return result;
}
/* 0x801174F4 | 0xC */
extern u8 lbl_8047AD71;
#if 0
asm void fn_801174F4(void) {
#include "src/game/gs_field_world_fn_801174F4.inc"
}
#else
void fn_801174F4(void) {
    lbl_8047AD71 = 0;
}
#endif
/* 0x80117500 | 0x14 */
extern u8 lbl_8047AD70;
extern u8 lbl_8047AD71;
extern u8 lbl_8047AD70;
#if 0
asm void fn_80117500(void) {
#include "src/game/gs_field_world_fn_80117500.inc"
}
#else
void fn_80117500(void) {
    lbl_8047AD71 = 1;
    lbl_8047AD70 = 0;
}
#endif
/* 0x801176C8 | 0x254 */
extern void GSgappTerminate(void);
extern void fn_800FE834(void);
extern u8 lbl_804083D0[0x30];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801176C8(void);
/* 0x8011791C | 0x1B8 */
extern void* GScameraGetActiveCamera();
extern void fn_800E01D0();
extern u8 lbl_804083D0[0x30];
extern u8 lbl_802727B8[];
extern void fn_80177A38(void);  /* referenced by asm .inc wrappers (fn_801171C8/80117330/8011791C/8012E388/8012EBD4); was undefined -> broke the TU parse */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011791C(void);
typedef struct GSFieldWorldResourceState {
    u8 unk_00[0x10];
    u32 field_10; /* 0x10 */
} GSFieldWorldResourceState;

/* 0x80117AD4 | 16 bytes | global_getter */
u32 fn_80117AD4(void) {
    return ((GSFieldWorldResourceState*)lbl_804083D0)->field_10;
}
/* 0x80117AE4 | 0x1A0 */
extern void fn_800E5550(void);
extern void fn_800EF5A4(void);
extern void fn_800E4BF4(void);
extern void GStextureCreate(void);
extern void fn_80113D34(void);
extern void GSmodelSetVisibility(void);
extern void fn_800EC188(void);
extern void fn_800E563C(void);
extern u32 lbl_80478B40;
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD94;
extern u32 lbl_8047AD80;
extern u32 lbl_8047AD84;
#pragma push
#pragma peephole off
#if 0
asm void fn_80117AE4(void) {
#include "src/game/gs_field_world_fn_80117AE4.inc"
}
#else
u8 fn_80117AE4(u32 arg1) {
    extern u32 fn_80113F48(void);
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E5550(void* a);
    extern void fn_800EF5A4(void* a);
    extern void fn_800E4BF4(void* a);
    extern void* GStextureCreate(u16 a, u16 b, u32 c, u32 d, u32 e);
    extern void* fn_80113D34(u32 a, u32 b);
    extern void GSmodelSetVisibility(void* a, u32 b);
    extern void fn_800EC188(void* a, u32 b);
    extern void fn_800ECCA8(void* a, u32 b);
    extern void fn_800EC990(void* a);
    extern void fn_800E563C(void* a, void* b);
    u32 count;
    u8 found;

    if ((s32)lbl_80478B40 == (s32)arg1) { return; }

    if (lbl_8047AD88 != 0) {
        fn_800E5550(fn_800F9318(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 8)));
        if (lbl_8047AD8C != 0) {
            fn_800EF5A4((void*)lbl_8047AD8C);
            lbl_8047AD8C = 0;
        }
        if (lbl_8047AD90 != 0) {
            fn_800E4BF4((void*)lbl_8047AD90);
            lbl_8047AD90 = 0;
        }
        lbl_8047AD94 = 0;
        lbl_80478B40 = (u32)-1;
    }

    count = lbl_8047AD84;
    found = 0;
    lbl_8047AD88 = lbl_8047AD80;
    while (count != 0) {
        u8* e = (u8*)lbl_8047AD88;
        if (*(u32*)(e + 4) == arg1) {
            found = 1;
            break;
        }
        lbl_8047AD88 = (u32)e + 0x18;
        count--;
    }
    if (!found) {
        lbl_8047AD88 = 0;
        return 0;
    }

    lbl_8047AD8C = (u32)GStextureCreate(*(u16*)((u8*)lbl_8047AD88 + 0), *(u16*)((u8*)lbl_8047AD88 + 2), 0x44, 0, 0);
    if (lbl_8047AD8C == 0) {
        lbl_8047AD88 = 0;
        return 0;
    }
    lbl_8047AD90 = (u32)fn_80113D34(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 0xc));
    GSmodelSetVisibility((void*)lbl_8047AD90, 0);
    fn_800EC188((void*)lbl_8047AD90, 1);
    fn_800ECCA8((void*)lbl_8047AD90, *(u32*)((u8*)lbl_8047AD88 + 0x10));
    fn_800EC990((void*)lbl_8047AD90);
    lbl_8047AD94 = (u32)fn_800F9318(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 0x14));
    fn_800E563C(fn_800F9318(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 8)), (void*)lbl_8047AD8C);
    lbl_80478B40 = arg1;
    return 1;
}
#endif
#pragma pop
/* 0x80117C84 | 0x90 */
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD94;
extern u32 lbl_80478B40;
extern u32 lbl_8047AD80;
extern u32 lbl_8047AD84;
#if 0
asm void fn_80117C84(void) {
#include "src/game/gs_field_world_fn_80117C84.inc"
}
#else
#pragma peephole off
void fn_80117C84(void) {
    extern u32 fn_80113F48(void);
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E5550(void* a);
    extern void fn_800EF5A4(void* a);
    extern void fn_800E4BF4(void* a);
    u8* ptr = (u8*)lbl_8047AD88;
    if (ptr != NULL) {
        fn_800E5550(fn_800F9318(fn_80113F48(), *(u32*)(ptr + 8)));
        if (lbl_8047AD8C != 0) {
            fn_800EF5A4((void*)lbl_8047AD8C);
            lbl_8047AD8C = 0;
        }
        if (lbl_8047AD90 != 0) {
            fn_800E4BF4((void*)lbl_8047AD90);
            lbl_8047AD90 = 0;
        }
        lbl_8047AD94 = 0;
        lbl_8047AD88 = 0;
        lbl_80478B40 = (u32)-1;
    }
    lbl_8047AD80 = 0;
    lbl_8047AD84 = 0;
}
#pragma peephole on
#endif
/* 0x80117D14 | 0x144 */
extern void fn_800EC134(u32);
extern void fn_800D4604();
extern void fn_800D377C();
extern void fn_800D3410();
extern void fn_800D9B24();
extern void fn_800D9AF0();
extern void fn_800D258C();
extern void fn_800D9D68();
extern void fn_800D9C24();
extern void _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID(void);
extern void fn_800E3760();
extern void fn_800D3190(void);
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD94;
#if 0
asm void fn_80117D14(void) {
#include "src/game/gs_field_world_fn_80117D14.inc"
}
#else
#pragma push
#pragma peephole off
void fn_80117D14(void)
{
    u16 a0;
    u16 a1;
    u16 a2;
    u16 a3;
    u16 b0;
    u16 b1;
    u16 b2;
    u16 b3;
    u32 saved;
    u8* size;

    if (lbl_8047AD88 != 0) {
        if (lbl_8047AD90 != 0) {
            fn_800EC134(lbl_8047AD90);
            saved = (u32)GScameraGetActiveCamera();
            fn_800D4604(2);
            fn_800D377C(1);
            fn_800D3410(lbl_8047AD8C, 0);
            fn_800D9B24(&a0, &a1, &a2, &a3);
            fn_800D9AF0(&b0, &b1, &b2, &b3);
            fn_800D258C(lbl_8047AD94);

            size = (u8*)lbl_8047AD88;
            fn_800D9D68(0, 0,
                        (u16)(*(u16*)(size + 0) - 1),
                        (u16)(*(u16*)(size + 2) - 1));
            size = (u8*)lbl_8047AD88;
            fn_800D9C24(0, 0,
                        (u16)(*(u16*)(size + 0) - 1),
                        (u16)(*(u16*)(size + 2) - 1));
            _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID();
            fn_800E3760(lbl_8047AD90, 0x3010);
            fn_800D3190();
            fn_800D377C(1);
            fn_800D258C(saved);
            fn_800D9D68(a0, a1, a2, a3);
            fn_800D9C24(b0, b1, b2, b3);
            _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID();
            fn_800D4604(1);
        }
    }
}
#pragma pop
#endif
/* 0x80117E58 | 0x1C8 */
extern u32 lbl_8047AD80;
extern u32 lbl_8047AD84;
extern u32 lbl_80478B40;
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD94;
#pragma push
#pragma peephole off
#if 0
asm void fn_80117E58(void) {
#include "src/game/gs_field_world_fn_80117E58.inc"
}
#else
void fn_80117E58(void* arg) {
    extern u32 fn_80113F48(void);
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E5550(void* a);
    extern void fn_800EF5A4(void* a);
    extern void fn_800E4BF4(void* a);
    extern void* GStextureCreate(u16 a, u16 b, u32 c, u32 d, u32 e);
    extern void* fn_80113D34(u32 a, u32 b);
    extern void GSmodelSetVisibility(void* a, u32 b);
    extern void fn_800EC188(void* a, u32 b);
    extern void fn_800ECCA8(void* a, u32 b);
    extern void fn_800EC990(void* a);
    extern void fn_800E563C(void* a, void* b);
    u8* p48;
    u8* ptr;
    u32 count;
    u8 found;

    p48 = *(u8**)((u8*)arg + 0x48);
    if (p48 == NULL) { return; }
    lbl_8047AD80 = *(u32*)(*(u8**)p48 + 0x4);
    if (lbl_8047AD80 == 0) { return; }
    lbl_8047AD84 = *(u32*)(*(u8**)(*(u8**)(*(u8**)((u8*)arg + 0x48))) + 0x0);
    if (lbl_8047AD84 == 0) { return; }
    if ((s32)lbl_80478B40 == 0) { return; }

    ptr = (u8*)lbl_8047AD88;
    if (ptr != NULL) {
        fn_800E5550(fn_800F9318(fn_80113F48(), *(u32*)(ptr + 8)));
        if (lbl_8047AD8C != 0) {
            fn_800EF5A4((void*)lbl_8047AD8C);
            lbl_8047AD8C = 0;
        }
        if (lbl_8047AD90 != 0) {
            fn_800E4BF4((void*)lbl_8047AD90);
            lbl_8047AD90 = 0;
        }
        lbl_8047AD94 = 0;
        lbl_80478B40 = (u32)-1;
    }

    count = lbl_8047AD84;
    found = 0;
    lbl_8047AD88 = lbl_8047AD80;
    while (count != 0) {
        u8* e = (u8*)lbl_8047AD88;
        if (*(u32*)(e + 4) == 0) {
            found = 1;
            break;
        }
        lbl_8047AD88 = (u32)e + 0x18;
        count--;
    }
    if (!found) {
        lbl_8047AD88 = 0;
        return;
    }

    lbl_8047AD8C = (u32)GStextureCreate(*(u16*)((u8*)lbl_8047AD88 + 0), *(u16*)((u8*)lbl_8047AD88 + 2), 0x44, 0, 0);
    if (lbl_8047AD8C == 0) {
        lbl_8047AD88 = 0;
        return;
    }
    lbl_8047AD90 = (u32)fn_80113D34(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 0xc));
    GSmodelSetVisibility((void*)lbl_8047AD90, 0);
    fn_800EC188((void*)lbl_8047AD90, 1);
    fn_800ECCA8((void*)lbl_8047AD90, *(u32*)((u8*)lbl_8047AD88 + 0x10));
    fn_800EC990((void*)lbl_8047AD90);
    lbl_8047AD94 = (u32)fn_800F9318(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 0x14));
    fn_800E563C(fn_800F9318(fn_80113F48(), *(u32*)((u8*)lbl_8047AD88 + 8)), (void*)lbl_8047AD8C);
    lbl_80478B40 = 0;
}
#endif
#pragma pop
/* 0x48 | fn_80118020 | single_call_straight */
extern void fn_800FF4D4(void);
extern u32 lbl_802727C8[];
#if 0
asm void fn_80118020(void) {
#include "src/game/gs_field_world_fn_80118020.inc"
}
#else
#pragma peephole off
void fn_80118020(void) {
    extern void fn_800FF4D4(void* ptr, u32 val);
    u32 local[3];
    local[0] = lbl_802727C8[0];
    local[1] = lbl_802727C8[1];
    local[2] = lbl_802727C8[2];
    fn_800FF4D4(local, 1);
}
#pragma peephole on
#endif
/* 0x80118070 | 0x90 */
extern u32 lbl_8047AD88;
extern u32 lbl_8047AD8C;
extern u32 lbl_8047AD90;
extern u32 lbl_8047AD94;
extern u32 lbl_80478B40;
extern u32 lbl_8047AD80;
extern u32 lbl_8047AD84;
#if 0
asm void fn_80118070(void) {
#include "src/game/gs_field_world_fn_80118070.inc"
}
#else
#pragma peephole off
void fn_80118070(void) {
    extern u32 fn_80113F48(void);
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E5550(void* a);
    extern void fn_800EF5A4(void* a);
    extern void fn_800E4BF4(void* a);
    u8* ptr = (u8*)lbl_8047AD88;
    if (ptr != NULL) {
        fn_800E5550(fn_800F9318(fn_80113F48(), *(u32*)(ptr + 8)));
        if (lbl_8047AD8C != 0) {
            fn_800EF5A4((void*)lbl_8047AD8C);
            lbl_8047AD8C = 0;
        }
        if (lbl_8047AD90 != 0) {
            fn_800E4BF4((void*)lbl_8047AD90);
            lbl_8047AD90 = 0;
        }
        lbl_8047AD94 = 0;
        lbl_8047AD88 = 0;
        lbl_80478B40 = (u32)-1;
    }
    lbl_8047AD80 = 0;
    lbl_8047AD84 = 0;
}
#pragma peephole on
#endif
/* 0x80118100 | 0x4 | void_stub */
#if 0
asm void fn_80118100(void) {
#include "src/game/gs_field_world_fn_80118100.inc"
}
#else
#pragma optimization_level 4
void fn_80118100(void) {
}
#endif
/* 0x80118104 | 0xAC */
extern void psSetBillboardCamera();
extern void fn_8016AB94();
#if 0
asm void fn_80118104(void) {
#include "src/game/gs_field_world_fn_80118104.inc"
}
#else
#pragma optimization_level 4
void fn_80118104(u32 a, u8 b) {
    void* result;
    u32 val;

    result = GScameraGetActiveCamera();
    if (result != NULL) {
        psSetBillboardCamera(*(void**)((u8*)result + 0xC));
        switch (a) {
        case 0x10:
            val = 0;
            break;
        case 0x1000:
            val = 1;
            break;
        case 0x2000:
            val = 2;
            break;
        }
        if ((u8)b == 0) {
            fn_8016AB94(1, val);
        } else {
            fn_8016AB94(2, val);
        }
    }
}
#endif
/* 0x801181B0 | 0x23C */
extern void psGetParticleChildCount(void* ptr);
extern void psKillFamily();
extern void fn_800EC160();
extern void fn_80169484();
extern void psKillGenerator();
extern u32 lbl_8047AD9C;
extern u32 lbl_8047ADA0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801181B0(void);
/* 0x801183EC | 0x488 */
extern void fn_800E06EC(void);
extern void fn_800DFEEC(void);
extern void fn_800E0108(void);
extern void psInterpretParticles(void);
extern void psExecGenerator(void);
extern void fn_800057A0(void);
extern void jumptable_8035BB88();
extern u32 lbl_8047AD9C;
extern u32 lbl_8047ADA0;
extern u8 lbl_8047ADB0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801183EC(void);
/* 0x80118874 | 0x1F4 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80118874(void);
/* 0x80118A68 | 0x1B8 */
#if 0
asm void fn_80118A68(void) {
#include "src/game/gs_field_world_fn_80118A68.inc"
}
#else
void fn_80118A68(u8* obj, u32 notify) {
    u32 i;
    u8* model;
    u8* base;
    u8* scan;
    s32* active;

    if ((notify & 0xFF) == 1) {
        model = *(u8**)(obj + 0x10);
        psKillFamily(*(u16*)(model + 0x18), model[0x15]);
    }

    active = (s32*)(obj + 0x44);
    if (*active != 0 && *active != 0) {
        fn_800EC160(*(u32*)(obj + 0x48), 0);
        *(u32*)(obj + 0x48) = 0;
        *(u32*)(obj + 0x4C) = 0;
        obj[6] = 0;
        obj[5] = 0;
        fn_80169484(*(u32*)(obj + 0x10));
        *active = 0;

        if (*active == 0) {
            fn_800E01D0(obj + 0x14, obj + 0x50);
            *(f32*)(*(u8**)(obj + 0x10) + 0x20) = *(f32*)(obj + 0x50);
            *(f32*)(*(u8**)(obj + 0x10) + 0x24) = *(f32*)(obj + 0x54);
            *(f32*)(*(u8**)(obj + 0x10) + 0x28) = *(f32*)(obj + 0x58);
        } else {
            fn_800E01D0(obj + 0x50, obj + 0x50);
        }

        if (*active == 0) {
            fn_800E01D0(obj + 0x20, obj + 0x5C);
            *(f32*)(*(u8**)(obj + 0x10) + 0x8C) = *(f32*)(obj + 0x5C);
            *(f32*)(*(u8**)(obj + 0x10) + 0x90) = *(f32*)(obj + 0x60);
            *(f32*)(*(u8**)(obj + 0x10) + 0x94) = *(f32*)(obj + 0x64);
        } else {
            fn_800E01D0(obj + 0x5C, obj + 0x5C);
        }

        if (*active == 0) {
            fn_800E01D0(obj + 0x2C, obj + 0x68);
            *(f32*)(*(u8**)(obj + 0x10) + 0x98) = *(f32*)(obj + 0x68);
            *(f32*)(*(u8**)(obj + 0x10) + 0x9C) = *(f32*)(obj + 0x6C);
            *(f32*)(*(u8**)(obj + 0x10) + 0xA0) = *(f32*)(obj + 0x70);
        } else {
            fn_800E01D0(obj + 0x68, obj + 0x68);
        }
    }

    psKillGenerator(*(u32*)(obj + 0x10));

    base = *(u8**)(obj + 0x0C);
    scan = base;
    for (i = 0; i < 0x40; i++) {
        if (*(u32*)(scan + 8) == (u32)obj) {
            *(u32*)(scan + 8) = 0;
            break;
        }
        scan += 4;
    }

    obj[0] = 0;
}
#endif
/* 0x68 | fn_80118C20 | guarded_call */
extern void psSetParticleVisibility();  /* K&R: called with 0 or 1 args */
void fn_80118C20(u8* arg1, void* arg2, u32 arg3, u32 arg4, u32 arg5) {
    u8* scan = arg1;
    u32 i = 0;
    for (i = 0; i < 0x40; i++, scan += 4) {
        void* entry = *(void**)(scan + 8);
        if (entry) {
            psSetParticleVisibility(*(void**)((u8*)entry + 0x10), arg2);
        }
    }
}
/* 0x80118C88 | 36 bytes | load_then_call */
void fn_80118C88(void* obj) {
    extern void psSetParticleVisibility();
    psSetParticleVisibility(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CAC | 36 bytes | load_then_call */
void fn_80118CAC(void* obj) {
    fn_80169340(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CD0 | 36 bytes | load_then_call */
void fn_80118CD0(void* obj) {
    fn_80169368(*(void**)((u8*)obj + 0x10));
}
/* 0x80118CF4 | 36 bytes | load_then_call */
void fn_80118CF4(void* obj) {
    psSetTornadoScaling(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D18 | 36 bytes | load_then_call */
void fn_80118D18(void* obj) {
    fn_801693DC(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D3C | 36 bytes | load_then_call */
void fn_80118D3C(void* obj) {
    psSetOffsetRotationInLocal(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D60 | 36 bytes | load_then_call */
void fn_80118D60(void* obj) {
    fn_8016945C(*(void**)((u8*)obj + 0x10));
}
/* 0x80118D84 | 36 bytes | load_then_call */
void fn_80118D84(void* obj) {
    psGetParticleChildCount(*(void**)((u8*)obj + 0x10));
}
/* 0x80118DA8 | 0x38 */
extern s32 psGetGeneratorChildMaxLife(u32);
s32 fn_80118DA8(u8* ptr) {
    if (ptr[1] == 1) { return -1; }
    return psGetGeneratorChildMaxLife(*(u32*)(&ptr[0x10]));
}
/* 0x80118DE0 | 0xAC */
extern void psSetGeneratorAngleRadiusScale(void);
#if 0
asm void fn_80118DE0(void) {
#include "src/game/gs_field_world_fn_80118DE0.inc"
}
#else
void fn_80118DE0(u8* arg1, f32* arg2, u32 arg3, u32 arg4) {
    extern void fn_800E01D0();
    extern void psSetGeneratorAngleRadiusScale();
    if ((s32)*(u32*)(arg1 + 0x44) == 0) {
        fn_800E01D0(arg1 + 0x2c);
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x98) = arg2[0];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x9c) = arg2[1];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0xa0) = arg2[2];
    } else {
        fn_800E01D0(arg1 + 0x68);
    }
    if ((arg3 & 0xFF) == 1) {
        psSetGeneratorAngleRadiusScale(*(void**)(arg1 + 0x10), arg2, (void*)arg4);
    }
}
#endif
/* 0x78 | fn_80118E8C | two_call_arg_check */
void fn_80118E8C(u8* arg1, f32* arg2, u32 arg3, u32 arg4, u32 arg5) {
    if ((s32)*(u32*)(arg1 + 0x44) == 0) {
        fn_800E01D0(arg1 + 0x20);
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x8c) = arg2[0];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x90) = arg2[1];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x94) = arg2[2];
    } else {
        fn_800E01D0(arg1 + 0x5c);
    }
}
/* 0x78 | fn_80118F04 | two_call_arg_check */
void fn_80118F04(u8* arg1, f32* arg2, u32 arg3, u32 arg4, u32 arg5) {
    if ((s32)*(u32*)(arg1 + 0x44) == 0) {
        fn_800E01D0(arg1 + 0x14);
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x20) = arg2[0];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x24) = arg2[1];
        *(f32*)(*(u8**)(arg1 + 0x10) + 0x28) = arg2[2];
    } else {
        fn_800E01D0(arg1 + 0x50);
    }
}
/* 0x80118F7C | 0x34 */
void fn_80118F7C(u8* obj, void* arg) {
    f32 f1 = *(f32*)(&obj[0x38]);
    f32 f2 = *(f32*)(&obj[0x3C]);
    f32 f3 = *(f32*)(&obj[0x40]);
    fn_800E01F4(arg, f1, f2, f3);
}
/* 0x80118FB0 | 0x12C */
extern void fn_80169494(void);
extern f32 lbl_8047CFE8;
extern f32 lbl_8047CFEC;
#if 0
asm void fn_80118FB0(void) {
#include "src/game/gs_field_world_fn_80118FB0.inc"
}
#else
void fn_80118FB0(u8* obj, u8* desc, u32 state, u32 byte5, u32 init_from_zero, u32 attach_model) {
    extern void fn_800E01D0(void* dst, void* src);
    extern void fn_800E01F4(void* dst, f32 x, f32 y, f32 z);
    extern void fn_80169494(u32 model, u32 value);
    f32 zero;
    f32 one;

    if (*(s32*)(obj + 0x44) == 0 && (s32)state != 0) {
        *(u32*)(obj + 0x48) = *(u32*)(desc + 0x4);
        *(u32*)(obj + 0x4C) = *(u16*)(desc + 0x2);
        if ((u8)init_from_zero == 1) {
            zero = lbl_8047CFE8;
            fn_800E01F4(obj + 0x50, zero, zero, zero);
            zero = lbl_8047CFE8;
            fn_800E01F4(obj + 0x5C, zero, zero, zero);
            one = lbl_8047CFEC;
            fn_800E01F4(obj + 0x68, one, one, one);
        } else {
            fn_800E01D0(obj + 0x50, obj + 0x14);
            fn_800E01D0(obj + 0x5C, obj + 0x20);
            fn_800E01D0(obj + 0x68, obj + 0x2C);
        }
        zero = lbl_8047CFE8;
        fn_800E01F4(obj + 0x14, zero, zero, zero);
        zero = lbl_8047CFE8;
        fn_800E01F4(obj + 0x20, zero, zero, zero);
        one = lbl_8047CFEC;
        fn_800E01F4(obj + 0x2C, one, one, one);
        if ((u8)attach_model != 0) {
            fn_80169494(*(u32*)(obj + 0x10), *(u32*)(desc + 0x8));
        }
        *(u32*)(obj + 0x44) = state;
        obj[5] = (u8)byte5;
        obj[6] = 1;
    }
}
#endif
/* 0x801190DC | 0x2E0 */
extern void fn_80173718(void);
extern void fn_800D3094(void);
extern u32 lbl_8047ADAC;
extern u32 lbl_8047ADA8;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801190DC(void);
/* 0x801193BC | 0x1F0 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801193BC(void);
/* 0x801195AC | 0x278 */
extern void psInitDataBank(void);
extern void DCFlushRange();
extern u8 lbl_802727D8[];
extern u32 lbl_8047AD9C;
extern u32 lbl_8047ADA0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801195AC(void);
/* 0x80119824 | 0x10C */
extern void fn_8016A01C(void);
extern void psInitGenerator(void);
extern void fn_8016AAF4(void);
extern void fn_8019733C(void);
extern void fn_8019D618(void);
extern void psSetPointJObjNodup(void);
extern void fn_8019D610(void);
extern u32 lbl_8047ADA0;
extern u16 lbl_8047AD98;
extern u32 lbl_8047AD9C;
extern u32 lbl_8047ADAC;
extern u16 lbl_8047ADA4;
extern u32 lbl_8047ADA8;
#if 1
void fn_80119824(u32 count1, u32 count2) {
    extern u16 lbl_8047AD98;
    extern u32 lbl_8047AD9C;
    extern u32 lbl_8047ADA0;
    extern u16 lbl_8047ADA4;
    extern u32 lbl_8047ADA8;
    extern u32 lbl_8047ADAC;
    extern u16 _toolentryAlloc__FUl(u32 size);
    extern void* fn_800E27B0(u16 handle);
    extern void psInitParticle(s32 a);
    extern void psInitGenerator(s32 a);
    extern void psInitAppSRT(s32 a, s32 b);
    extern void fn_8019733C(void* fp);
    extern void fn_8019D618(void* fp);
    extern void fn_8019D610(void* fp);
    extern void fn_80119BD0(void);
    extern void psSetPointJObjNodup(void);
    u32 h;
    u32 i;
    lbl_8047ADA0 = count1;
    h = _toolentryAlloc__FUl(count1 * 0x108);
    lbl_8047AD98 = (u16)h;
    if ((u16)h == 0) { return; }
    lbl_8047AD9C = (u32)fn_800E27B0((u16)h);
    for (i = 0; i < lbl_8047ADA0; i++) {
        *(u8*)(lbl_8047AD9C + i * 0x108) = 0;
    }
    lbl_8047ADAC = count2;
    h = _toolentryAlloc__FUl(count2 * 0x74);
    lbl_8047ADA4 = (u16)h;
    if ((u16)h == 0) { return; }
    lbl_8047ADA8 = (u32)fn_800E27B0((u16)h);
    for (i = 0; i < lbl_8047ADAC; i++) {
        *(u8*)(lbl_8047ADA8 + i * 0x74) = 0;
    }
    psInitParticle(0);
    psInitGenerator(0);
    psInitAppSRT(0, 0x74);
    fn_8019733C(fn_80119BD0);
    fn_8019D618(fn_80119BD0);
    fn_8019D610(psSetPointJObjNodup);
}
#else
void fn_80119824(void) {
    extern u16 lbl_8047AD98;
    extern u32 lbl_8047AD9C;
    extern u32 lbl_8047ADA0;
    extern u16 lbl_8047ADA4;
    extern u32 lbl_8047ADA8;
    extern u32 lbl_8047ADAC;
    extern void fn_8016A01C();
    extern void fn_8016AAF4();
    extern void psInitGenerator();
    extern void fn_8019733C();
    extern void fn_8019D610();
    extern void fn_8019D618();
    extern void fn_80119BD0();
    extern void psSetPointJObjNodup();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    r0 = r3;
    r3 = r0 * 0x108;
    r31 = r4;
    lbl_8047ADA0 = r0;
    ((void(*)(void))_toolentryAlloc__FUl)();
    r0 = r3 & 0xFFFF;
    lbl_8047AD98 = r3;
    if ((s32)r0 == (s32)0) return;
    r3 = r0;
    ((void(*)(void))fn_800E27B0)();
    r5 = 0x0;
    lbl_8047AD9C = r3;
    r4 = r5;
    r6 = 0x0;
    while (r0 = lbl_8047ADA0, r6 < r0) {

    r3 = lbl_8047AD9C;
    r6 = r6 + 0x1;
    *(u8*)(r3 + r5) = r4;
    r5 = r5 + 0x108;
    }

    r3 = r31 * 0x74;
    lbl_8047ADAC = r31;
    ((void(*)(void))_toolentryAlloc__FUl)();
    r0 = r3 & 0xFFFF;
    lbl_8047ADA4 = r3;
    if (r6 == (u32)r0) return;
    r3 = r0;
    ((void(*)(void))fn_800E27B0)();
    r6 = 0x0;
    lbl_8047ADA8 = r3;
    r5 = r6;
    r4 = r6;
    while (r0 = lbl_8047ADAC, r6 < r0) {

    r3 = lbl_8047ADA8;
    r6 = r6 + 0x1;
    *(u8*)(r3 + r5) = r4;
    r5 = r5 + 0x74;
    }

    r3 = 0x0;
    fn_8016A01C();
    r3 = 0x0;
    psInitGenerator();
    r3 = 0x0;
    r4 = 0x74;
    fn_8016AAF4();
    r3 = (u32)fn_80119BD0;
    r3 = (u32)fn_80119BD0;
    fn_8019733C();
    r3 = (u32)fn_80119BD0;
    r3 = (u32)fn_80119BD0;
    fn_8019D618();
    r3 = (u32)psSetPointJObjNodup;
    r3 = (u32)psSetPointJObjNodup;
    fn_8019D610();
    return;
}
#endif
/* 0x80119930 | 0x2A0 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80119930(void);
/* 0x80119BD0 | 0x1C0 */
extern void fn_800E3B6C(void);
extern void fn_800E3BF8(void);
extern void fn_800E6DC0(void);
extern void GSpartGetJObjIndex(void);
extern void fn_800E3CBC(void);
extern void fn_800E3BF0(void);
extern void fn_800E3D08(void);
extern void psSetParticleVisibility();
extern f32 lbl_8047CFE8;
extern f32 lbl_8047CFEC;
#if 0
asm void fn_80119BD0(void) {
#include "src/game/gs_field_world_fn_80119BD0.inc"
}
#else
void fn_80119BD0(u32 arg1, u32 arg2, u32 arg5, u8* arg6) {
    extern u8* fn_800E3B6C();
    extern u8* fn_800E3BF8();
    extern u32 fn_800E6DC0();
    extern u32 GSpartGetJObjIndex();
    extern u8* fn_800EE150();
    extern u32 fn_800E3CBC();
    extern u8* fn_801190DC();
    extern u32 fn_800E3BF0();
    extern u32 fn_800E3D08();
    u8* node = arg6;
    u8* resource;
    u8* texture;
    u8* entry;
    u32 index;
    s32 handle;

    while ((node != NULL ? *(u8**)(node + 0x0C) : NULL) != NULL) {
        if (node == NULL) {
            node = NULL;
        } else {
            node = *(u8**)(node + 0x0C);
        }
    }

    resource = fn_800E3B6C(node);
    if (resource == NULL) {
        return;
    }

    texture = fn_800E3BF8(resource);
    if (texture == NULL) {
        return;
    }

    if ((fn_800E6DC0(resource) & 0xFF) != 0) {
        node = *(u8**)(node + 0x10);
    }

    index = GSpartGetJObjIndex(node, arg6);
    if (index == 0xFFFFFFFF) {
        return;
    }

    entry = fn_800EE150(resource, index);
    if (entry == NULL) {
        return;
    }

    node = fn_801190DC(texture, arg5, fn_800E3CBC(resource));
    if (node != NULL) {
        handle = fn_800E3BF0(resource);
        if (*(s32*)(node + 0x44) == 0 && handle != 0) {
            *(u32*)(node + 0x48) = *(u32*)(entry + 0x04);
            *(u32*)(node + 0x4C) = *(u16*)(entry + 0x02);
            fn_800E01F4(node + 0x50, lbl_8047CFE8, lbl_8047CFE8, lbl_8047CFE8);
            fn_800E01F4(node + 0x5C, lbl_8047CFE8, lbl_8047CFE8, lbl_8047CFE8);
            fn_800E01F4(node + 0x68, lbl_8047CFEC, lbl_8047CFEC, lbl_8047CFEC);
            fn_800E01F4(node + 0x14, lbl_8047CFE8, lbl_8047CFE8, lbl_8047CFE8);
            fn_800E01F4(node + 0x20, lbl_8047CFE8, lbl_8047CFE8, lbl_8047CFE8);
            fn_800E01F4(node + 0x2C, lbl_8047CFEC, lbl_8047CFEC, lbl_8047CFEC);
            *(u32*)(node + 0x44) = handle;
            *(u8*)(node + 0x05) = 0;
            *(u8*)(node + 0x06) = 1;
        }
    }

    fn_800EE828(entry);
    if ((fn_800E3D08(resource) & 0xFF) == 0) {
        psSetParticleVisibility(*(void**)(node + 0x10), 0);
    }
}
#endif
/* 0x8011A0A8 | 0x1D8 */
extern void fn_80135E44(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011A0A8(void);
/* 0x8011A280 | 0x164 */
#if 0
asm void fn_8011A280(void) {
#include "src/game/gs_field_world_fn_8011A280.inc"
}
#else
void fn_8011A280(u8* arg1, u16 arg2, u32 arg3) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, u8* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119FA0(u8* ptr, u32 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (arg2 == 0) { goto exit; }
    if (arg2 != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == arg2) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    /* Phase 2 */
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_80119FA0(base, arg3);
    }

exit:
    return;
}
#endif
/* 0x8011A3E4 | 0x18C */
s32 fn_8011A3E4(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern s32 fn_8011A018(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return 0; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = (u8)fn_80119E90(val);
    base = (0, fn_80119F10(val));
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return 0; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base == NULL) { return 0; }
    return fn_8011A018(base);
}
/* 0x8011A570 | 0x164 */
#if 0
asm void fn_8011A570(void) {
#include "src/game/gs_field_world_fn_8011A570.inc"
}
#else
void fn_8011A570(u8* arg1, u16 arg2, u32 arg3) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, u8* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119F90(u8* ptr, u32 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (arg2 == 0) { goto exit; }
    if (arg2 != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == arg2) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    /* Phase 2 */
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_80119F90(base, arg3);
    }

exit:
    return;
}
#endif
/* 0x8011A6D4 | 0x18C */
s32 fn_8011A6D4(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_8011A000(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return 0; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return 0; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_8011A000(base);
    }
    return 0;
}
/* 0x8011A860 | 0x18C */
s32 fn_8011A860(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_8011A078(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return 0; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return 0; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_8011A078(base);
    }
    return 0;
}
/* 0x8011A9EC | 0x164 */
#if 0
asm void fn_8011A9EC(void) {
#include "src/game/gs_field_world_fn_8011A9EC.inc"
}
#else
void fn_8011A9EC(u8* arg1, u16 arg2, u32 arg3) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, u8* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119FD0(u8* ptr, u32 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (arg2 == 0) { goto exit; }
    if (arg2 != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == arg2) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    /* Phase 2 */
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_80119FD0(base, arg3);
    }

exit:
    return;
}
#endif
/* 0x8011AB50 | 0x164 */
#if 0
asm void fn_8011AB50(void) {
#include "src/game/gs_field_world_fn_8011AB50.inc"
}
#else
void fn_8011AB50(u8* arg1, u16 arg2, u32 arg3) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, u8* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119FC0(u8* ptr, u32 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (arg2 == 0) { goto exit; }
    if (arg2 != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == arg2) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    /* Phase 2 */
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_80119FC0(base, arg3);
    }

exit:
    return;
}
#endif
/* 0x8011ACB4 | 0x18C */
s32 fn_8011ACB4(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_8011A048(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return -1; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return -1; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_8011A048(base);
    }
    return -1;
}
/* 0x8011AE40 | 0x18C */
s32 fn_8011AE40(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_8011A060(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return -1; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return -1; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_8011A060(base);
    }
    return -1;
}
/* 0x8011AFCC | 0x164 */
#if 0
asm void fn_8011AFCC(void) {
#include "src/game/gs_field_world_fn_8011AFCC.inc"
}
#else
void fn_8011AFCC(u8* arg1, u16 arg2, u32 arg3) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, u8* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119FB0(u8* ptr, u8 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (arg2 == 0) { goto exit; }
    if (arg2 != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == arg2) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    /* Phase 2 */
    idx = fn_80119E90(arg2);
    base = fn_80119F10(arg2);
    base = statusGetStatus(base, arg1, 0, fn_80119ED0(arg2), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base != NULL) {
        fn_80119FB0(base, arg3);
    }

exit:
    return;
}
#endif
/* 0x8011B130 | 0x190 */
s32 fn_8011B130(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern u8 fn_8011A030(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return -1; }
    if (val != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = (u8)fn_80119E90(val);
    base = (0, fn_80119F10(val));
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { return -1; }

    idx = fn_80119E90(val);
    base = fn_80119F10(val);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base == NULL) { return -1; }
    return (u8)fn_8011A030(base);
}
/* 0x8011B2C0 | 0x184 */
extern s32 fn_80101AC4(u32);
#if 0
asm void fn_8011B2C0(void) {
#include "src/game/gs_field_world_fn_8011B2C0.inc"
}
#else
void fn_8011B2C0(void* obj, u16 id, u16 arg3) {
    extern u8 fn_80119D90(u16 idx);
    extern u8 fn_80119DD0(u16 idx);
    extern u8 fn_80119E50(u16 idx);
    extern u8 fn_80119E90(u16 idx);
    extern u16 fn_80119ED0(u16 idx);
    extern u8 fn_80119F10(u16 idx);
    extern void fn_80119F90(u8* ptr, u16 val);
    extern void fn_80119FA0(u8* ptr, u32 val);
    extern void fn_80119FB0(u8* ptr, u8 val);
    extern void fn_80119FC0(u8* ptr, u8 val);
    extern void fn_80119FD0(u8* ptr, u8 val);
    extern void fn_80119FE0(u8* ptr, u16 val);
    extern void fn_80119FF0(u8* ptr, u16 val);
    extern u8 fn_8011A030(u8* ptr);
    extern u8* statusGetStatus(u32 a, void* b, u32 c, u32 d, u32 e);
    u32 idx;
    u8 type;
    u32 old_count;
    u32 selector;
    u32 span;
    u8* elem;

    if (id == 0) { return; }

    idx = (u8)fn_80119E90(id);
    selector = fn_80119F10(id);
    span = fn_80119ED0(id);
    elem = statusGetStatus(selector, obj, 0, span, 0);
    if (elem != NULL) {
        elem += (u16)idx << 4;
    }
    if (elem == NULL) { return; }

    type = fn_80119E50(id);
    if (type == 4) {
        old_count = fn_8011A030(elem);
    } else {
        old_count = 0;
    }
    if (elem != NULL) {
        fn_80119FF0(elem, 0);
        fn_80119FE0(elem, 0);
        fn_80119FD0(elem, 0);
        fn_80119FC0(elem, 0);
        fn_80119FB0(elem, 0);
        fn_80119FA0(elem, 0);
        fn_80119F90(elem, 0);
    }
    fn_80119FF0(elem, id);
    fn_80119FE0(elem, arg3);
    fn_80119FD0(elem, (s8)fn_80101AC4(fn_80119D90(id)));
    if (type == 4) {
        u32 count = old_count + 1;
        u8 limit = fn_80119DD0(id);
        if (limit < (u8)count) {
            count = limit;
        }
        fn_80119FB0(elem, count);
    }
}
#endif
/* 0x8011B444 | 0x238 */
s32 fn_8011B444(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u32 fn_8011A090(u8* ptr);
    extern u8 fn_80119E50(u16 val);
    extern u16 fn_80119E10(u32 val);
    u8* base;
    u16 idx;
    u32 entry_val;
    s32 kind;
    u8 flag;

    if (val == 0) { return 1; }
    idx = fn_80119E90(val);
    base = (0, fn_80119F10(val));
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) { return 0; }

    entry_val = fn_8011A090(base);
    kind = (u8)fn_80119E50(val);

    if (kind == 2) { goto case2; }
    if (kind >= 2) { goto ge2; }
    if (kind == 0) { goto case0; }
    if (kind >= 0) { goto case1; }
    goto default_case;
ge2:
    if (kind == 4) { goto case4; }
    if (kind >= 4) { goto default_case; }
    goto case3;

case0:
    return 2;
case1:
    return 2;
case2:
    if (base != NULL) { goto case2_check; }
    flag = 0; goto case2_done;
case2_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto case2_done; }
    flag = 1;
case2_done:
    {
        u32 f = (u8)flag;
        u32 lz = __cntlzw(f);
        return (s32)(lz >> 5) + 1;
    }

case3:
    if (base != NULL) { goto case3_check; }
    flag = 0; goto case3_flag_done;
case3_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto case3_flag_done; }
    flag = 1;
case3_flag_done:
    if ((u8)flag == 0) { return 2; }
    if ((u16)entry_val == val) { goto case3_return1; }
    if ((u16)fn_80119E10(entry_val) == val) { goto case3_return1; }
    return 2;
case3_return1:
    return 1;

case4:
    if (base != NULL) { goto case4_check; }
    flag = 0; goto case4_flag_done;
case4_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto case4_flag_done; }
    flag = 1;
case4_flag_done:
    if ((u8)flag == 0) { return 2; }
    if ((u16)entry_val == val) { goto case4_return2; }
    if ((u16)fn_80119E10(entry_val) != val) { goto case4_return1; }
case4_return2:
    return 2;
case4_return1:
    return 1;
default_case:
    return 0;
}
/* 0x8011B67C | 0x10C */
s32 fn_8011B67C(void* obj, u16 val) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    u8* base;
    u16 idx;
    u8 flag;

    if (val == 0) { return 0; }
    idx = fn_80119E90(val);
    base = (0, fn_80119F10(val));
    base = statusGetStatus(base, obj, 0, fn_80119ED0(val), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) { return 0; }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto flag_done;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto flag_done; }
    flag = 1;
flag_done:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == val) {
            return 1;
        }
    }
    return 0;
}
/* 0x8011B788 | 0x1C8 */
#if 0
asm void fn_8011B788(void) {
#include "src/game/gs_field_world_fn_8011B788.inc"
}
#else
void fn_8011B788(void* obj, u16 id) {
    extern u8 fn_80119E90(u16 val);
    extern u8* fn_80119F10(u16 val);
    extern u32 fn_80119ED0(u16 val);
    extern u8* statusGetStatus(u8* a, void* b, u32 c, u32 d, u32 e);
    extern u16 fn_8011A090(u8* ptr);
    extern void fn_80119FF0(u8* ptr, u32 val);
    extern void fn_80119FE0(u8* ptr, u32 val);
    extern void fn_80119FD0(u8* ptr, u32 val);
    extern void fn_80119FC0(u8* ptr, u32 val);
    extern void fn_80119FB0(u8* ptr, u32 val);
    extern void fn_80119FA0(u8* ptr, u32 val);
    extern void fn_80119F90(u8* ptr, u32 val);
    u8* base;
    u16 idx;
    u8 flag;

    if (id == 0) { goto exit; }
    if (id != 0) { goto body_start; }
    flag = 0; goto flag_check;
body_start:
    idx = fn_80119E90(id);
    base = fn_80119F10(id);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(id), 0);
    if (base != NULL) { goto offset_calc; }
    base = NULL;
    goto check_base;
offset_calc:
    base = base + idx * 16;
check_base:
    if (base == NULL) {
        flag = 0;
        goto flag_check;
    }
    if (base != NULL) { goto a090_check; }
    flag = 0; goto inner_flag_check;
a090_check:
    if ((u16)fn_8011A090(base) == 0) { flag = 0; goto inner_flag_check; }
    flag = 1;
inner_flag_check:
    if ((u8)flag == 1) {
        if ((u16)fn_8011A090(base) == id) {
            flag = 1;
            goto flag_check;
        }
    }
    flag = 0;
flag_check:
    if ((u8)flag == 0) { goto exit; }

    idx = fn_80119E90(id);
    base = fn_80119F10(id);
    base = statusGetStatus(base, obj, 0, fn_80119ED0(id), 0);
    if (base != NULL) { goto p2_offset; }
    base = NULL;
    goto p2_check;
p2_offset:
    base = base + idx * 16;
p2_check:
    if (base == NULL) { goto exit; }
    if (base == NULL) { goto exit; }
    fn_80119FF0(base, 0);
    fn_80119FE0(base, 0);
    fn_80119FD0(base, 0);
    fn_80119FC0(base, 0);
    fn_80119FB0(base, 0);
    fn_80119FA0(base, 0);
    fn_80119F90(base, 0);

exit:
    return;
}
#endif
/* 0x8011B950 | 0xBC */
#if 0
asm void fn_8011B950(void) {
#include "src/game/gs_field_world_fn_8011B950.inc"
}
#else
void fn_8011B950(u8* base, u16 count) {
    extern void fn_80119FF0(u8* a, u32 b);
    extern void fn_80119FE0(u8* a, u32 b);
    extern void fn_80119FD0(u8* a, u32 b);
    extern void fn_80119FC0(u8* a, u32 b);
    extern void fn_80119FB0(u8* a, u32 b);
    extern void fn_80119FA0(u8* a, u32 b);
    extern void fn_80119F90(u8* a, u32 b);
    u8* elem;
    u16 i;
    for (i = 0; i < count; i++) {
        elem = base + ((u16)i << 4);
        if (elem != NULL) {
            fn_80119FF0(elem, 0);
            fn_80119FE0(elem, 0);
            fn_80119FD0(elem, 0);
            fn_80119FC0(elem, 0);
            fn_80119FB0(elem, 0);
            fn_80119FA0(elem, 0);
            fn_80119F90(elem, 0);
        }
    }
}
#endif
/* 0x8011BA0C | 0xB4 */
extern void jumptable_8035C260();
#if 0
asm void fn_8011BA0C(void) {
#include "src/game/gs_field_world_fn_8011BA0C.inc"
}
#else
u32 fn_8011BA0C(u8 type) {
    switch (type) {
    case 0:
        return 0;
    case 1:
        return 0xEC50;
    case 2:
        return 0xEC51;
    case 3:
        return 0xEC52;
    case 4:
        return 0xED09;
    case 5:
        return 0xEC53;
    case 6:
        return 0xEC54;
    case 7:
        return 0xEC55;
    case 8:
        return 0xEC56;
    case 9:
        return 0xEC57;
    case 10:
        return 0xEC58;
    case 11:
        return 0xEC59;
    default:
        return 0;
    }
}
#endif
/* 0x8011BAC0 | 0xAC */
#if 0
asm void fn_8011BAC0(void) {
#include "src/game/gs_field_world_fn_8011BAC0.inc"
}
#else
/* key typed u32 so raw stays unmasked (mr r29,r3); dead k==0&&k==0x163&&k==0x165 chain
 * reproduces the folded cmplwi sequence; 3-iter fn_8011BEB4 scan. byte-match verified. */
u32 fn_8011BAC0(u32 key, u8 target) {
    extern u8 fn_8011BEB4(u32 a, u32 b, u32 c, u32 d);
    u32 raw = key;
    u8 t;
    u8 i;
    u16 k = (u16)key;
    if (k == 0 && k == 0x163 && k == 0x165) {
        return 0;
    }
    t = target;
    if (t == 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (t == fn_8011BEB4(0, raw, 0x1a, i)) {
            return 1;
        }
    }
    return 0;
}
#endif
/* 0x6C | fn_8011BB6C | single_call_straight */
u8 fn_8011BB6C(u32 arg1, u8 arg2) {
    extern u8 fn_8011BEB4(u32 a, u32 b, u32 c, u32 d);
    u8 r = fn_8011BEB4(0, arg1, 2, 0);
    u8 a2 = arg2;
    return (u8)(r + a2 * (r * 20) / 100);
}
/* 0x8011BBD8 | 0x2DC */
extern void fn_8020A154(void);
extern void fn_8020A144(void);
extern void fn_8020A134(void);
extern void fn_8020A124(void);
extern void fn_8020A114(void);
extern void fn_8020A104(void);
extern void fn_8020A0F4(void);
extern void fn_8020A0E4(void);
extern void fn_8020A0D4(void);
extern void fn_8020A0C4(void);
extern void fn_8020A0B4(void);
extern void fn_8020A0A4(void);
extern void jumptable_8035C290();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011BBD8(void);
/* 0x8011BEB4 | 0x31C */
extern void fn_8020A2A0(void);
extern void fn_8020A288(void);
extern void fn_8020A270(void);
extern void fn_8020A258(void);
extern void fn_8020A224(void);
extern void fn_8020A20C(void);
extern void fn_8020A1F4(void);
extern void fn_8020A1DC(void);
extern void fn_8020A1C4(void);
extern void fn_8020A1AC(void);
extern void fn_8020A194(void);
extern void fn_8020A17C(void);
extern void fn_8020A164(void);
extern void jumptable_8035C35C();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011BEB4(void);
/* 0x8011C430 | 0x20 */
void fn_8011C430(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 3) { return; }
    ptr[idx + 0x34] = val;
}
/* 0x8011C588 | 0x30 */
u8 fn_8011C588(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 3) { return 0; }
    return ptr[idx + 0x34];
}
/* 0x8011CA34 | 0x2C */
extern u32 lbl_80478DF8;
extern u32 lbl_80478DFC;
void* wazaDataBiosGetPtr(u16 idx) {
    u32* hdr = (u32*)lbl_80478DF8;
    if (idx >= hdr[0]) {
        return (void*)lbl_80478DFC;
    }
    return (u8*)lbl_80478DFC + (u32)idx * 0x38;
}
/* 0x8011CA60 | 0x3C */
extern u32 lbl_80478B78;
extern u32 lbl_8035F9A8[];
u32 pokemonNakigoeDataBiosGetDataAddress(u8* ptr) {
    u16 idx;
    if (ptr == NULL) { return 0; }
    idx = *(u16*)(&ptr[0x10]);
    if (idx >= lbl_80478B78) { return 0; }
    return lbl_8035F9A8[idx];
}
/* 0x8011CAB8 | 0x28 */
extern u32 lbl_80478E68;
extern u32 lbl_80478E6C;
#pragma push
#pragma peephole off
void* pokemonDpFilterDataBiosGetPtr(u16 idx) {
    u32* hdr = (u32*)lbl_80478E68;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E6C + idx;
}
#pragma pop
/* 0x8011CAE0 | 0x30 */
s8 pokemonFriendFilterDataBiosGetValue(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 3) { return 0; }
    return (s8)ptr[idx];
}
/* 0x8011CB10 | 0x2C */
extern u32 lbl_80478B70;
extern u8 lbl_8035F988[];
void* fn_8011CB10(u16 idx) {
    if (idx >= lbl_80478B70) { return NULL; }
    return (u8*)lbl_8035F988 + (u32)idx * 3;
}
/* 0x8011CB6C | 0x2C */
extern u32 lbl_80478B68;
extern u8 lbl_8035F5E0[];
void* fn_8011CB6C(u16 idx) {
    if (idx >= lbl_80478B68) { return NULL; }
    return (u8*)lbl_8035F5E0 + (u32)idx * 0xC;
}
/* 0x8011CBC8 | 0x2C */
extern u32 lbl_80478E58;
extern u32 lbl_80478E5C;
#pragma push
#pragma peephole off
void* pokemonSeikakuRateDataBiosGetPtr(u8 idx) {
    u32* hdr = (u32*)lbl_80478E58;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E5C + (u32)idx * 2;
}
#pragma pop
/* 0x8011CBF4 | 0x30 */
u8 fn_8011CBF4(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[idx + 0x1F];
}
/* 0x8011CC24 | 0x30 */
u8 fn_8011CC24(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 7) { return 0; }
    return ptr[idx + 0x18];
}
/* 0x8011CE18 | 0x2C */
extern u32 lbl_80478E60;
extern u32 lbl_80478E64;
void* fn_8011CE18(u8 idx) {
    u32* hdr = (u32*)lbl_80478E60;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478E64 + (u32)idx * 0x28;
}
/* 0x8011CE44 | 0x30 */
u32 pokemonGrowDataBiosGetExp(u8* ptr, u8 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x65) { return 0; }
    return *(u32*)(&ptr[(u32)idx * 4]);
}
/* 0x8011CE74 | 0x2C */
extern u32 lbl_80478B60;
extern u8 lbl_8035E940[];
void* fn_8011CE74(u8 idx) {
    if (idx >= lbl_80478B60) { return NULL; }
    return (u8*)lbl_8035E940 + (u32)idx * 0x194;
}
/* 0x8011CED0 | 0x20 */
void fn_8011CED0(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x6E] = val;
}
/* 0x8011CEF0 | 0x24 */
void fn_8011CEF0(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 8) { return; }
    ptr += idx * 2;
    *(u16*)(&ptr[0x74]) = val;
}
/* 0x8011CF44 | 0x2C */
extern void _flagSet(u32);
void fn_8011CF44(u8* ptr) {
    if (ptr == NULL) { return; }
    _flagSet(*(u32*)(&ptr[0x28]));
}
/* 0x8011CF70 | 0x2C */
void fn_8011CF70(u8* ptr) {
    if (ptr == NULL) { return; }
    _flagSet(*(u32*)(&ptr[0x24]));
}
/* 0x40 | fn_8011CFEC | compound_indexed_setter */
void fn_8011CFEC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D02C | compound_indexed_setter */
void fn_8011D02C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D06C | compound_indexed_setter */
void fn_8011D06C(u8* ptr, u16 idx, u32 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 2) {
        sub = NULL;
    } else {
        sub = ptr + idx * 8 + 0x10C;
    }
    if (sub == NULL) { return; }
    *(u32*)(sub + 0x4) = val;
}
/* 0x8011D0AC | 0x20 */
void fn_8011D0AC(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 0x3A) { return; }
    ptr[idx + 0x34] = val;
}
/* 0x40 | fn_8011D0CC | compound_indexed_setter */
void fn_8011D0CC(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D10C | compound_indexed_setter */
void fn_8011D10C(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 0x14) {
        sub = NULL;
    } else {
        sub = ptr + idx * 4 + 0xBA;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x40 | fn_8011D14C | compound_indexed_setter */
void fn_8011D14C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x40 | fn_8011D18C | compound_indexed_setter */
void fn_8011D18C(u8* ptr, u16 idx, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x40 | fn_8011D1CC | compound_indexed_setter */
void fn_8011D1CC(u8* ptr, u16 idx, u8 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else if (idx >= 5) {
        sub = NULL;
    } else {
        sub = ptr + idx * 6 + 0x9C;
    }
    if (sub == NULL) { return; }
    *(u8*)(sub + 0x0) = val;
}
/* 0x8011D20C | 0x20 */
void fn_8011D20C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x32] = val;
}
/* 0x8011D22C | 0x20 */
void fn_8011D22C(u8* ptr, u16 idx, u8 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr[idx + 0x30] = val;
}
/* 0x8011D24C | 0x24 */
void fn_8011D24C(u8* ptr, u16 idx, u16 val) {
    if (ptr == NULL) { return; }
    if (idx >= 2) { return; }
    ptr += idx * 2;
    *(u16*)(&ptr[0x70]) = val;
}
/* 0x8011D2C0 | 36 bytes | compound_setter */
void fn_8011D2C0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D2E4 | 36 bytes | compound_setter */
void fn_8011D2E4(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D308 | 36 bytes | compound_setter */
void fn_8011D308(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D32C | 36 bytes | compound_setter */
void fn_8011D32C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D350 | 36 bytes | compound_setter */
void fn_8011D350(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D374 | 36 bytes | compound_setter */
void fn_8011D374(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D398 | 36 bytes | compound_setter */
void fn_8011D398(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011D3BC | 36 bytes | compound_setter */
void fn_8011D3BC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011D3E0 | 36 bytes | compound_setter */
void fn_8011D3E0(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011D404 | 36 bytes | compound_setter */
void fn_8011D404(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011D428 | 36 bytes | compound_setter */
void fn_8011D428(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011D44C | 36 bytes | compound_setter */
void fn_8011D44C(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x8011D480 | 0x14 */
void fn_8011D480(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xF8]) = (u32)val;
}
/* 0x68 | fn_8011D504 | compound_chained_setter */
void fn_8011D504(u8* ptr, u8 val) {
    u16 idx;
    void* entry;
    u8 check;
    if (ptr == NULL) { return; }
    if (ptr == NULL) {
        idx = 0;
    } else {
        idx = *(u16*)(ptr + 0x0);
    }
    if (idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)idx * 0x11C;
    }
    if (entry == NULL) {
        check = 0;
    } else {
        check = *(u8*)((u8*)entry + 0x33);
    }
    if (check == 0) {
        val = 0;
    }
    *(u8*)(ptr + 0xCC) = val;
}
/* 0x8011D58C | 36 bytes | compound_setter */
void fn_8011D58C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xC) = val;
}
/* 0x8011D5B0 | 36 bytes | compound_setter */
void fn_8011D5B0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xB) = val;
}
/* 0x8011D5D4 | 36 bytes | compound_setter */
void fn_8011D5D4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0xA) = val;
}
/* 0x8011D5F8 | 36 bytes | compound_setter */
void fn_8011D5F8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x9) = val;
}
/* 0x8011D61C | 36 bytes | compound_setter */
void fn_8011D61C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x8) = val;
}
/* 0x8011D640 | 36 bytes | compound_setter */
void fn_8011D640(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x7) = val;
}
/* 0x8011D664 | 36 bytes | compound_setter */
void fn_8011D664(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x6) = val;
}
/* 0x8011D688 | 36 bytes | compound_setter */
void fn_8011D688(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x5) = val;
}
/* 0x8011D6AC | 36 bytes | compound_setter */
void fn_8011D6AC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D6D0 | 36 bytes | compound_setter */
void fn_8011D6D0(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D6F4 | 36 bytes | compound_setter */
void fn_8011D6F4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D718 | 36 bytes | compound_setter */
void fn_8011D718(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D73C | 36 bytes | compound_setter */
void fn_8011D73C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D770 | 36 bytes | compound_setter */
void fn_8011D770(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D794 | 36 bytes | compound_setter */
void fn_8011D794(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D7B8 | 36 bytes | compound_setter */
void fn_8011D7B8(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D7DC | 36 bytes | compound_setter */
void fn_8011D7DC(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D800 | 36 bytes | compound_setter */
void fn_8011D800(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D824 | 36 bytes | compound_setter */
void fn_8011D824(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x4) = val;
}
/* 0x8011D848 | 36 bytes | compound_setter */
void fn_8011D848(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x3) = val;
}
/* 0x8011D86C | 36 bytes | compound_setter */
void fn_8011D86C(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x2) = val;
}
/* 0x8011D890 | 36 bytes | compound_setter */
void fn_8011D890(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x1) = val;
}
/* 0x8011D8B4 | 36 bytes | compound_setter */
void fn_8011D8B4(void* ptr, u8 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) return;
    *(u8*)((u8*)sub + 0x0) = val;
}
/* 0x8011D8D8 | 0x1C */
void fn_8011D8D8(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val >= 0x639C) { val = 0x639C; }
    *(u32*)(&ptr[0xDC]) = (u32)val;
}
/* 0x8011D904 | 0x20 */
void fn_8011D904(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xB0]) = val;
}
/* 0x8011D924 | 0x34 */
void fn_8011D924(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011D958 | 0x34 */
void fn_8011D958(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011D98C | 0x34 */
void fn_8011D98C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011D9C0 | 0x34 */
void fn_8011D9C0(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011D9F4 | 0x34 */
void fn_8011D9F4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DA28 | 0x34 */
void fn_8011DA28(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0xA4;
    }
    if (sub == NULL) return;
    if (val > 0x1F) { val = 0x1F; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DA5C | 0x34 */
void fn_8011DA5C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0xA) = val;
}
/* 0x8011DA90 | 0x34 */
void fn_8011DA90(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x8) = val;
}
/* 0x8011DAC4 | 0x34 */
void fn_8011DAC4(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x6) = val;
}
/* 0x8011DAF8 | 0x34 */
void fn_8011DAF8(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x4) = val;
}
/* 0x8011DB2C | 0x34 */
void fn_8011DB2C(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x2) = val;
}
/* 0x8011DB60 | 0x34 */
void fn_8011DB60(u8* ptr, u16 val) {
    u8* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x98;
    }
    if (sub == NULL) return;
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(sub + 0x0) = val;
}
/* 0x8011DB94 | 36 bytes | compound_setter */
void fn_8011DB94(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0xA) = val;
}
/* 0x8011DBB8 | 36 bytes | compound_setter */
void fn_8011DBB8(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x8) = val;
}
/* 0x8011DBDC | 36 bytes | compound_setter */
void fn_8011DBDC(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x6) = val;
}
/* 0x8011DC00 | 36 bytes | compound_setter */
void fn_8011DC00(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x4) = val;
}
/* 0x8011DC24 | 36 bytes | compound_setter */
void fn_8011DC24(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x2) = val;
}
/* 0x8011DC48 | 36 bytes | compound_setter */
void fn_8011DC48(void* ptr, u16 val) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) return;
    *(u16*)((u8*)sub + 0x0) = val;
}
/* 0x48 | fn_8011DC6C | compound_clamp_setter */
void fn_8011DC6C(u8* ptr, u16 val) {
    void* sub;
    u16 subVal;
    if (ptr == NULL) { return; }
    *(u16*)(ptr + 0x8A) = val;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) {
        subVal = 0;
    } else {
        subVal = *(u16*)((u8*)sub + 0x0);
    }
    if (subVal >= *(u16*)(ptr + 0x8A)) { return; }
    *(u16*)(ptr + 0x8A) = subVal;
}
/* 0x8011DCC4 | 0xBC */
#if 0
asm void fn_8011DCC4(void) {
#include "src/game/gs_field_world_fn_8011DCC4.inc"
}
#else
void fn_8011DCC4(u8* ptr, u32 arg2, u8 arg3) {
    extern u8* fn_8011F260(u8* a, u32 b, u32 c);
    extern u32 fn_80123CD4(u8* a, u32 b);
    extern void* wazaDataBiosGetPtr(u32 val);
    u8* result;
    result = fn_8011F260(ptr, arg2, 0);
    if (result == NULL) { return; }
    if ((u8)arg3 > 3) { arg3 = 3; }
    if ((u8)fn_80123CD4(ptr, arg2) == 1) {
        u8* tmp = fn_8011F260(ptr, arg2, 1);
        u32 val = tmp == NULL ? 0 : *(u16*)tmp;
        if ((u8)fn_8011CA04(wazaDataBiosGetPtr(val)) <= 4) { return; }
    }
    result[3] = arg3;
}
#endif
/* 0x7C | fn_8011DD80 | call_clamp_store */
void fn_8011DD80(u32 arg1, s32 arg2, u8 maxVal) {
    extern void* fn_8011F260();
    extern u8 fn_80123E70(u8* ptr, s32 idx);
    u8* result;
    u8 val;
    result = fn_8011F260(arg1, arg2, 0);
    if (result == NULL) { return; }
    val = fn_80123E70((u8*)arg1, arg2);
    if (val < maxVal) {
        maxVal = val;
    }
    *(u8*)(result + 0x2) = maxVal;
}
/* 0x8011DDFC | 0x3C */
void fn_8011DDFC(void* ctx, u32 p1, u32 value) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 0);
    if (result != 0) {
        *(u16*)result = value;
    }
}
/* 0x8011DE48 | 0x20 */
void fn_8011DE48(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    if (val > 0x64) { val = 0x64; }
    ptr[0x60] = val;
}
/* 0x8011DE68 | 0x20 */
void fn_8011DE68(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    if (val > 0xFF) { val = 0xFF; }
    *(u16*)(&ptr[0xE4]) = val;
}
/* 0x8011DEA8 | 0x3C */
void fn_8011DEA8(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x44, src, 0xB);
}
/* 0x70 | fn_8011DEE4 | dual_memcpy_setter */
void fn_8011DEE4(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x2E, src, 0xB);
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x44, src, 0xB);
}
/* 0x8011DF54 | 0x3C */
void fn_8011DF54(u8* ptr, void* src) {
    extern void GScharLenCpy();
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0x18, src, 0xB);
}
/* 0x8011E048 | 0x30 */
u8 fn_8011E048(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x6E];
}
/* 0x8011E078 | 0x34 */
u16 fn_8011E078(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 8) { return 0; }
    ptr += idx * 2;
    return *(u16*)(&ptr[0x74]);
}
/* 0x8011E0F4 | 0x34 */
extern void* fn_801906A0(u32);
void* fn_8011E0F4(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x28]));
}
/* 0x8011E128 | 0x34 */
void* fn_8011E128(u8* ptr) {
    if (ptr == NULL) { return NULL; }
    return fn_801906A0(*(u32*)(&ptr[0x24]));
}
/* 0x8011E2AC | 0x30 */
u8 fn_8011E2AC(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 0x3A) { return 0; }
    return ptr[idx + 0x34];
}
/* 0x8011E444 | 0x30 */
u8 fn_8011E444(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x32];
}
/* 0x8011E474 | 0x30 */
u8 fn_8011E474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return 0; }
    if (idx >= 2) { return 0; }
    return ptr[idx + 0x30];
}
/* 0x8011E57C | 44 bytes | compound_getter */
u16 fn_8011E57C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E5A8 | 44 bytes | compound_getter */
u16 fn_8011E5A8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E5D4 | 44 bytes | compound_getter */
u16 fn_8011E5D4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E600 | 44 bytes | compound_getter */
u16 fn_8011E600(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E62C | 44 bytes | compound_getter */
u16 fn_8011E62C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x90;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E658 | 44 bytes | compound_getter */
u16 fn_8011E658(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011E684 | 44 bytes | compound_getter */
u16 fn_8011E684(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011E6B0 | 44 bytes | compound_getter */
u16 fn_8011E6B0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011E6DC | 44 bytes | compound_getter */
u16 fn_8011E6DC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011E708 | 44 bytes | compound_getter */
u16 fn_8011E708(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011E734 | 44 bytes | compound_getter */
u16 fn_8011E734(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x84;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011E778 | 0x2C */
extern u32 lbl_80478F90;
extern u32 lbl_80478F94;
void* fn_8011E778(u16 idx) {
    u32* hdr = (u32*)lbl_80478F90;
    if (idx >= hdr[0]) { return NULL; }
    return (u8*)lbl_80478F94 + (u32)idx * 0x11C;
}
/* 0x8011E7A4 | 0x1C */
u8 fn_8011E7A4(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return (u8)(*(u32*)(&ptr[0xF8]));
}
/* 0x8011E90C | 44 bytes | compound_getter */
u8 fn_8011E90C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xC);
}
/* 0x8011E938 | 44 bytes | compound_getter */
u8 fn_8011E938(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xB);
}
/* 0x8011E964 | 44 bytes | compound_getter */
u8 fn_8011E964(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0xA);
}
/* 0x8011E990 | 44 bytes | compound_getter */
u8 fn_8011E990(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x9);
}
/* 0x8011E9BC | 44 bytes | compound_getter */
u8 fn_8011E9BC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x8);
}
/* 0x8011E9E8 | 44 bytes | compound_getter */
u8 fn_8011E9E8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x7);
}
/* 0x8011EA14 | 44 bytes | compound_getter */
u8 fn_8011EA14(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x6);
}
/* 0x8011EA40 | 44 bytes | compound_getter */
u8 fn_8011EA40(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x5);
}
/* 0x8011EA6C | 44 bytes | compound_getter */
u8 fn_8011EA6C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EA98 | 44 bytes | compound_getter */
u8 fn_8011EA98(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EAC4 | 44 bytes | compound_getter */
u8 fn_8011EAC4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EAF0 | 44 bytes | compound_getter */
u8 fn_8011EAF0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EB1C | 44 bytes | compound_getter */
u8 fn_8011EB1C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xBD;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EB60 | 44 bytes | compound_getter */
u8 fn_8011EB60(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EB8C | 44 bytes | compound_getter */
u8 fn_8011EB8C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EBB8 | 44 bytes | compound_getter */
u8 fn_8011EBB8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011EBE4 | 44 bytes | compound_getter */
u8 fn_8011EBE4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011EC10 | 44 bytes | compound_getter */
u8 fn_8011EC10(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB7;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EC3C | 44 bytes | compound_getter */
u8 fn_8011EC3C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x4);
}
/* 0x8011EC68 | 44 bytes | compound_getter */
u8 fn_8011EC68(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x3);
}
/* 0x8011EC94 | 44 bytes | compound_getter */
u8 fn_8011EC94(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x2);
}
/* 0x8011ECC0 | 44 bytes | compound_getter */
u8 fn_8011ECC0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x1);
}
/* 0x8011ECEC | 44 bytes | compound_getter */
u8 fn_8011ECEC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xB2;
    }
    if (sub == NULL) { return 0; }
    return *(u8*)((u8*)sub + 0x0);
}
/* 0x8011EDC4 | 0x34 */
void* fn_8011EDC4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0xE8;
}
/* 0x8011EE70 | 44 bytes | compound_getter */
u16 fn_8011EE70(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EE9C | 44 bytes | compound_getter */
u16 fn_8011EE9C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EEC8 | 44 bytes | compound_getter */
u16 fn_8011EEC8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EEF4 | 44 bytes | compound_getter */
u16 fn_8011EEF4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011EF20 | 44 bytes | compound_getter */
u16 fn_8011EF20(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011EF4C | 44 bytes | compound_getter */
u16 fn_8011EF4C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0xA4;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011EF78 | 44 bytes | compound_getter */
u16 fn_8011EF78(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011EFA4 | 44 bytes | compound_getter */
u16 fn_8011EFA4(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011EFD0 | 44 bytes | compound_getter */
u16 fn_8011EFD0(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011EFFC | 44 bytes | compound_getter */
u16 fn_8011EFFC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F028 | 44 bytes | compound_getter */
u16 fn_8011F028(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F054 | 44 bytes | compound_getter */
u16 fn_8011F054(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x98;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F080 | 44 bytes | compound_getter */
u16 fn_8011F080(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0xA);
}
/* 0x8011F0AC | 44 bytes | compound_getter */
u16 fn_8011F0AC(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x8);
}
/* 0x8011F0D8 | 44 bytes | compound_getter */
u16 fn_8011F0D8(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x6);
}
/* 0x8011F104 | 44 bytes | compound_getter */
u16 fn_8011F104(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x4);
}
/* 0x8011F130 | 44 bytes | compound_getter */
u16 fn_8011F130(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x2);
}
/* 0x8011F15C | 44 bytes | compound_getter */
u16 fn_8011F15C(void* ptr) {
    void* sub;
    if (ptr == NULL) {
    sub = NULL;
    } else {
    sub = (u8*)ptr + 0x8C;
    }
    if (sub == NULL) { return 0; }
    return *(u16*)((u8*)sub + 0x0);
}
/* 0x8011F1B8 | 0x38 */
u8 fn_8011F1B8(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x3);
}
/* 0x8011F1F0 | 0x38 */
u8 fn_8011F1F0(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *((u8*)result + 0x2);
}
/* 0x8011F228 | 0x38 */
u16 fn_8011F228(void* ctx, u32 p1) {
    extern void* fn_8011F260();
    void* result = fn_8011F260(ctx, p1, 1);
    if (result == 0) return 0;
    return *(u16*)result;
}
/* 0x8011F260 | 0x1FC */
extern u8 lbl_80478B58[4];
extern u8 lbl_80478B5C[4];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011F260(void);
/* 0x8011F474 | 0x34 */
void* fn_8011F474(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 1) { return NULL; }
    return ptr + (u32)idx * 16 + 0x64;
}
/* 0x8011F4D8 | 24 bytes | nc_addi_ptr */
void* fn_8011F4D8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x44;
}
/* 0x8011F4F0 | 24 bytes | nc_addi_ptr */
void* fn_8011F4F0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x2E;
}
/* 0x8011F508 | 24 bytes | nc_addi_ptr */
void* fn_8011F508(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x18;
}
/* 0x8011F598 | 24 bytes | nc_addi_ptr */
void* fn_8011F598(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}
/* 0x8011F5E0 | 0x1C */
void fn_8011F5E0(u32* dst, u32* src) {
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    *dst = *src;
}
/* 0x8011F5FC | 0x38 */
#ifndef PCPORT
typedef struct { u32 data[0x4E]; } GfwBuf0x138;
#endif
void fn_8011F5FC(u32* dst, u32* src) {
#ifdef PCPORT
    u32 i;
#endif
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
#ifdef PCPORT
    for (i = 0; i < 0x4E; i++) {
        dst[i] = src[i];
    }
#else
    *(GfwBuf0x138*)dst = *(GfwBuf0x138*)src;
#endif
}
/* 0x8011F634 | 0xA4 */
#if 0
asm void fn_8011F634(void) {
#include "src/game/gs_field_world_fn_8011F634.inc"
}
#else
u32 fn_8011F634(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_8011F77C(u8* a);
    extern void* fn_8011CE18(u32 val);
    extern u32 fn_8011CBF4(void* a, u32 b);
    u8 byte1;
    u32 val2;
    if (ptr == NULL) { return 0; }
    if ((u8)fn_8012640C(ptr, 0, 0xc2, 0) == 0) { return 0; }
    byte1 = (u8)fn_8012640C(ptr, 0, 0xbf, 0);
    val2 = fn_8011F77C(ptr);
    if ((u8)val2 <= 7) {
        return fn_8011CBF4(fn_8011CE18(byte1), val2);
    }
    return 0;
}
#endif
/* 0x8011F6D8 | 0xA4 */
#if 0
asm void fn_8011F6D8(void) {
#include "src/game/gs_field_world_fn_8011F6D8.inc"
}
#else
u32 fn_8011F6D8(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_8011F77C(u8* a);
    extern void* fn_8011CE18(u32 val);
    extern u32 fn_8011CC24(void* a, u32 b);
    u8 byte1;
    u32 val2;
    if (ptr == NULL) { return 0; }
    if ((u8)fn_8012640C(ptr, 0, 0xc2, 0) == 0) { return 0; }
    byte1 = (u8)fn_8012640C(ptr, 0, 0xbf, 0);
    val2 = fn_8011F77C(ptr);
    if ((u8)val2 <= 7) {
        return fn_8011CC24(fn_8011CE18(byte1), val2);
    }
    return 0;
}
#endif
/* 0x8011F77C | 0x194 */
extern f32 lbl_8047CFF0;
extern f64 lbl_8047D008;
extern f32 lbl_8047CFF4;
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF8;
extern f32 lbl_8047CFFC;
extern f32 lbl_8047D000;
extern f32 lbl_8047D004;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011F77C(void);
/* 0x8011F910 | 0x2BC */
extern void fn_801440A0(void);
extern void fn_80144014(void);
extern void fn_80143E14(void);
extern f64 lbl_8047D008;
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF0;
extern f32 lbl_8047CFF4;
extern f32 lbl_8047D018;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011F910(void);
/* 0x48 | fn_8011FBCC | generic */
extern f32 lbl_8047CFF4;
#if 0
asm void fn_8011FBCC(void) {
#include "src/game/gs_field_world_fn_8011FBCC.inc"
}
#else
void fn_8011FBCC(u8* ptr, f32 f1) {
    extern void fn_801254B4(u8* ptr, u32 a, u32 b, u32 c, u32 d);
    if (ptr == NULL) { return; }
    fn_801254B4(ptr, 0, 0xc5, 0, (u32)(s32)(lbl_8047CFF4 * f1));
}
#endif
/* 0x60 | fn_8011FC14 | generic */
extern f32 lbl_8047CFF0;
extern f64 lbl_8047D008;
extern f32 lbl_8047CFF4;
#if 0
asm void fn_8011FC14(void) {
#include "src/game/gs_field_world_fn_8011FC14.inc"
}
#else
f32 fn_8011FC14(u8* ptr) {
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    u32 val;
    u32 sp[2];
    f32 result;
    if (ptr == NULL) { return lbl_8047CFF0; }
    val = fn_8012640C(ptr, 0, 0xc5, 0);
    sp[1] = val ^ 0x80000000;
    sp[0] = 0x43300000;
    result = *(f64*)sp - lbl_8047D008;
    return result / lbl_8047CFF4;
}
#endif
/* 0x8011FC74 | 0x30 */
u8 fn_8011FC74(u32 arg) {
    extern u32 fn_8012640C(u32 a, u32 b, u32 c, u32 d);
    return (u8)fn_8012640C(arg, 0, 0xC2, 0);
}
/* 0x8011FCA4 | 0x124 */
extern void* fn_801EEEB8();
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011FCA4(void);
/* 0x8011FDC8 | 0x504 */
extern void GScharCpy(void);
extern void fn_8010BBB8(void);
extern void fn_8001D994(void);
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF0;
extern f64 lbl_8047D008;
extern f32 lbl_8047CFF4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8011FDC8(void);
/* 0x801202CC | 0x1DC */
extern f64 lbl_8047D010;
extern f32 lbl_8047CFF0;
extern f64 lbl_8047D008;
extern f32 lbl_8047CFF4;
#if 0
asm void fn_801202CC(void) {
#include "src/game/gs_field_world_fn_801202CC.inc"
}
#else
void fn_801202CC(u8* ptr, u8* out) {
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    extern void* fn_8011CE74(u8 idx);
    extern u32 pokemonGrowDataBiosGetExp(void* ptr, u8 idx);
    u8 idx;
    u32 base;
    void* table;
    u32 next;
    u32 sp[2];
    u32 val;
    f32 scale;

    idx = (u8)fn_8012640C(ptr, 0, 0x7A, 0);
    if (ptr == NULL) {
        base = 0;
    } else {
        table = fn_8011CE74((u8)fn_8012640C(0, (u16)fn_8012640C(ptr, 0, 0x6E, 0), 0x11, 0));
        if (table == NULL) {
            base = 0;
        } else {
            base = pokemonGrowDataBiosGetExp(table, idx);
        }
    }

    if ((u8)fn_8012640C(ptr, 0, 0xC2, 0) == 0) {
        if (ptr == NULL) {
            next = 0;
        } else {
            table = fn_8011CE74((u8)fn_8012640C(0, (u16)fn_8012640C(ptr, 0, 0x6E, 0), 0x11, 0));
            if (table == NULL) {
                next = 0;
            } else {
                next = pokemonGrowDataBiosGetExp(table, (u8)(idx + 1));
            }
        }
        *(u32*)(out + 0x1C) = next - base;
        *(u32*)(out + 0x20) = fn_8012640C(ptr, 0, 0x79, 0) - base;
    } else {
        val = (u16)(u32)fn_801EEEB8((u16)fn_8012640C(ptr, 0, 0xC3, 0));
        sp[0] = 0x43300000;
        sp[1] = val;
        *(f32*)(out + 0x1C) = *(f64*)sp - lbl_8047D010;
        if (ptr == NULL) {
            scale = lbl_8047CFF0;
        } else {
            sp[0] = 0x43300000;
            sp[1] = fn_8012640C(ptr, 0, 0xC5, 0) ^ 0x80000000;
            scale = (*(f64*)sp - lbl_8047D008) / lbl_8047CFF4;
        }
        *(f32*)(out + 0x20) = scale;
    }
}
#endif
/* 0x801204A8 | 0x1CC */
extern void fn_800FA280(void);
extern void fn_8010C4D4(void);
extern void fn_8010C46C(void);
#if 0
asm void fn_801204A8(void) {
#include "src/game/gs_field_world_fn_801204A8.inc"
}
#else
void fn_801204A8(u8* ptr, u8* out) {
    extern u32 fn_800FA280();
    extern u32 fn_8010C46C();
    extern u32 fn_8010C4D4();
    extern u32 fn_8011BB6C();
    extern u32 fn_8011BEB4();
    extern u32 fn_8012640C();
    u8 i;
    u8 valid;
    u32 slot2;
    u16 id;
    u16 resolved;
    u16 extra;
    u32 byte;
    u8* slot;

    memset(out, 0, 0x48);
    *(u32*)out = fn_8012640C(ptr, 0, 0x77, 0);
    for (i = 0; i < 4; i++) {
        slot = out + i * 0xC + 4;
        if (ptr == NULL) {
            valid = 0;
        } else if ((s32)fn_8012640C(ptr, 0, 0x7F, i) == 0) {
            valid = 0;
        } else if ((s32)fn_8012640C(ptr, 0, 0x7F, i) == 0x163) {
            valid = 0;
        } else {
            valid = 1;
        }
        if (valid == 0) {
            *(u32*)(slot + 0x0) = 0;
            *(u32*)(slot + 0x4) = 0;
            *(u8*)(slot + 0xA) = 0;
            *(u8*)(slot + 0xB) = 0;
        } else {
            id = (u16)fn_8012640C(ptr, 0, 0x7F, i);
            resolved = (u16)fn_8011BEB4(0, id, 3, 0);
            *(u32*)(slot + 0x0) = fn_800FA280(fn_8011BEB4(0, id, 1, 0));
            *(u32*)(slot + 0x4) = fn_800FA280(fn_8010C4D4(resolved));
            *(u16*)(slot + 0x8) = (u16)fn_8010C46C(resolved);
            if (ptr == NULL) {
                byte = 0;
            } else {
                slot2 = i + 4;
                extra = (u16)fn_8012640C(ptr, 0, 0x7F, slot2);
                byte = fn_8011BB6C(extra, (u8)fn_8012640C(ptr, 0, 0x81, slot2));
            }
            *(u8*)(slot + 0xA) = byte;
            *(u8*)(slot + 0xB) = (u8)fn_8012640C(ptr, 0, 0x80, i);
        }
    }
}
#endif
/* 0x80120674 | 0x1F8 */
extern void fn_800E0C54(void);
extern u8 lbl_8027296C[];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80120674(void);
/* 0x8012086C | 0x294 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012086C(void);
/* 0x80120B00 | 0x16C */
#if 0
asm void fn_80120B00(void) {
#include "src/game/gs_field_world_fn_80120B00.inc"
}
#else
void fn_80120B00(u8* ptr, u16* out1, u16* out2) {
    extern u32 fn_8012640C(u8*, u32, u32, u32);
    u16 v93 = (u16)fn_8012640C(ptr, 0, 0x93, 0);
    u16 v94 = (u16)fn_8012640C(ptr, 0, 0x94, 0);
    u16 v95 = (u16)fn_8012640C(ptr, 0, 0x95, 0);
    u16 v96 = (u16)fn_8012640C(ptr, 0, 0x96, 0);
    u16 v97 = (u16)fn_8012640C(ptr, 0, 0x97, 0);
    u16 v98 = (u16)fn_8012640C(ptr, 0, 0x98, 0);
    u8 hi = (u8)(((v93 & 2) >> 1) | (v94 & 2) | ((v95 << 1) & 4) |
                 ((v98 << 2) & 8) | ((v96 << 3) & 0x10) | ((v97 << 4) & 0x20));
    u8 lo = (u8)((v93 & 1) | ((v94 & 1) << 1) | ((v95 & 1) << 2) |
                 ((v98 & 1) << 3) | ((v96 & 1) << 4) | ((v97 & 1) << 5));
    u16 val1 = (u16)((s32)(hi * 0x28) / 0x3F + 0x1E);
    u32 val2 = (u8)((s32)(lo * 0xF) / 0x3F + 1);

    if ((u8)val2 >= 9) {
        val2 = (u16)((u8)val2 + 1);
    }
    if (out1 != NULL) {
        *out1 = val1;
    }
    if (out2 != NULL) {
        *out2 = (u16)val2;
    }
}
#endif
/* 0x80120CDC | 0x90 */
#if 0
asm void fn_80120CDC(void) {
#include "src/game/gs_field_world_fn_80120CDC.inc"
}
#else
u32 fn_80120CDC(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern void* fn_8011CE74(u32 val);
    extern u32 pokemonGrowDataBiosGetExp(void* a, u32 b);
    u8 byte1, byte2;
    void* result;
    byte1 = (u8)fn_8012640C(NULL, (u16)fn_8012640C(ptr, 0, 0x6e, 0), 0x11, 0);
    byte2 = (u8)fn_8012640C(ptr, 0, 0x7a, 0);
    result = fn_8011CE74(byte1);
    if (result == NULL) { return 0; }
    return pokemonGrowDataBiosGetExp(result, byte2);
}
#endif
/* 0x80120DD0 | 0x210 */
#if 0
asm void fn_80120DD0(void) {
#include "src/game/gs_field_world_fn_80120DD0.inc"
}
#else
u32 fn_80120DD0(u8* ptr) {
    extern u16 fn_80119ED0(u32 val);
    extern u8 fn_8011B67C(u8* ptr, u32 val);
    extern void fn_80119F50(u32 val);
    u8 result;
    u32 acc = 0;
    if (ptr == NULL) { return 0; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check1; } }
    result = fn_8011B67C(ptr, 3);
    check1: if ((u8)result == 1) { acc = 3; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check2; } }
    result = fn_8011B67C(ptr, 4);
    check2: if ((u8)result == 1) { acc = 4; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 8);
    check3: if ((u8)result == 1) { acc = 8; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 5);
    check4: if ((u8)result == 1) { acc = 5; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C(ptr, 6);
    check5: if ((u8)result == 1) { acc = 6; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C(ptr, 7);
    check6: if ((u8)result == 1) { acc = 7; }

    fn_80119F50(acc);
}
#endif
/* 0x80120FE0 | 0x218 */
#if 0
asm void fn_80120FE0(void) {
#include "src/game/gs_field_world_fn_80120FE0.inc"
}
#else
u32 fn_80120FE0(u8* ptr) {
    extern u16 fn_80119ED0(u32 val);
    extern u32 fn_8011B67C(u8* ptr, u32 val);
    u32 result;
    if (ptr == NULL) { return 0; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 3);
    check3: if ((u8)result == 1) { return 0x3a; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 4);
    check4: if ((u8)result == 1) { return 0x3a; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C(ptr, 5);
    check5: if ((u8)result == 1) { return 0x3b; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C(ptr, 6);
    check6: if ((u8)result == 1) { return 0x3c; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7; } }
    result = fn_8011B67C(ptr, 7);
    check7: if ((u8)result == 1) { return 0x3d; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8; } }
    result = fn_8011B67C(ptr, 8);
    check8: if ((u8)result == 1) { return 0x3e; }

    return 0;
}
#endif
/* 0x801211F8 | 0x218 */
#if 0
asm void fn_801211F8(void) {
#include "src/game/gs_field_world_fn_801211F8.inc"
}
#else
u32 fn_801211F8(u8* ptr) {
    extern u16 fn_80119ED0(u32 val);
    extern u32 fn_8011B67C(u8* ptr, u32 val);
    u32 result;
    if (ptr == NULL) { return 0; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 3);
    check3: if ((u8)result == 1) { return 0x3; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 4);
    check4: if ((u8)result == 1) { return 0x4; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C(ptr, 5);
    check5: if ((u8)result == 1) { return 0x5; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C(ptr, 6);
    check6: if ((u8)result == 1) { return 0x6; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7; } }
    result = fn_8011B67C(ptr, 7);
    check7: if ((u8)result == 1) { return 0x7; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8; } }
    result = fn_8011B67C(ptr, 8);
    check8: if ((u8)result == 1) { return 0x8; }

    return 0;
}
#endif
/* 0x78 | fn_80121484 | status_guarded_call */
void fn_80121484(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011A0A8();
    if ((u16)fn_80119ED0(arg3) == 0x7C ||
        (u16)fn_80119ED0(arg3) == 0xC8) {
        fn_8011A0A8(obj, arg2, arg3);
    }
}
/* 0x78 | fn_801214FC | status_guarded_call */
void fn_801214FC(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011A280();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011A280(obj, arg2, arg3);
    }
}
/* 0x78 | fn_801215E4 | status_guarded_call */
void fn_801215E4(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011A570();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011A570(obj, arg2, arg3);
    }
}
/* 0x78 | fn_8012173C | status_guarded_call */
void fn_8012173C(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011A9EC();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011A9EC(obj, arg2, arg3);
    }
}
/* 0x78 | fn_801217B4 | status_guarded_call */
void fn_801217B4(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011AB50();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011AB50(obj, arg2, arg3);
    }
}
/* 0x78 | fn_8012190C | status_guarded_call */
void fn_8012190C(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011AFCC();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011AFCC(obj, arg2, arg3);
    }
}
/* 0x78 | fn_801219F4 | status_guarded_call */
void fn_801219F4(void* obj, u32 arg2, u32 arg3) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011B2C0(obj, arg2, arg3);
    }
}
/* 0x68 | fn_80121B4C | status_guarded_call */
void fn_80121B4C(void* obj, u32 arg2) {
    extern u16 fn_80119ED0();
    extern void fn_8011B788();
    if ((u16)fn_80119ED0(arg2) == 0x7C ||
        (u16)fn_80119ED0(arg2) == 0xC8) {
        fn_8011B788(obj, arg2);
    }
}
/* 0x64 | fn_80121BB4 | generic -- depends on fn_8011F5FC signature */
void fn_80121BB4(u32* arg1, u32* arg2) {
    u32 tmp[0x4e];
    if (arg1 == NULL) { return; }
    if (arg2 == NULL) { return; }
    fn_8011F5FC(tmp, arg1);
    fn_8011F5FC(arg1, arg2);
    fn_8011F5FC(arg2, tmp);
}
/* 0x80121C18 | 0x428 */
extern u32 fn_801DE190(u32 idx, u32 base, u32 flag);
extern void fn_801DA3CC(void);
extern void fn_801DA36C(void);
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
#if 0
asm void fn_80121C18(void) {
#include "src/game/gs_field_world_fn_80121C18.inc"
}
#else
void* fn_80121C18(void* arg) {
    extern u32 fn_8012640C(void* obj, u32 a, u32 b, u32 c);
    extern u16 fn_80119ED0(u32 val);
    extern u32 fn_8011B67C(u8* ptr, u32 val);
    extern u32 fn_801DE190(u32 idx, u32 base, u32 flag);
    extern void fn_801DA36C(void* obj, u32 val);
    extern void fn_801DA3CC(void* obj, u32 val);
    u32 r30, r28, r29, r5, tmp, xv, tmp2;
    u32 result, acc;
    u8 valid;

    r30 = fn_8012640C(arg, 0, 0x6f, 0);
    r28 = (u16)fn_8012640C(arg, 0, 0x6e, 0);

    if (r28 == 0) {
        valid = 0;
    } else {
        if (fn_8012640C(NULL, r28, 1, 0) == 0) { valid = 0; }
        else if (r28 >= *(u32*)(u32)lbl_80478F90) { valid = 0; }
        else { valid = 1; }
    }
    if ((u8)valid == 0) { return 0; }

    r28 = fn_8012640C(NULL, r28, 0x66, 0);

    if (arg == NULL) {
        r5 = 0;
    } else {
        r29 = fn_8012640C(arg, 0, 0x75, 0);
        tmp = fn_8012640C(arg, 0, 0x6f, 0);
        xv = (r29 >> 16) ^ (r29 & 0xFFFF);
        tmp2 = (tmp >> 16) ^ (tmp & 0xFFFF);
        xv = xv ^ tmp2 ^ 8;
        xv = __cntlzw(xv);
        r5 = (u32)(8u << xv) >> 31;
    }

    r30 = fn_801DE190((u16)r28, r30, r5);

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8a; } }
    result = fn_8011B67C((u8*)arg, 8);
    check8a: if ((u8)result == 1) { fn_801DA3CC((void*)r30, 1); }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7a; } }
    result = fn_8011B67C((u8*)arg, 7);
    check7a: if ((u8)result == 1) { fn_801DA3CC((void*)r30, 2); }

    if (arg == NULL) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C((u8*)arg, 3);
    check3: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C((u8*)arg, 4);
    check4: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C((u8*)arg, 5);
    check5: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C((u8*)arg, 6);
    check6: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7b; } }
    result = fn_8011B67C((u8*)arg, 7);
    check7b: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8b; } }
    result = fn_8011B67C((u8*)arg, 8);
    check8b: if ((u8)result == 1) { acc = 0; goto done; }

    acc = 1;
    done:
    if ((u8)acc == 1) {
        fn_801DA36C((void*)r30, 1);
        fn_801DA36C((void*)r30, 2);
    }
    return (void*)r30;
}
#if 0
void fn_80121C18_old(void) {
    extern void fn_80119ED0();
    extern void fn_8011B67C();
    extern u32 fn_8012640C();
    extern void fn_801DA36C();
    extern void fn_801DA3CC();
    extern void fn_801DE190();
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    r31 = r3;
    fn_8012640C();
    r0 = r3;
    r3 = r31;
    r30 = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r4 = r28;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8012640C();
    if (r3 == (u32)0x0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r3 = lbl_80478F90;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if (r28 >= (u32)r0) {
        r0 = 0x0;
        goto L_80121CB4;
    }
    r0 = 0x1;
L_80121CB4: ;
do {
    r0 = r0 & 0xFF;
    if (r28 == (u32)r0) {
        r3 = 0x0;
        return;
    }
    r4 = r28;
    r3 = 0x0;
    r5 = 0x66;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3;
    if (r31 == (u32)0x0) {
        r5 = 0x0;
    } else {
        r3 = r31;
        r4 = 0x0;
        r5 = 0x75;
        r6 = 0x0;
        fn_8012640C();
        r29 = r3;
        r3 = r31;
        r4 = 0x0;
        r5 = 0x6f;
        r6 = 0x0;
        fn_8012640C();
        r4 = (u32)r29 >> 16;
        r0 = r29 & 0xFFFF;
        r5 = (u32)r3 >> 16;
        r6 = r3 & 0xFFFF;
        r0 = r4 ^ r0;
        r3 = 0x8;
        r0 = r5 ^ r0;
        r0 = r6 ^ r0;
        r0 = r3 ^ r0;
        r0 = __cntlzw(r0);
        r0 = r3 << r0;
        r5 = (u32)r0 >> 31;
    }
    r4 = r30;
    r3 = r28 & 0xFFFF;
    fn_801DE190();
    r0 = r3;
    r3 = 0x8;
    r30 = r0;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x1;
        fn_801DA3CC();
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x2;
        fn_801DA3CC();
    }
    if (r31 == (u32)0x0) {
        r0 = 0x0;
        break;
    }
    r3 = 0x3;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x3, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x3;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r3 = 0x4;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x4, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x4;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r3 = 0x5;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x5, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x5;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r3 = 0x6;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x6, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x6;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r3 = 0x7;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x7, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x7;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r3 = 0x8;
    fn_80119ED0();
    r0 = r3 & 0xFFFF;
    if (r0 == (u32)0x7c || ((r3 = 0x8, fn_80119ED0(), r0 = r3 & 0xFFFF), r0 == (u32)0xc8)) {
        r3 = r31;
        r4 = 0x8;
        fn_8011B67C();
    } else {
        r3 = 0x0;
    }
    r0 = r3 & 0xFF;
    if (r0 == (u32)0x1) {
        r0 = 0x0;
        break;
    }
    r0 = 0x1;
} while (0);
    r0 = r0 & 0xFF;
    if (r0 == (u32)0x1) {
        r3 = r30;
        r4 = 0x1;
        fn_801DA36C();
        r3 = r30;
        r4 = 0x2;
        fn_801DA36C();
    }
    r3 = r30;
    return;
}
#endif
#endif
/* 0x80122040 | 0x2F4 */
#if 0
asm void fn_80122040(void) {
#include "src/game/gs_field_world_fn_80122040.inc"
}
#else
void fn_80122040(u8* ptr, void* obj) {
    extern u16 fn_80119ED0(u32 val);
    extern u32 fn_8011B67C(u8* ptr, u32 val);
    extern void fn_801DA36C(void* obj, u32 val);
    extern void fn_801DA3CC(void* obj, u32 val);
    u32 result;
    u32 acc;

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8a; } }
    result = fn_8011B67C(ptr, 8);
    check8a: if ((u8)result == 1) { fn_801DA3CC(obj, 1); }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7a; } }
    result = fn_8011B67C(ptr, 7);
    check7a: if ((u8)result == 1) { fn_801DA3CC(obj, 2); }

    if (ptr == NULL) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 3);
    check3: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 4);
    check4: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C(ptr, 5);
    check5: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C(ptr, 6);
    check6: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7b; } }
    result = fn_8011B67C(ptr, 7);
    check7b: if ((u8)result == 1) { acc = 0; goto done; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8b; } }
    result = fn_8011B67C(ptr, 8);
    check8b: if ((u8)result == 1) { acc = 0; goto done; }

    acc = 1;
    done:
    if ((u8)acc == 1) {
        fn_801DA36C(obj, 1);
        fn_801DA36C(obj, 2);
    }
}
#endif
/* 0x80122370 | 0x360 */
extern void fn_80135530(void);
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80122370(void);
/* 0x801226D0 | 0x324 */
extern u8 lbl_80272948[];
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801226D0(void);
/* 0x80122BC0 | 0xA4 */
#if 0
asm void fn_80122BC0(void) {
#include "src/game/gs_field_world_fn_80122BC0.inc"
}
#else
#pragma optimization_level 4
u8 fn_80122BC0(u8* ptr, s32 b) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    s32 val1;
    s32 val2;

    if (!(u16)b) {
        return 0;
    }
    if (ptr == NULL) {
        return 0;
    }
    val1 = (s32)(u16)fn_8012640C(ptr, 0, 0x83, 0);
    val2 = (s32)(u16)fn_8012640C(ptr, 0, 0x87, 0);
    return (u8)((s32)val2 / (s32)(u16)b >= val1);
}
#endif
/* 0x80122C64 | 0x178 */
#if 0
asm void fn_80122C64(void) {
#include "src/game/gs_field_world_fn_80122C64.inc"
}
#else
u32 fn_80122C64(u8* ptr) {
    extern u16 fn_80119ED0(u32 val);
    extern u8 fn_8011B67C(u8* ptr, u32 val);
    u8 result;
    if (ptr == NULL) { return 0; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check1; } }
    result = fn_8011B67C(ptr, 3);
    check1: if ((u8)result == 1) { return 1; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check2; } }
    result = fn_8011B67C(ptr, 5);
    check2: if ((u8)result == 1) { return 1; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 6);
    check3: if ((u8)result == 1) { return 1; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 4);
    check4: if ((u8)result == 1) { return 1; }

    return 0;
}
#endif
/* 0x80122DDC | 0x218 */
#if 0
asm void fn_80122DDC(void) {
#include "src/game/gs_field_world_fn_80122DDC.inc"
}
#else
u32 fn_80122DDC(u8* ptr) {
    extern u16 fn_80119ED0(u32 val);
    extern u32 fn_8011B67C(u8* ptr, u32 val);
    u32 result;
    if (ptr == NULL) { return 0; }

    if ((u16)fn_80119ED0(3) != 0x7c) { if ((u16)fn_80119ED0(3) != 0xc8) { result = 0; goto check3; } }
    result = fn_8011B67C(ptr, 3);
    check3: if ((u8)result == 1) { return 0x0; }

    if ((u16)fn_80119ED0(4) != 0x7c) { if ((u16)fn_80119ED0(4) != 0xc8) { result = 0; goto check4; } }
    result = fn_8011B67C(ptr, 4);
    check4: if ((u8)result == 1) { return 0x0; }

    if ((u16)fn_80119ED0(5) != 0x7c) { if ((u16)fn_80119ED0(5) != 0xc8) { result = 0; goto check5; } }
    result = fn_8011B67C(ptr, 5);
    check5: if ((u8)result == 1) { return 0x0; }

    if ((u16)fn_80119ED0(6) != 0x7c) { if ((u16)fn_80119ED0(6) != 0xc8) { result = 0; goto check6; } }
    result = fn_8011B67C(ptr, 6);
    check6: if ((u8)result == 1) { return 0x0; }

    if ((u16)fn_80119ED0(7) != 0x7c) { if ((u16)fn_80119ED0(7) != 0xc8) { result = 0; goto check7; } }
    result = fn_8011B67C(ptr, 7);
    check7: if ((u8)result == 1) { return 0x0; }

    if ((u16)fn_80119ED0(8) != 0x7c) { if ((u16)fn_80119ED0(8) != 0xc8) { result = 0; goto check8; } }
    result = fn_8011B67C(ptr, 8);
    check8: if ((u8)result == 1) { return 0x0; }

    return 1;
}
#endif
/* 0x80122FF4 | 0x9C */
extern void fn_80008154(void);
extern void fn_80142CF4(void);
#if 0
asm void fn_80122FF4(void) {
#include "src/game/gs_field_world_fn_80122FF4.inc"
}
#else
u16 fn_80122FF4(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_80008154(void);
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    u16 val;
    u16 check;
    val = (u16)fn_8012640C(ptr, 0, 0x82, 0);
    if (val == 0) { return 0; }
    if ((u8)fn_80008154() == 1) {
        check = (u16)itemGetStatus(0, val, 7, 0);
        if (check == 0x1a || check == 0x1e) {
            return 0x63;
        }
    }
    return (u16)itemGetStatus(0, val, 0xa, 0);
}
#endif
/* 0x801230E0 | 0x30 */
u16 fn_801230E0(u32 arg) {
    extern u32 fn_8012640C(u32 a, u32 b, u32 c, u32 d);
    return (u16)fn_8012640C(arg, 0, 0x82, 0);
}
/* 0x80123110 | 0x94 */
#if 0
asm void fn_80123110(void) {
#include "src/game/gs_field_world_fn_80123110.inc"
}
#else
u32 fn_80123110(u8* ptr, register u32 arg2, u8 flag) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u16 e);
    if (ptr == NULL) { return 0; }
    if ((u8)flag == 0) {
        arg2 = (u16)fn_8012640C(ptr, 0, 0x82, 0);
        fn_801254B4(ptr, 0, 0x82, 0, 0);
    } else {
        fn_801254B4(ptr, 0, 0x82, 0, arg2);
    }
    return arg2;
}
#endif
/* 0x801231A4 | 0x13C */
extern u32 fn_80131574(u32);
#if 0
asm void fn_801231A4(void) {
#include "src/game/gs_field_world_fn_801231A4.inc"
}
#else
u8 fn_801231A4(u8* ptr)
{
    u32 count;
    u16 threshold;
    u16 wanted;
    s32 result;

    if (ptr == NULL) {
        return 2;
    }

    count = fn_8012640C(ptr, 0, 0x6F, 0);
    threshold = (u16)fn_8012640C(0, (u16)fn_8012640C(ptr, 0, 0x6E, 0), 0x13, 0);
    if (ptr == NULL) {
        result = 2;
    } else {
        wanted = (u16)fn_8012640C(0, (u16)fn_8012640C(ptr, 0, 0x6E, 0), 0x13, 0);
        if ((s32)wanted == (s32)(u8)fn_80131574(0)) {
            result = 0;
        } else if ((s32)wanted == (s32)(u8)fn_80131574(1)) {
            result = 1;
        } else if ((s32)wanted == (s32)(u8)fn_80131574(2)) {
            result = 2;
        } else {
            result = -1;
        }
    }
    if ((s8)result >= 0) {
        goto done;
    }
    if ((u16)threshold <= (u16)(u8)count) {
        goto zero;
    }
    result = 1;
    goto done;
zero:
    result = 0;
done:
    return (u8)result;
}
#endif
/* 0x801232E0 | 0x88 */
extern void fn_801EE958(void);
extern void fn_801EEB34(void);
#if 0
asm void fn_801232E0(void) {
#include "src/game/gs_field_world_fn_801232E0.inc"
}
#else
void fn_801232E0(u8* ptr, u8 flag) {
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    extern void fn_801EE958(u16 val, u32 a);
    extern void fn_801EEB34(u16 val, u32 a);
    u16 val;
    if (ptr == NULL) { return; }
    if ((u8)fn_8012640C(ptr, 0, 0xc2, 0) == 0) { return; }
    val = (u16)fn_8012640C(ptr, 0, 0xc3, 0);
    fn_801EE958(val, 1);
    if ((u8)flag == 0) { return; }
    fn_801EEB34(val, 1);
}
#endif
/* 0x80123368 | 0x8C */
extern void memoDataSet(void);
#if 0
asm void fn_80123368(void) {
#include "src/game/gs_field_world_fn_80123368.inc"
}
#else
void fn_80123368(u8* ptr, u8 flag) {
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    extern void memoDataSet(u32 a, u8* b);
    extern void fn_801254B4(u8* ptr, u32 a, u32 b, u32 c, u32 d);
    if (ptr == NULL) { return; }
    fn_8012640C(ptr, 0, 0x6e, 0);
    memoDataSet(0, ptr);
    fn_801254B4(ptr, 0, 0x62, 0, 1);
    if ((u8)flag == 0) { return; }
    fn_801254B4(ptr, 0, 0x63, 0, 1);
}
#endif
/* 0x801233F4 | 0x190 */
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801233F4(void);
/* 0x80123584 | 0x98 */
#if 0
asm void fn_80123584(void) {
#include "src/game/gs_field_world_fn_80123584.inc"
}
#else
u32 fn_80123584(u8* ptr, u32 arg2) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    u32 val;
    u8 target;
    u32 i;
    if (ptr == NULL) { return 0; }
    val = fn_8012640C(ptr, 0, 0x6e, 0) & 0xFFFF;
    target = (u8)arg2;
    i = 0;
    while ((u8)i < 0x14) {
        if ((s32)fn_8012640C(NULL, val, 0x1d, i & 0xFF) == (s32)target) { break; }
        i++;
    }
    return i;
}
#endif
/* 0x8012361C | 0xDC */
#if 0
asm void fn_8012361C(void) {
#include "src/game/gs_field_world_fn_8012361C.inc"
}
#else
s32 fn_8012361C(u8* ptr, u8 target, u8* buf_ptr, u8* counter_ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern s32 fn_801237B8(u8* a, u16 b, u8* c);
    u16 val;
    u16 result;
    if (ptr == NULL) { return -2; }
    if (ptr != NULL && counter_ptr == NULL) {
        result = 0;
        goto _check;
    }
    val = (u16)fn_8012640C(ptr, 0, 0x6e, 0);
    target = (u8)target;
    while (*counter_ptr < 0x14) {
        if ((s32)fn_8012640C(NULL, val, 0x1d, *counter_ptr) == (s32)target) {
            result = (u16)fn_8012640C(NULL, val, 0x1e, *counter_ptr);
            goto _check;
        }
        (*counter_ptr)++;
    }
    result = 0;
_check:
    if ((u16)result == 0) { return -3; }
    return fn_801237B8(ptr, result, buf_ptr);
}
#endif
/* 0x801236F8 | 0xC0 */
#if 0
asm void fn_801236F8(void) {
#include "src/game/gs_field_world_fn_801236F8.inc"
}
#else
u16 fn_801236F8(u8* ptr, u8 arg2, u8* counter_ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    u16 val;
    if (ptr == NULL || counter_ptr == NULL) { return 0; }
    val = (u16)fn_8012640C(ptr, 0, 0x6e, 0);
    arg2 = (u8)arg2;
    while (*counter_ptr < 0x14) {
        if ((s32)fn_8012640C(NULL, val, 0x1d, *counter_ptr) == (s32)arg2) {
            return (u16)fn_8012640C(NULL, val, 0x1e, *counter_ptr);
        }
        (*counter_ptr)++;
    }
    return 0;
}
#endif
/* 0x801237B8 | 0x3A4 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801237B8(void);
/* 0x80123B5C | 0xF8 */
#if 0
asm void fn_80123B5C(void) {
#include "src/game/gs_field_world_fn_80123B5C.inc"
}
#else
s32 fn_80123B5C(u8* ptr, u16 target) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    s8 i;
    s32 ext_i;
    u8 flag;
    if (ptr == NULL) { return -1; }
    for (i = 0; (s8)i < 4; i++) {
        ext_i = (s8)i;
        if (ptr == NULL) {
            flag = 0;
        } else {
            if ((s32)fn_8012640C(ptr, 0, 0x7f, ext_i) == 0) {
                flag = 0;
            } else if ((s32)fn_8012640C(ptr, 0, 0x7f, ext_i) == 0x163) {
                flag = 0;
            } else {
                flag = 1;
            }
        }
        if ((u8)flag != 0) {
            if ((s32)fn_8012640C(ptr, 0, 0x7f, (s8)i) == (s32)target) {
                return i;
            }
        }
    }
    return -1;
}
#endif
/* 0x80 | fn_80123C54 | generic */
void fn_80123C54(void* ptr, u32 idx, u32 arg) {
    extern u32* fn_8011F260(void* a, u32 b, u32 c);
    extern void fn_8011F5E0(u32* dst, u32* src);
    u32 tmp;
    u32* val1;
    u32* val2;
    if (ptr == NULL) { return; }
    val1 = fn_8011F260(ptr, idx, 0);
    val2 = fn_8011F260(ptr, arg, 0);
    fn_8011F5E0(&tmp, val1);
    fn_8011F5E0(val1, val2);
    fn_8011F5E0(val2, &tmp);
}
/* 0x80123CD4 | 0x84 */
#if 0
asm void fn_80123CD4(void) {
#include "src/game/gs_field_world_fn_80123CD4.inc"
}
#else
u32 fn_80123CD4(u8* ptr, u32 arg2) {
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    if (ptr == NULL) { return 0; }
    if ((s32)fn_8012640C(ptr, 0, 0x7f, arg2) == 0) { return 0; }
    return fn_8012640C(ptr, 0, 0x7f, arg2) != 0x163;
}
#endif
/* 0x80123D58 | 0x118 */
#if 0
asm void fn_80123D58(void) {
#include "src/game/gs_field_world_fn_80123D58.inc"
}
#else
void fn_80123D58(u8* ptr, u32 slot, u32 val) {
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_8011BB6C(u16 a, u8 b);
    u16 slot2;
    u16 result;
    u8 byte;
    if (ptr == NULL) { return; }
    if (ptr != NULL) {
        fn_801254B4(ptr, 0, 0x7f, slot, 0);
        fn_801254B4(ptr, 0, 0x80, slot, 0);
        fn_801254B4(ptr, 0, 0x81, slot, 0);
    }
    fn_801254B4(ptr, 0, 0x7f, slot, val);
    if (ptr == NULL) {
        byte = 0;
    } else {
        slot2 = (u16)slot + 4;
        result = (u16)fn_8012640C(ptr, 0, 0x7f, slot2);
        byte = (u8)fn_8011BB6C(result, (u8)fn_8012640C(ptr, 0, 0x81, slot2));
    }
    fn_801254B4(ptr, 0, 0x80, slot, (u8)byte);
}
#endif
/* 0x80123EF0 | 0xCC */
#if 0
asm void fn_80123EF0(void) {
#include "src/game/gs_field_world_fn_80123EF0.inc"
}
#else
void fn_80123EF0(u8* arg1, u32 arg2, u8 arg3, u16 arg4, u8 arg5, u32 arg6, u32 arg7) {
    extern void fn_801254B4(u8* ptr, u32 a, u32 b, u32 c, u32 d);
    if (arg1 == NULL) { return; }
    fn_801254B4(arg1, 0, 0x71, 0, arg2);
    fn_801254B4(arg1, 0, 0x72, 0, (u32)arg3);
    fn_801254B4(arg1, 0, 0x73, 0, (u32)arg4);
    fn_801254B4(arg1, 0, 0x74, 0, (u32)arg5);
    fn_801254B4(arg1, 0, 0x75, 0, arg6);
    fn_801254B4(arg1, 0, 0x76, 0, arg7);
}
#endif
/* 0x80123FBC | 0x108 */
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
#if 0
asm void fn_80123FBC(void) {
#include "src/game/gs_field_world_fn_80123FBC.inc"
}
#else
u32 fn_80123FBC(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 gamedataAttestCheckValid(u32 val);
    u16 val;
    u8 flag;
    if (ptr == NULL) { return 0; }
    val = (u16)fn_8012640C(ptr, 0, 0x6e, 0);
    if (val == 0) { return 0; }
    flag = 0;
    if (val == 0) {
        flag = 0;
    } else {
        if ((u32)fn_8012640C(NULL, val, 1, 0) == 0) { flag = 0; }
        else if (val >= *(u32*)(u32)lbl_80478F90) { flag = 0; }
        else { flag = 1; }
    }
    if ((u8)flag == 0) { return 0; }
    if ((u8)gamedataAttestCheckValid(fn_8012640C(ptr, 0, 0x70, 0)) == 0) { return 0; }
    return (u8)fn_8012640C(ptr, 0, 0xb8, 0) != 1;
}
#endif
/* 0x801240C4 | 0x34C */
extern void fn_80135AD0(void);
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801240C4(void);
/* 0x80124410 | 0x4B4 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80124410(void);
/* 0x801248C4 | 0xB4 */
#if 0
asm void fn_801248C4(void) {
#include "src/game/gs_field_world_fn_801248C4.inc"
}
#else
u16 fn_801248C4(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    u32 val;
    if (ptr == NULL) { return 0; }
    val = fn_8012640C(ptr, 0, 0x6e, 0) & 0xFFFF;
    if ((s32)fn_8012640C(NULL, val, 0x17, 1) == 0) {
        return (u16)fn_8012640C(NULL, val, 0x17, 0);
    }
    return (u16)fn_8012640C(NULL, val, 0x17, (u8)fn_8012640C(ptr, 0, 0xb7, 0));
}
#endif
/* 0x68 | fn_801249F8 | generic */
#if 0
asm void fn_801249F8(void) {
#include "src/game/gs_field_world_fn_801249F8.inc"
}
#else
void fn_801249F8(u8* ptr, u16 count) {
    extern void fn_80124A60(u8* ptr);
    u16 i;
    if (ptr == NULL) { return; }
    for (i = 0; i < count; i++) {
        fn_80124A60(ptr + (u16)i * 0x138);
    }
}
#endif
/* 0x80125238 | 0xA8 */
#if 0
asm void fn_80125238(void) {
#include "src/game/gs_field_world_fn_80125238.inc"
}
#else
void fn_80125238(u8* ptr) {
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern void fn_8011B950(u8* a, u32 b);
    fn_801254B4(ptr, 0, 0xc3, 0, 0);
    if (ptr != NULL) {
        fn_801254B4(ptr, 0, 0xc5, 0, (u32)-100);
    }
    fn_801254B4(ptr, 0, 0xc6, 0, 0);
    fn_801254B4(ptr, 0, 0xc7, 0, 0);
    fn_8011B950((u8*)fn_8012640C(ptr, 0, 0xc8, 0), 1);
}
#endif
/* 0x801252E0 | 0x34 */
void fn_801252E0(u8* ptr) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern void fn_8011B950(u8* a, u32 b);
    fn_8011B950((u8*)fn_8012640C(ptr, 0, 0x7c, 0), 1);
}
/* 0x7C | fn_80125314 | generic */
void fn_80125314(u8* ptr, u32 arg2) {
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    if (ptr == NULL) { return; }
    fn_801254B4(ptr, 0, 0x7f, arg2, 0);
    fn_801254B4(ptr, 0, 0x80, arg2, 0);
    fn_801254B4(ptr, 0, 0x81, arg2, 0);
}
/* 0x80125390 | 0x94 */
#pragma push
#pragma opt_propagation off
#if 0
asm void fn_80125390(void) {
#include "src/game/gs_field_world_fn_80125390.inc"
}
#else
/* w_sg2 2026-06-18: 100% byte-exact. Levers: inline a/b halves (correct emission order)
   + `eight` as a named temp (forces shared register for the `^8` xor and the `8<<n` slw)
   + declare `eight` BEFORE `lb` so it claims r3 (b's freed reg) while lb takes a fresh r6. */
u32 fn_80125390(void* ctx) {
    extern u32 fn_8012640C(void* a, u32 b, u32 c, u32 d);
    u32 a;      /* r31 */
    u32 b;
    u32 eight;  /* declared BEFORE lb so it claims r3 (b's freed reg); lb -> r6 */
    u32 lb;
    if (ctx == NULL) {
        return 0;
    }
    a = fn_8012640C(ctx, 0, 0x75, 0);
    b = fn_8012640C(ctx, 0, 0x6F, 0);
    eight = 8;
    lb = b & 0xFFFF;
    return (u32)(eight << __cntlzw((a >> 16) ^ (a & 0xFFFF) ^ (b >> 16) ^ lb ^ eight)) >> 31;
}
#endif
#pragma pop
/* 0x48 | fn_80125424 | null_guard_chain */
void fn_80125424(void* ptr, u32 arg2) {
    extern void fn_801254B4();
    if (ptr == NULL) { return; }
    fn_801254B4(ptr, 0, 0x79, 0, arg2);
    fn_8012546C(ptr);
}
/* 0x48 | fn_8012546C | null_guard_chain */
void fn_8012546C(void* ptr) {
    extern u32 fn_8012640C();
    extern void fn_8012795C();
    u8 val;
    if (ptr == NULL) { return; }
    val = fn_8012640C(ptr, 0, 0xC0, 0);
    fn_8012795C(ptr, val);
}
/* 0x8012795C | 0x700 */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012795C(void);
/* 0x8012805C | 0x2A4 */
extern void fadeSet(void);
extern void fadeCheck(void);
extern void evolutionOpen(void);
extern f32 lbl_8047D020;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012805C(void);
/* 0x80128300 | 0x224 */
extern u8 lbl_802729A4[];
extern u8 lbl_80272998[];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80128300(void);
/* 0x80128524 | 0x1A4 */
extern void fn_800F9EE4(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80128524(void);
/* 0x801286C8 | 0x39C */
extern void jumptable_80363468();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801286C8(void);
/* 0x80128A64 | 0x25C */
extern void fn_80143F6C(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80128A64(void);
/* 0x80128CC0 | 0x1C */
void* fn_80128CC0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1C45C;
}
/* 0x80128CDC | 0x1C */
void* fn_80128CDC(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1B014;
}
/* 0x80128CF8 | 0x1C */
void* fn_80128CF8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xE3E8;
}
/* 0x80128D14 | 0x1C */
void* fn_80128D14(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x9A1C;
}
/* 0x80128D30 | 0x1C */
void* fn_80128D30(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x82A8;
}
/* 0x80128D4C | 0x1C */
void* fn_80128D4C(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8168;
}
/* 0x80128D68 | 24 bytes | nc_addi_ptr */
void* fn_80128D68(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x7D20;
}
/* 0x80128D80 | 0x1C */
void* fn_80128D80(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BE5C;
}
/* 0x80128D9C | 0x1C */
void* fn_80128D9C(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BDDC;
}
/* 0x80128DB8 | 0x1C */
void* fn_80128DB8(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x1BDBC;
}
/* 0x80128DD4 | 24 bytes | nc_addi_ptr */
void* fn_80128DD4(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xB88;
}
/* 0x80128DEC | 24 bytes | nc_addi_ptr */
void* fn_80128DEC(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x70;
}
/* 0x80128E04 | 16 bytes | nc_bnelr */
u32 fn_80128E04(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
/* 0x80128E14 | 0x10 */
void fn_80128E14(void* ptr) {
    if (ptr == NULL) { return; }
    lbl_8047ADB8 = (u32)ptr;
}
/* 0x80128E2C | 0xC */
extern u8 lbl_80408400[];
void* fn_80128E2C(void) {
    return (void*)lbl_80408400;
}
/* 0x80128E38 | 0x25C */
extern void fn_8013528C(void);
extern void fn_800F9D04(void);
extern void fn_80135030(void);
extern u8 lbl_8047D028[8];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80128E38(void);
/* 0x80129094 | 0x1EC */
extern void fn_80135338(void);
extern void fn_80134F88(void);
extern void fn_801908D4(void);
extern void fn_801D1FA8(void);
extern void sodateyaInit(void);
extern void fn_8006B6B4(void);
extern void fn_80260070(void);
extern void fn_80083CBC(void);
extern void fn_801EF128(void);
extern void exribbonInit(void);
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80129094(void);
/* 0x80129280 | 0x104 */
extern void jumptable_803634A8();
#if 0
asm void fn_80129280(void) {
#include "src/game/gs_field_world_fn_80129280.inc"
}
#else
u32 fn_80129280(u8* arg1, u16 arg2) {
    extern u8* fn_80128E24(u8* ptr);
    extern u32 fn_80128E04(u8* ptr);
    extern u32 fn_80128DEC(u8* ptr);
    extern u32 fn_80128DD4(u8* ptr);
    extern u32 fn_80128DB8(u8* ptr);
    extern u32 fn_80128D9C(u8* ptr);
    extern u32 fn_80128D80(u8* ptr);
    extern u32 fn_80128D68(u8* ptr);
    extern u32 fn_80128D4C(u8* ptr);
    extern u32 fn_80128D30(u8* ptr);
    extern u32 fn_80128D14(u8* ptr);
    extern u32 fn_80128CF8(u8* ptr);
    extern u32 fn_80128CDC(u8* ptr);
    extern u32 fn_80128CC0(u8* ptr);
    if ((u32)arg2 >= 0x11) { return 0; }
    if (arg1 == NULL) {
        arg1 = fn_80128E24(arg1);
        if (arg1 == NULL) { return 0; }
    }
    switch (arg2) {
        case 0x0: return (u32)arg1;
        case 0x1: return fn_80128E04(arg1);
        case 0x2: return fn_80128DEC(arg1);
        case 0x3: return fn_80128DD4(arg1);
        case 0x4: return fn_80128DB8(arg1);
        case 0x5: return fn_80128D9C(arg1);
        case 0x6: return fn_80128D80(arg1);
        case 0x7: return 0x8;
        case 0x8: return 0x20;
        case 0x9: return 0x180;
        case 0xa: return fn_80128D68(arg1);
        case 0xb: return fn_80128D4C(arg1);
        case 0xc: return fn_80128D30(arg1);
        case 0xd: return fn_80128D14(arg1);
        case 0xe: return fn_80128CF8(arg1);
        case 0xf: return fn_80128CDC(arg1);
        case 0x10: return fn_80128CC0(arg1);
        default: return 0;
    }
}
#endif
/* 0x78 | heroDecPokecoupon | multi_call_guarded */
void heroDecPokecoupon(u8* ptr, s32 offset) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern void fn_8012A450(u8* ptr, u32 a, u32 b);
    fn_8012A450(ptr, 0xd, fn_8012A5B0(ptr, 0xd, 0) - offset);
    if (offset <= 0) {
        fn_8012A450(ptr, 0xe, fn_8012A5B0(ptr, 0xe, 0) - offset);
    }
}
/* 0x78 | heroAddPokecoupon | multi_call_guarded */
void heroAddPokecoupon(u8* ptr, s32 offset) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern void fn_8012A450(u8* ptr, u32 a, u32 b);
    u32 val;
    val = fn_8012A5B0(ptr, 0xd, 0);
    val += offset;
    fn_8012A450(ptr, 0xd, val);
    if (offset >= 0) {
        val = fn_8012A5B0(ptr, 0xe, 0);
        val += offset;
        fn_8012A450(ptr, 0xe, val);
    }
}
/* 0x50 | heroDecPokedoru | call_sequence */
#if 0
asm void heroDecPokedoru(void) {
#include "src/game/gs_field_world_fn_80129474.inc"
}
#else
void heroDecPokedoru(u8* ptr, u32 offset) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern void fn_8012A450(u8* ptr, u32 a, u32 b);
    fn_8012A450(ptr, 0xc, fn_8012A5B0(ptr, 0xc, 0) - offset);
}
#endif
/* 0x50 | heroAddPokedoru | call_sequence */
#if 0
asm void heroAddPokedoru(void) {
#include "src/game/gs_field_world_fn_801294C4.inc"
}
#else
void heroAddPokedoru(u8* ptr, u32 offset) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern void fn_8012A450(u8* ptr, u32 a, u32 b);
    u32 val = fn_8012A5B0(ptr, 0xc, 0);
    val += offset;
    fn_8012A450(ptr, 0xc, val);
}
#endif
/* 0x80129514 | 0x88 */
extern void fn_80140A9C(void);
#if 0
asm void fn_80129514(void) {
#include "src/game/gs_field_world_fn_80129514.inc"
}
#else
void fn_80129514(u8* ptr, s32 arg2, s32 arg3) {
    extern void* fn_8012A5B0(u8* a, u32 b, u32 c);
    extern void fn_80140A9C(u8* a, u8* b);
    u16 local;
    u8* val;
    if (&local != NULL) { local = 0xa; }
    val = (u8*)fn_8012A5B0(ptr, 0xa, 0);
    if (val == NULL) { return; }
    if ((u16)arg2 >= local) { return; }
    if ((u16)arg3 >= local) { return; }
    fn_80140A9C(val + (u16)arg2 * 4, val + (u16)arg3 * 4);
}
#endif
/* 0x8012959C | 0xB4 */
extern void fn_80140ACC(void);
#if 0
asm void fn_8012959C(void) {
#include "src/game/gs_field_world_fn_8012959C.inc"
}
#else
s32 fn_8012959C(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern void* fn_8012A5B0(u8* a, u32 b, u32 c);
    extern s32 fn_80140ACC(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    void* val;
    if (&local_c != NULL) { local_c = 0xa; }
    if (&local_a != NULL) { local_a = 1; }
    if (&local_8 != NULL) { local_8 = 0; }
    val = fn_8012A5B0(ptr, 0xa, 0);
    if (val == NULL) { return -1; }
    return fn_80140ACC(val, local_c, arg2, arg3, arg4, local_a, local_8);
}
#endif
/* 0x80129650 | 0xC8 */
extern void fn_80141308(void);
#if 0
asm void fn_80129650(void) {
#include "src/game/gs_field_world_fn_80129650.inc"
}
#else
s32 fn_80129650(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern void* fn_8012A5B0(u8* a, u32 b, u32 c);
    extern s32 fn_80141308(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g, u8 h);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_9;
    u8 local_8;
    void* val;
    if (&local_c != NULL) { local_c = 0xa; }
    if (&local_a != NULL) { local_a = 1; }
    if (&local_9 != NULL) { local_9 = 0; }
    if (&local_8 != NULL) { local_8 = 1; }
    val = fn_8012A5B0(ptr, 0xa, 0);
    if (val == NULL) { return -1; }
    return fn_80141308(val, local_c, arg2, arg3, arg4, local_a, local_9, local_8);
}
#endif
/* 0x80129718 | 0xC0 */
extern void fn_80142368(void);
#if 0
asm void fn_80129718(void) {
#include "src/game/gs_field_world_fn_80129718.inc"
}
#else
u32 fn_80129718(u8* ptr, u32 arg2) {
    extern void* fn_8012A5B0(u8* a, u32 b, u32 c);
    extern s32 fn_80142368(void* a, u16 b, u32 c, u32 d, u16 e);
    u16 local_a = 0;
    u16 local_8 = 0;
    void* val;
    if (&local_a != NULL) { local_a = 0xa; }
    if (&local_8 != NULL) { local_8 = 1; }
    val = fn_8012A5B0(ptr, 0xa, 0);
    if (val == NULL) { return 0; }
    if ((u32)fn_80142368(val, local_a, arg2, 1, local_8) != 0) { return 1; }
    return fn_80142368(val, local_a, arg2, 2, local_8) != 0;
}
#endif
/* 0x68 | heroHizukiItemGetItemAryPtr | guarded_call */
#if 0
asm void heroHizukiItemGetItemAryPtr(void) {
#include "src/game/gs_field_world_fn_801297D8.inc"
}
#else
u32 heroHizukiItemGetItemAryPtr(u8* ptr, u16* out_a, u16* out_b, u8* out_c, u8* out_d) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    if (out_a != NULL) { *out_a = 0xa; }
    if (out_b != NULL) { *out_b = 1; }
    if (out_c != NULL) { *out_c = 0; }
    if (out_d != NULL) { *out_d = 1; }
    return fn_8012A5B0(ptr, 0xa, 0);
}
#endif
/* 0x78 | heroCheckSetMonohiroiAllTemotiPokemon | generic */
void heroCheckSetMonohiroiAllTemotiPokemon(u8* arg1) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern u8 fn_80123FBC(u32 a);
    extern void fn_80120674(u32 a);
    u32 result;
    u32 i;
    for (i = 0; (u16)i < 6; i++) {
        result = fn_8012A5B0(arg1, 3, i);
        if (fn_80123FBC(result)) {
            fn_80120674(result);
        }
    }
}
/* 0x801298B8 | 0x90 */
extern void fn_80140588(void);
#if 0
asm void fn_801298B8(void) {
#include "src/game/gs_field_world_fn_801298B8.inc"
}
#else
s32 fn_801298B8(u8* ptr, u32 arg2) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* fn_80129BC8(u8* ptr, u8 a, u16* b, u16* c, u32 d, u8* e);
    extern s32 fn_80140588(void* a, u16 b, u32 c, u16 d, u8 e);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    void* result;
    result = fn_80129BC8(ptr, (u8)itemGetStatus(0, arg2, 2, 0), &local_c, &local_a, 0, &local_8);
    if (result == NULL) { return -1; }
    return fn_80140588(result, local_c, arg2, local_a, local_8);
}
#endif
/* 0x80 | fn_80129948 | generic */
void fn_80129948(u8* arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    extern void* fn_80129BC8(u8* a, u32 b, u16* c, u32 d, u32 e, u32 f);
    extern void fn_80140A9C(u8* a, u8* b);
    u16 local_8;
    u8* result;
    result = (u8*)fn_80129BC8(arg1, arg2, &local_8, 0, 0, 0);
    if (result == NULL) { return; }
    if ((u16)arg3 >= local_8) { return; }
    if ((u16)arg4 >= local_8) { return; }
    fn_80140A9C(result + (u16)arg3 * 4, result + (u16)arg4 * 4);
}
/* 0x801299C8 | 0xB0 */
#if 0
asm void fn_801299C8(void) {
#include "src/game/gs_field_world_fn_801299C8.inc"
}
#else
s32 fn_801299C8(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* fn_80129BC8(u8* a, u8 b, u16* c, u16* d, u8* e, u32 f);
    extern s32 fn_80140ACC(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_8;
    u8 tmp;
    void* result;
    tmp = (u8)itemGetStatus(0, arg2, 2, 0);
    result = fn_80129BC8(ptr, tmp, &local_c, &local_a, &local_8, 0);
    if (result == NULL) { return -1; }
    return fn_80140ACC(result, local_c, arg2, arg3, arg4, local_a, local_8);
}
#endif
/* 0x80129A78 | 0xB4 */
#if 0
asm void fn_80129A78(void) {
#include "src/game/gs_field_world_fn_80129A78.inc"
}
#else
s32 fn_80129A78(u8* ptr, u32 arg2, u32 arg3, u32 arg4) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* fn_80129BC8(u8* a, u8 b, u16* c, u16* d, u8* e, u8* f);
    extern s32 fn_80141308(void* a, u16 b, u32 c, u32 d, u32 e, u16 f, u8 g, u8 h);
    u16 local_c = 0;
    u16 local_a = 0;
    u8 local_9;
    u8 local_8;
    u8 tmp;
    void* result;
    tmp = (u8)itemGetStatus(0, arg2, 2, 0);
    result = fn_80129BC8(ptr, tmp, &local_c, &local_a, &local_9, &local_8);
    if (result == NULL) { return -1; }
    return fn_80141308(result, local_c, arg2, arg3, arg4, local_a, local_9, local_8);
}
#endif
/* 0x80129B2C | 0x9C */
#if 0
asm void fn_80129B2C(void) {
#include "src/game/gs_field_world_fn_80129B2C.inc"
}
#else
u32 fn_80129B2C(u8* ptr, u32 arg2) {
    extern u32 itemGetStatus(u32 a, u32 b, u32 c, u32 d);
    extern void* fn_80129BC8(u8* ptr, u8 a, u16* b, u16* c, u32 d, u32 e);
    extern s32 fn_80142368(void* a, u16 b, u32 c, u32 d, u16 e);
    u16 local_a = 0;
    u16 local_8 = 0;
    void* result;
    result = fn_80129BC8(ptr, (u8)itemGetStatus(0, arg2, 2, 0), &local_a, &local_8, 0, 0);
    if (result == NULL) { return 0; }
    return fn_80142368(result, local_a, arg2, 0, local_8) != 0;
}
#endif
/* 0x80129BC8 | 0x19C */
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80129BC8(void);
/* 0x80129D64 | 0xBC */
#if 0
asm void fn_80129D64(void) {
#include "src/game/gs_field_world_fn_80129D64.inc"
}
#else
u32 fn_80129D64(u8* ptr, u8* arg2) {
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_800F9EE4(u32 a, u32 b);
    u32 val1, temp, val2, result2;
    val1 = fn_8012A5B0(ptr, 2, 0);
    temp = fn_8012A5B0(ptr, 1, 0);
    val2 = fn_8012640C(arg2, 0, 0x75, 0);
    result2 = fn_8012640C(arg2, 0, 0x76, 0);
    if (val1 != val2) { return 0; }
    return fn_800F9EE4(temp, result2) == 0;
}
#endif
/* 0x80129E20 | 0x100 */
extern void fn_80134BC0(void);
#if 0
asm void fn_80129E20(void) {
#include "src/game/gs_field_world_fn_80129E20.inc"
}
#else
s32 fn_80129E20(u8* ptr, void* buf, u8 flag) {
    extern void* fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 fn_80123FBC(void* val);
    extern void fn_8011F5FC(void* a, void* b);
    extern u32 fn_80134BC0(u32 a, void* b, s32 c);
    u8 local_buf[0x138];
    u8 i;
    s16 ret;
    void* val;

    if (buf == NULL) { return 6; }
    fn_8011F5FC(local_buf, buf);
    if (&local_buf == NULL) { i = 6; goto after_loop; }
    for (i = 0; i < 6; i++) {
        val = fn_8012A5B0(ptr, 3, i);
        if ((u8)fn_80123FBC(val) != 1) {
            fn_8011F5FC(val, local_buf);
            goto after_loop;
        }
    }
    i = 6;
after_loop:
    ret = (s16)(u8)i;
    if ((u8)i >= 6) {
        if ((u8)flag == 0) { return -2; }
        return (fn_80134BC0(0, local_buf, -1) == 1) ? -1 : -2;
    }
    return ret;
}
#endif
/* 0x80129F20 | 0x16C */
#if 0
asm void fn_80129F20(void) {
#include "src/game/gs_field_world_fn_80129F20.inc"
}
#else
s32 fn_80129F20(u8* ptr, u8* buf, u32 arg3, u16 arg4, u8 flag) {
    extern u32 fn_8012640C(u8* a, u32 b, u32 c, u32 d);
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 fn_80123FBC(u32 val);
    extern void fn_8011F5FC(void* a, void* b);
    extern void fn_80123EF0(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80134BC0(u32 a, void* b, s32 c);
    u8 local_buf[0x138];
    u8 field7a;
    u8 status;
    u32 val2;
    u32 val1;
    u8 i;
    void* slot;

    if (buf == NULL) { return 6; }
    field7a = (u8)fn_8012640C(buf, 0, 0x7a, 0);
    status = (u8)fn_8012A5B0(ptr, 0xb, 0);
    val2 = fn_8012A5B0(ptr, 2, 0);
    val1 = fn_8012A5B0(ptr, 1, 0);
    fn_8011F5FC(local_buf, buf);
    fn_80123EF0(local_buf, arg3, field7a, arg4, status, val2, val1);

    if (&local_buf == NULL) {
        i = 6;
    } else {
        i = 0;
        while ((u8)i < 6) {
            slot = (void*)fn_8012A5B0(ptr, 3, (u8)i);
            if ((u8)fn_80123FBC((u32)slot) != 1) {
                fn_8011F5FC(slot, local_buf);
                break;
            }
            i++;
        }
        if ((u8)i >= 6) {
            i = 6;
        }
    }
    if ((u8)i >= 6) {
        if ((u8)flag == 0) { return -2; }
        return (fn_80134BC0(0, local_buf, -1) == 1) ? -1 : -2;
    }
    return (s16)(u8)i;
}
#endif
/* 0x8012A08C | 0xA4 */
#if 0
asm void fn_8012A08C(void) {
#include "src/game/gs_field_world_fn_8012A08C.inc"
}
#else
u32 fn_8012A08C(u8* ptr, void* arg2) {
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 fn_80123FBC(u32 val);
    extern void fn_8011F5FC(u32 a, void* b);
    u32 val;
    u32 i;
    if (arg2 == NULL) { return 6; }
    i = 0;
    while ((u8)i < 6) {
        val = fn_8012A5B0(ptr, 3, i & 0xFF);
        if ((u8)fn_80123FBC(val) != 1) {
            fn_8011F5FC(val, arg2);
            return i;
        }
        i++;
    }
    return 6;
}
#endif
/* 0x8012A1A4 | 0xA4 */
#if 0
asm void fn_8012A1A4(void) {
#include "src/game/gs_field_world_fn_8012A1A4.inc"
}
#else
void fn_8012A1A4(u8* ptr, u32 arg2, u8 arg3) {
    extern void fn_8012A248(u8* ptr);
    extern u32 fn_800E0C54(void);
    extern void fn_8012A450(u8* ptr, u32 a, u32 b);
    extern u32 fn_800FA280(u32 val);
    u32 lo;
    u32 hi;
    fn_8012A248(ptr);
    lo = fn_800E0C54() & 0xFFFF;
    hi = fn_800E0C54() << 16;
    fn_8012A450(ptr, 2, hi | lo);
    fn_8012A450(ptr, 1, arg2);
    fn_8012A450(ptr, 0xb, arg3);
    fn_8012A450(ptr, 0x17, fn_800FA280(0xfa2));
}
#endif
/* 0x8012A248 | 0x208 */
extern void fn_80142A88(void);
#if 0
asm void fn_8012A248(void) {
#include "src/game/gs_field_world_fn_8012A248.inc"
}
#else
void fn_8012A248(u8* ptr) {
    extern void fn_8012A450(u8* p, u32 a, u32 b);
    extern u8* fn_8012A5B0(u8* p, u32 a, u32 b);
    extern void fn_801249F8(u8* p, u16 count);
    extern void fn_80142A88(u8* p, u32 v);
    u16 local = 0;

    fn_8012A450(ptr, 1, (u32)&local);
    fn_8012A450(ptr, 2, 0);
    fn_801249F8(fn_8012A5B0(ptr, 3, 0), 6);
    fn_80142A88(fn_8012A5B0(ptr, 4, 0), 0x14);
    fn_80142A88(fn_8012A5B0(ptr, 5, 0), 0x2b);
    fn_80142A88(fn_8012A5B0(ptr, 6, 0), 0x10);
    fn_80142A88(fn_8012A5B0(ptr, 7, 0), 0x40);
    fn_80142A88(fn_8012A5B0(ptr, 8, 0), 0x2e);
    fn_80142A88(fn_8012A5B0(ptr, 9, 0), 0x3);
    fn_8012A450(ptr, 0xb, 2);
    fn_8012A450(ptr, 0xc, 0);
    fn_8012A450(ptr, 0xd, 0);
    fn_8012A450(ptr, 0xe, 0);
    fn_8012A450(ptr, 0xf, 1);
    fn_8012A450(ptr, 0x10, 1);
    fn_8012A450(ptr, 0x11, 1);
    fn_8012A450(ptr, 0x12, 1);
    fn_8012A450(ptr, 0x13, 1);
    fn_8012A450(ptr, 0x14, 1);
    fn_8012A450(ptr, 0x15, 1);
    fn_8012A450(ptr, 0x16, 1);
    fn_8012A450(ptr, 0x17, (u32)&local);
    fn_8012A450(ptr, 0x18, 0);
    fn_80142A88(fn_8012A5B0(ptr, 0xa, 0), 0xa);
    fn_8012A450(ptr, 0x19, 0);
}
#endif
/* 0x8012A450 | 0x160 */
extern void jumptable_803634F0();
#if 0
asm void fn_8012A450(void) {
#include "src/game/gs_field_world_fn_8012A450.inc"
}
#else
void fn_8012A450(u8* ptr, u32 selector, u32 value) {
    extern u32 fn_80129280(u8*, u16);
    u16 sel = (u16)selector;

    if (sel == 0) { return; }
    if (sel >= 0x1A) { return; }

    if (ptr == NULL) {
        ptr = (u8*)fn_80129280(NULL, 0);
        if (ptr == NULL) { return; }
        ptr = (u8*)fn_80129280(ptr, 2);
        if (ptr == NULL) { return; }
    }

    switch (sel) {
    case 1:
        heroBiosSetNamePtr(ptr, (void*)value);
        break;
    case 2:
        fn_8012AA54(ptr, value);
        break;
    case 3:
        fn_8012AA44(ptr, (u8)value);
        break;
    case 4:
        fn_8012A86C(ptr, value);
        break;
    case 5:
        fn_8012A824(ptr, value);
        break;
    case 6:
        fn_8012A7DC(ptr, value);
        break;
    case 7:
        fn_8012AA1C(ptr, (u8)value);
        break;
    case 8:
        fn_8012AA0C(ptr, (u8)value);
        break;
    case 9:
        fn_8012A9FC(ptr, (u8)value);
        break;
    case 10:
        fn_8012A9EC(ptr, (u8)value);
        break;
    case 11:
        fn_8012A9DC(ptr, (u8)value);
        break;
    case 12:
        fn_8012A9CC(ptr, (u8)value);
        break;
    case 13:
        fn_8012A9BC(ptr, (u8)value);
        break;
    case 14:
        fn_8012A9AC(ptr, (u8)value);
        break;
    case 15:
        heroBiosSetHizukiNamePtr(ptr, (void*)value);
        break;
    case 16:
        fn_8012A7B4(ptr, (u8)value);
        break;
    case 17:
        fn_8012A774(ptr, (u8)value);
        break;
    }
}
#endif
/* 0x8012A5B0 | 0x1C4 */
extern void jumptable_80363558();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012A5B0(void);
/* 0x8012A7DC | 0x30 */
void fn_8012A7DC(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA8C]) = val;
}
/* 0x8012A824 | 0x30 */
void fn_8012A824(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA88]) = val;
}
/* 0x8012A86C | 0x30 */
void fn_8012A86C(u8* ptr, s32 val) {
    if (ptr == NULL) { return; }
    if (val < 0) { val = 0; }
    if (val > 0x98967F) { val = 0x98967F; }
    *(s32*)(&ptr[0xA84]) = val;
}
/* 0x8012A89C | 0x38 */
void heroBiosSetHizukiNamePtr(u8* ptr, void* src) {
    if (ptr == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(ptr + 0xAC2, src, 0xB);
}
/* 0x8012A8D4 | 24 bytes | nc_addi_ptr */
void* fn_8012A8D4(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0xAC2;
}
/* 0x8012AA64 | 0x38 */
void heroBiosSetNamePtr(void* dst, void* src) {
    extern void GScharLenCpy();
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
    GScharLenCpy(dst, src, 0xB);
}
/* 0x8012AA9C | 0x34 */
void* fn_8012AA9C(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0xA) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA9A;
}
/* 0x8012AAD0 | 0x34 */
void* fn_8012AAD0(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x3) { return NULL; }
    return ptr + (u32)idx * 4 + 0xA74;
}
/* 0x8012AB04 | 0x34 */
void* fn_8012AB04(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2E) { return NULL; }
    return ptr + (u32)idx * 4 + 0x9BC;
}
/* 0x8012AB38 | 0x34 */
void* fn_8012AB38(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x40) { return NULL; }
    return ptr + (u32)idx * 4 + 0x8BC;
}
/* 0x8012AB6C | 0x34 */
void* fn_8012AB6C(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x10) { return NULL; }
    return ptr + (u32)idx * 4 + 0x87C;
}
/* 0x8012ABA0 | 0x34 */
void* fn_8012ABA0(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x2B) { return NULL; }
    return ptr + (u32)idx * 4 + 0x7D0;
}
/* 0x8012ABD4 | 0x34 */
void* fn_8012ABD4(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 0x14) { return NULL; }
    return ptr + (u32)idx * 4 + 0x780;
}
/* 0x8012AC08 | 0x34 */
void* fn_8012AC08(u8* ptr, u16 idx) {
    if (ptr == NULL) { return NULL; }
    if (idx >= 6) { return NULL; }
    return ptr + (u32)idx * 0x138 + 0x30;
}
/* 0x8012AC54 | 16 bytes | nc_bnelr */
u32 fn_8012AC54(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
/* 0x8012AC64 | 0x38 */
#ifndef PCPORT
typedef struct { u32 data[0x2C6]; } GfwBuf0xB18;
#endif
void fn_8012AC64(u32* dst, u32* src) {
#ifdef PCPORT
    u32 i;
#endif
    if (dst == NULL) { return; }
    if (src == NULL) { return; }
#ifdef PCPORT
    for (i = 0; i < 0x2C6; i++) {
        dst[i] = src[i];
    }
#else
    *(GfwBuf0xB18*)dst = *(GfwBuf0xB18*)src;
#endif
}
/* 0x8012AC9C | 0xB4 */
extern u8 lbl_80426BD0[];
#if 0
asm void fn_8012AC9C(void) {
#include "src/game/gs_field_world_fn_8012AC9C.inc"
}
#else
void fn_8012AC9C(void) {
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 fn_80123FBC(u32 val);
    extern void fn_8011F1A0(u32 val);
    extern void* fn_801440A0(void);
    extern u32 fn_80143F6C(void* a);
    extern void fn_80122370(u32 a, u32 b, u32 c);
    u32* counter = (u32*)(lbl_80426BD0 + 0x184);
    u32 val;
    s32 i;
    u32 obj;
    void* result;

    *counter = *counter + 1;
    if ((s32)*counter < 0x100) { return; }
    *counter = 0;
    i = 0;
    do {
        obj = fn_8012A5B0(NULL, 3, (u16)i);
        if (obj != 0) {
            if ((u8)fn_80123FBC(obj) != 0) {
                fn_8011F1A0(obj);
                result = fn_801440A0();
                if (result == NULL) {
                    val = 0;
                } else {
                    val = fn_80143F6C(result);
                }
                fn_80122370(obj, val, 5);
            }
        }
        i++;
    } while (i < 6);
}
#endif
/* 0x8012AD50 | 0x434 */
extern void fadeEffectDokuStart(void);
extern void fn_8018C69C(void);
extern void fn_8018CA20(void);
extern void fn_801067E8(void);
extern void fn_801065B8(void);
extern void fn_801D0AFC(void);
extern void fn_80113FE8(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012AD50(void);
/* 0x8012B184 | 0x18 */
extern u8 lbl_80426BD0[];
void fn_8012B184(s32 val) {
    if (val < 0) { return; }
    *(u32*)(lbl_80426BD0 + 0x188) = (u32)val;
}
/* 0x8012B19C | 0x448 */
extern void fn_8018D998(void);
extern void peopleSearchID(void);
extern void peopleInfoBiosGetPtr(void);
extern void fn_8018F5E4(void);
extern void fn_8010F320(void);
extern void fn_800A3AC0(void);
extern void fn_800A3A78(void);
extern void fn_8010FDF8(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D03C;
extern f32 lbl_8047D040;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern u8 lbl_80478AC0[4];
extern f32 lbl_8047D060;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012B19C(void);
/* 0x8012B5E4 | 0x4EC */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D03C;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D060;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012B5E4(void);
/* 0x8012BAD0 | 0x20 */
void fn_8012BAD0(u32 a, u32 b, u32 c, u32 d, u32 e) {
    u8* base = lbl_80426BD0;
    *(u32*)(base + 0x18C) = a;
    *(u32*)(base + 0x190) = b;
    *(u32*)(base + 0x194) = c;
    *(u32*)(base + 0x198) = d;
    *(u32*)(base + 0x19C) = e;
}
/* 0x8012BAF0 | 0xB8 */
#if 0
asm void fn_8012BAF0(void) {
#include "src/game/gs_field_world_fn_8012BAF0.inc"
}
#else
void fn_8012BAF0(u8 type, void* src, u32 val) {
    extern u8 lbl_80426BD0[];
    switch (type) {
        case 1:
            memcpy(lbl_80426BD0 + 0x270, src, 0xd0);
            *(u32*)(lbl_80426BD0 + 0x414) = val;
            break;
        case 2:
            memcpy(lbl_80426BD0 + 0x340, src, 0xd0);
            *(u32*)(lbl_80426BD0 + 0x418) = val;
            break;
        case 3:
            memcpy(lbl_80426BD0 + 0x1a0, src, 0xd0);
            *(u32*)(lbl_80426BD0 + 0x410) = val;
            break;
    }
}
#endif
/* 0x8012BBA8 | 0xFC */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
#if 0
asm void fn_8012BBA8(void) {
#include "src/game/gs_field_world_fn_8012BBA8.inc"
}
#else
void fn_8012BBA8(void) {
    extern u8 lbl_80426BD0[];
    extern void fn_8018CA20(u32 a, u32 b, u32 c);
    extern void fn_8018C7C8(u32 a, u32 b, u32 c);
    extern void fn_8018C69C(u32 a, u32 b, u32 c);
    s32 i;
    s32 offset;
    s32 idx;
    u32 table[2];
    u32 val;
    u8 flag;
    i = 0;
    offset = 0;
    idx = 0;
    do {
        if (i >= 0 && i < 2) {
            if (*(u16*)(&lbl_80426BD0[offset] + 4) & 1) {
                flag = 1;
            } else {
                flag = 0;
            }
        } else {
            flag = 0;
        }
        if ((u8)flag != 0) {
            table[0] = *(u32*)&lbl_8047D030;
            table[1] = *(u32*)&lbl_8047D034;
            if (i >= 0 && i < 2) {
                val = table[idx / 4];
            }
            fn_8018CA20(0, val, 1);
            fn_8018C7C8(0, val, 0x700);
            fn_8018C69C(0, val, 0x80000008);
        }
        i++;
        offset += 0x20;
        idx += 4;
    } while (i < 2);
}
#endif
/* 0x8012BCA4 | 0x13C */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012BCA4(void);
/* 0x8012BDE0 | 0xD4 */
#if 0
asm void fn_8012BDE0(void) {
#include "src/game/gs_field_world_fn_8012BDE0.inc"
}
#else
#pragma push
#pragma optimization_level 1
u32 fn_8012BDE0(u32 r3, u32 r4) {
    u32 r0 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    r5 = (u32)&lbl_80426BD0;
    r7 = 0x0;
    r0 = *(u32*)((u8*)r5 + 0x140);
    if (r0 != (u32)0x0) {
        r5 = r5 + 0x8;
        r7 = 0x1;
        r0 = *(u32*)((u8*)r5 + 0x140);
        if (r0 != (u32)0x0) {
            r0 = *(u32*)((u8*)r5 + 0x148);
            r7 = 0x2;
            r5 = r5 + 0x8;
            if (r0 != (u32)0x0) {
                r0 = *(u32*)((u8*)r5 + 0x148);
                r7 = 0x3;
                r5 = r5 + 0x8;
                if (r0 != (u32)0x0) {
                    r0 = *(u32*)((u8*)r5 + 0x148);
                    r7 = 0x4;
                    r5 = r5 + 0x8;
                    if (r0 != (u32)0x0) {
                        r0 = *(u32*)((u8*)r5 + 0x148);
                        r7 = 0x5;
                        r5 = r5 + 0x8;
                        if (r0 != (u32)0x0) {
                            r0 = *(u32*)((u8*)r5 + 0x148);
                            r7 = 0x6;
                            r5 = r5 + 0x8;
                            if (r0 != (u32)0x0) {
                                r0 = *(u32*)((u8*)r5 + 0x148);
                                r7 = 0x7;
                                if (r0 != (u32)0x0) {
                                    r7 = 0x8;
    }
    }
    }
    }
    }
    }
    }
    }
    if ((s32)r7 >= (s32)0x8) {
        return (u32)-0x1;
    }
    r5 = (u32)&lbl_80426BD0;
    r6 = r7 << 3;
    r0 = (u32)&lbl_80426BD0;
    r5 = r0 + r6;
    *(u32*)((u8*)r5 + 0x140) = r3;
    r3 = r7;
    *(u32*)((u8*)r5 + 0x144) = r4;
    return r3;
}
#pragma pop
#endif
/* 0x8012BEB4 | 0x200 */
extern void fn_800D3088(void);
extern f64 lbl_8047D068;
extern f32 lbl_8047D038;
extern f32 lbl_8047D040;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012BEB4(void);
/* 0x8012C0B4 | 0x48C */
extern void fn_8018CD08(void);
extern void fn_8018FCBC(void);
extern void fn_8018FC50(void);
extern void fn_800CE148(void);
extern void fn_800CDBE0(void);
extern void GScolsys2CheckGetEventID(void);
extern void fn_8018790C(void);
extern void fn_800F7D38(void);
extern void fn_800F7C8C(void);
extern void fn_8018BA04(void);
extern void fn_80187D48(void);
extern void fn_8018D7D0(void);
extern void fn_80183730(void);
extern void fn_8018397C(void);
extern void fn_801812E8(void);
extern void fn_80189490(void);
extern void fn_80183688(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D070;
extern f32 lbl_8047D074;
extern f32 lbl_8047D078;
extern f32 lbl_8047D07C;
extern f32 lbl_8047D038;
extern f32 lbl_8047D080;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012C0B4(void);
/* 0x8012C540 | 0x120 */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D078;
extern f32 lbl_8047D07C;
extern f32 lbl_8047D038;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012C540(void);
/* 0x8012C660 | 0x424 */
extern void fn_8018F4C8(void);
extern void fn_800EC578(void);
extern void GSmodelGetAnimFrame(void);
extern void fn_800EC5FC(void);
extern void GSmodelSetBlendFactor(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D084;
extern f32 lbl_8047D088;
extern f32 lbl_8047D038;
extern f32 lbl_8047D080;
extern f32 lbl_8047D08C;
extern f32 lbl_8047D040;
extern f32 lbl_8047D090;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012C660(void);
/* 0x8012D39C | 0x454 */
extern f32 lbl_8047D0A8;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D080;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012D39C(void);
/* 0x8012D7F0 | 0x6A4 */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f64 lbl_8047D068;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D060;
extern f32 lbl_8047D0AC;
extern f32 lbl_8047D080;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012D7F0(void);
/* 0x8012DE94 | 0x4F4 */
extern void fn_800E3C64(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
extern f64 lbl_8047D058;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f32 lbl_8047D0B0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012DE94(void);
/* 0x8012E388 | 0x430 */
extern void fn_800F7A7C(void);
extern void fn_800F7A08(void);
extern void fn_800F7BC4(void);
extern void fn_80188214(void);
extern void fn_80166458(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f64 lbl_8047D0C8;
extern f32 lbl_8047D0B4;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D084;
extern f32 lbl_8047D0B8;
extern f32 lbl_8047D0BC;
extern f32 lbl_8047D078;
extern f32 lbl_8047D0C0;
extern f32 lbl_8047D0C4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012E388(void);
/* 0x8012E7B8 | 0x41C */
extern void fn_800F7AF0(void);
extern void fn_801887D8(void);
extern void PSVECDistance(void);
extern f32 lbl_8047D038;
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f64 lbl_8047D068;
extern f32 lbl_8047D080;
extern f32 lbl_8047D0D0;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D0D4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012E7B8(void);
/* 0x8012EBD4 | 0x3E4 */
extern void fn_801337B0(void);
extern void fn_80102620(void);
extern void fn_8018C424(void);
extern void fn_8000D710(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern u8 lbl_80272A38[];
extern f32 lbl_8047D064;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012EBD4(void);
/* 0x8012F008 | 0x114 */
extern void fn_80188AF4(u32, u32);
extern void fn_80188F78(u32, u32);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
#if 0
asm void fn_8012F008(void) {
#include "src/game/gs_field_world_fn_8012F008.inc"
}
#else
u32 fn_8012F008(s32 idx, s32 state)
{
    u32 values[2];
    u32 value;
    u8* entry;
    u32 valid;

    if (state < 0 || state >= 2) {
        return 0;
    }

    if (idx < 0 || idx >= 2) {
        valid = 0;
    } else {
        if ((*(u16*)(lbl_80426BD0 + idx * 0x20 + 4) & 1) == 0) {
            valid = 0;
        } else {
            valid = 1;
        }
    }
    if ((valid & 0xFF) == 0) {
        return 0;
    }

    values[0] = *(u32*)&lbl_8047D030;
    values[1] = *(u32*)&lbl_8047D034;
    if (idx >= 0 && idx < 2) {
        value = values[idx];
    }

    entry = lbl_80426BD0 + idx * 0x20;
    if (*(s32*)(entry + 0xC) == 1) {
        fn_80188AF4(0, value);
    }
    if (state == 1) {
        fn_80188F78(0, value);
    }
    *(u32*)(entry + 0xC) = state;
    return 1;
}
#endif
/* 0x8012F11C | 0x34 */
u32 heroMoveIsMember(s32 idx) {
    u8* ptr;
    u16 val;
    if (idx < 0 || idx >= 2) { return 0; }
    ptr = (u8*)lbl_80426BD0;
    ptr += (u32)idx * 32;
    val = *(u16*)(ptr + 4);
    return (u32)(val & 1);
}
/* 0x8012F150 | 0xAC */
extern f32 lbl_8047D038;
extern f32 lbl_8047D0D4;
#if 0
asm void fn_8012F150(void) {
#include "src/game/gs_field_world_fn_8012F150.inc"
}
#else
s32 fn_8012F150(s32 idx) {
    f32 f1, f2;
    s32 i;
    if (idx < 0 || idx >= 2) return 0;
    if (idx == (s32)*(u32*)lbl_80426BD0) return 0;
    f1 = lbl_8047D038;
    *(u16*)(lbl_80426BD0 + ((u32)idx << 5) + 4) &= 0xFFFE;
    f2 = lbl_8047D0D4;
    *(f32*)(lbl_80426BD0 + (*(u32*)lbl_80426BD0 << 5) + 8) = f1;
    for (i = 0; i < 2; i++) {
        if (*(u16*)(lbl_80426BD0 + (u32)i * 0x20 + 4) & 1) {
            if ((s32)*(u32*)lbl_80426BD0 != i) {
                *(f32*)(lbl_80426BD0 + (u32)i * 0x20 + 8) = f2;
                f2 = lbl_8047D0D4;
                f2 = f2 + f2;
            }
        }
    }
    return 1;
}
#endif
/* 0x8012F1FC | 0x210 */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
extern f32 lbl_8047D0D4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012F1FC(void);
/* 0x8012F40C | 0x204 */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
extern f32 lbl_8047D0D4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012F40C(void);
/* 0x8012F610 | 0x4C8 */
extern void fn_800E3D6C(void);
extern void fn_8010E138(void);
extern void fn_800E4170(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D0AC;
extern f32 lbl_8047D0D8;
extern f32 lbl_8047D07C;
extern f32 lbl_8047D038;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012F610(void);
/* 0x8012FAD8 | 0x1FC */
extern void fn_8011393C(void);
extern void fn_8006AE18(void);
extern u8 lbl_802729C0[];
extern u8 lbl_80272A10[];
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012FAD8(void);
/* 0x8012FCD4 | 0x380 */
extern void fn_8018E050(void);
extern void fn_800EB528(void);
extern void fn_8018CB5C(void);
extern void fn_80189328(void);
extern void fn_8018BF24(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8012FCD4(void);
/* 0x80130054 | 0x1F8 */
extern f32 lbl_8047D034;
extern f32 lbl_8047D030;
extern f32 lbl_8047D038;
extern f32 lbl_8047D0D4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_80130054(void);
/* 0x8013024C | 0x414 */
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f32 lbl_8047D038;
extern f32 lbl_8047D0D4;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_8013024C(void);
/* 0x80130660 | 0x110 */
extern void gamedataGetStatus(void);
extern void gamedataAttestCreate(void);
#if 0
asm void fn_80130660(void) {
#include "src/game/gs_field_world_fn_80130660.inc"
}
#else
void fn_80130660(u8* arg1) {
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void gamedataAttestCreate(u32* a, u32 b, u32 c, u8 d, u8 e);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32* d);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_800FA280(u32 val);
    extern void fn_80123EF0(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80124410(u8* a, s32 b, s32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void memoDataSet(u32 a, u8* b);
    extern s32 fn_80129E20(u8* a, u8* b, u32 c);
    u32 temp;
    u8 buf[0x13c];
    u8 r31, r30_u8;
    u32 tmp;

    r31 = (u8)gamedataGetStatus(0, 5);
    r30_u8 = (u8)gamedataGetStatus(0, 4);
    gamedataAttestCreate(&temp, 0x9, 3, r30_u8, r31);
    fn_801240C4(buf, 0xfb, 0xa, &temp);
    fn_801254B4(buf, 0, 0x99, 0, 0x46);
    fn_80123EF0(buf, 0xff, 0xa, 4, 1, 0x7991, fn_800FA280(0x12af));
    tmp = fn_80124410(buf, -1, -1, 0, 0x7991);
    fn_801254B4(buf, 0, 0x6f, 0, tmp);
    fn_8012546C(buf);
    memoDataSet(0, buf);
    fn_80129E20(arg1, buf, 1);
}
#endif
/* 0x80130770 | 0x120 */
#if 0
asm void fn_80130770(void) {
#include "src/game/gs_field_world_fn_80130770.inc"
}
#else
void fn_80130770(u8* arg1) {
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void gamedataAttestCreate(u32* a, u32 b, u32 c, u8 d, u8 e);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32* d);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_800FA280(u32 val);
    extern void fn_80123EF0(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80124410(u8* a, s32 b, s32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void memoDataSet(u32 a, u8* b);
    extern s32 fn_80129E20(u8* a, u8* b, u32 c);
    extern u16 fn_80123110(u8* a, u32 b, u8 c);
    u32 temp;
    u8 buf[0x13c];
    u8 r31, r30_u8;
    u32 tmp;

    r31 = (u8)gamedataGetStatus(0, 5);
    r30_u8 = (u8)gamedataGetStatus(0, 4);
    gamedataAttestCreate(&temp, 0x9, 3, r30_u8, r31);
    fn_801240C4(buf, 0x19, 0xa, &temp);
    fn_801254B4(buf, 0, 0x99, 0, 0x46);
    fn_80123EF0(buf, 0xff, 0xa, 4, 0, 0x7991, fn_800FA280(0x12ae));
    tmp = fn_80124410(buf, -1, -1, 0, 0x7991);
    fn_801254B4(buf, 0, 0x6f, 0, tmp);
    fn_80123110(buf, 0xca, 1);
    fn_8012546C(buf);
    memoDataSet(0, buf);
    fn_80129E20(arg1, buf, 1);
}
#endif
/* 0x80130890 | 0x110 */
#if 0
asm void fn_80130890(void) {
#include "src/game/gs_field_world_fn_80130890.inc"
}
#else
void fn_80130890(u8* arg1) {
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void gamedataAttestCreate(u32* a, u32 b, u32 c, u8 d, u8 e);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32* d);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_800FA280(u32 val);
    extern void fn_80123EF0(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80124410(u8* a, s32 b, s32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void memoDataSet(u32 a, u8* b);
    extern s32 fn_80129E20(u8* a, u8* b, u32 c);
    u32 temp;
    u8 buf[0x13c];
    u8 r31, r30_u8;
    u32 tmp;

    r31 = (u8)gamedataGetStatus(0, 5);
    r30_u8 = (u8)gamedataGetStatus(0, 4);
    gamedataAttestCreate(&temp, 0x8, 3, r30_u8, r31);
    fn_801240C4(buf, 0xfa, 0x46, &temp);
    fn_801254B4(buf, 0, 0x99, 0, 0x46);
    fn_80123EF0(buf, 0xff, 0x46, 4, 0, 0x2740, fn_800FA280(0x12ad));
    tmp = fn_80124410(buf, -1, -1, 0, 0x2740);
    fn_801254B4(buf, 0, 0x6f, 0, tmp);
    fn_8012546C(buf);
    memoDataSet(0, buf);
    fn_80129E20(arg1, buf, 1);
}
#endif
/* 0x801309A0 | 0xE8 */
#if 0
asm void fn_801309A0(void) {
#include "src/game/gs_field_world_fn_801309A0.inc"
}
#else
void fn_801309A0(u32 arg1) {
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32 d);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern u32 fn_800FA280(u32 val);
    extern void fn_80123EF0(u8* a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g);
    extern u32 fn_80124410(u8* a, s32 b, s32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void memoDataSet(u32 a, u8* b);
    extern void fn_80129E20(u32 a, u8* b, u32 c);
    u8 local[0x144];
    u32 tmp;
    fn_801240C4(local, 0x161, 0xd, gamedataGetStatus(0, 1));
    fn_801254B4(local, 0, 0x99, 0, 0x46);
    tmp = fn_800FA280(0x12ac);
    fn_80123EF0(local, 0xfe, 0xd, 4, 0, 0x911D, tmp);
    tmp = fn_80124410(local, -1, -1, 0, 0x911D);
    fn_801254B4(local, 0, 0x6f, 0, tmp);
    fn_8012546C(local);
    memoDataSet(0, local);
    fn_80129E20(arg1, local, 1);
}
#endif
/* 0x80130A88 | 0x128 */
#if 0
asm void fn_80130A88(void) {
#include "src/game/gs_field_world_fn_80130A88.inc"
}
#else
void fn_80130A88(u32 arg1) {
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32 d);
    extern void fn_80123D58(u8* a, u32 b, u32 c);
    extern u32 fn_80124410(u8* a, u32 b, s32 c, u32 d, u32 e);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void fn_80129F20(u32 a, u8* b, u32 c, u32 d, u32 e);
    u8 buf[0x140];
    u32 val, tmp;
    val = fn_8012A5B0((u8*)arg1, 2, 0);
    fn_801240C4(buf, 0xc4, 0x19, gamedataGetStatus(0, 1));
    fn_80123D58(buf, 0, 0x5d);
    fn_80123D58(buf, 1, 0xd8);
    fn_80123D58(buf, 2, 0x73);
    fn_80123D58(buf, 3, 0x10e);
    tmp = fn_80124410(buf, 0, -1, 0, val);
    fn_801254B4(buf, 0, 0x6f, 0, tmp);
    fn_801254B4(buf, 0, 0x79, 0, 0x3ff4);
    fn_801254B4(buf, 0, 0x99, 0, 0xdc);
    fn_8012546C(buf);
    fn_80129F20(arg1, buf, 0xfe, 4, 0);
}
#endif
/* 0x80130BB0 | 0x128 */
#if 0
asm void fn_80130BB0(void) {
#include "src/game/gs_field_world_fn_80130BB0.inc"
}
#else
void fn_80130BB0(u32 arg1) {
    extern u32 fn_8012A5B0(u8* a, u32 b, u32 c);
    extern u32 gamedataGetStatus(u32 a, u32 b);
    extern void fn_801240C4(u8* a, u32 b, u32 c, u32 d);
    extern void fn_80123D58(u8* a, u32 b, u32 c);
    extern u32 fn_80124410(u8* a, u32 b, s32 c, u32 d, u32 e);
    extern void fn_801254B4(u8* a, u32 b, u32 c, u32 d, u32 e);
    extern void fn_8012546C(u8* a);
    extern void fn_80129F20(u32 a, u8* b, u32 c, u32 d, u32 e);
    u8 buf[0x140];
    u32 val, tmp;
    val = fn_8012A5B0((u8*)arg1, 2, 0);
    fn_801240C4(buf, 0xc5, 0x1a, gamedataGetStatus(0, 1));
    fn_80123D58(buf, 0, 0x2c);
    fn_80123D58(buf, 1, 0x122);
    fn_80123D58(buf, 2, 0x10d);
    fn_80123D58(buf, 3, 0x121);
    tmp = fn_80124410(buf, 0, -1, 0, val);
    fn_801254B4(buf, 0, 0x6f, 0, tmp);
    fn_801254B4(buf, 0, 0x79, 0, 0x4a6f);
    fn_801254B4(buf, 0, 0x99, 0, 0xdc);
    fn_8012546C(buf);
    fn_80129F20(arg1, buf, 0xfe, 4, 0);
}
#endif
extern void fn_800F76E4();
extern void fn_80112700(void);
#if 0
asm void floorReadScriptPostFunc(void) {
#include "src/game/gs_field_world_fn_80114D18.inc"
}
#else
#pragma optimization_level 4
#pragma peephole off
void* floorReadScriptPostFunc(u32 a, u32 b) {
    void* result;

    result = fn_800F9318(a, b);
    if (fn_800FF548() == 0 && result != NULL) {
        fn_800F76E4(result);
        fn_80112700();
    }
    return result;
}
#pragma peephole reset
#endif
extern void fn_800FC39C();
#if 0
asm void floorReadFontPostFunc(void) {
#include "src/game/gs_field_world_fn_80114E0C.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
void* floorReadFontPostFunc(u32 a, u32 b) {
    void* result;

    if (fn_800FF548() != 0) {
        return NULL;
    }
    result = fn_800F9318(a, b);
    if (result != NULL) {
        fn_800FC39C(result);
    }
    return result;
}
#pragma pop
#endif
extern void GSmsgOpen();
#if 0
asm void floorReadMsgPostFunc(void) {
#include "src/game/gs_field_world_fn_80114F18.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
void* floorReadMsgPostFunc(u32 a, u32 b) {
    void* result;

    if (fn_800FF548() != 0) {
        return NULL;
    }
    result = fn_800F9318(a, b);
    if (result != NULL) {
        GSmsgOpen(result);
    }
    return result;
}
#pragma pop
#endif
#if 0
asm void floorReadNormalPreFunc(void) {
#include "src/game/gs_field_world_fn_80115024.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
void* floorReadNormalPreFunc(u32 a, u32 b, u32 size) {
    void* result;

    result = fn_800F9418((size + 0x1F) & ~0x1F, 0x20, a, b, 0);
    if (result == NULL) {
        fn_800DD970(lbl_802725CC, size);
    }
    return result;
}
#pragma pop
#endif
extern u8 fn_800FF554(void);
extern void fn_800F760C();
#if 0
asm void fn_80115124(void) {
#include "src/game/gs_field_world_fn_80115124.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115124(void* ptr) {
    if (fn_800FF554() != 0) {
        return 0;
    }
    fn_800F760C(ptr);
    return 1;
}
#pragma pop
#endif
extern void fn_800FC2A8();
#if 0
asm void fn_80115170(void) {
#include "src/game/gs_field_world_fn_80115170.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115170(void* ptr) {
    if (fn_800FF554() != 0) {
        return 0;
    }
    GSmsgFontClose(ptr);
    return 1;
}
#pragma pop
#endif
extern void GSmsgClose();
#if 0
asm void fn_801151BC(void) {
#include "src/game/gs_field_world_fn_801151BC.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801151BC(void* ptr) {
    if (fn_800FF554() != 0) {
        return 0;
    }
    GSmsgClose(ptr);
    return 1;
}
#pragma pop
#endif
extern u8 lbl_8035BA98[];
extern const char lbl_80272608[];
#if 0
asm void fn_801155CC(void) {
#include "src/game/gs_field_world_fn_801155CC.inc"
}
#else
#pragma push
#pragma scheduling on
#pragma peephole off
#pragma optimization_level 4
void* fn_801155CC(u8* ptr) {
    void* sub;

    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA98);
        return NULL;
    }
    sub = *(void**)(ptr + 0x1C);
    if (sub == NULL) {
        return NULL;
    }
    return *(void**)sub;
}
#pragma pop
#endif
extern u8 lbl_8035BA7C[];
#if 0
asm void fn_80115628(void) {
#include "src/game/gs_field_world_fn_80115628.inc"
}
#else
#pragma push
#pragma scheduling on
#pragma peephole off
#pragma optimization_level 4
void* fn_80115628(u8* ptr) {
    void* sub;

    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA7C);
        return NULL;
    }
    sub = *(void**)(ptr + 0x18);
    if (sub == NULL) {
        return NULL;
    }
    return *(void**)sub;
}
#pragma pop
#endif
extern u8 lbl_8035BA60[];
#if 0
asm void fn_80115684(void) {
#include "src/game/gs_field_world_fn_80115684.inc"
}
#else
#pragma push
#pragma scheduling on
#pragma peephole off
#pragma optimization_level 4
void* fn_80115684(u8* ptr, u32 idx) {
    void* sub;
    void* arr;

    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA60);
        return NULL;
    }
    sub = *(void**)(ptr + 0x14);
    if (sub == NULL) {
        return NULL;
    }
    arr = *(void**)sub;
    if (idx >= *(u32*)*(void**)arr) {
        return NULL;
    }
    return (u8*)*(void**)((u8*)arr + 4) + idx * 0x24;
}
#pragma pop
#endif
extern u8 lbl_8035BA48[];
#if 0
asm void fn_80115704(void) {
#include "src/game/gs_field_world_fn_80115704.inc"
}
#else
#pragma push
#pragma scheduling on
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115704(u8* ptr) {
    void* sub;

    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA48);
        return 0;
    }
    sub = *(void**)(ptr + 0x14);
    if (sub == NULL) {
        return 0;
    }
    return *(u32*)*(u32*)*(u32*)sub;
}
#pragma pop
#endif
extern u8 lbl_8035BA2C[];
#if 0
asm void fn_80115768(void) {
#include "src/game/gs_field_world_fn_80115768.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115768(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA2C);
        return 0;
    }
    return *(u32*)(ptr + 0x44);
}
#pragma pop
#endif
extern u8 lbl_8035BA10[];
#if 0
asm void fn_801157B0(void) {
#include "src/game/gs_field_world_fn_801157B0.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801157B0(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035BA10);
        return 0;
    }
    return *(u32*)(ptr + 0x40);
}
#pragma pop
#endif
extern u8 lbl_8035B9F8[];
#if 0
asm void fn_801157F8(void) {
#include "src/game/gs_field_world_fn_801157F8.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801157F8(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B9F8);
        return 0;
    }
    return *(u32*)(ptr + 0x3C);
}
#pragma pop
#endif
extern u8 lbl_8035B9DC[];
#if 0
asm void fn_80115840(void) {
#include "src/game/gs_field_world_fn_80115840.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115840(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B9DC);
        return 0;
    }
    return *(u32*)(ptr + 0x24);
}
#pragma pop
#endif
extern u8 lbl_8035B9C0[];
#if 0
asm void fn_80115888(void) {
#include "src/game/gs_field_world_fn_80115888.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115888(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B9C0);
        return 0;
    }
    return *(u32*)(ptr + 0x38);
}
#pragma pop
#endif
extern u8 lbl_8035B9A4[];
#if 0
asm void fn_801158D0(void) {
#include "src/game/gs_field_world_fn_801158D0.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801158D0(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B9A4);
        return 0;
    }
    return *(u32*)(ptr + 0x34);
}
#pragma pop
#endif
extern u8 lbl_8035B988[];
#if 0
asm void fn_80115918(void) {
#include "src/game/gs_field_world_fn_80115918.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115918(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B988);
        return 0;
    }
    return *(u32*)(ptr + 0x30);
}
#pragma pop
#endif
extern u8 lbl_8035B96C[];
#if 0
asm void fn_80115960(void) {
#include "src/game/gs_field_world_fn_80115960.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115960(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B96C);
        return 0;
    }
    return *(u32*)(ptr + 0x2C);
}
#pragma pop
#endif
extern u8 lbl_8035B950[];
#if 0
asm void fn_801159A8(void) {
#include "src/game/gs_field_world_fn_801159A8.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801159A8(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B950);
        return 0;
    }
    return *(u32*)(ptr + 0x28);
}
#pragma pop
#endif
extern u8 lbl_8035B938[];
#if 0
asm void fn_801159F0(void) {
#include "src/game/gs_field_world_fn_801159F0.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_801159F0(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B938);
        return 0;
    }
    return *(u32*)(ptr + 0x0C);
}
#pragma pop
#endif
extern u8 lbl_8035B904[];
#if 0
asm void fn_80115A80(void) {
#include "src/game/gs_field_world_fn_80115A80.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115A80(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B904);
        return 0;
    }
    return *(u32*)(ptr + 0x4);
}
#pragma pop
#endif
extern u16 fn_801EF624();
extern u8 lbl_8035B8E8[];
#if 0
asm void fn_80115AC8(void) {
#include "src/game/gs_field_world_fn_80115AC8.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
u8 fn_80115AC8(u8* ptr) {
    u8 val;

    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B8E8);
        return 0;
    }
    val = (ptr[0] >> 5) & 7;
    if (val == 2) {
        if ((u16)fn_801EF624(ptr) == 0) {
            return 1;
        }
    }
    val = (ptr[0] >> 5) & 7;
    return val;
}
#pragma pop
#endif
extern const char lbl_802726AC[];
extern const char lbl_8035B8CC[];
#if 0
asm void fn_80115B48(void) {
#include "src/game/gs_field_world_fn_80115B48.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u32 fn_80115B48(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_802726AC, lbl_8035B8CC);
        return 0;
    }
    return *(u32*)(ptr + 0x8);
}
#pragma pop
#endif
extern u8 lbl_8035B8B4[];
#if 0
asm void fn_80115B90(void) {
#include "src/game/gs_field_world_fn_80115B90.inc"
}
#else
#pragma push
#pragma peephole off
#pragma optimization_level 4
u8 fn_80115B90(u8* ptr) {
    if (ptr == NULL) {
        fn_800DD970(lbl_80272608, lbl_8035B8B4);
        return 0;
    }
    return *(u8*)(ptr + 0x1);
}
#pragma pop
#endif
extern u32 lbl_80478FB8;
extern u32 lbl_80478FBC;
#if 0
asm void fn_80115C48(void) {
#include "src/game/gs_field_world_fn_80115C48.inc"
}
#else
#pragma push
#pragma peephole off
#pragma scheduling on
void* fn_80115C48(u32 key) {
    u8* p = (u8*)lbl_80478FBC;
    u32 i;
    for (i = *(u32*)lbl_80478FB8; i != 0; i--) {
        if (*(u32*)(p + 0xC) == key) return p;
        p += 0x4C;
    }
    fn_800DD970((char*)lbl_802726D4, lbl_8035B8A0);
    return 0;
}
#pragma pop
#endif
extern u32 lbl_80478EB8;
extern u8 lbl_8035BB70[];
extern u32 lbl_80478EBC;
#if 0
asm void floorEventChangeTresure(void) {
#include "src/game/gs_field_world_floorEventChangeTresure.inc"
}
#else
s32 floorEventChangeTresure(u32 index, u16 val, u8 byte) {
    extern u32 lbl_80478EB8;
    extern u32 lbl_80478EBC;
    extern u8 lbl_8035BB70[];
    extern u8 lbl_80272708[];
    u32 count;
    u8* entry;
    count = *(u32*)lbl_80478EB8;
    if (index >= count) {
        fn_800DD970((const char*)lbl_80272708, lbl_8035BB70);
        return -1;
    }
    entry = (u8*)lbl_80478EBC + index * 0x1c;
    *(u32*)(entry + 0xc) = val;
    *(u8*)(entry + 0x1) = byte;
    return 0;
}
#endif
extern u32 lbl_8047CFC0;
extern u32 lbl_8047CFC8;
extern u32 lbl_8047CFC4;
#if 0
asm void floorCharacterBiosGetRot(void) {
#include "src/game/gs_field_world_fn_80116F68.inc"
}
#else
#pragma push
#pragma peephole off
s32 floorCharacterBiosGetRot(void* a, void* b) {
    if (a == 0) return 0;
    if (b == 0) return 0;
    fn_800E01F4(b, *(f32*)&lbl_8047CFC0,
                *(f32*)&lbl_8047CFC4 * (f32)(s32)*(s16*)((u8*)a + 4),
                *(f32*)&lbl_8047CFC0);
    return 1;
}
#pragma pop
#endif
#if 0
asm void floorCharacterBiosGetPos(void) {
#include "src/game/gs_field_world_fn_80116FE0.inc"
}
#else
#pragma optimization_level 4
#pragma push
#pragma peephole off
u32 floorCharacterBiosGetPos(u8* ptr, void* obj) {
    if (ptr == NULL) {
        return 0;
    }
    if (obj == NULL) {
        return 0;
    }
    fn_800E01F4(obj, *(f32*)(ptr + 0x18), *(f32*)(ptr + 0x1C), *(f32*)(ptr + 0x20));
    return 1;
}
#pragma pop
#endif
#if 0
asm void fn_801170A4(void) {
#include "src/game/gs_field_world_fn_801170A4.inc"
}
#else
#pragma peephole off
u32 fn_801170A4(u8* arg1, u32 arg2) {
    extern void* fn_800FF56C(void);
    extern void* fn_80115C48(void* a);
    extern u8* fn_80115A80(void* a);
    extern u32 fn_80115684(void* a, u32 b);
    void* temp;
    if (arg1 == NULL) { return 0; }
    temp = fn_80115C48(fn_800FF56C());
    if (arg1 != fn_80115A80(temp)) { return 0; }
    return fn_80115684(temp, arg2);
}
#pragma peephole on
#endif
extern void fn_800E0020(void);
extern u32 lbl_8047AD68;
extern u32 lbl_8047AD6C;
extern f32 lbl_8047CFD0;
extern f32 lbl_8047CFDC;
extern f32 lbl_8047CFE0;
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
u8 floorUpdateFieldCamera(void);
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119D90(void) {
#include "src/game/gs_field_world_fn_80119D90.inc"
}
#else
#pragma optimization_level 4
u8 fn_80119D90(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return entry[0x4];
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119DD0(void) {
#include "src/game/gs_field_world_fn_80119DD0.inc"
}
#else
#pragma optimization_level 4
u8 fn_80119DD0(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return entry[0x3];
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119E10(void) {
#include "src/game/gs_field_world_fn_80119E10.inc"
}
#else
#pragma optimization_level 4
u16 fn_80119E10(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u16*)(entry + 0xa);
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119E50(void) {
#include "src/game/gs_field_world_fn_80119E50.inc"
}
#else
#pragma optimization_level 4
u8 fn_80119E50(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return entry[0x2];
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119E90(void) {
#include "src/game/gs_field_world_fn_80119E90.inc"
}
#else
#pragma optimization_level 4
u8 fn_80119E90(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return entry[0x1];
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm u16 fn_80119ED0() {
#include "src/game/gs_field_world_fn_80119ED0.inc"
}
#else
#pragma optimization_level 4
u16 fn_80119ED0(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u16*)(entry + 0x8);
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119F10(void) {
#include "src/game/gs_field_world_fn_80119F10.inc"
}
#else
#pragma optimization_level 4
u8 fn_80119F10(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return entry[0x0];
}
#endif
extern u32 lbl_80478B48;  /* NPC count (SDA) */
#if 0
asm void fn_80119F50(void) {
#include "src/game/gs_field_world_fn_80119F50.inc"
}
#else
#pragma optimization_level 4
u32 fn_80119F50(u16 idx) {
    u8* entry;
    if ((u32)idx >= lbl_80478B48) { entry = NULL; } else { entry = lbl_8035BBA8 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0x10);
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C1D0(void) {
#include "src/game/gs_field_world_fn_8011C1D0.inc"
}
#else
#pragma optimization_level 4
void fn_8011C1D0(u8* ptr, u32 val) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return; }
    *(u32*)(entry + 0x10) = val;
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C220(void) {
#include "src/game/gs_field_world_fn_8011C220.inc"
}
#else
#pragma optimization_level 4
void fn_8011C220(u8* ptr, u32 val) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return; }
    *(u32*)(entry + 0xc) = val;
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C270(void) {
#include "src/game/gs_field_world_fn_8011C270.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011C270(u8* ptr) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0x10);
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C2D0(void) {
#include "src/game/gs_field_world_fn_8011C2D0.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011C2D0(u8* ptr) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0xc);
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C330(void) {
#include "src/game/gs_field_world_fn_8011C330.inc"
}
#else
#pragma optimization_level 4
void fn_8011C330(u8* ptr, u32 val) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return; }
    *(u32*)(entry + 0x8) = val;
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C380(void) {
#include "src/game/gs_field_world_fn_8011C380.inc"
}
#else
#pragma optimization_level 4
void fn_8011C380(u8* ptr, u32 val) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return; }
    *(u32*)(entry + 0x4) = val;
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C3D0(void) {
#include "src/game/gs_field_world_fn_8011C3D0.inc"
}
#else
#pragma optimization_level 4
void fn_8011C3D0(u8* ptr, u32 val) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return; }
    *(u32*)(entry + 0x0) = val;
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C450(void) {
#include "src/game/gs_field_world_fn_8011C450.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011C450(u8* ptr) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0x8);
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C4B0(void) {
#include "src/game/gs_field_world_fn_8011C4B0.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011C4B0(u8* ptr) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0x4);
}
#endif
extern u32 lbl_80478B50;  /* field obj count (SDA) */
#if 0
asm void fn_8011C510(void) {
#include "src/game/gs_field_world_fn_8011C510.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011C510(u8* ptr) {
    u16 idx; u8* entry;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)(ptr + 0x1c); }
    if ((u32)idx >= lbl_80478B50) { entry = NULL; } else { entry = lbl_8035C430 + (u32)idx * 0x14; }
    if (entry == NULL) { return 0; }
    return *(u32*)(entry + 0x0);
}
#endif
#if 0
asm void fn_8011E1D4(void) {
#include "src/game/gs_field_world_fn_8011E1D4.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E1D4(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E21C(void) {
#include "src/game/gs_field_world_fn_8011E21C.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E21C(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E264(void) {
#include "src/game/gs_field_world_fn_8011E264.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011E264(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 2) {
        elem = NULL;
    } else {
        elem = ptr + 0x10C + ((u16)idx << 3);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u32*)(elem + 4);
}
#endif
#if 0
asm void fn_8011E2DC(void) {
#include "src/game/gs_field_world_fn_8011E2DC.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E2DC(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 0x14) {
        elem = NULL;
    } else {
        elem = ptr + 0xBA + ((u16)idx << 2);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E324(void) {
#include "src/game/gs_field_world_fn_8011E324.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E324(u8* ptr, s32 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 0x14) {
        elem = NULL;
    } else {
        elem = ptr + 0xBA + ((u16)idx << 2);
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E36C(void) {
#include "src/game/gs_field_world_fn_8011E36C.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E36C(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 4);
}
#endif
#if 0
asm void fn_8011E3B4(void) {
#include "src/game/gs_field_world_fn_8011E3B4.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E3B4(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u16*)(elem + 2);
}
#endif
#if 0
asm void fn_8011E3FC(void) {
#include "src/game/gs_field_world_fn_8011E3FC.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E3FC(u8* ptr, u16 idx) {
    u8* elem;

    if (ptr == NULL) {
        elem = NULL;
    } else if ((u16)idx >= 5) {
        elem = NULL;
    } else {
        elem = ptr + 0x9C + (u16)idx * 6;
    }
    if (elem == NULL) {
        return 0;
    }
    return *(u8*)elem;
}
#endif
#if 0
asm void fn_8011E550(void) {
#include "src/game/gs_field_world_fn_8011E550.inc"
}
#else
#pragma optimization_level 4
u16 fn_8011E550(u8* ptr) {
    u8* sub;

    if (ptr == NULL) {
        sub = NULL;
    } else {
        sub = ptr + 0x90;
    }
    if (sub == NULL) {
        return 0;
    }
    return *(u16*)(sub + 0xA);
}
#endif
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
extern u32 lbl_80478F94;  /* obj data base (SDA) */
#if 0
asm void fn_8011E868(void) {
#include "src/game/gs_field_world_fn_8011E868.inc"
}
#else
#pragma optimization_level 4
u8 fn_8011E868(u8* ptr) {
    u16 idx; u8* entry; u8 flag;
    if (ptr == NULL) { return 0; }
    if (ptr == NULL) { idx = 0; } else { idx = *(u16*)ptr; }
    if ((u32)idx >= *(u32*)lbl_80478F90) {
        entry = NULL;
    } else {
        entry = (u8*)lbl_80478F94 + (u32)idx * 0x11C;
    }
    if (entry == NULL) { flag = 0; } else { flag = entry[0x33]; }
    if ((u8)flag == 0) { return 0; }
    return ptr[0xcc];
}
#endif
#if 0
asm void fn_8011ED18(void) {
#include "src/game/gs_field_world_fn_8011ED18.inc"
}
#else
#pragma optimization_level 4
void* fn_8011ED18(u8* ptr) {
    u32 val;

    if (ptr == NULL) {
        return NULL;
    }
    val = (ptr == NULL) ? 0 : *(u16*)(ptr + 0xD8);
    if ((u16)val == 0) {
        return NULL;
    }
    return fn_801EEEB8(val);
}
#endif
#if 0
asm void fn_8011ED68(void) {
#include "src/game/gs_field_world_fn_8011ED68.inc"
}
#else
#pragma optimization_level 4
u32 fn_8011ED68(u8* ptr) {
    u16 val;
    s32 val2;

    if (ptr == NULL) {
        return 0;
    }
    val = (ptr == NULL) ? 0 : *(u16*)(ptr + 0xD8);
    if ((u16)val == 0) {
        return 0;
    }
    val2 = (ptr == NULL) ? 0 : *(s32*)(ptr + 0xDC);
    if (val2 < 0) {
        return 0;
    }
    return 1;
}
#endif
extern u32 lbl_80478F90;  /* obj header ptr (SDA) */
#if 0
asm void fn_80120C6C(void) {
#include "src/game/gs_field_world_fn_80120C6C.inc"
}
#else
/* returns (obj-header[0] > key) after a fn_8012640C lookup; the obj-header ptr lbl_80478F90
 * is re-read via volatile cast. byte-match verified via objdiff. */
u32 fn_80120C6C(u32 a, u16 key) {
    extern u32 fn_8012640C(u32 a, u16 key, u32 c, u32 d);
    if ((u16)key == 0) {
        return 0;
    }
    if (fn_8012640C(a, key, 1, 0) == 0) {
        return 0;
    }
    return (*(u32* volatile*)&lbl_80478F90)[0] > key;
}
#endif
extern void* fn_80143B08(void* ptr);
extern void fn_80143ABC(void* ptr, u8 val);
#if 0
asm void fn_80120D6C(void) {
#include "src/game/gs_field_world_fn_80120D6C.inc"
}
#else
#pragma optimization_level 4
s32 fn_80120D6C(u8* ptr, void* arg2) {
    u8 val; void* tmp;
    if (ptr == NULL) { return -1; }
    val = (u8)fn_8012640C(ptr, 0, 0xbf, 0);
    tmp = fn_80143B08(arg2);
    fn_80143ABC(tmp, val);
}
#endif
#if 0
asm void fn_80121410(void) {
#include "src/game/gs_field_world_fn_80121410.inc"
}
#else
#pragma optimization_level 4
s32 fn_80121410(u8* ptr) {
    u16 val1; u16 val2;
    if (ptr == NULL) { return 0; }
    val1 = (u16)fn_8012640C(ptr, 0, 0x83, 0);
    val2 = (u16)fn_8012640C(ptr, 0, 0x87, 0);
    return (u8)(val1 == val2);
}
#endif
#if 0
asm void fn_80121574(void) {
#include "src/game/gs_field_world_fn_80121574.inc"
}
#else
#pragma optimization_level 4
s32 fn_80121574(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return 0;
    }
    return (s32)fn_8011A3E4(a, b);
}
#endif
#if 0
asm void fn_8012165C(void) {
#include "src/game/gs_field_world_fn_8012165C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8012165C(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return 0;
    }
    return fn_8011A6D4(a, b);
}
#endif
#if 0
asm void fn_801216CC(void) {
#include "src/game/gs_field_world_fn_801216CC.inc"
}
#else
#pragma optimization_level 4
s32 fn_801216CC(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return 0;
    }
    return fn_8011A860(a, b);
}
#endif
#if 0
asm void fn_8012182C(void) {
#include "src/game/gs_field_world_fn_8012182C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8012182C(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return -1;
    }
    return fn_8011ACB4(a, b);
}
#endif
#if 0
asm void fn_8012189C(void) {
#include "src/game/gs_field_world_fn_8012189C.inc"
}
#else
#pragma optimization_level 4
s32 fn_8012189C(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return -1;
    }
    return fn_8011AE40(a, b);
}
#endif
#if 0
asm void fn_80121984(void) {
#include "src/game/gs_field_world_fn_80121984.inc"
}
#else
#pragma optimization_level 4
s32 fn_80121984(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return 0;
    }
    return fn_8011B130(a, b);
}
#endif
#if 0
asm void fn_80121A6C(void) {
#include "src/game/gs_field_world_fn_80121A6C.inc"
}
#else
#pragma optimization_level 4
s32 fn_80121A6C(void* a, u16 b) {
    if ((u16)fn_80119ED0(b) != 0x7C && (u16)fn_80119ED0(b) != 0xC8) {
        return 0;
    }
    return fn_8011B444(a, b);
}
#endif
#if 0
asm void fn_80121ADC(void) {
#include "src/game/gs_field_world_fn_80121ADC.inc"
}
#else
u8 fn_80121ADC(u8* ptr, u32 slot) {
    extern u16 fn_80119ED0(u32 val);
    extern u8 fn_8011B67C(u8* ptr, u32 val);
    if ((u16)fn_80119ED0(slot) != 0x7c) { if ((u16)fn_80119ED0(slot) != 0xc8) { return 0; } }
    return fn_8011B67C(ptr, slot);
}
#endif
#if 0
asm void fn_80122334(void) {
#include "src/game/gs_field_world_fn_80122334.inc"
}
#else
#pragma optimization_level 1
u8 fn_80122334(u32 val) {
    u32 s;
    s = (val >> 12) & 0x30;
    s = (s & 0xFFFFFF3Fu) | ((val >> 18) & 0xC0);
    s = (s & 0xFFFFFFF3u) | ((val >> 6) & 0x0C);
    s = (s & 0xFFFFFFFCu) | (val & 0x03);
    return (u8)(s % 28);
}
#pragma optimization_level reset
#endif
#if 0
asm void fn_801229F4(void) {
#include "src/game/gs_field_world_fn_801229F4.inc"
}
#else
#pragma optimization_level 4
u32 fn_801229F4(u8* ptr, u8 idx) {
    u16 val;
    u8 val2;
    void* sub;

    if (ptr == NULL) {
        return 0;
    }
    val = (u16)fn_8012640C(ptr, 0, 0x6E, 0);
    val2 = (u8)fn_8012640C(0, val, 0x11, 0);
    sub = fn_8011CE74(val2);
    if (sub == NULL) {
        return 0;
    }
    return pokemonGrowDataBiosGetExp(sub, idx);
}
#endif
#if 0
asm void fn_80122A70(void) {
#include "src/game/gs_field_world_fn_80122A70.inc"
}
#else
#pragma optimization_level 4
s32 fn_80122A70(u8* ptr) {
    u16 val1;
    u16 val2;

    if (ptr == NULL) {
        return 0;
    }
    val1 = (u16)fn_8012640C(ptr, 0, 0x83, 0);
    val2 = (u16)fn_8012640C(ptr, 0, 0x87, 0);
    return (val1 * 100) / val2;
}
#endif
#if 0
asm void fn_80122AE0(void) {
#include "src/game/gs_field_world_fn_80122AE0.inc"
}
#else
#pragma optimization_level 4
u16 fn_80122AE0(u8* ptr, s32 b) {
    u16 val;

    if (ptr == NULL) {
        return 0;
    }
    if (!(u16)b) {
        return 0;
    }
    val = (s32)(u16)fn_8012640C(ptr, 0, 0x83, 0) / (s32)(u16)b;
    if (val == 0) {
        val = 1;
    }
    return val;
}
#endif
#if 0
asm void fn_80122B50(void) {
#include "src/game/gs_field_world_fn_80122B50.inc"
}
#else
#pragma optimization_level 4
u16 fn_80122B50(u8* ptr, s32 b) {
    u16 val;

    if (ptr == NULL) {
        return 0;
    }
    if (!(u16)b) {
        return 0;
    }
    val = (s32)(u16)fn_8012640C(ptr, 0, 0x87, 0) / (s32)(u16)b;
    if (val == 0) {
        val = 1;
    }
    return val;
}
#endif
#if 0
asm void fn_80123090(void) {
#include "src/game/gs_field_world_fn_80123090.inc"
}
#else
#pragma optimization_level 4
u16 fn_80123090(u8* ptr) {
    extern u32 itemGetStatus(u32 a, u16 b, u32 c, u32 d);
    u16 val;
    val = (u16)fn_8012640C(ptr, 0, 0x82, 0);
    if (val == 0) { return 0; }
    return (u16)itemGetStatus(0, val, 7, 0);
}
#endif
#if 0
asm void fn_80123E70(void) {
#include "src/game/gs_field_world_fn_80123E70.inc"
}
#else
#pragma optimization_level 4
s32 fn_80123E70(u8* ptr, u16 idx) {
    extern u8 fn_8011BB6C(u16 type_id, u8 val);
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u16 c);
    u32 sub; u16 val1;
    if (ptr == NULL) { return 0; }
    sub = (u16)idx + 4;
    val1 = (u16)fn_8012640C(ptr, 0, 0x7f, (u16)sub);
    fn_8011BB6C(val1, (u8)fn_8012640C(ptr, 0, 0x81, (u16)sub));
}
#endif
#if 0
asm void fn_80124978(void) {
#include "src/game/gs_field_world_fn_80124978.inc"
}
#else
#pragma optimization_level 4
void fn_80124978(u8* ptr, u32 arg2) {
    extern void fn_801254B4(u8* ptr, u32 a, u32 b, u32 c, u8 d);
    u16 val1;
    if (ptr == NULL) { return; }
    val1 = (u16)fn_8012640C(ptr, 0, 0x6e, 0);
    if ((s32)fn_8012640C(0, val1, 0x17, 1) == 0) { arg2 = 0; }
    fn_801254B4(ptr, 0, 0xb7, 0, (u8)arg2);
}
#endif
extern void fn_80135708(void);
#if 0
asm void fn_80124A60(void) {
#include "src/game/gs_field_world_fn_80124A60.inc"
}
#else
void fn_80124A60(u8* ptr) {
    extern void fn_801254B4(u8* ptr, u32 a, u32 b, u32 c, u32 d);
    extern u32 fn_8012640C(u8* ptr, u32 a, u32 b, u32 c);
    extern void fn_8011B950(u32 base, u16 count);
    extern void gamedataAttestInit(u32 val);
    extern void fn_8011D480(u8* ptr, u8 val);
    u32 i;
    u16 local;
    local = 0;
    if (ptr == NULL) { return; }
    fn_801254B4(ptr, 0, 0xc9, 0, 0);
    fn_801254B4(ptr, 0, 0xc3, 0, 0);
    if (ptr != NULL) { fn_801254B4(ptr, 0, 0xc5, 0, (u32)-0x64); }
    fn_801254B4(ptr, 0, 0xc6, 0, 0);
    fn_801254B4(ptr, 0, 0xc7, 0, 0);
    fn_8011B950(fn_8012640C(ptr, 0, 0xc8, 0), 1);
    fn_801254B4(ptr, 0, 0x6e, 0, 0);
    fn_801254B4(ptr, 0, 0x6f, 0, 0);
    gamedataAttestInit(fn_8012640C(ptr, 0, 0x70, 0));
    if (ptr != NULL) {
        fn_801254B4(ptr, 0, 0x71, 0, 0);
        fn_801254B4(ptr, 0, 0x72, 0, 0);
        fn_801254B4(ptr, 0, 0x73, 0, 0);
        fn_801254B4(ptr, 0, 0x74, 0, 2);
        fn_801254B4(ptr, 0, 0x75, 0, 0);
        fn_801254B4(ptr, 0, 0x76, 0, (u32)&local);
    }
    fn_801254B4(ptr, 0, 0x77, 0, (u32)&local);
    fn_801254B4(ptr, 0, 0x79, 0, 0);
    fn_801254B4(ptr, 0, 0x7a, 0, 0);
    fn_8011B950(fn_8012640C(ptr, 0, 0x7c, 0), 1);
    fn_801254B4(ptr, 0, 0x7d, 0, 0);
    if (ptr != NULL) {
        for (i = 0; (u16)i < 4; i++) {
            if (ptr != NULL) {
                fn_801254B4(ptr, 0, 0x7f, (u32)i, 0);
                fn_801254B4(ptr, 0, 0x80, (u32)i, 0);
                fn_801254B4(ptr, 0, 0x81, (u32)i, 0);
            }
        }
    }
    if (ptr != NULL) {
        fn_8012640C(ptr, 0, 0x82, 0);
        fn_801254B4(ptr, 0, 0x82, 0, 0);
    }
    fn_801254B4(ptr, 0, 0x83, 0, 0);
    fn_801254B4(ptr, 0, 0x87, 0, 0);
    fn_801254B4(ptr, 0, 0x88, 0, 0);
    fn_801254B4(ptr, 0, 0x89, 0, 0);
    fn_801254B4(ptr, 0, 0x8a, 0, 0);
    fn_801254B4(ptr, 0, 0x8b, 0, 0);
    fn_801254B4(ptr, 0, 0x8c, 0, 0);
    fn_801254B4(ptr, 0, 0x8d, 0, 0);
    fn_801254B4(ptr, 0, 0x8e, 0, 0);
    fn_801254B4(ptr, 0, 0x8f, 0, 0);
    fn_801254B4(ptr, 0, 0x90, 0, 0);
    fn_801254B4(ptr, 0, 0x91, 0, 0);
    fn_801254B4(ptr, 0, 0x92, 0, 0);
    fn_801254B4(ptr, 0, 0x93, 0, 0);
    fn_801254B4(ptr, 0, 0x94, 0, 0);
    fn_801254B4(ptr, 0, 0x95, 0, 0);
    fn_801254B4(ptr, 0, 0x96, 0, 0);
    fn_801254B4(ptr, 0, 0x97, 0, 0);
    fn_801254B4(ptr, 0, 0x98, 0, 0);
    fn_801254B4(ptr, 0, 0x99, 0, 0);
    fn_801254B4(ptr, 0, 0x9c, 0, 0);
    fn_801254B4(ptr, 0, 0x9d, 0, 0);
    fn_801254B4(ptr, 0, 0x9e, 0, 0);
    fn_801254B4(ptr, 0, 0x9f, 0, 0);
    fn_801254B4(ptr, 0, 0xa0, 0, 0);
    fn_801254B4(ptr, 0, 0xa1, 0, 0);
    fn_801254B4(ptr, 0, 0xa3, 0, 0);
    fn_801254B4(ptr, 0, 0xa4, 0, 0);
    fn_801254B4(ptr, 0, 0xa5, 0, 0);
    fn_801254B4(ptr, 0, 0xa6, 0, 0);
    fn_801254B4(ptr, 0, 0xa7, 0, 0);
    fn_801254B4(ptr, 0, 0xa8, 0, 0);
    fn_801254B4(ptr, 0, 0xa9, 0, 0);
    fn_801254B4(ptr, 0, 0xaa, 0, 0);
    fn_801254B4(ptr, 0, 0xab, 0, 0);
    fn_801254B4(ptr, 0, 0xac, 0, 0);
    fn_801254B4(ptr, 0, 0xad, 0, 0);
    fn_801254B4(ptr, 0, 0xae, 0, 0);
    fn_801254B4(ptr, 0, 0xaf, 0, 0);
    fn_801254B4(ptr, 0, 0xb0, 0, 0);
    fn_801254B4(ptr, 0, 0xb1, 0, 0);
    fn_801254B4(ptr, 0, 0xb2, 0, 0);
    fn_801254B4(ptr, 0, 0xb3, 0, 0);
    fn_801254B4(ptr, 0, 0xb4, 0, 0);
    fn_801254B4(ptr, 0, 0xb5, 0, 0);
    fn_801254B4(ptr, 0, 0xb6, 0, 0);
    fn_801254B4(ptr, 0, 0xb7, 0, 0);
    fn_801254B4(ptr, 0, 0xb8, 0, 0);
    fn_801254B4(ptr, 0, 0xb9, 0, 0);
    fn_801254B4(ptr, 0, 0xbb, 0, 0);
    fn_801254B4(ptr, 0, 0xbc, 0, (u32)0xff);
    fn_801254B4(ptr, 0, 0xbd, 0, 0);
    fn_801254B4(ptr, 0, 0xbe, 0, 0);
    fn_8011D480(ptr, 0);
}
#endif
extern void fn_801FD938(void);
extern void fn_801FD928(void);
extern void fn_801FD918(void);
extern void fn_801FD908(void);
extern void fn_801FD8F8(void);
extern void fn_801FD8D0(void);
extern void fn_801FD8C0(void);
extern void fn_801FD8B0(void);
extern void fn_8020981C(void);
extern void fn_802097C8(void);
extern void fn_8020990C(void);
extern void fn_80209960(void);
extern void fn_80209FAC(void);
extern void fn_801FD8A0(void);
extern void fn_801FD890(void);
extern void fn_801FD880(void);
extern void fn_801FD870(void);
extern void fn_801FD860(void);
extern void fn_801FD850(void);
extern void fn_801FD840(void);
extern void fn_801FD820(void);
extern void fn_801FD7F8(void);
extern void fn_801FCF7C(void);
extern void fn_801FCF6C(void);
extern void fn_801FCF5C(void);
extern void fn_801FCF4C(void);
extern void fn_801FCF3C(void);
extern void fn_801FCF2C(void);
extern void fn_801FCF1C(void);
extern void fn_801FCF0C(void);
extern void fn_801FCEFC(void);
extern void fn_801FD728(void);
extern void fn_801FD718(void);
extern void fn_801FD708(void);
extern void fn_801FD6F8(void);
extern void fn_801FD660(void);
extern void fn_801FD5F0(void);
extern void fn_801FD5C8(void);
extern void fn_801FD6E8(void);
extern void fn_801FD6D8(void);
extern void fn_801FD6C8(void);
extern void fn_801FD6B8(void);
extern void fn_801FD330(void);
extern void fn_801FD320(void);
extern void fn_801FD310(void);
extern void fn_801FD300(void);
extern void fn_801FD2F0(void);
extern void fn_801FD2E0(void);
extern void fn_801FD2D0(void);
extern void fn_801FD2C0(void);
extern void fn_801FD2B0(void);
extern void fn_801FD2A0(void);
extern void fn_801FD290(void);
extern void fn_801FD280(void);
extern void fn_801FD270(void);
extern void fn_801FD260(void);
extern void fn_801FD250(void);
extern void fn_801FD240(void);
extern void fn_801FD230(void);
extern void fn_801FD220(void);
extern void fn_801FD210(void);
extern void fn_801FD200(void);
extern void fn_801FD1F0(void);
extern void fn_801FD1E0(void);
extern void fn_801FD1D0(void);
extern void fn_801FD1C0(void);
extern void fn_801FD1B0(void);
extern void fn_801FD1A0(void);
extern void fn_801FD178(void);
extern void fn_801FD150(void);
extern void jumptable_8035E028();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
void fn_801254B4(void);
extern void fn_8011E4A4(void);
extern void fn_801FDB60(void);
extern void fn_801FDB48(void);
extern void fn_801FDB14(void);
extern void fn_801FDAFC(void);
extern void fn_801FDAE4(void);
extern void fn_801FDACC(void);
extern void fn_801FDAB4(void);
extern void fn_801FD8E0(void);
extern void fn_801FDA9C(void);
extern void fn_801FDA84(void);
extern void fn_801FDA6C(void);
extern void fn_801FDA38(void);
extern void fn_801FDA20(void);
extern void fn_80205184(void);
extern void fn_80205224(void);
extern void fn_802096E8(void);
extern void fn_801FDA08(void);
extern void fn_801FD9F0(void);
extern void fn_801FD9D8(void);
extern void fn_801FD9C0(void);
extern void fn_801FD9A8(void);
extern void fn_801FD990(void);
extern void fn_801FD978(void);
extern void fn_801FD960(void);
extern void fn_801FD948(void);
extern void fn_801FD808(void);
extern void fn_801FD064(void);
extern void fn_801FD04C(void);
extern void fn_801FD034(void);
extern void fn_801FD01C(void);
extern void fn_801FD004(void);
extern void fn_801FCFEC(void);
extern void fn_801FCFD4(void);
extern void fn_801FCFBC(void);
extern void fn_801FCFA4(void);
extern void fn_801FCF8C(void);
extern void fn_801FD7E0(void);
extern void fn_801FD7C8(void);
extern void fn_801FD7B0(void);
extern void fn_801FD798(void);
extern void fn_801FD684(void);
extern void fn_801FD648(void);
extern void fn_801FD614(void);
extern void fn_801FD5D8(void);
extern void fn_801FD5B0(void);
extern void fn_801FD780(void);
extern void fn_801FD768(void);
extern void fn_801FD750(void);
extern void fn_801FD738(void);
extern void fn_801FD598(void);
extern void fn_801FD580(void);
extern void fn_801FD568(void);
extern void fn_801FD550(void);
extern void fn_801FD538(void);
extern void fn_801FD520(void);
extern void fn_801FD508(void);
extern void fn_801FD4F0(void);
extern void fn_801FD4D8(void);
extern void fn_801FD4C0(void);
extern void fn_801FD4A8(void);
extern void fn_801FD490(void);
extern void fn_801FD478(void);
extern void fn_801FD460(void);
extern void fn_801FD448(void);
extern void fn_801FD430(void);
extern void fn_801FD418(void);
extern void fn_801FD400(void);
extern void fn_801FD3E8(void);
extern void fn_801FD3D0(void);
extern void fn_801FD3B8(void);
extern void fn_801FD3A0(void);
extern void fn_801FD388(void);
extern void fn_801FD370(void);
extern void fn_801FD358(void);
extern void fn_801FD340(void);
extern void fn_801FD188(void);
extern void fn_801FD160(void);
extern void fn_801FD11C(void);
extern void fn_802062FC(void);
extern void jumptable_8035E4B0();
/* undecompiled: fn removed (ROM-derived asm), forward-declared for callers */
u32 fn_8012640C(void);
#if 0
asm void heroCheckValid(void) {
#include "src/game/gs_field_world_fn_8012A130.inc"
}
#else
u32 heroCheckValid(u8* ptr) {
    extern u32 fn_8012A5B0(u8* ptr, u32 a, u32 b);
    extern s32 GScharCmp(u32 val, u16* out);
    u16 local = 0;
    if (GScharCmp(fn_8012A5B0(ptr, 1, 0), &local) == 0) { return 0; }
    return fn_8012A5B0(ptr, 0xb, 0) != 2;
}
#endif
extern void fn_801885C4(void);
extern void fn_800A3B7C(void);
extern void fn_8018F678(void);
extern void fn_8018F658(void);
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
extern f64 lbl_8047D068;
extern f32 lbl_8047D038;
extern f64 lbl_8047D048;
extern f64 lbl_8047D050;
extern f64 lbl_8047D058;
extern f32 lbl_8047D080;
extern f32 lbl_8047D060;
extern f32 lbl_8047D094;
extern f32 lbl_8047D098;
extern f32 lbl_8047D09C;
extern f32 lbl_8047D0A0;
extern f32 lbl_8047D0A4;
#if 0
asm void fn_8012CA84(void) {
#include "src/game/gs_field_world_fn_8012CA84.inc"
}
#else
/* FUNCTIONAL decomp of fn_8012CA84
 * Field movement/heading processor - computes turn amount from direction input. */
void fn_8012CA84(s32 playerIdx, f32* dirVec, f32* fwdVec) {
    extern u8 lbl_80478AC0[4];       /* sdata constant 0.0f */
    extern f32 lbl_8047D030;        /* handle pair [0] (read as u32 bits) */
    extern f32 lbl_8047D034;        /* handle pair [1] (read as u32 bits) */
    extern f32 lbl_8047D038;        /* 0.0f */
    extern f32 lbl_8047D060;        /* small-magnitude threshold */
    extern f32 lbl_8047D080;        /* 1.0f */
    extern f32 lbl_8047D094;        /* TWO_PI */
    extern f32 lbl_8047D098;        /* negative angle-wrap bound */
    extern f32 lbl_8047D09C;        /* -PI */
    extern f32 lbl_8047D0A0;        /* PI */
    extern f32 lbl_8047D0A4;        /* minimum turn threshold */
    extern u32  fn_800D3088(void);                        /* frame count */
    extern void fn_800A3A9C(void*, void*, void*);         /* VECSubtract(a, b, out) */
    extern void fn_800A3AC0(void*, void*, f32);           /* VECScale(in, out, s) */
    extern f32  fn_800A3B7C(void*, void*);                /* VECDotProduct */
    extern f64  fn_800CE2D8(f32, f32);                    /* atan2f(y, x) */
    extern void fn_800E3D6C(void*, void*);                /* getRotation(obj, out) */
    extern void fn_800E3D98(void*, void*);                /* getPosition(obj, out) */
    extern void* fn_800F9318(u32, u32);                   /* resolveHandle(group, id) */
    extern void fn_8012C660(void*, s32, f32);             /* applyTurnResult(obj, idx, amt) */
    extern void fn_8018790C(u32, u32);                    /* stopMovement(group, handle) */
    extern void fn_8018805C(u32, u32, f32, f32);          /* setHeading(grp, hdl, angle, spd) */
    extern void fn_801885C4(u32, u32, f32*, u32);         /* setMoveDirection(grp, hdl, dir, flags) */
    extern f32  fn_801887D8(u32, u32, f32*);              /* computeTurnAmount(grp, hdl, dir) */
    extern void fn_8018D998(u32, u32);                    /* selectEntity(grp, hdl) */
    extern void* peopleSearchID(void);                       /* getEntityData() */
    extern void* peopleInfoBiosGetPtr(s32);                        /* getAngleConfig(param) */
    extern f32  fn_8018F678(void*);                       /* getMaxTurnRatePos(obj) */
    extern f32  fn_8018F658(void*);                       /* getMaxTurnRateNeg(obj) */
    extern u8   lbl_80426BD0[];                           /* player state array (stride 0x20) */
    extern f32  sqrtf(f32);                               /* host CRT */

    u32 htbl[2];
    u32 entityHandle = 0;
    u32 finalHandle  = 0;
    f32 turnAmount   = 0.0f;
    f32 frameTime;
    f32 dirMag;
    void* obj;
    f32 posA[3];
    f32 posB[3];
    f32 diffVec[3];
    f32 scaledDir[3];
    f32 targetPos[3];
    f32 playerPos[3];
    f32 rotation[3];

    frameTime = (f32)(u32)fn_800D3088();

    htbl[0] = *(u32*)&lbl_8047D030;
    htbl[1] = *(u32*)&lbl_8047D034;
    if (playerIdx >= 0 && playerIdx < 2) {
        entityHandle = htbl[playerIdx];
    }

    {
        f32 sq = dirVec[0] * dirVec[0] + dirVec[2] * dirVec[2];
        /* original is inline frsqrte + 3x Newton-Raphson; sqrtf is x86-equivalent */
        dirMag = (sq > 0.0f) ? sqrtf(sq) : 0.0f;
    }

    if (dirMag > lbl_8047D038) {
        /* ======== MOVING: direction-based heading ======== */
        f32 clampedSpeed;
        u32 h = 0;

        htbl[0] = *(u32*)&lbl_8047D030;
        htbl[1] = *(u32*)&lbl_8047D034;
        if (playerIdx >= 0 && playerIdx < 2) {
            h = htbl[playerIdx];
        }
        obj = fn_800F9318(0, h);
        fn_800E3D98(obj, posA);
        posA[1] = lbl_8047D038;

        fn_801885C4(0, entityHandle, dirVec, 0);

        h = 0;
        htbl[0] = *(u32*)&lbl_8047D030;
        htbl[1] = *(u32*)&lbl_8047D034;
        if (playerIdx >= 0 && playerIdx < 2) {
            h = htbl[playerIdx];
        }
        obj = fn_800F9318(0, h);
        fn_800E3D98(obj, posB);
        posB[1] = lbl_8047D038;

        fn_800A3A9C(posB, posA, diffVec);

        clampedSpeed = dirMag / frameTime;
        if (clampedSpeed > lbl_8047D080) {
            clampedSpeed = lbl_8047D080;
        }

        fn_800A3AC0(diffVec, scaledDir, clampedSpeed / dirMag);

        {
            f32 fwdSq = fwdVec[0] * fwdVec[0] + fwdVec[2] * fwdVec[2];
            f32 fwdMag = (fwdSq > 0.0f) ? sqrtf(fwdSq) : 0.0f;

            if (fwdMag > lbl_8047D060) {
                f32 angle = (f32)fn_800CE2D8(fwdVec[0], fwdVec[2]);
                fn_8018805C(0, entityHandle, angle, clampedSpeed);
            }
        }

        turnAmount = fn_801887D8(0, entityHandle, scaledDir);

        if (fn_800A3B7C(diffVec, fwdVec) < lbl_8047D038) {
            turnAmount = -turnAmount;
        }

    } else {
        /* ======== STATIONARY: face-target or idle ======== */
        u8 hasTarget = 0;
        u32 mode;

        if (playerIdx >= 0 && playerIdx < 2) {
            u16 flags = *(u16*)(&lbl_80426BD0[playerIdx * 0x20] + 4);
            hasTarget = (u8)(flags & 1);
        }

        mode = hasTarget
             ? *(u32*)(&lbl_80426BD0[playerIdx * 0x20] + 0xC)
             : 2u;

        if (mode != 1) {
            fn_8018790C(0, entityHandle);
            turnAmount = lbl_8047D038;
        } else {
            /* ---- FACE-TARGET: rotate toward another entity ---- */
            s32 targetIdx;
            u32 tgtH = 0, plrH = 0, rotH = 0, turnH = 0;
            f32 heading, targetAngle, angleDiff;
            f32 newAngle = 0.0f;
            f32 maxPos, maxNeg;
            void* angleObj;
            void* tPtr;
            s32 turnParam;
            u8 shouldTurn = 0;
            u8 turnValid  = 0;

            targetIdx = *(s32*)lbl_80426BD0;
            htbl[0] = *(u32*)&lbl_8047D030;
            htbl[1] = *(u32*)&lbl_8047D034;
            if (targetIdx >= 0 && targetIdx < 2) {
                tgtH = htbl[targetIdx];
            }
            obj = fn_800F9318(0, tgtH);
            fn_800E3D98(obj, targetPos);

            htbl[0] = *(u32*)&lbl_8047D030;
            htbl[1] = *(u32*)&lbl_8047D034;
            if (playerIdx >= 0 && playerIdx < 2) {
                plrH = htbl[playerIdx];
            }
            obj = fn_800F9318(0, plrH);
            fn_800E3D98(obj, playerPos);

            htbl[0] = *(u32*)&lbl_8047D030;
            htbl[1] = *(u32*)&lbl_8047D034;
            if (playerIdx >= 0 && playerIdx < 2) {
                rotH = htbl[playerIdx];
            }
            obj = fn_800F9318(0, rotH);
            fn_800E3D6C(obj, rotation);

            heading = rotation[1];
            while (heading >= lbl_8047D094) { heading -= lbl_8047D094; }
            while (heading <= lbl_8047D098) { heading += lbl_8047D094; }
            rotation[1] = heading;

            targetAngle = (f32)fn_800CE2D8(
                targetPos[0] - playerPos[0],
                targetPos[2] - playerPos[2]
            );

            angleDiff = targetAngle - heading;
            if (angleDiff < lbl_8047D09C) {
                angleDiff += lbl_8047D094;
            } else if (angleDiff > lbl_8047D0A0) {
                angleDiff -= lbl_8047D094;
            }

            htbl[0] = *(u32*)&lbl_8047D030;
            htbl[1] = *(u32*)&lbl_8047D034;
            if (playerIdx >= 0 && playerIdx < 2) {
                turnH = htbl[playerIdx];
                turnValid = 1;
            }

            if (!turnValid) {
                turnParam = -1;
            } else {
                fn_8018D998(0, turnH);
                tPtr = peopleSearchID();
                if (tPtr != NULL) {
                    turnParam = *(s32*)((u8*)tPtr + 0x30);
                } else {
                    turnParam = -1;
                }
            }

            angleObj = peopleInfoBiosGetPtr(turnParam);

            {
                f32 r = fn_8018F678(angleObj);
                maxPos = (r > lbl_8047D038)
                       ? fn_8018F678(angleObj)
                       : -fn_8018F678(angleObj);
            }

            {
                f32 r = fn_8018F658(angleObj);
                maxNeg = (r > lbl_8047D038)
                       ? fn_8018F658(angleObj)
                       : -fn_8018F658(angleObj);
            }

            if (angleDiff < lbl_8047D038) {
                f32 absDiff = -angleDiff;
                if (absDiff > maxNeg) {
                    newAngle = targetAngle + maxNeg;
                    if (newAngle >= lbl_8047D094) {
                        newAngle -= lbl_8047D094;
                    }
                    shouldTurn = 1;
                }
            } else {
                if (angleDiff > maxPos) {
                    f32 adj = targetAngle - maxPos;
                    if (adj < lbl_8047D09C) {
                        adj += lbl_8047D094;
                    } else if (adj > lbl_8047D0A0) {
                        adj -= lbl_8047D094;
                    }
                    newAngle = adj;
                    shouldTurn = 1;
                }
            }

            if (shouldTurn) {
                f32 delta   = newAngle - heading;
                f32 wrapped = delta;
                f32 magnitude;

                if (delta < lbl_8047D09C) {
                    wrapped = delta + lbl_8047D094;
                } else if (delta > lbl_8047D0A0) {
                    wrapped = delta - lbl_8047D094;
                }

                if (wrapped > lbl_8047D038) {
                    if (delta < lbl_8047D09C) {
                        magnitude = delta + lbl_8047D094;
                    } else if (delta > lbl_8047D0A0) {
                        magnitude = delta - lbl_8047D094;
                    } else {
                        magnitude = delta;
                    }
                } else {
                    if (delta < lbl_8047D09C) {
                        delta += lbl_8047D094;
                    } else if (delta > lbl_8047D0A0) {
                        delta -= lbl_8047D094;
                    }
                    magnitude = -delta;
                }

                if (magnitude < lbl_8047D0A4) {
                    newAngle   = heading;
                    shouldTurn = 0;
                }
            }

            if (shouldTurn) {
                fn_8018805C(0, entityHandle, newAngle, lbl_8047D080);
                turnAmount = lbl_8047D080;
            } else {
                fn_8018790C(0, entityHandle);
                turnAmount = lbl_8047D038;
            }
        }
    }

    htbl[0] = *(u32*)&lbl_8047D030;
    htbl[1] = *(u32*)&lbl_8047D034;
    if (playerIdx >= 0 && playerIdx < 2) {
        finalHandle = htbl[playerIdx];
    }
    obj = fn_800F9318(0, finalHandle);
    fn_8012C660(obj, playerIdx, turnAmount);
}
#endif
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
#if 0
asm void fn_8012D2BC(void) {
#include "src/game/gs_field_world_fn_8012D2BC.inc"
}
#else
void fn_8012D2BC(u32 param) {
    extern u8 lbl_80426BD0[];
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E3D6C(void* a, u32 b);
    s32 idx;
    u32 table[2];
    u32 val;
    void* result;

    idx = *(s32*)lbl_80426BD0;
    table[0] = *(u32*)&lbl_8047D030;
    table[1] = *(u32*)&lbl_8047D034;
    if (idx >= 0 && idx < 2) {
        val = table[idx];
    }
    result = fn_800F9318(0, val);
    fn_800E3D6C(result, param);
}
#endif
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
#if 0
asm void fn_8012D32C(void) {
#include "src/game/gs_field_world_fn_8012D32C.inc"
}
#else
void fn_8012D32C(u32 param) {
    extern u8 lbl_80426BD0[];
    extern void* fn_800F9318(u32 a, u32 b);
    extern void fn_800E3D98(void* a, u32 b);
    s32 idx;
    u32 table[2];
    u32 val;
    void* result;

    idx = *(s32*)lbl_80426BD0;
    table[0] = *(u32*)&lbl_8047D030;
    table[1] = *(u32*)&lbl_8047D034;
    if (idx >= 0 && idx < 2) {
        val = table[idx];
    }
    result = fn_800F9318(0, val);
    fn_800E3D98(result, param);
}
#endif
extern f32 lbl_8047D030;
extern f32 lbl_8047D034;
#if 0
asm void fn_8012EFB8(void) {
#include "src/game/gs_field_world_fn_8012EFB8.inc"
}
#else
u32 fn_8012EFB8(u32* out_zero, u32* out_val, s32 index) {
    u32 local[2];
    local[0] = *(u32*)&lbl_8047D030;
    local[1] = *(u32*)&lbl_8047D034;
    if (index < 0 || index >= 2) { return 0; }
    *out_zero = 0;
    *out_val = local[index];
    return 1;
}
#endif
