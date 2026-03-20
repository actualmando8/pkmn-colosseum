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

extern u8 lbl_8047AD20;
extern u8 lbl_8047AD21;
extern u8 lbl_8047AD22;
extern u8 lbl_8047AD23;
extern u8 lbl_8047AD24;
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

extern u32 lbl_80404ACC;

/* Forward declarations for converted functions */
u32 fn_801046B8(void);
u8 fn_801096E8(u8 val);
u8 fn_801096F8(u8 val);
void fn_801019F8(void);
void fn_80101A70(void);
void fn_80101A9C(void);
void fn_80101AC4(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
void fn_80101B34(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
void fn_80101B90(void);
void fn_80101D8C(void);
void fn_801026A4(void);
void fn_80104160(void);
void fn_80104828(void);
void fn_80109220(void* node, u32 arg);
void fn_801096AC(void);
void fn_8010977C(void);
void* fn_80104704(s32 key);
void* fn_80105624(void);


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

/* 0x70 | fn_80101AC4 | multi_call_guarded */
void fn_80101AC4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_80101A70();
    { fn_80101A9C(); return; }
    fn_800E0C54();
}

/* 0x54 | fn_80101B34 | guarded_call */
void fn_80101B34(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (1 /* guard r0 != 0x3 */) { return; }
    fn_800BE31C();
}

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

/* 0x4C | fn_80101FB8 | two_call_arg_check */
void fn_80101FB8(u32 arg1) {
    if (arg1 != 0x1) { return; }
    fn_800B8FD8();
    fn_800BD91C();
}

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

/* 0x54 | fn_8010206C | call_sequence */
void fn_8010206C(void) {
    fn_8010977C();
    fn_801096F8(0);
    fn_801096E8(0);
    fn_801096AC();
}

/* 0x78 | fn_801020C0 | generic */
u32 fn_801020C0(void) {
    fn_8005DA18();
    fn_8005D934();
    return 0;
}

/* 0x80102138 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102138(void) {
    /* TODO: match -- 192 bytes at 0x80102138 */
}
#pragma pop

/* 0x5C | fn_801021F8 | linked_list_iterate */
void fn_801021F8(void* obj, u32 arg) {
    void* node = *(void**)((u8*)obj + 0x20);
    while (node != NULL) {
        fn_80109220(node, 0);
        node = *(void**)((u8*)node + 0x0);
    }
}

/* 0x64 | fn_80102254 | nullcheck_multi_field */
void fn_80102254(void* arg) {
    void* result = (void*)fn_80104704(0);
    if (result == NULL) { return; }
    *(u8*)((u8*)result + 0x0) = 0; /* r0 */
    *(u8*)((u8*)result + 0x0) = 0; /* r0 */
}

/* 0x801022B8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801022B8(void) {
    /* TODO: match -- 224 bytes at 0x801022B8 */
}
#pragma pop

/* 0x4C | fn_80102398 | generic_call_check_store */
void fn_80102398(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    void* result = fn_80104704(arg1);
    if (result == NULL) { return; }
    /* store to offset 0x95 */
}

/* 0x44 | fn_801023E4 | generic */
u32 fn_801023E4(void) {
    fn_80104704(0);
    return -1;
}

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

/* 0x58 | fn_80102510 | generic */
void fn_80102510(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_801046B8();
    fn_80104704(0);
    fn_80104828();
    fn_80104704(0);
}

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

/* 0x58 | fn_8010264C | call_sequence */
void fn_8010264C(void) {
    fn_801046B8();
    fn_801026A4();
}

/* 0x801026A4 | 0x1C4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801026A4(void) {
    /* TODO: match -- 452 bytes at 0x801026A4 */
}
#pragma pop

/* 0x48 | fn_80102868 | nullcheck_multi_field */
void fn_80102868(void* arg) {
    void* result = (void*)fn_80104704(0);
    if (result == NULL) { return; }
    *(u16*)((u8*)result + 0x84) = 0; /* r30 */
    *(u16*)((u8*)result + 0x86) = 0; /* r31 */
}

