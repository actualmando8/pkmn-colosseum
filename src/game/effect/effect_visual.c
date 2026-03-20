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
extern void* fn_800F9318(u16 group, u16 model, u16 param);

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
