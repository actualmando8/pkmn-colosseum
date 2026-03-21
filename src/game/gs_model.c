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

/* Additional external references */
extern u32  lbl_80478B20;            /* resource entry count (SDA) */
extern u8   lbl_80315690[];          /* resource info table (data) */
extern u32  lbl_8047ACF4;            /* callback function pointer (SDA) */
extern u32  lbl_8047ACF8;            /* callback result store (SDA) */
extern u16  fn_800E0C54(void);       /* GSrandom */
extern u32  fn_800BE31C(void);       /* time/tick get */
extern void fn_800D2738(void);       /* GSgfx sync A */
extern void fn_800E4BF4(void);       /* GSgfx sync B */
extern u32  OSGetTick(void);         /* dolphin OS get tick */
extern void fn_800D9ED8(u32 arg);    /* GSgfx mode set */
extern void fn_800D88DC(u32 arg);    /* GSgfx config A */
extern void fn_800D888C(u32 arg);    /* GSgfx config B */
extern void fn_800D9B58(void);       /* GSgfx render param */
extern void fn_800F0308(void);       /* GSthread context switch */
extern void fn_800EF5FC(void);       /* GSmem defrag/gc */
extern void fn_8005DA18(void);       /* HSD_PadFlushQueue */
extern void fn_8005D934(void);       /* HSD_PadRead */
extern void fn_8005D858(void);       /* HSD_PadReset */
extern void fn_8005D7F8(void);       /* HSD_PadInit */
extern void fn_8005D798(void);       /* HSD_PadGetRawTrigger */
extern void fn_8005D830(void);       /* HSD_PadGetStatus */
extern void fn_80166A28(void);       /* sound system update */
extern void fn_800B8FD8(void);       /* VI configure */
extern void fn_800BD91C(void);       /* VI setup */

/* Forward declarations for functions defined later in this TU */
void fn_80109664(void);
void fn_80109718(void);
void fn_80109764(void);

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
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r4 = (u32)&lbl_80402518;
    r0 = 0x20;
    r4 = (u32)&lbl_80402518;
    r7 = 0x0;
    r6 = r4;
    ctr_fn = (void(*)(void))r0;
L_80101928: ;
    r5 = *(u32*)((u8*)r6 + 0x44);
    if ((s32)r5 == (s32)0x0) goto L_80101954;
    r4 = *(u32*)((u8*)r6 + 0x40);
    r0 = *(u32*)((u8*)r3 + 0x40);
    if ((u32)r4 != (u32)r0) goto L_80101954;
    /* subi r0, r5, 0x1 */;
    r3 = 0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    return;
L_80101954: ;
    r5 = *(u32*)((u8*)r6 + 0x8C);
    r6 = r6 + 0x48;
    if ((s32)r5 == (s32)0x0) goto L_80101984;
    r4 = *(u32*)((u8*)r6 + 0x40);
    r0 = *(u32*)((u8*)r3 + 0x40);
    if ((u32)r4 != (u32)r0) goto L_80101984;
    /* subi r0, r5, 0x1 */;
    r3 = 0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    return;
L_80101984: ;
    r5 = *(u32*)((u8*)r6 + 0x8C);
    r6 = r6 + 0x48;
    if ((s32)r5 == (s32)0x0) goto L_801019B4;
    r4 = *(u32*)((u8*)r6 + 0x40);
    r0 = *(u32*)((u8*)r3 + 0x40);
    if ((u32)r4 != (u32)r0) goto L_801019B4;
    /* subi r0, r5, 0x1 */;
    r3 = 0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    return;
L_801019B4: ;
    r5 = *(u32*)((u8*)r6 + 0x8C);
    r6 = r6 + 0x48;
    if ((s32)r5 == (s32)0x0) goto L_801019E4;
    r4 = *(u32*)((u8*)r6 + 0x40);
    r0 = *(u32*)((u8*)r3 + 0x40);
    if ((u32)r4 != (u32)r0) goto L_801019E4;
    /* subi r0, r5, 0x1 */;
    r3 = 0x1;
    *(u32*)((u8*)r6 + 0x44) = r0;
    return;
L_801019E4: ;
    r7 = r7 + 0x3;
    r6 = r6 + 0x48;
    if (--ctr != 0) goto L_80101928;
    r3 = 0x1;
    return;
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
    extern void fn_800D3088();
    extern void fn_800D9E4C();
    extern void fn_800DA4C4();
    extern void fn_800F7434();
    extern void fn_800FE35C();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_80102F38();
    extern void fn_801038F8();
    extern void fn_801046B8();
    extern void fn_80104704();
    extern void fn_8010474C();
    extern void fn_80104CA0();
    extern void fn_80105624();
    extern void fn_80106F98();
    extern void fn_801070F4();
    extern void fn_801071D0();
    extern void fn_80108C14();
    extern void fn_801093C8();
    extern void fn_801C43F4();
    extern void fn_801D2404();
    extern u8 lbl_80404ACC;
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r26, 0x18(r1) */;
    r27 = 0x1;
    fn_801D2404();
    fn_801046B8();
    fn_80104704();
    r4 = (u32)&lbl_80404ACC;
    r29 = r3;
    r3 = (u32)&lbl_80404ACC;
    fn_801038F8();
    r3 = 0x0;
    fn_800D9E4C();
    fn_801093C8();
    r3 = 0x1;
    ((void(*)(void))fn_800D9ED8)();
    r3 = (u32)&lbl_80404ACC;
    r30 = (u32)&lbl_80404ACC;
    r28 = *(u32*)((u8*)r30 + 0xC);
    goto L_80102D84;
L_80102904: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r28 + 0x1);
    r31 = r3;
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x2) goto L_80102998;
    if ((s32)r0 >= (s32)0x2) goto L_80102934;
    if ((s32)r0 == (s32)0x0) goto L_80102944;
    if ((s32)r0 >= (s32)0x0) goto L_80102958;
    goto L_80102A10;
L_80102934: ;
    if ((s32)r0 == (s32)0x4) goto L_80102A10;
    if ((s32)r0 >= (s32)0x4) goto L_80102A10;
    goto L_801029EC;
L_80102944: ;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x1) = r3;
    *(u8*)((u8*)r28 + 0x2) = r0;
    goto L_80102A10;
L_80102958: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_801070F4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80102A10;
    r0 = *(u8*)((u8*)r31 + 0x1);
    /* extrwi r0, r0, 3, 27 */;
    if ((u32)r0 != (u32)0x0) goto L_80102984;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x98) = r0;
L_80102984: ;
    r3 = 0x2;
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x1) = r3;
    *(u8*)((u8*)r28 + 0x2) = r0;
    goto L_80102A10;
L_80102998: ;
    r0 = *(u8*)((u8*)r28 + 0xA);
    if ((u32)r0 == (u32)0x0) goto L_80102A10;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    r3 = r0 & 0x7;
    ((void(*)(void))fn_8005D7F8)();
    if ((u32)r3 == (u32)0x0) goto L_801029D8;
    r4 = 0x5;
    ((void(*)(void))fn_8005D798)();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_801029D8;
    ((void(*)(void))fn_80166A28)();
L_801029D8: ;
    r3 = 0x3;
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x1) = r3;
    *(u8*)((u8*)r28 + 0x2) = r0;
    goto L_80102A10;
L_801029EC: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_801070F4();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80102A10;
    r3 = 0x4;
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x1) = r3;
    *(u8*)((u8*)r28 + 0x2) = r0;
L_80102A10: ;
    if ((u32)r29 == (u32)0x0) goto L_80102D30;
    if ((u32)r28 != (u32)r29) goto L_80102D30;
    r0 = *(u8*)((u8*)r28 + 0x98);
    if ((u32)r0 != (u32)0x0) goto L_80102C0C;
    r0 = *(u8*)((u8*)r28 + 0x1);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x3) goto L_80102B74;
    if ((s32)r0 >= (s32)0x3) goto L_80102B74;
    if ((s32)r0 >= (s32)0x2) goto L_80102A4C;
    goto L_80102B74;
L_80102A4C: ;
    r0 = *(u8*)((u8*)r31 + 0x1);
    /* extrwi r0, r0, 3, 27 */;
    if ((s32)r0 == (s32)0x2) goto L_80102AF0;
    if ((s32)r0 >= (s32)0x2) goto L_80102A70;
    if ((s32)r0 == (s32)0x0) goto L_80102A80;
    if ((s32)r0 >= (s32)0x0) goto L_80102A8C;
    goto L_80102AF0;
L_80102A70: ;
    if ((s32)r0 == (s32)0x4) goto L_80102AE0;
    if ((s32)r0 >= (s32)0x4) goto L_80102AF0;
    goto L_80102ACC;
L_80102A80: ;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x98) = r0;
    goto L_80102AF0;
L_80102A8C: ;
    if ((u32)r28 == (u32)0x0) goto L_80102AF0;
    fn_80105624();
    r3 = *(u16*)((u8*)r3 + 0x4);
    r0 = r3 & 0x00000010;
    if ((s32)r0 == (s32)0x0) goto L_80102AB0;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x98) = r0;
L_80102AB0: ;
    r0 = r3 & 0x00000020;
    if ((s32)r0 == (s32)0x0) goto L_80102AF0;
    r0 = 0x1;
    *(u8*)((u8*)r28 + 0x98) = r0;
    *(u8*)((u8*)r28 + 0x99) = r0;
    goto L_80102AF0;
L_80102ACC: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800F7434();
    goto L_80102AF0;
L_80102AE0: ;
    r12 = *(u32*)((u8*)r31 + 0x10);
    r3 = r28;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80102AF0: ;
    r0 = *(u8*)((u8*)r28 + 0x1B);
    r0 = r0 & 0x00000008;
    if ((s32)r0 == (s32)0x0) goto L_80102B18;
    r0 = *(u8*)((u8*)r28 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_80102B18;
    r0 = 0x0;
    *(u8*)((u8*)r28 + 0x99) = r0;
    *(u8*)((u8*)r28 + 0x98) = r0;
L_80102B18: ;
    r0 = *(u8*)((u8*)r28 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_80102B38;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0x94) = r0;
    r0 = *(u32*)((u8*)r28 + 0x4);
    *(u32*)((u8*)r30 + 0x98) = r0;
    goto L_80102B74;
L_80102B38: ;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x0);
    r0 = r0 & 0x00008000;
    if ((s32)r0 == (s32)0x0) goto L_80102B74;
    r3 = 0x1;
    *(u32*)((u8*)r30 + 0x94) = r3;
    r0 = *(u32*)((u8*)r28 + 0x4);
    *(u32*)((u8*)r30 + 0x98) = r0;
    r0 = *(u8*)((u8*)r28 + 0x1B);
    r0 = r0 & 0x00000010;
    if ((s32)r0 != (s32)0x0) goto L_80102B74;
    *(u8*)((u8*)r28 + 0x98) = r3;
    *(u8*)((u8*)r28 + 0x99) = r3;
L_80102B74: ;
    r0 = *(u8*)((u8*)r28 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_80102BB8;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    r3 = r0 & 0x7;
    ((void(*)(void))fn_8005D7F8)();
    if ((u32)r3 == (u32)0x0) goto L_80102BF8;
    r4 = 0x3;
    ((void(*)(void))fn_8005D798)();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_80102BF8;
    ((void(*)(void))fn_80166A28)();
    goto L_80102BF8;
L_80102BB8: ;
    r0 = *(u8*)((u8*)r28 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_80102BF8;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    r3 = r0 & 0x7;
    ((void(*)(void))fn_8005D7F8)();
    if ((u32)r3 == (u32)0x0) goto L_80102BF8;
    r4 = 0x2;
    ((void(*)(void))fn_8005D798)();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_80102BF8;
    ((void(*)(void))fn_80166A28)();
L_80102BF8: ;
    r0 = *(u8*)((u8*)r28 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_80102C0C;
    r3 = *(u32*)((u8*)r28 + 0x4);
    fn_80106F98();
L_80102C0C: ;
    r0 = *(u8*)((u8*)r28 + 0x98);
    if ((u32)r0 != (u32)0x0) goto L_80102D30;
    r0 = *(u8*)((u8*)r28 + 0x1);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x2) goto L_80102C2C;
    goto L_80102D30;
L_80102C2C: ;
    r0 = *(u16*)((u8*)r28 + 0x94);
    *(u16*)((u8*)r28 + 0x96) = r0;
    r0 = *(u8*)((u8*)r28 + 0x19);
    r0 = (s8)r0;
    if ((s32)r0 <= (s32)0x0) goto L_80102D30;
    r0 = *(u8*)((u8*)r28 + 0x1A);
    if ((s32)r0 == (s32)0x3) goto L_80102C78;
    if ((s32)r0 >= (s32)0x3) goto L_80102C60;
    if ((s32)r0 >= (s32)0x1) goto L_80102C6C;
    goto L_80102C9C;
L_80102C60: ;
    if ((s32)r0 >= (s32)0x5) goto L_80102C9C;
    goto L_80102C8C;
L_80102C6C: ;
    r3 = r28;
    fn_80102F38();
    goto L_80102C9C;
L_80102C78: ;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r4 = 0x0;
    /* crclr cr1eq */;
    fn_800F7434();
    goto L_80102C9C;
L_80102C8C: ;
    r12 = *(u32*)((u8*)r31 + 0xC);
    r3 = r28;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80102C9C: ;
    r3 = *(u16*)((u8*)r28 + 0x96);
    r0 = *(u16*)((u8*)r28 + 0x94);
    *(u16*)(sp + 0xC) = r3;
    *(u16*)(sp + 0x8) = r0;
    r3 = *(u8*)(sp + 0xC);
    r0 = *(u8*)(sp + 0x8);
    r3 = (s8)r3;
    r0 = (s8)r0;
    if ((s32)r3 != (s32)r0) goto L_80102CE4;
    r3 = *(u8*)(sp + 0xD);
    r0 = *(u8*)(sp + 0x9);
    r3 = (s8)r3;
    r0 = (s8)r0;
    if ((s32)r3 != (s32)r0) goto L_80102CE4;
    r0 = 0x0;
    goto L_80102CE8;
L_80102CE4: ;
    r0 = 0x1;
L_80102CE8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80102D30;
    r3 = r28;
    fn_80104CA0();
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    r3 = r0 & 0x7;
    ((void(*)(void))fn_8005D7F8)();
    if ((u32)r3 == (u32)0x0) goto L_80102D30;
    r4 = 0x1;
    ((void(*)(void))fn_8005D798)();
    r3 = r3 & 0xFFFF;
    if ((u32)r3 == (u32)0x0) goto L_80102D30;
    ((void(*)(void))fn_80166A28)();
L_80102D30: ;
    r0 = *(u8*)((u8*)r28 + 0x0);
    r0 = (s8)r0;
    r0 = r0 & 0x00000004;
    if ((s32)r0 == (s32)0x0) goto L_80102D80;
    r3 = r28;
    fn_801071D0();
    r0 = *(u32*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_80102D80;
    r26 = 0x0;
    goto L_80102D74;
L_80102D60: ;
    r12 = *(u32*)((u8*)r31 + 0x14);
    r3 = r28;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r26 = r26 + 0x1;
L_80102D74: ;
    fn_800D3088();
    if ((u32)r26 < (u32)r3) goto L_80102D60;
L_80102D80: ;
    r28 = *(u32*)((u8*)r28 + 0x10);
L_80102D84: ;
    if ((u32)r28 != (u32)0x0) goto L_80102904;
    r28 = *(u32*)((u8*)r30 + 0xC);
    goto L_80102DB0;
L_80102D94: ;
    r0 = *(u8*)((u8*)r28 + 0x1);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x4) goto L_80102DAC;
    r3 = r28;
    fn_8010474C();
L_80102DAC: ;
    r28 = *(u32*)((u8*)r28 + 0x10);
L_80102DB0: ;
    if ((u32)r28 != (u32)0x0) goto L_80102D94;
    r28 = *(u32*)((u8*)r30 + 0xC);
    goto L_80102E78;
L_80102DC0: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80102DEC;
    r0 = *(u8*)((u8*)r28 + 0x9);
    r0 = (s8)r0;
    if ((s32)r0 < (s32)0x50) goto L_80102DEC;
    r27 = 0x0;
    fn_801C43F4();
    r3 = 0x1;
    ((void(*)(void))fn_800D9ED8)();
L_80102DEC: ;
    r0 = *(u8*)((u8*)r28 + 0x0);
    r0 = (s8)r0;
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_80102E74;
    r3 = *(s16*)((u8*)r28 + 0x84);
    r4 = *(s16*)((u8*)r28 + 0x86);
    fn_800FE6D0();
    fn_800FE35C();
    fn_800FE4D4();
    r29 = *(u32*)((u8*)r28 + 0x1C);
    goto L_80102E2C;
L_80102E1C: ;
    r3 = r28;
    r4 = r29;
    fn_80108C14();
    r29 = *(u32*)((u8*)r29 + 0x0);
L_80102E2C: ;
    if ((u32)r29 != (u32)0x0) goto L_80102E1C;
    r29 = *(u32*)((u8*)r28 + 0x20);
    goto L_80102E4C;
L_80102E3C: ;
    r3 = r28;
    r4 = r29;
    fn_80108C14();
    r29 = *(u32*)((u8*)r29 + 0x0);
L_80102E4C: ;
    if ((u32)r29 != (u32)0x0) goto L_80102E3C;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r12 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r12 == (u32)0x0) goto L_80102E74;
    r3 = r28;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80102E74: ;
    r28 = *(u32*)((u8*)r28 + 0x10);
L_80102E78: ;
    if ((u32)r28 != (u32)0x0) goto L_80102DC0;
    r3 = 0x0;
    r4 = 0x0;
    fn_800FE6D0();
    fn_800FE35C();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80102EB0;
    fn_801C43F4();
L_80102EB0: ;
    r3 = 0x0;
    ((void(*)(void))fn_800D9ED8)();
    r3 = 0x1;
    fn_800D9E4C();
    /* lmw r26, 0x18(r1) */;
    return;
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
    extern void fn_80104318();
    extern void fn_80105624();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r20, 0x10(r1) */;
    r30 = r3;
    r21 = 0x0;
    if ((u32)r30 == (u32)0x0) goto L_80103470;
    r0 = *(u8*)((u8*)r30 + 0x1A);
    if ((u32)r0 != (u32)0x2) goto L_80102F68;
    r21 = 0x1;
L_80102F68: ;
    r3 = *(u32*)((u8*)r30 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r22 = r3;
    r3 = r30;
    fn_80104318();
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_80102F94;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x95) = r0;
    goto L_80103470;
L_80102F94: ;
    r25 = 0x10;
    r24 = 0xc;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x6);
    r23 = r0 & 0x1;
    r27 = r0 & 0x00000002;
    r28 = r0 & 0x00000004;
    r29 = r0 & 0x00000008;
L_80102FB4: ;
    r3 = *(s16*)((u8*)r22 + 0x4);
    r26 = 0x0;
    ((void(*)(void))fn_8005D934)();
    if ((s32)r23 == (s32)0x0) goto L_80103040;
    r20 = 0x1e0;
L_80102FCC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80103024;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_80103020;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103008;
    r0 = -r0;
L_80103008: ;
    if ((s32)r0 >= (s32)r25) goto L_80103020;
    if ((s32)r20 <= (s32)r5) goto L_80103020;
    r20 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_80103020: ;
    r26 = r26 + 0x1;
L_80103024: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801031C0;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_80102FCC;
L_80103040: ;
    if ((s32)r27 == (s32)0x0) goto L_801030C0;
    r20 = 0x1e0;
L_8010304C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801030A4;
    r4 = *(s16*)((u8*)r31 + 0x4);
    r0 = *(s16*)((u8*)r3 + 0x4);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_801030A0;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103088;
    r0 = -r0;
L_80103088: ;
    if ((s32)r0 >= (s32)r25) goto L_801030A0;
    if ((s32)r20 <= (s32)r5) goto L_801030A0;
    r20 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_801030A0: ;
    r26 = r26 + 0x1;
L_801030A4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801031C0;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_8010304C;
L_801030C0: ;
    if ((s32)r28 == (s32)0x0) goto L_80103140;
    r20 = 0x280;
L_801030CC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80103124;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_80103120;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103108;
    r0 = -r0;
L_80103108: ;
    if ((s32)r0 >= (s32)r24) goto L_80103120;
    if ((s32)r20 <= (s32)r5) goto L_80103120;
    r20 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_80103120: ;
    r26 = r26 + 0x1;
L_80103124: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801031C0;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_801030CC;
L_80103140: ;
    if ((s32)r29 == (s32)0x0) goto L_801031C0;
    r20 = 0x280;
L_8010314C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801031A4;
    r4 = *(s16*)((u8*)r31 + 0x2);
    r0 = *(s16*)((u8*)r3 + 0x2);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_801031A0;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103188;
    r0 = -r0;
L_80103188: ;
    if ((s32)r0 >= (s32)r24) goto L_801031A0;
    if ((s32)r20 <= (s32)r5) goto L_801031A0;
    r20 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_801031A0: ;
    r26 = r26 + 0x1;
L_801031A4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801031C0;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_8010314C;
L_801031C0: ;
    r3 = *(u8*)((u8*)r30 + 0x97);
    r0 = *(u8*)((u8*)r30 + 0x95);
    r3 = (s8)r3;
    r0 = (s8)r0;
    if ((s32)r3 != (s32)r0) goto L_80103208;
    r25 = r25 + 0x10;
    r24 = r24 + 0xc;
    if ((s32)r25 < (s32)0x280) goto L_801031EC;
    r25 = 0x280;
L_801031EC: ;
    if ((s32)r24 < (s32)0x1e0) goto L_801031F8;
    r24 = 0x1e0;
L_801031F8: ;
    if ((s32)r25 != (s32)0x280) goto L_80102FB4;
    if ((s32)r24 != (s32)0x1e0) goto L_80102FB4;
L_80103208: ;
    r0 = r21 & 0xFFFF;
    if ((u32)r0 == (u32)0x0) goto L_80103470;
    r24 = 0x10;
    r25 = 0xc;
L_8010321C: ;
    r3 = *(u8*)((u8*)r30 + 0x97);
    r0 = *(u8*)((u8*)r30 + 0x95);
    r3 = (s8)r3;
    r0 = (s8)r0;
    if ((s32)r3 != (s32)r0) goto L_80103470;
    r3 = *(s16*)((u8*)r22 + 0x4);
    ((void(*)(void))fn_8005D934)();
    r26 = 0x0;
    if ((s32)r23 == (s32)0x0) goto L_801032C0;
    r21 = r26;
L_8010324C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801032A4;
    r4 = *(s16*)((u8*)r31 + 0x4);
    r0 = *(s16*)((u8*)r3 + 0x4);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_801032A0;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103288;
    r0 = -r0;
L_80103288: ;
    if ((s32)r0 >= (s32)r24) goto L_801032A0;
    if ((s32)r21 >= (s32)r5) goto L_801032A0;
    r21 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_801032A0: ;
    r26 = r26 + 0x1;
L_801032A4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80103440;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_8010324C;
L_801032C0: ;
    if ((s32)r27 == (s32)0x0) goto L_80103340;
    r21 = r26;
L_801032CC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80103324;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_80103320;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103308;
    r0 = -r0;
L_80103308: ;
    if ((s32)r0 >= (s32)r24) goto L_80103320;
    if ((s32)r21 >= (s32)r5) goto L_80103320;
    r21 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_80103320: ;
    r26 = r26 + 0x1;
L_80103324: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80103440;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_801032CC;
L_80103340: ;
    if ((s32)r28 == (s32)0x0) goto L_801033C0;
    r21 = r26;
L_8010334C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801033A4;
    r4 = *(s16*)((u8*)r31 + 0x2);
    r0 = *(s16*)((u8*)r3 + 0x2);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_801033A0;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103388;
    r0 = -r0;
L_80103388: ;
    if ((s32)r0 >= (s32)r25) goto L_801033A0;
    if ((s32)r21 >= (s32)r5) goto L_801033A0;
    r21 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_801033A0: ;
    r26 = r26 + 0x1;
L_801033A4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80103440;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_8010334C;
L_801033C0: ;
    if ((s32)r29 == (s32)0x0) goto L_80103440;
    r21 = r26;
L_801033CC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80103424;
    r4 = *(s16*)((u8*)r3 + 0x2);
    r0 = *(s16*)((u8*)r31 + 0x2);
    r5 = r0 - r4;
    if ((s32)r5 <= (s32)0x0) goto L_80103420;
    r4 = *(s16*)((u8*)r3 + 0x4);
    r0 = *(s16*)((u8*)r31 + 0x4);
    r0 = r0 - r4;
    if ((s32)r0 >= (s32)0x0) goto L_80103408;
    r0 = -r0;
L_80103408: ;
    if ((s32)r0 >= (s32)r25) goto L_80103420;
    if ((s32)r21 >= (s32)r5) goto L_80103420;
    r21 = r5;
    *(u8*)((u8*)r30 + 0x95) = r26;
L_80103420: ;
    r26 = r26 + 0x1;
L_80103424: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80103440;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_801033CC;
L_80103440: ;
    r24 = r24 + 0x10;
    r25 = r25 + 0xc;
    if ((s32)r24 < (s32)0x280) goto L_80103454;
    r24 = 0x280;
L_80103454: ;
    if ((s32)r25 < (s32)0x1e0) goto L_80103460;
    r25 = 0x1e0;
L_80103460: ;
    if ((s32)r24 != (s32)0x280) goto L_8010321C;
    if ((s32)r25 != (s32)0x1e0) goto L_8010321C;
L_80103470: ;
    /* lmw r20, 0x10(r1) */;
    return;
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
    extern u8 lbl_8047CE20[];
    extern u8 lbl_8047CE28[];
    extern u8 lbl_8047CE30[];
    extern void fn_800CE77C();
    extern u8 jumptable_8035B400[];
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98(r1), 0, qr0 */;
    r29 = r3;
    r30 = r4;
    r0 = *(u32*)((u8*)r30 + 0x0);
    r31 = 0x0;
    if ((u32)r0 == (u32)0x0) goto L_80107E54;
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_80107818;
    r3 = *(s16*)((u8*)r30 + 0x14);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x14) = r0;
    r0 = *(u8*)((u8*)r30 + 0x8);
    if ((s32)r0 == (s32)0x2) goto L_8010761C;
    if ((s32)r0 >= (s32)0x2) goto L_80107540;
    if ((s32)r0 >= (s32)0x1) goto L_8010754C;
    goto L_80107800;
L_80107540: ;
    if ((s32)r0 >= (s32)0x4) goto L_80107800;
    goto L_80107714;
L_8010754C: ;
    r3 = *(s16*)((u8*)r30 + 0x14);
    r5 = (0x4330 << 16);
    r0 = *(s16*)((u8*)r30 + 0x16);
    /* xoris r3, r3, 0x8000 */;
    r6 = *(s16*)((u8*)r30 + 0xC);
    /* xoris r4, r0, 0x8000 */;
    r3 = *(s16*)((u8*)r30 + 0x10);
    /* xoris r0, r6, 0x8000 */;
    f2 = *(f64*)lbl_8047CE28;
    r3 = r3 - r6;
    f0 = *(f64*)(sp + 0x8);
    /* xoris r3, r3, 0x8000 */;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f2;
    f3 = f1 / f0;
    f1 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x24) = r0;
    f0 = *(f64*)(sp + 0x20);
    f1 = f1 - f2;
    f0 = f0 - f2;
    f0 = f3 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x28) = f0;
    r0 = *(u32*)(sp + 0x2C);
    *(u16*)((u8*)r29 + 0x0) = r0;
    r4 = *(s16*)((u8*)r30 + 0xE);
    r3 = *(s16*)((u8*)r30 + 0x12);
    /* xoris r0, r4, 0x8000 */;
    r3 = r3 - r4;
    *(u32*)(sp + 0x3C) = r0;
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x34) = r0;
    f0 = *(f64*)(sp + 0x38);
    f1 = *(f64*)(sp + 0x30);
    f0 = f0 - f2;
    f1 = f1 - f2;
    f0 = f3 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x40) = f0;
    r0 = *(u32*)(sp + 0x44);
    *(u16*)((u8*)r29 + 0x2) = r0;
    goto L_80107800;
L_8010761C: ;
    r0 = *(s16*)((u8*)r30 + 0x14);
    r5 = (0x4330 << 16);
    r3 = *(s16*)((u8*)r30 + 0x16);
    /* xoris r6, r0, 0x8000 */;
    r7 = *(s16*)((u8*)r30 + 0xC);
    /* xoris r4, r3, 0x8000 */;
    r3 = *(s16*)((u8*)r30 + 0x10);
    /* xoris r0, r7, 0x8000 */;
    f4 = *(f64*)lbl_8047CE28;
    r3 = r3 - r7;
    f0 = *(f64*)(sp + 0x40);
    /* xoris r3, r3, 0x8000 */;
    f2 = f0 - f4;
    f0 = *(f64*)(sp + 0x38);
    f1 = f0 - f4;
    f0 = *(f64*)(sp + 0x30);
    f1 = f2 * f1;
    f0 = f0 - f4;
    f3 = f1 / f0;
    f1 = *(f64*)(sp + 0x28);
    f0 = *(f64*)(sp + 0x20);
    *(u32*)(sp + 0x1C) = r0;
    f2 = f1 - f4;
    f1 = f0 - f4;
    f0 = *(f64*)(sp + 0x18);
    f2 = f3 / f2;
    f0 = f0 - f4;
    f0 = f2 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x10) = f0;
    r0 = *(u32*)(sp + 0x14);
    *(u16*)((u8*)r29 + 0x0) = r0;
    r4 = *(s16*)((u8*)r30 + 0xE);
    r3 = *(s16*)((u8*)r30 + 0x12);
    /* xoris r0, r4, 0x8000 */;
    r3 = r3 - r4;
    *(u32*)(sp + 0x4C) = r0;
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0xC) = r0;
    f0 = *(f64*)(sp + 0x48);
    f1 = *(f64*)(sp + 0x8);
    f0 = f0 - f4;
    f1 = f1 - f4;
    f0 = f2 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x50) = f0;
    r0 = *(u32*)(sp + 0x54);
    *(u16*)((u8*)r29 + 0x2) = r0;
    goto L_80107800;
L_80107714: ;
    r3 = *(s16*)((u8*)r30 + 0x16);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x50) = r0;
    /* xoris r0, r3, 0x8000 */;
    f1 = *(f64*)lbl_8047CE28;
    *(u32*)(sp + 0x54) = r0;
    f0 = *(f64*)(sp + 0x50);
    f1 = f0 - f1;
    fn_800CE77C();
    r3 = *(s16*)((u8*)r30 + 0x14);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x48) = r0;
    f31 = f1;
    /* xoris r0, r3, 0x8000 */;
    f1 = *(f64*)lbl_8047CE28;
    *(u32*)(sp + 0x4C) = r0;
    f0 = *(f64*)(sp + 0x48);
    f1 = f0 - f1;
    fn_800CE77C();
    f3 = f1 / f31;
    r3 = (0x4330 << 16);
    r5 = *(s16*)((u8*)r30 + 0xC);
    r4 = *(s16*)((u8*)r30 + 0x10);
    /* xoris r0, r5, 0x8000 */;
    r4 = r4 - r5;
    *(u32*)(sp + 0x3C) = r0;
    /* xoris r0, r4, 0x8000 */;
    f2 = *(f64*)lbl_8047CE28;
    *(u32*)(sp + 0x44) = r0;
    f3 = (f32)f3;
    f1 = *(f64*)(sp + 0x40);
    f0 = *(f64*)(sp + 0x38);
    f1 = f1 - f2;
    f0 = f0 - f2;
    f0 = f3 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u16*)((u8*)r29 + 0x0) = r0;
    r4 = *(s16*)((u8*)r30 + 0xE);
    r3 = *(s16*)((u8*)r30 + 0x12);
    /* xoris r0, r4, 0x8000 */;
    r3 = r3 - r4;
    *(u32*)(sp + 0x24) = r0;
    /* xoris r0, r3, 0x8000 */;
    *(u32*)(sp + 0x2C) = r0;
    f0 = *(f64*)(sp + 0x20);
    f1 = *(f64*)(sp + 0x28);
    f0 = f0 - f2;
    f1 = f1 - f2;
    f0 = f3 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u16*)((u8*)r29 + 0x2) = r0;
L_80107800: ;
    r3 = *(s16*)((u8*)r30 + 0x14);
    r0 = *(s16*)((u8*)r30 + 0x16);
    if ((s32)r3 < (s32)r0) goto L_80107818;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x8) = r0;
L_80107818: ;
    r0 = *(u8*)((u8*)r30 + 0x9);
    if ((u32)r0 == (u32)0x0) goto L_801079A8;
    r3 = *(s16*)((u8*)r30 + 0x20);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x20) = r0;
    r0 = *(u8*)((u8*)r30 + 0x9);
    if ((s32)r0 == (s32)0x1) goto L_80107840;
    goto L_80107990;
L_80107840: ;
    r3 = *(s16*)((u8*)r30 + 0x20);
    r4 = (0x4330 << 16);
    r0 = *(s16*)((u8*)r30 + 0x22);
    /* xoris r5, r3, 0x8000 */;
    r6 = *(u8*)((u8*)r30 + 0x18);
    /* xoris r3, r0, 0x8000 */;
    r0 = *(u8*)((u8*)r30 + 0x1C);
    f3 = *(f64*)lbl_8047CE28;
    r0 = r0 - r6;
    f0 = *(f64*)(sp + 0x50);
    /* xoris r0, r0, 0x8000 */;
    f1 = f0 - f3;
    f2 = *(f64*)lbl_8047CE30;
    f0 = *(f64*)(sp + 0x48);
    *(u32*)(sp + 0x44) = r0;
    f0 = f0 - f3;
    f4 = f1 / f0;
    f1 = *(f64*)(sp + 0x40);
    f0 = *(f64*)(sp + 0x38);
    f1 = f1 - f3;
    f0 = f0 - f2;
    f0 = f4 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x30) = f0;
    r0 = *(u32*)(sp + 0x34);
    *(u8*)((u8*)r29 + 0x4) = r0;
    r3 = *(u8*)((u8*)r30 + 0x19);
    r0 = *(u8*)((u8*)r30 + 0x1D);
    r0 = r0 - r3;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x20);
    *(u32*)(sp + 0x2C) = r0;
    f0 = f0 - f2;
    f1 = *(f64*)(sp + 0x28);
    f1 = f1 - f3;
    f0 = f4 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x18) = f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u8*)((u8*)r29 + 0x5) = r0;
    r3 = *(u8*)((u8*)r30 + 0x1A);
    r0 = *(u8*)((u8*)r30 + 0x1E);
    r0 = r0 - r3;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f0 = f0 - f2;
    f1 = *(f64*)(sp + 0x10);
    f1 = f1 - f3;
    f0 = f4 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x58) = f0;
    r0 = *(u32*)(sp + 0x5C);
    *(u8*)((u8*)r29 + 0x6) = r0;
    r3 = *(u8*)((u8*)r30 + 0x1B);
    r0 = *(u8*)((u8*)r30 + 0x1F);
    r0 = r0 - r3;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x68);
    *(u32*)(sp + 0x64) = r0;
    f0 = f0 - f2;
    f1 = *(f64*)(sp + 0x60);
    f1 = f1 - f3;
    f0 = f4 * f1 + f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x70) = f0;
    r0 = *(u32*)(sp + 0x74);
    *(u8*)((u8*)r29 + 0x7) = r0;
L_80107990: ;
    r3 = *(s16*)((u8*)r30 + 0x20);
    r0 = *(s16*)((u8*)r30 + 0x22);
    if ((s32)r3 < (s32)r0) goto L_801079A8;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x9) = r0;
L_801079A8: ;
    r0 = *(u8*)((u8*)r30 + 0xA);
    if ((u32)r0 == (u32)0x0) goto L_80107A4C;
    r3 = *(s16*)((u8*)r30 + 0x34);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x34) = r0;
    r0 = *(u8*)((u8*)r30 + 0xA);
    if ((s32)r0 == (s32)0x1) goto L_801079D0;
    goto L_80107A34;
L_801079D0: ;
    r4 = *(s16*)((u8*)r30 + 0x34);
    r3 = (0x4330 << 16);
    r0 = *(s16*)((u8*)r30 + 0x36);
    /* xoris r4, r4, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047CE28;
    f4 = *(f32*)((u8*)r30 + 0x24);
    f0 = *(f64*)(sp + 0x70);
    *(u32*)(sp + 0x6C) = r0;
    f2 = f0 - f3;
    f0 = *(f32*)((u8*)r30 + 0x2C);
    f0 = f0 - f4;
    f1 = *(f64*)(sp + 0x68);
    f1 = f1 - f3;
    f2 = f2 / f1;
    f0 = f2 * f0 + f4;
    *(f32*)((u8*)r29 + 0xC) = f0;
    f1 = *(f32*)((u8*)r30 + 0x28);
    f0 = *(f32*)((u8*)r30 + 0x30);
    f0 = f0 - f1;
    f0 = f2 * f0 + f1;
    *(f32*)((u8*)r29 + 0x10) = f0;
L_80107A34: ;
    r3 = *(s16*)((u8*)r30 + 0x34);
    r0 = *(s16*)((u8*)r30 + 0x36);
    if ((s32)r3 < (s32)r0) goto L_80107A4C;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0xA) = r0;
L_80107A4C: ;
    r3 = *(s16*)((u8*)r30 + 0x6);
    if ((s32)r3 <= (s32)0x0) goto L_80107A64;
    /* subi r0, r3, 0x1 */;
    *(u16*)((u8*)r30 + 0x6) = r0;
    goto L_80107E54;
L_80107A64: ;
    r0 = *(s16*)((u8*)r30 + 0x4);
    r3 = *(u32*)((u8*)r30 + 0x0);
    r0 = r0 * 0xc;
    r3 = r3 + r0;
    r5 = *(u8*)((u8*)r3 + 0x1);
    if ((u32)r5 > (u32)0xb) goto L_80107E38;
    r4 = (u32)jumptable_8035B400;
    r0 = r5 << 2;
    r4 = (u32)jumptable_8035B400;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r0 = 0x0;
    r31 = 0x1;
    *(u32*)((u8*)r30 + 0x0) = r0;
    *(u16*)((u8*)r30 + 0x4) = r0;
    goto L_80107E38;
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x6) = r0;
    goto L_80107E38;
    if ((u32)r5 != (u32)0x9) goto L_80107AD4;
    r0 = *(s16*)((u8*)r29 + 0x8);
    *(u16*)((u8*)r30 + 0x10) = r0;
    r0 = *(s16*)((u8*)r29 + 0xA);
    *(u16*)((u8*)r30 + 0x12) = r0;
    goto L_80107B38;
L_80107AD4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((s32)r0 == (s32)0x1) goto L_80107B10;
    if ((s32)r0 >= (s32)0x1) goto L_80107B38;
    if ((s32)r0 >= (s32)0x0) goto L_80107AF4;
    goto L_80107B38;
L_80107AF4: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x10) = r0;
    r0 = *(u32*)((u8*)r3 + 0x8);
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x12) = r0;
    goto L_80107B38;
L_80107B10: ;
    r4 = *(s16*)((u8*)r29 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = r4 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x10) = r0;
    r4 = *(s16*)((u8*)r29 + 0x2);
    r0 = *(u32*)((u8*)r3 + 0x8);
    r0 = r4 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x12) = r0;
L_80107B38: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 2, 27 */;
    r0 = r0 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_80107B54;
    r0 = *(s16*)((u8*)r29 + 0x0);
    *(u16*)((u8*)r30 + 0x10) = r0;
L_80107B54: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 2, 27 */;
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_80107B70;
    r0 = *(s16*)((u8*)r29 + 0x2);
    *(u16*)((u8*)r30 + 0x12) = r0;
L_80107B70: ;
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x16) = r0;
    r0 = *(s16*)((u8*)r30 + 0x16);
    if ((s32)r0 != (s32)0x0) goto L_80107BA0;
    r3 = *(s16*)((u8*)r30 + 0x10);
    r0 = 0x0;
    *(u16*)((u8*)r29 + 0x0) = r3;
    r3 = *(s16*)((u8*)r30 + 0x12);
    *(u16*)((u8*)r29 + 0x2) = r3;
    *(u8*)((u8*)r30 + 0x8) = r0;
    goto L_80107E38;
L_80107BA0: ;
    r4 = *(s16*)((u8*)r29 + 0x0);
    r0 = 0x0;
    *(u16*)((u8*)r30 + 0xC) = r4;
    r4 = *(s16*)((u8*)r29 + 0x2);
    *(u16*)((u8*)r30 + 0xE) = r4;
    *(u16*)((u8*)r30 + 0x14) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 2, 25 */;
    if ((s32)r0 == (s32)0x2) goto L_80107C00;
    if ((s32)r0 >= (s32)0x2) goto L_80107BDC;
    if ((s32)r0 == (s32)0x0) goto L_80107BE8;
    if ((s32)r0 >= (s32)0x0) goto L_80107BF4;
    goto L_80107E38;
L_80107BDC: ;
    if ((s32)r0 >= (s32)0x4) goto L_80107E38;
    goto L_80107C0C;
L_80107BE8: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x8) = r0;
    goto L_80107E38;
L_80107BF4: ;
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x8) = r0;
    goto L_80107E38;
L_80107C00: ;
    r0 = 0x3;
    *(u8*)((u8*)r30 + 0x8) = r0;
    goto L_80107E38;
L_80107C0C: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x8) = r0;
    goto L_80107E38;
    r5 = *(u8*)((u8*)r29 + 0x4);
    r4 = 0x0;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x18) = r5;
    r5 = *(u8*)((u8*)r29 + 0x5);
    *(u8*)((u8*)r30 + 0x19) = r5;
    r5 = *(u8*)((u8*)r29 + 0x6);
    *(u8*)((u8*)r30 + 0x1A) = r5;
    r5 = *(u8*)((u8*)r29 + 0x7);
    *(u8*)((u8*)r30 + 0x1B) = r5;
    r5 = *(u32*)((u8*)r3 + 0x4);
    r5 = (u32)r5 >> 24;
    *(u8*)((u8*)r30 + 0x1C) = r5;
    r5 = *(u32*)((u8*)r3 + 0x4);
    /* extrwi r5, r5, 8, 8 */;
    *(u8*)((u8*)r30 + 0x1D) = r5;
    r5 = *(u32*)((u8*)r3 + 0x4);
    /* extrwi r5, r5, 8, 16 */;
    *(u8*)((u8*)r30 + 0x1E) = r5;
    r5 = *(u32*)((u8*)r3 + 0x4);
    r5 = r5 & 0xFF;
    *(u8*)((u8*)r30 + 0x1F) = r5;
    *(u16*)((u8*)r30 + 0x20) = r4;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x22) = r3;
    *(u8*)((u8*)r30 + 0x9) = r0;
    r0 = *(s16*)((u8*)r30 + 0x22);
    if ((s32)r0 != (s32)0x0) goto L_80107E38;
    r0 = *(u8*)((u8*)r30 + 0x1C);
    *(u8*)((u8*)r29 + 0x4) = r0;
    r0 = *(u8*)((u8*)r30 + 0x1D);
    *(u8*)((u8*)r29 + 0x5) = r0;
    r0 = *(u8*)((u8*)r30 + 0x1E);
    *(u8*)((u8*)r29 + 0x6) = r0;
    r0 = *(u8*)((u8*)r30 + 0x1F);
    *(u8*)((u8*)r29 + 0x7) = r0;
    *(u8*)((u8*)r30 + 0x9) = r4;
    goto L_80107E38;
    f0 = *(f32*)((u8*)r29 + 0xC);
    r5 = (0x4330 << 16);
    r4 = 0x0;
    f2 = *(f64*)lbl_8047CE28;
    r0 = 0x1;
    *(f32*)((u8*)r30 + 0x24) = f0;
    f1 = *(f32*)lbl_8047CE20;
    f0 = *(f32*)((u8*)r29 + 0x10);
    *(f32*)((u8*)r30 + 0x28) = f0;
    r5 = *(u32*)((u8*)r3 + 0x4);
    /* xoris r5, r5, 0x8000 */;
    f0 = *(f64*)(sp + 0x70);
    f0 = f0 - f2;
    f0 = f0 / f1;
    *(f32*)((u8*)r30 + 0x2C) = f0;
    r5 = *(u32*)((u8*)r3 + 0x8);
    /* xoris r5, r5, 0x8000 */;
    f0 = *(f64*)(sp + 0x68);
    f0 = f0 - f2;
    f0 = f0 / f1;
    *(f32*)((u8*)r30 + 0x30) = f0;
    *(u16*)((u8*)r30 + 0x34) = r4;
    r3 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x36) = r3;
    *(u8*)((u8*)r30 + 0xA) = r0;
    r0 = *(s16*)((u8*)r30 + 0x36);
    if ((s32)r0 != (s32)0x0) goto L_80107E38;
    f0 = *(f32*)((u8*)r30 + 0x2C);
    *(f32*)((u8*)r29 + 0xC) = f0;
    f0 = *(f32*)((u8*)r30 + 0x30);
    *(f32*)((u8*)r29 + 0x10) = f0;
    *(u8*)((u8*)r30 + 0xA) = r4;
    goto L_80107E38;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_80107D6C;
    r0 = *(u8*)((u8*)r29 + 0x20);
    r0 = r0 | 0x2;
    r0 = (s8)r0;
    *(u8*)((u8*)r29 + 0x20) = r0;
    goto L_80107E38;
L_80107D6C: ;
    r0 = *(u8*)((u8*)r29 + 0x20);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r29 + 0x20) = r0;
    goto L_80107E38;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)0x0) goto L_80107D98;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x3A) = r0;
    goto L_80107E38;
L_80107D98: ;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x3A) = r0;
    goto L_80107E38;
    r0 = *(u8*)((u8*)r30 + 0xB);
    if ((u32)r0 != (u32)0x0) goto L_80107DC0;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0xB) = r0;
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x38) = r0;
L_80107DC0: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 != (u32)0x0) goto L_80107DDC;
    r4 = *(s16*)((u8*)r30 + 0x38);
    /* subi r0, r4, 0x1 */;
    *(u16*)((u8*)r30 + 0x38) = r0;
L_80107DDC: ;
    r0 = *(s16*)((u8*)r30 + 0x38);
    if ((s32)r0 >= (s32)0x0) goto L_80107DF8;
    r0 = 0x0;
    *(u16*)((u8*)r30 + 0x38) = r0;
    *(u8*)((u8*)r30 + 0xB) = r0;
    goto L_80107E38;
L_80107DF8: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    r0 = (s16)r0;
    *(u16*)((u8*)r30 + 0x4) = r0;
    goto L_80107A4C;
    r3 = *(u32*)((u8*)r3 + 0x4);
    ((void(*)(void))fn_8005D858)();
    r0 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)((u8*)r29 + 0x14) = r0;
    r0 = *(s16*)((u8*)r3 + 0x8);
    *(u16*)((u8*)r29 + 0x18) = r0;
    r0 = *(s16*)((u8*)r3 + 0xA);
    *(u16*)((u8*)r29 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r3 + 0xC);
    *(u16*)((u8*)r29 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r3 + 0xE);
    *(u16*)((u8*)r29 + 0x1E) = r0;
L_80107E38: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80107E54;
    r3 = *(s16*)((u8*)r30 + 0x4);
    r0 = r3 + 0x1;
    *(u16*)((u8*)r30 + 0x4) = r0;
    goto L_80107A4C;
L_80107E54: ;
    /* psq_l f31, 0x98(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    r31 = *(u32*)(sp + 0x8C);
    r30 = *(u32*)(sp + 0x88);
    r29 = *(u32*)(sp + 0x84);
    return;
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
    extern u8 lbl_80314F98[];
    extern u8 lbl_8031554C[];
    extern u8 lbl_80404B68[];
    extern u8 lbl_8047CE28[];
    extern u8 lbl_8047CE30[];
    extern u8 lbl_8047CE38[];
    extern u8 lbl_8047CE3C[];
    extern u8 lbl_8047CE40[];
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800DFEEC();
    extern void fn_800E01F4();
    extern void fn_800E0718();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800F92D4();
    u8 sp[0xC0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
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
    f32 f7 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0xB0) = f31;
    /* psq_st f31, 0xb8(r1), 0, qr0 */;
    *(f64*)(sp + 0xA0) = f30;
    /* psq_st f30, 0xa8(r1), 0, qr0 */;
    /* stmw r21, 0x74(r1) */;
    r26 = r4;
    r5 = *(u8*)((u8*)r26 + 0x64);
    r4 = (0x8081 << 16);
    r0 = *(u8*)((u8*)r3 + 0x88);
    r9 = (u32)lbl_80404B68;
    r7 = *(u8*)((u8*)r26 + 0x65);
    /* subi r10, r4, 0x7f7f */;
    r4 = *(u8*)((u8*)r3 + 0x89);
    r8 = r5 * r0;
    r6 = *(u8*)((u8*)r26 + 0x66);
    r31 = (u32)lbl_80404B68;
    r5 = *(u8*)((u8*)r3 + 0x8A);
    r0 = *(u8*)((u8*)r3 + 0x8B);
    r3 = 0x3;
    r7 = r7 * r4;
    r4 = *(u8*)((u8*)r26 + 0x67);
    r5 = r6 * r5;
    r0 = r4 * r0;
    r6 = (s32)((s64)r10 * (s64)r8 >> 32);
    r4 = (s32)((s64)r10 * (s64)r7 >> 32);
    r6 = r6 + r8;
    r8 = (s32)r6 >> 7;
    r6 = (s32)((s64)r10 * (s64)r5 >> 32);
    r9 = (u32)r8 >> 31;
    r4 = r4 + r7;
    r8 = r8 + r9;
    r7 = (s32)r4 >> 7;
    r30 = r8 & 0xFF;
    r4 = (s32)((s64)r10 * (s64)r0 >> 32);
    r5 = r6 + r5;
    r6 = (u32)r7 >> 31;
    r5 = (s32)r5 >> 7;
    r7 = r7 + r6;
    r6 = (u32)r5 >> 31;
    r0 = r4 + r0;
    r5 = r5 + r6;
    r0 = (s32)r0 >> 7;
    r29 = r7 & 0xFF;
    r4 = (u32)r0 >> 31;
    r28 = r5 & 0xFF;
    r0 = r0 + r4;
    r27 = r0 & 0xFF;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x4;
    ((void(*)(void))fn_800D888C)();
    r3 = *(u32*)((u8*)r26 + 0x58);
    fn_800F92D4();
    r25 = r3;
    if ((u32)r25 == (u32)0x0) goto L_80108858;
    r4 = r25;
    r3 = 0x0;
    fn_800D85D4();
    r0 = *(s16*)((u8*)r26 + 0x54);
    if ((s32)r0 >= (s32)0x0) goto L_80108684;
    r0 = -r0;
L_80108684: ;
    r3 = *(s16*)((u8*)r26 + 0x60);
    if ((s32)r3 == (s32)r0) goto L_801086B4;
    /* subi r0, r3, 0x2 */;
    r3 = *(s16*)((u8*)r26 + 0x5C);
    r22 = (s16)r0;
    r0 = r3 + 0x1;
    r24 = (s16)r0;
    if ((s32)r22 >= (s32)0x0) goto L_801086BC;
    r22 = 0x0;
    goto L_801086BC;
L_801086B4: ;
    r24 = *(s16*)((u8*)r26 + 0x5C);
    r22 = r3;
L_801086BC: ;
    r0 = *(s16*)((u8*)r26 + 0x56);
    if ((s32)r0 >= (s32)0x0) goto L_801086CC;
    r0 = -r0;
L_801086CC: ;
    r3 = *(s16*)((u8*)r26 + 0x62);
    if ((s32)r3 == (s32)r0) goto L_801086FC;
    /* subi r0, r3, 0x2 */;
    r3 = *(s16*)((u8*)r26 + 0x5E);
    r21 = (s16)r0;
    r0 = r3 + 0x1;
    r23 = (s16)r0;
    if ((s32)r21 >= (s32)0x0) goto L_80108704;
    r21 = 0x0;
    goto L_80108704;
L_801086FC: ;
    r23 = *(s16*)((u8*)r26 + 0x5E);
    r21 = r3;
L_80108704: ;
    r3 = r25;
    fn_800EF4FC();
    r0 = (s16)r24;
    r3 = r3 & 0xFFFF;
    r5 = (0x4330 << 16);
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CE30;
    r4 = r31 + 0x58;
    f1 = *(f64*)lbl_8047CE28;
    r3 = r25;
    f0 = *(f64*)(sp + 0x28);
    *(u32*)(sp + 0x34) = r0;
    f2 = f0 - f2;
    f0 = *(f64*)(sp + 0x30);
    f0 = f0 - f1;
    f0 = f0 / f2;
    *(f32*)((u8*)r4 + 0x4) = f0;
    *(f32*)((u8*)r31 + 0x58) = f0;
    fn_800EF4FC();
    r4 = (s16)r24;
    r0 = (s16)r22;
    r0 = r4 + r0;
    r3 = r3 & 0xFFFF;
    r5 = (0x4330 << 16);
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CE30;
    r4 = r31 + 0x58;
    f1 = *(f64*)lbl_8047CE28;
    r3 = r25;
    f0 = *(f64*)(sp + 0x38);
    *(u32*)(sp + 0x44) = r0;
    f2 = f0 - f2;
    f0 = *(f64*)(sp + 0x40);
    f0 = f0 - f1;
    f0 = f0 / f2;
    *(f32*)((u8*)r4 + 0xC) = f0;
    *(f32*)((u8*)r4 + 0x8) = f0;
    fn_800EF4F4();
    r4 = (s16)r23;
    r0 = (s16)r21;
    r0 = r4 + r0;
    r3 = r3 & 0xFFFF;
    r5 = (0x4330 << 16);
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CE30;
    r4 = r31 + 0x48;
    f1 = *(f64*)lbl_8047CE28;
    r3 = r25;
    f0 = *(f64*)(sp + 0x48);
    *(u32*)(sp + 0x54) = r0;
    f2 = f0 - f2;
    f0 = *(f64*)(sp + 0x50);
    f0 = f0 - f1;
    f0 = f0 / f2;
    *(f32*)((u8*)r4 + 0xC) = f0;
    *(f32*)((u8*)r31 + 0x48) = f0;
    fn_800EF4F4();
    r0 = (s16)r23;
    r4 = (0x4330 << 16);
    r3 = r3 & 0xFFFF;
    /* xoris r0, r0, 0x8000 */;
    f2 = *(f64*)lbl_8047CE30;
    r3 = r31 + 0x48;
    f1 = *(f64*)lbl_8047CE28;
    f0 = *(f64*)(sp + 0x58);
    *(u32*)(sp + 0x64) = r0;
    f2 = f0 - f2;
    f0 = *(f64*)(sp + 0x60);
    f0 = f0 - f1;
    f0 = f0 / f2;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    goto L_80108888;
L_80108858: ;
    f1 = *(f32*)lbl_8047CE38;
    r3 = r31 + 0x48;
    f0 = *(f32*)lbl_8047CE3C;
    r4 = r31 + 0x58;
    *(f32*)((u8*)r4 + 0x4) = f1;
    *(f32*)((u8*)r31 + 0x58) = f1;
    *(f32*)((u8*)r4 + 0xC) = f0;
    *(f32*)((u8*)r4 + 0x8) = f0;
    *(f32*)((u8*)r3 + 0xC) = f0;
    *(f32*)((u8*)r31 + 0x48) = f0;
    *(f32*)((u8*)r3 + 0x8) = f1;
    *(f32*)((u8*)r3 + 0x4) = f1;
L_80108888: ;
    r5 = *(s16*)((u8*)r26 + 0x54);
    if ((s32)r5 >= (s32)0x0) goto L_8010889C;
    r0 = -r5;
    goto L_801088A0;
L_8010889C: ;
    r0 = r5;
L_801088A0: ;
    r7 = *(s16*)((u8*)r26 + 0x50);
    r3 = (0x4330 << 16);
    /* xoris r4, r0, 0x8000 */;
    r6 = *(s16*)((u8*)r26 + 0x56);
    /* xoris r0, r7, 0x8000 */;
    f3 = *(f64*)lbl_8047CE28;
    f1 = *(f32*)lbl_8047CE40;
    f0 = *(f64*)(sp + 0x60);
    *(u32*)(sp + 0x5C) = r0;
    f2 = f0 - f3;
    f0 = *(f64*)(sp + 0x58);
    f0 = f0 - f3;
    f31 = f2 * f1 + f0;
    if ((s32)r6 >= (s32)0x0) goto L_801088EC;
    r6 = -r6;
L_801088EC: ;
    r0 = *(s16*)((u8*)r26 + 0x52);
    r3 = (0x4330 << 16);
    /* xoris r4, r6, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047CE28;
    f1 = *(f32*)lbl_8047CE40;
    f0 = *(f64*)(sp + 0x50);
    *(u32*)(sp + 0x4C) = r0;
    f2 = f0 - f3;
    f0 = *(f64*)(sp + 0x48);
    f0 = f0 - f3;
    f30 = f2 * f1 + f0;
    if ((s32)r5 >= (s32)0x0) goto L_80108990;
    if ((s32)r5 >= (s32)0x0) goto L_80108938;
    r5 = -r5;
L_80108938: ;
    r3 = r7 + r5;
    r0 = (0x4330 << 16);
    /* xoris r4, r3, 0x8000 */;
    *(u32*)(sp + 0x60) = r0;
    f1 = *(f64*)lbl_8047CE28;
    r3 = r31 + 0x78;
    f0 = *(f64*)(sp + 0x60);
    *(u32*)(sp + 0x58) = r0;
    f0 = f0 - f1;
    f0 = f0 - f31;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r31 + 0x78) = f0;
    r0 = *(s16*)((u8*)r26 + 0x50);
    /* xoris r0, r0, 0x8000 */;
    *(u32*)(sp + 0x5C) = r0;
    f0 = *(f64*)(sp + 0x58);
    f0 = f0 - f1;
    f0 = f0 - f31;
    *(f32*)((u8*)r3 + 0xC) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    goto L_801089F8;
L_80108990: ;
    /* xoris r0, r7, 0x8000 */;
    r3 = r31 + 0x78;
    *(u32*)(sp + 0x64) = r0;
    f0 = *(f64*)(sp + 0x60);
    f0 = f0 - f3;
    f0 = f0 - f31;
    *(f32*)((u8*)r3 + 0x4) = f0;
    *(f32*)((u8*)r31 + 0x78) = f0;
    r5 = *(s16*)((u8*)r26 + 0x54);
    if ((s32)r5 >= (s32)0x0) goto L_801089C4;
    r5 = -r5;
L_801089C4: ;
    r4 = *(s16*)((u8*)r26 + 0x50);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x58) = r0;
    r3 = r31 + 0x78;
    r0 = r4 + r5;
    f1 = *(f64*)lbl_8047CE28;
    /* xoris r0, r0, 0x8000 */;
    *(u32*)(sp + 0x5C) = r0;
    f0 = *(f64*)(sp + 0x58);
    f0 = f0 - f1;
    f0 = f0 - f31;
    *(f32*)((u8*)r3 + 0xC) = f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
L_801089F8: ;
    r5 = *(s16*)((u8*)r26 + 0x56);
    if ((s32)r5 >= (s32)0x0) goto L_80108A74;
    r4 = *(s16*)((u8*)r26 + 0x52);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x60) = r0;
    r3 = r31 + 0x68;
    /* xoris r0, r4, 0x8000 */;
    f1 = *(f64*)lbl_8047CE28;
    *(u32*)(sp + 0x64) = r0;
    f0 = *(f64*)(sp + 0x60);
    f0 = f0 - f1;
    f0 = f0 - f30;
    *(f32*)((u8*)r3 + 0xC) = f0;
    *(f32*)((u8*)r31 + 0x68) = f0;
    if ((s32)r5 >= (s32)0x0) goto L_80108A3C;
    r5 = -r5;
L_80108A3C: ;
    r4 = *(s16*)((u8*)r26 + 0x52);
    r0 = (0x4330 << 16);
    *(u32*)(sp + 0x58) = r0;
    r3 = r31 + 0x68;
    r0 = r4 + r5;
    f1 = *(f64*)lbl_8047CE28;
    /* xoris r0, r0, 0x8000 */;
    *(u32*)(sp + 0x5C) = r0;
    f0 = *(f64*)(sp + 0x58);
    f0 = f0 - f1;
    f0 = f0 - f30;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    goto L_80108AD4;
L_80108A74: ;
    r0 = *(s16*)((u8*)r26 + 0x52);
    if ((s32)r5 >= (s32)0x0) goto L_80108A80;
    r5 = -r5;
L_80108A80: ;
    r3 = r0 + r5;
    r0 = (0x4330 << 16);
    /* xoris r4, r3, 0x8000 */;
    *(u32*)(sp + 0x60) = r0;
    f1 = *(f64*)lbl_8047CE28;
    r3 = r31 + 0x68;
    f0 = *(f64*)(sp + 0x60);
    *(u32*)(sp + 0x58) = r0;
    f0 = f0 - f1;
    f0 = f0 - f30;
    *(f32*)((u8*)r3 + 0xC) = f0;
    *(f32*)((u8*)r31 + 0x68) = f0;
    r0 = *(s16*)((u8*)r26 + 0x52);
    /* xoris r0, r0, 0x8000 */;
    *(u32*)(sp + 0x5C) = r0;
    f0 = *(f64*)(sp + 0x58);
    f0 = f0 - f1;
    f0 = f0 - f30;
    *(f32*)((u8*)r3 + 0x8) = f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
L_80108AD4: ;
    r3 = (u32)lbl_8031554C;
    f1 = *(f32*)((u8*)r26 + 0x70);
    r4 = (u32)lbl_8031554C;
    r3 = r1 + 0x14;
    fn_800E0718();
    r23 = r31 + 0x68;
    r24 = r31 + 0x78;
    r21 = r23;
    r25 = 0x0;
    r22 = r24;
L_80108AFC: ;
    f1 = *(f32*)((u8*)r22 + 0x0);
    r3 = r1 + 0x8;
    f2 = *(f32*)lbl_8047CE38;
    f3 = *(f32*)((u8*)r21 + 0x0);
    fn_800E01F4();
    r3 = r1 + 0x8;
    r4 = r1 + 0x14;
    r5 = r3;
    fn_800DFEEC();
    f2 = *(f32*)(sp + 0x8);
    r25 = r25 + 0x1;
    f0 = *(f32*)((u8*)r26 + 0x68);
    f1 = *(f32*)(sp + 0x10);
    f0 = f2 * f0;
    *(f32*)((u8*)r22 + 0x0) = f0;
    r22 = r22 + 0x4;
    f0 = *(f32*)((u8*)r26 + 0x6C);
    f0 = f1 * f0;
    *(f32*)((u8*)r21 + 0x0) = f0;
    r21 = r21 + 0x4;
    if ((s32)r25 < (s32)0x4) goto L_80108AFC;
    r3 = 0x6;
    fn_800D6A00();
    r3 = (u32)lbl_80314F98;
    r3 = (u32)lbl_80314F98;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    r21 = r31 + 0x48;
    r22 = r31 + 0x58;
    r25 = 0x0;
L_80108B7C: ;
    f1 = *(f32*)((u8*)r24 + 0x0);
    f0 = *(f32*)((u8*)r23 + 0x0);
    f1 = f31 + f1;
    f0 = f30 + f0;
    f1 = (f64)(s32)f1;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x60) = f1;
    *(f64*)(sp + 0x58) = f0;
    r3 = *(u32*)(sp + 0x64);
    r4 = *(u32*)(sp + 0x5C);
    fn_800D61E4();
    r4 = r30;
    r5 = r29;
    r6 = r28;
    r7 = r27;
    r3 = 0x0;
    fn_800D5CB8();
    f1 = *(f32*)((u8*)r22 + 0x0);
    r3 = 0x0;
    f2 = *(f32*)((u8*)r21 + 0x0);
    fn_800D59B8();
    r23 = r23 + 0x4;
    r24 = r24 + 0x4;
    r21 = r21 + 0x4;
    r22 = r22 + 0x4;
    r25 = r25 + 0x1;
    if ((s32)r25 < (s32)0x4) goto L_80108B7C;
    fn_800D6728();
    /* psq_l f31, 0xb8(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0xB0);
    /* psq_l f30, 0xa8(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0xA0);
    /* lmw r21, 0x74(r1) */;
    return;
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
    extern u8 lbl_80314E08[];
    extern u8 lbl_8047CE3C[];
    extern void fn_8001E644();
    extern void fn_8001EA98();
    extern void fn_800D5648();
    extern void fn_800D5CB8();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800DA100();
    extern void fn_800DA4C4();
    extern void fn_800FBB34();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_80108580();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f7 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r26 = r4;
    r0 = *(u8*)((u8*)r26 + 0x74);
    if ((s32)r0 == (s32)0x1) goto L_80108C68;
    if ((s32)r0 >= (s32)0x1) goto L_80108C48;
    if ((s32)r0 >= (s32)0x0) goto L_80108C54;
    goto L_80108CA8;
L_80108C48: ;
    if ((s32)r0 >= (s32)0x3) goto L_80108CA8;
    goto L_80108C98;
L_80108C54: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    goto L_80108CA8;
L_80108C68: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x1;
    r4 = 0x4;
    r5 = 0x0;
    fn_800DA4C4();
    goto L_80108CA8;
L_80108C98: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x1;
    fn_800DA4C4();
L_80108CA8: ;
    r0 = *(u8*)((u8*)r26 + 0x5);
    r0 = r0 & 0x00000008;
    if ((s32)r0 == (s32)0x0) goto L_80108D80;
    r5 = *(s16*)((u8*)r25 + 0x84);
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r25 + 0x86);
    r0 = *(s16*)((u8*)r26 + 0x52);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    r12 = *(u32*)((u8*)r26 + 0x48);
    r3 = r25;
    r4 = r26;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r3 = *(s16*)((u8*)r25 + 0x84);
    r4 = *(s16*)((u8*)r25 + 0x86);
    fn_800FE6D0();
    fn_800FE4D4();
    r0 = *(u8*)((u8*)r26 + 0x74);
    if ((s32)r0 == (s32)0x1) goto L_80108D40;
    if ((s32)r0 >= (s32)0x1) goto L_80108D20;
    if ((s32)r0 >= (s32)0x0) goto L_80108D2C;
    goto L_80108D80;
L_80108D20: ;
    if ((s32)r0 >= (s32)0x3) goto L_80108D80;
    goto L_80108D70;
L_80108D2C: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    goto L_80108D80;
L_80108D40: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x1;
    r7 = 0x0;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x1;
    r4 = 0x4;
    r5 = 0x0;
    fn_800DA4C4();
    goto L_80108D80;
L_80108D70: ;
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x1;
    fn_800DA4C4();
L_80108D80: ;
    if ((u32)r26 != (u32)0x0) goto L_80108D90;
    r0 = 0x0;
    goto L_80108DB0;
L_80108D90: ;
    r0 = *(u8*)((u8*)r26 + 0x4);
    r0 = (s8)r0;
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_80108DAC;
    r0 = 0x1;
    goto L_80108DB0;
L_80108DAC: ;
    r0 = 0x0;
L_80108DB0: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801091E0;
    r4 = *(u8*)((u8*)r26 + 0x64);
    r3 = (0x8081 << 16);
    r0 = *(u8*)((u8*)r25 + 0x88);
    /* subi r9, r3, 0x7f7f */;
    r7 = *(u8*)((u8*)r26 + 0x65);
    r6 = *(u8*)((u8*)r25 + 0x89);
    r8 = r4 * r0;
    r5 = *(u8*)((u8*)r26 + 0x66);
    r4 = *(u8*)((u8*)r25 + 0x8A);
    r3 = *(u8*)((u8*)r26 + 0x67);
    r0 = *(u8*)((u8*)r25 + 0x8B);
    r6 = r7 * r6;
    r4 = r5 * r4;
    r0 = r3 * r0;
    r5 = (s32)((s64)r9 * (s64)r8 >> 32);
    r3 = (s32)((s64)r9 * (s64)r6 >> 32);
    r5 = r5 + r8;
    r7 = (s32)r5 >> 7;
    r5 = (s32)((s64)r9 * (s64)r4 >> 32);
    r8 = (u32)r7 >> 31;
    r3 = r3 + r6;
    r7 = r7 + r8;
    r6 = (s32)r3 >> 7;
    r31 = r7 & 0xFF;
    r3 = (s32)((s64)r9 * (s64)r0 >> 32);
    r4 = r5 + r4;
    r5 = (u32)r6 >> 31;
    r4 = (s32)r4 >> 7;
    r6 = r6 + r5;
    r5 = (u32)r4 >> 31;
    r0 = r3 + r0;
    r30 = r6 & 0xFF;
    r0 = (s32)r0 >> 7;
    r5 = r4 + r5;
    r4 = (u32)r0 >> 31;
    r3 = r31 << 24;
    r0 = r0 + r4;
    r29 = r5 & 0xFF;
    r28 = r0 & 0xFF;
    r0 = r30 << 16;
    r4 = r29 << 8;
    r0 = r3 | r0;
    r0 = r4 | r0;
    r27 = r28 | r0;
    if ((u32)r28 == (u32)0x0) goto L_801091E0;
    r0 = *(u8*)((u8*)r26 + 0x5);
    r0 = r0 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_80108E90;
    r3 = r25;
    r4 = r26;
    fn_80108580();
L_80108E90: ;
    r0 = *(u8*)((u8*)r26 + 0x5);
    r0 = r0 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_801091BC;
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    r3 = (0x1 << 16);
    r4 = *(u32*)((u8*)r26 + 0x8);
    r0 = r3 + 0x2;
    if ((s32)r4 == (s32)r0) goto L_80108FE0;
    if ((s32)r4 >= (s32)r0) goto L_80108ED8;
    if ((s32)r4 == (s32)r3) goto L_80108EEC;
    if ((s32)r4 >= (s32)r3) goto L_80108FAC;
    goto L_801091BC;
L_80108ED8: ;
    r0 = r3 + 0x4;
    if ((s32)r4 == (s32)r0) goto L_80108FC4;
    if ((s32)r4 >= (s32)r0) goto L_801091BC;
    goto L_801090D8;
L_80108EEC: ;
    r3 = 0x3;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x3;
    fn_800D67BC();
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r26 + 0x52);
    fn_800D61E4();
    r4 = r31;
    r5 = r30;
    r6 = r29;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    r5 = *(s16*)((u8*)r26 + 0x50);
    r3 = *(s16*)((u8*)r26 + 0x5C);
    r4 = *(s16*)((u8*)r26 + 0x52);
    r0 = *(s16*)((u8*)r26 + 0x5E);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800D61E4();
    r4 = r31;
    r5 = r30;
    r6 = r29;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    r5 = *(s16*)((u8*)r26 + 0x50);
    r3 = *(s16*)((u8*)r26 + 0x54);
    r4 = *(s16*)((u8*)r26 + 0x52);
    r0 = *(s16*)((u8*)r26 + 0x56);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800D61E4();
    r4 = r31;
    r5 = r30;
    r6 = r29;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    fn_800D6728();
    goto L_801091BC;
L_80108FAC: ;
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r26 + 0x52);
    r5 = *(s16*)((u8*)r26 + 0x54);
    r6 = *(s16*)((u8*)r26 + 0x56);
    fn_8001EA98();
    goto L_801091BC;
L_80108FC4: ;
    r3 = *(s16*)((u8*)r26 + 0x50);
    r7 = r28;
    r4 = *(s16*)((u8*)r26 + 0x52);
    r5 = *(s16*)((u8*)r26 + 0x54);
    r6 = *(s16*)((u8*)r26 + 0x56);
    fn_8001E644();
    goto L_801091BC;
L_80108FE0: ;
    r0 = *(s16*)((u8*)r26 + 0x5C);
    r4 = (0x8081 << 16);
    r3 = *(s16*)((u8*)r26 + 0x5E);
    /* subi r8, r4, 0x7f7f */;
    r4 = r0 & 0xFFFF;
    r0 = *(s16*)((u8*)r26 + 0x60);
    r7 = r4 * r31;
    r4 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    r3 = 0x7;
    r5 = r4 * r30;
    r0 = r0 * r29;
    r4 = (s32)((s64)r8 * (s64)r7 >> 32);
    r6 = (s32)((s64)r8 * (s64)r5 >> 32);
    r4 = r4 + r7;
    r7 = (s32)r4 >> 7;
    r4 = (s32)((s64)r8 * (s64)r0 >> 32);
    r8 = (u32)r7 >> 31;
    r5 = r6 + r5;
    r6 = r7 + r8;
    r5 = (s32)r5 >> 7;
    r31 = r6 & 0xFF;
    r0 = r4 + r0;
    r6 = (u32)r5 >> 31;
    r0 = (s32)r0 >> 7;
    r4 = (u32)r0 >> 31;
    r5 = r5 + r6;
    r0 = r0 + r4;
    r25 = r5 & 0xFF;
    r29 = r0 & 0xFF;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x2;
    fn_800D67BC();
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r26 + 0x52);
    fn_800D61E4();
    r4 = r31;
    r5 = r25;
    r6 = r29;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    r5 = *(s16*)((u8*)r26 + 0x50);
    r3 = *(s16*)((u8*)r26 + 0x54);
    r4 = *(s16*)((u8*)r26 + 0x52);
    r0 = *(s16*)((u8*)r26 + 0x56);
    r3 = r5 + r3;
    r3 = (s16)r3;
    r0 = r4 + r0;
    r4 = (s16)r0;
    fn_800D61E4();
    r4 = r31;
    r5 = r25;
    r6 = r29;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    fn_800D6728();
    goto L_801091BC;
L_801090D8: ;
    r0 = *(s16*)((u8*)r26 + 0x5C);
    r4 = (0x8081 << 16);
    r3 = *(s16*)((u8*)r26 + 0x5E);
    /* subi r7, r4, 0x7f7f */;
    r4 = r0 & 0xFFFF;
    r0 = *(s16*)((u8*)r26 + 0x60);
    r6 = r4 * r31;
    r3 = r3 & 0xFFFF;
    r0 = r0 & 0xFFFF;
    f1 = *(f32*)lbl_8047CE3C;
    r4 = r3 * r30;
    r0 = r0 * r29;
    r3 = (s32)((s64)r7 * (s64)r6 >> 32);
    r5 = (s32)((s64)r7 * (s64)r4 >> 32);
    r3 = r3 + r6;
    r6 = (s32)r3 >> 7;
    r3 = (s32)((s64)r7 * (s64)r0 >> 32);
    r7 = (u32)r6 >> 31;
    r4 = r5 + r4;
    r5 = r6 + r7;
    r4 = (s32)r4 >> 7;
    r25 = r5 & 0xFF;
    r0 = r3 + r0;
    r5 = (u32)r4 >> 31;
    r0 = (s32)r0 >> 7;
    r3 = (u32)r0 >> 31;
    r4 = r4 + r5;
    r0 = r0 + r3;
    r29 = r4 & 0xFF;
    r30 = r0 & 0xFF;
    fn_800D5648();
    r3 = 0x1;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x2;
    fn_800D67BC();
    r3 = *(s16*)((u8*)r26 + 0x50);
    r4 = *(s16*)((u8*)r26 + 0x52);
    fn_800D61E4();
    r4 = r25;
    r5 = r29;
    r6 = r30;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    r3 = *(s16*)((u8*)r26 + 0x54);
    r4 = *(s16*)((u8*)r26 + 0x56);
    fn_800D61E4();
    r4 = r25;
    r5 = r29;
    r6 = r30;
    r7 = r28;
    r3 = 0x0;
    fn_800D5CB8();
    fn_800D6728();
L_801091BC: ;
    r8 = *(u32*)((u8*)r26 + 0x4C);
    if ((u32)r8 == (u32)0x0) goto L_801091E0;
    r3 = *(s16*)((u8*)r26 + 0x50);
    r7 = r27;
    r4 = *(s16*)((u8*)r26 + 0x52);
    r5 = *(s16*)((u8*)r26 + 0x54);
    r6 = *(s16*)((u8*)r26 + 0x56);
    fn_800FBB34();
L_801091E0: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* ===== Small accessor / utility functions ===== */

/* fn_80101A28 -- calls fn_800D2738 then returns 1 */
u32 GSmodel_GetResourceCount(void) {
    fn_800D2738();
    return 1;
}

/* fn_80101A4C -- calls fn_800E4BF4 then returns 1 */
u32 GSmodel_GetResourceByIndex(u32 index) {
    fn_800E4BF4();
    return 1;
}

/* fn_80102004 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
u32 GSmodel_GetJointCount(void* model) {
    extern u8 lbl_80404ACC;
    u32 r3 = 0;

    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r3 + 0x94);
    return;
}
#pragma pop

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
u32 fn_80101A70(u32 idx);
u32 fn_80101A9C(u32 idx);
u32 fn_80101AC4(u32 idx);
void fn_80101B34(u32 arg1);
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


/* 0x801019F8 | 0x30 -- memset(lbl_80402518, 0, 0x2400) */
void fn_801019F8(void) {
    memset(lbl_80402518, 0, GSMODEL_TABLE_SIZE);
}

/* 0x80101A70 | 0x2C -- resource handle getter: table[idx*8 + 4] */
u32 fn_80101A70(u32 idx) {
    if (idx >= lbl_80478B20) {
        return 0;
    }
    return *(u32*)(lbl_80315690 + idx * 8 + 4);
}

/* 0x80101A9C | 0x28 -- resource active getter: table[idx*8 + 0] */
u32 fn_80101A9C(u32 idx) {
    if (idx >= lbl_80478B20) {
        return 0;
    }
    return *(u32*)(lbl_80315690 + idx * 8);
}

/* 0x70 | fn_80101AC4 -- get random resource index in range [active..handle] */
u32 fn_80101AC4(u32 idx) {
    u32 handle = fn_80101A70(idx);
    u32 active = fn_80101A9C(idx);
    u32 rnd;
    u32 range;

    if (handle == active) {
        return active;
    }

    rnd = fn_800E0C54();
    range = handle - active + 1;
    return active + (rnd % range);
}

/* 0x54 | fn_80101B34 -- conditional callback with tick store */
void fn_80101B34(u32 arg1) {
    typedef void (*CallbackFn)(u32);
    CallbackFn cb;

    if ((arg1 & 0xFFFF) == 3) {
        lbl_8047ACF8 = fn_800BE31C();
    }

    cb = (CallbackFn)lbl_8047ACF4;
    if (cb != NULL) {
        cb(arg1);
    }
}

/* 0x80101B90 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101B90(void) {
    extern u8 lbl_80478B28[];
    extern u8 lbl_8047ACE8[];
    extern u8 lbl_8047ACEC[];
    extern u8 lbl_8047CD80[];
    extern u8 lbl_8047CD84[];
    extern u8 lbl_8047CD88[];
    extern u8 lbl_8047CD8C[];
    extern u8 lbl_8047CD90[];
    extern u8 lbl_8047CD94[];
    extern u8 lbl_8047CD98[];
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
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
    f32 f6 = 0.0f;

    r28 = r3;
    r0 = *(u32*)&lbl_8047ACF0;
    if ((s32)r0 == (s32)0x0) goto L_80101D3C;
    OSGetTick();
    r31 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_800D9ED8)();
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    f1 = *(f32*)lbl_8047CD80;
    f3 = *(f32*)lbl_8047CD84;
    f2 = f1;
    f4 = *(f32*)lbl_8047CD88;
    ((void(*)(void))fn_800D9B58)();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x2;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x7;
    fn_800D6A00();
    r3 = 0x0;
    fn_800D7820();
    /* extrwi r5, r28, 8, 8 */;
    /* extrwi r4, r28, 8, 16 */;
    r0 = r28 & 0xFF;
    r3 = 0x2;
    r30 = r5;
    r29 = r4;
    r28 = r0;
    fn_800D67BC();
    r5 = *(u32*)lbl_8047ACE8;
    r3 = (0x4330 << 16);
    r4 = *(u32*)lbl_8047ACEC;
    r0 = *(u8*)lbl_80478B28;
    r4 = r4 - r5;
    f5 = *(f64*)lbl_8047CD98;
    f0 = *(f32*)lbl_8047CD8C;
    f1 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f4 = f1 - f5;
    f6 = *(f32*)lbl_8047CD84;
    f2 = *(f32*)lbl_8047CD90;
    f1 = *(f64*)(sp + 0x10);
    f3 = *(f32*)lbl_8047CD80;
    f1 = f1 - f5;
    f1 = f4 / f1;
    f0 = f1 / f0;
    f1 = f6 * f0;
    fn_800D6680();
    r4 = r30;
    r5 = r29;
    r6 = r28;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    r4 = *(u32*)lbl_8047ACE8;
    r3 = (0x4330 << 16);
    r0 = *(u8*)lbl_80478B28;
    r4 = r31 - r4;
    f5 = *(f64*)lbl_8047CD98;
    f0 = *(f32*)lbl_8047CD8C;
    f1 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x24) = r0;
    f4 = f1 - f5;
    f6 = *(f32*)lbl_8047CD84;
    f2 = *(f32*)lbl_8047CD94;
    f1 = *(f64*)(sp + 0x20);
    f3 = *(f32*)lbl_8047CD80;
    f1 = f1 - f5;
    f1 = f4 / f1;
    f0 = f1 / f0;
    f1 = f6 * f0;
    fn_800D6680();
    r4 = r30;
    r5 = r29;
    r6 = r28;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    fn_800D6728();
    *(u32*)lbl_8047ACEC = r31;
L_80101D3C: ;
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    r28 = *(u32*)(sp + 0x30);
    return;
}
#pragma pop

/* 0x80101D5C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101D5C(void) {
    extern void fn_800B8C58();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r0 = *(u32*)&lbl_8047ACF0;
    if ((s32)r0 == (s32)0x0) goto L_80101D7C;
    r3 = 0x3;
    fn_800B8C58();
L_80101D7C: ;
    return;
}
#pragma pop

/* 0x80101D8C | 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80101D8C(void) {
    extern u8 lbl_80478B28[];
    extern u8 lbl_8047ACE8[];
    extern u8 lbl_8047ACEC[];
    extern u8 lbl_8047CD80[];
    extern u8 lbl_8047CD84[];
    extern u8 lbl_8047CD88[];
    extern u8 lbl_8047CD98[];
    extern u8 lbl_8047CDA0[];
    extern u8 lbl_8047CDA4[];
    extern u8 lbl_8047CDA8[];
    extern u8 lbl_8047CDAC[];
    extern u8 lbl_8047CDB0[];
    extern u8 lbl_8047CDB8[];
    extern void fn_800BE30C();
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x40) = f31;
    /* psq_st f31, 0x48(r1), 0, qr0 */;
    *(f64*)(sp + 0x30) = f30;
    /* psq_st f30, 0x38(r1), 0, qr0 */;
    r0 = *(u32*)&lbl_8047ACF0;
    if ((s32)r0 == (s32)0x0) goto L_80101F8C;
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    f1 = *(f32*)lbl_8047CD80;
    f3 = *(f32*)lbl_8047CD84;
    f2 = f1;
    f4 = *(f32*)lbl_8047CD88;
    ((void(*)(void))fn_800D9B58)();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x2;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r5 = *(u32*)&lbl_8047ACF8;
    r4 = (0x4330 << 16);
    r0 = *(u8*)lbl_80478B28;
    r3 = 0x7;
    f2 = *(f64*)lbl_8047CD98;
    f0 = *(f32*)lbl_8047CDA0;
    f1 = *(f64*)(sp + 0x8);
    *(u32*)(sp + 0x14) = r0;
    f1 = f1 - f2;
    f3 = *(f32*)lbl_8047CD84;
    f1 = f1 / f0;
    f0 = *(f64*)(sp + 0x10);
    f0 = f0 - f2;
    f0 = f1 / f0;
    f30 = f3 * f0;
    fn_800D6A00();
    r3 = 0x0;
    fn_800D7820();
    r3 = 0x2;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047CD80;
    f2 = *(f32*)lbl_8047CDA4;
    f3 = f1;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = f30;
    f2 = *(f32*)lbl_8047CDA8;
    f3 = *(f32*)lbl_8047CD80;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0x0;
    r5 = 0xff;
    r6 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    fn_800D6728();
    r3 = 0x1;
    fn_800D6A00();
    r3 = 0x0;
    fn_800D7820();
    f31 = *(f64*)lbl_8047CDB8;
    r29 = 0x0;
    r30 = 0x0;
    r31 = (0x4330 << 16);
    goto L_80101F6C;
L_80101EF0: ;
    r0 = (s32)r30 / (s32)r3;
    r3 = 0x2;
    /* xoris r0, r0, 0x8000 */;
    *(u32*)(sp + 0x14) = r0;
    f0 = *(f64*)(sp + 0x10);
    f30 = f0 - f31;
    fn_800D67BC();
    f1 = f30;
    f2 = *(f32*)lbl_8047CDAC;
    f3 = *(f32*)lbl_8047CD80;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = f30;
    f2 = *(f32*)lbl_8047CDB0;
    f3 = *(f32*)lbl_8047CD80;
    fn_800D6680();
    r3 = 0x0;
    r4 = 0xff;
    r5 = 0xff;
    r6 = 0xff;
    r7 = 0xff;
    fn_800D5CB8();
    fn_800D6728();
    r30 = r30 + 0x280;
    r29 = r29 + 0x1;
L_80101F6C: ;
    r3 = *(u8*)lbl_80478B28;
    r0 = (s8)r29;
    if ((s32)r0 <= (s32)r3) goto L_80101EF0;
    OSGetTick();
    *(u32*)lbl_8047ACE8 = r3;
    *(u32*)lbl_8047ACEC = r3;
    fn_800BE30C();
L_80101F8C: ;
    /* psq_l f31, 0x48(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x40);
    /* psq_l f30, 0x38(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x30);
    r31 = *(u32*)(sp + 0x2C);
    r30 = *(u32*)(sp + 0x28);
    r29 = *(u32*)(sp + 0x24);
    return;
}
#pragma pop

/* 0x4C | fn_80101FB8 | two_call_arg_check */
void fn_80101FB8(u32 arg1) {
    if (arg1 != 0x1) { return; }
    fn_800B8FD8();
    fn_800BD91C();
}

/* 0x80102014 | 0x24 -- calls fn_80109718(0) */
void fn_80102014(void) {
    fn_80109718();
}

/* 0x80102038 | 0x34 -- init sequence: fn_801096AC, fn_80109664, fn_80109764 */
extern f32 lbl_8047CDC0;
void fn_80102038(f32 arg) {
    fn_801096AC();
    fn_80109664();
    fn_80109764();
}

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
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r4;
    ((void(*)(void))fn_8005DA18)();
    if ((u32)r3 != (u32)0x0) goto L_80102168;
    r3 = -0x3;
    goto L_801021DC;
L_80102168: ;
    r0 = *(s16*)((u8*)r3 + 0x4);
    r30 = 0x0;
    r31 = r0;
L_80102174: ;
    r3 = (s16)r31;
    ((void(*)(void))fn_8005D934)();
    r0 = (s16)r31;
    if ((u32)r0 != (u32)r29) goto L_801021A8;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801021A0;
    r3 = r30;
    goto L_801021DC;
L_801021A0: ;
    r3 = -0x1;
    goto L_801021DC;
L_801021A8: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801021BC;
    r30 = r30 + 0x1;
L_801021BC: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801021D8;
    r0 = *(s16*)((u8*)r3 + 0x18);
    r31 = r0;
    goto L_80102174;
L_801021D8: ;
    r3 = -0x2;
L_801021DC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern void fn_80104704();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_801022F8;
    r4 = *(u8*)((u8*)r3 + 0x95);
    r0 = *(u8*)((u8*)r3 + 0x94);
    r3 = (s8)r4;
    r0 = (s8)r0;
    r31 = r3 + r0;
    goto L_801022FC;
L_801022F8: ;
    r31 = -0x1;
L_801022FC: ;
    if ((s32)r31 != (s32)-0x1) goto L_8010230C;
    r3 = 0x0;
    goto L_8010237C;
L_8010230C: ;
    r3 = r29;
    ((void(*)(void))fn_8005DA18)();
    if ((u32)r3 != (u32)0x0) goto L_80102324;
    r3 = 0x0;
    goto L_8010237C;
L_80102324: ;
    r0 = *(s16*)((u8*)r3 + 0x4);
    r29 = 0x0;
    r30 = r0;
L_80102330: ;
    r3 = (s16)r30;
    ((void(*)(void))fn_8005D934)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_8010235C;
    if ((s32)r29 != (s32)r31) goto L_80102358;
    r3 = (s16)r30;
    goto L_8010237C;
L_80102358: ;
    r29 = r29 + 0x1;
L_8010235C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80102378;
    r0 = *(s16*)((u8*)r3 + 0x18);
    r30 = r0;
    goto L_80102330;
L_80102378: ;
    r3 = 0x0;
L_8010237C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern u8 lbl_80271E10[];
    extern u8 lbl_8035B060[];
    extern void fn_800F037C();
    extern void fn_80104704();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    r0 = r4 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80102494;
L_80102448: ;
    r3 = r31;
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_80102460;
    r3 = 0x0;
    goto L_801024AC;
L_80102460: ;
    fn_800F037C();
    if ((u32)r3 != (u32)0x0) goto L_8010248C;
    r3 = (u32)lbl_80271E10;
    r4 = (u32)lbl_8035B060;
    r3 = (u32)lbl_80271E10;
    r5 = r31;
    r4 = (u32)lbl_8035B060;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    goto L_801024A8;
L_8010248C: ;
    ((void(*)(void))fn_800F0308)();
    goto L_80102448;
L_80102494: ;
    fn_80104704();
    r0 = -r3;
    r0 = r0 | r3;
    r3 = (u32)r0 >> 31;
    goto L_801024AC;
L_801024A8: ;
    r3 = 0x0;
L_801024AC: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x801024C0 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801024C0(void) {
    extern void fn_80104828();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x4;
    fn_80104828();
    return;
}
#pragma pop

/* 0x801024E8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801024E8(void) {
    extern void fn_80104828();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0x4;
    fn_80104828();
    return;
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
    extern u8 lbl_80271E10[];
    extern u8 lbl_8035B060[];
    extern void fn_800F037C();
    extern void fn_80104704();
    extern void fn_80104828();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r31 = r5;
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_801025A0;
    r3 = 0x1;
    goto L_80102604;
L_801025A0: ;
    r4 = r30;
    fn_80104828();
    r0 = r31 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801025F8;
L_801025B4: ;
    r3 = r29;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_80102600;
    fn_800F037C();
    if ((u32)r3 != (u32)0x0) goto L_801025F0;
    r3 = (u32)lbl_80271E10;
    r4 = (u32)lbl_8035B060;
    r3 = (u32)lbl_80271E10;
    r5 = r29;
    r4 = (u32)lbl_8035B060;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    goto L_80102600;
L_801025F0: ;
    ((void(*)(void))fn_800F0308)();
    goto L_801025B4;
L_801025F8: ;
    r3 = r29;
    fn_80104704();
L_80102600: ;
    r3 = 0x0;
L_80102604: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80102620 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80102620(void) {
    extern void fn_80104704();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    fn_80104704();
    r0 = -r3;
    r0 = r0 | r3;
    r3 = (u32)r0 >> 31;
    return;
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
    extern u8 lbl_80271E10[];
    extern u8 lbl_8035B060[];
    extern void fn_800F037C();
    extern void fn_80103EAC();
    extern void fn_801043A4();
    extern void fn_801045A8();
    extern void fn_80104704();
    extern void fn_80104828();
    extern void fn_80104E80();
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    /* stmw r26, 0x78(r1) */;
    r27 = r3;
    r11 = r4;
    r28 = r5;
    r29 = r6;
    r30 = r7;
    if ((s32)r0 != (s32)0) goto L_801026EC;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_801026EC: ;
    r6 = r1 + 0x98;
    r0 = r1 + 0x8;
    r3 = (0x600 << 16);
    r9 = r1 + 0x6c;
    r3 = r28;
    r4 = r27;
    r5 = r11;
    r6 = r29;
    r7 = r8;
    r8 = r9;
    *(u32*)(sp + 0x74) = r0;
    r31 = 0x0;
    fn_80104E80();
    r0 = r30 & 0xFF;
    r26 = r3;
    if ((u32)r0 == (u32)0x0) goto L_801027D4;
    r3 = r27;
    r4 = r30;
    fn_801045A8();
    r3 = r27;
    fn_801043A4();
    r0 = r3;
    r3 = r27;
    r31 = r0;
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x3);
    if ((u32)r0 == (u32)0x0) goto L_8010279C;
    r0 = *(u16*)((u8*)r26 + 0x94);
    r4 = r1 + 0x68;
    *(u16*)(sp + 0x68) = r0;
    r3 = *(u8*)((u8*)r3 + 0x3);
    fn_80103EAC();
L_8010279C: ;
    if ((u32)r28 == (u32)0x0) goto L_801027D4;
    r3 = r27;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_801027CC;
    r4 = *(u8*)((u8*)r3 + 0x95);
    r0 = *(u8*)((u8*)r3 + 0x94);
    r3 = (s8)r4;
    r0 = (s8)r0;
    r0 = r3 + r0;
    goto L_801027D0;
L_801027CC: ;
    r0 = -0x1;
L_801027D0: ;
    *(u32*)((u8*)r28 + 0x0) = r0;
L_801027D4: ;
    r0 = r29 & 0x1;
    if ((u32)r0 == (u32)0x0) goto L_80102850;
    r3 = r27;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_80102850;
    r4 = 0x0;
    fn_80104828();
    r0 = r30 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80102848;
L_80102804: ;
    r3 = r27;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_80102850;
    fn_800F037C();
    if ((u32)r3 != (u32)0x0) goto L_80102840;
    r3 = (u32)lbl_80271E10;
    r4 = (u32)lbl_8035B060;
    r3 = (u32)lbl_80271E10;
    r5 = r27;
    r4 = (u32)lbl_8035B060;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    goto L_80102850;
L_80102840: ;
    ((void(*)(void))fn_800F0308)();
    goto L_80102804;
L_80102848: ;
    r3 = r27;
    fn_80104704();
L_80102850: ;
    r3 = r31;
    /* lmw r26, 0x78(r1) */;
    return;
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
    extern void fn_8008AB8C();
    extern void fn_8008ABA0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r4;
    r30 = r3 + 0x1;
    r29 = 0x0;
    r3 = r30;
    fn_8008ABA0();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010351C;
    r3 = 0x0;
    goto L_801035F8;
L_8010351C: ;
    r3 = r30;
    fn_8008AB8C();
    r3 = r3 & 0xFFFF;
    r0 = r3 & 0x00000040;
    if ((s32)r0 == (s32)0x0) goto L_8010353C;
    r0 = r29 | 0x1;
    r29 = r0 & 0xFFFF;
L_8010353C: ;
    r0 = r3 & 0x00000080;
    if ((s32)r0 == (s32)0x0) goto L_80103550;
    r0 = r29 | 0x2;
    r29 = r0 & 0xFFFF;
L_80103550: ;
    r0 = r3 & 0x00000020;
    if ((s32)r0 == (s32)0x0) goto L_80103564;
    r0 = r29 | 0x4;
    r29 = r0 & 0xFFFF;
L_80103564: ;
    r0 = r3 & 0x00000010;
    if ((s32)r0 == (s32)0x0) goto L_80103578;
    r0 = r29 | 0x8;
    r29 = r0 & 0xFFFF;
L_80103578: ;
    r0 = r3 & 0x1;
    if ((s32)r0 == (s32)0x0) goto L_8010358C;
    r0 = r29 | 0x10;
    r29 = r0 & 0xFFFF;
L_8010358C: ;
    r0 = r3 & 0x00000002;
    if ((s32)r0 == (s32)0x0) goto L_801035A0;
    r0 = r29 | 0x20;
    r29 = r0 & 0xFFFF;
L_801035A0: ;
    r0 = r3 & 0x00000004;
    if ((s32)r0 == (s32)0x0) goto L_801035B4;
    r0 = r29 | 0x100;
    r29 = r0 & 0xFFFF;
L_801035B4: ;
    r0 = r3 & 0x00000200;
    if ((s32)r0 == (s32)0x0) goto L_801035C8;
    r0 = r29 | 0x200;
    r29 = r0 & 0xFFFF;
L_801035C8: ;
    r0 = r3 & 0x00000100;
    if ((s32)r0 == (s32)0x0) goto L_801035DC;
    r0 = r29 | 0x400;
    r29 = r0 & 0xFFFF;
L_801035DC: ;
    r0 = r3 & 0x00000008;
    if ((s32)r0 == (s32)0x0) goto L_801035F0;
    r0 = r29 | 0x800;
    r29 = r0 & 0xFFFF;
L_801035F0: ;
    *(u16*)((u8*)r31 + 0x0) = r29;
    r3 = 0x1;
L_801035F8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80103614 | 0x2E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103614(void) {
    extern u8 lbl_80271E00[];
    extern u8 lbl_8047CDC8[];
    extern u8 lbl_8047CDCC[];
    extern u8 lbl_8047CDD0[];
    extern u8 lbl_8047CDD4[];
    extern u8 lbl_8047CDD8[];
    extern void fn_800CE2D8();
    extern void fn_800F7A08();
    extern void fn_800F7A7C();
    extern void fn_800F7BC4();
    extern void fn_800F7EF8();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r29 = r4;
    r4 = (u32)lbl_80271E00;
    r0 = r3 << 2;
    r4 = (u32)lbl_80271E00;
    r3 = r1 + 0x8;
    r7 = *(u32*)((u8*)r4 + 0x0);
    r31 = 0x0;
    r6 = *(u32*)((u8*)r4 + 0x4);
    r5 = *(u32*)((u8*)r4 + 0x8);
    r4 = *(u32*)((u8*)r4 + 0xC);
    r30 = *(u32*)(r3 + r0);
    r3 = r30;
    fn_800F7EF8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80103688;
    r3 = 0x0;
    goto L_801038D8;
L_80103688: ;
    r3 = r30;
    r4 = 0x0;
    fn_800F7A08();
    r28 = r3;
    r3 = r30;
    r4 = 0x0;
    fn_800F7A7C();
    r0 = (s8)r3;
    if ((s32)r0 >= (s32)0x0) goto L_801036B4;
    r0 = -r0;
L_801036B4: ;
    if ((s32)r0 > (s32)0x20) goto L_801036D4;
    r0 = (s8)r28;
    if ((s32)r0 >= (s32)0x0) goto L_801036CC;
    r0 = -r0;
L_801036CC: ;
    if ((s32)r0 <= (s32)0x20) goto L_801037D8;
L_801036D4: ;
    r4 = (s8)r3;
    r0 = (s8)r28;
    r3 = (0x4330 << 16);
    f2 = *(f64*)lbl_8047CDD8;
    /* xoris r4, r4, 0x8000 */;
    /* xoris r0, r0, 0x8000 */;
    f0 = *(f64*)(sp + 0x18);
    *(u32*)(sp + 0x24) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x20);
    f2 = f0 - f2;
    fn_800CE2D8();
    f2 = (f32)f1;
    f0 = *(f32*)&lbl_8047CDC0;
    if (f2 <= f0) goto L_80103728;
    f1 = f2;
    goto L_8010372C;
L_80103728: ;
    f1 = -f2;
L_8010372C: ;
    f0 = *(f32*)lbl_8047CDC8;
    if (f1 >= f0) goto L_80103744;
    r0 = r31 | 0x2;
    r31 = r0 & 0xFFFF;
    goto L_80103770;
L_80103744: ;
    f0 = *(f32*)&lbl_8047CDC0;
    if (f2 <= f0) goto L_80103758;
    f1 = f2;
    goto L_8010375C;
L_80103758: ;
    f1 = -f2;
L_8010375C: ;
    f0 = *(f32*)lbl_8047CDCC;
    if (f1 <= f0) goto L_80103770;
    r0 = r31 | 0x1;
    r31 = r0 & 0xFFFF;
L_80103770: ;
    f0 = *(f32*)&lbl_8047CDC0;
    f1 = *(f32*)lbl_8047CDD0;
    if (f2 <= f0) goto L_80103788;
    f0 = f2;
    goto L_8010378C;
L_80103788: ;
    f0 = -f2;
L_8010378C: ;
    if (f1 >= f0) goto L_801037D8;
    f0 = *(f32*)&lbl_8047CDC0;
    if (f2 <= f0) goto L_801037A8;
    f1 = f2;
    goto L_801037AC;
L_801037A8: ;
    f1 = -f2;
L_801037AC: ;
    f0 = *(f32*)lbl_8047CDD4;
    if (f1 >= f0) goto L_801037D8;
    f0 = *(f32*)&lbl_8047CDC0;
    if (f2 >= f0) goto L_801037D0;
    r0 = r31 | 0x4;
    r31 = r0 & 0xFFFF;
    goto L_801037D8;
L_801037D0: ;
    r0 = r31 | 0x8;
    r31 = r0 & 0xFFFF;
L_801037D8: ;
    r3 = r30;
    fn_800F7BC4();
    r0 = r3 & 0x00000008;
    if ((u32)r0 == (u32)0x0) goto L_801037F4;
    r0 = r31 | 0x1;
    r31 = r0 & 0xFFFF;
L_801037F4: ;
    r0 = r3 & 0x00000004;
    if ((u32)r0 == (u32)0x0) goto L_80103808;
    r0 = r31 | 0x2;
    r31 = r0 & 0xFFFF;
L_80103808: ;
    r0 = r3 & 0x1;
    if ((u32)r0 == (u32)0x0) goto L_8010381C;
    r0 = r31 | 0x4;
    r31 = r0 & 0xFFFF;
L_8010381C: ;
    r0 = r3 & 0x00000002;
    if ((u32)r0 == (u32)0x0) goto L_80103830;
    r0 = r31 | 0x8;
    r31 = r0 & 0xFFFF;
L_80103830: ;
    r0 = r3 & 0x00000100;
    if ((u32)r0 == (u32)0x0) goto L_80103844;
    r0 = r31 | 0x10;
    r31 = r0 & 0xFFFF;
L_80103844: ;
    r0 = r3 & 0x00000200;
    if ((u32)r0 == (u32)0x0) goto L_80103858;
    r0 = r31 | 0x20;
    r31 = r0 & 0xFFFF;
L_80103858: ;
    r0 = r3 & 0x00000400;
    if ((u32)r0 == (u32)0x0) goto L_8010386C;
    r0 = r31 | 0x40;
    r31 = r0 & 0xFFFF;
L_8010386C: ;
    r0 = r3 & 0x00000800;
    if ((u32)r0 == (u32)0x0) goto L_80103880;
    r0 = r31 | 0x80;
    r31 = r0 & 0xFFFF;
L_80103880: ;
    r0 = r3 & 0x00000010;
    if ((u32)r0 == (u32)0x0) goto L_80103894;
    r0 = r31 | 0x100;
    r31 = r0 & 0xFFFF;
L_80103894: ;
    r0 = r3 & 0x00000040;
    if ((u32)r0 == (u32)0x0) goto L_801038A8;
    r0 = r31 | 0x200;
    r31 = r0 & 0xFFFF;
L_801038A8: ;
    r0 = r3 & 0x00000020;
    if ((u32)r0 == (u32)0x0) goto L_801038BC;
    r0 = r31 | 0x400;
    r31 = r0 & 0xFFFF;
L_801038BC: ;
    r0 = r3 & 0x00001000;
    if ((u32)r0 == (u32)0x0) goto L_801038D0;
    r0 = r31 | 0x800;
    r31 = r0 & 0xFFFF;
L_801038D0: ;
    *(u16*)((u8*)r29 + 0x0) = r31;
    r3 = 0x1;
L_801038D8: ;
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    r29 = *(u32*)(sp + 0x34);
    r28 = *(u32*)(sp + 0x30);
    return;
}
#pragma pop

/* 0x801038F8 | 0x2B0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801038F8(void) {
    extern u8 lbl_8047AD04[];
    extern u8 lbl_8047AD08[];
    extern void fn_800D3088();
    extern void fn_801034DC();
    extern void fn_80103614();
    extern void fn_801046B8();
    extern void fn_80104704();
    extern void fn_801054B8();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r22, 0x18(r1) */;
    r3 = (u32)&lbl_80404ACC;
    r26 = 0x0;
    r31 = (u32)&lbl_80404ACC;
    r30 = (u32)lbl_8047AD04;
    r29 = (u32)lbl_8047AD08;
L_8010391C: ;
    r5 = *(u8*)((u8*)r30 + 0x0);
    r0 = 0x0;
    *(u16*)(sp + 0x8) = r0;
    r3 = r26;
    r28 = r31 + 0x2a;
    r4 = r1 + 0x8;
    *(u8*)((u8*)r29 + 0x0) = r5;
    fn_80103614();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80103978;
    r3 = r26;
    r4 = r1 + 0x8;
    fn_801034DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80103970;
    r0 = 0x0;
    r3 = 0x0;
    *(u16*)(sp + 0x8) = r0;
    goto L_8010397C;
L_80103970: ;
    r3 = 0x2;
    goto L_8010397C;
L_80103978: ;
    r3 = 0x1;
L_8010397C: ;
    r0 = r3 & 0xFF;
    if ((s32)r0 == (s32)0x1) goto L_801039E8;
    if ((s32)r0 >= (s32)0x1) goto L_80103998;
    if ((s32)r0 >= (s32)0x0) goto L_801039A4;
    goto L_80103A28;
L_80103998: ;
    if ((s32)r0 >= (s32)0x3) goto L_80103A28;
    goto L_801039F4;
L_801039A4: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((s32)r0 >= (s32)0x2) goto L_801039B8;
    goto L_801039C4;
L_801039B8: ;
    if ((s32)r0 >= (s32)0x4) goto L_801039C4;
    goto L_801039D0;
L_801039C4: ;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x0) = r0;
    goto L_80103A2C;
L_801039D0: ;
    r3 = (0x1 << 16);
    r4 = 0x3;
    r0 = r3 + -0x8000;
    *(u8*)((u8*)r30 + 0x0) = r4;
    *(u16*)(sp + 0x8) = r0;
    goto L_80103A2C;
L_801039E8: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x0) = r0;
    goto L_80103A2C;
L_801039F4: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    if ((s32)r0 == (s32)0x1) goto L_80103A1C;
    if ((s32)r0 >= (s32)0x1) goto L_80103A0C;
    goto L_80103A10;
L_80103A0C: ;
L_80103A10: ;
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x0) = r0;
    goto L_80103A2C;
L_80103A1C: ;
    r0 = 0x2;
    *(u8*)((u8*)r30 + 0x0) = r0;
    goto L_80103A2C;
L_80103A28: ;
    *(u8*)((u8*)r30 + 0x0) = r3;
L_80103A2C: ;
    r3 = *(u16*)((u8*)r28 + 0x0);
    r24 = 0x0;
    r0 = *(u16*)(sp + 0x8);
    r23 = 0x0;
    *(u16*)((u8*)r28 + 0x2) = r3;
    r27 = 0x0;
    r3 = *(u16*)((u8*)r28 + 0x2);
    r3 = r3 ^ 0xffff;
    r0 = r3 & r0;
    r25 = r0 & 0xFFFF;
L_80103A54: ;
    r0 = 0x1;
    r0 = r0 << r27;
    r22 = r0 & 0xFFFF;
    r0 = r25 & r22;
    if ((s32)r0 == (s32)0x0) goto L_80103A80;
    r0 = r27 + 0xa;
    r3 = 0xf;
    *(u8*)(r28 + r0) = r3;
    r24 = r24 | r22;
    goto L_80103ADC;
L_80103A80: ;
    r0 = *(u16*)(sp + 0x8);
    r0 = r0 & r22;
    if ((s32)r0 == (s32)0x0) goto L_80103ADC;
    fn_800D3088();
    r4 = r27 + 0xa;
    r0 = *(u8*)(r28 + r4);
    r0 = r0 - r3;
    r0 = (s8)r0;
    *(u8*)(r28 + r4) = r0;
    r0 = *(u8*)(r28 + r4);
    r0 = (s8)r0;
    if ((s32)r0 > (s32)0x0) goto L_80103ACC;
    r0 = 0x5;
    r24 = r24 | r22;
    *(u8*)(r28 + r4) = r0;
    r23 = r23 | r22;
    goto L_80103ADC;
L_80103ACC: ;
    r0 = *(u16*)((u8*)r28 + 0x8);
    r0 = r22 & r0;
    r0 = r23 | r0;
    r23 = r0 & 0xFFFF;
L_80103ADC: ;
    r27 = r27 + 0x1;
    if ((s32)r27 < (s32)0x10) goto L_80103A54;
    r0 = r25 & 0xF;
    if ((s32)r0 == (s32)0x0) goto L_80103B08;
    r0 = 0xf;
    *(u8*)((u8*)r28 + 0xA) = r0;
    *(u8*)((u8*)r28 + 0xB) = r0;
    *(u8*)((u8*)r28 + 0xC) = r0;
    *(u8*)((u8*)r28 + 0xD) = r0;
L_80103B08: ;
    r0 = *(u16*)(sp + 0x8);
    r31 = r31 + 0x1a;
    r30 = r30 + 0x1;
    r29 = r29 + 0x1;
    *(u16*)((u8*)r28 + 0x0) = r0;
    r26 = r26 + 0x1;
    *(u16*)((u8*)r28 + 0x4) = r25;
    *(u16*)((u8*)r28 + 0x6) = r24;
    *(u16*)((u8*)r28 + 0x8) = r23;
    if ((s32)r26 < (s32)0x4) goto L_8010391C;
    fn_801046B8();
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_80103B4C;
    r3 = 0x1;
    goto L_80103B50;
L_80103B4C: ;
    r3 = *(u8*)((u8*)r3 + 0xB);
L_80103B50: ;
    fn_801054B8();
    r5 = (u32)&lbl_80404ACC;
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x4);
    r5 = (u32)&lbl_80404ACC;
    *(u32*)((u8*)r5 + 0x10) = r4;
    *(u32*)((u8*)r5 + 0x14) = r0;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0xC);
    *(u32*)((u8*)r5 + 0x18) = r4;
    *(u32*)((u8*)r5 + 0x1C) = r0;
    r4 = *(u32*)((u8*)r3 + 0x10);
    r0 = *(u32*)((u8*)r3 + 0x14);
    *(u32*)((u8*)r5 + 0x20) = r4;
    *(u32*)((u8*)r5 + 0x24) = r0;
    r0 = *(u16*)((u8*)r3 + 0x18);
    *(u16*)((u8*)r5 + 0x28) = r0;
    /* lmw r22, 0x18(r1) */;
    return;
}
#pragma pop

/* 0x80103BA8 | 0x108 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103BA8(void) {
    extern void fn_801054B8();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((s32)r4 == (s32)0x3) goto L_80103BF4;
    if ((s32)r4 >= (s32)0x3) goto L_80103BD8;
    if ((s32)r4 == (s32)0x1) goto L_80103BE4;
    if ((s32)r4 >= (s32)0x1) goto L_80103BEC;
    goto L_80103C04;
L_80103BD8: ;
    if ((s32)r4 >= (s32)0x5) goto L_80103C04;
    goto L_80103BFC;
L_80103BE4: ;
    r3 = 0x1;
    goto L_80103C08;
L_80103BEC: ;
    r3 = 0x2;
    goto L_80103C08;
L_80103BF4: ;
    r3 = 0x4;
    goto L_80103C08;
L_80103BFC: ;
    r3 = 0x8;
    goto L_80103C08;
L_80103C04: ;
    r3 = 0x0;
L_80103C08: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80103C54;
    fn_801054B8();
    r4 = *(u32*)((u8*)r3 + 0x0);
    r0 = *(u32*)((u8*)r3 + 0x4);
    *(u32*)(sp + 0xC) = r0;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r0 = *(u32*)((u8*)r3 + 0xC);
    *(u32*)(sp + 0x14) = r0;
    r4 = *(u32*)((u8*)r3 + 0x10);
    r0 = *(u32*)((u8*)r3 + 0x14);
    *(u32*)(sp + 0x1C) = r0;
    r0 = *(u16*)((u8*)r3 + 0x18);
    *(u16*)(sp + 0x20) = r0;
    goto L_80103C64;
L_80103C54: ;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x1a;
    memset((void*)r3, (int)r4, (u32)r5);
L_80103C64: ;
    r3 = *(u32*)(sp + 0x8);
    r0 = *(u32*)(sp + 0xC);
    *(u32*)((u8*)r31 + 0x0) = r3;
    *(u32*)((u8*)r31 + 0x4) = r0;
    r3 = *(u32*)(sp + 0x10);
    r0 = *(u32*)(sp + 0x14);
    *(u32*)((u8*)r31 + 0x8) = r3;
    *(u32*)((u8*)r31 + 0xC) = r0;
    r3 = *(u32*)(sp + 0x18);
    r0 = *(u32*)(sp + 0x1C);
    *(u32*)((u8*)r31 + 0x10) = r3;
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(u16*)(sp + 0x20);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r31 = *(u32*)(sp + 0x2C);
    return;
}
#pragma pop

/* 0x80103CB0 | 16 bytes | global_getter */
u8 fn_80103CB0(void) {
    return *(u8*)((u8*)lbl_80404ACC + 0x92);
}

/* 0x80103CC0 | 0x18 -- swap byte at lbl_80404ACC+0x92 */
u8 fn_80103CC0(u8 val) {
    u8 old = *(u8*)((u8*)&lbl_80404ACC + 0x92);
    *(u8*)((u8*)&lbl_80404ACC + 0x92) = val;
    return old;
}

/* 0x80103CD8 | 0x190 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80103CD8(void) {
    extern u8 lbl_803156E0[];
    extern u8 lbl_803254E0[];
    extern u8 lbl_803357E0[];
    extern u8 lbl_8047AD00[];
    extern u8 lbl_8047AD04[];
    extern u8 lbl_8047AD08[];
    extern void fn_800D7868();
    extern void fn_800D7894();
    extern void fn_800EFD3C();
    extern void fn_800F9378();
    extern void fn_80103EF4();
    extern void fn_80105410();
    extern void fn_80109810();
    extern void fn_8010C224();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    f32 f7 = 0.0f;

    fn_80105410();
    fn_80103EF4();
    fn_80109810();
    r3 = 0x18;
    fn_8010C224();
    r0 = *(u32*)lbl_8047AD00;
    r5 = 0x0;
    r6 = (u32)&lbl_80404ACC;
    r4 = (u32)lbl_8047AD04;
    r3 = (u32)lbl_8047AD08;
    r6 = (u32)&lbl_80404ACC;
    r0 = 0x1;
    *(u8*)((u8*)r6 + 0x92) = r0;
    *(u8*)lbl_8047AD04 = r5;
    *(u8*)lbl_8047AD08 = r5;
    *(u8*)((u8*)r4 + 0x1) = r5;
    *(u8*)((u8*)r3 + 0x1) = r5;
    *(u8*)((u8*)r4 + 0x2) = r5;
    *(u8*)((u8*)r3 + 0x2) = r5;
    *(u8*)((u8*)r4 + 0x3) = r5;
    *(u8*)((u8*)r3 + 0x3) = r5;
    if ((u32)r0 != (u32)0x0) goto L_80103DF8;
    fn_800D7894();
    *(u32*)lbl_8047AD00 = r3;
    r4 = 0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x3;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
    r3 = *(u32*)lbl_8047AD00;
    r4 = 0x4;
    r5 = 0x0;
    r6 = 0x6;
    r7 = 0xa;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
    r3 = *(u32*)lbl_8047AD00;
    r4 = 0x6;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
    r3 = *(u32*)lbl_8047AD00;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
    r3 = *(u32*)lbl_8047AD00;
    r4 = 0x8;
    r5 = 0x0;
    r6 = 0x8;
    r7 = 0x4;
    r8 = 0x0;
    r9 = 0x0;
    r10 = 0x0;
    fn_800D7868();
L_80103DF8: ;
    r3 = (u32)lbl_803156E0;
    r3 = (u32)lbl_803156E0;
    fn_800EFD3C();
    r5 = (0x31a << 16);
    r4 = 0x0;
    r5 = r5 + 0x1200;
    r6 = 0x0;
    fn_800F9378();
    r3 = (u32)lbl_803254E0;
    r3 = (u32)lbl_803254E0;
    fn_800EFD3C();
    r5 = (0x622 << 16);
    r4 = 0x0;
    r5 = r5 + 0x1200;
    r6 = 0x0;
    fn_800F9378();
    r3 = (u32)lbl_803357E0;
    r3 = (u32)lbl_803357E0;
    fn_800EFD3C();
    r5 = (0x6f7 << 16);
    r4 = 0x0;
    r5 = r5 + 0x1200;
    r6 = 0x0;
    fn_800F9378();
    return;
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
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r4;
    if ((u32)r30 != (u32)0x0) goto L_80104028;
    r3 = 0x0;
    goto L_80104088;
L_80104028: ;
    r3 = *(u16*)((u8*)r30 + 0xAC);
    if ((u32)r3 == (u32)0x0) goto L_80104048;
    ((void(*)(void))fn_800E24B0)();
    r3 = *(u16*)((u8*)r30 + 0xAC);
    ((void(*)(void))fn_800E209C)();
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0xB0) = r0;
L_80104048: ;
    if ((s32)r31 > (s32)0x0) goto L_80104058;
    r3 = 0x0;
    goto L_80104088;
L_80104058: ;
    r3 = r31;
    ((void(*)(void))fn_800E3534)();
    *(u16*)((u8*)r30 + 0xAC) = r3;
    r3 = *(u16*)((u8*)r30 + 0xAC);
    if ((u32)r3 == (u32)0x0) goto L_8010407C;
    ((void(*)(void))fn_800E27B0)();
    *(u32*)((u8*)r30 + 0xB0) = r3;
    goto L_80104084;
L_8010407C: ;
    r3 = 0x0;
    goto L_80104088;
L_80104084: ;
    r3 = *(u32*)((u8*)r30 + 0xB0);
L_80104088: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801040A0 | 24 bytes | beq_addi_ptr */
void* fn_801040A0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x9C;
}

/* 0x801040B8 | 0x18 -- set indexed field: ptr[idx*4 + 0x60] = val */
void fn_801040B8(void* ptr, u32 idx, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + idx * 4 + 0x60) = val;
}

/* 0x801040D0 | 0x20 -- get indexed field: ptr[idx*4 + 0x60] */
u32 fn_801040D0(void* ptr, u32 idx) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + idx * 4 + 0x60);
}

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
    extern u8 lbl_8047CDEC[];
    extern void fn_800FE6AC();
    extern void fn_80108C14();
    u8 sp[0x160];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f7 = 0.0f;

    /* stmw r23, 0x13c(r1) */;
    r24 = r3;
    r25 = r4;
    r26 = r5;
    r27 = r6;
    r23 = r7;
    r28 = r8;
    r29 = r10;
    r30 = r1 + 0x8;
    r3 = r9 & 0xFFFF;
    ((void(*)(void))fn_8005D858)();
    r31 = r3;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x78;
    memset((void*)r3, (int)r4, (u32)r5);
    f0 = *(f32*)lbl_8047CDEC;
    r0 = 0x7;
    *(u8*)(sp + 0xC) = r0;
    *(f32*)(sp + 0x70) = f0;
    *(f32*)(sp + 0x74) = f0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* extrwi r0, r0, 2, 26 */;
    if ((u32)r0 != (u32)0x1) goto L_801041F0;
    r0 = *(u8*)(sp + 0xD);
    r0 = r0 | 0x1;
    r0 = r0 & 0xFF;
    *(u8*)(sp + 0xD) = r0;
    r0 = *(u32*)((u8*)r31 + 0x10);
    *(u32*)(sp + 0x60) = r0;
    goto L_80104210;
L_801041F0: ;
    if ((u32)r0 != (u32)0x2) goto L_80104210;
    r0 = *(u8*)(sp + 0xD);
    r0 = r0 | 0x2;
    r0 = r0 & 0xFF;
    *(u8*)(sp + 0xD) = r0;
    r0 = *(u32*)((u8*)r31 + 0x10);
    *(u32*)(sp + 0x10) = r0;
L_80104210: ;
    r5 = *(u8*)((u8*)r31 + 0x5);
    r3 = (0x8081 << 16);
    r0 = r29 & 0x1;
    r4 = *(u8*)(sp + 0x6F);
    r5 = (s8)r5;
    /* subi r3, r3, 0x7f7f */;
    r5 = r24 + r5;
    r0 = (s16)r5;
    *(u16*)(sp + 0x58) = r0;
    r0 = *(u8*)((u8*)r31 + 0x6);
    r0 = (s8)r0;
    *(u16*)(sp + 0x5C) = r26;
    r0 = r25 + r0;
    r0 = (s16)r0;
    *(u16*)(sp + 0x5E) = r27;
    *(u16*)(sp + 0x5A) = r0;
    r0 = *(s16*)((u8*)r31 + 0x8);
    *(u16*)(sp + 0x64) = r0;
    r0 = *(s16*)((u8*)r31 + 0xA);
    *(u16*)(sp + 0x66) = r0;
    r0 = *(s16*)((u8*)r31 + 0xC);
    *(u16*)(sp + 0x68) = r0;
    r0 = *(s16*)((u8*)r31 + 0xE);
    *(u16*)(sp + 0x6A) = r0;
    r0 = *(u8*)((u8*)r31 + 0x7);
    r0 = r0 * r4;
    r3 = (s32)((s64)r3 * (s64)r0 >> 32);
    r0 = r3 + r0;
    r0 = (s32)r0 >> 7;
    r3 = (u32)r0 >> 31;
    r0 = r0 + r3;
    r0 = r0 & 0xFF;
    *(u8*)(sp + 0x6F) = r0;
    if ((u32)r0 == (u32)0x0) goto L_801042AC;
    r0 = (s16)r26;
    r0 = -r0;
    r0 = (s16)r0;
    *(u16*)(sp + 0x5C) = r0;
L_801042AC: ;
    r0 = r29 & 0x00000002;
    if ((u32)r0 == (u32)0x0) goto L_801042C8;
    r0 = *(s16*)((u8*)r1 + 0x5E);
    r0 = -r0;
    r0 = (s16)r0;
    *(u16*)(sp + 0x5E) = r0;
L_801042C8: ;
    if ((u32)r28 != (u32)0x0) goto L_801042F8;
    r28 = r1 + 0x80;
    r4 = 0x0;
    r3 = r28;
    r5 = 0xb4;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r1 + 0x104;
    r4 = r1 + 0x106;
    fn_800FE6AC();
    r0 = -0x1;
    *(u32*)(sp + 0x108) = r0;
L_801042F8: ;
    r3 = r28;
    r4 = r30;
    fn_80108C14();
    /* lmw r23, 0x13c(r1) */;
    return;
}
#pragma pop

/* 0x80104318 | 0x8C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104318(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r3 = *(u32*)((u8*)r30 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r3 = *(s16*)((u8*)r3 + 0x4);
    ((void(*)(void))fn_8005D934)();
    r31 = 0x0;
L_80104344: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_8010436C;
    r0 = *(u8*)((u8*)r30 + 0x95);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)r31) goto L_80104368;
    goto L_8010438C;
L_80104368: ;
    r31 = r31 + 0x1;
L_8010436C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80104388;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_80104344;
L_80104388: ;
    r3 = 0x0;
L_8010438C: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801043A4 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801043A4(void) {
    extern void fn_801022B8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    if ((s32)r29 > (s32)0x0) goto L_801043D0;
    r31 = 0x0;
    goto L_80104400;
L_801043D0: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r31 = *(u32*)((u8*)r3 + 0xC);
    goto L_801043F4;
L_801043E0: ;
    r0 = *(u32*)((u8*)r31 + 0x4);
    if ((s32)r0 != (s32)r29) goto L_801043F0;
    goto L_80104400;
L_801043F0: ;
    r31 = *(u32*)((u8*)r31 + 0x10);
L_801043F4: ;
    if ((u32)r31 != (u32)0x0) goto L_801043E0;
    r31 = 0x0;
L_80104400: ;
    if ((u32)r31 == (u32)0x0) goto L_801044AC;
    r0 = *(u8*)((u8*)r31 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_8010441C;
    r30 = -0x1;
    goto L_801044B0;
L_8010441C: ;
    r3 = *(u32*)((u8*)r31 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    if ((u32)r3 == (u32)0x0) goto L_80104494;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 2, 24 */;
    if ((s32)r0 == (s32)0x2) goto L_8010447C;
    if ((s32)r0 >= (s32)0x2) goto L_80104450;
    if ((s32)r0 == (s32)0x0) goto L_8010445C;
    if ((s32)r0 >= (s32)0x0) goto L_80104464;
    goto L_801044B0;
L_80104450: ;
    if ((s32)r0 >= (s32)0x4) goto L_801044B0;
    goto L_80104484;
L_8010445C: ;
    r30 = 0x0;
    goto L_801044B0;
L_80104464: ;
    r3 = *(u8*)((u8*)r31 + 0x94);
    r0 = *(u8*)((u8*)r31 + 0x95);
    r3 = (s8)r3;
    r0 = (s8)r0;
    r30 = r3 + r0;
    goto L_801044B0;
L_8010447C: ;
    r30 = *(u32*)((u8*)r31 + 0x80);
    goto L_801044B0;
L_80104484: ;
    r3 = r29;
    fn_801022B8();
    r30 = r3;
    goto L_801044B0;
L_80104494: ;
    r3 = *(u8*)((u8*)r31 + 0x94);
    r0 = *(u8*)((u8*)r31 + 0x95);
    r3 = (s8)r3;
    r0 = (s8)r0;
    r30 = r3 + r0;
    goto L_801044B0;
L_801044AC: ;
    r30 = -0x1;
L_801044B0: ;
    r3 = r30;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern u8 lbl_80271E40[];
    extern u8 lbl_80271E64[];
    extern u8 lbl_8035B070[];
    extern void fn_800F037C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r3 = (u32)&lbl_80404ACC;
    r30 = (u32)&lbl_80404ACC;
    r31 = r4 & 0xFF;
L_801045D0: ;
    if ((s32)r29 > (s32)0x0) goto L_801045E0;
    r3 = 0x0;
    goto L_80104608;
L_801045E0: ;
    r3 = *(u32*)((u8*)r30 + 0xC);
    goto L_801045FC;
L_801045E8: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 != (s32)r29) goto L_801045F8;
    goto L_80104608;
L_801045F8: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_801045FC: ;
    if ((u32)r3 != (u32)0x0) goto L_801045E8;
    r3 = 0x0;
L_80104608: ;
    if ((u32)r3 != (u32)0x0) goto L_80104634;
    r3 = (u32)lbl_80271E40;
    r4 = (u32)lbl_8035B070;
    r3 = (u32)lbl_80271E40;
    r5 = r29;
    r4 = (u32)lbl_8035B070;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
    goto L_8010469C;
L_80104634: ;
    r0 = *(u8*)((u8*)r3 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_80104648;
    r3 = 0x0;
    goto L_8010469C;
L_80104648: ;
    r0 = *(u8*)((u8*)r3 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_8010465C;
    r3 = 0x0;
    goto L_8010469C;
L_8010465C: ;
    if ((u32)r31 == (u32)0x0) goto L_80104698;
    fn_800F037C();
    if ((u32)r3 != (u32)0x0) goto L_80104690;
    r3 = (u32)lbl_80271E64;
    r4 = (u32)lbl_8035B070;
    r3 = (u32)lbl_80271E64;
    r5 = r29;
    r4 = (u32)lbl_8035B070;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    goto L_80104698;
L_80104690: ;
    ((void(*)(void))fn_800F0308)();
    goto L_801045D0;
L_80104698: ;
    r3 = 0x1;
L_8010469C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x801046B8 | 16 bytes | global_getter */
u32 fn_801046B8(void) {
    return *(u32*)((u8*)lbl_80404ACC + 0x4);
}

/* 0x801046C8 | 0x3C -- linked list search by s16 key at offset 6 */
void* fn_801046C8(void* obj, u16 key) {
    void* node;
    if (obj == NULL) { return NULL; }
    node = *(void**)((u8*)obj + 0x1C);
    while (node != NULL) {
        if (*(s16*)((u8*)node + 0x6) == (s16)key) { return node; }
        node = *(void**)((u8*)node + 0x0);
    }
    return NULL;
}

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
    extern void fn_8010925C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r3 = *(u32*)((u8*)r31 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r4 = r3;
    r0 = *(u32*)((u8*)r4 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_80104790;
    r0 = 0x5;
    r3 = r31;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r12 = *(u32*)((u8*)r4 + 0x14);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80104790: ;
    if ((u32)r31 == (u32)0x0) goto L_80104814;
    r3 = *(u32*)((u8*)r31 + 0x14);
    if ((u32)r3 != (u32)0x0) goto L_801047B8;
    r3 = (u32)&lbl_80404ACC;
    r0 = *(u32*)((u8*)r31 + 0x10);
    r3 = (u32)&lbl_80404ACC;
    *(u32*)((u8*)r3 + 0xC) = r0;
    goto L_801047C0;
L_801047B8: ;
    r0 = *(u32*)((u8*)r31 + 0x10);
    *(u32*)((u8*)r3 + 0x10) = r0;
L_801047C0: ;
    r3 = *(u32*)((u8*)r31 + 0x10);
    if ((u32)r3 == (u32)0x0) goto L_801047D4;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r3 + 0x14) = r0;
L_801047D4: ;
    r3 = r31 + 0x1c;
    fn_8010925C();
    r3 = r31 + 0x20;
    fn_8010925C();
    r3 = *(u16*)((u8*)r31 + 0xAC);
    if ((u32)r3 == (u32)0x0) goto L_80104808;
    ((void(*)(void))fn_800E24B0)();
    r3 = *(u16*)((u8*)r31 + 0xAC);
    ((void(*)(void))fn_800E209C)();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0xB0) = r0;
    *(u16*)((u8*)r31 + 0xAC) = r0;
L_80104808: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    *(u32*)((u8*)r31 + 0x4) = r0;
L_80104814: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80104828 | 0x26C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104828(void) {
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r26, 0x8(r1) */;
    r29 = r3;
    r26 = r4;
    r31 = r26 & 0x00000002;
    r28 = r26 & 0x00000004;
    r30 = 0x0;
    if ((u32)r31 != (u32)0x0) goto L_801048E0;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r27 = *(u32*)((u8*)r3 + 0xC);
    goto L_801048D8;
L_80104864: ;
    r3 = r27;
    if ((u32)r27 != (u32)r29) goto L_80104890;
    r0 = 0x0;
    goto L_8010489C;
    goto L_80104890;
L_8010487C: ;
    r3 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r3 != (u32)r29) goto L_80104890;
    r0 = 0x1;
    goto L_8010489C;
L_80104890: ;
    if ((u32)r3 != (u32)0x0) goto L_8010487C;
    r0 = 0x0;
L_8010489C: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801048D4;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801048CC;
    r3 = *(u32*)((u8*)r27 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 28 */;
    if ((u32)r0 != (u32)0x0) goto L_801048D4;
L_801048CC: ;
    r0 = 0x1;
    *(u8*)((u8*)r27 + 0xA) = r0;
L_801048D4: ;
    r27 = *(u32*)((u8*)r27 + 0x10);
L_801048D8: ;
    if ((u32)r27 != (u32)0x0) goto L_80104864;
L_801048E0: ;
    r0 = r26 & 0x1;
    if ((u32)r0 != (u32)0x0) goto L_80104958;
    if ((u32)r29 == (u32)0x0) goto L_80104958;
    r0 = r28 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80104918;
    r3 = *(u32*)((u8*)r29 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 28 */;
    if ((u32)r0 != (u32)0x0) goto L_80104920;
L_80104918: ;
    r0 = 0x1;
    *(u8*)((u8*)r29 + 0xA) = r0;
L_80104920: ;
    if ((u32)r31 == (u32)0x0) goto L_80104958;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_80104950;
L_80104938: ;
    r0 = *(u32*)((u8*)r3 + 0xC);
    if ((u32)r0 != (u32)r29) goto L_8010494C;
    r0 = *(u32*)((u8*)r29 + 0xC);
    *(u32*)((u8*)r3 + 0xC) = r0;
L_8010494C: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_80104950: ;
    if ((u32)r3 != (u32)0x0) goto L_80104938;
L_80104958: ;
    r3 = (u32)&lbl_80404ACC;
    r4 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r4 + 0x4);
    if ((s32)r3 > (s32)0x0) goto L_80104974;
    r5 = 0x0;
    goto L_8010499C;
L_80104974: ;
    r5 = *(u32*)((u8*)r4 + 0xC);
    goto L_80104990;
L_8010497C: ;
    r0 = *(u32*)((u8*)r5 + 0x4);
    if ((s32)r0 != (s32)r3) goto L_8010498C;
    goto L_8010499C;
L_8010498C: ;
    r5 = *(u32*)((u8*)r5 + 0x10);
L_80104990: ;
    if ((u32)r5 != (u32)0x0) goto L_8010497C;
    r5 = 0x0;
L_8010499C: ;
    r3 = (u32)&lbl_80404ACC;
    r29 = r5;
    r3 = (u32)&lbl_80404ACC;
    goto L_80104A64;
L_801049AC: ;
    r0 = *(u8*)((u8*)r29 + 0xA);
    if ((u32)r0 != (u32)0x0) goto L_801049C4;
    r0 = *(u8*)((u8*)r29 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80104A54;
L_801049C4: ;
    r6 = *(u32*)((u8*)r3 + 0xC);
    goto L_80104A3C;
L_801049CC: ;
    r0 = *(u8*)((u8*)r6 + 0xA);
    if ((u32)r0 != (u32)0x0) goto L_80104A38;
    r0 = *(u8*)((u8*)r6 + 0x18);
    if ((u32)r0 != (u32)0x0) goto L_80104A38;
    r0 = *(u32*)((u8*)r29 + 0xC);
    r5 = r6;
    if ((u32)r6 != (u32)r0) goto L_80104A14;
    r0 = 0x0;
    goto L_80104A20;
    goto L_80104A14;
L_80104A00: ;
    r5 = *(u32*)((u8*)r5 + 0xC);
    if ((u32)r5 != (u32)r0) goto L_80104A14;
    r0 = 0x1;
    goto L_80104A20;
L_80104A14: ;
    if ((u32)r5 != (u32)0x0) goto L_80104A00;
    r0 = 0x0;
L_80104A20: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80104A38;
    r0 = *(u32*)((u8*)r6 + 0x4);
    r30 = 0x1;
    *(u32*)((u8*)r4 + 0x4) = r0;
L_80104A38: ;
    r6 = *(u32*)((u8*)r6 + 0x10);
L_80104A3C: ;
    if ((u32)r6 != (u32)0x0) goto L_801049CC;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80104A6C;
    goto L_80104A60;
L_80104A54: ;
    r0 = *(u32*)((u8*)r29 + 0x4);
    *(u32*)((u8*)r4 + 0x4) = r0;
    goto L_80104A6C;
L_80104A60: ;
    r29 = *(u32*)((u8*)r29 + 0xC);
L_80104A64: ;
    if ((u32)r29 != (u32)0x0) goto L_801049AC;
L_80104A6C: ;
    if ((u32)r29 != (u32)0x0) goto L_80104A7C;
    r0 = 0x0;
    *(u32*)((u8*)r4 + 0x4) = r0;
L_80104A7C: ;
    r3 = 0x0;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x80104A94 | 0x20C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104A94(void) {
    extern void fn_801052F4();
    extern void fn_80108518();
    extern void fn_8010925C();
    extern void fn_80109290();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f7 = 0.0f;

    r29 = r3;
    r3 = *(u32*)((u8*)r29 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r3 = *(s16*)((u8*)r3 + 0x4);
    r28 = r3;
    ((void(*)(void))fn_8005D934)();
    r0 = r3;
    r3 = r29 + 0x1c;
    r30 = r0;
    fn_8010925C();
    r3 = r30;
    r4 = r1 + 0xa;
    r5 = r1 + 0x8;
    fn_801052F4();
L_80104AE8: ;
    r3 = (s16)r28;
    ((void(*)(void))fn_8005D934)();
    r30 = r3;
    r3 = r29 + 0x1c;
    fn_80109290();
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_80104B18;
    r3 = r29 + 0x1c;
    fn_8010925C();
    r3 = 0x1;
    goto L_80104C80;
L_80104B18: ;
    *(u16*)((u8*)r31 + 0x6) = r28;
    r0 = *(s16*)((u8*)r30 + 0x2);
    *(u16*)((u8*)r31 + 0x50) = r0;
    r0 = *(s16*)((u8*)r30 + 0x4);
    *(u16*)((u8*)r31 + 0x52) = r0;
    r0 = *(s16*)((u8*)r30 + 0x6);
    *(u16*)((u8*)r31 + 0x54) = r0;
    r0 = *(s16*)((u8*)r30 + 0x8);
    *(u16*)((u8*)r31 + 0x56) = r0;
    r0 = *(u8*)((u8*)r30 + 0x1);
    *(u8*)((u8*)r31 + 0x67) = r0;
    r0 = *(u8*)((u8*)r30 + 0x0);
    /* extrwi r0, r0, 2, 26 */;
    *(u8*)((u8*)r31 + 0x74) = r0;
    r3 = *(s16*)((u8*)r30 + 0xA);
    if ((s32)r3 == (s32)0x0) goto L_80104C34;
    ((void(*)(void))fn_8005D858)();
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 2, 26 */;
    if ((u32)r0 != (u32)0x1) goto L_80104B8C;
    r0 = *(u8*)((u8*)r31 + 0x5);
    r0 = r0 | 0x1;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r31 + 0x5) = r0;
    r0 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)((u8*)r31 + 0x58) = r0;
    goto L_80104BAC;
L_80104B8C: ;
    if ((u32)r0 != (u32)0x2) goto L_80104BAC;
    r0 = *(u8*)((u8*)r31 + 0x5);
    r0 = r0 | 0x2;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r31 + 0x5) = r0;
    r0 = *(u32*)((u8*)r3 + 0x10);
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80104BAC: ;
    r0 = *(s16*)((u8*)r3 + 0x8);
    r4 = (0x8081 << 16);
    /* subi r5, r4, 0x7f7f */;
    *(u16*)((u8*)r31 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r3 + 0xA);
    *(u16*)((u8*)r31 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r3 + 0xC);
    *(u16*)((u8*)r31 + 0x60) = r0;
    r0 = *(s16*)((u8*)r3 + 0xE);
    *(u16*)((u8*)r31 + 0x62) = r0;
    r4 = *(u8*)((u8*)r31 + 0x67);
    r0 = *(u8*)((u8*)r3 + 0x7);
    r0 = r4 * r0;
    r4 = (s32)((s64)r5 * (s64)r0 >> 32);
    r0 = r4 + r0;
    r0 = (s32)r0 >> 7;
    r4 = (u32)r0 >> 31;
    r0 = r0 + r4;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r31 + 0x67) = r0;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 == (u32)0x0) goto L_80104C1C;
    r0 = *(s16*)((u8*)r1 + 0xA);
    *(u16*)((u8*)r31 + 0x54) = r0;
    r0 = *(s16*)((u8*)r1 + 0x8);
    *(u16*)((u8*)r31 + 0x56) = r0;
L_80104C1C: ;
    r0 = *(u16*)((u8*)r3 + 0x0);
    r4 = r0 & 0xFFF;
    if ((u32)r4 == (u32)0x0) goto L_80104C34;
    r3 = r31 + 0xc;
    fn_80108518();
L_80104C34: ;
    r0 = *(u32*)((u8*)r30 + 0x10);
    *(u32*)((u8*)r31 + 0x4C) = r0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_80104C60;
    r0 = *(u8*)((u8*)r31 + 0x5);
    r0 = r0 | 0x8;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r31 + 0x5) = r0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    *(u32*)((u8*)r31 + 0x48) = r0;
L_80104C60: ;
    r0 = *(u8*)((u8*)r30 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80104C7C;
    r0 = *(s16*)((u8*)r30 + 0x18);
    r28 = r0;
    goto L_80104AE8;
L_80104C7C: ;
    r3 = 0x0;
L_80104C80: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80104CA0 | 0x1E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104CA0(void) {
    extern void fn_80108518();
    extern void fn_8010925C();
    extern void fn_80109290();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r31 = r3;
    r3 = *(u32*)((u8*)r28 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r3 = *(s16*)((u8*)r3 + 0x4);
    ((void(*)(void))fn_8005D934)();
    r29 = 0x0;
L_80104CE0: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_80104D0C;
    r0 = *(u8*)((u8*)r28 + 0x95);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)r29) goto L_80104D08;
    r30 = r3;
    goto L_80104D2C;
L_80104D08: ;
    r29 = r29 + 0x1;
L_80104D0C: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80104D28;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_80104CE0;
L_80104D28: ;
    r30 = 0x0;
L_80104D2C: ;
    if ((u32)r30 == (u32)0x0) goto L_80104E60;
    r3 = r28 + 0x20;
    fn_8010925C();
    r3 = *(s16*)((u8*)r30 + 0xC);
    if ((s32)r3 == (s32)0x0) goto L_80104E60;
    ((void(*)(void))fn_8005D858)();
    r29 = r3;
L_80104D50: ;
    r3 = r28 + 0x20;
    fn_80109290();
    if ((u32)r3 != (u32)0x0) goto L_80104D6C;
    r3 = r28 + 0x20;
    fn_8010925C();
    goto L_80104E60;
L_80104D6C: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    /* extrwi r0, r0, 2, 26 */;
    if ((u32)r0 != (u32)0x1) goto L_80104D98;
    r0 = *(u8*)((u8*)r3 + 0x5);
    r0 = r0 | 0x1;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r3 + 0x5) = r0;
    r0 = *(u32*)((u8*)r29 + 0x10);
    *(u32*)((u8*)r3 + 0x58) = r0;
    goto L_80104DB8;
L_80104D98: ;
    if ((u32)r0 != (u32)0x2) goto L_80104DB8;
    r0 = *(u8*)((u8*)r3 + 0x5);
    r0 = r0 | 0x2;
    r0 = r0 & 0xFF;
    *(u8*)((u8*)r3 + 0x5) = r0;
    r0 = *(u32*)((u8*)r29 + 0x10);
    *(u32*)((u8*)r3 + 0x8) = r0;
L_80104DB8: ;
    r0 = *(s16*)((u8*)r31 + 0x4);
    *(u16*)((u8*)r3 + 0x6) = r0;
    r0 = *(u8*)((u8*)r29 + 0x5);
    r4 = *(s16*)((u8*)r30 + 0x2);
    r0 = (s8)r0;
    r0 = r4 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r3 + 0x50) = r0;
    r0 = *(u8*)((u8*)r29 + 0x6);
    r4 = *(s16*)((u8*)r30 + 0x4);
    r0 = (s8)r0;
    r0 = r4 + r0;
    r0 = (s16)r0;
    *(u16*)((u8*)r3 + 0x52) = r0;
    r0 = *(s16*)((u8*)r29 + 0xC);
    *(u16*)((u8*)r3 + 0x54) = r0;
    r0 = *(s16*)((u8*)r29 + 0xE);
    *(u16*)((u8*)r3 + 0x56) = r0;
    r0 = *(s16*)((u8*)r29 + 0x8);
    *(u16*)((u8*)r3 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r29 + 0xA);
    *(u16*)((u8*)r3 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r29 + 0xC);
    *(u16*)((u8*)r3 + 0x60) = r0;
    r0 = *(s16*)((u8*)r29 + 0xE);
    *(u16*)((u8*)r3 + 0x62) = r0;
    r0 = *(u8*)((u8*)r29 + 0x7);
    *(u8*)((u8*)r3 + 0x67) = r0;
    r0 = *(u16*)((u8*)r29 + 0x0);
    r4 = r0 & 0xFFF;
    if ((u32)r4 == (u32)0x0) goto L_80104E40;
    r3 = r3 + 0xc;
    fn_80108518();
L_80104E40: ;
    r0 = *(u8*)((u8*)r29 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 != (u32)0x0) goto L_80104E60;
    r3 = *(s16*)((u8*)r29 + 0x14);
    ((void(*)(void))fn_8005D858)();
    r29 = r3;
    goto L_80104D50;
L_80104E60: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80104E80 | 0x474 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80104E80(void) {
    extern u8 lbl_80271E94[];
    extern u8 lbl_8047CDEC[];
    extern void fn_80103484();
    extern void fn_80103CB0();
    extern void fn_80103E68();
    extern void fn_80104A94();
    extern void fn_80104CA0();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r23, 0xc(r1) */;
    r27 = r3;
    r28 = r4;
    r23 = r5;
    r29 = r6;
    r30 = r7;
    r31 = r8;
    if ((s32)r23 <= (s32)0x0) goto L_80104ED8;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_80104ED0;
L_80104EC0: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)r23) goto L_80104ED8;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_80104ED0: ;
    if ((u32)r3 != (u32)0x0) goto L_80104EC0;
L_80104ED8: ;
    r3 = r28;
    ((void(*)(void))fn_8005DA18)();
    r26 = r3;
    if ((s32)r28 > (s32)0x0) goto L_80104EF4;
    r4 = 0x0;
    goto L_80104F24;
L_80104EF4: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r4 = *(u32*)((u8*)r3 + 0xC);
    goto L_80104F18;
L_80104F04: ;
    r0 = *(u32*)((u8*)r4 + 0x4);
    if ((s32)r0 != (s32)r28) goto L_80104F14;
    goto L_80104F24;
L_80104F14: ;
    r4 = *(u32*)((u8*)r4 + 0x10);
L_80104F18: ;
    if ((u32)r4 != (u32)0x0) goto L_80104F04;
    r4 = 0x0;
L_80104F24: ;
    r25 = r4;
    if ((u32)r4 != (u32)0x0) goto L_801051AC;
    r0 = *(u8*)((u8*)r26 + 0x0);
    /* extrwi r0, r0, 1, 27 */;
    if ((u32)r0 == (u32)0x0) goto L_80104F48;
    r24 = 0x64;
    goto L_80104FDC;
L_80104F48: ;
    r0 = r29 & 0x00000002;
    if ((u32)r0 == (u32)0x0) goto L_80104F5C;
    r24 = 0x64;
    goto L_80104FDC;
L_80104F5C: ;
    if ((s32)r23 != (s32)0x0) goto L_80104F70;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r23 = *(u32*)((u8*)r3 + 0x4);
L_80104F70: ;
    if ((s32)r23 > (s32)0x0) goto L_80104F80;
    r3 = 0x0;
    goto L_80104FB0;
L_80104F80: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_80104FA4;
L_80104F90: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 != (s32)r23) goto L_80104FA0;
    goto L_80104FB0;
L_80104FA0: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_80104FA4: ;
    if ((u32)r3 != (u32)0x0) goto L_80104F90;
    r3 = 0x0;
L_80104FB0: ;
    if ((u32)r3 == (u32)0x0) goto L_80104FC8;
    r3 = *(u8*)((u8*)r3 + 0x9);
    r0 = r3 + 0x1;
    r24 = (s8)r0;
    goto L_80104FCC;
L_80104FC8: ;
    r24 = 0x0;
L_80104FCC: ;
    r0 = (s8)r24;
    if ((s32)r0 <= (s32)0x64) goto L_80104FDC;
    r24 = 0x64;
L_80104FDC: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r25 = *(u32*)((u8*)r3 + 0x8);
    r0 = *(u16*)((u8*)r3 + 0x0);
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_80105040;
L_80104FF8: ;
    r0 = *(u8*)((u8*)r25 + 0x0);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_80105038;
    r3 = r25;
    r4 = 0x0;
    r5 = 0xb4;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = 0x7;
    f0 = *(f32*)lbl_8047CDEC;
    *(u8*)((u8*)r25 + 0x0) = r0;
    r0 = -0x1;
    *(f32*)((u8*)r25 + 0x8C) = f0;
    *(f32*)((u8*)r25 + 0x90) = f0;
    *(u32*)((u8*)r25 + 0x88) = r0;
    goto L_80105054;
L_80105038: ;
    r25 = r25 + 0xb4;
    if (--ctr != 0) goto L_80104FF8;
L_80105040: ;
    r3 = (u32)lbl_80271E94;
    r3 = (u32)lbl_80271E94;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r25 = 0x0;
L_80105054: ;
    if ((u32)r25 != (u32)0x0) goto L_80105064;
    r3 = 0x0;
    goto L_801052E0;
L_80105064: ;
    r3 = 0x0;
    *(u8*)((u8*)r25 + 0x1) = r3;
    *(u32*)((u8*)r25 + 0x4) = r28;
    *(u8*)((u8*)r25 + 0x9) = r24;
    r0 = *(u8*)((u8*)r26 + 0x1);
    /* extrwi r0, r0, 3, 24 */;
    *(u8*)((u8*)r25 + 0x1A) = r0;
    if ((s32)r23 > (s32)0x0) goto L_8010508C;
    goto L_801050BC;
L_8010508C: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r3 = *(u32*)((u8*)r3 + 0xC);
    goto L_801050B0;
L_8010509C: ;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 != (s32)r23) goto L_801050AC;
    goto L_801050BC;
L_801050AC: ;
    r3 = *(u32*)((u8*)r3 + 0x10);
L_801050B0: ;
    if ((u32)r3 != (u32)0x0) goto L_8010509C;
    r3 = 0x0;
L_801050BC: ;
    *(u32*)((u8*)r25 + 0xC) = r3;
    r0 = *(s16*)((u8*)r26 + 0x6);
    *(u16*)((u8*)r25 + 0x84) = r0;
    r0 = *(s16*)((u8*)r26 + 0x8);
    *(u16*)((u8*)r25 + 0x86) = r0;
    r3 = *(s16*)((u8*)r26 + 0x4);
    ((void(*)(void))fn_8005D934)();
L_801050D8: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 24 */;
    if ((u32)r0 == (u32)0x0) goto L_801050F4;
    r4 = *(u8*)((u8*)r25 + 0x19);
    r0 = r4 + 0x1;
    *(u8*)((u8*)r25 + 0x19) = r0;
L_801050F4: ;
    r0 = *(u8*)((u8*)r3 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_80105110;
    r3 = *(s16*)((u8*)r3 + 0x18);
    ((void(*)(void))fn_8005D934)();
    goto L_801050D8;
L_80105110: ;
    r3 = r25;
    fn_80104A94();
    r3 = (u32)&lbl_80404ACC;
    r4 = (u32)&lbl_80404ACC;
    r5 = *(u32*)((u8*)r4 + 0xC);
    if ((u32)r5 != (u32)0x0) goto L_80105134;
    *(u32*)((u8*)r4 + 0xC) = r25;
    goto L_8010519C;
L_80105134: ;
    r0 = (s8)r24;
L_80105138: ;
    r3 = *(u8*)((u8*)r5 + 0x9);
    r3 = (s8)r3;
    if ((s32)r3 <= (s32)r0) goto L_80105174;
    r0 = *(u32*)((u8*)r5 + 0x14);
    *(u32*)((u8*)r25 + 0x14) = r0;
    *(u32*)((u8*)r25 + 0x10) = r5;
    r3 = *(u32*)((u8*)r5 + 0x14);
    if ((u32)r3 != (u32)0x0) goto L_80105168;
    *(u32*)((u8*)r4 + 0xC) = r25;
    goto L_8010516C;
L_80105168: ;
    *(u32*)((u8*)r3 + 0x10) = r25;
L_8010516C: ;
    *(u32*)((u8*)r5 + 0x14) = r25;
    goto L_8010519C;
L_80105174: ;
    r3 = *(u32*)((u8*)r5 + 0x10);
    if ((u32)r3 != (u32)0x0) goto L_80105194;
    *(u32*)((u8*)r5 + 0x10) = r25;
    r0 = 0x0;
    *(u32*)((u8*)r25 + 0x14) = r5;
    *(u32*)((u8*)r25 + 0x10) = r0;
    goto L_8010519C;
L_80105194: ;
    r5 = r3;
    goto L_80105138;
L_8010519C: ;
    r3 = r28;
    r4 = 0x4;
    fn_80103484();
    goto L_801051C4;
L_801051AC: ;
    r3 = 0x0;
    r0 = 0x1;
    *(u8*)((u8*)r4 + 0x98) = r3;
    *(u8*)((u8*)r4 + 0x99) = r3;
    *(u8*)((u8*)r4 + 0x1) = r3;
    *(u8*)((u8*)r4 + 0x2) = r0;
L_801051C4: ;
    if ((u32)r27 != (u32)0x0) goto L_801051E8;
    r3 = *(u8*)((u8*)r26 + 0x3);
    if ((u32)r3 == (u32)0x0) goto L_801051FC;
    fn_80103E68();
    r0 = (u32)r3 >> 16;
    *(u16*)((u8*)r25 + 0x94) = r0;
    goto L_801051FC;
L_801051E8: ;
    r0 = 0x0;
    *(u8*)((u8*)r25 + 0x94) = r0;
    r0 = *(u32*)((u8*)r27 + 0x0);
    r0 = (s8)r0;
    *(u8*)((u8*)r25 + 0x95) = r0;
L_801051FC: ;
    r3 = r25;
    fn_80104CA0();
    fn_80103CB0();
    *(u8*)((u8*)r25 + 0xB) = r3;
    r0 = r29 & 0xFF;
    *(u8*)((u8*)r25 + 0x1B) = r0;
    if ((s32)r30 <= (s32)0x8) goto L_80105220;
    r30 = 0x8;
L_80105220: ;
    r23 = r25;
    r24 = 0x0;
    goto L_80105248;
L_8010522C: ;
    r3 = r31;
    r4 = 0x1;
    __va_arg();
    r0 = *(u32*)((u8*)r3 + 0x0);
    r24 = r24 + 0x1;
    *(u32*)((u8*)r23 + 0x60) = r0;
    r23 = r23 + 0x4;
L_80105248: ;
    if ((s32)r24 < (s32)r30) goto L_8010522C;
    r0 = *(u8*)((u8*)r26 + 0x1);
    /* extrwi r0, r0, 3, 27 */;
    if ((u32)r0 != (u32)0x0) goto L_8010526C;
    r0 = 0x1;
    *(u8*)((u8*)r25 + 0x98) = r0;
    goto L_80105284;
L_8010526C: ;
    r0 = r29 & 0x00000004;
    if ((u32)r0 != (u32)0x0) goto L_80105284;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    *(u32*)((u8*)r3 + 0x4) = r28;
L_80105284: ;
    r3 = (u32)&lbl_80404ACC;
    r3 = (u32)&lbl_80404ACC;
    r0 = *(u32*)((u8*)r3 + 0x4);
    if ((s32)r0 == (s32)r28) goto L_801052A0;
    r0 = 0x1;
    *(u8*)((u8*)r25 + 0x18) = r0;
L_801052A0: ;
    r12 = *(u32*)((u8*)r26 + 0x14);
    if ((u32)r12 == (u32)0x0) goto L_801052B8;
    r3 = r25;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801052B8: ;
    r0 = *(u8*)((u8*)r25 + 0x2);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x0) goto L_801052DC;
    r3 = 0x0;
    r0 = 0x2;
    *(u8*)((u8*)r25 + 0xA) = r3;
    *(u8*)((u8*)r25 + 0x1) = r0;
    *(u8*)((u8*)r25 + 0x2) = r3;
L_801052DC: ;
    r3 = r25;
L_801052E0: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x801052F4 | 0x11C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801052F4(void) {
    extern void fn_800FA444();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r25, 0x14(r1) */;
    r25 = r3;
    r26 = r4;
    r27 = r5;
    r0 = 0x0;
    r31 = 0x280;
    *(u16*)((u8*)r26 + 0x0) = r0;
    r30 = 0x1e0;
    r29 = 0x0;
    r28 = 0x0;
    *(u16*)((u8*)r27 + 0x0) = r0;
L_8010532C: ;
    r3 = *(s16*)((u8*)r25 + 0xA);
    ((void(*)(void))fn_8005D858)();
    r3 = *(u32*)((u8*)r25 + 0x10);
    if ((u32)r3 == (u32)0x0) goto L_80105348;
    fn_800FA444();
    goto L_8010534C;
L_80105348: ;
    r3 = 0x0;
L_8010534C: ;
    r5 = *(s16*)((u8*)r25 + 0x2);
    r4 = (u32)r3 >> 16;
    r0 = r3 & 0xFFFF;
    r3 = (s16)r4;
    r4 = (s16)r0;
    if ((s32)r31 <= (s32)r5) goto L_8010536C;
    r31 = r5;
L_8010536C: ;
    r0 = *(s16*)((u8*)r25 + 0x6);
    r0 = r5 + r0;
    if ((s32)r29 >= (s32)r0) goto L_80105380;
    r29 = r0;
L_80105380: ;
    r0 = r5 + r3;
    if ((s32)r29 >= (s32)r0) goto L_80105390;
    r29 = r0;
L_80105390: ;
    r3 = *(s16*)((u8*)r25 + 0x4);
    if ((s32)r30 <= (s32)r3) goto L_801053A0;
    r30 = r3;
L_801053A0: ;
    r0 = *(s16*)((u8*)r25 + 0x8);
    r0 = r3 + r0;
    if ((s32)r28 >= (s32)r0) goto L_801053B4;
    r28 = r0;
L_801053B4: ;
    r0 = r3 + r4;
    if ((s32)r28 >= (s32)r0) goto L_801053C4;
    r28 = r0;
L_801053C4: ;
    r0 = *(u8*)((u8*)r25 + 0x0);
    /* extrwi r0, r0, 1, 25 */;
    if ((u32)r0 != (u32)0x0) goto L_801053E4;
    r3 = *(s16*)((u8*)r25 + 0x18);
    ((void(*)(void))fn_8005D934)();
    r25 = r3;
    goto L_8010532C;
L_801053E4: ;
    r3 = r29 - r31;
    r0 = r28 - r30;
    r3 = (s16)r3;
    *(u16*)((u8*)r26 + 0x0) = r3;
    r0 = (s16)r0;
    *(u16*)((u8*)r27 + 0x0) = r0;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80105410 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105410(void) {
    extern u8 lbl_80271EC4[];
    extern void fn_80109358();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r3 = (u32)&lbl_80404ACC;
    r4 = 0x0;
    r3 = (u32)&lbl_80404ACC;
    r5 = 0x9c;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = r30 & 0xFFFF;
    r31 = r0 * 0xb4;
    r3 = r31;
    ((void(*)(void))fn_800E3534)();
    r0 = r3 & 0xFFFF;
    r4 = (u32)&lbl_80404ACC;
    r4 = (u32)&lbl_80404ACC;
    *(u16*)((u8*)r4 + 0x2) = r3;
    if ((u32)r0 != (u32)0x0) goto L_80105478;
    r3 = (u32)lbl_80271EC4;
    r3 = (u32)lbl_80271EC4;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    goto L_801054A0;
L_80105478: ;
    r3 = r0;
    ((void(*)(void))fn_800E27B0)();
    r4 = (u32)&lbl_80404ACC;
    r5 = r31;
    r6 = (u32)&lbl_80404ACC;
    r4 = 0x0;
    *(u32*)((u8*)r6 + 0x8) = r3;
    *(u16*)((u8*)r6 + 0x0) = r30;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_80109358();
L_801054A0: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801054B8 | 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801054B8(void) {
    extern u8 lbl_80404AB0[];
    extern u8 lbl_8047CDE8[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r3;
    r0 = *(u32*)lbl_8047CDE8;
    r3 = (u32)lbl_80404AB0;
    r3 = (u32)lbl_80404AB0;
    r4 = 0x0;
    *(u32*)(sp + 0x8) = r0;
    r5 = 0x1a;
    memset((void*)r3, (int)r4, (u32)r5);
    r4 = (u32)&lbl_80404ACC;
    r3 = (u32)lbl_80404AB0;
    r11 = (u32)&lbl_80404ACC;
    r12 = r1 + 0x8;
    r31 = r29 & 0xFF;
    r10 = (u32)lbl_80404AB0;
    r30 = 0x0;
    r0 = 0x2;
    ctr_fn = (void(*)(void))r0;
L_80105514: ;
    r0 = *(u8*)((u8*)r12 + 0x0);
    r29 = r11 + 0x2a;
    r0 = r31 & r0;
    if ((s32)r0 == (s32)0x0) goto L_80105580;
    r3 = (u32)lbl_80404AB0;
    r0 = *(u16*)((u8*)r29 + 0x0);
    r9 = (u32)lbl_80404AB0;
    r5 = *(u16*)((u8*)r10 + 0x2);
    r3 = *(u16*)((u8*)r9 + 0x0);
    r4 = *(u16*)((u8*)r29 + 0x2);
    r8 = r3 | r0;
    r3 = *(u16*)((u8*)r10 + 0x4);
    r0 = *(u16*)((u8*)r29 + 0x4);
    r7 = r5 | r4;
    r5 = *(u16*)((u8*)r10 + 0x6);
    r4 = *(u16*)((u8*)r29 + 0x6);
    r6 = r3 | r0;
    r3 = *(u16*)((u8*)r10 + 0x8);
    r0 = *(u16*)((u8*)r29 + 0x8);
    r4 = r5 | r4;
    *(u16*)((u8*)r9 + 0x0) = r8;
    r0 = r3 | r0;
    *(u16*)((u8*)r10 + 0x2) = r7;
    *(u16*)((u8*)r10 + 0x4) = r6;
    *(u16*)((u8*)r10 + 0x6) = r4;
    *(u16*)((u8*)r10 + 0x8) = r0;
L_80105580: ;
    r11 = r11 + 0x1a;
    r0 = *(u8*)((u8*)r12 + 0x1);
    r29 = r11 + 0x2a;
    r0 = r31 & r0;
    if ((s32)r0 == (s32)0x0) goto L_801055F0;
    r3 = (u32)lbl_80404AB0;
    r0 = *(u16*)((u8*)r29 + 0x0);
    r9 = (u32)lbl_80404AB0;
    r5 = *(u16*)((u8*)r10 + 0x2);
    r3 = *(u16*)((u8*)r9 + 0x0);
    r4 = *(u16*)((u8*)r29 + 0x2);
    r8 = r3 | r0;
    r3 = *(u16*)((u8*)r10 + 0x4);
    r0 = *(u16*)((u8*)r29 + 0x4);
    r7 = r5 | r4;
    r5 = *(u16*)((u8*)r10 + 0x6);
    r4 = *(u16*)((u8*)r29 + 0x6);
    r6 = r3 | r0;
    r3 = *(u16*)((u8*)r10 + 0x8);
    r0 = *(u16*)((u8*)r29 + 0x8);
    r4 = r5 | r4;
    *(u16*)((u8*)r9 + 0x0) = r8;
    r0 = r3 | r0;
    *(u16*)((u8*)r10 + 0x2) = r7;
    *(u16*)((u8*)r10 + 0x4) = r6;
    *(u16*)((u8*)r10 + 0x6) = r4;
    *(u16*)((u8*)r10 + 0x8) = r0;
L_801055F0: ;
    r11 = r11 + 0x1a;
    r12 = r12 + 0x2;
    r30 = r30 + 0x1;
    if (--ctr != 0) goto L_80105514;
    r3 = (u32)lbl_80404AB0;
    r3 = (u32)lbl_80404AB0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern u8 lbl_80314E08[];
    extern u8 lbl_8047CDF0[];
    extern u8 lbl_8047CDF4[];
    extern u8 lbl_8047CDF8[];
    extern u8 lbl_8047CDFC[];
    extern u8 lbl_8047CE00[];
    extern u8 lbl_8047CE08[];
    extern u8 lbl_8047CE10[];
    extern u8 lbl_8047CE18[];
    extern void fn_800CE318();
    extern void fn_800D5648();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800FE35C();
    extern void fn_800FE38C();
    extern u8 jumptable_8035B088[];
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    *(f64*)(sp + 0x40) = f31;
    /* psq_st f31, 0x48(r1), 0, qr0 */;
    r31 = r4;
    r6 = *(u8*)((u8*)r3 + 0x8B);
    r5 = (0x8102 << 16);
    r0 = *(u8*)((u8*)r31 + 0x67);
    r4 = (0x4330 << 16);
    r3 = *(s16*)((u8*)r31 + 0x6);
    /* subi r7, r5, 0x7dfd */;
    r5 = r6 * r0;
    r8 = *(u32*)lbl_8047CDF0;
    /* subi r0, r3, 0x68 */;
    r6 = *(u32*)lbl_8047CDF4;
    f1 = *(f64*)lbl_8047CE10;
    r3 = (s32)((s64)r7 * (s64)r5 >> 32);
    r3 = r3 + r5;
    r3 = (s32)r3 >> 15;
    r4 = (u32)r3 >> 31;
    r3 = r3 + r4;
    /* xoris r3, r3, 0x8000 */;
    f0 = *(f64*)(sp + 0x10);
    f31 = f0 - f1;
    if ((u32)r0 > (u32)0x1b) goto L_801058A8;
    r3 = (u32)jumptable_8035B088;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035B088;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r5 = *(u8*)(sp + 0xB);
    r4 = (0x4330 << 16);
    r0 = *(u8*)(sp + 0xF);
    r3 = 0x1;
    f2 = *(f64*)lbl_8047CE18;
    f0 = *(f64*)(sp + 0x10);
    *(u32*)(sp + 0x24) = r0;
    f1 = f0 - f2;
    f0 = *(f64*)(sp + 0x20);
    f1 = f1 * f31;
    f0 = f0 - f2;
    f1 = (f64)(s32)f1;
    f0 = f0 * f31;
    *(f64*)(sp + 0x18) = f1;
    f0 = (f64)(s32)f0;
    r0 = *(u32*)(sp + 0x1C);
    *(u8*)(sp + 0xB) = r0;
    *(f64*)(sp + 0x28) = f0;
    r0 = *(u32*)(sp + 0x2C);
    *(u8*)(sp + 0xF) = r0;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    r3 = 0x6;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    r3 = 0x4;
    fn_800D67BC();
    r3 = 0x0;
    r4 = 0x0;
    fn_800D61E4();
    r4 = *(u32*)(sp + 0x8);
    r3 = 0x0;
    fn_800D5BA0();
    r3 = *(s16*)((u8*)r31 + 0x54);
    r4 = 0x0;
    fn_800D61E4();
    r4 = *(u32*)(sp + 0x8);
    r3 = 0x0;
    fn_800D5BA0();
    r3 = *(s16*)((u8*)r31 + 0x54);
    r4 = *(s16*)((u8*)r31 + 0x56);
    fn_800D61E4();
    r4 = *(u32*)(sp + 0xC);
    r3 = 0x0;
    fn_800D5BA0();
    r4 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    fn_800D61E4();
    r4 = *(u32*)(sp + 0xC);
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r3 = 0x0;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r4 = 0x0;
    fn_800FE38C();
    goto L_801058A8;
    r3 = 0x1;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x6;
    ((void(*)(void))fn_800D888C)();
    f1 = *(f32*)lbl_8047CDF8;
    fn_800D5648();
    r3 = 0x1;
    fn_800D6A00();
    r3 = (u32)lbl_80314E08;
    r3 = (u32)lbl_80314E08;
    fn_800D7820();
    f0 = *(f32*)lbl_8047CDFC;
    r0 = 0xff;
    *(u8*)(sp + 0x8) = r0;
    r30 = 0x0;
    f0 = f0 * f31;
    *(u8*)(sp + 0x9) = r0;
    f0 = (f64)(s32)f0;
    *(u8*)(sp + 0xA) = r0;
    *(f64*)(sp + 0x28) = f0;
    r0 = *(u32*)(sp + 0x2C);
    *(u8*)(sp + 0xB) = r0;
    goto L_8010586C;
L_8010582C: ;
    r3 = 0x2;
    fn_800D67BC();
    r4 = r30;
    r3 = 0x0;
    fn_800D61E4();
    r4 = *(u32*)(sp + 0x8);
    r3 = 0x0;
    fn_800D5BA0();
    r3 = *(s16*)((u8*)r31 + 0x54);
    r4 = r30;
    fn_800D61E4();
    r4 = *(u32*)(sp + 0x8);
    r3 = 0x0;
    fn_800D5BA0();
    fn_800D6728();
    r30 = r30 + 0x4;
L_8010586C: ;
    r0 = *(s16*)((u8*)r31 + 0x56);
    r3 = (s16)r30;
    if ((s32)r3 < (s32)r0) goto L_8010582C;
    fn_800FE35C();
    goto L_801058A8;
    f1 = *(f32*)((u8*)r31 + 0x70);
    f0 = *(f32*)lbl_8047CE00;
    f2 = *(f64*)lbl_8047CE08;
    f0 = f1 + f0;
    *(f32*)((u8*)r31 + 0x70) = f0;
    f1 = *(f32*)((u8*)r31 + 0x70);
    fn_800CE318();
    f0 = (f32)f1;
    *(f32*)((u8*)r31 + 0x70) = f0;
L_801058A8: ;
    r3 = 0x0;
    /* psq_l f31, 0x48(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x40);
    r31 = *(u32*)(sp + 0x3C);
    r30 = *(u32*)(sp + 0x38);
    return;
}
#pragma pop

/* 0x801058CC | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801058CC(void) {
    extern void fn_8001EC08();
    extern void fn_800FBE7C();
    extern void fn_800FE4D4();
    extern void fn_800FE6D0();
    extern void fn_801040A0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* stmw r27, 0xc(r1) */;
    r30 = r3;
    r27 = r4;
    fn_801040A0();
    r0 = *(u32*)((u8*)r30 + 0x4);
    r31 = r3;
    if ((s32)r0 != (s32)0x50) goto L_80105904;
    r29 = *(s16*)((u8*)r31 + 0xC);
    r28 = *(s16*)((u8*)r31 + 0xE);
    goto L_80105914;
L_80105904: ;
    r3 = *(s16*)((u8*)r27 + 0x54);
    r0 = *(s16*)((u8*)r27 + 0x56);
    r29 = r3;
    r28 = r0;
L_80105914: ;
    r3 = *(s16*)((u8*)r27 + 0x56);
    r0 = *(s16*)((u8*)r27 + 0x52);
    /* subi r3, r3, 0x4 */;
    r6 = *(s16*)((u8*)r30 + 0x84);
    r3 = r3 - r28;
    r5 = *(s16*)((u8*)r27 + 0x50);
    r4 = (s16)r3;
    r3 = *(s16*)((u8*)r30 + 0x86);
    r0 = r0 + r4;
    r4 = r6 + r5;
    r0 = r3 + r0;
    r3 = (s16)r4;
    r4 = (s16)r0;
    fn_800FE6D0();
    fn_800FE4D4();
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((s32)r0 >= (s32)0x52) goto L_80105974;
    if ((s32)r0 == (s32)0x40) goto L_80105980;
    if ((s32)r0 < (s32)0x40) goto L_801059BC;
    if ((s32)r0 >= (s32)0x50) goto L_80105980;
    goto L_801059BC;
L_80105974: ;
    if ((s32)r0 == (s32)0x10c) goto L_801059A0;
    goto L_801059BC;
L_80105980: ;
    r7 = *(u8*)((u8*)r30 + 0x8B);
    r5 = (s16)r29;
    r6 = (s16)r28;
    r3 = 0x0;
    r4 = 0x2;
    r8 = 0x1;
    fn_8001EC08();
    goto L_801059BC;
L_801059A0: ;
    r7 = *(u8*)((u8*)r30 + 0x8B);
    r5 = (s16)r29;
    r6 = (s16)r28;
    r3 = 0x0;
    r4 = 0x2;
    r8 = 0x0;
    fn_8001EC08();
L_801059BC: ;
    r0 = *(u8*)((u8*)r30 + 0x1);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x3) goto L_80105A24;
    if ((s32)r0 >= (s32)0x3) goto L_801059E0;
    if ((s32)r0 >= (s32)0x2) goto L_801059EC;
    goto L_80105A24;
L_801059E0: ;
    if ((s32)r0 == (s32)0x5) goto L_80105A24;
    goto L_80105A24;
L_801059EC: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((s32)r0 == (s32)0x51) goto L_80105A24;
    r0 = *(u8*)((u8*)r30 + 0x0);
    r3 = *(u32*)((u8*)r31 + 0x0);
    r0 = (s8)r0;
    r4 = *(u32*)((u8*)r31 + 0x8);
    r0 = r0 & 0x00000004;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r5 = r0 & 0xFF;
    fn_800FBE7C();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x8) = r0;
L_80105A24: ;
    r3 = 0x0;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* 0x80105A3C | 0x1F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105A3C(void) {
    extern void fn_800FA3D0();
    extern void fn_800FA444();
    extern void fn_800FBD88();
    extern void fn_800FBF74();
    extern void fn_801040A0();
    extern void fn_801040D0();
    extern void fn_80107ED8();
    extern void fn_801080CC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    fn_801040A0();
    r0 = *(u8*)((u8*)r30 + 0x1);
    r31 = r3;
    r29 = 0x0;
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x3) goto L_80105BD8;
    if ((s32)r0 >= (s32)0x3) goto L_80105A90;
    if ((s32)r0 == (s32)0x1) goto L_80105C10;
    if ((s32)r0 >= (s32)0x1) goto L_80105B88;
    if ((s32)r0 >= (s32)0x0) goto L_80105A9C;
    goto L_80105C10;
L_80105A90: ;
    if ((s32)r0 == (s32)0x5) goto L_80105C00;
    goto L_80105C10;
L_80105A9C: ;
    r3 = *(u32*)((u8*)r31 + 0x0);
    if ((u32)r3 == (u32)0x0) goto L_80105AAC;
    fn_800FBD88();
L_80105AAC: ;
    r3 = r30;
    r4 = 0x0;
    fn_801040D0();
    *(u32*)((u8*)r31 + 0x0) = r3;
    r3 = r30;
    r4 = 0x1;
    fn_801040D0();
    r0 = r3 & 0xFF;
    r3 = r30;
    *(u8*)((u8*)r31 + 0x4) = r0;
    r4 = 0x2;
    fn_801040D0();
    r0 = r3 & 0xFF;
    *(u8*)((u8*)r31 + 0x5) = r0;
    r0 = *(u8*)((u8*)r30 + 0x2);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_80105AF8;
    r29 = 0x1;
L_80105AF8: ;
    r3 = *(u32*)((u8*)r30 + 0x4);
    r4 = 0x2a;
    fn_80107ED8();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80105B14;
    r29 = 0x1;
L_80105B14: ;
    r0 = *(u8*)((u8*)r30 + 0xA);
    if ((u32)r0 == (u32)0x0) goto L_80105B24;
    r29 = 0x1;
L_80105B24: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80105B58;
    r3 = *(u32*)((u8*)r30 + 0x4);
    if ((s32)r3 == (s32)0x40) goto L_80105B48;
    r4 = 0x22;
    fn_801080CC();
    goto L_80105B50;
L_80105B48: ;
    r4 = 0x26;
    fn_801080CC();
L_80105B50: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x2) = r0;
L_80105B58: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    if ((s32)r0 != (s32)0x50) goto L_80105C10;
    r3 = *(u32*)((u8*)r31 + 0x0);
    fn_800FA444();
    r4 = (u32)r3 >> 16;
    r0 = r3 & 0xFFFF;
    r3 = (s16)r4;
    *(u16*)((u8*)r31 + 0xC) = r3;
    r0 = (s16)r0;
    *(u16*)((u8*)r31 + 0xE) = r0;
    goto L_80105C10;
L_80105B88: ;
    r0 = *(u8*)((u8*)r30 + 0x2);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_80105BB0;
    r3 = *(u32*)((u8*)r31 + 0x0);
    r4 = *(u8*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r31 + 0x5);
    fn_800FBF74();
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x2) = r0;
L_80105BB0: ;
    r3 = *(u32*)((u8*)r31 + 0x0);
    fn_800FA3D0();
    if ((s32)r3 == (s32)0x0) goto L_80105BCC;
    r0 = 0x0;
    *(u8*)((u8*)r30 + 0x98) = r0;
    goto L_80105C10;
L_80105BCC: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x98) = r0;
    goto L_80105C10;
L_80105BD8: ;
    r0 = *(u8*)((u8*)r30 + 0x2);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_80105C10;
    r3 = *(u32*)((u8*)r30 + 0x4);
    r4 = 0x2a;
    fn_801080CC();
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x2) = r0;
    goto L_80105C10;
L_80105C00: ;
    r3 = *(u32*)((u8*)r31 + 0x0);
    fn_800FBD88();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x0) = r0;
L_80105C10: ;
    r3 = 0x0;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80105C30 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105C30(void) {
    extern void fn_801040A0();
    extern void fn_80105624();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    fn_801040A0();
    r31 = r3;
    fn_80105624();
    r0 = *(u16*)((u8*)r3 + 0x4);
    *(u32*)((u8*)r31 + 0x8) = r0;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80105C68 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105C68(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern u8 jumptable_8035B0F8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80105D30;
    r0 = *(u8*)lbl_80478B30;
    r31 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_80105D04;
    r3 = (u32)jumptable_8035B0F8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035B0F8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_80105D04;
    r31 = 0xf;
    goto L_80105D04;
    r31 = 0x10;
    goto L_80105D04;
    r31 = 0x40;
    goto L_80105D04;
    r31 = 0x50;
    goto L_80105D04;
    r31 = 0x51;
    goto L_80105D04;
    r31 = 0x10c;
    goto L_80105D04;
    r31 = 0xe6;
    goto L_80105D04;
    r31 = 0xe8;
    goto L_80105D04;
    r31 = 0x107;
L_80105D04: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80105D28;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80105D28: ;
    r0 = -0x1;
    *(u8*)lbl_80478B30 = r0;
L_80105D30: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80105D48 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105D48(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B120[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    r30 = r4;
    r5 = 0x107;
    if ((s32)r5 == (s32)0x0) goto L_80105E60;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x9) goto L_80105E24;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80105E24;
    r31 = 0x0;
    if ((u32)r4 > (u32)0x9) goto L_80105E00;
    r3 = (u32)jumptable_8035B120;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B120;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_80105E00;
    r31 = 0xf;
    goto L_80105E00;
    r31 = 0x10;
    goto L_80105E00;
    r31 = 0x40;
    goto L_80105E00;
    r31 = 0x50;
    goto L_80105E00;
    r31 = 0x51;
    goto L_80105E00;
    r31 = 0x10c;
    goto L_80105E00;
    r31 = 0xe6;
    goto L_80105E00;
    r31 = 0xe8;
    goto L_80105E00;
    r31 = r5;
L_80105E00: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80105E24;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80105E24: ;
    r0 = 0x9;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = 0x0;
    r4 = r3;
    *(u32*)(sp + 0x8) = r0;
    r7 = r30;
    r9 = r29;
    r3 = 0x107;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    r10 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80105E60: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* 0x80105E7C | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80105E7C(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B148[];
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    r30 = r4;
    r5 = 0x51;
    if ((s32)r5 == (s32)0x0) goto L_80105F94;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x5) goto L_80105F58;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80105F58;
    r31 = 0x0;
    if ((u32)r4 > (u32)0x9) goto L_80105F34;
    r3 = (u32)jumptable_8035B148;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B148;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_80105F34;
    r31 = 0xf;
    goto L_80105F34;
    r31 = 0x10;
    goto L_80105F34;
    r31 = 0x40;
    goto L_80105F34;
    r31 = 0x50;
    goto L_80105F34;
    r31 = r5;
    goto L_80105F34;
    r31 = 0x10c;
    goto L_80105F34;
    r31 = 0xe6;
    goto L_80105F34;
    r31 = 0xe8;
    goto L_80105F34;
    r31 = 0x107;
L_80105F34: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80105F58;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80105F58: ;
    r0 = 0x5;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = 0x0;
    r4 = r3;
    *(u32*)(sp + 0x8) = r0;
    r7 = r30;
    r9 = r29;
    r3 = 0x51;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    r10 = 0x1;
    /* crclr cr1eq */;
    fn_801026A4();
L_80105F94: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    extern u8 lbl_8047AD10[];
    extern void fn_801026A4();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r0 = 0x10c;
    r6 = 0x0;
    if ((s32)r0 != (s32)0x0) goto L_80106018;
    goto L_8010606C;
L_80106018: ;
    r0 = r5 & 0xFF;
    r4 = 0x1;
    *(u8*)lbl_8047AD10 = r4;
    if ((u32)r0 == (u32)0x0) goto L_80106034;
    r0 = r6 | 0x1;
    r6 = r0 & 0xFF;
L_80106034: ;
    r4 = 0x0;
    r0 = r6 | 0x2;
    r9 = r3;
    r10 = r0 & 0xFF;
    r3 = 0x10c;
    r4 = -0x1;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r6 = r3;
L_8010606C: ;
    r3 = r6;
    return;
}
#pragma pop

/* 0x80106080 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106080(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern u8 jumptable_8035B170[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106148;
    r0 = *(u8*)lbl_80478B30;
    r31 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_8010611C;
    r3 = (u32)jumptable_8035B170;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035B170;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_8010611C;
    r31 = 0xf;
    goto L_8010611C;
    r31 = 0x10;
    goto L_8010611C;
    r31 = 0x40;
    goto L_8010611C;
    r31 = 0x50;
    goto L_8010611C;
    r31 = 0x51;
    goto L_8010611C;
    r31 = 0x10c;
    goto L_8010611C;
    r31 = 0xe6;
    goto L_8010611C;
    r31 = 0xe8;
    goto L_8010611C;
    r31 = 0x107;
L_8010611C: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106140;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80106140: ;
    r0 = -0x1;
    *(u8*)lbl_80478B30 = r0;
L_80106148: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80106160 | 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106160(void) {
    extern u8 lbl_80478B30[];
    extern void fn_80104704();
    extern u8 jumptable_8035B198[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = *(u8*)lbl_80478B30;
    r3 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_801061E4;
    r4 = (u32)jumptable_8035B198;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035B198;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x40;
    goto L_801061E4;
    r3 = 0xf;
    goto L_801061E4;
    r3 = 0x10;
    goto L_801061E4;
    r3 = 0x40;
    goto L_801061E4;
    r3 = 0x50;
    goto L_801061E4;
    r3 = 0x51;
    goto L_801061E4;
    r3 = 0x10c;
    goto L_801061E4;
    r3 = 0xe6;
    goto L_801061E4;
    r3 = 0xe8;
    goto L_801061E4;
    r3 = 0x107;
L_801061E4: ;
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_801061F8;
    r0 = -0x1;
    goto L_80106224;
L_801061F8: ;
    r0 = *(u8*)((u8*)r3 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_8010620C;
    r0 = 0x0;
    goto L_80106224;
L_8010620C: ;
    r0 = *(u8*)((u8*)r3 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_80106220;
    r0 = 0x0;
    goto L_80106224;
L_80106220: ;
    r0 = 0x1;
L_80106224: ;
    r0 = (s8)r0;
    r0 = (u32)r0 >> 31;
    r0 = r0 ^ 0x1;
    r3 = r0 & 0xFF;
    return;
}
#pragma pop

/* 0x80106244 | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106244(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B1C0[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0x1c(r1) */;
    r30 = r3;
    r31 = r4;
    r27 = r5;
    r5 = 0x50;
    r29 = 0x0;
    if ((s32)r5 != (s32)0x0) goto L_80106274;
    goto L_8010637C;
L_80106274: ;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x4) goto L_80106324;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106324;
    r28 = r29;
    if ((u32)r4 > (u32)0x9) goto L_80106300;
    r3 = (u32)jumptable_8035B1C0;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B1C0;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r28 = 0x40;
    goto L_80106300;
    r28 = 0xf;
    goto L_80106300;
    r28 = 0x10;
    goto L_80106300;
    r28 = 0x40;
    goto L_80106300;
    r28 = r5;
    goto L_80106300;
    r28 = 0x51;
    goto L_80106300;
    r28 = 0x10c;
    goto L_80106300;
    r28 = 0xe6;
    goto L_80106300;
    r28 = 0xe8;
    goto L_80106300;
    r28 = 0x107;
L_80106300: ;
    r3 = r28;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106324;
    r3 = r28;
    r5 = r31;
    r4 = 0x2;
    fn_80102568();
L_80106324: ;
    r0 = 0x4;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106344;
    r0 = r29 | 0x1;
    r29 = r0 & 0xFF;
L_80106344: ;
    r4 = 0x0;
    r0 = r29 | 0x2;
    r4 = r3;
    r10 = r0 & 0xFF;
    r7 = r31;
    r9 = r30;
    r3 = 0x50;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r29 = r3;
L_8010637C: ;
    r3 = r29;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x80106394 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106394(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B1E8[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0x1c(r1) */;
    r30 = r3;
    r31 = r4;
    r27 = r5;
    r5 = 0x50;
    r29 = 0x0;
    if ((s32)r5 != (s32)0x0) goto L_801063C4;
    goto L_801064C8;
L_801063C4: ;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x4) goto L_80106474;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106474;
    r28 = r29;
    if ((u32)r4 > (u32)0x9) goto L_80106450;
    r3 = (u32)jumptable_8035B1E8;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B1E8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r28 = 0x40;
    goto L_80106450;
    r28 = 0xf;
    goto L_80106450;
    r28 = 0x10;
    goto L_80106450;
    r28 = 0x40;
    goto L_80106450;
    r28 = r5;
    goto L_80106450;
    r28 = 0x51;
    goto L_80106450;
    r28 = 0x10c;
    goto L_80106450;
    r28 = 0xe6;
    goto L_80106450;
    r28 = 0xe8;
    goto L_80106450;
    r28 = 0x107;
L_80106450: ;
    r3 = r28;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106474;
    r3 = r28;
    r5 = r31;
    r4 = 0x2;
    fn_80102568();
L_80106474: ;
    r0 = 0x4;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106494;
    r0 = r29 | 0x1;
    r29 = r0 & 0xFF;
L_80106494: ;
    r0 = 0x0;
    r4 = r3;
    *(u32*)(sp + 0x8) = r0;
    r7 = r31;
    r9 = r30;
    r10 = r29 & 0xFF;
    r3 = 0x50;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r29 = r3;
L_801064C8: ;
    r3 = r29;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x801064E0 | 0xD8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801064E0(void) {
    extern u8 lbl_80478B30[];
    extern void fn_80104704();
    extern u8 jumptable_8035B210[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = *(u8*)lbl_80478B30;
    r3 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_80106564;
    r4 = (u32)jumptable_8035B210;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035B210;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x40;
    goto L_80106564;
    r3 = 0xf;
    goto L_80106564;
    r3 = 0x10;
    goto L_80106564;
    r3 = 0x40;
    goto L_80106564;
    r3 = 0x50;
    goto L_80106564;
    r3 = 0x51;
    goto L_80106564;
    r3 = 0x10c;
    goto L_80106564;
    r3 = 0xe6;
    goto L_80106564;
    r3 = 0xe8;
    goto L_80106564;
    r3 = 0x107;
L_80106564: ;
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_80106578;
    r0 = -0x1;
    goto L_801065A4;
L_80106578: ;
    r0 = *(u8*)((u8*)r3 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_8010658C;
    r0 = 0x0;
    goto L_801065A4;
L_8010658C: ;
    r0 = *(u8*)((u8*)r3 + 0x99);
    if ((u32)r0 == (u32)0x0) goto L_801065A0;
    r0 = 0x0;
    goto L_801065A4;
L_801065A0: ;
    r0 = 0x1;
L_801065A4: ;
    r3 = (s8)r0;
    return;
}
#pragma pop

/* 0x801065B8 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801065B8(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern u8 jumptable_8035B238[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106680;
    r0 = *(u8*)lbl_80478B30;
    r31 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_80106654;
    r3 = (u32)jumptable_8035B238;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035B238;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_80106654;
    r31 = 0xf;
    goto L_80106654;
    r31 = 0x10;
    goto L_80106654;
    r31 = 0x40;
    goto L_80106654;
    r31 = 0x50;
    goto L_80106654;
    r31 = 0x51;
    goto L_80106654;
    r31 = 0x10c;
    goto L_80106654;
    r31 = 0xe6;
    goto L_80106654;
    r31 = 0xe8;
    goto L_80106654;
    r31 = 0x107;
L_80106654: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106678;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80106678: ;
    r0 = -0x1;
    *(u8*)lbl_80478B30 = r0;
L_80106680: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80106698 | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106698(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B260[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r26, 0x18(r1) */;
    r29 = r3;
    r30 = r4;
    r26 = r5;
    r31 = r6;
    r5 = 0x40;
    r28 = 0x0;
    if ((s32)r5 != (s32)0x0) goto L_801066CC;
    goto L_801067D0;
L_801066CC: ;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x3) goto L_8010677C;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_8010677C;
    r27 = r28;
    if ((u32)r4 > (u32)0x9) goto L_80106758;
    r3 = (u32)jumptable_8035B260;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B260;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r27 = r5;
    goto L_80106758;
    r27 = 0xf;
    goto L_80106758;
    r27 = 0x10;
    goto L_80106758;
    r27 = r5;
    goto L_80106758;
    r27 = 0x50;
    goto L_80106758;
    r27 = 0x51;
    goto L_80106758;
    r27 = 0x10c;
    goto L_80106758;
    r27 = 0xe6;
    goto L_80106758;
    r27 = 0xe8;
    goto L_80106758;
    r27 = 0x107;
L_80106758: ;
    r3 = r27;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010677C;
    r3 = r27;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_8010677C: ;
    r0 = 0x3;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010679C;
    r0 = r28 | 0x1;
    r28 = r0 & 0xFF;
L_8010679C: ;
    r0 = r31 & 0xFF;
    r4 = r3;
    *(u32*)(sp + 0x8) = r0;
    r7 = r30;
    r9 = r29;
    r10 = r28 & 0xFF;
    r3 = 0x40;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r28 = r3;
L_801067D0: ;
    r3 = r28;
    /* lmw r26, 0x18(r1) */;
    return;
}
#pragma pop

/* 0x801067E8 | 0x14C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801067E8(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B288[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0x1c(r1) */;
    r30 = r3;
    r31 = r4;
    r27 = r5;
    r5 = 0x40;
    r29 = 0x0;
    if ((s32)r5 != (s32)0x0) goto L_80106818;
    goto L_8010691C;
L_80106818: ;
    r0 = *(u8*)lbl_80478B30;
    r4 = (s8)r0;
    if ((s32)r4 == (s32)0x3) goto L_801068C8;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_801068C8;
    r28 = r29;
    if ((u32)r4 > (u32)0x9) goto L_801068A4;
    r3 = (u32)jumptable_8035B288;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B288;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r28 = r5;
    goto L_801068A4;
    r28 = 0xf;
    goto L_801068A4;
    r28 = 0x10;
    goto L_801068A4;
    r28 = r5;
    goto L_801068A4;
    r28 = 0x50;
    goto L_801068A4;
    r28 = 0x51;
    goto L_801068A4;
    r28 = 0x10c;
    goto L_801068A4;
    r28 = 0xe6;
    goto L_801068A4;
    r28 = 0xe8;
    goto L_801068A4;
    r28 = 0x107;
L_801068A4: ;
    r3 = r28;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801068C8;
    r3 = r28;
    r5 = r31;
    r4 = 0x2;
    fn_80102568();
L_801068C8: ;
    r0 = 0x3;
    *(u8*)lbl_80478B30 = r0;
    fn_801046B8();
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_801068E8;
    r0 = r29 | 0x1;
    r29 = r0 & 0xFF;
L_801068E8: ;
    r0 = 0x0;
    r4 = r3;
    *(u32*)(sp + 0x8) = r0;
    r7 = r31;
    r9 = r30;
    r10 = r29 & 0xFF;
    r3 = 0x40;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
    r29 = r3;
L_8010691C: ;
    r3 = r29;
    /* lmw r27, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x80106934 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106934(void) {
    extern u8 lbl_80478B30[];
    extern void fn_80104704();
    extern u8 jumptable_8035B2B0[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;

    r0 = *(u8*)lbl_80478B30;
    r3 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_801069B8;
    r4 = (u32)jumptable_8035B2B0;
    r0 = r0 << 2;
    r4 = (u32)jumptable_8035B2B0;
    r0 = *(u32*)(r4 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x40;
    goto L_801069B8;
    r3 = 0xf;
    goto L_801069B8;
    r3 = 0x10;
    goto L_801069B8;
    r3 = 0x40;
    goto L_801069B8;
    r3 = 0x50;
    goto L_801069B8;
    r3 = 0x51;
    goto L_801069B8;
    r3 = 0x10c;
    goto L_801069B8;
    r3 = 0xe6;
    goto L_801069B8;
    r3 = 0xe8;
    goto L_801069B8;
    r3 = 0x107;
L_801069B8: ;
    fn_80104704();
    if ((u32)r3 != (u32)0x0) goto L_801069CC;
    r3 = -0x1;
    goto L_801069EC;
L_801069CC: ;
    r0 = *(u8*)((u8*)r3 + 0x98);
    if ((u32)r0 == (u32)0x0) goto L_801069E0;
    r3 = 0x0;
    goto L_801069EC;
L_801069E0: ;
    r0 = *(u8*)((u8*)r3 + 0x99);
    r0 = __cntlzw(r0);
    r3 = (u32)r0 >> 5;
L_801069EC: ;
    return;
}
#pragma pop

/* 0x801069FC | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801069FC(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern u8 jumptable_8035B2D8[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106AC4;
    r0 = *(u8*)lbl_80478B30;
    r31 = 0x0;
    r0 = (s8)r0;
    if ((u32)r0 > (u32)0x9) goto L_80106A98;
    r3 = (u32)jumptable_8035B2D8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_8035B2D8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r31 = 0x40;
    goto L_80106A98;
    r31 = 0xf;
    goto L_80106A98;
    r31 = 0x10;
    goto L_80106A98;
    r31 = 0x40;
    goto L_80106A98;
    r31 = 0x50;
    goto L_80106A98;
    r31 = 0x51;
    goto L_80106A98;
    r31 = 0x10c;
    goto L_80106A98;
    r31 = 0xe6;
    goto L_80106A98;
    r31 = 0xe8;
    goto L_80106A98;
    r31 = 0x107;
L_80106A98: ;
    r3 = r31;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106ABC;
    r3 = r31;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80106ABC: ;
    r0 = -0x1;
    *(u8*)lbl_80478B30 = r0;
L_80106AC4: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80106ADC | 0x260 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106ADC(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B300[];
    extern u8 jumptable_8035B328[];
    extern u8 jumptable_8035B350[];
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r23, 0x1c(r1) */;
    r24 = r3;
    r25 = r4;
    r30 = r5;
    r26 = r6;
    r27 = r7;
    r28 = (s8)r24;
    r29 = 0x0;
    r31 = r29;
    r4 = r29;
    if ((u32)r28 > (u32)0x9) goto L_80106B7C;
    r3 = (u32)jumptable_8035B350;
    r0 = r28 << 2;
    r3 = (u32)jumptable_8035B350;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = 0x40;
    goto L_80106B7C;
    r4 = 0xf;
    goto L_80106B7C;
    r4 = 0x10;
    goto L_80106B7C;
    r4 = 0x40;
    goto L_80106B7C;
    r4 = 0x50;
    goto L_80106B7C;
    r4 = 0x51;
    goto L_80106B7C;
    r4 = 0x10c;
    goto L_80106B7C;
    r4 = 0xe6;
    goto L_80106B7C;
    r4 = 0xe8;
    goto L_80106B7C;
    r4 = 0x107;
L_80106B7C: ;
    if ((s32)r4 != (s32)0x0) goto L_80106B8C;
    r3 = 0x0;
    goto L_80106D28;
L_80106B8C: ;
    r0 = (s8)r24;
    if ((s32)r0 == (s32)0x6) goto L_80106C58;
    r3 = *(u8*)lbl_80478B30;
    r4 = (s8)r3;
    if ((s32)r0 == (s32)r4) goto L_80106C48;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106C48;
    r23 = 0x0;
    if ((u32)r4 > (u32)0x9) goto L_80106C24;
    r3 = (u32)jumptable_8035B328;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B328;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r23 = 0x40;
    goto L_80106C24;
    r23 = 0xf;
    goto L_80106C24;
    r23 = 0x10;
    goto L_80106C24;
    r23 = 0x40;
    goto L_80106C24;
    r23 = 0x50;
    goto L_80106C24;
    r23 = 0x51;
    goto L_80106C24;
    r23 = 0x10c;
    goto L_80106C24;
    r23 = 0xe6;
    goto L_80106C24;
    r23 = 0xe8;
    goto L_80106C24;
    r23 = 0x107;
L_80106C24: ;
    r3 = r23;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106C48;
    r3 = r23;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80106C48: ;
    *(u8*)lbl_80478B30 = r24;
    fn_801046B8();
    r4 = r3;
    goto L_80106C68;
L_80106C58: ;
    r29 = 0x1;
    r4 = -0x1;
    *(u8*)lbl_8047AD10 = r29;
    r30 = 0x0;
L_80106C68: ;
    r0 = r26 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106C7C;
    r0 = r31 | 0x1;
    r31 = r0 & 0xFF;
L_80106C7C: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106C90;
    r0 = r31 | 0x2;
    r31 = r0 & 0xFF;
L_80106C90: ;
    r3 = 0x0;
    if ((u32)r28 > (u32)0x9) goto L_80106D00;
    r5 = (u32)jumptable_8035B300;
    r0 = r28 << 2;
    r5 = (u32)jumptable_8035B300;
    r0 = *(u32*)(r5 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x40;
    goto L_80106D00;
    r3 = 0xf;
    goto L_80106D00;
    r3 = 0x10;
    goto L_80106D00;
    r3 = 0x40;
    goto L_80106D00;
    r3 = 0x50;
    goto L_80106D00;
    r3 = 0x51;
    goto L_80106D00;
    r3 = 0x10c;
    goto L_80106D00;
    r3 = 0xe6;
    goto L_80106D00;
    r3 = 0xe8;
    goto L_80106D00;
    r3 = 0x107;
L_80106D00: ;
    r0 = r27 & 0xFF;
    r7 = r30;
    *(u32*)(sp + 0x8) = r0;
    r9 = r25;
    r10 = r31 & 0xFF;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
L_80106D28: ;
    /* lmw r23, 0x1c(r1) */;
    return;
}
#pragma pop

/* 0x80106D3C | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106D3C(void) {
    extern u8 lbl_80478B30[];
    extern u8 lbl_8047AD10[];
    extern void fn_80102568();
    extern void fn_80102620();
    extern void fn_801026A4();
    extern void fn_801046B8();
    extern u8 jumptable_8035B378[];
    extern u8 jumptable_8035B3A0[];
    extern u8 jumptable_8035B3C8[];
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r24, 0x10(r1) */;
    r25 = r3;
    r26 = r4;
    r30 = r5;
    r27 = r6;
    r28 = (s8)r25;
    r29 = 0x0;
    r31 = r29;
    r4 = r29;
    if ((u32)r28 > (u32)0x9) goto L_80106DD8;
    r3 = (u32)jumptable_8035B3C8;
    r0 = r28 << 2;
    r3 = (u32)jumptable_8035B3C8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r4 = 0x40;
    goto L_80106DD8;
    r4 = 0xf;
    goto L_80106DD8;
    r4 = 0x10;
    goto L_80106DD8;
    r4 = 0x40;
    goto L_80106DD8;
    r4 = 0x50;
    goto L_80106DD8;
    r4 = 0x51;
    goto L_80106DD8;
    r4 = 0x10c;
    goto L_80106DD8;
    r4 = 0xe6;
    goto L_80106DD8;
    r4 = 0xe8;
    goto L_80106DD8;
    r4 = 0x107;
L_80106DD8: ;
    if ((s32)r4 != (s32)0x0) goto L_80106DE8;
    r3 = 0x0;
    goto L_80106F84;
L_80106DE8: ;
    r0 = (s8)r25;
    if ((s32)r0 == (s32)0x6) goto L_80106EB4;
    r3 = *(u8*)lbl_80478B30;
    r4 = (s8)r3;
    if ((s32)r0 == (s32)r4) goto L_80106EA4;
    r0 = *(u8*)lbl_8047AD10;
    if ((u32)r0 != (u32)0x0) goto L_80106EA4;
    r24 = 0x0;
    if ((u32)r4 > (u32)0x9) goto L_80106E80;
    r3 = (u32)jumptable_8035B3A0;
    r0 = r4 << 2;
    r3 = (u32)jumptable_8035B3A0;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r24 = 0x40;
    goto L_80106E80;
    r24 = 0xf;
    goto L_80106E80;
    r24 = 0x10;
    goto L_80106E80;
    r24 = 0x40;
    goto L_80106E80;
    r24 = 0x50;
    goto L_80106E80;
    r24 = 0x51;
    goto L_80106E80;
    r24 = 0x10c;
    goto L_80106E80;
    r24 = 0xe6;
    goto L_80106E80;
    r24 = 0xe8;
    goto L_80106E80;
    r24 = 0x107;
L_80106E80: ;
    r3 = r24;
    fn_80102620();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106EA4;
    r3 = r24;
    r5 = r30;
    r4 = 0x2;
    fn_80102568();
L_80106EA4: ;
    *(u8*)lbl_80478B30 = r25;
    fn_801046B8();
    r4 = r3;
    goto L_80106EC4;
L_80106EB4: ;
    r29 = 0x1;
    r4 = -0x1;
    *(u8*)lbl_8047AD10 = r29;
    r30 = 0x0;
L_80106EC4: ;
    r0 = r27 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106ED8;
    r0 = r31 | 0x1;
    r31 = r0 & 0xFF;
L_80106ED8: ;
    r0 = r29 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80106EEC;
    r0 = r31 | 0x2;
    r31 = r0 & 0xFF;
L_80106EEC: ;
    r3 = 0x0;
    if ((u32)r28 > (u32)0x9) goto L_80106F5C;
    r5 = (u32)jumptable_8035B378;
    r0 = r28 << 2;
    r5 = (u32)jumptable_8035B378;
    r0 = *(u32*)(r5 + r0);
    ctr_fn = (void(*)(void))r0;
    /* indirect jump via ctr */;
    r3 = 0x40;
    goto L_80106F5C;
    r3 = 0xf;
    goto L_80106F5C;
    r3 = 0x10;
    goto L_80106F5C;
    r3 = 0x40;
    goto L_80106F5C;
    r3 = 0x50;
    goto L_80106F5C;
    r3 = 0x51;
    goto L_80106F5C;
    r3 = 0x10c;
    goto L_80106F5C;
    r3 = 0xe6;
    goto L_80106F5C;
    r3 = 0xe8;
    goto L_80106F5C;
    r3 = 0x107;
L_80106F5C: ;
    r0 = 0x0;
    r7 = r30;
    *(u32*)(sp + 0x8) = r0;
    r9 = r26;
    r10 = r31 & 0xFF;
    r5 = 0x0;
    r6 = 0x0;
    r8 = 0x3;
    /* crclr cr1eq */;
    fn_801026A4();
L_80106F84: ;
    /* lmw r24, 0x10(r1) */;
    return;
}
#pragma pop

/* 0x80106F98 | 0x15C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80106F98(void) {
    extern u8 lbl_80404B68[];
    extern void fn_800D3088();
    extern void fn_80104704();
    extern void fn_801074D4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_801070D8;
    r4 = (u32)lbl_80404B68;
    r30 = *(u32*)((u8*)r3 + 0x20);
    r31 = (u32)lbl_80404B68;
    goto L_801070D0;
L_80106FCC: ;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0xC) = r0;
    *(u16*)((u8*)r30 + 0x10) = r0;
    r0 = *(s16*)((u8*)r30 + 0x50);
    *(u16*)((u8*)r31 + 0x0) = r0;
    r0 = *(s16*)((u8*)r30 + 0x52);
    *(u16*)((u8*)r31 + 0x2) = r0;
    r0 = *(u32*)((u8*)r30 + 0x64);
    *(u32*)((u8*)r31 + 0x4) = r0;
    f0 = *(f32*)((u8*)r30 + 0x68);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r30 + 0x6C);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r0 = *(u8*)((u8*)r30 + 0x4);
    *(u8*)((u8*)r31 + 0x20) = r0;
    r0 = *(u32*)((u8*)r30 + 0x58);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5C);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5E);
    *(u16*)((u8*)r31 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r30 + 0x60);
    *(u16*)((u8*)r31 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r30 + 0x62);
    *(u16*)((u8*)r31 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r31 + 0x8) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x4);
    r29 = 0x0;
    *(u16*)((u8*)r31 + 0xA) = r0;
    goto L_80107068;
L_80107058: ;
    r3 = r31;
    r4 = r30 + 0xc;
    fn_801074D4();
    r29 = r29 + 0x1;
L_80107068: ;
    fn_800D3088();
    if ((u32)r29 < (u32)r3) goto L_80107058;
    r0 = *(s16*)((u8*)r31 + 0x0);
    *(u16*)((u8*)r30 + 0x50) = r0;
    r0 = *(s16*)((u8*)r31 + 0x2);
    *(u16*)((u8*)r30 + 0x52) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r30 + 0x64) = r0;
    f0 = *(f32*)((u8*)r31 + 0xC);
    *(f32*)((u8*)r30 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r30 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r31 + 0x20);
    *(u8*)((u8*)r30 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r30 + 0x58) = r0;
    r0 = *(s16*)((u8*)r31 + 0x18);
    *(u16*)((u8*)r30 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1A);
    *(u16*)((u8*)r30 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r30 + 0x60) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1E);
    *(u16*)((u8*)r30 + 0x62) = r0;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801070D0: ;
    if ((u32)r30 != (u32)0x0) goto L_80106FCC;
L_801070D8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
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
    fn_801046C8(NULL, 0);
    return 0;
}

/* 0x801071D0 | 0x304 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801071D0(void) {
    extern u8 lbl_80404B68[];
    extern u8 lbl_80404B8C[];
    extern void fn_800D3088();
    extern void fn_801074D4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r29 = r3;
    r3 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r29 + 0x84);
    r30 = (u32)lbl_80404B8C;
    *(u16*)((u8*)r30 + 0x0) = r0;
    r0 = *(s16*)((u8*)r29 + 0x86);
    *(u16*)((u8*)r30 + 0x2) = r0;
    r0 = *(u32*)((u8*)r29 + 0x88);
    *(u32*)((u8*)r30 + 0x4) = r0;
    r0 = *(u8*)((u8*)r29 + 0x0);
    *(u8*)((u8*)r30 + 0x20) = r0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r4 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r3 + 0x6);
    r3 = (u32)lbl_80404B8C;
    *(u16*)((u8*)r3 + 0x8) = r0;
    r3 = *(u32*)((u8*)r29 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r4 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r3 + 0x8);
    r31 = (u32)lbl_80404B8C;
    r28 = 0x0;
    *(u16*)((u8*)r31 + 0xA) = r0;
    goto L_80107260;
L_80107250: ;
    r3 = r31;
    r4 = r29 + 0x24;
    fn_801074D4();
    r28 = r28 + 0x1;
L_80107260: ;
    fn_800D3088();
    if ((u32)r28 < (u32)r3) goto L_80107250;
    r4 = (u32)lbl_80404B8C;
    r3 = (u32)lbl_80404B68;
    r4 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r4 + 0x0);
    r31 = (u32)lbl_80404B68;
    *(u16*)((u8*)r29 + 0x84) = r0;
    r0 = *(s16*)((u8*)r30 + 0x2);
    *(u16*)((u8*)r29 + 0x86) = r0;
    r0 = *(u32*)((u8*)r30 + 0x4);
    *(u32*)((u8*)r29 + 0x88) = r0;
    r0 = *(u8*)((u8*)r30 + 0x20);
    *(u8*)((u8*)r29 + 0x0) = r0;
    r30 = *(u32*)((u8*)r29 + 0x1C);
    goto L_8010739C;
L_801072A4: ;
    r0 = *(s16*)((u8*)r30 + 0x50);
    *(u16*)((u8*)r31 + 0x0) = r0;
    r0 = *(s16*)((u8*)r30 + 0x52);
    *(u16*)((u8*)r31 + 0x2) = r0;
    r0 = *(u32*)((u8*)r30 + 0x64);
    *(u32*)((u8*)r31 + 0x4) = r0;
    f0 = *(f32*)((u8*)r30 + 0x68);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r30 + 0x6C);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r0 = *(u8*)((u8*)r30 + 0x4);
    *(u8*)((u8*)r31 + 0x20) = r0;
    r0 = *(u32*)((u8*)r30 + 0x58);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5C);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5E);
    *(u16*)((u8*)r31 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r30 + 0x60);
    *(u16*)((u8*)r31 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r30 + 0x62);
    *(u16*)((u8*)r31 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r31 + 0x8) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x4);
    r28 = 0x0;
    *(u16*)((u8*)r31 + 0xA) = r0;
    goto L_80107334;
L_80107324: ;
    r3 = r31;
    r4 = r30 + 0xc;
    fn_801074D4();
    r28 = r28 + 0x1;
L_80107334: ;
    fn_800D3088();
    if ((u32)r28 < (u32)r3) goto L_80107324;
    r0 = *(s16*)((u8*)r31 + 0x0);
    *(u16*)((u8*)r30 + 0x50) = r0;
    r0 = *(s16*)((u8*)r31 + 0x2);
    *(u16*)((u8*)r30 + 0x52) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r30 + 0x64) = r0;
    f0 = *(f32*)((u8*)r31 + 0xC);
    *(f32*)((u8*)r30 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r30 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r31 + 0x20);
    *(u8*)((u8*)r30 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r30 + 0x58) = r0;
    r0 = *(s16*)((u8*)r31 + 0x18);
    *(u16*)((u8*)r30 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1A);
    *(u16*)((u8*)r30 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r30 + 0x60) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1E);
    *(u16*)((u8*)r30 + 0x62) = r0;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_8010739C: ;
    if ((u32)r30 != (u32)0x0) goto L_801072A4;
    r30 = *(u32*)((u8*)r29 + 0x20);
    r3 = (u32)lbl_80404B68;
    r29 = (u32)lbl_80404B68;
    goto L_801074AC;
L_801073B4: ;
    r0 = *(s16*)((u8*)r30 + 0x50);
    *(u16*)((u8*)r29 + 0x0) = r0;
    r0 = *(s16*)((u8*)r30 + 0x52);
    *(u16*)((u8*)r31 + 0x2) = r0;
    r0 = *(u32*)((u8*)r30 + 0x64);
    *(u32*)((u8*)r31 + 0x4) = r0;
    f0 = *(f32*)((u8*)r30 + 0x68);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r30 + 0x6C);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r0 = *(u8*)((u8*)r30 + 0x4);
    *(u8*)((u8*)r31 + 0x20) = r0;
    r0 = *(u32*)((u8*)r30 + 0x58);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5C);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5E);
    *(u16*)((u8*)r31 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r30 + 0x60);
    *(u16*)((u8*)r31 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r30 + 0x62);
    *(u16*)((u8*)r31 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r31 + 0x8) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x4);
    r28 = 0x0;
    *(u16*)((u8*)r31 + 0xA) = r0;
    goto L_80107444;
L_80107434: ;
    r3 = r29;
    r4 = r30 + 0xc;
    fn_801074D4();
    r28 = r28 + 0x1;
L_80107444: ;
    fn_800D3088();
    if ((u32)r28 < (u32)r3) goto L_80107434;
    r0 = *(s16*)((u8*)r29 + 0x0);
    *(u16*)((u8*)r30 + 0x50) = r0;
    r0 = *(s16*)((u8*)r31 + 0x2);
    *(u16*)((u8*)r30 + 0x52) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r30 + 0x64) = r0;
    f0 = *(f32*)((u8*)r31 + 0xC);
    *(f32*)((u8*)r30 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r30 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r31 + 0x20);
    *(u8*)((u8*)r30 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r30 + 0x58) = r0;
    r0 = *(s16*)((u8*)r31 + 0x18);
    *(u16*)((u8*)r30 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1A);
    *(u16*)((u8*)r30 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r30 + 0x60) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1E);
    *(u16*)((u8*)r30 + 0x62) = r0;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801074AC: ;
    if ((u32)r30 != (u32)0x0) goto L_801073B4;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x60 | fn_80107E78 | generic */
u32 fn_80107E78(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_801046C8(NULL, 0);
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
    extern u8 lbl_80404B68[];
    extern void fn_800D3088();
    extern void fn_80104704();
    extern void fn_801074D4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r28 = r4;
    fn_80104704();
    if ((u32)r3 == (u32)0x0) goto L_801080AC;
    r4 = (u32)lbl_80404B68;
    r29 = *(u32*)((u8*)r3 + 0x20);
    r30 = r28 & 0xFFFF;
    r31 = (u32)lbl_80404B68;
    goto L_801080A4;
L_80107F78: ;
    if ((u32)r30 != (u32)0x0) goto L_80107F90;
    r0 = 0x0;
    *(u32*)((u8*)r29 + 0xC) = r0;
    *(u16*)((u8*)r29 + 0x10) = r0;
    goto L_80107FAC;
L_80107F90: ;
    r3 = r29 + 0xc;
    r4 = 0x0;
    r5 = 0x3c;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r30;
    ((void(*)(void))fn_8005D830)();
    *(u32*)((u8*)r29 + 0xC) = r3;
L_80107FAC: ;
    r0 = *(s16*)((u8*)r29 + 0x50);
    *(u16*)((u8*)r31 + 0x0) = r0;
    r0 = *(s16*)((u8*)r29 + 0x52);
    *(u16*)((u8*)r31 + 0x2) = r0;
    r0 = *(u32*)((u8*)r29 + 0x64);
    *(u32*)((u8*)r31 + 0x4) = r0;
    f0 = *(f32*)((u8*)r29 + 0x68);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r29 + 0x6C);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r0 = *(u8*)((u8*)r29 + 0x4);
    *(u8*)((u8*)r31 + 0x20) = r0;
    r0 = *(u32*)((u8*)r29 + 0x58);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(s16*)((u8*)r29 + 0x5C);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r0 = *(s16*)((u8*)r29 + 0x5E);
    *(u16*)((u8*)r31 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r29 + 0x60);
    *(u16*)((u8*)r31 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r29 + 0x62);
    *(u16*)((u8*)r31 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r29 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r31 + 0x8) = r0;
    r3 = *(s16*)((u8*)r29 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x4);
    r28 = 0x0;
    *(u16*)((u8*)r31 + 0xA) = r0;
    goto L_8010803C;
L_8010802C: ;
    r3 = r31;
    r4 = r29 + 0xc;
    fn_801074D4();
    r28 = r28 + 0x1;
L_8010803C: ;
    fn_800D3088();
    if ((u32)r28 < (u32)r3) goto L_8010802C;
    r0 = *(s16*)((u8*)r31 + 0x0);
    *(u16*)((u8*)r29 + 0x50) = r0;
    r0 = *(s16*)((u8*)r31 + 0x2);
    *(u16*)((u8*)r29 + 0x52) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r29 + 0x64) = r0;
    f0 = *(f32*)((u8*)r31 + 0xC);
    *(f32*)((u8*)r29 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r29 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r31 + 0x20);
    *(u8*)((u8*)r29 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r29 + 0x58) = r0;
    r0 = *(s16*)((u8*)r31 + 0x18);
    *(u16*)((u8*)r29 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1A);
    *(u16*)((u8*)r29 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r29 + 0x60) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1E);
    *(u16*)((u8*)r29 + 0x62) = r0;
    r29 = *(u32*)((u8*)r29 + 0x0);
L_801080A4: ;
    if ((u32)r29 != (u32)0x0) goto L_80107F78;
L_801080AC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801080CC | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801080CC(void) {
    extern u8 lbl_80404B8C[];
    extern void fn_800D3088();
    extern void fn_80104704();
    extern void fn_801074D4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r4;
    fn_80104704();
    r31 = r3;
    if ((u32)r31 == (u32)0x0) goto L_801081D8;
    r0 = r28 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80108118;
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
    *(u16*)((u8*)r31 + 0x28) = r0;
    goto L_80108134;
L_80108118: ;
    r3 = r31 + 0x24;
    r4 = 0x0;
    r5 = 0x3c;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r28 & 0xFFFF;
    ((void(*)(void))fn_8005D830)();
    *(u32*)((u8*)r31 + 0x24) = r3;
L_80108134: ;
    r3 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r31 + 0x84);
    r29 = (u32)lbl_80404B8C;
    *(u16*)((u8*)r29 + 0x0) = r0;
    r0 = *(s16*)((u8*)r31 + 0x86);
    *(u16*)((u8*)r29 + 0x2) = r0;
    r0 = *(u32*)((u8*)r31 + 0x88);
    *(u32*)((u8*)r29 + 0x4) = r0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    *(u8*)((u8*)r29 + 0x20) = r0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r4 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r3 + 0x6);
    r3 = (u32)lbl_80404B8C;
    *(u16*)((u8*)r3 + 0x8) = r0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    ((void(*)(void))fn_8005DA18)();
    r4 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r3 + 0x8);
    r30 = (u32)lbl_80404B8C;
    r28 = 0x0;
    *(u16*)((u8*)r30 + 0xA) = r0;
    goto L_801081A4;
L_80108194: ;
    r3 = r30;
    r4 = r31 + 0x24;
    fn_801074D4();
    r28 = r28 + 0x1;
L_801081A4: ;
    fn_800D3088();
    if ((u32)r28 < (u32)r3) goto L_80108194;
    r3 = (u32)lbl_80404B8C;
    r3 = (u32)lbl_80404B8C;
    r0 = *(s16*)((u8*)r3 + 0x0);
    *(u16*)((u8*)r31 + 0x84) = r0;
    r0 = *(s16*)((u8*)r29 + 0x2);
    *(u16*)((u8*)r31 + 0x86) = r0;
    r0 = *(u32*)((u8*)r29 + 0x4);
    *(u32*)((u8*)r31 + 0x88) = r0;
    r0 = *(u8*)((u8*)r29 + 0x20);
    *(u8*)((u8*)r31 + 0x0) = r0;
L_801081D8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x801081F8 | 0x320 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801081F8(void) {
    extern u8 lbl_80404B68[];
    extern u8 lbl_8047AD1C[];
    extern void fn_800D3088();
    extern void fn_801046C8();
    extern void fn_801074D4();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    /* stmw r23, 0xc(r1) */;
    r25 = r5;
    if ((u32)r3 != (u32)0x0) goto L_801083A8;
    r3 = (u32)lbl_80404B68;
    r29 = r4 & 0xFFFF;
    r28 = r25 & 0xFFFF;
    r26 = 0x0;
    r30 = (u32)lbl_80404B68;
    r31 = 0x0;
L_8010822C: ;
    r0 = *(u32*)lbl_8047AD1C;
    r27 = r0 + r31;
    r0 = *(u8*)((u8*)r27 + 0x4);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x0) goto L_80108394;
    r0 = *(s16*)((u8*)r27 + 0x6);
    if ((s32)r0 != (s32)r29) goto L_80108394;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_8010826C;
    r0 = 0x0;
    *(u32*)((u8*)r27 + 0xC) = r0;
    *(u16*)((u8*)r27 + 0x10) = r0;
    goto L_80108288;
L_8010826C: ;
    r3 = r27 + 0xc;
    r4 = 0x0;
    r5 = 0x3c;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r28;
    ((void(*)(void))fn_8005D830)();
    *(u32*)((u8*)r27 + 0xC) = r3;
L_80108288: ;
    r3 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r27 + 0x50);
    r3 = (u32)lbl_80404B68;
    *(u16*)((u8*)r3 + 0x0) = r0;
    r0 = *(s16*)((u8*)r27 + 0x52);
    *(u16*)((u8*)r30 + 0x2) = r0;
    r0 = *(u32*)((u8*)r27 + 0x64);
    *(u32*)((u8*)r30 + 0x4) = r0;
    f0 = *(f32*)((u8*)r27 + 0x68);
    *(f32*)((u8*)r30 + 0xC) = f0;
    f0 = *(f32*)((u8*)r27 + 0x6C);
    *(f32*)((u8*)r30 + 0x10) = f0;
    r0 = *(u8*)((u8*)r27 + 0x4);
    *(u8*)((u8*)r30 + 0x20) = r0;
    r0 = *(u32*)((u8*)r27 + 0x58);
    *(u32*)((u8*)r30 + 0x14) = r0;
    r0 = *(s16*)((u8*)r27 + 0x5C);
    *(u16*)((u8*)r30 + 0x18) = r0;
    r0 = *(s16*)((u8*)r27 + 0x5E);
    *(u16*)((u8*)r30 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r27 + 0x60);
    *(u16*)((u8*)r30 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r27 + 0x62);
    *(u16*)((u8*)r30 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r27 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x2);
    *(u16*)((u8*)r30 + 0x8) = r0;
    r3 = *(s16*)((u8*)r27 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r0 = *(s16*)((u8*)r3 + 0x4);
    r23 = 0x0;
    *(u16*)((u8*)r30 + 0xA) = r0;
    r3 = (u32)lbl_80404B68;
    r24 = (u32)lbl_80404B68;
    goto L_80108328;
L_80108318: ;
    r3 = r24;
    r4 = r27 + 0xc;
    fn_801074D4();
    r23 = r23 + 0x1;
L_80108328: ;
    fn_800D3088();
    if ((u32)r23 < (u32)r3) goto L_80108318;
    r3 = (u32)lbl_80404B68;
    r3 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r3 + 0x0);
    *(u16*)((u8*)r27 + 0x50) = r0;
    r0 = *(s16*)((u8*)r30 + 0x2);
    *(u16*)((u8*)r27 + 0x52) = r0;
    r0 = *(u32*)((u8*)r30 + 0x4);
    *(u32*)((u8*)r27 + 0x64) = r0;
    f0 = *(f32*)((u8*)r30 + 0xC);
    *(f32*)((u8*)r27 + 0x68) = f0;
    f0 = *(f32*)((u8*)r30 + 0x10);
    *(f32*)((u8*)r27 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r30 + 0x20);
    *(u8*)((u8*)r27 + 0x4) = r0;
    r0 = *(u32*)((u8*)r30 + 0x14);
    *(u32*)((u8*)r27 + 0x58) = r0;
    r0 = *(s16*)((u8*)r30 + 0x18);
    *(u16*)((u8*)r27 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r30 + 0x1A);
    *(u16*)((u8*)r27 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r30 + 0x1C);
    *(u16*)((u8*)r27 + 0x60) = r0;
    r0 = *(s16*)((u8*)r30 + 0x1E);
    *(u16*)((u8*)r27 + 0x62) = r0;
L_80108394: ;
    r31 = r31 + 0x78;
    r26 = r26 + 0x1;
    if ((s32)r26 < (s32)0x168) goto L_8010822C;
    goto L_80108504;
L_801083A8: ;
    fn_801046C8();
    r30 = r3;
    if ((u32)r30 == (u32)0x0) goto L_80108504;
    r0 = r25 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_801083D4;
    r0 = 0x0;
    *(u32*)((u8*)r30 + 0xC) = r0;
    *(u16*)((u8*)r30 + 0x10) = r0;
    goto L_801083F0;
L_801083D4: ;
    r3 = r30 + 0xc;
    r4 = 0x0;
    r5 = 0x3c;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r25 & 0xFFFF;
    ((void(*)(void))fn_8005D830)();
    *(u32*)((u8*)r30 + 0xC) = r3;
L_801083F0: ;
    r3 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r30 + 0x50);
    r31 = (u32)lbl_80404B68;
    *(u16*)((u8*)r31 + 0x0) = r0;
    r0 = *(s16*)((u8*)r30 + 0x52);
    *(u16*)((u8*)r31 + 0x2) = r0;
    r0 = *(u32*)((u8*)r30 + 0x64);
    *(u32*)((u8*)r31 + 0x4) = r0;
    f0 = *(f32*)((u8*)r30 + 0x68);
    *(f32*)((u8*)r31 + 0xC) = f0;
    f0 = *(f32*)((u8*)r30 + 0x6C);
    *(f32*)((u8*)r31 + 0x10) = f0;
    r0 = *(u8*)((u8*)r30 + 0x4);
    *(u8*)((u8*)r31 + 0x20) = r0;
    r0 = *(u32*)((u8*)r30 + 0x58);
    *(u32*)((u8*)r31 + 0x14) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5C);
    *(u16*)((u8*)r31 + 0x18) = r0;
    r0 = *(s16*)((u8*)r30 + 0x5E);
    *(u16*)((u8*)r31 + 0x1A) = r0;
    r0 = *(s16*)((u8*)r30 + 0x60);
    *(u16*)((u8*)r31 + 0x1C) = r0;
    r0 = *(s16*)((u8*)r30 + 0x62);
    *(u16*)((u8*)r31 + 0x1E) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r4 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r3 + 0x2);
    r3 = (u32)lbl_80404B68;
    *(u16*)((u8*)r3 + 0x8) = r0;
    r3 = *(s16*)((u8*)r30 + 0x6);
    ((void(*)(void))fn_8005D934)();
    r4 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r3 + 0x4);
    r25 = (u32)lbl_80404B68;
    r23 = 0x0;
    *(u16*)((u8*)r25 + 0xA) = r0;
    goto L_80108498;
L_80108488: ;
    r3 = r25;
    r4 = r30 + 0xc;
    fn_801074D4();
    r23 = r23 + 0x1;
L_80108498: ;
    fn_800D3088();
    if ((u32)r23 < (u32)r3) goto L_80108488;
    r3 = (u32)lbl_80404B68;
    r3 = (u32)lbl_80404B68;
    r0 = *(s16*)((u8*)r3 + 0x0);
    *(u16*)((u8*)r30 + 0x50) = r0;
    r0 = *(s16*)((u8*)r31 + 0x2);
    *(u16*)((u8*)r30 + 0x52) = r0;
    r0 = *(u32*)((u8*)r31 + 0x4);
    *(u32*)((u8*)r30 + 0x64) = r0;
    f0 = *(f32*)((u8*)r31 + 0xC);
    *(f32*)((u8*)r30 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r30 + 0x6C) = f0;
    r0 = *(u8*)((u8*)r31 + 0x20);
    *(u8*)((u8*)r30 + 0x4) = r0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    *(u32*)((u8*)r30 + 0x58) = r0;
    r0 = *(s16*)((u8*)r31 + 0x18);
    *(u16*)((u8*)r30 + 0x5C) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1A);
    *(u16*)((u8*)r30 + 0x5E) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r30 + 0x60) = r0;
    r0 = *(s16*)((u8*)r31 + 0x1E);
    *(u16*)((u8*)r30 + 0x62) = r0;
L_80108504: ;
    /* lmw r23, 0xc(r1) */;
    return;
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
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r0 = r4 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80109248;
    r0 = *(u8*)((u8*)r3 + 0x4);
    r0 = r0 | 0x2;
    r0 = (s8)r0;
    *(u8*)((u8*)r3 + 0x4) = r0;
    return;
L_80109248: ;
    r0 = *(u8*)((u8*)r3 + 0x4);
    r0 = r0 & 0xFFFFFFFD;
    r0 = (s8)r0;
    *(u8*)((u8*)r3 + 0x4) = r0;
    return;
}
#pragma pop

/* 0x8010925C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010925C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    if ((u32)r3 == (u32)0x0) return;
    r5 = r3;
    r0 = 0x0;
    goto L_80109278;
L_80109270: ;
    *(u8*)((u8*)r4 + 0x4) = r0;
    r5 = *(u32*)((u8*)r5 + 0x0);
L_80109278: ;
    r4 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r4 != (u32)0x0) goto L_80109270;
    r0 = 0x0;
    *(u32*)((u8*)r3 + 0x0) = r0;
    return;
}
#pragma pop

/* 0x80109290 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109290(void) {
    extern u8 lbl_80271EE8[];
    extern u8 lbl_8047AD1C[];
    extern u8 lbl_8047CE3C[];
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    if ((u32)r30 != (u32)0x0) goto L_801092B8;
    r3 = 0x0;
    goto L_80109340;
L_801092B8: ;
    r31 = *(u32*)lbl_8047AD1C;
    r0 = 0x168;
    ctr_fn = (void(*)(void))r0;
L_801092C4: ;
    r0 = *(u8*)((u8*)r31 + 0x4);
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_80109324;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x78;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = 0x7;
    f0 = *(f32*)lbl_8047CE3C;
    *(u8*)((u8*)r31 + 0x4) = r0;
    r0 = -0x1;
    r3 = r30;
    *(f32*)((u8*)r31 + 0x68) = f0;
    *(f32*)((u8*)r31 + 0x6C) = f0;
    *(u32*)((u8*)r31 + 0x64) = r0;
    goto L_8010930C;
L_80109308: ;
    r3 = r0;
L_8010930C: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80109308;
    *(u32*)((u8*)r3 + 0x0) = r31;
    r3 = r31;
    goto L_80109340;
L_80109324: ;
    r31 = r31 + 0x78;
    if (--ctr != 0) goto L_801092C4;
    r3 = (u32)lbl_80271EE8;
    r3 = (u32)lbl_80271EE8;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
L_80109340: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
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
    extern u8 lbl_80314AE8[];
    extern u8 lbl_8047AD2C[];
    extern u8 lbl_8047AD30[];
    extern u8 lbl_8047AD34[];
    extern u8 lbl_8047AD38[];
    extern u8 lbl_8047AD3C[];
    extern u8 lbl_8047CE48[];
    extern u8 lbl_8047CE4C[];
    extern u8 lbl_8047CE50[];
    extern u8 lbl_8047CE54[];
    extern u8 lbl_8047CE58[];
    extern u8 lbl_8047CE5C[];
    extern u8 lbl_8047CE60[];
    extern u8 lbl_8047CE68[];
    extern void fn_800D3088();
    extern void fn_800D37CC();
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D9F40();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f31 = 0.0f;

    *(f64*)(sp + 0x20) = f31;
    /* psq_st f31, 0x28(r1), 0, qr0 */;
    r0 = *(u8*)&lbl_8047AD24;
    if ((u32)r0 == (u32)0x0) goto L_80109454;
    fn_800D37CC();
    /* xoris r3, r3, 0x8000 */;
    r0 = (0x4330 << 16);
    f1 = *(f64*)lbl_8047CE60;
    *(u32*)(sp + 0x8) = r0;
    f0 = *(f64*)(sp + 0x8);
    f31 = f0 - f1;
    fn_800D3088();
    r0 = (0x4330 << 16);
    f2 = *(f64*)lbl_8047CE68;
    *(u32*)(sp + 0x10) = r0;
    f0 = *(f32*)lbl_8047AD3C;
    f1 = *(f64*)(sp + 0x10);
    f3 = *(f32*)lbl_8047AD38;
    f1 = f1 - f2;
    f1 = f1 / f31;
    f0 = f0 + f1;
    *(f32*)lbl_8047AD3C = f0;
    /* cror eq, gt, eq */;
    if (f0 != f3) goto L_80109454;
    r0 = 0x0;
    *(f32*)lbl_8047AD3C = f3;
    *(u8*)&lbl_8047AD24 = r0;
L_80109454: ;
    r0 = *(u8*)&lbl_8047AD22;
    if ((u32)r0 == (u32)0x0) goto L_80109648;
    r0 = *(u8*)&lbl_8047AD20;
    if ((u32)r0 == (u32)0x0) goto L_80109648;
    f1 = *(f32*)lbl_8047AD3C;
    f0 = *(f32*)lbl_8047AD38;
    r0 = *(u8*)&lbl_8047AD23;
    f3 = f1 / f0;
    f4 = *(f32*)lbl_8047AD2C;
    f1 = *(f32*)lbl_8047AD34;
    f2 = *(f32*)lbl_8047CE48;
    f0 = *(f32*)lbl_8047CE4C;
    f1 = f1 - f4;
    f3 = f3 * f1 + f4;
    f1 = f2 * f3;
    *(f32*)lbl_8047AD30 = f3;
    f0 = f1 / f0;
    f0 = f2 - f0;
    f0 = (f64)(s32)f0;
    *(f64*)(sp + 0x10) = f0;
    r31 = *(u32*)(sp + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_801094C4;
    r3 = 0x1;
    ((void(*)(void))fn_800D9ED8)();
    goto L_801094CC;
L_801094C4: ;
    r3 = 0x0;
    ((void(*)(void))fn_800D9ED8)();
L_801094CC: ;
    r3 = 0x3;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x4;
    ((void(*)(void))fn_800D888C)();
    f1 = *(f32*)lbl_8047CE50;
    f3 = *(f32*)lbl_8047CE54;
    f2 = f1;
    f4 = *(f32*)lbl_8047CE58;
    ((void(*)(void))fn_800D9B58)();
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x2;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x4;
    fn_800D6A00();
    r3 = (u32)lbl_80314AE8;
    r3 = (u32)lbl_80314AE8;
    fn_800D7820();
    r4 = *(u32*)&lbl_8047AD28;
    r3 = 0x0;
    fn_800D85D4();
    r3 = 0x4;
    fn_800D67BC();
    f1 = *(f32*)lbl_8047CE50;
    f2 = f1;
    f3 = f1;
    fn_800D6680();
    r4 = r31;
    r5 = r31;
    r6 = r31;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047CE50;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    f2 = *(f32*)lbl_8047CE50;
    f1 = *(f32*)lbl_8047CE54;
    f3 = f2;
    fn_800D6680();
    r4 = r31;
    r5 = r31;
    r6 = r31;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047CE5C;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047CE50;
    fn_800D59B8();
    f1 = *(f32*)lbl_8047CE50;
    f2 = *(f32*)lbl_8047CE58;
    f3 = f1;
    fn_800D6680();
    r4 = r31;
    r5 = r31;
    r6 = r31;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047CE50;
    r3 = 0x0;
    f2 = *(f32*)lbl_8047CE5C;
    fn_800D59B8();
    f1 = *(f32*)lbl_8047CE54;
    f2 = *(f32*)lbl_8047CE58;
    f3 = *(f32*)lbl_8047CE50;
    fn_800D6680();
    r4 = r31;
    r5 = r31;
    r6 = r31;
    r3 = 0x0;
    r7 = 0xff;
    fn_800D5CB8();
    f1 = *(f32*)lbl_8047CE5C;
    r3 = 0x0;
    f2 = f1;
    fn_800D59B8();
    fn_800D6728();
    r0 = *(u8*)&lbl_8047AD23;
    if ((u32)r0 != (u32)0x0) goto L_80109648;
    r3 = 0x0;
    fn_800D9F40();
L_80109648: ;
    /* psq_l f31, 0x28(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x20);
    r31 = *(u32*)(sp + 0x1C);
    return;
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
    extern u8 lbl_8047AD2C[];
    extern u8 lbl_8047AD30[];
    extern u8 lbl_8047AD34[];
    extern u8 lbl_8047AD38[];
    extern u8 lbl_8047AD3C[];
    extern u8 lbl_8047CE50[];
    extern u8 lbl_8047CE5C[];
    u32 r0 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    f0 = *(f32*)lbl_8047CE50;
    r0 = 0x1;
    f3 = *(f32*)lbl_8047AD30;
    *(u8*)&lbl_8047AD24 = r0;
    *(f32*)lbl_8047AD2C = f3;
    *(f32*)lbl_8047AD34 = f1;
    *(f32*)lbl_8047AD30 = f0;
    *(f32*)lbl_8047AD38 = f2;
    *(f32*)lbl_8047AD3C = f0;
    if (f0 != f2) return;
    f0 = *(f32*)lbl_8047CE5C;
    *(f32*)lbl_8047AD38 = f0;
    *(f32*)lbl_8047AD3C = f0;
    return;
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
    extern u8 lbl_8047AD2C[];
    extern u8 lbl_8047AD30[];
    extern u8 lbl_8047AD34[];
    extern u8 lbl_8047AD38[];
    extern u8 lbl_8047AD3C[];
    extern u8 lbl_8047CE50[];
    extern void fn_800DC390();
    extern void fn_80109884();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;

    r31 = r3;
    f0 = *(f32*)lbl_8047CE50;
    r6 = 0x0;
    r0 = 0x1;
    r3 = (u32)fn_80109884;
    r4 = (u32)fn_80109884;
    *(u8*)&lbl_8047AD24 = r6;
    r3 = *(u32*)&lbl_8047AD28;
    r5 = 0x0;
    *(u8*)&lbl_8047AD21 = r0;
    *(u8*)&lbl_8047AD20 = r6;
    *(u8*)&lbl_8047AD22 = r6;
    *(f32*)lbl_8047AD2C = f0;
    *(f32*)lbl_8047AD30 = f0;
    *(f32*)lbl_8047AD34 = f0;
    *(f32*)lbl_8047AD38 = f0;
    *(f32*)lbl_8047AD3C = f0;
    fn_800DC390();
    r31 = r31 & 0xFF;
    goto L_801097EC;
L_801097DC: ;
    r0 = *(u8*)&lbl_8047AD20;
    if ((u32)r0 != (u32)0x0) goto L_801097F4;
    ((void(*)(void))fn_800F0308)();
L_801097EC: ;
    if ((u32)r31 != (u32)0x0) goto L_801097DC;
L_801097F4: ;
    r0 = *(u8*)&lbl_8047AD20;
    r3 = r0 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x74 | fn_80109810 | generic */
u32 fn_80109810(void) {
    /* refs: lbl_8047AD20, lbl_8047AD21, lbl_8047AD22, lbl_8047AD23, lbl_8047AD24, lbl_8047AD28, lbl_8047AD2C, lbl_8047AD30, lbl_8047AD34, lbl_8047AD38, lbl_8047AD3C, lbl_8047CE50 */
    fn_800EF5FC();
    return 0;
}

/* 0x80109884 | 0x10 -- set lbl_8047AD20 = 1, return 0 */
u32 fn_80109884(void) {
    lbl_8047AD20 = 1;
    return 0;
}

/* 0x80109894 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109894(void) {
    extern u8 lbl_8047CE70[];
    extern void fn_800EC1BC();
    extern void fn_800EC990();
    extern void fn_800EC9DC();
    extern void fn_800ECCA8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_801098B8;
    r3 = 0x0;
    goto L_80109920;
L_801098B8: ;
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r0 == (u32)0x0) goto L_801098D8;
    r0 = *(u8*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)0x0) goto L_8010991C;
    *(u32*)((u8*)r31 + 0xC) = r4;
    goto L_8010991C;
L_801098D8: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)0x0) goto L_8010991C;
    *(u32*)((u8*)r31 + 0x1C) = r4;
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_800EC1BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010991C;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = *(u32*)((u8*)r31 + 0x1C);
    fn_800ECCA8();
    r3 = *(u32*)((u8*)r31 + 0x24);
    f1 = *(f32*)lbl_8047CE70;
    fn_800EC9DC();
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_800EC990();
L_8010991C: ;
    r3 = 0x1;
L_80109920: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80109934 | 0x25C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109934(void) {
    extern u8 lbl_8035B468[];
    extern void fn_800D2248();
    extern void fn_800D2584();
    extern void fn_800D258C();
    extern void fn_800D3190();
    extern void fn_800D3410();
    extern void fn_800D377C();
    extern void fn_800D4604();
    extern void fn_800D9AF0();
    extern void fn_800D9B24();
    extern void fn_800D9C24();
    extern void fn_800D9D68();
    extern void fn_800DCC34();
    extern void fn_800E3760();
    extern void fn_800E638C();
    extern void fn_800E6478();
    extern void fn_800EC134();
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_801DA448();
    extern void fn_801DA4E8();
    extern void fn_801DAAAC();
    extern void fn_801DAC3C();
    extern void fn_801DB088();
    u8 sp[0x30];
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

    /* stmw r27, 0x1c(r1) */;
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_80109958;
    r3 = 0x0;
    goto L_80109B7C;
L_80109958: ;
    r0 = *(u32*)((u8*)r31 + 0x34);
    if ((u32)r0 != (u32)0x0) goto L_8010996C;
    r3 = 0x0;
    goto L_80109B7C;
L_8010996C: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((u32)r0 == (u32)0x2) goto L_80109980;
    r3 = 0x0;
    goto L_80109B7C;
L_80109980: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_8010999C;
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_801DAC3C();
    r30 = r3;
    goto L_801099A0;
L_8010999C: ;
    r30 = *(u32*)((u8*)r31 + 0x24);
L_801099A0: ;
    r28 = r30;
    if ((u32)r30 != (u32)0x0) goto L_801099B4;
    r3 = 0x0;
    goto L_80109B7C;
L_801099B4: ;
    r3 = r30;
    fn_800EC134();
    r4 = (u32)lbl_8035B468;
    r3 = r30;
    r4 = (u32)lbl_8035B468;
    fn_800E6478();
    r30 = r31;
    r27 = 0x0;
L_801099D4: ;
    r3 = *(u32*)((u8*)r30 + 0x3C);
    if ((u32)r3 == (u32)0x0) goto L_801099E8;
    r4 = 0x1;
    fn_800DCC34();
L_801099E8: ;
    r30 = r30 + 0x4;
    r27 = r27 + 0x1;
    if ((s32)r27 < (s32)0x3) goto L_801099D4;
    fn_800D2584();
    r0 = r3;
    r3 = 0x2;
    r29 = r0;
    fn_800D4604();
    r3 = 0x2;
    fn_800D377C();
    r3 = r1 + 0x16;
    r4 = r1 + 0x14;
    r5 = r1 + 0x12;
    r6 = r1 + 0x10;
    fn_800D9B24();
    r3 = r1 + 0xe;
    r4 = r1 + 0xc;
    r5 = r1 + 0xa;
    r6 = r1 + 0x8;
    fn_800D9AF0();
    r3 = *(u32*)((u8*)r31 + 0x38);
    fn_800D258C();
    r3 = *(u32*)((u8*)r31 + 0x38);
    fn_800D258C();
    r3 = *(u32*)((u8*)r31 + 0x34);
    fn_800EF4FC();
    /* subi r0, r3, 0x1 */;
    r3 = *(u32*)((u8*)r31 + 0x34);
    r30 = r0 & 0xFFFF;
    fn_800EF4F4();
    /* subi r0, r3, 0x1 */;
    r5 = r30;
    r27 = r0 & 0xFFFF;
    r3 = 0x0;
    r6 = r27;
    r4 = 0x0;
    fn_800D9D68();
    r5 = r30;
    r6 = r27;
    r3 = 0x0;
    r4 = 0x0;
    fn_800D9C24();
    fn_800D2248();
    r3 = *(u32*)((u8*)r31 + 0x34);
    r4 = 0x0;
    fn_800D3410();
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_80109AF0;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x1;
    fn_801DA4E8();
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x1;
    fn_801DA448();
    fn_801DB088();
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_801DAAAC();
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x0;
    fn_801DA4E8();
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x0;
    fn_801DA448();
    goto L_80109AFC;
L_80109AF0: ;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x3010;
    fn_800E3760();
L_80109AFC: ;
    fn_800D3190();
    r3 = 0x1;
    fn_800D377C();
    r3 = r29;
    fn_800D258C();
    r3 = *(u16*)(sp + 0x16);
    r4 = *(u16*)(sp + 0x14);
    r5 = *(u16*)(sp + 0x12);
    r6 = *(u16*)(sp + 0x10);
    fn_800D9D68();
    r3 = *(u16*)(sp + 0xE);
    r4 = *(u16*)(sp + 0xC);
    r5 = *(u16*)(sp + 0xA);
    r6 = *(u16*)(sp + 0x8);
    fn_800D9C24();
    fn_800D2248();
    r3 = 0x1;
    fn_800D4604();
    r30 = r31;
    r29 = 0x0;
L_80109B4C: ;
    r3 = *(u32*)((u8*)r30 + 0x3C);
    if ((u32)r3 == (u32)0x0) goto L_80109B60;
    r4 = 0x0;
    fn_800DCC34();
L_80109B60: ;
    r30 = r30 + 0x4;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x3) goto L_80109B4C;
    r3 = r28;
    fn_800E638C();
    r3 = *(u32*)((u8*)r31 + 0x34);
L_80109B7C: ;
    /* lmw r27, 0x1c(r1) */;
    return;
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
    extern void fn_801DB100();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_80109C20;
    r3 = 0x0;
    goto L_80109C74;
L_80109C20: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_80109C50;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_80109C68;
    fn_801DB100();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
    goto L_80109C68;
L_80109C50: ;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_80109C68;
    ((void(*)(void))fn_800E4BF4)();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
L_80109C68: ;
    r0 = 0x0;
    r3 = 0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_80109C74: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80109C88 | 0x388 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80109C88(void) {
    extern void fn_800F0494();
    extern void fn_800F04BC();
    extern void fn_800F0654();
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_80121ADC();
    extern void fn_8012640C();
    extern void fn_8010A88C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r4;
    if ((u32)r30 != (u32)0x0) goto L_80109CB4;
    r3 = 0x0;
    goto L_80109FF8;
L_80109CB4: ;
    if ((u32)r30 != (u32)0x0) goto L_80109CC0;
    r0 = 0x0;
    goto L_80109E84;
L_80109CC0: ;
    r0 = 0x1;
    r3 = r31;
    *(u8*)(sp + 0x8) = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    *(u16*)(sp + 0x14) = r4;
    r5 = 0x66;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    *(u16*)(sp + 0xC) = r0;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) goto L_80109D48;
    r3 = r31;
    r4 = 0x3e;
    fn_80121ADC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80109D3C;
    r0 = 0x87;
    *(u16*)(sp + 0xE) = r0;
    goto L_80109D50;
L_80109D3C: ;
    r0 = 0x25;
    *(u16*)(sp + 0xE) = r0;
    goto L_80109D50;
L_80109D48: ;
    r0 = 0x0;
    *(u16*)(sp + 0xE) = r0;
L_80109D50: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    fn_8012640C();
    r0 = *(u8*)((u8*)r30 + 0x1);
    r4 = r3 & 0xFF;
    *(u8*)(sp + 0x16) = r4;
    if ((u32)r0 == (u32)0x0) goto L_80109E0C;
    r3 = *(u8*)((u8*)r30 + 0x4);
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r3 != (u32)r0) goto L_80109E04;
    if ((u32)r3 == (u32)0x0) goto L_80109DEC;
    r3 = *(u16*)((u8*)r30 + 0x8);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_80109E04;
    r3 = *(u16*)((u8*)r30 + 0xA);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_80109E04;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r3 != (u32)r0) goto L_80109E04;
    r0 = *(u8*)((u8*)r30 + 0x12);
    if ((u32)r0 != (u32)r4) goto L_80109E04;
    r0 = 0x1;
    goto L_80109E84;
L_80109DEC: ;
    r3 = *(u32*)((u8*)r30 + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_80109E04;
    r0 = 0x1;
    goto L_80109E84;
L_80109E04: ;
    r0 = 0x0;
    goto L_80109E84;
L_80109E0C: ;
    r3 = *(u8*)((u8*)r30 + 0x14);
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r3 != (u32)r0) goto L_80109E80;
    if ((u32)r3 == (u32)0x0) goto L_80109E68;
    r3 = *(u16*)((u8*)r30 + 0x18);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_80109E80;
    r3 = *(u16*)((u8*)r30 + 0x1A);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_80109E80;
    r3 = *(u32*)((u8*)r30 + 0x1C);
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r3 != (u32)r0) goto L_80109E80;
    r0 = *(u8*)((u8*)r30 + 0x22);
    if ((u32)r0 != (u32)r4) goto L_80109E80;
    r0 = 0x1;
    goto L_80109E84;
L_80109E68: ;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_80109E80;
    r0 = 0x1;
    goto L_80109E84;
L_80109E80: ;
    r0 = 0x0;
L_80109E84: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80109E98;
    r3 = 0x0;
    goto L_80109FF8;
L_80109E98: ;
    r0 = *(u8*)((u8*)r30 + 0x0);
    if ((s32)r0 == (s32)0x0) goto L_80109EB0;
    if ((s32)r0 < (s32)0x0) goto L_80109EB8;
    goto L_80109EB8;
L_80109EB0: ;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x0) = r0;
L_80109EB8: ;
    r0 = 0x1;
    r3 = r31;
    *(u8*)((u8*)r30 + 0x4) = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    *(u16*)((u8*)r30 + 0x10) = r4;
    r5 = 0x66;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    *(u16*)((u8*)r30 + 0x8) = r0;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) goto L_80109F40;
    r3 = r31;
    r4 = 0x3e;
    fn_80121ADC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_80109F34;
    r0 = 0x87;
    *(u16*)((u8*)r30 + 0xA) = r0;
    goto L_80109F48;
L_80109F34: ;
    r0 = 0x25;
    *(u16*)((u8*)r30 + 0xA) = r0;
    goto L_80109F48;
L_80109F40: ;
    r0 = 0x0;
    *(u16*)((u8*)r30 + 0xA) = r0;
L_80109F48: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    *(u32*)((u8*)r30 + 0xC) = r3;
    r3 = r31;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFF;
    r0 = 0x1;
    *(u8*)((u8*)r30 + 0x12) = r3;
    *(u8*)((u8*)r30 + 0x1) = r0;
    r3 = *(u32*)((u8*)r30 + 0x28);
    if ((u32)r3 == (u32)0x0) goto L_80109FB0;
    fn_800F04BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80109FA8;
    r3 = 0x1;
    goto L_80109FF8;
L_80109FA8: ;
    r3 = *(u32*)((u8*)r30 + 0x28);
    fn_800F0494();
L_80109FB0: ;
    fn_800FF560();
    r5 = (u32)fn_8010A88C;
    r4 = r3;
    r8 = (u32)fn_8010A88C;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x1;
    fn_800F07A8();
    *(u32*)((u8*)r30 + 0x28) = r3;
    r3 = *(u32*)((u8*)r30 + 0x28);
    if ((u32)r3 == (u32)0x0) goto L_80109FF4;
    r5 = r30;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800F0654();
L_80109FF4: ;
    r3 = 0x1;
L_80109FF8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x8010A010 | 0x200 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A010(void) {
    extern void fn_800F0494();
    extern void fn_800F04BC();
    extern void fn_800F0654();
    extern void fn_800F07A8();
    extern void fn_800FF560();
    extern void fn_8010A88C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010A034;
    r3 = 0x0;
    goto L_8010A1FC;
L_8010A034: ;
    if ((u32)r31 != (u32)0x0) goto L_8010A040;
    r0 = 0x0;
    goto L_8010A140;
L_8010A040: ;
    r0 = *(u8*)((u8*)r31 + 0x1);
    r5 = 0x0;
    *(u8*)(sp + 0x8) = r5;
    if ((u32)r0 == (u32)0x0) goto L_8010A0D0;
    r0 = *(u8*)((u8*)r31 + 0x4);
    if ((u32)r0 != (u32)r5) goto L_8010A0C8;
    if ((u32)r0 == (u32)0x0) goto L_8010A0B4;
    r3 = *(u16*)((u8*)r31 + 0x8);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A0C8;
    r3 = *(u16*)((u8*)r31 + 0xA);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_8010A0C8;
    r0 = *(u32*)((u8*)r31 + 0xC);
    if ((u32)r0 != (u32)r5) goto L_8010A0C8;
    r3 = *(u8*)((u8*)r31 + 0x12);
    r0 = *(u8*)(sp + 0x16);
    if ((u32)r3 != (u32)r0) goto L_8010A0C8;
    r0 = 0x1;
    goto L_8010A140;
L_8010A0B4: ;
    r0 = *(u32*)((u8*)r31 + 0x8);
    if ((u32)r0 != (u32)r4) goto L_8010A0C8;
    r0 = 0x1;
    goto L_8010A140;
L_8010A0C8: ;
    r0 = 0x0;
    goto L_8010A140;
L_8010A0D0: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 != (u32)r5) goto L_8010A13C;
    if ((u32)r0 == (u32)0x0) goto L_8010A128;
    r3 = *(u16*)((u8*)r31 + 0x18);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A13C;
    r3 = *(u16*)((u8*)r31 + 0x1A);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_8010A13C;
    r0 = *(u32*)((u8*)r31 + 0x1C);
    if ((u32)r0 != (u32)r5) goto L_8010A13C;
    r3 = *(u8*)((u8*)r31 + 0x22);
    r0 = *(u8*)(sp + 0x16);
    if ((u32)r3 != (u32)r0) goto L_8010A13C;
    r0 = 0x1;
    goto L_8010A140;
L_8010A128: ;
    r0 = *(u32*)((u8*)r31 + 0x18);
    if ((u32)r0 != (u32)r4) goto L_8010A13C;
    r0 = 0x1;
    goto L_8010A140;
L_8010A13C: ;
    r0 = 0x0;
L_8010A140: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010A154;
    r3 = 0x0;
    goto L_8010A1FC;
L_8010A154: ;
    r0 = *(u8*)((u8*)r31 + 0x0);
    if ((s32)r0 == (s32)0x0) goto L_8010A16C;
    if ((s32)r0 < (s32)0x0) goto L_8010A174;
    goto L_8010A174;
L_8010A16C: ;
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_8010A174: ;
    r3 = 0x0;
    r0 = 0x1;
    *(u8*)((u8*)r31 + 0x4) = r3;
    *(u32*)((u8*)r31 + 0x8) = r4;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((u32)r3 == (u32)0x0) goto L_8010A1B4;
    fn_800F04BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010A1AC;
    r3 = 0x1;
    goto L_8010A1FC;
L_8010A1AC: ;
    r3 = *(u32*)((u8*)r31 + 0x28);
    fn_800F0494();
L_8010A1B4: ;
    fn_800FF560();
    r5 = (u32)fn_8010A88C;
    r4 = r3;
    r8 = (u32)fn_8010A88C;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x1;
    fn_800F07A8();
    *(u32*)((u8*)r31 + 0x28) = r3;
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((u32)r3 == (u32)0x0) goto L_8010A1F8;
    r5 = r31;
    r4 = 0x1;
    /* crclr cr1eq */;
    fn_800F0654();
L_8010A1F8: ;
    r3 = 0x0;
L_8010A1FC: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* 0x8010A210 | 0x210 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A210(void) {
    extern void fn_80121ADC();
    extern void fn_8012640C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    r30 = r4;
    if ((u32)r31 != (u32)0x0) goto L_8010A23C;
    r3 = 0x0;
    goto L_8010A408;
L_8010A23C: ;
    r0 = 0x1;
    r3 = r30;
    *(u8*)(sp + 0x8) = r0;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r4 = r3 & 0xFFFF;
    r3 = 0x0;
    *(u16*)(sp + 0x14) = r4;
    r5 = 0x66;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    r3 = r30;
    *(u16*)(sp + 0xC) = r0;
    r4 = 0x0;
    r5 = 0xc2;
    r6 = 0x0;
    fn_8012640C();
    if ((s32)r3 == (s32)0x0) goto L_8010A2C4;
    r3 = r30;
    r4 = 0x3e;
    fn_80121ADC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_8010A2B8;
    r0 = 0x87;
    *(u16*)(sp + 0xE) = r0;
    goto L_8010A2CC;
L_8010A2B8: ;
    r0 = 0x25;
    *(u16*)(sp + 0xE) = r0;
    goto L_8010A2CC;
L_8010A2C4: ;
    r0 = 0x0;
    *(u16*)(sp + 0xE) = r0;
L_8010A2CC: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6f;
    r6 = 0x0;
    fn_8012640C();
    r3 = r30;
    r4 = 0x0;
    r5 = 0xc1;
    r6 = 0x0;
    fn_8012640C();
    r0 = *(u8*)((u8*)r31 + 0x1);
    r4 = r3 & 0xFF;
    *(u8*)(sp + 0x16) = r4;
    if ((u32)r0 == (u32)0x0) goto L_8010A38C;
    r3 = *(u8*)((u8*)r31 + 0x4);
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r3 != (u32)r0) goto L_8010A380;
    if ((u32)r3 == (u32)0x0) goto L_8010A368;
    r3 = *(u16*)((u8*)r31 + 0x8);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A380;
    r3 = *(u16*)((u8*)r31 + 0xA);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_8010A380;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r3 != (u32)r0) goto L_8010A380;
    r0 = *(u8*)((u8*)r31 + 0x12);
    if ((u32)r0 != (u32)r4) goto L_8010A380;
    r0 = 0x1;
    goto L_8010A384;
L_8010A368: ;
    r3 = *(u32*)((u8*)r31 + 0x8);
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A380;
    r0 = 0x1;
    goto L_8010A384;
L_8010A380: ;
    r0 = 0x0;
L_8010A384: ;
    r3 = r0;
    goto L_8010A408;
L_8010A38C: ;
    r3 = *(u8*)((u8*)r31 + 0x14);
    r0 = *(u8*)(sp + 0x8);
    if ((u32)r3 != (u32)r0) goto L_8010A400;
    if ((u32)r3 == (u32)0x0) goto L_8010A3E8;
    r3 = *(u16*)((u8*)r31 + 0x18);
    r0 = *(u16*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A400;
    r3 = *(u16*)((u8*)r31 + 0x1A);
    r0 = *(u16*)(sp + 0xE);
    if ((u32)r3 != (u32)r0) goto L_8010A400;
    r3 = *(u32*)((u8*)r31 + 0x1C);
    r0 = *(u32*)(sp + 0x10);
    if ((u32)r3 != (u32)r0) goto L_8010A400;
    r0 = *(u8*)((u8*)r31 + 0x22);
    if ((u32)r0 != (u32)r4) goto L_8010A400;
    r0 = 0x1;
    goto L_8010A404;
L_8010A3E8: ;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r0 = *(u32*)(sp + 0xC);
    if ((u32)r3 != (u32)r0) goto L_8010A400;
    r0 = 0x1;
    goto L_8010A404;
L_8010A400: ;
    r0 = 0x0;
L_8010A404: ;
    r3 = r0;
L_8010A408: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x8010A420 | 0x19C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A420(void) {
    extern u8 lbl_80271F80[];
    extern u8 lbl_8035B458[];
    extern u8 lbl_8047AD40[];
    extern u8 lbl_8047AD44[];
    extern void fn_800DCD98();
    extern void fn_800EF5A4();
    extern void fn_800F0494();
    extern void fn_800F04BC();
    extern void fn_801DAC90();
    extern void fn_801DB100();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010A450;
    r3 = 0x0;
    goto L_8010A59C;
L_8010A450: ;
    r0 = *(u32*)lbl_8047AD40;
    if ((s32)r0 > (s32)0x0) goto L_8010A47C;
    r3 = (u32)lbl_80271F80;
    r4 = (u32)lbl_8035B458;
    r3 = (u32)lbl_80271F80;
    r4 = (u32)lbl_8035B458;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
    goto L_8010A59C;
L_8010A47C: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x1) = r0;
L_8010A484: ;
    r3 = *(u32*)((u8*)r31 + 0x28);
    if ((u32)r3 == (u32)0x0) goto L_8010A4B4;
    fn_800F04BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010A4AC;
    r3 = *(u32*)((u8*)r31 + 0x28);
    fn_800F0494();
    goto L_8010A4B4;
L_8010A4AC: ;
    ((void(*)(void))fn_800F0308)();
    goto L_8010A484;
L_8010A4B4: ;
    if ((u32)r31 == (u32)0x0) goto L_8010A50C;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_8010A4EC;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_8010A504;
    fn_801DB100();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
    goto L_8010A504;
L_8010A4EC: ;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_8010A504;
    ((void(*)(void))fn_800E4BF4)();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
L_8010A504: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_8010A50C: ;
    r29 = r31;
    r28 = 0x0;
    r30 = 0x0;
L_8010A518: ;
    r3 = *(u32*)((u8*)r29 + 0x3C);
    if ((u32)r3 == (u32)0x0) goto L_8010A52C;
    fn_800DCD98();
    *(u32*)((u8*)r29 + 0x3C) = r30;
L_8010A52C: ;
    r29 = r29 + 0x4;
    r28 = r28 + 0x1;
    if ((s32)r28 < (s32)0x3) goto L_8010A518;
    r3 = *(u32*)((u8*)r31 + 0x38);
    if ((u32)r3 == (u32)0x0) goto L_8010A554;
    ((void(*)(void))fn_800D2738)();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x38) = r0;
L_8010A554: ;
    r3 = *(u32*)((u8*)r31 + 0x34);
    if ((u32)r3 == (u32)0x0) goto L_8010A56C;
    fn_800EF5A4();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x34) = r0;
L_8010A56C: ;
    r3 = *(u32*)lbl_8047AD40;
    /* subi r0, r3, 0x1 */;
    *(u32*)lbl_8047AD40 = r0;
    if ((s32)r0 != (s32)0x0) goto L_8010A598;
    r0 = *(u8*)lbl_8047AD44;
    if ((u32)r0 == (u32)0x0) goto L_8010A598;
    fn_801DAC90();
    r0 = 0x0;
    *(u8*)lbl_8047AD44 = r0;
L_8010A598: ;
    r3 = 0x1;
L_8010A59C: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x8010A5BC | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A5BC(void) {
    extern u8 lbl_80271F38[];
    extern u8 lbl_8035B448[];
    extern u8 lbl_8047AD40[];
    extern u8 lbl_8047AD44[];
    extern u8 lbl_8047CE74[];
    extern void fn_800D203C();
    extern void fn_800D207C();
    extern void fn_800D29A0();
    extern void fn_800DCC34();
    extern void fn_800DCC3C();
    extern void fn_800DCC60();
    extern void fn_800DCC84();
    extern void fn_800DCCF0();
    extern void fn_800DCFBC();
    extern void fn_800E01F4();
    extern void fn_800EF578();
    u8 sp[0x70];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    /* stmw r23, 0x4c(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r4 = (u32)lbl_80271F38;
    r31 = (u32)lbl_80271F38;
    r23 = *(u32*)((u8*)r31 + 0xC);
    r24 = *(u32*)((u8*)r31 + 0x10);
    r25 = *(u32*)((u8*)r31 + 0x14);
    r26 = *(u32*)((u8*)r31 + 0x18);
    r27 = *(u32*)((u8*)r31 + 0x1C);
    r12 = *(u32*)((u8*)r31 + 0x20);
    r11 = *(u32*)((u8*)r31 + 0x24);
    r10 = *(u32*)((u8*)r31 + 0x28);
    r9 = *(u32*)((u8*)r31 + 0x2C);
    r8 = *(u32*)((u8*)r31 + 0x30);
    r7 = *(u32*)((u8*)r31 + 0x34);
    r6 = *(u32*)((u8*)r31 + 0x38);
    r5 = *(u32*)((u8*)r31 + 0x3C);
    r4 = *(u32*)((u8*)r31 + 0x40);
    r0 = *(u32*)((u8*)r31 + 0x44);
    *(u32*)(sp + 0x10) = r0;
    if ((u32)r28 != (u32)0x0) goto L_8010A668;
    r3 = 0x0;
    goto L_8010A878;
L_8010A668: ;
    r4 = 0x0;
    r5 = 0x48;
    memset((void*)r3, (int)r4, (u32)r5);
    r0 = *(u32*)lbl_8047AD40;
    if ((s32)r0 < (s32)0x4) goto L_8010A69C;
    r4 = (u32)lbl_8035B448;
    r3 = r31 + 0x70;
    r4 = (u32)lbl_8035B448;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
    goto L_8010A878;
L_8010A69C: ;
    *(u32*)((u8*)r28 + 0x2C) = r29;
    *(u32*)((u8*)r28 + 0x30) = r30;
    if ((s32)r29 >= (s32)0x100) goto L_8010A6B0;
    r29 = 0x100;
L_8010A6B0: ;
    if ((s32)r29 <= (s32)0x280) goto L_8010A6BC;
    r29 = 0x280;
L_8010A6BC: ;
    if ((s32)r30 >= (s32)0x100) goto L_8010A6C8;
    r30 = 0x100;
L_8010A6C8: ;
    if ((s32)r30 <= (s32)0x1e0) goto L_8010A6D4;
    r30 = 0x1e0;
L_8010A6D4: ;
    r3 = r29 & 0xFFFF;
    r4 = r30 & 0xFFFF;
    r5 = 0x45;
    r6 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_800EF5FC)();
    *(u32*)((u8*)r28 + 0x34) = r3;
    r3 = *(u32*)((u8*)r28 + 0x34);
    if ((u32)r3 != (u32)0x0) goto L_8010A718;
    r4 = (u32)lbl_8035B448;
    r3 = r31 + 0x8c;
    r4 = (u32)lbl_8035B448;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
    goto L_8010A878;
L_8010A718: ;
    r4 = 0x2;
    r5 = 0x2;
    r6 = 0x0;
    fn_800EF578();
    fn_800D29A0();
    *(u32*)((u8*)r28 + 0x38) = r3;
    r3 = *(u32*)((u8*)r28 + 0x38);
    if ((u32)r3 != (u32)0x0) goto L_8010A758;
    r4 = (u32)lbl_8035B448;
    r3 = r31 + 0xac;
    r4 = (u32)lbl_8035B448;
    /* crclr cr1eq */;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
    goto L_8010A878;
L_8010A758: ;
    r4 = r1 + 0x38;
    fn_800D207C();
    r3 = *(u32*)((u8*)r28 + 0x38);
    r4 = r1 + 0x2c;
    fn_800D203C();
    r23 = r28;
    r24 = 0x0;
L_8010A774: ;
    fn_800DCFBC();
    r25 = r3;
    if ((u32)r25 == (u32)0x0) goto L_8010A844;
    if ((s32)r24 == (s32)0x1) goto L_8010A7C0;
    if ((s32)r24 >= (s32)0x1) goto L_8010A79C;
    if ((s32)r24 >= (s32)0x0) goto L_8010A7A8;
    goto L_8010A834;
L_8010A79C: ;
    if ((s32)r24 >= (s32)0x3) goto L_8010A834;
    goto L_8010A7F0;
L_8010A7A8: ;
    r4 = 0x0;
    fn_800DCCF0();
    r3 = r25;
    r4 = r1 + 0x8;
    fn_800DCC84();
    goto L_8010A834;
L_8010A7C0: ;
    r4 = 0x2;
    fn_800DCCF0();
    r3 = r25;
    r4 = r1 + 0x14;
    fn_800DCC84();
    r3 = r25;
    r4 = r1 + 0x38;
    fn_800DCC60();
    r3 = r25;
    r4 = r1 + 0x20;
    fn_800DCC3C();
    goto L_8010A834;
L_8010A7F0: ;
    f1 = *(f32*)lbl_8047CE74;
    r3 = r1 + 0x38;
    f2 = f1;
    f3 = f1;
    fn_800E01F4();
    r3 = r25;
    r4 = 0x2;
    fn_800DCCF0();
    r3 = r25;
    r4 = r1 + 0x14;
    fn_800DCC84();
    r3 = r25;
    r4 = r1 + 0x38;
    fn_800DCC60();
    r3 = r25;
    r4 = r1 + 0x20;
    fn_800DCC3C();
L_8010A834: ;
    r3 = r25;
    r4 = 0x0;
    fn_800DCC34();
    *(u32*)((u8*)r23 + 0x3C) = r25;
L_8010A844: ;
    r23 = r23 + 0x4;
    r24 = r24 + 0x1;
    if ((s32)r24 < (s32)0x3) goto L_8010A774;
    r0 = *(u32*)lbl_8047AD40;
    if ((s32)r0 != (s32)0x0) goto L_8010A868;
    r0 = 0x0;
    *(u8*)lbl_8047AD44 = r0;
L_8010A868: ;
    r4 = *(u32*)lbl_8047AD40;
    r3 = 0x1;
    r0 = r4 + 0x1;
    *(u32*)lbl_8047AD40 = r0;
L_8010A878: ;
    /* lmw r23, 0x4c(r1) */;
    return;
}
#pragma pop

/* 0x8010A88C | 0x274 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010A88C(void) {
    extern u8 lbl_8047AD44[];
    extern u8 lbl_8047CE70[];
    extern u8 lbl_8047CE78[];
    extern u8 lbl_8047CE7C[];
    extern u8 lbl_8047CE80[];
    extern void fn_800E01F4();
    extern void fn_800E4014();
    extern void fn_800E4170();
    extern void fn_800EC1BC();
    extern void fn_800EC990();
    extern void fn_800EC9DC();
    extern void fn_800ECB74();
    extern void fn_800ECCA8();
    extern void fn_8010AB00();
    extern void fn_8010B560();
    extern void fn_80113D58();
    extern void fn_801DA914();
    extern void fn_801DA9E8();
    extern void fn_801DAC3C();
    extern void fn_801DADC0();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fn_801DE190();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010A8B4;
    r3 = 0x0;
    goto L_8010AAE8;
L_8010A8B4: ;
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r0 != (u32)0x0) goto L_8010A8D0;
    r3 = 0x0;
    goto L_8010AAE8;
    goto L_8010A8D0;
L_8010A8CC: ;
    ((void(*)(void))fn_800F0308)();
L_8010A8D0: ;
    fn_8010B560();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010A8CC;
    r0 = *(u8*)lbl_8047AD44;
    if ((u32)r0 != (u32)0x0) goto L_8010A8FC;
    r0 = 0x1;
    r3 = 0x4;
    *(u8*)lbl_8047AD44 = r0;
    fn_801DADC0();
L_8010A8FC: ;
    if ((u32)r31 == (u32)0x0) goto L_8010A954;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_8010A934;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_8010A94C;
    fn_801DB100();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
    goto L_8010A94C;
L_8010A934: ;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 == (u32)0x0) goto L_8010A94C;
    ((void(*)(void))fn_800E4BF4)();
    r0 = 0x0;
    *(u32*)((u8*)r31 + 0x24) = r0;
L_8010A94C: ;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
L_8010A954: ;
    r3 = 0x1;
    r0 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r3;
    *(u8*)((u8*)r31 + 0x1) = r0;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r0 = *(u32*)((u8*)r31 + 0x8);
    *(u32*)((u8*)r31 + 0x14) = r3;
    *(u32*)((u8*)r31 + 0x18) = r0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r0 = *(u32*)((u8*)r31 + 0x10);
    *(u32*)((u8*)r31 + 0x1C) = r3;
    *(u32*)((u8*)r31 + 0x20) = r0;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_8010AA34;
    r3 = *(u16*)((u8*)r31 + 0x18);
    r4 = *(u32*)((u8*)r31 + 0x1C);
    r5 = *(u8*)((u8*)r31 + 0x22);
    fn_801DE190();
    *(u32*)((u8*)r31 + 0x24) = r3;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 != (u32)0x0) goto L_8010A9C0;
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_8010AAE8;
L_8010A9C0: ;
    fn_801DAC3C();
    r30 = r3;
    if ((u32)r30 != (u32)0x0) goto L_8010A9E0;
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_8010AAE8;
L_8010A9E0: ;
    r4 = 0x0;
    fn_800E4014();
    r4 = *(u16*)((u8*)r31 + 0x1A);
    if ((u32)r4 == (u32)0x0) goto L_8010AA94;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r5 = 0x4;
    r6 = 0x0;
    fn_801DDD28();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010AA94;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r5 = 0x4;
    r4 = *(u16*)((u8*)r31 + 0x1A);
    fn_801DA914();
    r3 = *(u32*)((u8*)r31 + 0x24);
    r5 = 0x4;
    r4 = *(u16*)((u8*)r31 + 0x1A);
    fn_801DA9E8();
    goto L_8010AA94;
L_8010AA34: ;
    r3 = *(u32*)((u8*)r31 + 0x18);
    fn_80113D58();
    *(u32*)((u8*)r31 + 0x24) = r3;
    r3 = *(u32*)((u8*)r31 + 0x24);
    if ((u32)r3 != (u32)0x0) goto L_8010AA54;
    r3 = 0x0;
    goto L_8010AAE8;
L_8010AA54: ;
    fn_800EC1BC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010AA84;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = *(u32*)((u8*)r31 + 0x1C);
    fn_800ECCA8();
    r3 = *(u32*)((u8*)r31 + 0x24);
    f1 = *(f32*)lbl_8047CE70;
    fn_800EC9DC();
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_800EC990();
L_8010AA84: ;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r4 = 0x0;
    fn_800E4014();
    r30 = *(u32*)((u8*)r31 + 0x24);
L_8010AA94: ;
    r3 = r30;
    r4 = 0x1;
    fn_800ECB74();
    f1 = *(f32*)lbl_8047CE78;
    r3 = r1 + 0x8;
    f2 = *(f32*)lbl_8047CE7C;
    f3 = *(f32*)lbl_8047CE80;
    fn_800E01F4();
    r3 = r30;
    r4 = r1 + 0x8;
    fn_800E4170();
    r3 = r31;
    fn_8010AB00();
    r0 = 0x2;
    *(u8*)((u8*)r31 + 0x0) = r0;
    r0 = *(u8*)((u8*)r31 + 0x1);
    if ((u32)r0 == (u32)0x1) goto L_8010A8FC;
    r0 = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r31 + 0x28) = r0;
L_8010AAE8: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    return;
}
#pragma pop

/* 0x8010AB00 | 0x32C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010AB00(void) {
    extern u8 lbl_80271F38[];
    extern u8 lbl_8035B430[];
    extern u8 lbl_8035B43C[];
    extern u8 lbl_8047CE80[];
    extern u8 lbl_8047CE84[];
    extern u8 lbl_8047CE88[];
    extern u8 lbl_8047CE8C[];
    extern u8 lbl_8047CE90[];
    extern u8 lbl_8047CE94[];
    extern u8 lbl_8047CE98[];
    extern u8 lbl_8047CE9C[];
    extern u8 lbl_8047CEA0[];
    extern u8 lbl_8047CEA4[];
    extern u8 lbl_8047CEA8[];
    extern u8 lbl_8047CEB0[];
    extern void fn_800CE220();
    extern void fn_800D1F04();
    extern void fn_800D1FDC();
    extern void fn_800D207C();
    extern void fn_800D20CC();
    extern void fn_800DCC34();
    extern void fn_800DCC3C();
    extern void fn_800DCC60();
    extern void fn_800DCC84();
    extern void fn_800DCCF0();
    extern void fn_800E01F4();
    extern void fn_800E3C5C();
    extern void fn_800E6B20();
    extern void fn_800E6BC8();
    extern void fn_800EE0E8();
    extern void fn_800EE150();
    extern void fn_800EE3BC();
    extern void fn_800EE828();
    extern void fn_80190E34();
    extern void fn_801DAC24();
    extern void fn_801DAC3C();
    extern void fn_8025FA20();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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

    *(f64*)(sp + 0x90) = f31;
    /* psq_st f31, 0x98(r1), 0, qr0 */;
    *(f64*)(sp + 0x80) = f30;
    /* psq_st f30, 0x88(r1), 0, qr0 */;
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010AB3C;
    r3 = 0x0;
    goto L_8010AE00;
L_8010AB3C: ;
    r0 = *(u8*)((u8*)r31 + 0x14);
    if ((u32)r0 == (u32)0x0) goto L_8010ABDC;
    r3 = *(u32*)((u8*)r31 + 0x24);
    fn_801DAC3C();
    r0 = r3;
    r3 = *(u32*)((u8*)r31 + 0x24);
    r30 = r0;
    fn_801DAC24();
    if ((s32)r3 == (s32)0x1) goto L_8010ABAC;
    if ((s32)r3 >= (s32)0x1) goto L_8010AB84;
    if ((s32)r3 == (s32)-0x1) goto L_8010AB9C;
    if ((s32)r3 >= (s32)-0x1) goto L_8010ABA4;
    if ((s32)r3 >= (s32)-0x2) goto L_8010AB94;
    goto L_8010ABC4;
L_8010AB84: ;
    if ((s32)r3 == (s32)0x3) goto L_8010ABBC;
    if ((s32)r3 >= (s32)0x3) goto L_8010ABC4;
    goto L_8010ABB4;
L_8010AB94: ;
    f31 = *(f32*)lbl_8047CE84;
    goto L_8010ABC8;
L_8010AB9C: ;
    f31 = *(f32*)lbl_8047CE88;
    goto L_8010ABC8;
L_8010ABA4: ;
    f31 = *(f32*)lbl_8047CE8C;
    goto L_8010ABC8;
L_8010ABAC: ;
    f31 = *(f32*)lbl_8047CE90;
    goto L_8010ABC8;
L_8010ABB4: ;
    f31 = *(f32*)lbl_8047CE94;
    goto L_8010ABC8;
L_8010ABBC: ;
    f31 = *(f32*)lbl_8047CE98;
    goto L_8010ABC8;
L_8010ABC4: ;
    f31 = *(f32*)lbl_8047CE8C;
L_8010ABC8: ;
    r3 = *(u16*)((u8*)r31 + 0x20);
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    fn_8025FA20();
    goto L_8010ABE8;
L_8010ABDC: ;
    r30 = *(u32*)((u8*)r31 + 0x24);
    f31 = *(f32*)lbl_8047CE8C;
    *(f32*)(sp + 0xC) = f31;
L_8010ABE8: ;
    if ((u32)r30 != (u32)0x0) goto L_8010AC00;
    r0 = 0x0;
    r3 = 0x0;
    *(u8*)((u8*)r31 + 0x0) = r0;
    goto L_8010AE00;
L_8010AC00: ;
    r3 = r30;
    fn_800E6BC8();
    r3 = r30;
    fn_800EE0E8();
    r4 = r3;
    r3 = r30;
    /* subi r4, r4, 0x1 */;
    fn_800EE150();
    r29 = r3;
    if ((u32)r29 == (u32)0x0) goto L_8010AC44;
    r4 = r1 + 0x38;
    r5 = 0x0;
    r6 = 0x0;
    fn_800EE3BC();
    r3 = r29;
    fn_800EE828();
L_8010AC44: ;
    r3 = r30;
    fn_800E6B20();
    r3 = r30;
    fn_800E3C5C();
    r4 = r1 + 0x50;
    fn_80190E34();
    r3 = *(u32*)((u8*)r31 + 0x38);
    if ((u32)r3 != (u32)0x0) goto L_8010AC70;
    r3 = 0x0;
    goto L_8010AE00;
L_8010AC70: ;
    r4 = r1 + 0x1c;
    r5 = r1 + 0x18;
    r6 = r1 + 0x14;
    r7 = r1 + 0x10;
    fn_800D1FDC();
    r4 = *(u32*)((u8*)r31 + 0x2C);
    r3 = (0x4330 << 16);
    r0 = *(u32*)((u8*)r31 + 0x30);
    /* xoris r4, r4, 0x8000 */;
    f0 = *(f32*)lbl_8047CE9C;
    /* xoris r0, r0, 0x8000 */;
    f3 = *(f64*)lbl_8047CEB0;
    f4 = *(f32*)(sp + 0x50);
    f1 = *(f64*)(sp + 0x60);
    *(u32*)(sp + 0x6C) = r0;
    f2 = f1 - f3;
    f5 = *(f32*)(sp + 0x54);
    f1 = *(f64*)(sp + 0x68);
    *(f32*)(sp + 0x1C) = f0;
    f0 = f1 - f3;
    f0 = f2 / f0;
    *(f32*)(sp + 0x18) = f0;
    if (f4 <= f5) goto L_8010ACE4;
    f30 = f4;
    goto L_8010ACE8;
L_8010ACE4: ;
    f30 = f5;
L_8010ACE8: ;
    f1 = *(f32*)lbl_8047CEA0;
    fn_800CE220();
    f3 = f30 / f31;
    f4 = *(f32*)lbl_8047CE84;
    f0 = *(f32*)(sp + 0xC);
    r3 = r1 + 0x44;
    f2 = *(f32*)(sp + 0x3C);
    f5 = (f32)f1;
    f3 = f4 * f3;
    f1 = *(f32*)lbl_8047CE8C;
    f3 = f3 / f5;
    f3 = f3 * f0;
    fn_800E01F4();
    r3 = *(u32*)((u8*)r31 + 0x38);
    r4 = r1 + 0x44;
    fn_800D207C();
    r3 = *(u32*)((u8*)r31 + 0x38);
    f1 = *(f32*)(sp + 0x1C);
    f2 = *(f32*)(sp + 0x18);
    f3 = *(f32*)(sp + 0x14);
    f4 = *(f32*)(sp + 0x10);
    fn_800D20CC();
    r3 = (u32)lbl_8035B43C;
    f1 = *(f32*)lbl_8047CE80;
    f2 = *(f32*)(sp + 0x3C);
    r3 = (u32)lbl_8035B43C;
    f3 = *(f32*)(sp + 0x40);
    fn_800E01F4();
    r3 = (u32)lbl_8035B430;
    r5 = (u32)lbl_8035B43C;
    r4 = (u32)lbl_8035B430;
    r3 = *(u32*)((u8*)r31 + 0x38);
    r5 = (u32)lbl_8035B43C;
    fn_800D1F04();
    r4 = (u32)lbl_80271F38;
    r3 = r1 + 0x20;
    r8 = (u32)lbl_80271F38;
    r4 = r1 + 0x44;
    r7 = *(u32*)((u8*)r8 + 0x0);
    r5 = 0xc;
    r6 = *(u32*)((u8*)r8 + 0x4);
    r0 = *(u32*)((u8*)r8 + 0x8);
    *(u32*)(sp + 0x34) = r0;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    f3 = *(f32*)(sp + 0x20);
    r4 = 0x2;
    f2 = *(f32*)lbl_8047CEA4;
    f1 = *(f32*)(sp + 0x24);
    f0 = *(f32*)lbl_8047CEA8;
    f2 = f3 - f2;
    f0 = f1 + f0;
    *(f32*)(sp + 0x20) = f2;
    *(f32*)(sp + 0x24) = f0;
    r3 = *(u32*)((u8*)r31 + 0x44);
    fn_800DCCF0();
    r3 = *(u32*)((u8*)r31 + 0x44);
    r4 = r1 + 0x2c;
    fn_800DCC84();
    r3 = *(u32*)((u8*)r31 + 0x44);
    r4 = r1 + 0x20;
    fn_800DCC60();
    r3 = *(u32*)((u8*)r31 + 0x44);
    r4 = r1 + 0x38;
    fn_800DCC3C();
    r3 = *(u32*)((u8*)r31 + 0x44);
    r4 = 0x1;
    fn_800DCC34();
    r3 = 0x1;
L_8010AE00: ;
    /* psq_l f31, 0x98(r1), 0, qr0 */;
    f31 = *(f64*)(sp + 0x90);
    /* psq_l f30, 0x88(r1), 0, qr0 */;
    f30 = *(f64*)(sp + 0x80);
    r31 = *(u32*)(sp + 0x7C);
    r30 = *(u32*)(sp + 0x78);
    r29 = *(u32*)(sp + 0x74);
    return;
}
#pragma pop

/* 0x8010AE2C | 0x1F0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010AE2C(void) {
    extern u8 lbl_8035B478[];
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_8010B16C();
    extern void fn_8010C3FC();
    extern void fn_8011E8DC();
    extern void fn_8011F5B0();
    extern void fn_80122334();
    extern void fn_80123FBC();
    extern void fn_80125390();
    extern void fn_8012640C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r4;
    r31 = r5;
    r4 = 0x0;
    r5 = 0xcc;
    r6 = 0x0;
    fn_8012640C();
    r28 = r3;
    if ((u32)r28 != (u32)0x0) goto L_8010AE74;
    r29 = 0x0;
    goto L_8010AF60;
L_8010AE74: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010AE8C;
    r29 = 0x33d;
    goto L_8010AF60;
L_8010AE8C: ;
    r3 = r28;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010AEA8;
    r29 = 0x33d;
    goto L_8010AF60;
L_8010AEA8: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    if ((u32)r29 != (u32)0xc9) goto L_8010AF18;
    r3 = r28;
    fn_8011F5B0();
    fn_80122334();
    r29 = r3 & 0xFF;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010AF00;
    r3 = (u32)lbl_8035B478;
    r0 = r29 << 2;
    r3 = (u32)lbl_8035B478;
    r29 = *(u16*)(r3 + r0);
    goto L_8010AF60;
L_8010AF00: ;
    r3 = (u32)lbl_8035B478;
    r0 = r29 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = r3 + r0;
    r29 = *(u16*)((u8*)r3 + 0x2);
    goto L_8010AF60;
L_8010AF18: ;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010AF48;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    goto L_8010AF60;
L_8010AF48: ;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x1;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
L_8010AF60: ;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8010B16C();
L_8010AF70: ;
    r3 = r29;
    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010AFB0;
L_8010AF94: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010AFA4;
    goto L_8010AFB4;
L_8010AFA4: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010AF94;
L_8010AFB0: ;
    r4 = -0x1;
L_8010AFB4: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010AFC4;
    r0 = -0x1;
    goto L_8010AFE4;
L_8010AFC4: ;
    r3 = r4 << 4;
    r0 = r3 + 0x6;
    r0 = *(u8*)(r6 + r0);
    if ((u32)r0 != (u32)0x2) goto L_8010AFE0;
    r0 = 0x1;
    goto L_8010AFE4;
L_8010AFE0: ;
    r0 = 0x0;
L_8010AFE4: ;
    r0 = (s8)r0;
    if ((s32)r0 != (s32)0x0) goto L_8010AFF8;
    ((void(*)(void))fn_800F0308)();
    goto L_8010AF70;
L_8010AFF8: ;
    r3 = 0x1;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x8010B01C | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B01C(void) {
    extern u8 lbl_8035B478[];
    extern void fn_8010B16C();
    extern void fn_8011E8DC();
    extern void fn_8011F5B0();
    extern void fn_80122334();
    extern void fn_80123FBC();
    extern void fn_80125390();
    extern void fn_8012640C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    if ((u32)r28 != (u32)0x0) goto L_8010B054;
    r3 = 0x0;
    goto L_8010B140;
L_8010B054: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B06C;
    r3 = 0x33d;
    goto L_8010B140;
L_8010B06C: ;
    r3 = r28;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B088;
    r3 = 0x33d;
    goto L_8010B140;
L_8010B088: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    if ((u32)r31 != (u32)0xc9) goto L_8010B0F8;
    r3 = r28;
    fn_8011F5B0();
    fn_80122334();
    r31 = r3 & 0xFF;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B0E0;
    r3 = (u32)lbl_8035B478;
    r0 = r31 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = *(u16*)(r3 + r0);
    goto L_8010B140;
L_8010B0E0: ;
    r3 = (u32)lbl_8035B478;
    r0 = r31 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = r3 + r0;
    r3 = *(u16*)((u8*)r3 + 0x2);
    goto L_8010B140;
L_8010B0F8: ;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B128;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    goto L_8010B140;
L_8010B128: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x1;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
L_8010B140: ;
    r4 = r29;
    r5 = r30;
    fn_8010B16C();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x8010B16C | 0x3F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B16C(void) {
    extern u8 lbl_8035B478[];
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_8010BD6C();
    extern void fn_8010C3FC();
    extern void fn_8011E8DC();
    extern void fn_8011F5B0();
    extern void fn_80122334();
    extern void fn_80123FBC();
    extern void fn_80125390();
    extern void fn_8012640C();
    extern void fn_8017B000();
    extern void fn_8017B1CC();
    extern void fn_8010B5C4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r29 = r3;
    r30 = r4;
    r31 = r5;
L_8010B194: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B2C8;
    r28 = 0x0;
    if ((u32)r30 == (u32)0x0) goto L_8010B1C0;
    r12 = r30;
    r3 = r31;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r28 = r3;
L_8010B1C0: ;
    if ((u32)r28 == (u32)0x0) goto L_8010B2C8;
    if ((u32)r28 != (u32)0x0) goto L_8010B1D4;
    r0 = 0x0;
    goto L_8010B2C4;
L_8010B1D4: ;
    r3 = r28;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B1F0;
    r0 = 0x33d;
    goto L_8010B2C4;
L_8010B1F0: ;
    r3 = r28;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B20C;
    r0 = 0x33d;
    goto L_8010B2C4;
L_8010B20C: ;
    r3 = r28;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r29 = r3 & 0xFFFF;
    if ((u32)r29 != (u32)0xc9) goto L_8010B27C;
    r3 = r28;
    fn_8011F5B0();
    fn_80122334();
    r29 = r3 & 0xFF;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B264;
    r3 = (u32)lbl_8035B478;
    r0 = r29 << 2;
    r3 = (u32)lbl_8035B478;
    r0 = *(u16*)(r3 + r0);
    goto L_8010B2C4;
L_8010B264: ;
    r3 = (u32)lbl_8035B478;
    r4 = r29 << 2;
    r0 = (u32)lbl_8035B478;
    r3 = r0 + r4;
    r0 = *(u16*)((u8*)r3 + 0x2);
    goto L_8010B2C4;
L_8010B27C: ;
    r3 = r28;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B2AC;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
    goto L_8010B2C4;
L_8010B2AC: ;
    r4 = r29;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x1;
    fn_8012640C();
    r0 = r3 & 0xFFFF;
L_8010B2C4: ;
    r29 = r0;
L_8010B2C8: ;
    r0 = r29 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B3CC;
    r6 = *(u32*)lbl_8047AD4C;
    r3 = 0x0;
    r5 = *(u32*)lbl_8047AD48;
    r4 = r6;
    ctr_fn = (void(*)(void))r5;
    if ((s32)r5 <= (s32)0x0) goto L_8010B308;
L_8010B2F0: ;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8010B308;
    r4 = r4 + 0x10;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_8010B2F0;
L_8010B308: ;
    r4 = r6;
    ctr_fn = (void(*)(void))r3;
    if ((s32)r3 <= (s32)0x0) goto L_8010B334;
L_8010B318: ;
    r0 = *(u8*)((u8*)r4 + 0x6);
    if ((u32)r0 != (u32)0x1) goto L_8010B32C;
    r0 = 0x1;
    goto L_8010B338;
L_8010B32C: ;
    r4 = r4 + 0x10;
    if (--ctr != 0) goto L_8010B318;
L_8010B334: ;
    r0 = 0x0;
L_8010B338: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B34C;
    r3 = 0x0;
    goto L_8010B540;
L_8010B34C: ;
    r3 = r6;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r5;
    if ((s32)r5 <= (s32)0x0) goto L_8010B378;
L_8010B360: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8010B378;
    r3 = r3 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010B360;
L_8010B378: ;
    r3 = r6;
    r5 = 0x0;
    ctr_fn = (void(*)(void))r4;
    if ((s32)r4 <= (s32)0x0) goto L_8010B3B4;
L_8010B38C: ;
    r0 = *(u8*)((u8*)r3 + 0x6);
    if ((u32)r0 != (u32)0x0) goto L_8010B3A8;
    r3 = r5 << 4;
    r0 = r3 + 0x4;
    r29 = *(u16*)(r6 + r0);
    goto L_8010B3B4;
L_8010B3A8: ;
    r3 = r3 + 0x10;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_8010B38C;
L_8010B3B4: ;
    if ((s32)r5 != (s32)r4) goto L_8010B43C;
    r3 = 0x5c0;
    fn_8017B1CC();
    r3 = 0x0;
    goto L_8010B540;
L_8010B3CC: ;
    r3 = r29;
    fn_8010C3FC();
    r5 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010B408;
L_8010B3EC: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010B3FC;
    goto L_8010B40C;
L_8010B3FC: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010B3EC;
L_8010B408: ;
    r4 = -0x1;
L_8010B40C: ;
    if ((s32)r4 < (s32)0x0) goto L_8010B42C;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8010BD6C();
    r29 = 0x0;
    goto L_8010B194;
L_8010B42C: ;
    r3 = r29;
    r4 = r30;
    r5 = r31;
    fn_8010BD6C();
L_8010B43C: ;
    r3 = r29;
    fn_8010C3FC();
    r28 = r3;
    if ((u32)r28 != (u32)0x0) goto L_8010B458;
    r29 = 0x0;
    goto L_8010B194;
L_8010B458: ;
    r5 = *(u32*)lbl_8047AD4C;
    r3 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r4 = r5;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010B48C;
L_8010B474: ;
    r0 = *(u32*)((u8*)r4 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8010B48C;
    r4 = r4 + 0x10;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_8010B474;
L_8010B48C: ;
    ctr_fn = (void(*)(void))r3;
    if ((s32)r3 <= (s32)0x0) goto L_8010B4B4;
L_8010B498: ;
    r0 = *(u8*)((u8*)r5 + 0x6);
    if ((u32)r0 != (u32)0x1) goto L_8010B4AC;
    r0 = 0x1;
    goto L_8010B4B8;
L_8010B4AC: ;
    r5 = r5 + 0x10;
    if (--ctr != 0) goto L_8010B498;
L_8010B4B4: ;
    r0 = 0x0;
L_8010B4B8: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B4CC;
    r3 = 0x1;
    goto L_8010B540;
L_8010B4CC: ;
    r3 = r29;
    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010B50C;
L_8010B4F0: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010B500;
    goto L_8010B510;
L_8010B500: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010B4F0;
L_8010B50C: ;
    r4 = -0x1;
L_8010B510: ;
    r0 = r4 << 4;
    r3 = (u32)fn_8010B5C4;
    r4 = r6 + r0;
    r7 = r29 & 0xFFFF;
    r0 = 0x1;
    r5 = (u32)fn_8010B5C4;
    *(u8*)((u8*)r4 + 0x6) = r0;
    r4 = r28;
    r3 = 0x5c0;
    r6 = 0x0;
    fn_8017B000();
    r3 = 0x1;
L_8010B540: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
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
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern u8 lbl_8047AD54[];
    extern void fn_800BB29C();
    extern void fn_800EFD3C();
    extern void fn_800F9210();
    extern void fn_800F9318();
    extern void fn_800F9378();
    extern void fn_8010B16C();
    extern void fn_8010C3FC();
    extern void fn_8010C364();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r5 & 0xFFFF;
    r3 = r30;
    fn_8010C3FC();
    r31 = r3;
    r3 = r30;
    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010B628;
L_8010B60C: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010B61C;
    goto L_8010B62C;
L_8010B61C: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010B60C;
L_8010B628: ;
    r4 = -0x1;
L_8010B62C: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010B64C;
    if ((u32)r31 == (u32)0x0) goto L_8010B700;
    r4 = r31;
    r3 = 0x5c0;
    fn_800F9210();
    goto L_8010B700;
L_8010B64C: ;
    r0 = r4 << 4;
    r4 = r31;
    r30 = r6 + r0;
    r3 = 0x5c0;
    fn_800F9318();
    r0 = 0x0;
    r4 = r3;
    *(u8*)((u8*)r3 + 0x7) = r0;
    r5 = 0x6ec0;
    r0 = *(u32*)((u8*)r3 + 0x28);
    r0 = r0 - r3;
    *(u32*)((u8*)r3 + 0x28) = r0;
    r0 = *(u8*)((u8*)r30 + 0x7);
    r3 = *(u32*)lbl_8047AD54;
    r0 = (s8)r0;
    r0 = r0 << 3;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0x4);
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = r31;
    r3 = 0x5c0;
    fn_800F9210();
    r0 = *(u8*)((u8*)r30 + 0x7);
    r3 = *(u32*)lbl_8047AD54;
    r0 = (s8)r0;
    r0 = r0 << 3;
    r3 = r3 + r0;
    r3 = *(u32*)((u8*)r3 + 0x4);
    fn_800EFD3C();
    r4 = (u32)fn_8010C364;
    r5 = r31;
    r6 = (u32)fn_8010C364;
    r31 = r3;
    r4 = 0x5c0;
    fn_800F9378();
    r3 = r31;
    r4 = 0x6ec0;
    DCFlushRange();
    fn_800BB29C();
    r0 = 0x2;
    r3 = 0x0;
    *(u8*)((u8*)r30 + 0x6) = r0;
    r4 = *(u32*)((u8*)r30 + 0x8);
    r5 = *(u32*)((u8*)r30 + 0xC);
    fn_8010B16C();
L_8010B700: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8010B718 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B718(void) {
    extern u8 lbl_8035B478[];
    extern u8 lbl_80404BF0[];
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800F92D4();
    extern void fn_80108580();
    extern void fn_8010C388();
    extern void fn_8010C3FC();
    extern void fn_8011E8DC();
    extern void fn_8011F5B0();
    extern void fn_80122334();
    extern void fn_80123FBC();
    extern void fn_80125390();
    extern void fn_8012640C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r26, 0x8(r1) */;
    r28 = r3;
    r29 = r4;
    r27 = r5;
    if ((u32)r27 != (u32)0x0) goto L_8010B744;
    r30 = 0x0;
    goto L_8010B834;
L_8010B744: ;
    r3 = r27;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B760;
    r30 = 0x33d;
    goto L_8010B834;
L_8010B760: ;
    r3 = r27;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B77C;
    r30 = 0x33d;
    goto L_8010B834;
L_8010B77C: ;
    r3 = r27;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r26 = r3 & 0xFFFF;
    if ((u32)r26 != (u32)0xc9) goto L_8010B7EC;
    r3 = r27;
    fn_8011F5B0();
    fn_80122334();
    r26 = r3 & 0xFF;
    r3 = r27;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B7D4;
    r3 = (u32)lbl_8035B478;
    r0 = r26 << 2;
    r3 = (u32)lbl_8035B478;
    r30 = *(u16*)(r3 + r0);
    goto L_8010B834;
L_8010B7D4: ;
    r3 = (u32)lbl_8035B478;
    r0 = r26 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = r3 + r0;
    r30 = *(u16*)((u8*)r3 + 0x2);
    goto L_8010B834;
L_8010B7EC: ;
    r3 = r27;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010B81C;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
    goto L_8010B834;
L_8010B81C: ;
    r4 = r26;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x1;
    fn_8012640C();
    r30 = r3 & 0xFFFF;
L_8010B834: ;
    r3 = r30;
    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010B874;
L_8010B858: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010B868;
    goto L_8010B878;
L_8010B868: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010B858;
L_8010B874: ;
    r4 = -0x1;
L_8010B878: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010B888;
    r0 = -0x1;
    goto L_8010B8A8;
L_8010B888: ;
    r0 = r4 << 4;
    r3 = r6 + r0;
    r0 = *(u8*)((u8*)r3 + 0x6);
    if ((u32)r0 != (u32)0x2) goto L_8010B8A4;
    r0 = 0x1;
    goto L_8010B8A8;
L_8010B8A4: ;
    r0 = 0x0;
L_8010B8A8: ;
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x1) goto L_8010B8BC;
    r3 = 0x0;
    goto L_8010B9D4;
L_8010B8BC: ;
    r3 = r30;
    fn_8010C3FC();
    r26 = r3;
    if ((u32)r26 != (u32)0x0) goto L_8010B8D8;
    r3 = 0x0;
    goto L_8010B9D4;
L_8010B8D8: ;
    fn_800F92D4();
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010B8F0;
    r3 = 0x0;
    goto L_8010B9D4;
L_8010B8F0: ;
    r3 = (u32)lbl_80404BF0;
    r0 = 0xf;
    r3 = (u32)lbl_80404BF0;
    /* subi r4, r29, 0x4 */;
    /* subi r5, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8010B908: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8010B908;
    r3 = (u32)lbl_80404BF0;
    r4 = 0x0;
    r27 = (u32)lbl_80404BF0;
    r0 = 0x2a;
    *(u32*)((u8*)r27 + 0x58) = r26;
    r3 = r30;
    *(u16*)((u8*)r27 + 0x50) = r4;
    *(u16*)((u8*)r27 + 0x52) = r4;
    *(u16*)((u8*)r27 + 0x5C) = r4;
    *(u16*)((u8*)r27 + 0x5E) = r4;
    *(u16*)((u8*)r27 + 0x60) = r0;
    *(u16*)((u8*)r27 + 0x62) = r0;
    fn_8010C388();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010B978;
    r3 = r31;
    fn_800EF4F4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x2a) goto L_8010B978;
    r0 = 0x2a;
    *(u16*)((u8*)r27 + 0x5E) = r0;
L_8010B978: ;
    r3 = (u32)lbl_80404BF0;
    r29 = (u32)lbl_80404BF0;
    r0 = *(s16*)((u8*)r29 + 0x54);
    if ((s32)r0 >= (s32)0x0) goto L_8010B9C0;
    r3 = r31;
    fn_800EF4FC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x2a) goto L_8010B9C0;
    r3 = *(s16*)((u8*)r29 + 0x54);
    r0 = 0x2a;
    *(u16*)((u8*)r27 + 0x5C) = r0;
    if ((s32)r3 >= (s32)0x0) goto L_8010B9B8;
    r3 = -r3;
L_8010B9B8: ;
    r0 = (s16)r3;
    *(u16*)((u8*)r29 + 0x54) = r0;
L_8010B9C0: ;
    r4 = (u32)lbl_80404BF0;
    r3 = r28;
    r4 = (u32)lbl_80404BF0;
    fn_80108580();
    r3 = 0x1;
L_8010B9D4: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8010B9E8 | 0x1D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010B9E8(void) {
    extern u8 lbl_80404BF0[];
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_800EF4F4();
    extern void fn_800EF4FC();
    extern void fn_800F92D4();
    extern void fn_80108580();
    extern void fn_8010C388();
    extern void fn_8010C3FC();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r26, 0x8(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r5;
    r3 = r30;
    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010BA44;
L_8010BA28: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010BA38;
    goto L_8010BA48;
L_8010BA38: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010BA28;
L_8010BA44: ;
    r4 = -0x1;
L_8010BA48: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010BA58;
    r0 = -0x1;
    goto L_8010BA78;
L_8010BA58: ;
    r0 = r4 << 4;
    r3 = r6 + r0;
    r0 = *(u8*)((u8*)r3 + 0x6);
    if ((u32)r0 != (u32)0x2) goto L_8010BA74;
    r0 = 0x1;
    goto L_8010BA78;
L_8010BA74: ;
    r0 = 0x0;
L_8010BA78: ;
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x1) goto L_8010BA8C;
    r3 = 0x0;
    goto L_8010BBA4;
L_8010BA8C: ;
    r3 = r30;
    fn_8010C3FC();
    r26 = r3;
    if ((u32)r26 != (u32)0x0) goto L_8010BAA8;
    r3 = 0x0;
    goto L_8010BBA4;
L_8010BAA8: ;
    fn_800F92D4();
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010BAC0;
    r3 = 0x0;
    goto L_8010BBA4;
L_8010BAC0: ;
    r3 = (u32)lbl_80404BF0;
    r0 = 0xf;
    r3 = (u32)lbl_80404BF0;
    /* subi r4, r29, 0x4 */;
    /* subi r5, r3, 0x4 */;
    ctr_fn = (void(*)(void))r0;
L_8010BAD8: ;
    r3 = *(u32*)((u8*)r4 + 0x4);
    r0 = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = r0;
    if (--ctr != 0) goto L_8010BAD8;
    r3 = (u32)lbl_80404BF0;
    r4 = 0x0;
    r27 = (u32)lbl_80404BF0;
    r0 = 0x2a;
    *(u32*)((u8*)r27 + 0x58) = r26;
    r3 = r30;
    *(u16*)((u8*)r27 + 0x50) = r4;
    *(u16*)((u8*)r27 + 0x52) = r4;
    *(u16*)((u8*)r27 + 0x5C) = r4;
    *(u16*)((u8*)r27 + 0x5E) = r4;
    *(u16*)((u8*)r27 + 0x60) = r0;
    *(u16*)((u8*)r27 + 0x62) = r0;
    fn_8010C388();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010BB48;
    r3 = r31;
    fn_800EF4F4();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x2a) goto L_8010BB48;
    r0 = 0x2a;
    *(u16*)((u8*)r27 + 0x5E) = r0;
L_8010BB48: ;
    r3 = (u32)lbl_80404BF0;
    r29 = (u32)lbl_80404BF0;
    r0 = *(s16*)((u8*)r29 + 0x54);
    if ((s32)r0 >= (s32)0x0) goto L_8010BB90;
    r3 = r31;
    fn_800EF4FC();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0x2a) goto L_8010BB90;
    r3 = *(s16*)((u8*)r29 + 0x54);
    r0 = 0x2a;
    *(u16*)((u8*)r27 + 0x5C) = r0;
    if ((s32)r3 >= (s32)0x0) goto L_8010BB88;
    r3 = -r3;
L_8010BB88: ;
    r0 = (s16)r3;
    *(u16*)((u8*)r29 + 0x54) = r0;
L_8010BB90: ;
    r4 = (u32)lbl_80404BF0;
    r3 = r28;
    r4 = (u32)lbl_80404BF0;
    fn_80108580();
    r3 = 0x1;
L_8010BBA4: ;
    /* lmw r26, 0x8(r1) */;
    return;
}
#pragma pop

/* 0x8010BBB8 | 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BBB8(void) {
    extern u8 lbl_8035B478[];
    extern void fn_8011E8DC();
    extern void fn_8011F5B0();
    extern void fn_80122334();
    extern void fn_80123FBC();
    extern void fn_80125390();
    extern void fn_8012640C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    if ((u32)r30 != (u32)0x0) goto L_8010BBE0;
    r3 = 0x0;
    goto L_8010BCCC;
L_8010BBE0: ;
    fn_80123FBC();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010BBF8;
    r3 = 0x33d;
    goto L_8010BCCC;
L_8010BBF8: ;
    r3 = r30;
    fn_8011E8DC();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_8010BC14;
    r3 = 0x33d;
    goto L_8010BCCC;
L_8010BC14: ;
    r3 = r30;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    fn_8012640C();
    r31 = r3 & 0xFFFF;
    if ((u32)r31 != (u32)0xc9) goto L_8010BC84;
    r3 = r30;
    fn_8011F5B0();
    fn_80122334();
    r31 = r3 & 0xFF;
    r3 = r30;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010BC6C;
    r3 = (u32)lbl_8035B478;
    r0 = r31 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = *(u16*)(r3 + r0);
    goto L_8010BCCC;
L_8010BC6C: ;
    r3 = (u32)lbl_8035B478;
    r0 = r31 << 2;
    r3 = (u32)lbl_8035B478;
    r3 = r3 + r0;
    r3 = *(u16*)((u8*)r3 + 0x2);
    goto L_8010BCCC;
L_8010BC84: ;
    r3 = r30;
    fn_80125390();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8010BCB4;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x0;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
    goto L_8010BCCC;
L_8010BCB4: ;
    r4 = r31;
    r3 = 0x0;
    r5 = 0x5b;
    r6 = 0x1;
    fn_8012640C();
    r3 = r3 & 0xFFFF;
L_8010BCCC: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x8010BCE4 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BCE4(void) {
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_8010C3FC();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    fn_8010C3FC();
    r6 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    r5 = r6;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010BD2C;
L_8010BD10: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010BD20;
    goto L_8010BD30;
L_8010BD20: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010BD10;
L_8010BD2C: ;
    r4 = -0x1;
L_8010BD30: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010BD40;
    r3 = -0x1;
    goto L_8010BD5C;
L_8010BD40: ;
    r0 = r4 << 4;
    r3 = r6 + r0;
    r0 = *(u8*)((u8*)r3 + 0x6);
    r0 = 0x2 - r0;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r3 = (s8)r0;
L_8010BD5C: ;
    return;
}
#pragma pop

/* 0x8010BD6C | 0x4B4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8010BD6C(void) {
    extern u8 lbl_8047AD48[];
    extern u8 lbl_8047AD4C[];
    extern void fn_800F9210();
    extern void fn_8010C3FC();
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r27, 0x2c(r1) */;
    r27 = r3;
    r28 = r4;
    r29 = r5;
    fn_8010C3FC();
    r31 = r3;
    if ((u32)r31 != (u32)0x0) goto L_8010BDA0;
    r3 = -0x2;
    goto L_8010C20C;
L_8010BDA0: ;
    r3 = *(u32*)lbl_8047AD4C;
    r30 = 0x0;
    r0 = *(u32*)lbl_8047AD48;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r0 <= (s32)0x0) goto L_8010BDD0;
L_8010BDB8: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_8010BDD0;
    r3 = r3 + 0x10;
    r30 = r30 + 0x1;
    if (--ctr != 0) goto L_8010BDB8;
L_8010BDD0: ;
    r3 = r27;
    fn_8010C3FC();
    r7 = *(u32*)lbl_8047AD4C;
    r4 = 0x0;
    r6 = *(u32*)lbl_8047AD48;
    r5 = r7;
    ctr_fn = (void(*)(void))r6;
    if ((s32)r6 <= (s32)0x0) goto L_8010BE10;
L_8010BDF4: ;
    r0 = *(u32*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8010BE04;
    goto L_8010BE14;
L_8010BE04: ;
    r5 = r5 + 0x10;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_8010BDF4;
L_8010BE10: ;
    r4 = -0x1;
L_8010BE14: ;
    if ((s32)r4 >= (s32)0x0) goto L_8010C09C;
    if ((s32)r30 != (s32)r6) goto L_8010C054;
    r0 = *(u8*)((u8*)r7 + 0x6);
    r4 = *(u32*)((u8*)r7 + 0x0);
    r5 = *(u32*)((u8*)r7 + 0x4);
    r3 = *(u32*)((u8*)r7 + 0x8);
    r0 = *(u32*)((u8*)r7 + 0xC);
    *(u32*)(sp + 0x24) = r0;
    if ((u32)r0 != (u32)0x2) goto L_8010BE58;
    r3 = 0x5c0;
    fn_800F9210();
L_8010BE58: ;
    /* subi r0, r30, 0x1 */;
    r4 = 0x0;
    if ((s32)r0 <= (s32)0x0) goto L_8010C020;
    /* subi r5, r30, 0x9 */;
    if ((s32)r0 <= (s32)0x8) goto L_8010BFD8;
    r3 = 0x0;
    r0 = r5 + 0x7;
    r0 = (u32)r0 >> 3;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r5 <= (s32)0x0) goto L_8010BFD8;
L_8010BE8C: ;
    r0 = *(u32*)lbl_8047AD4C;
    r4 = r4 + 0x8;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x10);
    r0 = *(u32*)((u8*)r6 + 0x14);
    *(u32*)((u8*)r6 + 0x0) = r5;
    *(u32*)((u8*)r6 + 0x4) = r0;
    r5 = *(u32*)((u8*)r6 + 0x18);
    r0 = *(u32*)((u8*)r6 + 0x1C);
    *(u32*)((u8*)r6 + 0x8) = r5;
    *(u32*)((u8*)r6 + 0xC) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x20);
    r0 = *(u32*)((u8*)r6 + 0x24);
    *(u32*)((u8*)r6 + 0x10) = r5;
    *(u32*)((u8*)r6 + 0x14) = r0;
    r5 = *(u32*)((u8*)r6 + 0x28);
    r0 = *(u32*)((u8*)r6 + 0x2C);
    *(u32*)((u8*)r6 + 0x18) = r5;
    *(u32*)((u8*)r6 + 0x1C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x30);
    r0 = *(u32*)((u8*)r6 + 0x34);
    *(u32*)((u8*)r6 + 0x20) = r5;
    *(u32*)((u8*)r6 + 0x24) = r0;
    r5 = *(u32*)((u8*)r6 + 0x38);
    r0 = *(u32*)((u8*)r6 + 0x3C);
    *(u32*)((u8*)r6 + 0x28) = r5;
    *(u32*)((u8*)r6 + 0x2C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x40);
    r0 = *(u32*)((u8*)r6 + 0x44);
    *(u32*)((u8*)r6 + 0x30) = r5;
    *(u32*)((u8*)r6 + 0x34) = r0;
    r5 = *(u32*)((u8*)r6 + 0x48);
    r0 = *(u32*)((u8*)r6 + 0x4C);
    *(u32*)((u8*)r6 + 0x38) = r5;
    *(u32*)((u8*)r6 + 0x3C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x50);
    r0 = *(u32*)((u8*)r6 + 0x54);
    *(u32*)((u8*)r6 + 0x40) = r5;
    *(u32*)((u8*)r6 + 0x44) = r0;
    r5 = *(u32*)((u8*)r6 + 0x58);
    r0 = *(u32*)((u8*)r6 + 0x5C);
    *(u32*)((u8*)r6 + 0x48) = r5;
    *(u32*)((u8*)r6 + 0x4C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x60);
    r0 = *(u32*)((u8*)r6 + 0x64);
    *(u32*)((u8*)r6 + 0x50) = r5;
    *(u32*)((u8*)r6 + 0x54) = r0;
    r5 = *(u32*)((u8*)r6 + 0x68);
    r0 = *(u32*)((u8*)r6 + 0x6C);
    *(u32*)((u8*)r6 + 0x58) = r5;
    *(u32*)((u8*)r6 + 0x5C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r5 = *(u32*)((u8*)r6 + 0x70);
    r0 = *(u32*)((u8*)r6 + 0x74);
    *(u32*)((u8*)r6 + 0x60) = r5;
    *(u32*)((u8*)r6 + 0x64) = r0;
    r5 = *(u32*)((u8*)r6 + 0x78);
    r0 = *(u32*)((u8*)r6 + 0x7C);
    *(u32*)((u8*)r6 + 0x68) = r5;
    *(u32*)((u8*)r6 + 0x6C) = r0;
    r0 = *(u32*)lbl_8047AD4C;
    r6 = r0 + r3;
    r3 = r3 + 0x80;
    r5 = *(u32*)((u8*)r6 + 0x80);
    r0 = *(u32*)((u8*)r6 + 0x84);
    *(u32*)((u8*)r6 + 0x70) = r5;
    *(u32*)((u8*)r6 + 0x74) = r0;
    r5 = *(u32*)((u8*)r6 + 0x88);
    r0 = *(u32*)((u8*)r6 + 0x8C);
    *(u32*)((u8*)r6 + 0x78) = r5;
    *(u32*)((u8*)r6 + 0x7C) = r0;
    if (--ctr != 0) goto L_8010BE8C;
L_8010BFD8: ;
    r5 = r4 << 4;
    /* subi r3, r30, 0x1 */;
    r0 = r3 - r4;
    ctr_fn = (void(*)(void))r0;
    if ((s32)r4 >= (s32)r3) goto L_8010C020;
L_8010BFF0: ;
    r0 = *(u32*)lbl_8047AD4C;
    r4 = r0 + r5;
    r5 = r5 + 0x10;
    r3 = *(u32*)((u8*)r4 + 0x10);
    r0 = *(u32*)((u8*)r4 + 0x14);
    *(u32*)((u8*)r4 + 0x0) = r3;
    *(u32*)((u8*)r4 + 0x4) = r0;
    r3 = *(u32*)((u8*)r4 + 0x18);
    r0 = *(u32*)((u8*)r4 + 0x1C);
    *(u32*)((u8*)r4 + 0x8) = r3;
    *(u32*)((u8*)r4 + 0xC) = r0;
    if (--ctr != 0) goto L_8010BFF0;
L_8010C020: ;
    r3 = *(u32*)lbl_8047AD48;
    r5 = *(u32*)lbl_8047AD4C;
    /* subi r30, r3, 0x1 */;
    r0 = *(u32*)(sp + 0x18);
    r3 = r30 << 4;
    r4 = *(u32*)(sp + 0x1C);
    r5 = r5 + r3;
    r3 = *(u32*)(sp + 0x20);
    *(u32*)((u8*)r5 + 0x0) = r0;
    r0 = *(u32*)(sp + 0x24);
    *(u32*)((u8*)r5 + 0x4) = r4;
    *(u32*)((u8*)r5 + 0x8) = r3;
    *(u32*)((u8*)r5 + 0xC) = r0;
L_8010C054: ;
    r3 = *(u32*)lbl_8047AD4C;
    r6 = r30 << 4;
    r5 = 0x0;
    r4 = r30;
    *(u32*)(r3 + r6) = r31;
    r0 = *(u32*)lbl_8047AD4C;
    r3 = r0 + r6;
    *(u16*)((u8*)r3 + 0x4) = r27;
    r0 = *(u32*)lbl_8047AD4C;
    r3 = r0 + r6;
    *(u8*)((u8*)r3 + 0x6) = r5;
    r0 = *(u32*)lbl_8047AD4C;
    r3 = r0 + r6;
    *(u32*)((u8*)r3 + 0x8) = r28;
    r0 = *(u32*)lbl_8047AD4C;
    r3 = r0 + r6;
    *(u32*)((u8*)r3 + 0xC) = r29;
    goto L_8010C208;
L_8010C09C: ;
    r8 = r4 << 4;
    r7 = r7 + r8;
    r6 = *(u32*)((u8*)r7 + 0x0);
    r5 = *(u32*)((u8*)r7 + 0x4);
    r3 = *(u32*)((u8*)r7 + 0x8);
    r0 = *(u32*)((u8*)r7 + 0xC);
    *(u32*)(sp + 0x14) = r0;
    /* subi r0, r30, 0x1 */;
    r5 = r0 - r4;
    if ((s32)r4 >= (s32)r0) goto L_8010C1DC;
    r3 = (u32)r5 >> 2;
    r0 = r5;
    ctr_fn = (void(*)(void))r3;
    if ((u32)r3 == (u32)0x0) goto L_8010C1A4;
L_8010C0E8: ;
    r3 = *(u32*)lbl_8047AD4C;
    r7 = r3 + r8;
    r8 = r8 + 0x10;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r3;
    r6 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r7 + 0x8) = r6;
    *(u32*)((u8*)r7 + 0xC) = r3;
    r3 = *(u32*)lbl_8047AD4C;
    r7 = r3 + r8;
    r8 = r8 + 0x10;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r3;
    r6 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r7 + 0x8) = r6;
    *(u32*)((u8*)r7 + 0xC) = r3;
    r3 = *(u32*)lbl_8047AD4C;
    r7 = r3 + r8;
    r8 = r8 + 0x10;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r3;
    r6 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r7 + 0x8) = r6;
    *(u32*)((u8*)r7 + 0xC) = r3;
    r3 = *(u32*)lbl_8047AD4C;
    r7 = r3 + r8;
    r8 = r8 + 0x10;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r3;
    r6 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r7 + 0x8) = r6;
    *(u32*)((u8*)r7 + 0xC) = r3;
    if (--ctr != 0) goto L_8010C0E8;
    r5 = r5 & 0x3;
    if ((u32)r3 == (u32)0x0) goto L_8010C1D8;
L_8010C1A4: ;
    ctr_fn = (void(*)(void))r5;
L_8010C1A8: ;
    r3 = *(u32*)lbl_8047AD4C;
    r7 = r3 + r8;
    r8 = r8 + 0x10;
    r6 = *(u32*)((u8*)r7 + 0x10);
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r3;
    r6 = *(u32*)((u8*)r7 + 0x18);
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r7 + 0x8) = r6;
    *(u32*)((u8*)r7 + 0xC) = r3;
    if (--ctr != 0) goto L_8010C1A8;
L_8010C1D8: ;
    r4 = r4 + r0;
L_8010C1DC: ;
    r5 = *(u32*)lbl_8047AD4C;
    r0 = r4 << 4;
    r3 = *(u32*)(sp + 0x8);
    r5 = r5 + r0;
    r0 = *(u32*)(sp + 0xC);
    *(u32*)((u8*)r5 + 0x0) = r3;
    r3 = *(u32*)(sp + 0x10);
    *(u32*)((u8*)r5 + 0x4) = r0;
    r0 = *(u32*)(sp + 0x14);
    *(u32*)((u8*)r5 + 0x8) = r3;
    *(u32*)((u8*)r5 + 0xC) = r0;
L_8010C208: ;
    r3 = r4;
L_8010C20C: ;
    /* lmw r27, 0x2c(r1) */;
    return;
}
#pragma pop