/* 0x64 | fn_80102ED4 | guarded_call */
void fn_80102ED4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (0 /* guard r31 == 0 */) { return; }
    fn_80105624();
}

/* 0x58 | fn_80103484 | generic */
void fn_80103484(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_8005DA18();
    fn_8005D7F8();
    fn_8005D798();
    fn_80166A28();
}

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

/* 0x80103CB0 | 16 bytes | global_getter */
u8 fn_80103CB0(void) {
    return *(u8*)((u8*)lbl_80404ACC + 0x92);
}

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

/* 0x44 | fn_80103E68 | framed_no_calls */
void fn_80103E68(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047CDE4 */
}

/* 0x48 | fn_80103EAC | framed_no_calls */
void fn_80103EAC(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047CDE0 */
}

/* 0x80 | fn_80103EF4 | generic */
void fn_80103EF4(void) {
    /* refs: lbl_80404A98 */
}

/* 0x70 | fn_80103F74 | linked_list_iterate */
void fn_80103F74(void* obj) {
    void* node = *(void**)((u8*)obj + 0x1C);
    while (node != NULL) {
        fn_80109220(node, 0);
        node = *(void**)((u8*)node + 0x0);
    }
}

/* 0x80103FE4 | 24 bytes | beq_default_getter */
u32 fn_80103FE4(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xB0);
}

/* 0x80103FFC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103FFC(void) {
    /* TODO: match -- 164 bytes at 0x80103FFC */
}
#pragma pop

/* 0x801040A0 | 24 bytes | beq_addi_ptr */
void* fn_801040A0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x9C;
}

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

/* 0x70 | fn_801040F0 | two_call_arg_check */
void fn_801040F0(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8) {
    if (arg1 == 0) { return; }
    fn_8005D858();
    fn_80104160();
}

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

/* 0x60 | fn_801044D0 | leaf_list_search */
void* fn_801044D0(s32 key) {
    void* node;
    if (key <= 0) { return NULL; }
    node = *(void**)((u8*)lbl_80404ACC + 0xC);
    while (node != NULL) {
        if (*(s32*)((u8*)node + 0x4) == key) { return node; }
        node = *(void**)((u8*)node + 0x10);
    }
    return NULL;
}

/* 0x78 | fn_80104530 | framed_no_calls */
void fn_80104530(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047E718 */
}

/* 0x801045A8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801045A8(void) {
    /* TODO: match -- 272 bytes at 0x801045A8 */
}
#pragma pop

/* 0x801046B8 | 16 bytes | global_getter */
u32 fn_801046B8(void) {
    return *(u32*)((u8*)lbl_80404ACC + 0x4);
}

/* 0x801046C8 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801046C8(void) {
    /* TODO: match -- 60 bytes at 0x801046C8 */
}
#pragma pop

/* 0x48 | fn_80104704 | leaf_list_search */
void* fn_80104704(s32 key) {
    void* node;
    if (key <= 0) { return NULL; }
    node = *(void**)((u8*)lbl_80404ACC + 0xC);
    while (node != NULL) {
        if (*(s32*)((u8*)node + 0x4) == key) { return node; }
        node = *(void**)((u8*)node + 0x10);
    }
    return NULL;
}

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

/* 0x80105624 | 16 bytes | global_addr */
void* fn_80105624(void) {
    return (u8*)lbl_80404ACC + 0x10;
}

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

/* 0x48 | fn_80105FB0 | multi_call_cond */
u32 fn_80105FB0(void) {
    { fn_80102620(); /* check */ }
    fn_80102568();
    return 268;
}

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

/* 0x7C | fn_801070F4 | call_then_multi_check */
void* fn_801070F4(void) {
    void* result = (void*)fn_80104704(0);
    /* multi-branch on result */
    return result;
}

