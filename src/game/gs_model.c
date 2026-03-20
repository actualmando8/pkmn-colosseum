/**
 * @file gs_model.c
 * @brief GSmodel -- 3D model management system.
 *
 * This module sits between GSfloor (0x80101910) and GScolsys (0x8010C220)
 * in the link order, managing 3D model loading, animation, and resource
 * lifecycle for the Genius Sonority engine.
 *
 * Decompiled from 145 functions in range 0x80101910 - 0x8010C220.
 *
 * Selected functions:
 *   fn_80101910 (GSmodel_FindLoadedResource)
 *   fn_801019F8 (GSmodel_ClearResourceTable)
 *   fn_80101A28 (GSmodel_GetResourceCount)
 *   fn_80101A4C (GSmodel_GetResourceByIndex)
 *   fn_80101A70 (GSmodel_GetResourceHandle)
 *   fn_80101A9C (GSmodel_SetResourceActive)
 *   fn_80101AC4 (GSmodel_AllocModelSlot)
 *   fn_80101B34 (GSmodel_FreeModelSlot)
 *   fn_80101B90 (GSmodel_LoadFromFSYS)
 *   fn_80101D8C (GSmodel_SetupJoints)
 *   fn_80102004 (GSmodel_GetJointCount)
 *   fn_801028B0 (GSmodel_AttachAnimation)
 *   fn_80102F38 (GSmodel_UpdateAnimation)
 *   fn_801074D4 (GSmodel_RenderModel)
 *   fn_80108580 (GSmodel_BuildDisplayList)
 *   fn_80108C14 (GSmodel_ProcessSkinning)
 *
 * Architecture:
 *   - Model data is stored in a BSS table at lbl_80402518 (0x2400 bytes)
 *   - Each model slot is 0x48 bytes (table has 0x20 * 3 = 0x60 slots)
 *   - Models reference GSmem handles for their data
 *   - Animation is driven by per-frame update calls
 *   - Joint hierarchy is used for skeletal animation
 *   - Display list caching for static geometry
 *
 * Code patterns observed:
 *   - fn_80101910: Unrolled loop searching 4 slots per iteration
 *     (stride 0x48, checks offset 0x44 for ref count, offset 0x40
 *      for resource ID match). Uses bdnz with ctr=0x20.
 *   - fn_801019F8: memset(lbl_80402518, 0, 0x2400) to clear table
 *   - Many functions access lbl_8047AA80 (GSgfx state pointer)
 *     indicating tight integration with the renderer
 *   - Float operations (fmr, fadds, fmuls) for animation interpolation
 *
 * No debug strings reference this range directly -- the module operates
 * silently, using asserts only through GSgfx/GSmem error paths.
 *
 * Address range: 0x80101910 - 0x8010C220 (44KB, 145 functions)
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 n);
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */

/* GSmem */
extern u16   fn_800E3534(u32 size);                     /* GSmemAllocRaw */
extern void* fn_800E27B0(u16 handle);                   /* GSmemGetPtr */
extern void* fn_800E24B0(u16 handle);                   /* GSmemLock */
extern void  fn_800E209C(u16 handle);                   /* GSmemFree */

/* GSgfx state */
extern u8 lbl_8047AA80[];  /* GSgfx state pointer (via sda21) */

/* Matrix math */
extern void  fn_800A2D38(void);                         /* MTXIdentity */
extern void  fn_800A2D64(void* mtxA, void* mtxB);      /* MTXConcat */
extern void  fn_800A37CC(void* mtx, void* vec, void* out); /* MTXMultVec3 */

/* Model resource table (BSS) */
extern u8 lbl_80402518[];  /* model resource table -- 0x2400 bytes */

/* ===================================================================
 * Constants
 * =================================================================== */

/** Size of the model resource table in bytes */
#define GSMODEL_TABLE_SIZE      0x2400

/** Size of one model resource slot */
#define GSMODEL_SLOT_SIZE       0x48

/** Maximum number of model slots (0x2400 / 0x48 = ~0x80, but used as 0x60) */
#define GSMODEL_MAX_SLOTS       0x60

/** Number of slots checked per unrolled iteration */
#define GSMODEL_UNROLL_COUNT    3

/* ===================================================================
 * Model resource slot structure
 * =================================================================== */

/**
 * Each model resource occupies a 0x48-byte slot in the table.
 * Inferred from disassembly access patterns.
 */
typedef struct GSModelSlot {
    /* 0x00 */ u32  flags;          /**< slot state flags */
    /* 0x04 */ u16  memHandle;      /**< GSmem handle for model data */
    /* 0x06 */ u16  animHandle;     /**< GSmem handle for animation data */
    /* 0x08 */ void* dataPtr;       /**< resolved pointer to model data */
    /* 0x0C */ void* animPtr;       /**< resolved pointer to animation */
    /* 0x10 */ u32  jointCount;     /**< number of joints in skeleton */
    /* 0x14 */ void* jointTable;    /**< pointer to joint hierarchy */
    /* 0x18 */ f32  animFrame;      /**< current animation frame */
    /* 0x1C */ f32  animSpeed;      /**< playback speed multiplier */
    /* 0x20 */ f32  animEnd;        /**< animation end frame */
    /* 0x24 */ u32  animFlags;      /**< loop, pingpong, etc. */
    /* 0x28 */ void* displayList;   /**< cached GX display list */
    /* 0x2C */ u32  dlSize;         /**< display list size */
    /* 0x30 */ void* skinWeights;   /**< vertex skinning weight table */
    /* 0x34 */ u32  vertexCount;    /**< number of vertices */
    /* 0x38 */ void* materialPtr;   /**< pointer to material data */
    /* 0x3C */ void* texturePtr;    /**< pointer to texture data */
    /* 0x40 */ u32  resourceId;     /**< resource ID for lookup */
    /* 0x44 */ u32  refCount;       /**< reference count (0 = free) */
} GSModelSlot;

