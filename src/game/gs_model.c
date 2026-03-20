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

/* ===================================================================
 * Generated: 1 pattern-matched + 131 stubs
 * Range: 0x80101910 - 0x8010C220
 * =================================================================== */

/* 0x801019F8 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801019F8(void) {
    /* TODO: match -- 48 bytes at 0x801019F8 */
}
#pragma pop

/* 0x80101A70 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101A70(void) {
    /* TODO: match -- 44 bytes at 0x80101A70 */
}
#pragma pop

/* 0x80101A9C | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101A9C(void) {
    /* TODO: match -- 40 bytes at 0x80101A9C */
}
#pragma pop

/* 0x80101AC4 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101AC4(void) {
    /* TODO: match -- 112 bytes at 0x80101AC4 */
}
#pragma pop

/* 0x80101B34 | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101B34(void) {
    /* TODO: match -- 84 bytes at 0x80101B34 */
}
#pragma pop

/* 0x80101B90 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101B90(void) {
    /* TODO: match -- 460 bytes at 0x80101B90 */
}
#pragma pop

/* 0x80101D5C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101D5C(void) {
    /* TODO: match -- 48 bytes at 0x80101D5C */
}
#pragma pop

/* 0x80101D8C | 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101D8C(void) {
    /* TODO: match -- 556 bytes at 0x80101D8C */
}
#pragma pop

/* 0x80101FB8 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101FB8(void) {
    /* TODO: match -- 76 bytes at 0x80101FB8 */
}
#pragma pop

/* 0x80102014 | 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102014(void) {
    /* TODO: match -- 36 bytes at 0x80102014 */
}
#pragma pop

/* 0x80102038 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102038(void) {
    /* TODO: match -- 52 bytes at 0x80102038 */
}
#pragma pop

/* 0x8010206C | 0x54 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010206C(void) {
    /* TODO: match -- 84 bytes at 0x8010206C */
}
#pragma pop

/* 0x801020C0 | 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801020C0(void) {
    /* TODO: match -- 120 bytes at 0x801020C0 */
}
#pragma pop

/* 0x80102138 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102138(void) {
    /* TODO: match -- 192 bytes at 0x80102138 */
}
#pragma pop

/* 0x801021F8 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801021F8(void) {
    /* TODO: match -- 92 bytes at 0x801021F8 */
}
#pragma pop

/* 0x80102254 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102254(void) {
    /* TODO: match -- 100 bytes at 0x80102254 */
}
#pragma pop

/* 0x801022B8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801022B8(void) {
    /* TODO: match -- 224 bytes at 0x801022B8 */
}
#pragma pop

/* 0x80102398 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102398(void) {
    /* TODO: match -- 76 bytes at 0x80102398 */
}
#pragma pop

/* 0x801023E4 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801023E4(void) {
    /* TODO: match -- 68 bytes at 0x801023E4 */
}
#pragma pop

/* 0x80102428 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102428(void) {
    /* TODO: match -- 152 bytes at 0x80102428 */
}
#pragma pop

/* 0x801024C0 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801024C0(void) {
    /* TODO: match -- 40 bytes at 0x801024C0 */
}
#pragma pop

/* 0x801024E8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801024E8(void) {
    /* TODO: match -- 40 bytes at 0x801024E8 */
}
#pragma pop

/* 0x80102510 | 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102510(void) {
    /* TODO: match -- 88 bytes at 0x80102510 */
}
#pragma pop

/* 0x80102568 | 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102568(void) {
    /* TODO: match -- 184 bytes at 0x80102568 */
}
#pragma pop

/* 0x80102620 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102620(void) {
    /* TODO: match -- 44 bytes at 0x80102620 */
}
#pragma pop

/* 0x8010264C | 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010264C(void) {
    /* TODO: match -- 88 bytes at 0x8010264C */
}
#pragma pop

/* 0x801026A4 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801026A4(void) {
    /* TODO: match -- 452 bytes at 0x801026A4 */
}
#pragma pop

/* 0x80102868 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102868(void) {
    /* TODO: match -- 72 bytes at 0x80102868 */
}
#pragma pop

/* 0x80102ED4 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102ED4(void) {
    /* TODO: match -- 100 bytes at 0x80102ED4 */
}
#pragma pop

/* 0x80103484 | 0x58 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103484(void) {
    /* TODO: match -- 88 bytes at 0x80103484 */
}
#pragma pop

/* 0x801034DC | 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801034DC(void) {
    /* TODO: match -- 312 bytes at 0x801034DC */
}
#pragma pop

/* 0x80103614 | 0x2E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103614(void) {
    /* TODO: match -- 740 bytes at 0x80103614 */
}
#pragma pop

/* 0x801038F8 | 0x2B0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801038F8(void) {
    /* TODO: match -- 688 bytes at 0x801038F8 */
}
#pragma pop

