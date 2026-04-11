/**
 * @file effect_visual.c
 * @brief Visual effect sub-modules for Pokemon Colosseum.
 *
 * This file contains the individual visual effect implementations that
 * plug into the GSeffect system (gs_effect.c). Each effect registers
 * its own start/update/render/stop callbacks via GSEffectAllocSlot
 * and GSEffectRegister.
 *
 * Address range: 0x801380D4 - 0x801402AC (approximately 0x8200 bytes)
 *
 * Sub-modules identified by rodata debug strings:
 *
 *   lightningStartEffect   (fn_801380D4)  -- Lightning bolt effect
 *     "lightningStartEffect: Could not start lightning effect -
 *      invalid model: group %d, model %d."
 *     "lightningStartEffect: Could not start lightning effect -
 *      not enough memory!"
 *     "lightningStartEffect: Could not start lightning effect!"
 *     Struct size: 0x70 bytes (passed to GSEffectAllocSlot)
 *     Callbacks: start=fn_80138630, stop=fn_801386DC,
 *                update=fn_80138680, render=fn_801387C0
 *     Core logic: fn_8013814C (0x4E4 bytes -- main render with
 *       GX pipeline setup: fog, lighting, material color,
 *       position/rotation matrix, GX draw calls)
 *     Helper: fn_80138838 (0x2C8 bytes -- model transform)
 *
 *   leaffxStartEffect      (fn_80138B00)  -- Leaf particle effect
 *     "leaffxStartEffect: Could not start leaf effect -
 *      invalid leaf model: groupRes %d, modelRes %d!"
 *     "leaffxStartEffect: Could not start leaf effect!"
 *     "_leaffxGenerateLeafData: Could not create leaf clone model."
 *     Struct size: 0x98 bytes
 *     Callbacks: start=fn_80138CCC, stop=fn_80139074,
 *                update=fn_80138BBC, render=fn_80139378
 *     Core logic: fn_80138DE4 (0x290 bytes), fn_80139074 (0x304 bytes)
 *
 *   electronStartEffect    (fn_80139820)  -- Electrical arc effect
 *     "electronStartEffect: Could not start electron effect -
 *      invalid model: groupRes %d, modelRes %d!"
 *     "leaffxStartEffect: Could not start electron effect!"
 *     Struct size: 0x78 bytes
 *     Callbacks: start=fn_801398E0, stop=fn_8013A1D4,
 *                update=fn_80139934, render=fn_80139AC4
 *     Core logic: fn_80139D10 (0x170 bytes), fn_80139E80 (0x354 bytes)
 *
 *   filterStart             (fn_8013A42C)  -- Full-screen colour filter
 *     "filterStart: Could not start filter effect - invalid filter!"
 *     "filterStart: Could not start filter effect!"
 *     Struct size: 0x14 bytes
 *     Callbacks: start=fn_8013A520, stop=fn_8013AB60,
 *                update=fn_8013A49C, render=fn_8013AD9C
 *     Core logic: fn_8013A520 (0x56C bytes -- large switch on filter type)
 *
 *   surfEffectStart         (fn_8013AABC)  -- Water surface wave effect
 *     "surfEffectStart: Could not start wave effect!"
 *     Struct size: 0x28 bytes
 *
 *   seaEffectStart          (fn_8013B490)  -- Ocean/sea effect
 *     "seaEffectStart: Could not start sea effect!"
 *     Struct size: 0x2C bytes
 *     Callbacks: start=fn_8013B558, stop=fn_8013B85C,
 *                update=fn_8013B504, render=fn_8013B5E4
 *
 *   envMapEffectInit        (fn_8013C5A0)  -- Environment-mapped reflections
 *     "envMapEffectInit: Could not initialise env map effect!"
 *     "envMapEffectStart: Could not start env map effect!"
 *     Struct size: 0x48 bytes
 *     Callbacks: start=fn_8013C670, stop=fn_8013CBF0,
 *                update=fn_8013C614, render=fn_8013C718
 *     Core logic: fn_8013C074 (0x52C bytes), fn_8013CA48 (0x1A8 bytes),
 *                 fn_8013CE58 (0x250 bytes)
 *
 *   blurEffectStart         (fn_8013D6B8)  -- Motion blur post-process
 *     "blurEffectStart: Could not start blur effect!"
 *     Struct size: 0x24 bytes
 *     Callbacks: start=fn_8013D730, stop=fn_8013D984,
 *                update=fn_8013D7CC, render=fn_8013D804
 *     Core logic: fn_8013D0A8 (0x55C bytes -- large GX pipeline setup)
 *
 *   auraEffectStart         (fn_8013DC18)  -- Aura glow (Shadow Pokemon)
 *     "auraEffectStart: Could not start aura effect!"
 *     Struct size: 0x20 bytes
 *     Callbacks: start=fn_8013DDCC, stop=fn_8013E258,
 *                update=fn_8013DC94, render=fn_8013DE6C
 *     Core logic: fn_8013DE6C (0x3EC bytes -- aura rendering)
 *
 *   distortionEffectStart   (fn_8013E4D4)  -- Screen distortion / heat-haze
 *     "distortionEffectStart: Could not start distortion effect!"
 *     "_distortionEffectUpdateMatrices: Could not project points on screen"
 *     Struct size: 0x50 bytes
 *     Uses rodata string "translate" for matrix node lookup
 *     Callbacks: start=fn_8013E5AC, stop=fn_8013E8A4,
 *                update=fn_8013E54C, render=fn_8013E658
 *     Core logic: fn_8013E6C4 (0x1E0 bytes), fn_8013EA44 (0x5BC bytes)
 *
 *   billboardEffectStart    (fn_8013F000)  -- Billboard sprite particles
 *     "billboardEffectStart: Could not start billboard effect!"
 *     Struct size: 0xB4 bytes
 *     Callbacks: start=fn_8013F114, stop=fn_8013F344,
 *                update=fn_8013F078, render=fn_8013F410
 *     Core logic: fn_8013F80C (0x170 bytes), fn_8013F97C (0x264 bytes),
 *                 fn_8013FF0C (0x22C bytes -- billboard transform setup)
 *
 *   patchiru texture effect (fn_8013FBE0)  -- Patchiru (Spinda) special effect
 *     "Failed to create Patchiru texture"
 *     Struct size: 0x40 bytes
 *     Uses custom texture generation for the Spinda spot pattern
 *     Callbacks: start=fn_8013FCC4, stop=fn_8013FDD0,
 *                update=fn_8013FC58, render=fn_8013FD68
 *
 * External references:
 *   fn_80131428 (GSEffectAllocSlot)  -- allocate effect slot
 *   fn_80131200 (GSEffectRegister)   -- register callbacks
 *   fn_8013139C (GSEffectResetState) -- re-trigger effect
 *   fn_800F9318 (floor resource lookup)
 *   fn_800E3D98 (GSmem / resource helper)
 *   fn_800D2248, fn_800D2584, fn_800D1F84, fn_800D1FDC -- matrix/vector ops
 *   fn_800DA4C4, fn_800DA2BC, fn_800DA1E8, fn_800DA028 -- GX TEV/material setup
 *   fn_800D88DC, fn_800D888C -- GX blend/alpha mode
 *   fn_800D7820 -- GX draw begin
 *   fn_800E05C0, fn_800E048C -- matrix position/rotation
 *   fn_800D7F14 -- GX load matrix
 *   fn_800D6A00 -- GX cull mode
 *   fn_800D67BC, fn_800D6680, fn_800D5CB8 -- GX position/color/texcoord
 *   fn_800B9404 -- fog setup
 *   fn_800B8DF4, fn_800B856C -- GX begin/end frame
 *   fn_800EF5A4 -- model release
 *   fn_801779EC -- camera matrix helper
 *
 * All 12 sub-modules follow the same pattern:
 *   1. xxxStart allocates a slot via fn_80131428 with a struct size
 *   2. Registers callbacks via fn_80131200
 *   3. Calls fn_8013139C to enter IDLE state
 *   4. The update callback handles per-frame logic
 *   5. The render callback sets up GX pipeline and draws geometry
 *   6. The stop callback frees resources
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);     /* OSReport */
extern void* memset(void* dst, int val, u32 size);

