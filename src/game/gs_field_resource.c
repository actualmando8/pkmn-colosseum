/**
 * @file gs_field_resource.c
 * @brief GSfield floor resource pre-load callbacks.
 *
 * Decompiled from:
 *   fn_8011432C (floorReadGFLPreFunc)
 *   fn_801143A0 (floorReadGFLPreFunc_ReturnZero)
 *   fn_801143A8 (floorReadSoundPreFunc_CheckBuffer)
 *   fn_801143EC (floorReadSoundPreFunc_Validate)
 *   fn_8011445C (floorReadSoundPreFunc_AllocBuffer)
 *   fn_801144D0 (floorReadSoundPreFunc)
 *   fn_801145C0 (floorReadParticlePreFunc_Validate)
 *   fn_80114634 (floorReadParticlePreFunc_CheckScene)
 *   fn_801146A4 (floorReadParticlePreFunc_AllocBuffer)
 *   fn_80114714 (floorReadWZXPreFunc_CheckOverlap)
 *   fn_80114760 (floorReadWZXPreFunc)
 *   fn_801147D4 (floorReadPKXPreFunc_CheckOverlap)
 *   fn_80114820 (floorReadPKXPreFunc)
 *   fn_80114878 (floorReadPKXPreFunc_AllocBuffer)
 *   fn_801148CC (floorReadTexPreFunc)
 *   fn_80114AE0 (floorReadCameraPreFunc)
 *   fn_80114CA8 (floorReadMapPreFunc)
 *
 * Each floor archive (FSYS) contains multiple resource types that need
 * to be loaded into memory before the floor becomes active. The pre-func
 * callbacks allocate memory, validate buffer sizes, and set up the
 * loading pipeline.
 *
 * Resource types handled:
 *   - GFL: Floor geometry data (ground mesh, walls)
 *   - Sound: BGM/SE wave data buffers
 *   - Particle: VFX particle system data
 *   - WZX: Walkability/collision mesh
 *   - PKX: Pokemon model data (for overworld encounters)
 *   - Texture: Shared texture packs
 *   - Camera: Pre-set camera angles/positions
 *   - Map: Minimap/area data
 *   - Script: Event script bytecode
 *   - Font: Text rendering data
 *   - Message: Localized string tables
 *   - Normal: Default/miscellaneous data
 *
 * Debug strings:
 *   "floorReadGFLPreFunc(): can't alloc %d bytes of memory"
 *   "ERROR: Over Sound Buffer! snd_res_id=%d buffer size=%d"
 *   "floorReadParticlePreFunc(): can't alloc %d bytes of memory"
 *   "floorReadWZXPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadPKXPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadTexPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadCameraPreFunc: can't alloc %d bytes of memory"
 *   "floorReadMapPreFunc: can't alloc %d bytes of memory"
 *   "floorReadScriptPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadFontPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadMsgPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadNormalPreFunc(): can't alloc %d bytes of memory"
 *
 * Address range: 0x8011432C - 0x80114CA8
 */

#include "dolphin/types.h"
#include "game/world/gs_field.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* fn_800F9418(u32 size, u32 alignment,
                          u32 loadParam, u32 loadParam2,
                          void* callback);               /* GSres alloc+load */

/* ===== String constants (rodata) ===== */
extern const char lbl_80272200[]; /* "floorReadGFLPreFunc(): can't alloc %d bytes..." */
extern const char lbl_80272238[]; /* "ERROR: Over Sound Buffer! snd_res_id=%d..." */
extern const char lbl_80272270[]; /* "floorReadParticlePreFunc(): can't alloc..." */
extern const char lbl_802722B8[]; /* "floorReadWZXPreFunc(): can't alloc..." */
extern const char lbl_802722F0[]; /* "floorReadPKXPreFunc(): can't alloc..." */
extern const char lbl_8027235C[]; /* "floorReadTexPreFunc(): can't alloc..." */
extern const char lbl_80272428[]; /* "floorReadCameraPreFunc: can't alloc..." */
extern const char lbl_802724E8[]; /* "floorReadMapPreFunc: can't alloc..." */
extern const char lbl_80272520[]; /* "floorReadScriptPreFunc(): can't alloc..." */
extern const char lbl_8027255C[]; /* "floorReadFontPreFunc(): can't alloc..." */
extern const char lbl_80272594[]; /* "floorReadMsgPreFunc(): can't alloc..." */
extern const char lbl_802725CC[]; /* "floorReadNormalPreFunc(): can't alloc..." */
extern const char lbl_802722AC[]; /* "scene_data" */

/* ===== BSS / global state ===== */
extern u32 lbl_8047B0B0;  /* sound buffer size limit */

/* ===== Internal callbacks referenced by pre-funcs ===== */
extern void fn_80115094(void);  /* GFL resource completion callback */