/* ==================================================================
 * fn_80101910 -- GSmodel_FindLoadedResource
 *
 * Search the model resource table for a loaded resource matching
 * the resource ID in r3->offset_0x40. Uses an unrolled loop
 * checking 3 consecutive slots per iteration (via bdnz, ctr=0x20).
 *
 * If found, decrements the reference count and returns 1.
 * If not found after 0x60 slots, returns 1 (all searched).
 *
 * From disassembly (0x80101910, 0xE8 bytes):
 *   lis r4, lbl_80402518@ha
 *   li r0, 0x20              ; 32 iterations
 *   addi r4, r4, lbl_80402518@l
 *   mtctr r0
 *   ; ... unrolled: check slot, slot+0x48, slot+0x90, advance by 0xD8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSmodel_FindLoadedResource(void* query) {
    /* TODO: match -- 232 bytes at 0x80101910 */
}
#pragma pop

/* ==================================================================
 * fn_801019F8 -- GSmodel_ClearResourceTable
 *
 * Clear the entire model resource table by zeroing 0x2400 bytes.
 *
 * From disassembly (0x801019F8, 0x30 bytes):
 *   lis r3, lbl_80402518@ha
 *   li r4, 0x0
 *   addi r3, r3, lbl_80402518@l
 *   li r5, 0x2400
 *   bl memset
 * ================================================================== */
void GSmodel_ClearResourceTable(void) {
    memset(lbl_80402518, 0, GSMODEL_TABLE_SIZE);
}

/* ==================================================================
 * fn_801028B0 -- GSmodel_AttachAnimation
 *
 * Attach an animation resource to a loaded model. Sets up the
 * joint hierarchy traversal and initializes playback state.
 * 1572 bytes -- one of the larger functions.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmodel_AttachAnimation(void* model, void* animData, f32 startFrame) {
    /* TODO: match -- 1572 bytes at 0x801028B0 */
}
#pragma pop

/* ==================================================================
 * fn_80102F38 -- GSmodel_UpdateAnimation
 *
 * Advance animation playback by one frame. Handles looping, speed
 * scaling, and blend transitions. 1356 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmodel_UpdateAnimation(void* model) {
    /* TODO: match -- 1356 bytes at 0x80102F38 */
}
#pragma pop

/* ==================================================================
 * fn_801074D4 -- GSmodel_RenderModel
 *
 * Render a model using its display list and current transform.
 * Largest function in this module at 2468 bytes. Sets up GX state,
 * binds textures and materials, then executes the display list.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmodel_RenderModel(void* model) {
    /* TODO: match -- 2468 bytes at 0x801074D4 */
}
#pragma pop

/* ==================================================================
 * fn_80108580 -- GSmodel_BuildDisplayList
 *
 * Build a GX display list for a model's static geometry. The
 * resulting display list is cached in the model slot for fast
 * re-rendering. 1684 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmodel_BuildDisplayList(void* model) {
    /* TODO: match -- 1684 bytes at 0x80108580 */
}
#pragma pop

/* ==================================================================
 * fn_80108C14 -- GSmodel_ProcessSkinning
 *
 * Compute skinned vertex positions from the joint hierarchy and
 * vertex weights. 1504 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSmodel_ProcessSkinning(void* model) {
    /* TODO: match -- 1504 bytes at 0x80108C14 */
}
#pragma pop

/* ===== Small accessor / utility functions ===== */

/* fn_80101A28 */
u32 GSmodel_GetResourceCount(void) {
    /* TODO: match -- 36 bytes at 0x80101A28 */
    return 0;
}

/* fn_80101A4C */
void* GSmodel_GetResourceByIndex(u32 index) {
    /* TODO: match -- 36 bytes at 0x80101A4C */
    return (void*)0;
}

/* fn_80102004 */
u32 GSmodel_GetJointCount(void* model) {
    /* TODO: match -- 16 bytes at 0x80102004 */
    return 0;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

extern u8 lbl_8047AD21;
extern u32 lbl_8047AD28;

/* Address: 0x80109708 | Size: 0x8 | Pattern: sda_getter */
u8 fn_80109708(void) {
    return lbl_8047AD21;
}

/* Address: 0x80109710 | Size: 0x8 | Pattern: sda_getter */
u32 fn_80109710(void) {
    return lbl_8047AD28;
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

extern u32 lbl_8047ACF0;

/* Address: 0x80101B88 | Size: 0x8 | Pattern: sda_setter */
void fn_80101B88(u32 val) {
    lbl_8047ACF0 = val;
}