/* GSmem allocator */
extern u16   fn_800E3534(u32 size);
extern void* fn_800E27B0(u16 handle);

/* Effect system core */
extern u16   fn_80131428(void* callbacks, u16 dataSize);  /* GSEffectAllocSlot */
extern void  fn_80131200(u32 effectId,
                          GSEffectStartFunc startFunc,
                          GSEffectStopFunc  destroyFunc,
                          GSEffectStartFunc triggerFunc,
                          GSEffectStopFunc  stopFunc,
                          void* extraParam,
                          GSEffectUpdateFunc updateFunc,
                          GSEffectRenderFunc renderFunc);  /* GSEffectRegister */
extern void  fn_8013139C(u32 effectId, u32 param);        /* GSEffectResetState */

/* Floor resource system */
extern void* fn_800F9318(u16 group, u16 model);

/* Matrix / vector operations */
extern void  fn_800E3D98(void* dst, void* src);
extern void  fn_800D2248(void);
extern void  fn_800D2584(void);
extern void  fn_800D1F84(void* mtx, void* vec);
extern void  fn_800D1FDC(void* mtx, void* rx, void* ry, void* rz, void* scale);
extern void  fn_800E0040(void* vecA, void* vecB);
extern void  fn_800E0168(void* dst, void* srcA, void* srcB);
extern void  fn_800E0060(void* dst, void* src);

/* GX rendering pipeline */
extern void  fn_800DA4C4(u32 a, u32 b, u32 c);
extern void  fn_800DA2BC(u32 a, u32 b, u32 c);
extern void  fn_800DA1E8(u32 a, u32 b, u32 c);
extern void  fn_800DA028(u32 a);
extern void  fn_800D88DC(u32 a);
extern void  fn_800D888C(u32 a);
extern void  fn_800D7820(void* model);
extern void  fn_800E05C0(void* dst, f32 x, f32 y, f32 z);
extern void  fn_800E048C(void* mtx, f32 x, f32 y, f32 z);
extern void  fn_800D7F14(void* mtx);
extern void  fn_800D6A00(u32 mode);
extern void  fn_800D67BC(u16 index);
extern void  fn_800D6680(f32 x, f32 y, f32 z);
extern void  fn_800D5CB8(u32 a, u8 r, u8 g, u8 b, u8 alpha);
extern void  fn_800B9404(u32 index, u32 param);
extern void  fn_800B8DF4(void);
extern void  fn_800B856C(void);
extern void  fn_800EF5A4(void* model);
extern void  fn_800D9B58(u32 param);
extern void  fn_800D9ED8(u32 param);

/* Camera */
extern void* fn_801779EC(void);

/* DCFlush */
extern void  DCFlushRange(void* ptr, u32 size);

/* ===================================================================
 * STUBS -- Functions in this translation unit.
 *
 * These are stub declarations for the 148 functions in the range
 * 0x801380D4 - 0x801402AC.  Each corresponds to a function in the
 * auto-generated asm file auto_01_800055E0_text.s.
 *
 * Full decompilation of each is deferred; the asm files remain the
 * authoritative implementation.  The function addresses and sizes are
 * documented here for cross-reference.
 * =================================================================== */

/* ---- Lightning effect ---- */
/* fn_801380D4: lightningStartEffect -- allocate slot, register, reset */
/* fn_8013814C: _lightningRenderMain (0x4E4 bytes) -- GX pipeline + draw */
/* fn_80138630: _lightningStart callback */
/* fn_80138680: _lightningUpdate callback */
/* fn_801386DC: _lightningStop callback */
/* fn_801387C0: _lightningRender callback */
/* fn_80138838: _lightningTransformModel (0x2C8 bytes) */

/* ---- Leaf effect ---- */
/* fn_80138B00: leaffxStartEffect */
/* fn_80138B74: _leaffxHelper */
/* fn_80138BBC: _leaffxUpdate callback */
/* fn_80138CCC: _leaffxStart callback */
/* fn_80138DE4: _leaffxGenerateLeafData (0x290 bytes) */
/* fn_80139074: _leaffxStop callback (0x304 bytes) */
/* fn_80139378: _leaffxRender callback (0x4A8 bytes) */

/* ---- Electron effect ---- */
/* fn_80139820: electronStartEffect */
/* fn_80139898: _electronHelper */
/* fn_801398E0: _electronStart callback */
/* fn_80139934: _electronUpdate callback (0x190 bytes) */
/* fn_80139AC4: _electronRender callback (0x24C bytes) */
/* fn_80139D10: _electronCalcArc (0x170 bytes) */
/* fn_80139E80: _electronRenderArc (0x354 bytes) */
/* fn_8013A1D4: _electronStop callback (0x258 bytes) */

/* ---- Filter effect ---- */
/* fn_8013A42C: filterAlloc */
/* fn_8013A49C: _filterUpdate callback */
/* fn_8013A520: _filterStart callback (0x56C bytes -- large switch) */
/* fn_8013AA8C: _filterHelper */
/* fn_8013AABC: surfEffectStart */
/* fn_8013AB34: _surfHelper */
/* fn_8013AB60: _filterStop callback (0x208 bytes) */
/* fn_8013AD68: _filterHelper2 */
/* fn_8013AD9C: _filterRender callback (0x298 bytes) */

/* ---- Surf / wave effect ---- */
/* fn_8013B034: _surfUpdate callback */
/* fn_8013B0A0: _surfRender callback */
/* fn_8013B158: _surfCalcWave (0x110 bytes) */
/* fn_8013B268: _surfRenderWave (0x228 bytes) */

/* ---- Sea effect ---- */
/* fn_8013B490: seaEffectStart */
/* fn_8013B504: _seaUpdate callback */
/* fn_8013B558: _seaStart callback */
/* fn_8013B5E4: _seaRender callback (0x278 bytes) */
/* fn_8013B85C: _seaStop callback (0x23C bytes) */
/* fn_8013BA98: _seaCalcSurface (0x178 bytes) */
/* fn_8013BC10: _seaRenderSurface (0x1F4 bytes) */
/* fn_8013BE04: _seaRenderReflection (0x270 bytes) */

/* ---- EnvMap effect ---- */
/* fn_8013C074: _envMapRenderMain (0x52C bytes) */
/* fn_8013C5A0: envMapEffectInit */
/* fn_8013C614: _envMapUpdate callback */
/* fn_8013C670: _envMapStart callback */
/* fn_8013C718: _envMapRender callback (0x330 bytes) */
/* fn_8013CA48: _envMapCalcReflection (0x1A8 bytes) */
/* fn_8013CBF0: _envMapStop callback (0x268 bytes) */
/* fn_8013CE58: _envMapUpdateTexture (0x250 bytes) */

/* ---- Blur effect ---- */
/* fn_8013D0A8: _blurRenderMain (0x55C bytes -- large GX setup) */
/* fn_8013D604: _blurHelper */
/* fn_8013D6B8: blurEffectStart */
/* fn_8013D730: _blurStart callback */
/* fn_8013D7CC: _blurUpdate callback */
/* fn_8013D804: _blurRender callback (0x104 bytes) */
/* fn_8013D908: _blurCalcMotion */
/* fn_8013D984: _blurStop callback (0x1E0 bytes) */
/* fn_8013DB64: _blurCleanup */

/* ---- Aura effect (Shadow Pokemon) ---- */
/* fn_8013DC18: auraEffectStart -- allocate 0x20 bytes */
/* fn_8013DC94: _auraUpdate callback */
/* fn_8013DD10: _auraHelper */
/* fn_8013DD7C: _auraHelper2 */
/* fn_8013DDCC: _auraStart callback */
/* fn_8013DE6C: _auraRender callback (0x3EC bytes) */
/* fn_8013E258: _auraStop callback (0x218 bytes) */

