/**
 * @file tracefx.c
 * @brief TraceFX -- Trail / trace visual effects for Pokemon Colosseum.
 *
 * Decompiled from:
 *   fn_801364A8                          -- Trail resource/model load helper
 *   fn_80137114 (tracefxRender)          -- Per-frame trail rendering
 *   fn_8013735C (tracefxInit)            -- Initialise a TraceFXWork structure
 *   fn_8013757C (tracefxStartEffectImpl) -- Internal start implementation
 *   fn_80137780 (tracefxStopEffectImpl)  -- Internal stop / cleanup
 *   fn_8013796C (tracefxStartUpdate)     -- Begin trail update cycle
 *   fn_801379E4 (tracefxSetTrailParam)   -- Set a trail parameter
 *   fn_80137A2C (tracefxSetTrailColor)   -- Set trail RGBA colour
 *   tracefxStartEffect (tracefxStartEffect)     -- Public start API
 *   fn_80137D14 (tracefxAddSegment)      -- Add segments to a running trail
 *   fn_80137F58 (tracefxUpdate)          -- Per-frame trail logic update
 *
 * A prior campaign transplant had left invented duplicate definitions
 * named tracefxInit, tracefxAddSegment, and tracefxUpdate that reimplement
 * fn_8013735C, fn_80137D14, and fn_80137F58 under friendly names; none had
 * any callers anywhere in the tree (the real fn_ names are what get called
 * -- see fn_8013796C below), so they have been removed, along with the
 * tracefxStartEffect_Draft helper that only existed to call the fictional
 * tracefxInit and was itself unreferenced.
 *
 * Debug strings:
 *   "tracefxStartEffect: Could not start trail effect!"
 *       (lbl_80272B08 -- referenced when allocation fails)
 *
 * The trail effect system renders motion trails behind moving objects
 * (e.g., attack animations, Pokemon tails).  Each trail consists of a
 * chain of segments that are generated from model bone positions,
 * interpolated over time, and faded out as they age.
 *
 * A trail effect works by:
 *   1. Loading two model references: a "start bone" and an "end bone".
 *   2. Each frame, sampling the bone positions and creating a quad strip
 *      between consecutive samples.
 *   3. Applying colour fade and width taper over the segment lifetime.
 *   4. Rendering via the GS rendering pipeline (GSpart model system).
 *
 * fn_801364A8 was merged in from the former game/effect/effect_util.c
 * CodeCandidate bucket (0x8013151C - 0x80137114): it shares the
 * lbl_8047D118/lbl_8047D11C/lbl_8047D128 state and directly calls
 * fn_80137114/fn_8013735C/fn_8013757C/fn_80137780 below, so it is the
 * true first function of this translation unit.
 *
 * Address range: 0x801364A8 - 0x801380D4 (approx.)
 */

#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "game/effect/gs_effect.h"

/* ===== External engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);          /* OSReport / GSlog */
extern u32   GSgfxGetFrameCount(void);                   /* fn_800D37CC */
extern void* GSresGetResource(u32 group, u32 model);         /* GSfloor model load */
extern void* GSmodelGetPart(void* model, u16 partIdx);     /* GSpart get sub-part */
extern void  GSpartGetTransform(void* part, void* outPos,
                          void* a, void* b);             /* GSpart get position */
extern void  GSpartFree(void* part);                    /* GSpart commit */
extern void  GSvecCopy(void* dst, void* src);         /* Vec3 copy */
extern void  GSlerpGetLinearInterpolationVector(void* dst, void* srcA,
                          void* srcB, f32 t);            /* Vec3 lerp */