/* 0x80103BA8 | 0x108 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103BA8(void) {
    /* TODO: match -- 264 bytes at 0x80103BA8 */
}
#pragma pop

/* 0x80103CB0 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103CB0(void) {
    /* TODO: match -- 16 bytes at 0x80103CB0 */
}
#pragma pop

/* 0x80103CC0 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103CC0(void) {
    /* TODO: match -- 24 bytes at 0x80103CC0 */
}
#pragma pop

/* 0x80103CD8 | 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103CD8(void) {
    /* TODO: match -- 400 bytes at 0x80103CD8 */
}
#pragma pop

/* 0x80103E68 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103E68(void) {
    /* TODO: match -- 68 bytes at 0x80103E68 */
}
#pragma pop

/* 0x80103EAC | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103EAC(void) {
    /* TODO: match -- 72 bytes at 0x80103EAC */
}
#pragma pop

/* 0x80103EF4 | 0x80 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103EF4(void) {
    /* TODO: match -- 128 bytes at 0x80103EF4 */
}
#pragma pop

/* 0x80103F74 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103F74(void) {
    /* TODO: match -- 112 bytes at 0x80103F74 */
}
#pragma pop

/* 0x80103FE4 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103FE4(void) {
    /* TODO: match -- 24 bytes at 0x80103FE4 */
}
#pragma pop

/* 0x80103FFC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103FFC(void) {
    /* TODO: match -- 164 bytes at 0x80103FFC */
}
#pragma pop

/* 0x801040A0 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801040A0(void) {
    /* TODO: match -- 24 bytes at 0x801040A0 */
}
#pragma pop

/* 0x801040B8 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801040B8(void) {
    /* TODO: match -- 24 bytes at 0x801040B8 */
}
#pragma pop

/* 0x801040D0 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801040D0(void) {
    /* TODO: match -- 32 bytes at 0x801040D0 */
}
#pragma pop

/* 0x801040F0 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801040F0(void) {
    /* TODO: match -- 112 bytes at 0x801040F0 */
}
#pragma pop

/* 0x80104160 | 0x1B8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104160(void) {
    /* TODO: match -- 440 bytes at 0x80104160 */
}
#pragma pop

/* 0x80104318 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104318(void) {
    /* TODO: match -- 140 bytes at 0x80104318 */
}
#pragma pop

/* 0x801043A4 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801043A4(void) {
    /* TODO: match -- 300 bytes at 0x801043A4 */
}
#pragma pop

/* 0x801044D0 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801044D0(void) {
    /* TODO: match -- 96 bytes at 0x801044D0 */
}
#pragma pop

/* 0x80104530 | 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104530(void) {
    /* TODO: match -- 120 bytes at 0x80104530 */
}
#pragma pop

/* 0x801045A8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801045A8(void) {
    /* TODO: match -- 272 bytes at 0x801045A8 */
}
#pragma pop

/* 0x801046B8 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801046B8(void) {
    /* TODO: match -- 16 bytes at 0x801046B8 */
}
#pragma pop

/* 0x801046C8 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801046C8(void) {
    /* TODO: match -- 60 bytes at 0x801046C8 */
}
#pragma pop

/* 0x80104704 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104704(void) {
    /* TODO: match -- 72 bytes at 0x80104704 */
}
#pragma pop

/* 0x8010474C | 0xDC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010474C(void) {
    /* TODO: match -- 220 bytes at 0x8010474C */
}
#pragma pop

/* 0x80104828 | 0x26C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104828(void) {
    /* TODO: match -- 620 bytes at 0x80104828 */
}
#pragma pop

/* 0x80104A94 | 0x20C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104A94(void) {
    /* TODO: match -- 524 bytes at 0x80104A94 */
}
#pragma pop

/* 0x80104CA0 | 0x1E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104CA0(void) {
    /* TODO: match -- 480 bytes at 0x80104CA0 */
}
#pragma pop

/* 0x80104E80 | 0x474 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104E80(void) {
    /* TODO: match -- 1140 bytes at 0x80104E80 */
}
#pragma pop

/* 0x801052F4 | 0x11C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801052F4(void) {
    /* TODO: match -- 284 bytes at 0x801052F4 */
}
#pragma pop

/* 0x80105410 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105410(void) {
    /* TODO: match -- 168 bytes at 0x80105410 */
}
#pragma pop

/* 0x801054B8 | 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801054B8(void) {
    /* TODO: match -- 364 bytes at 0x801054B8 */
}
#pragma pop

/* 0x80105624 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105624(void) {
    /* TODO: match -- 16 bytes at 0x80105624 */
}
#pragma pop

/* 0x80105634 | 0x298 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105634(void) {
    /* TODO: match -- 664 bytes at 0x80105634 */
}
#pragma pop