/* ---- Distortion / heat-haze effect ---- */
/* fn_8013E470: _distortionHelper */
/* fn_8013E4D4: distortionEffectStart */
/* fn_8013E54C: _distortionUpdate callback */
/* fn_8013E5AC: _distortionStart callback */
/* fn_8013E658: _distortionRender callback */
/* fn_8013E6C4: _distortionCalcMatrices (0x1E0 bytes) */
/* fn_8013E8A4: _distortionStop callback (0x1A0 bytes) */
/* fn_8013EA44: _distortionEffectUpdateMatrices (0x5BC bytes) */

/* ---- Billboard sprite effect ---- */
/* fn_8013F000: billboardEffectStart -- allocate 0xB4 bytes */
/* fn_8013F078: _billboardUpdate callback */
/* fn_8013F114: _billboardStart callback (0x230 bytes) */
/* fn_8013F344: _billboardStop callback */
/* fn_8013F410: _billboardRender callback (0x3FC bytes) */
/* fn_8013F80C: _billboardCalcTransform (0x170 bytes) */
/* fn_8013F97C: _billboardRenderQuad (0x264 bytes) */

/* ---- Patchiru (Spinda) texture effect ---- */
/* fn_8013FBE0: patchiru texture start -- allocate 0x40 bytes */
/* fn_8013FC58: _patchiruUpdate callback */
/* fn_8013FCC4: _patchiruStart callback */
/* fn_8013FD68: _patchiruRender callback */
/* fn_8013FDD0: _patchiruStop callback (0x13C bytes) */

/* ---- Billboard transform helpers ---- */
/* fn_8013FF0C: _billboardTransformSetup (0x22C bytes) */
/* fn_80140138: _billboardTransformHelper */
/* fn_80140190: _billboardTransformHelper2 (0x11C bytes) */
/* fn_801402AC: _billboardTransformMain (0x2DC bytes) */
extern void fn_800E008C(void);
extern void fn_800D6728(void);
extern void fn_800D85D4(void);
extern void fn_800E0BA0(void);
extern void fn_800D59B8(void);
extern void fn_800D7E5C(void);
extern u32 lbl_8047D148;
extern u32 lbl_8047D14C;
extern u32 lbl_8047D150;
extern u32 lbl_8047D158;
extern u32 lbl_8047D154;
/* Forward declarations for self-referencing asm blocks */
extern void fn_80138838(void* ptr, u32 b);
extern u32 fn_80138B74(void* ptr);
extern void fn_80138BBC(void);
extern void fn_80138CCC(void);
extern void fn_80138DE4(void);
extern void fn_80139074(void);
extern void fn_80139378(void);
extern u32 fn_80139898(void* ptr);
extern u32 fn_801398E0(void* ptr);
extern void fn_80139934(void);
extern void fn_80139AC4(void);
extern void fn_80139D10(void);
extern void fn_80139E80(void);
extern void fn_8013A1D4(void);
extern u32 fn_8013A49C(void* ptr);
extern void fn_8013A520(void);
extern u32 fn_8013AA8C(void* ptr, u16 delta);
extern u32 fn_8013AD68(void* ptr);
extern void fn_8013AD9C(void);
extern u32 fn_8013B034(void* ptr);
extern void fn_8013B0A0(void);
extern void fn_8013B158(void);
extern void fn_8013B268(void);
extern u32 fn_8013B504(void* ptr);
extern u32 fn_8013B558(void* ptr);
extern void fn_8013B5E4(void);
extern void fn_8013B85C(void);
extern void fn_8013BA98(void);
extern void fn_8013BC10(void);
extern void fn_8013BE04(void);
extern void fn_8013C074(void);
extern u32 fn_8013C614(void* ptr);
extern void fn_8013C670(void);
extern void fn_8013C718(void);
extern void fn_8013CA48(void);
extern void fn_8013CBF0(void);
extern void fn_8013CE58(void* inner, void* ptr);
extern void fn_8013D0A8(void);
extern u32 fn_8013D730(void* ptr);
extern u32 fn_8013D7CC(void* ptr);
extern u32 fn_8013D804(void* ptr);
extern u32 fn_8013D908(void* ptr);
extern void fn_8013D984(void);
extern u32 fn_8013DC94(void* ptr);
extern u32 fn_8013DD10(void* ptr);
extern u32 fn_8013DD7C(void* ptr);
extern void fn_8013DDCC(void);
extern void fn_8013DE6C(void);
extern void fn_8013E258(void);
extern void fn_8013E470(void);
extern void fn_8013E54C(void);
extern void fn_8013E5AC(void);
extern u32 fn_8013E658(void* ptr);
extern void fn_8013E6C4(void);
extern void fn_8013E8A4(void);
extern void fn_8013EA44(void);
extern void fn_8013F078(void);
extern void fn_8013F114(void);
extern void fn_8013F344(void);
extern void fn_8013F410(void);
extern void fn_8013F80C(void);
extern void fn_8013F97C(void);
extern void fn_8013FC58(void);
extern void fn_8013FCC4(void);
extern u32 fn_8013FD68(void* ptr);
extern void fn_8013FDD0(void);
extern void fn_8013FF0C(void);