/* 0x60 | fn_80107170 | generic */
u32 fn_80107170(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80104704(0);
    fn_801046C8();
    return 0;
}

/* 0x801071D0 | 0x304 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801071D0(void) {
    /* TODO: match -- 772 bytes at 0x801071D0 */
}
#pragma pop

/* 0x60 | fn_80107E78 | generic */
u32 fn_80107E78(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_801046C8();
    fn_8005D830();
    return 0;
}

/* 0x60 | fn_80107ED8 | generic */
u32 fn_80107ED8(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80104704(0);
    fn_8005D830();
    return 0;
}

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

/* 0x68 | fn_80108518 | two_call_arg_check */
void fn_80108518(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    if (arg1 != 0) { return; }
    memset(NULL, 0, 0);
    /* store u32 to offset 0 */
    /* store u16 to offset 0x4 */
    /* store u32 to offset 0 */
    fn_8005D830();
}

/* 0x801091F4 | 0x2C | nc_getter_s8 */
s32 fn_801091F4(void* ptr) {
    if (ptr == NULL) { return 0; }
    return (s8)*((u8*)ptr + 0x4);
}

/* 0x80109220 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109220(void* node, u32 arg) {
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

/* 0x70 | fn_80109358 | multi_call_guarded */
void fn_80109358(void) {
    { fn_800E3534(0 /* TODO */); return; }
    fn_800DD970("");
    fn_800E27B0(0 /* TODO */);
    memset(NULL, 0, 0);
}

/* 0x801093C8 | 0x29C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801093C8(void) {
    /* TODO: match -- 668 bytes at 0x801093C8 */
}
#pragma pop

/* 0x48 | fn_80109664 | guarded_call */
void fn_80109664(void) {
    if (0 /* guard r0 == 0 */) { return; }
    fn_800F0308();
}

/* 0x801096AC | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801096AC(void) {
    /* TODO: match -- 60 bytes at 0x801096AC */
}
#pragma pop

/* 0x801096E8 | 16 bytes | sda_swap */
u8 fn_801096E8(u8 val) {
    u8 old = lbl_8047AD23;
    lbl_8047AD23 = val;
    return old;
}

/* 0x801096F8 | 16 bytes | sda_swap */
u8 fn_801096F8(u8 val) {
    u8 old = lbl_8047AD22;
    lbl_8047AD22 = val;
    return old;
}

/* 0x4C | fn_80109718 | guarded_call */
void fn_80109718(void) {
    if (1 /* guard r0 != 0 */) { return; }
    fn_800F0308();
}

/* 0x80109764 | 24 bytes | multi_sda_store */
void fn_80109764(void) {
    lbl_8047AD24 = 0;
    lbl_8047AD21 = 0;
    lbl_8047AD20 = 0;
    lbl_8047AD22 = 0;
}

/* 0x8010977C | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010977C(void) {
    /* TODO: match -- 148 bytes at 0x8010977C */
}
#pragma pop

/* 0x74 | fn_80109810 | generic */
u32 fn_80109810(void) {
    /* refs: lbl_8047AD20, lbl_8047AD21, lbl_8047AD22, lbl_8047AD23, lbl_8047AD24, lbl_8047AD28, lbl_8047AD2C, lbl_8047AD30, lbl_8047AD34, lbl_8047AD38, lbl_8047AD3C, lbl_8047CE50 */
    fn_800EF5FC();
    return 0;
}

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

/* 0x6C | fn_80109B90 | guarded_call */
u32 fn_80109B90(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (1 /* guard r30 != 0 */) { return 0; }
    if (0 /* guard r0 == 0x1 */) { return 0; }
    if (0 /* guard r31 == 0 */) { return 0; }
    fn_800F0308();
    return 1;
}

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

/* 0x64 | fn_8010B560 | generic */
u32 fn_8010B560(u32 arg1) {
    /* refs: lbl_8047AD48, lbl_8047AD4C */
    return 0;
}

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