extern void  GSmodelSetVisibility(void* model, u32 flag);        /* GSpart set visibility */
extern u16   _toolentryAlloc__FUl(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void  fn_800E24B0(u16 handle);                   /* GSmemLock/free step */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */
extern u32   wazaSequenceSysGetResID(void);                          /* Random seed generator */
extern void  fn_8010147C(u32 memOffset, u32 resId,
                          u32 size, u32 handle);         /* GSfloor load resource */
extern void  fn_801013A0(u32 memOffset, u32 size,
                          u32 data, u32 handle);         /* GSfloor load data */
extern void  memset(void* dst, u32 val, u32 size);
extern void  GXDrawDone(void);
extern void  fn_800B856C(void);
extern void  GStextureFree(void* p);
extern void* fn_80131428(void* owner, u32 size);
extern void  fn_80131200();
extern void  fn_8013139C(void* obj, u32 flag);

/* ===== GS immediate-mode render API (used by tracefxUpdate) ===== */
extern void  _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID(void);
extern void  fn_800DA4C4(s32 a, s32 b, s32 c);
extern void  fn_800DA2BC(s32 a, s32 b, s32 c);
extern void  fn_800DA1E8(s32 a, s32 b, s32 c);
extern void  fn_800DA028(s32 a);
extern void  fn_800D88DC(s32 a);
extern void  fn_800D888C(s32 a);
extern void  fn_800D7820(void* p);
extern void  fn_800D85D4(s32 a, void* p);
extern void  fn_800D6A00(s32 a);
extern void  fn_800D67BC(s32 a);
extern void  fn_800D6680(f32 x, f32 y, f32 z);
extern void  fn_800D5CB8(s32 a, u8 r, u8 g, u8 b, u8 al);
extern void  fn_800D59B8(s32 a, f32 s, f32 t);
extern void  fn_800D6728(void);

/* ===== Forward declarations (vtable callbacks) ===== */
BOOL  fn_801379E4(u8* w);
BOOL  fn_80137A2C(u8* w);
BOOL  fn_80137D14(u8* work, u32 steps);
int   fn_80137F58(u8* w);
BOOL  tracefxStartEffect(u8* w);

/* ===== String constants (rodata) ===== */
extern const char lbl_80272B08[]; /* "tracefxStartEffect: Could not start trail effect!" */

/* ===== SDA21 float constants ===== */
extern f32 lbl_8047D118;   /* 60.0f -- frames-per-second constant */
extern f32 lbl_8047D11C;
extern f64 lbl_8047D128;   /* 4503599627370496.0 -- int-to-float magic */
extern f32 lbl_8047D130;   /* lerp denominator constant */
extern f64 lbl_8047D140;   /* int-to-float magic (unsigned) */


/* 0x801364A8 | 0xC6C */
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
    extern void GSvecCopy();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void GSmodelSetVisibility();
    extern void GStextureSetWrap();
    extern void fn_800EFD14();
    extern void GStextureLoad();
    extern void GSresGetResource();
    extern void fn_801013A0();
    extern void fn_8010147C();
    extern void fn_80137114();
    extern void fn_8013735C();
    extern void fn_8013757C();
    extern void fn_80137780();
    extern void fn_8013D604();
    extern void wazaSequenceSysGetResID();
    extern u32 jumptable_80363C70[];
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
        GSvecCopy();
        r3 = r28 + 0x34;
        r4 = r29 + 0x24;
        GSvecCopy();
        r3 = r28 + 0x1c;
        r4 = r29 + 0xc;
        GSvecCopy();
        r3 = r28 + 0x28;
        r4 = r29 + 0x18;
        GSvecCopy();
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
            GStextureLoad();
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
            GStextureLoad();
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
            GStextureSetWrap();
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
        wazaSequenceSysGetResID();
        *(u16*)((u8*)r28 + 0x4A) = r3;
        wazaSequenceSysGetResID();
        *(u16*)((u8*)r28 + 0x4C) = r3;
        r3 = r27;
        r5 = 0x4e20;
        r4 = *(u32*)((u8*)r29 + 0x34);
        r6 = *(u16*)((u8*)r28 + 0x4A);
        fn_8010147C();
        r4 = *(u16*)((u8*)r28 + 0x4A);
        r3 = 0x4e20;
        GSresGetResource();
        r6 = *(u16*)((u8*)r28 + 0x4C);
        r4 = 0x4e20;
        r5 = 0x0;
        fn_801013A0();
        r4 = *(u16*)((u8*)r28 + 0x4C);
        r3 = 0x4e20;
        GSresGetResource();
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            GSmodelSetVisibility();
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
        wazaSequenceSysGetResID();
        *(u32*)((u8*)r28 + 0x60) = r3;
        wazaSequenceSysGetResID();
        *(u32*)((u8*)r28 + 0x5C) = r3;
        r3 = r31;
        r5 = 0x4e20;
        r4 = *(u32*)((u8*)r29 + 0x2C);
        r6 = *(u32*)((u8*)r28 + 0x60);
        fn_8010147C();
        r4 = *(u32*)((u8*)r28 + 0x60);
        r3 = 0x4e20;
        GSresGetResource();
        r6 = *(u32*)((u8*)r28 + 0x5C);
        r4 = 0x4e20;
        r5 = 0x0;
        fn_801013A0();
        r4 = *(u32*)((u8*)r28 + 0x5C);
        r3 = 0x4e20;
        GSresGetResource();
        if (r3 != (u32)0x0) {
            r4 = 0x0;
            GSmodelSetVisibility();
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
            wazaSequenceSysGetResID();
            *(u32*)((u8*)r28 + 0xC) = r3;
            wazaSequenceSysGetResID();
            *(u32*)((u8*)r28 + 0x10) = r3;
            r3 = r27;
            r5 = 0x4e20;
            r4 = *(u32*)((u8*)r29 + 0x0);
            r6 = *(u32*)((u8*)r28 + 0xC);
            fn_8010147C();
            r4 = *(u32*)((u8*)r28 + 0xC);
            r3 = 0x4e20;
            GSresGetResource();
            r6 = *(u32*)((u8*)r28 + 0x10);
            r4 = 0x4e20;
            r5 = 0x0;
            fn_801013A0();
            r4 = *(u32*)((u8*)r28 + 0x10);
            r3 = 0x4e20;
            GSresGetResource();
            if (r3 != (u32)0x0) {
                r4 = 0x0;
                GSmodelSetVisibility();
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
            GStextureLoad();
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

/* ===================================================================
 * Generated: 0 pattern-matched + 11 stubs
 * Range: 0x801364A8 - 0x801380D4
 * =================================================================== */

/* 0x80137114 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u8* fn_80137114(u8* work, u8* params, s32 frames) {
    extern void fn_8013DB64(u8*, s32, f32, f32);
    u8* data;
    s32 count;
    s32 duration;
    s32 i;
    f32 value;

    count = *(s32*)params;
    memset(work, 0, 0x20);
    if (*(s32*)(params + 4) == 3) {
        *(u32*)(work + 0x10) = *(u32*)(params + 8);
        data = params + 0x0C;
        for (i = 0; i < count; i++, data += 0x10) {
            duration =
                (s32)(((f32)*(s32*)(data + 8) *
                       (f32)(s32)GSgfxGetFrameCount()) /
                      lbl_8047D118);
            fn_8013DB64(work, duration, *(f32*)data,
                        *(f32*)(data + 4));
        }
        return data;
    }
    if (*(s32*)(params + 4) == 2) {
        *(u32*)(work + 0x10) = 1;
        data = params + 8;
        for (i = 0; i < count; i++, data += 0x10) {
            duration =
                (s32)(((f32)*(s32*)(data + 8) *
                       (f32)(s32)GSgfxGetFrameCount()) /
                      lbl_8047D118);
            fn_8013DB64(work, duration, *(f32*)data,
                        *(f32*)(data + 4));
        }
        return data;
    }
    if (*(s32*)(params + 4) == 1) {
        *(u32*)(work + 0x10) = 1;
        value = *(f32*)params;
        duration =
            (s32)(((f32)frames * (f32)(s32)GSgfxGetFrameCount()) /
                  lbl_8047D118) /
            3;
        fn_8013DB64(work, duration, lbl_8047D11C, value);
        fn_8013DB64(work, duration, value, value);
        fn_8013DB64(work, duration, lbl_8047D11C, lbl_8047D11C);
    }
    return params;
}
#pragma pop

/* 0x8013735C | 0x220  tracefxInit */
/* 86.32%: real correct C. Documented src-dup 5-reg stmw reg-coloring (target copies
 * params->r31 so r29 reuses for arena; CW coalescer keeps 4 regs) + float-const
 * band-isolation reloc (lbl_8047D118 @nn) = un-saveable in isolated band. equivalent.txt */
#pragma push
#pragma optimization_level 4
u32 fn_8013735C(void* work, void* params, u32 frames) {
    u8* w = (u8*)work;
    u8* p = (u8*)params;
    u32 arena;
    s32 memOffset;
    void* model;

    memset(w, 0, 0xac);

    switch (*(u32*)(p + 0x3c)) {
    case 1:
        *(u32*)(w + 0xa8) = 0;
        memOffset = -4;
        break;
    case 2:
    default:
        *(u32*)(w + 0xa8) = *(u32*)(p + 0x40);
        memOffset = 0;
        break;
    }

    *(s16*)(w + 0xa6) =
        (s16)(((f32)(s32)frames * (f32)(s32)fn_800D37CC()) / lbl_8047D118);

    *(f32*)(w + 0x48) = *(f32*)(p + 0x00);
    *(f32*)(w + 0x4c) = *(f32*)(p + 0x04);
    *(f32*)(w + 0x50) = *(f32*)(p + 0x08);

    *(s8*)(w + 0x63) = (s8)(*(s32*)(p + 0x0c) >> 24);
    *(u8*)(w + 0x62) = (u8)(*(u32*)(p + 0x0c) >> 16);
    *(u8*)(w + 0x61) = (u8)(*(u32*)(p + 0x0c) >> 8);
    *(s8*)(w + 0x60) = (s8)(*(u32*)(p + 0x0c));

    *(f32*)(w + 0x64) = *(f32*)(p + 0x10);
    *(f32*)(w + 0x68) = *(f32*)(p + 0x14);
    *(f32*)(w + 0x6c) = *(f32*)(p + 0x18);

    *(u16*)(w + 0x70) = (u16)*(u32*)(p + 0x1c);
    *(u16*)(w + 0x72) = (u16)*(u32*)(p + 0x20);
    if (*(u16*)(w + 0x70) % 2 == 0) {
        *(u16*)(w + 0x70) += 1;
    }
    if (*(u16*)(w + 0x72) % 2 == 0) {
        *(u16*)(w + 0x72) += 1;
    }

    *(f32*)(w + 0x90) = *(f32*)(p + 0x24);
    *(f32*)(w + 0x94) = *(f32*)(p + 0x28);
    *(f32*)(w + 0x98) = *(f32*)(p + 0x2c);
    *(f32*)(w + 0x9c) = *(f32*)(p + 0x30);
    *(f32*)(w + 0xa0) = *(f32*)(p + 0x34);

    arena = (((u32)params + memOffset) + 0x63) & ~0x1f;

    *(u32*)(w + 0x74) = 0x4e20;
    *(u32*)(w + 0x7c) = wazaSequenceSysGetResID();
    *(u32*)(w + 0x78) = wazaSequenceSysGetResID();

    fn_8010147C(arena, *(u32*)(p + 0x38), 0x4e20, *(u32*)(w + 0x7c));
    model = GSresGetResource(0x4e20, *(u32*)(w + 0x7c));
    fn_801013A0((u32)model, 0x4e20, 0, *(u32*)(w + 0x78));
    model = GSresGetResource(0x4e20, *(u32*)(w + 0x78));
    if (model != NULL) {
        GSmodelSetVisibility(model, 0);
    }
    arena += (*(u32*)(p + 0x38) + 0x1f) & ~0x1f;
    return arena;
}
#pragma pop

/* 0x8013757C | 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u8* fn_8013757C(u8* work, u8* params, s32 frames) {
    extern u16 fn_800E2C04(u32, u32);
    extern void* GStextureLoad(void*);
    extern void fn_800EFD14(void*, u16);
    extern void GStextureSetWrap(void*, s32, s32);
    extern u8 lbl_80314638[];
    extern u8 lbl_80314AE8[];
    u8* textureData;
    void* texture;
    u16 handle;
    u32 size;
    u32 color;

    memset(work, 0, 0x70);
    *(u16*)(work + 0x12) =
        (u16)(((f32)frames * (f32)(s32)GSgfxGetFrameCount()) /
              lbl_8047D118);

    color = *(u32*)(params + 0);
    work[0x23] = color >> 24;
    work[0x22] = color >> 16;
    work[0x21] = color >> 8;
    work[0x20] = color;
    color = *(u32*)(params + 4);
    work[0x27] = color >> 24;
    work[0x26] = color >> 16;
    work[0x25] = color >> 8;
    work[0x24] = color;
    GSvecCopy(work + 0x34, params + 0x0C);
    GSvecCopy(work + 0x40, params + 0x18);
    *(u16*)(work + 8) = *(u32*)(params + 8);
    *(f32*)(work + 0x4C) = *(f32*)(params + 0x24);
    *(f32*)(work + 0x50) = *(f32*)(params + 0x28);
    *(f32*)(work + 0x54) = *(f32*)(params + 0x2C);
    *(f32*)(work + 0x58) = *(f32*)(params + 0x30);
    *(f32*)(work + 0x5C) = *(f32*)(params + 0x34);
    *(f32*)(work + 0x60) = *(f32*)(params + 0x38);
    *(f32*)(work + 0x64) = *(f32*)(params + 0x3C);
    *(f32*)(work + 0x68) = *(f32*)(params + 0x40);
    *(f32*)(work + 0x6C) = *(f32*)(params + 0x44);
    *(u16*)(work + 0x0E) = *(u32*)(params + 0x48);

    textureData = (u8*)(((u32)params + 0x73) & ~0x1F);
    size = (*(u32*)(params + 0x4C) + 0x1F) & ~0x1F;
    handle = fn_800E2C04(size, 0x20);
    if (handle != 0) {
        void* copy = fn_800E27B0(handle);
        memcpy(copy, textureData, size);
        texture = GStextureLoad(copy);
        *(void**)(work + 0x1C) = texture;
        fn_800EFD14(texture, handle);
    } else {
        *(void**)(work + 0x1C) = NULL;
    }
    textureData += size;
    texture = *(void**)(work + 0x1C);
    if (texture != NULL) {
        GStextureSetWrap(texture, 2, 2);
    }
    *(void**)(work + 0x14) = lbl_80314638;
    *(void**)(work + 0x18) = lbl_80314AE8;
    return textureData;
}
#pragma pop

/* 0x80137780 | 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u8* fn_80137780(u8* work, u8* params) {
    typedef struct TraceColor {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } TraceColor;
    extern void fn_8013AB60(u8*, TraceColor*, TraceColor*, s32);
    u8* data;
    TraceColor first;
    TraceColor second;
    u32 firstValue;
    u32 secondValue;
    s32 duration;
    s32 count;
    s32 i;
    s32 offset;
    BOOL useRgb;
    BOOL useAlpha;

    count = *(s32*)(params + 4);
    memset(work, 0, 0x5C);
    work[0x4C] = *(u32*)params;
    useRgb = TRUE;
    useAlpha = FALSE;
    if (*(s32*)(params + 8) == 1 || *(s32*)(params + 8) == 2) {
        offset = -4;
    } else {
        offset = 0;
        if ((*(u32*)(params + 0x0C) & 4) != 0) {
            useRgb = FALSE;
        }
        if ((*(u32*)(params + 0x0C) & 1) != 0) {
            useAlpha = TRUE;
        }
    }
    if (*(u32*)params == 0) {
        useRgb = TRUE;
        useAlpha = TRUE;
    }
    work[0x4E] = useAlpha;
    data = params + offset + 0x10;

    for (i = 0; i < count; i++, data += 0x10) {
        firstValue = *(u32*)(data + 0);
        secondValue = *(u32*)(data + 4);
        if (useRgb) {
            first.r = firstValue;
            first.g = firstValue >> 8;
            first.b = firstValue >> 16;
            second.r = secondValue;
            second.g = secondValue >> 8;
            second.b = secondValue >> 16;
        } else {
            first.r = first.g = first.b = 0x7F;
            second.r = second.g = second.b = 0x7F;
        }
        if (useAlpha) {
            first.a = firstValue >> 24;
            second.a = secondValue >> 24;
        } else {
            first.a = second.a = 0xFF;
        }
        duration = *(s32*)(data + 8);
        if (duration >= 0) {
            duration =
                (s32)(((f32)duration * (f32)(s32)GSgfxGetFrameCount()) /
                      lbl_8047D118);
        } else {
            duration = -1;
        }
        fn_8013AB60(work, &first, &second, duration);
    }
    return data;
}
#pragma pop

/* 0x8013796C | 0x78 */
#pragma push
#pragma optimization_level 4
void* fn_8013796C(void* owner) {
    void* obj = fn_80131428(owner, 0x2C);
    if (obj != (void*)0) {
        fn_80131200(obj, 0, fn_801379E4, tracefxStartEffect, fn_80137A2C, 0,
                    fn_80137D14, fn_80137F58);
        fn_8013139C(obj, 0);
    }
    return obj;
}
#pragma pop

/* 0x801379E4 | 0x48 */
#pragma push
#pragma optimization_level 4
BOOL fn_801379E4(u8* w) {
    if (w != (void*)0) {
        GXDrawDone();
        fn_800B856C();
        if (*(void**)(w + 0x14) != (void*)0) {
            GStextureFree(*(void**)(w + 0x14));
        }
    }
    return TRUE;
}
#pragma pop

/* 0x80137A2C | 0x78 */
#pragma push
#pragma optimization_level 4
BOOL fn_80137A2C(u8* w) {
    u16 handle;
    if (w != (void*)0) {
        GXDrawDone();
        fn_800B856C();
        handle = *(u16*)(w + 0x0C);
        if (handle != 0) {
            fn_800E24B0(handle);
            fn_800E209C(handle);
        }
        handle = *(u16*)(w + 0x0E);
        if (handle != 0) {
            fn_800E24B0(handle);
            fn_800E209C(handle);
        }
    }
    return TRUE;
}
#pragma pop

/* 0x80137AA4 | 0x270 */
#pragma push
#pragma optimization_level 4
BOOL tracefxStartEffect(u8* w) {
    void* model;
    u16 max_count;
    u8* nodes;
    void* part;
    u8* weights;
    u32 i;
    u16 handle;
    u32 last_index;
    u32 count32;
    f32 step;
    u32 node_bytes;
    u16 count;

    if (w == (void*)0) {
        goto fail;
    }

    max_count = *(u16*)(w + 0x20);
    count = *(u16*)(w + 0x22);
    model = GSresGetResource(*(u16*)(w + 0x24), *(u16*)(w + 0x26));

    if (*(void**)(w + 0x14) == (void*)0) {
        return FALSE;
    }
    if (count == 0 || max_count == 0) {
        return FALSE;
    }
    if (model == (void*)0) {
        return FALSE;
    }

    part = GSmodelGetPart(model, *(u16*)(w + 0x28));
    if (part == (void*)0) {
        return FALSE;
    }
    GSpartFree(part);

    part = GSmodelGetPart(model, *(u16*)(w + 0x2A));
    if (part == (void*)0) {
        return FALSE;
    }
    GSpartFree(part);

    if (count > (max_count >> 1)) {
        count = max_count >> 1;
        *(u16*)(w + 0x22) = count;
    }

    node_bytes = (u16)count << 5;
    count32 = (u16)count;
    handle = _toolentryAlloc__FUl(node_bytes);
    if (handle == 0) {
        return FALSE;
    }

    *(u16*)(w + 0x0C) = handle;
    nodes = (u8*)fn_800E27B0(handle);
    *(u8**)(w + 0x04) = nodes;
    *(u8**)(w + 0x00) = nodes;
    memset(nodes, 0, node_bytes);

    last_index = count32 - 1;
    for (i = 0; (u16)i < count32; i++) {
        if ((u16)i == 0) {
            *(u8**)(nodes + (((u16)i << 5) + 0x1C)) = nodes + (last_index << 5);
        } else {
            *(u8**)(nodes + (((u16)i << 5) + 0x1C)) = nodes + (((u16)i - 1) << 5);
        }
        if ((u16)i == last_index) {
            *(u8**)(nodes + (((u16)i << 5) + 0x18)) = nodes;
        } else {
            *(u8**)(nodes + (((u16)i << 5) + 0x18)) = nodes + (((u16)i + 1) << 5);
        }
    }

    handle = _toolentryAlloc__FUl(count32 << 4);
    if (handle == 0) {
        fn_800E24B0(*(u16*)(w + 0x0C));
        fn_800E209C(*(u16*)(w + 0x0C));
        return FALSE;
    }

    *(u16*)(w + 0x0E) = handle;
    weights = (u8*)fn_800E27B0(handle);
    *(u8**)(w + 0x08) = weights;
    *(u16*)(w + 0x1C) = 0;
    *(u16*)(w + 0x1E) = 0;

    count32 = *(u16*)(w + 0x22);
    step = 1.0f / (f32)(s32)(count32 - 1);
    for (i = 0; (u16)i < count32; i++) {
        f32 t = (f32)(u32)(u16)i * step;
        *(f32*)(weights + 0x00) = t;
        *(f32*)(weights + 0x04) = 0.0f;
        *(f32*)(weights + 0x08) = t;
        *(f32*)(weights + 0x0C) = 1.0f;
        weights += 0x10;
    }

    return TRUE;

fail:
    GSlogWrite(lbl_80272B08);
    return FALSE;
}
#pragma pop

/* 0x80137D14 | 0x244 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
BOOL fn_80137D14(u8* work, u32 steps) {
    void* model;
    void* part;
    u8* node;
    Vec start;
    Vec end;
    Vec previousStart;
    Vec previousEnd;
    f32 fraction;
    u32 i;

    if (*(u16*)(work + 0x1C) >= *(u16*)(work + 0x20)) {
        return FALSE;
    }
    model = GSresGetResource(*(u16*)(work + 0x24),
                             *(u16*)(work + 0x26));
    if (model == NULL || *(void**)(work + 0x14) == NULL) {
        return FALSE;
    }
    part = GSmodelGetPart(model, *(u16*)(work + 0x28));
    if (part == NULL) {
        return FALSE;
    }
    GSpartGetTransform(part, &start, NULL, NULL);
    GSpartFree(part);
    part = GSmodelGetPart(model, *(u16*)(work + 0x2A));
    if (part == NULL) {
        return FALSE;
    }
    GSpartGetTransform(part, &end, NULL, NULL);
    GSpartFree(part);

    node = *(u8**)work;
    if (*(u16*)(work + 0x1C) != 0) {
        GSvecCopy(&previousStart, node);
        GSvecCopy(&previousEnd, node + 0x0C);
        fraction = 1.0f / (f32)steps;
    }
    for (i = 0; i < steps; i++) {
        node = *(u8**)(node + 0x18);
        if (*(u16*)(work + 0x1C) != 0) {
            f32 t = fraction * (f32)(i + 1);
            GSlerpGetLinearInterpolationVector(
                node, &previousStart, &start, t);
            GSlerpGetLinearInterpolationVector(
                node + 0x0C, &previousEnd, &end, t);
        } else {
            GSvecCopy(node, &start);
            GSvecCopy(node + 0x0C, &end);
        }
    }
    *(u8**)work = node;
    *(u16*)(work + 0x1C) += steps;
    *(u16*)(work + 0x1E) += steps;
    if (*(u16*)(work + 0x1C) > *(u16*)(work + 0x20)) {
        *(u16*)(work + 0x1C) = *(u16*)(work + 0x20);
    }
    if (*(u16*)(work + 0x1E) > *(u16*)(work + 0x22)) {
        *(u16*)(work + 0x1E) = *(u16*)(work + 0x22);
    }
    if (*(u16*)(work + 0x1E) >
        *(u16*)(work + 0x20) - *(u16*)(work + 0x1C)) {
        *(u16*)(work + 0x1E) =
            *(u16*)(work + 0x20) - *(u16*)(work + 0x1C);
    }
    return TRUE;
}
#pragma pop

/* 0x80137F58 | 0x17C */
#pragma push
#pragma optimization_level 4
int fn_80137F58(u8* w) {
    u16 count1;
    u16 count2;
    u8* node;
    u8* weight;
    u32 i;

    count1 = *(u16*)(w + 0x1C);
    count2 = *(u16*)(w + 0x1E);
    if (*(void**)(w + 0x14) == (void*)0) {
        return 0;
    }
    if (count1 <= 1) goto ret0;
    if (count2 <= 1) goto ret0;

    _cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID();
    fn_800DA4C4(1, 6, 7);
    fn_800DA2BC(1, 1, 0);
    fn_800DA1E8(1, 2, 1);
    fn_800DA028(0);
    fn_800D88DC(3);
    fn_800D888C(4);
    fn_800D7820(*(void**)(w + 0x10));
    fn_800D85D4(0, *(void**)(w + 0x14));
    fn_800D6A00(4);
    fn_800D67BC((count2 & 0x7FFF) << 1);

    node = *(u8**)(w + 0x00);
    weight = *(u8**)(w + 0x08) + (*(u16*)(w + 0x22) - count2) * 16;
    for (i = 0; (u16)i < count2; i++) {
        fn_800D6680(*(f32*)(node + 0x00), *(f32*)(node + 0x04), *(f32*)(node + 0x08));
        fn_800D5CB8(0, w[0x18], w[0x19], w[0x1A], w[0x1B]);
        fn_800D59B8(0, *(f32*)(weight + 0x00), *(f32*)(weight + 0x04));
        fn_800D6680(*(f32*)(node + 0x0C), *(f32*)(node + 0x10), *(f32*)(node + 0x14));
        fn_800D5CB8(0, w[0x18], w[0x19], w[0x1A], w[0x1B]);
        fn_800D59B8(0, *(f32*)(weight + 0x08), *(f32*)(weight + 0x0C));
        node = *(u8**)(node + 0x1C);
        weight += 0x10;
    }
    fn_800D6728();
    return 1;
ret0:
    return 0;
}
#pragma pop