#if 1
asm void fn_8013814C(void) {
#include "src/game/effect/effect_visual_fn_8013814C.inc"
}
#else
void fn_8013814C(void) { /* TODO */ }
#endif
#if 0
asm void fn_80138630(void* ptr) {
#include "src/game/effect/effect_visual_fn_80138630.inc"
}
#else
u32 fn_80138630(void* ptr) {
    if (ptr) {
        fn_800B8DF4();
        fn_800B856C();
        if (*(void**)((u8*)ptr + 0x1c) != NULL) {
            fn_800EF5A4(*(void**)((u8*)ptr + 0x1c));
        }
        return 1;
    }
    return 0;
}
#endif
extern void fn_800E24B0(u16 a);
extern void fn_800E209C(u16 a);
#if 0
asm void fn_80138680(void* ptr) {
#include "src/game/effect/effect_visual_fn_80138680.inc"
}
#else
u32 fn_80138680(void* ptr) {
    u32 val;
    u32 ret;
    if (ptr) {
        val = *(u16*)ptr;
        fn_800B8DF4();
        fn_800B856C();
        if (val) {
            fn_800E24B0(val);
            fn_800E209C(val);
        }
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
#endif
extern u8 lbl_80272B40[];
#if 1
asm void fn_801386DC(void) {
#include "src/game/effect/effect_visual_fn_801386DC.inc"
}
#else
void fn_801386DC(void) { /* TODO */ }
#endif
#if 1
asm void fn_801387C0(void* ptr, u32 delta) {
#include "src/game/effect/effect_visual_fn_801387C0.inc"
}
#else
u32 fn_801387C0(void* ptr, u32 delta) {
    u32 max;
    if (ptr == NULL) { goto fail; }
    max = *(u16*)((u8*)ptr + 0x12);
    if (max == 0) { goto work; }
    if (*(u16*)((u8*)ptr + 0x10) >= max) { goto fail; }
work:
    fn_80138838(ptr, 0);
    *(u16*)((u8*)ptr + 0x10) = *(u16*)((u8*)ptr + 0x10) + delta;
    return 1;
fail:
    return 0;
}
#endif
extern void fn_800E0BE4(void);
extern void fn_800CDBE0(void);
extern void fn_800CE148(void);
extern void fn_800E01F4(void);
extern void fn_800E01D0(void);
extern void fn_800DFFCC(void);
extern void fn_800E0718(void);
extern void fn_800DFEEC(void);
extern void fn_800E0518(void);
extern void fn_800DFF98(void);
extern void fn_800E013C(void);
extern void fn_800E019C(void);
extern u32 lbl_8047D154;
extern u32 lbl_8047D15C;
extern u32 lbl_8047D14C;
#if 1
asm void fn_80138838(void* ptr, u32 b) {
#include "src/game/effect/effect_visual_fn_80138838.inc"
}
#else
void fn_80138838(void* ptr, u32 b) { /* TODO */ }
#endif
#if 1
asm void fn_80138B00(void) {
#include "src/game/effect/effect_visual_fn_80138B00.inc"
}
#else
void fn_80138B00(void) { /* TODO */ }
#endif
extern void fn_800F9210(u32 a, u32 b);
#if 0
asm void fn_80138B74(void* ptr) {
#include "src/game/effect/effect_visual_fn_80138B74.inc"
}
#else
u32 fn_80138B74(void* ptr) {
    if (ptr) {
        fn_800F9210(*(u16*)((u8*)ptr + 0x40), *(u16*)((u8*)ptr + 0x44));
        fn_800F9210(*(u16*)((u8*)ptr + 0x40), *(u16*)((u8*)ptr + 0x42));
    }
    return 1;
}
#endif
extern void fn_800E4014(void* a, u32 b);
extern void fn_800E4BF4(void);
extern void fn_800EE0E8(void);
extern void fn_800EE150(void);
extern void fn_800EE758(void);
extern void fn_800EE6B4(void);
extern void fn_800DF21C(void);
extern void fn_800DF608(void);
extern void fn_800EE828(void);
extern u32 lbl_8047D160;
#if 1
asm void fn_80138BBC(void) {
#include "src/game/effect/effect_visual_fn_80138BBC.inc"
}
#else
void fn_80138BBC(void) { /* TODO */ }
#endif
extern u8 lbl_80272C30[];
extern u8 lbl_80272C90[];
#if 1
asm void fn_80138CCC(void) {
#include "src/game/effect/effect_visual_fn_80138CCC.inc"
}
#else
void fn_80138CCC(void) { /* TODO */ }
#endif
extern void fn_800E07E4(void);
extern void fn_800E06B8(void);
extern void fn_800E040C(void);
extern void fn_800E02C4(void);
extern void fn_800E03B4(void);
extern void fn_800E4598(void);
extern u32 lbl_8047D170;
extern u32 lbl_8047D164;
extern u32 lbl_8047D178;
extern u32 lbl_8047D160;
extern u32 lbl_8047D168;
#if 1
asm void fn_80138DE4(void) {
#include "src/game/effect/effect_visual_fn_80138DE4.inc"
}
#else
void fn_80138DE4(void) { /* TODO */ }
#endif
extern void fn_800E076C(void);
extern u32 lbl_8047D160;
extern u32 lbl_8047D180;
extern u32 lbl_8047D184;
#if 1
asm void fn_80139074(void) {
#include "src/game/effect/effect_visual_fn_80139074.inc"
}
#else
void fn_80139074(void) { /* TODO */ }
#endif
extern void fn_800E4C98(void);
extern void fn_800EE3BC(void);
extern u8 lbl_80272CC4[];
extern u32 lbl_8047D168;
extern u32 lbl_8047D180;
extern u32 lbl_8047D184;
extern u32 lbl_8047D188;
#if 1
asm void fn_80139378(void) {
#include "src/game/effect/effect_visual_fn_80139378.inc"
}
#else
void fn_80139378(void) { /* TODO */ }
#endif
#if 1
asm void fn_80139820(void) {
#include "src/game/effect/effect_visual_fn_80139820.inc"
}
#else
void fn_80139820(void) { /* TODO */ }
#endif
#if 0
asm void fn_80139898(void* ptr) {
#include "src/game/effect/effect_visual_fn_80139898.inc"
}
#else
u32 fn_80139898(void* ptr) {
    if (ptr) {
        fn_800B8DF4();
        fn_800B856C();
        if (*(void**)((u8*)ptr + 0x58) != NULL) {
            fn_800EF5A4(*(void**)((u8*)ptr + 0x58));
        }
    }
    return 1;
}
#endif
#if 0
asm void fn_801398E0(void* ptr) {
#include "src/game/effect/effect_visual_fn_801398E0.inc"
}
#else
u32 fn_801398E0(void* ptr) {
    u32 val;
    if (ptr) {
        val = *(u16*)((u8*)ptr + 0x4);
        if (val) {
            fn_800B8DF4();
            fn_800B856C();
            fn_800E24B0(val);
            fn_800E209C(val);
        }
    }
    return 1;
}
#endif
extern void fn_800EE7E0(void);
extern u32 lbl_8047D190;
#if 1
asm void fn_80139934(void) {
#include "src/game/effect/effect_visual_fn_80139934.inc"
}
#else
void fn_80139934(void) { /* TODO */ }
#endif
extern u32 lbl_8047D198;
extern u32 lbl_8047D1A0;
#if 1
asm void fn_80139AC4(void) {
#include "src/game/effect/effect_visual_fn_80139AC4.inc"
}
#else
void fn_80139AC4(void) { /* TODO */ }
#endif
extern u8 lbl_80272D08[];
extern u32 lbl_8047D190;
extern u8 lbl_80272D54[];
#if 1
asm void fn_80139D10(void) {
#include "src/game/effect/effect_visual_fn_80139D10.inc"
}
#else
void fn_80139D10(void) { /* TODO */ }
#endif
extern void fn_800E0108(void);
extern void fn_800E0560(void);
extern void fn_800E042C(void);
extern void fn_800E00AC(void);
extern u32 lbl_8047D1A0;
extern u32 lbl_8047D1A8;
extern u32 lbl_8047D190;
extern u32 lbl_8047D1AC;
extern u32 lbl_8047D1B0;
#if 1
asm void fn_80139E80(void) {
#include "src/game/effect/effect_visual_fn_80139E80.inc"
}
#else
void fn_80139E80(void) { /* TODO */ }
#endif
extern u32 lbl_8047D1B4;
extern u32 lbl_8047D190;
extern u32 lbl_8047D1A0;
extern u32 lbl_8047D1A8;
extern u32 lbl_8047D1AC;
#if 1
asm void fn_8013A1D4(void) {
#include "src/game/effect/effect_visual_fn_8013A1D4.inc"
}
#else
void fn_8013A1D4(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013A42C(void) {
#include "src/game/effect/effect_visual_fn_8013A42C.inc"
}
#else
void fn_8013A42C(void) { /* TODO */ }
#endif
extern u8 lbl_80272D90[];
extern u8 lbl_80272DF4[];
#if 0
asm u32 fn_8013A49C(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013A49C.inc"
}
#else
u32 fn_8013A49C(void* ptr) {
    void* res;
    if (ptr) {
        res = fn_800F9318(*(u16*)((u8*)ptr + 0xc), *(u16*)((u8*)ptr + 0xe));
        if (res == NULL) {
            fn_800DD970((const char*)lbl_80272D90, *(u16*)((u8*)ptr + 0xc), *(u16*)((u8*)ptr + 0xe));
            return 0;
        }
        *(u16*)((u8*)ptr + 0x8) = 0;
        return 1;
    }
    fn_800DD970((const char*)lbl_80272DF4);
    return 0;
}
#endif
extern void fn_800C46B0(void);
extern u32 lbl_8047D1B8;
extern u32 lbl_8047D1BC;
extern u32 lbl_8047D1C0;
extern u32 lbl_8047D1C4;
extern u32 lbl_8047D1C8;
extern u32 lbl_8047D1CC;
extern u32 lbl_8047D1D0;
extern u32 lbl_8047D1D8;
#if 1
asm void fn_8013A520(void) {
#include "src/game/effect/effect_visual_fn_8013A520.inc"
}
#else
void fn_8013A520(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013AA8C(void* ptr, u16 delta) {
#include "src/game/effect/effect_visual_fn_8013AA8C.inc"
}
#else
u32 fn_8013AA8C(void* ptr, u16 delta) {
    u16 cur;
    u16 max;
    if (ptr) {
        cur = *(u16*)((u8*)ptr + 0x8);
        max = *(u16*)((u8*)ptr + 0xa);
        if (cur < max) {
            *(u16*)((u8*)ptr + 0x8) = cur + delta;
            return 1;
        }
    }
    return 0;
}
#endif
#if 1
asm void fn_8013AABC(void) {
#include "src/game/effect/effect_visual_fn_8013AABC.inc"
}
#else
void fn_8013AABC(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013AB34(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013AB34.inc"
}
#else
u32 fn_8013AB34(void* ptr) {
    void* inner;
    u32 val;
    inner = *(void**)((u8*)ptr + 0x54);
    if (inner) {
        val = *(u32*)((u8*)inner + 0x8);
        if (val == 0xFFFFFFFF) {
            return 1;
        }
    }
    return 0;
}
#endif
extern u32 lbl_8047D1E8;
extern u32 lbl_8047D1E0;
#if 1
asm void fn_8013AB60(void) {
#include "src/game/effect/effect_visual_fn_8013AB60.inc"
}
#else
void fn_8013AB60(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013AD68(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013AD68.inc"
}
#else
u32 fn_8013AD68(void* ptr) {
    void* ptr2;
    if (ptr) {
        ptr2 = *(void**)((u8*)ptr + 0x54);
        if (ptr2) {
            *(void**)((u8*)ptr + 0x54) = *(void**)((u8*)ptr2 + 0x10);
            *(u32*)((u8*)ptr + 0x58) = 0;
            return 1;
        }
    }
    return 0;
}
#endif
extern void fn_80168408(void);
extern u32 lbl_8047D1E8;
extern u32 lbl_8047D1F0;
extern u32 lbl_8047D1F4;
#if 1
asm void fn_8013AD9C(void) {
#include "src/game/effect/effect_visual_fn_8013AD9C.inc"
}
#else
void fn_8013AD9C(void) { /* TODO */ }
#endif
#if 0
asm void fn_8013B034(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013B034.inc"
}
#else
u32 fn_8013B034(void* ptr) {
    void* next;
    u32 val;
    u32 ret;
    if (ptr) {
        ptr = *(void**)((u8*)ptr + 0x50);
        while (ptr != NULL) {
            val = *(u16*)((u8*)ptr + 0xc);
            next = *(void**)((u8*)ptr + 0x10);
            fn_800E24B0(val);
            fn_800E209C(val);
            ptr = next;
        }
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
#endif
extern void fn_800E5A74(void);
extern void fn_800E638C(void);
extern void fn_801684F0(void);
#if 1
asm void fn_8013B0A0(void) {
#include "src/game/effect/effect_visual_fn_8013B0A0.inc"
}
#else
void fn_8013B0A0(void) { /* TODO */ }
#endif
extern void fn_800E6478(void);
extern void fn_800E5BE0(void);
extern void fn_80168570(void);
extern u8 lbl_80363CA8[];
extern u8 lbl_80272E30[];
extern u8 lbl_80272E70[];
#if 1
asm void fn_8013B158(void) {
#include "src/game/effect/effect_visual_fn_8013B158.inc"
}
#else
void fn_8013B158(void) { /* TODO */ }
#endif
extern void fn_800E5B68(void);
extern u32 lbl_8047D1E8;
extern u32 lbl_8047D1F8;
extern u32 lbl_8047D1F0;
extern u32 lbl_8047D1FC;
#if 1
asm void fn_8013B268(void) {
#include "src/game/effect/effect_visual_fn_8013B268.inc"
}
#else
void fn_8013B268(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013B490(void) {
#include "src/game/effect/effect_visual_fn_8013B490.inc"
}
#else
void fn_8013B490(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013B504(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013B504.inc"
}
#else
u32 fn_8013B504(void* ptr) {
    if (ptr) {
        if (*(s32*)((u8*)ptr + 0x58) != 0) {
            fn_800F9210(*(u32*)((u8*)ptr + 0x50), *(u32*)((u8*)ptr + 0x54));
            fn_800F9210(*(u32*)((u8*)ptr + 0x50), *(u32*)((u8*)ptr + 0x58));
        }
    }
    return 1;
}
#endif
#if 0
asm u32 fn_8013B558(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013B558.inc"
}
#else
u32 fn_8013B558(void* ptr) {
    u16 val;
    if (ptr) {
        if (*(void**)ptr != NULL) {
            fn_800E4014(*(void**)ptr, 0);
        }
        fn_800B8DF4();
        fn_800B856C();
        val = *(u16*)((u8*)ptr + 0x20);
        if (val) {
            fn_800E24B0(val);
            fn_800E209C(val);
        }
        val = *(u16*)((u8*)ptr + 0xc0);
        if (val) {
            fn_800E24B0(val);
            fn_800E209C(val);
        }
    }
    return 1;
}
#endif
extern void fn_800E2C04(void);
extern void fn_800E0E14(void);
extern void fn_800E4170(void);
extern void fn_800EC1B0(void);
extern void fn_800EC35C(void);
extern void fn_800EC308(void);
extern void fn_800EC2A4(void);
extern void fn_800EC208(void);
extern void fn_800EC1E4(void);
extern u32 lbl_8047D200;
extern u32 lbl_8047D204;
extern u8 lbl_80272EA0[];
#if 1
asm void fn_8013B5E4(void) {
#include "src/game/effect/effect_visual_fn_8013B5E4.inc"
}
#else
void fn_8013B5E4(void) { /* TODO */ }
#endif
extern void fn_800E09E8(void);
extern u32 lbl_8047D208;
#if 1
asm void fn_8013B85C(void) {
#include "src/game/effect/effect_visual_fn_8013B85C.inc"
}
#else
void fn_8013B85C(void) { /* TODO */ }
#endif
extern u8 lbl_8031554C[];
extern u8 lbl_80315540[];
extern u32 lbl_8047D200;
#if 1
asm void fn_8013BA98(void) {
#include "src/game/effect/effect_visual_fn_8013BA98.inc"
}
#else
void fn_8013BA98(void) { /* TODO */ }
#endif
extern void fn_800E0204(void);
extern u32 lbl_8047D200;
extern u32 lbl_8047D210;
extern u32 lbl_8047D214;
extern u32 lbl_8047D218;
extern u32 lbl_8047D21C;
#if 1
asm void fn_8013BC10(void) {
#include "src/game/effect/effect_visual_fn_8013BC10.inc"
}
#else
void fn_8013BC10(void) { /* TODO */ }
#endif
extern u32 lbl_8047D220;
extern u32 lbl_8047D204;
extern u32 lbl_8047D228;
extern u32 lbl_8047D21C;
extern u32 lbl_8047D208;
#if 1
asm void fn_8013BE04(void) {
#include "src/game/effect/effect_visual_fn_8013BE04.inc"
}
#else
void fn_8013BE04(void) { /* TODO */ }
#endif
extern void fn_8019FF48(void);
extern void fn_801A8440(void);
extern void fn_801A6FF0(void);
#if 1
asm void fn_8013C074(void) {
#include "src/game/effect/effect_visual_fn_8013C074.inc"
}
#else
void fn_8013C074(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013C5A0(void) {
#include "src/game/effect/effect_visual_fn_8013C5A0.inc"
}
#else
void fn_8013C5A0(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013C614(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013C614.inc"
}
#else
u32 fn_8013C614(void* ptr) {
    if (ptr) {
        fn_800B8DF4();
        fn_800B856C();
        if (*(s32*)((u8*)ptr + 0x7c) != 0) {
            fn_800F9210(*(u32*)((u8*)ptr + 0x74), *(u32*)((u8*)ptr + 0x78));
            fn_800F9210(*(u32*)((u8*)ptr + 0x74), *(u32*)((u8*)ptr + 0x7c));
        }
    }
    return 1;
}
#endif
extern void fn_800EC1D4(void* a);
#if 1
asm void fn_8013C670(void) {
#include "src/game/effect/effect_visual_fn_8013C670.inc"
}
#else
void fn_8013C670(void) { /* TODO */ }
#endif
extern void fn_800EC188(void);
extern u32 lbl_8047D230;
extern u32 lbl_8047D234;
extern u32 lbl_8047D238;
extern u32 lbl_8047D23C;
extern u8 lbl_80272ED0[];
#if 1
asm void fn_8013C718(void) {
#include "src/game/effect/effect_visual_fn_8013C718.inc"
}
#else
void fn_8013C718(void) { /* TODO */ }
#endif
extern void fn_800E0CA0(void);
extern u32 lbl_8047D248;
extern u32 lbl_8047D23C;
extern u8 lbl_80363CB8[];
extern u32 lbl_8047D240;
extern u32 lbl_8047D244;
extern u32 lbl_8047D250;
#if 1
asm void fn_8013CA48(void) {
#include "src/game/effect/effect_visual_fn_8013CA48.inc"
}
#else
void fn_8013CA48(void) { /* TODO */ }
#endif
extern u32 lbl_8047D23C;
extern u32 lbl_8047D240;
extern u32 lbl_8047D258;
extern u32 lbl_8047D250;
extern u32 lbl_8047D248;
#if 1
asm void fn_8013CBF0(void) {
#include "src/game/effect/effect_visual_fn_8013CBF0.inc"
}
#else
void fn_8013CBF0(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013CE58(void* inner, void* ptr) {
#include "src/game/effect/effect_visual_fn_8013CE58.inc"
}
#else
void fn_8013CE58(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013D0A8(void) {
#include "src/game/effect/effect_visual_fn_8013D0A8.inc"
}
#else
void fn_8013D0A8(void) { /* TODO */ }
#endif
#if 0
asm void fn_8013D604(void* ptr, u32 val, f32 f1, f32 f2) {
#include "src/game/effect/effect_visual_fn_8013D604.inc"
}
#else
u32 fn_8013D604(void* ptr, u32 val, f32 f1, f32 f2) {
    u16 handle;
    void* node;
    void* cur;
    handle = fn_800E3534(0x14);
    if (handle) {
        node = fn_800E27B0(handle);
        *(u16*)((u8*)node + 0xc) = handle;
        *(f32*)((u8*)node + 0x0) = f1;
        *(f32*)((u8*)node + 0x4) = f2;
        *(u32*)((u8*)node + 0x8) = val;
        cur = *(void**)((u8*)ptr + 0xc);
        if (cur != NULL) {
            while (*(void**)((u8*)cur + 0x10) != NULL) {
                cur = *(void**)((u8*)cur + 0x10);
            }
            *(void**)((u8*)cur + 0x10) = node;
        } else {
            *(void**)((u8*)ptr + 0xc) = node;
        }
        *(void**)((u8*)node + 0x10) = NULL;
    }
    return 0;
}
#endif
#if 1
asm void fn_8013D6B8(void) {
#include "src/game/effect/effect_visual_fn_8013D6B8.inc"
}
#else
void fn_8013D6B8(void) { /* TODO */ }
#endif
extern u32 lbl_8047AEE0;
extern u16 lbl_8047AEE4;
#if 0
asm void fn_8013D730(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013D730.inc"
}
#else
u32 fn_8013D730(void* ptr) {
    void* next;
    u32 val;
    u32 ret;
    if (ptr) {
        next = *(void**)((u8*)ptr + 0xc);
        if (lbl_8047AEE0 != 0) {
            lbl_8047AEE4 -= 1;
            if (lbl_8047AEE4 == 0) {
                fn_800B8DF4();
                fn_800EF5A4((void*)lbl_8047AEE0);
                lbl_8047AEE0 = 0;
            }
        }
        while (next != NULL) {
            val = *(u16*)((u8*)next + 0xc);
            next = *(void**)((u8*)next + 0x10);
            fn_800E24B0(val);
            fn_800E209C(val);
        }
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
#endif
extern void fn_800E5FFC(void* ptr);
#if 0
asm u32 fn_8013D7CC(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013D7CC.inc"
}
#else
u32 fn_8013D7CC(void* ptr) {
    void* inner;
    if (ptr != NULL) {
        inner = *(void**)ptr;
        if (inner != NULL) {
            fn_800E5FFC(inner);
        }
    }
    return 1;
}
#endif
extern void* fn_800EF5FC(u32 a, u32 b, u32 size, u32 d, u32 e);
extern u32 lbl_8047AEE0;
extern u8 lbl_80466BC0[];
extern u16 lbl_8047AEE4;
extern u8 lbl_80272F00[];
#if 0
asm u32 fn_8013D804(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013D804.inc"
}
#else
u32 fn_8013D804(void* ptr) {
    void* next;
    u32 val;
    if (ptr) {
        if (lbl_8047AEE0 == 0) {
            lbl_8047AEE4 = 0;
            lbl_8047AEE0 = (u32)fn_800EF5FC(*(u16*)(lbl_80466BC0 + 4), *(u16*)(lbl_80466BC0 + 6), 0x44, 0, 0);
            if (lbl_8047AEE0 == 0) {
                if (ptr) {
                    ptr = *(void**)((u8*)ptr + 0xc);
                    if (lbl_8047AEE0 != 0) {
                        lbl_8047AEE4 -= 1;
                        if (lbl_8047AEE4 == 0) {
                            fn_800B8DF4();
                            fn_800EF5A4((void*)lbl_8047AEE0);
                            lbl_8047AEE0 = 0;
                        }
                    }
                    while (ptr != NULL) {
                        val = *(u16*)((u8*)ptr + 0xc);
                        ptr = *(void**)((u8*)ptr + 0x10);
                        fn_800E24B0(val);
                        fn_800E209C(val);
                    }
                }
                return 0;
            }
        }
        lbl_8047AEE4 = lbl_8047AEE4 + 1;
        return 1;
    }
    fn_800DD970((const char*)lbl_80272F00);
    return 0;
}
#endif
extern void fn_800E61BC(void* a, void* b, void* c, void* d, f32 e);
extern u32 lbl_8047AEE0;
extern u8 lbl_80272F38[];
#if 0
asm u32 fn_8013D908(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013D908.inc"
}
#else
u32 fn_8013D908(void* ptr) {
    void* inner;
    if (ptr) {
        *(void**)((u8*)ptr + 0x10) = *(void**)((u8*)ptr + 0xc);
        *(u32*)((u8*)ptr + 0x14) = 0;
        inner = *(void**)ptr;
        if (inner == NULL) { goto log; }
        if (lbl_8047AEE0 == 0) { goto log; }
        fn_800E61BC(inner, *(void**)((u8*)ptr + 0x4), *(void**)((u8*)ptr + 0x8),
                    (void*)lbl_8047AEE0, *(f32*)*(void**)((u8*)ptr + 0x10));
        return 1;
    }
log:
    fn_800DD970((const char*)lbl_80272F38);
    return 0;
}
#endif
extern void fn_800E3B3C(void);
extern void fn_800D4604(void);
extern void fn_800D377C(void);
extern void fn_800D3410(void);
extern void fn_800E3B08(void);
extern void fn_800E3C64(void);
extern void fn_800E3760(void);
extern void fn_800D3190(void);
extern void fn_800E5FAC(void);
extern void fn_800E60F0(void);
extern u32 lbl_8047AEE0;
extern u32 lbl_8047D260;
#if 1
asm void fn_8013D984(void) {
#include "src/game/effect/effect_visual_fn_8013D984.inc"
}
#else
void fn_8013D984(void) { /* TODO */ }
#endif
#if 0
asm void fn_8013DB64(void* ptr, u32 val, f32 f1, f32 f2) {
#include "src/game/effect/effect_visual_fn_8013DB64.inc"
}
#else
u32 fn_8013DB64(void* ptr, u32 val, f32 f1, f32 f2) {
    u16 handle;
    void* node;
    void* cur;
    handle = fn_800E3534(0x14);
    if (handle) {
        node = fn_800E27B0(handle);
        *(u16*)((u8*)node + 0xc) = handle;
        *(f32*)((u8*)node + 0x0) = f1;
        *(f32*)((u8*)node + 0x4) = f2;
        *(u32*)((u8*)node + 0x8) = val;
        cur = *(void**)((u8*)ptr + 0x14);
        if (cur != NULL) {
            while (*(void**)((u8*)cur + 0x10) != NULL) {
                cur = *(void**)((u8*)cur + 0x10);
            }
            *(void**)((u8*)cur + 0x10) = node;
        } else {
            *(void**)((u8*)ptr + 0x14) = node;
        }
        *(void**)((u8*)node + 0x10) = NULL;
    }
    return 0;
}
#endif
#if 1
asm void fn_8013DC18(void) {
#include "src/game/effect/effect_visual_fn_8013DC18.inc"
}
#else
void fn_8013DC18(void) { /* TODO */ }
#endif
#if 0
asm u32 fn_8013DC94(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013DC94.inc"
}
#else
u32 fn_8013DC94(void* ptr) {
    void* node;
    u16 val;
    if (ptr) {
        node = *(void**)((u8*)ptr + 0x14);
        if (*(void**)((u8*)ptr + 0xc) != NULL) {
            fn_800B8DF4();
            fn_800B856C();
            fn_800EF5A4(*(void**)((u8*)ptr + 0xc));
        }
        while (node) {
            val = *(u16*)((u8*)node + 0xc);
            node = *(void**)((u8*)node + 0x10);
            fn_800E24B0(val);
            fn_800E209C(val);
        }
    }
    return 1;
}
#endif
#if 0
asm u32 fn_8013DD10(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013DD10.inc"
}
#else
u32 fn_8013DD10(void* ptr) {
    if (ptr) {
        if (*(s32*)((u8*)ptr + 0x10) != 0) {
            *(void**)((u8*)ptr + 0xc) = fn_800EF5FC(0, 0, 0x44, 0, 0);
            if (*(void**)((u8*)ptr + 0xc) == NULL) {
                return 0;
            }
        }
    }
    return 1;
}
#endif
extern void fn_800DC298(void* a);
extern void fn_800E5790(void* a);
#if 0
asm u32 fn_8013DD7C(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013DD7C.inc"
}
#else
u32 fn_8013DD7C(void* ptr) {
    void* inner;
    if (ptr) {
        if (*(s32*)((u8*)ptr + 0x10) != 0) {
            fn_800DC298(*(void**)((u8*)ptr + 0xc));
        } else {
            inner = *(void**)ptr;
            if (inner) {
                fn_800E5790(inner);
            }
        }
    }
    return 1;
}
#endif
extern void fn_800DC390(void);
extern void fn_800E584C(void);
extern u8 lbl_80272F70[];
#if 1
asm void fn_8013DDCC(void) {
#include "src/game/effect/effect_visual_fn_8013DDCC.inc"
}
#else
void fn_8013DDCC(void) { /* TODO */ }
#endif
extern void fn_800EC960(void);
extern void fn_800D3068(void);
extern void fn_800EC53C(void);
extern void fn_800EC570(void);
extern void fn_800D45F8(void);
extern void fn_800DF3F0(void);
extern void fn_800DF550(void);
extern void fn_800DF188(void);
extern void fn_800EC990(void);
extern void fn_800ECA78(void);
extern void fn_800EC134(void);
extern void fn_800DF140(void);
extern void fn_800DF504(void);
extern u32 lbl_8047D288;
extern u32 lbl_8047D26C;
extern u32 lbl_8047D268;
extern u32 lbl_8047D270;
extern u32 lbl_8047D274;
extern u32 lbl_8047D278;
extern u32 lbl_8047D290;
extern u32 lbl_8047D27C;
extern u8 lbl_80363CC8[];
extern u32 lbl_8047D280;
#if 1
asm void fn_8013DE6C(void) {
#include "src/game/effect/effect_visual_fn_8013DE6C.inc"
}
#else
void fn_8013DE6C(void) { /* TODO */ }
#endif
extern u32 lbl_8047D27C;
extern u32 lbl_8047D288;
extern u32 lbl_8047D298;
extern u32 lbl_8047D29C;
extern u32 lbl_8047D2A0;
extern u8 lbl_80314AE8[];
extern u32 lbl_8047D280;
#if 1
asm void fn_8013E258(void) {
#include "src/game/effect/effect_visual_fn_8013E258.inc"
}
#else
void fn_8013E258(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013E470(void) {
#include "src/game/effect/effect_visual_fn_8013E470.inc"
}
#else
void fn_8013E470(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013E4D4(void) {
#include "src/game/effect/effect_visual_fn_8013E4D4.inc"
}
#else
void fn_8013E4D4(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013E54C(void) {
#include "src/game/effect/effect_visual_fn_8013E54C.inc"
}
#else
void fn_8013E54C(void) { /* TODO */ }
#endif
extern void fn_800EF548(void);
extern void fn_800EF504(void);
#if 1
asm void fn_8013E5AC(void) {
#include "src/game/effect/effect_visual_fn_8013E5AC.inc"
}
#else
void fn_8013E5AC(void) { /* TODO */ }
#endif
extern u8 lbl_80272FA0[];
#if 0
asm u32 fn_8013E658(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013E658.inc"
}
#else
u32 fn_8013E658(void* ptr) {
    if (ptr == NULL) { goto log; }
    *(u16*)((u8*)ptr + 0x30) = 0;
    if (*(void**)ptr == NULL) { goto log; }
    if (*(u8*)((u8*)ptr + 0x18) == 0) { goto log; }
    if (*(void**)((u8*)ptr + 0x4) == NULL) { goto log; }
    return 1;
log:
    fn_800DD970((const char*)lbl_80272FA0);
    return 0;
}
#endif
extern u32 lbl_8047D2AC;
extern u32 lbl_8047D2B0;
extern u32 lbl_8047D2B4;
extern u32 lbl_8047D2A8;
#if 1
asm void fn_8013E6C4(void) {
#include "src/game/effect/effect_visual_fn_8013E6C4.inc"
}
#else
void fn_8013E6C4(void) { /* TODO */ }
#endif
extern void fn_800CE318(void);
extern u32 lbl_8047D2B8;
extern u32 lbl_8047D2A8;
extern u32 lbl_8047D2BC;
extern u32 lbl_8047D2D8;
extern u32 lbl_8047D2C0;
extern u32 lbl_8047D2C8;
extern u32 lbl_8047D2D0;
extern u32 lbl_8047D2D4;
#if 1
asm void fn_8013E8A4(void) {
#include "src/game/effect/effect_visual_fn_8013E8A4.inc"
}
#else
void fn_8013E8A4(void) { /* TODO */ }
#endif
extern void fn_80196E10(void);
extern void fn_800E3C5C(void);
extern void fn_800E4514(void);
extern void fn_800E69C4(void);
extern void fn_800E66B8(void);
extern void fn_8019D620(void);
extern void fn_800E6804(void);
extern void fn_800E65CC(void);
extern void fn_800E68D8(void);
extern u8 lbl_80363CD8[];
extern u8 lbl_8047D2E0[];
extern u8 lbl_8047D2E8[];
extern u8 lbl_80272FD0[];
extern u8 lbl_8047D2F0[];
extern u32 lbl_8047D2D4;
extern u32 lbl_8047D2F8;
extern u32 lbl_8047D2A8;
#if 1
asm void fn_8013EA44(void) {
#include "src/game/effect/effect_visual_fn_8013EA44.inc"
}
#else
void fn_8013EA44(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013F000(void) {
#include "src/game/effect/effect_visual_fn_8013F000.inc"
}
#else
void fn_8013F000(void) { /* TODO */ }
#endif
extern void fn_800D75F4(void);
extern u32 lbl_8047AEE8;
extern u32 lbl_8047AEEC;
#if 1
asm void fn_8013F078(void) {
#include "src/game/effect/effect_visual_fn_8013F078.inc"
}
#else
void fn_8013F078(void) { /* TODO */ }
#endif
extern void fn_800D7894(void);
extern void fn_800D7868(void);
extern u32 lbl_8047AEE8;
extern u32 lbl_8047AEEC;
#if 1
asm void fn_8013F114(void) {
#include "src/game/effect/effect_visual_fn_8013F114.inc"
}
#else
void fn_8013F114(void) { /* TODO */ }
#endif
extern u32 lbl_8047AEE8;
extern u32 lbl_8047D300;
extern u8 lbl_80272FE0[];
#if 1
asm void fn_8013F344(void) {
#include "src/game/effect/effect_visual_fn_8013F344.inc"
}
#else
void fn_8013F344(void) { /* TODO */ }
#endif
extern void fn_800E3D08(void);
extern void fn_80118104(void);
extern void fn_800D848C(void);
extern void fn_800E064C(void);
extern void fn_800DC1D4(void);
extern void fn_800DBA54(void);
extern void fn_800DB9F0(void);
extern void fn_800DB988(void);
extern void fn_800DB900(void);
extern void fn_800DBCE4(void);
extern void fn_800DC224(void);
extern void fn_800DC14C(void);
extern void fn_800DC0D4(void);
extern void fn_800DC04C(void);
extern void fn_800DBFD4(void);
extern void fn_800D5C18(void);
extern void fn_800DBE5C(void);
extern u32 lbl_8047AEE8;
extern u32 lbl_8047AEF0;
extern u32 lbl_8047D304;
extern u32 lbl_8047D300;
extern u32 lbl_8047D308;
#if 1
asm void fn_8013F410(void) {
#include "src/game/effect/effect_visual_fn_8013F410.inc"
}
#else
void fn_8013F410(void) { /* TODO */ }
#endif
extern u32 lbl_8047AEE8;
extern u32 lbl_8047D310;
extern u32 lbl_8047D300;
extern u32 lbl_8047D308;
#if 1
asm void fn_8013F80C(void) {
#include "src/game/effect/effect_visual_fn_8013F80C.inc"
}
#else
void fn_8013F80C(void) { /* TODO */ }
#endif
extern void fn_800D7BF8(void);
extern void fn_800D1EB8(void);
extern void fn_800E0628(void);
extern void fn_800E0238(void);
extern void fn_800D2DE8(void);
extern void fn_800EF4FC(void);
extern void fn_800EF4F4(void);
extern void fn_800E03E8(void);
extern u32 lbl_8047D300;
extern u32 lbl_8047D304;
extern u32 lbl_8047D308;
extern u8 lbl_8027301C[];
extern u32 lbl_8047D318;
extern u32 lbl_8047D31C;
extern u32 lbl_8047D320;
extern u32 lbl_8047AEE8;
extern u32 lbl_8047D310;
extern u32 lbl_8047D324;
#if 1
asm void fn_8013F97C(void) {
#include "src/game/effect/effect_visual_fn_8013F97C.inc"
}
#else
void fn_8013F97C(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013FBE0(void) {
#include "src/game/effect/effect_visual_fn_8013FBE0.inc"
}
#else
void fn_8013FBE0(void) { /* TODO */ }
#endif
#if 1
asm void fn_8013FC58(void) {
#include "src/game/effect/effect_visual_fn_8013FC58.inc"
}
#else
void fn_8013FC58(void) { /* TODO */ }
#endif
extern void fn_801195AC(void);
#if 1
asm void fn_8013FCC4(void) {
#include "src/game/effect/effect_visual_fn_8013FCC4.inc"
}
#else
void fn_8013FCC4(void) { /* TODO */ }
#endif
extern void fn_80118874(void* a, u32 b);
extern void fn_800EC96C(void* a);
#if 0
asm u32 fn_8013FD68(void* ptr) {
#include "src/game/effect/effect_visual_fn_8013FD68.inc"
}
#else
u32 fn_8013FD68(void* ptr) {
    void* inner;
    if (ptr) {
        inner = *(void**)((u8*)ptr + 0x14);
        if (inner) {
            fn_80118874(inner, 0);
        }
        if (*(void**)((u8*)ptr + 0x10)) {
            fn_800EC96C(*(void**)((u8*)ptr + 0x10));
            fn_800EC1D4(*(void**)((u8*)ptr + 0x10));
            fn_800E4014(*(void**)((u8*)ptr + 0x10), 0);
        }
    }
    return 1;
}
#endif
extern void fn_801190DC(void);
extern void fn_800E3CC8(void);
extern void fn_800EC1BC(void);
extern void fn_800ECCA8(void);
extern void fn_800ECB74(void);
extern void fn_800EC9DC(void);
extern u32 lbl_8047D328;
extern u32 lbl_8047D32C;
extern u8 lbl_80273078[];
#if 1
asm void fn_8013FDD0(void) {
#include "src/game/effect/effect_visual_fn_8013FDD0.inc"
}
#else
void fn_8013FDD0(void) { /* TODO */ }
#endif
extern void fn_800CE220(void);
extern void fn_80118DA8(void);
extern void fn_80118F04(void);
extern void fn_800EC954(void);
extern void fn_800EC1C8(void);
extern void fn_800E43A4(void);
extern void fn_800E407C(void);
extern u8 lbl_80273060[];
extern u8 lbl_8027306C[];
extern u32 lbl_8047D334;
extern u32 lbl_8047D328;
extern u32 lbl_8047D330;
extern u32 lbl_8047D338;
extern u32 lbl_8047D32C;
#if 1
asm void fn_8013FF0C(void) {
#include "src/game/effect/effect_visual_fn_8013FF0C.inc"
}
#else
void fn_8013FF0C(void) { /* TODO */ }
#endif
#if 1
asm void fn_80140138(void) {
#include "src/game/effect/effect_visual_fn_80140138.inc"
}
#else
void fn_80140138(void) { /* TODO */ }
#endif
extern void fn_801402AC(void);
extern void fn_800DF028(void);
#if 1
asm void fn_80140190(void) {
#include "src/game/effect/effect_visual_fn_80140190.inc"
}
#else
void fn_80140190(void) { /* TODO */ }
#endif

#if 0
asm void fn_801436F0(void) {
#include "src/game/effect/effect_visual_fn_801436F0.inc"
}
#else
u32 fn_801436F0(void* ptr) {
    if (ptr == NULL) return 0;
    return !!(*(u8*)((u8*)ptr + 4) & 8);
}
#endif

#if 0
asm void fn_80143718(void) {
#include "src/game/effect/effect_visual_fn_80143718.inc"
}
#else
unsigned int fn_80143718(const void *ptr) {
    const unsigned char *p = (const unsigned char*)ptr;
    if (p == NULL) return 0;
    return (unsigned int)(unsigned char)p[0xF];
}
#endif

#if 0
asm void fn_80143730(void) {
#include "src/game/effect/effect_visual_fn_80143730.inc"
}
#else
unsigned int fn_80143730(const void *ptr) {
    const unsigned char *p = (const unsigned char*)ptr;
    if (p == NULL) return 0;
    return (unsigned int)(unsigned char)p[0xE];
}
#endif

#if 0
asm void fn_80143748(void) {
#include "src/game/effect/effect_visual_fn_80143748.inc"
}
#else
unsigned int fn_80143748(const void *ptr) {
    const unsigned char *p = (const unsigned char*)ptr;
    if (p == NULL) return 0;
    return (unsigned int)(unsigned char)p[0xD];
}
#endif

#if 0
asm void fn_80143778(void) {
#include "src/game/effect/effect_visual_fn_80143778.inc"
}
#else
unsigned int fn_80143778(const void *ptr) {
    if (ptr == NULL) return 0;
    return !!(*(u8*)((u8*)ptr + 4) & 0x10);
}
#endif

#if 0
asm void fn_801437D8(void) {
#include "src/game/effect/effect_visual_fn_801437B8.inc"
}
#else
unsigned int fn_801437D8(const void *ptr) {
    const unsigned char *p = (const unsigned char*)ptr;
    if (p == NULL) return 0;
    return ((p[4] & 0xFE) != 0);
}
#endif

#if 0
asm void fn_80143ABC(void) {
#include "src/game/effect/effect_visual_fn_80143ABC.inc"
}
#else
int fn_80143ABC(void* obj, u32 index) {
    u32 masked_index;
    if (obj == NULL) return 0;
    masked_index = (u16)index;
    if (masked_index >= 0x19) return 0;
    return (int)(s8)*((s8*)obj + masked_index + 4);
}
#endif
