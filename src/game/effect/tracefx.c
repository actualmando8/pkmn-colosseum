/**
 * @file tracefx.c
 * @brief TraceFX -- Trail / trace visual effects for Pokemon Colosseum.
 *
 * Decompiled from:
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
 * Address range: 0x80137114 - 0x801380D4 (approx.)
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"

/* ===== External engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);          /* OSReport / GSlog */
extern u32   GSgfxGetFrameCount(void);                   /* fn_800D37CC */
extern void* GSresGetResource(u32 group, u32 model);         /* GSfloor model load */
extern void* GSmodelGetPart(void* model, u16 partIdx);     /* GSpart get sub-part */
extern void  GSpartGetTransform(void* part, void* outPos,
                          void* a, void* b);             /* GSpart get position */
extern void  GSpartFree(void* part);                    /* GSpart commit */
extern void  fn_800E01D0(void* dst, void* src);         /* Vec3 copy */
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
extern void  fn_800EF5A4(void* p);
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
void  fn_80137D14(void);
int   fn_80137F58(u8* w);
BOOL  tracefxStartEffect(u8* w);

/* ===== String constants (rodata) ===== */
extern const char lbl_80272B08[]; /* "tracefxStartEffect: Could not start trail effect!" */

/* ===== SDA21 float constants ===== */
extern f32 lbl_8047D118;   /* 60.0f -- frames-per-second constant */
extern f64 lbl_8047D128;   /* 4503599627370496.0 -- int-to-float magic */
extern f32 lbl_8047D130;   /* lerp denominator constant */
extern f64 lbl_8047D140;   /* int-to-float magic (unsigned) */

/* ===================================================================
 * Generated: 0 pattern-matched + 10 stubs
 * Range: 0x80137114 - 0x801380D4
 * =================================================================== */

/* 0x80137114 | 0x248 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80137114(void) {
    /* TODO: match -- 584 bytes at 0x80137114 */
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
void fn_8013757C(void) {
    /* TODO: match -- 516 bytes at 0x8013757C */
}
#pragma pop

/* 0x80137780 | 0x1EC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80137780(void) {
    /* TODO: match -- 492 bytes at 0x80137780 */
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
            fn_800EF5A4(*(void**)(w + 0x14));
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
void fn_80137D14(void) {
    /* TODO: match -- 580 bytes at 0x80137D14 */
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