/* 0x801058CC | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801058CC(void) {
    /* TODO: match -- 368 bytes at 0x801058CC */
}
#pragma pop

/* 0x80105A3C | 0x1F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105A3C(void) {
    /* TODO: match -- 500 bytes at 0x80105A3C */
}
#pragma pop

/* 0x80105C30 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105C30(void) {
    /* TODO: match -- 56 bytes at 0x80105C30 */
}
#pragma pop

/* 0x80105C68 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105C68(void) {
    /* TODO: match -- 224 bytes at 0x80105C68 */
}
#pragma pop

/* 0x80105D48 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105D48(void) {
    /* TODO: match -- 308 bytes at 0x80105D48 */
}
#pragma pop

/* 0x80105E7C | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105E7C(void) {
    /* TODO: match -- 308 bytes at 0x80105E7C */
}
#pragma pop

/* 0x80105FB0 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105FB0(void) {
    /* TODO: match -- 72 bytes at 0x80105FB0 */
}
#pragma pop

/* 0x80105FF8 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105FF8(void) {
    /* TODO: match -- 136 bytes at 0x80105FF8 */
}
#pragma pop

/* 0x80106080 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106080(void) {
    /* TODO: match -- 224 bytes at 0x80106080 */
}
#pragma pop

/* 0x80106160 | 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106160(void) {
    /* TODO: match -- 228 bytes at 0x80106160 */
}
#pragma pop

/* 0x80106244 | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106244(void) {
    /* TODO: match -- 336 bytes at 0x80106244 */
}
#pragma pop

/* 0x80106394 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106394(void) {
    /* TODO: match -- 332 bytes at 0x80106394 */
}
#pragma pop

/* 0x801064E0 | 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801064E0(void) {
    /* TODO: match -- 216 bytes at 0x801064E0 */
}
#pragma pop

/* 0x801065B8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801065B8(void) {
    /* TODO: match -- 224 bytes at 0x801065B8 */
}
#pragma pop

/* 0x80106698 | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106698(void) {
    /* TODO: match -- 336 bytes at 0x80106698 */
}
#pragma pop

/* 0x801067E8 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801067E8(void) {
    /* TODO: match -- 332 bytes at 0x801067E8 */
}
#pragma pop

/* 0x80106934 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106934(void) {
    /* TODO: match -- 200 bytes at 0x80106934 */
}
#pragma pop

/* 0x801069FC | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801069FC(void) {
    /* TODO: match -- 224 bytes at 0x801069FC */
}
#pragma pop

/* 0x80106ADC | 0x260 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106ADC(void) {
    /* TODO: match -- 608 bytes at 0x80106ADC */
}
#pragma pop

/* 0x80106D3C | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106D3C(void) {
    /* TODO: match -- 604 bytes at 0x80106D3C */
}
#pragma pop

/* 0x80106F98 | 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106F98(void) {
    /* TODO: match -- 348 bytes at 0x80106F98 */
}
#pragma pop

/* 0x801070F4 | 0x7C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801070F4(void) {
    /* TODO: match -- 124 bytes at 0x801070F4 */
}
#pragma pop

/* 0x80107170 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80107170(void) {
    /* TODO: match -- 96 bytes at 0x80107170 */
}
#pragma pop

/* 0x801071D0 | 0x304 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801071D0(void) {
    /* TODO: match -- 772 bytes at 0x801071D0 */
}
#pragma pop

/* 0x80107E78 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80107E78(void) {
    /* TODO: match -- 96 bytes at 0x80107E78 */
}
#pragma pop

/* 0x80107ED8 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80107ED8(void) {
    /* TODO: match -- 96 bytes at 0x80107ED8 */
}
#pragma pop

/* 0x80107F38 | 0x194 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80107F38(void) {
    /* TODO: match -- 404 bytes at 0x80107F38 */
}
#pragma pop

/* 0x801080CC | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801080CC(void) {
    /* TODO: match -- 300 bytes at 0x801080CC */
}
#pragma pop

/* 0x801081F8 | 0x320 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801081F8(void) {
    /* TODO: match -- 800 bytes at 0x801081F8 */
}
#pragma pop

/* 0x80108518 | 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80108518(void) {
    /* TODO: match -- 104 bytes at 0x80108518 */
}
#pragma pop

/* 0x801091F4 | 0x2C | nc_getter_s8 */
s32 fn_801091F4(void* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)*((u8*)ptr + 0x4);
}

/* 0x80109220 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109220(void) {
    /* TODO: match -- 60 bytes at 0x80109220 */
}
#pragma pop

/* 0x8010925C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010925C(void) {
    /* TODO: match -- 52 bytes at 0x8010925C */
}
#pragma pop