/* ==================================================================
 * fn_8011432C -- floorReadGFLPreFunc
 *
 * Allocate memory for a GFL floor geometry resource.
 * Rounds size up to 32-byte alignment, calls fn_800F9418 to allocate,
 * and installs fn_80115094 as the completion callback.
 *
 * If allocation fails, logs an error via fn_800DD970 with the
 * "floorReadGFLPreFunc(): can't alloc" string.
 *
 * From disassembly (0x8011432C, 0x74 bytes):
 *   - addi r0, r30, 0x1f      ; round up to 32
 *   - clrrwi r3, r0, 5        ; mask low 5 bits
 *   - li r4, 0x20             ; alignment = 32
 *   - bl fn_800F9418           ; allocate
 *   - cmplwi r31, 0x0         ; check NULL
 *   - bne .success
 *   - lis r3, lbl_80272200@ha ; error string
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadGFLPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;

    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode,
                       (void*)fn_80115094);
    if (buf == (void*)0) {
        fn_800DD970(lbl_80272200, dataSize);
    }
    return buf;
}
#pragma pop

/* ==================================================================
 * fn_801143A0 -- floorReadGFLPreFunc_ReturnZero
 *
 * Stub that returns 0 (NULL). Used as a no-op pre-func for
 * resource types that don't need pre-allocation.
 *
 * From disassembly (0x801143A0, 0x8 bytes):
 *   li r3, 0x0
 *   blr
 * ================================================================== */
void* floorReadGFLPreFunc_ReturnZero(void) {
    return (void*)0;
}

/* ==================================================================
 * fn_801143A8 -- floorReadSoundPreFunc_CheckBuffer
 *
 * Check if the requested sound buffer size exceeds the limit.
 * If so, log an error. Otherwise allocate and pass to the loader.
 *
 * From disassembly (0x801143A8, 0x44 bytes):
 *   - addi r0, r5, 0x1f      ; round up
 *   - clrrwi r0, r0, 5       ; align
 *   - lwz r5, lbl_8047B0B0   ; buffer limit
 *   - cmplw r0, r5           ; check overflow
 *   - ble .ok
 *   - lis r3, lbl_80272238@ha ; "ERROR: Over Sound Buffer!"
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadSoundPreFunc_CheckBuffer(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match -- 68 bytes at 0x801143A8 */
}
#pragma pop

/* ==================================================================
 * fn_801144D0 -- floorReadSoundPreFunc
 *
 * Full sound buffer pre-allocation function. 0xF0 bytes.
 * Validates the sound resource ID, checks buffer capacity,
 * allocates aligned memory, and sets up the sound loading state.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadSoundPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match -- 240 bytes at 0x801144D0 */
}
#pragma pop

/* ==================================================================
 * fn_80114760 -- floorReadParticlePreFunc
 *
 * Allocate memory for particle effect data within a floor archive.
 * Checks "scene_data" label for the particle system's scene context.
 *
 * From disassembly references lbl_80272270 for error logging.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadParticlePreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match -- 0x74 bytes at 0x80114760 */
}
#pragma pop

/* ==================================================================
 * fn_80114820 -- floorReadWZXPreFunc
 *
 * Allocate memory for WZX collision mesh data.
 * References lbl_802722B8 for error logging.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadWZXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}
#pragma pop

/* ==================================================================
 * fn_80114878 -- floorReadPKXPreFunc
 *
 * Allocate memory for PKX (Pokemon model) data.
 * References lbl_802722F0 for error logging.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadPKXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}
#pragma pop

/* ==================================================================
 * fn_801148CC -- floorReadTexPreFunc
 *
 * Allocate memory for shared texture data.
 * References lbl_8027235C for error logging.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadTexPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}
#pragma pop

/* ==================================================================
 * fn_80114AE0 -- floorReadCameraPreFunc
 *
 * Allocate memory for pre-set camera data.
 * References lbl_80272428 for error logging.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void* floorReadCameraPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}
#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

/* Address: 0x801143A0 | Size: 0x8 | Pattern: return_constant */
u32 fn_801143A0(void) { return 0; }

/* ===================================================================
 * Generated: 0 pattern-matched + 14 stubs
 * Range: 0x8011432C - 0x80114CA8
 * =================================================================== */

/* 0x8011432C | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011432C(void) {
    /* TODO: match -- 116 bytes at 0x8011432C */
}
#pragma pop

/* 0x801143EC | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801143EC(void) {
    /* TODO: match -- 112 bytes at 0x801143EC */
}
#pragma pop

/* 0x8011445C | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011445C(void) {
    /* TODO: match -- 116 bytes at 0x8011445C */
}
#pragma pop

/* 0x801145C0 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801145C0(void) {
    /* TODO: match -- 116 bytes at 0x801145C0 */
}
#pragma pop

/* 0x80114634 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114634(void) {
    /* TODO: match -- 112 bytes at 0x80114634 */
}
#pragma pop

/* 0x801146A4 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801146A4(void) {
    /* TODO: match -- 112 bytes at 0x801146A4 */
}
#pragma pop

/* 0x80114714 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114714(void) {
    /* TODO: match -- 76 bytes at 0x80114714 */
}
#pragma pop

/* 0x801147D4 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801147D4(void) {
    /* TODO: match -- 52 bytes at 0x801147D4 */
}
#pragma pop

/* 0x80114808 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114808(void) {
    /* TODO: match -- 116 bytes at 0x80114808 */
}
#pragma pop

/* 0x8011487C | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011487C(void) {
    /* TODO: match -- 204 bytes at 0x8011487C */
}
#pragma pop

/* 0x80114948 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114948(void) {
    /* TODO: match -- 116 bytes at 0x80114948 */
}
#pragma pop

/* 0x801149BC | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801149BC(void) {
    /* TODO: match -- 180 bytes at 0x801149BC */
}
#pragma pop

/* 0x80114A70 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114A70(void) {
    /* TODO: match -- 112 bytes at 0x80114A70 */
}
#pragma pop

/* 0x80114AE0 | 0x1C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80114AE0(void) {
    /* TODO: match -- 456 bytes at 0x80114AE0 */
}
#pragma pop