/* 0x80109290 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109290(void) {
    /* TODO: match -- 200 bytes at 0x80109290 */
}
#pragma pop

/* 0x80109358 | 0x70 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109358(void) {
    /* TODO: match -- 112 bytes at 0x80109358 */
}
#pragma pop

/* 0x801093C8 | 0x29C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801093C8(void) {
    /* TODO: match -- 668 bytes at 0x801093C8 */
}
#pragma pop

/* 0x80109664 | 0x48 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109664(void) {
    /* TODO: match -- 72 bytes at 0x80109664 */
}
#pragma pop

/* 0x801096AC | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801096AC(void) {
    /* TODO: match -- 60 bytes at 0x801096AC */
}
#pragma pop

/* 0x801096E8 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801096E8(void) {
    /* TODO: match -- 16 bytes at 0x801096E8 */
}
#pragma pop

/* 0x801096F8 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801096F8(void) {
    /* TODO: match -- 16 bytes at 0x801096F8 */
}
#pragma pop

/* 0x80109718 | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109718(void) {
    /* TODO: match -- 76 bytes at 0x80109718 */
}
#pragma pop

/* 0x80109764 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109764(void) {
    /* TODO: match -- 24 bytes at 0x80109764 */
}
#pragma pop

/* 0x8010977C | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010977C(void) {
    /* TODO: match -- 148 bytes at 0x8010977C */
}
#pragma pop

/* 0x80109810 | 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109810(void) {
    /* TODO: match -- 116 bytes at 0x80109810 */
}
#pragma pop

/* 0x80109884 | 0x10 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109884(void) {
    /* TODO: match -- 16 bytes at 0x80109884 */
}
#pragma pop

/* 0x80109894 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109894(void) {
    /* TODO: match -- 160 bytes at 0x80109894 */
}
#pragma pop

/* 0x80109934 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109934(void) {
    /* TODO: match -- 604 bytes at 0x80109934 */
}
#pragma pop

/* 0x80109B90 | 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109B90(void) {
    /* TODO: match -- 108 bytes at 0x80109B90 */
}
#pragma pop

/* 0x80109BFC | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109BFC(void) {
    /* TODO: match -- 140 bytes at 0x80109BFC */
}
#pragma pop

/* 0x80109C88 | 0x388 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109C88(void) {
    /* TODO: match -- 904 bytes at 0x80109C88 */
}
#pragma pop

/* 0x8010A010 | 0x200 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A010(void) {
    /* TODO: match -- 512 bytes at 0x8010A010 */
}
#pragma pop

/* 0x8010A210 | 0x210 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A210(void) {
    /* TODO: match -- 528 bytes at 0x8010A210 */
}
#pragma pop

/* 0x8010A420 | 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A420(void) {
    /* TODO: match -- 412 bytes at 0x8010A420 */
}
#pragma pop

/* 0x8010A5BC | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A5BC(void) {
    /* TODO: match -- 720 bytes at 0x8010A5BC */
}
#pragma pop

/* 0x8010A88C | 0x274 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A88C(void) {
    /* TODO: match -- 628 bytes at 0x8010A88C */
}
#pragma pop

/* 0x8010AB00 | 0x32C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010AB00(void) {
    /* TODO: match -- 812 bytes at 0x8010AB00 */
}
#pragma pop

/* 0x8010AE2C | 0x1F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010AE2C(void) {
    /* TODO: match -- 496 bytes at 0x8010AE2C */
}
#pragma pop

/* 0x8010B01C | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B01C(void) {
    /* TODO: match -- 336 bytes at 0x8010B01C */
}
#pragma pop

/* 0x8010B16C | 0x3F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B16C(void) {
    /* TODO: match -- 1012 bytes at 0x8010B16C */
}
#pragma pop

/* 0x8010B560 | 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B560(void) {
    /* TODO: match -- 100 bytes at 0x8010B560 */
}
#pragma pop

/* 0x8010B5C4 | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B5C4(void) {
    /* TODO: match -- 340 bytes at 0x8010B5C4 */
}
#pragma pop

/* 0x8010B718 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B718(void) {
    /* TODO: match -- 720 bytes at 0x8010B718 */
}
#pragma pop

/* 0x8010B9E8 | 0x1D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B9E8(void) {
    /* TODO: match -- 464 bytes at 0x8010B9E8 */
}
#pragma pop

/* 0x8010BBB8 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BBB8(void) {
    /* TODO: match -- 300 bytes at 0x8010BBB8 */
}
#pragma pop

/* 0x8010BCE4 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BCE4(void) {
    /* TODO: match -- 136 bytes at 0x8010BCE4 */
}
#pragma pop

/* 0x8010BD6C | 0x4B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BD6C(void) {
    /* TODO: match -- 1204 bytes at 0x8010BD6C */
}
#pragma pop
