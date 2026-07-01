/**
 * @file hsd_displayfunc.c
 * @brief HSD displayfunc.c -- Billboard and render pass dispatch functions.
 *
 * Decompiled from:
 *   fn_80197A64 (HSD_DObjDisplayFunc1 -- billboard model-view setup)
 *   fn_80197B6C (HSD_DObjDisplayFunc2 -- render pass dispatch)
 *
 * Source file reference: "displayfunc.c" (rodata string at lbl_802746DC)
 *
 * These two functions are display callbacks registered with the HSD DObj
 * system. They handle:
 *
 * 1. HSD_DObjDisplayFunc1: Walks a linked list of DObj nodes to find
 *    the first non-hidden billboard node, then sets up the model-view
 *    matrix. If the billboard node is the same as the target DObj, it
 *    copies the world matrix directly. Otherwise, it concatenates the
 *    billboard's orientation with the target's position.
 *
 * 2. HSD_DObjDisplayFunc2: Dispatches rendering based on the DObj's
 *    render pass flags (bits 20-22 of the flags word at offset 0x14).
 *    Maps to four render pass subroutines:
 *      0x200 -> fn_80198038 (render pass: XLU / translucent)
 *      0x400 -> fn_80198B20 (render pass: OPA / opaque)
 *      0x600 -> fn_801985E0 (render pass: EFB / framebuffer)
 *      0x800 -> fn_80197C70 (render pass: billboard special)
 *    Falls back to a simple matrix multiply if no pass flags are set.
 *
 * These are part of the HAL SysDolphin library, customised for
 * Pokemon Colosseum's rendering pipeline.
 *
 * Assertion strings:
 *   "displayfunc.c" (file name for assert)
 *   "unkown type of billboard."  (sic -- typo in original)
 *   "unkown type of render pass." (sic -- typo in original)
 *
 * Address range: 0x80197A64 - 0x80197C70
 */

#include "dolphin/types.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_jobj.h"

/* ===== External functions ===== */
extern void __assert(const char* file, u32 line, const char* msg); /* HSD_Halt / assert */
extern void HSD_Panic(const char* file, u32 line, const char* msg);
extern void fn_800A2EB4(void* worldMtx, void* dstMtx);               /* MTXCopy (3x4 matrix) */
extern void fn_800A2D98(void* srcMtx, void* jointMtx, void* dstMtx); /* MTXConcat */
extern void fn_801A9DF0(void* a, void* b, void* c);                  /* HSD_MtxInverseConcat */

/* Render pass subroutines - fn_80198038/fn_801985E0/fn_80198B20 defined as asm wrappers below */
extern void fn_80197C70(void*, void*, void*); /* render pass: billboard (no .inc) */

/* ===== String constants (rodata) ===== */
extern const char lbl_802746DC[]; /* "displayfunc.c" */
extern const char lbl_802746EC[]; /* "unkown type of billboard.\n" */
extern const char lbl_80274680[]; /* "unkown type of render pass.\n" */

/* ===== SDA2 assertion expression strings ===== */
extern const char lbl_8047D9E8[]; /* "jobj" -- assertion expression for billboard null check */
extern const char lbl_8047D9F0[]; /* "x" -- assertion expression for billboard found check */

/* ========================================================================
 * Internal DObj-like structure fields (used by both functions):
 *
 * offset 0x0C: void* next   -- next DObj in chain
 * offset 0x14: u32   flags  -- DObj flags
 *   bit 1 (mask 0x02): HIDDEN flag
 *   bits 20-22 (mask 0x700): render pass type
 * offset 0x44: f32[3][4] -- joint/local matrix (Mtx)
 * offset 0x78: void*     -- world matrix pointer
 * ======================================================================== */

/* Flag masks */
#define DOBJ_FLAG_HIDDEN     0x02   /* bit 1: hidden, skip rendering */
#define DOBJ_RENDERPASS_MASK 0x700  /* bits 20-22 in shifted form */

/* Render pass types (after masking with 0x700) */
#define RENDERPASS_XLU       0x200
#define RENDERPASS_OPA       0x400
#define RENDERPASS_EFB       0x600
#define RENDERPASS_BILLBOARD 0x800

/* 0x80198038 | 0x5A8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
typedef struct DisplayFuncVec {
    f32 x;
    f32 y;
    f32 z;
} DisplayFuncVec;

extern void fn_800A3AC0(void*, void*, f32);
extern f32 fn_800A3B38(void*);
extern void fn_800A3B9C(void*, void*, void*);
extern const DisplayFuncVec lbl_802746D0; /* { 0.0f, 0.0f, 1.0f } */
extern f32 lbl_80478ACC;

#define DISPLAYFUNC_FLAG_2000 0x2000
#define DISPLAYFUNC_MTX(mtx, row, col) (((f32*) (mtx))[(row) * 4 + (col)])
#define DISPLAYFUNC_LOAD_COLUMN(dst, mtx, col) \
    do {                                       \
        (dst).x = DISPLAYFUNC_MTX(mtx, 0, col); \
        (dst).y = DISPLAYFUNC_MTX(mtx, 1, col); \
        (dst).z = DISPLAYFUNC_MTX(mtx, 2, col); \
    } while (0)
#define DISPLAYFUNC_STORE_COLUMN(dst, col, src, scale)      \
    do {                                                    \
        DISPLAYFUNC_MTX(dst, 0, col) = (src).x * (scale);   \
        DISPLAYFUNC_MTX(dst, 1, col) = (src).y * (scale);   \
        DISPLAYFUNC_MTX(dst, 2, col) = (src).z * (scale);   \
    } while (0)
#define DISPLAYFUNC_LOAD_TRANSLATION(dst, mtx) DISPLAYFUNC_LOAD_COLUMN(dst, mtx, 3)
#define DISPLAYFUNC_STORE_TRANSLATION(dst, src)       \
    do {                                             \
        DISPLAYFUNC_MTX(dst, 0, 3) = (src).x;        \
        DISPLAYFUNC_MTX(dst, 1, 3) = (src).y;        \
        DISPLAYFUNC_MTX(dst, 2, 3) = (src).z;        \
    } while (0)
#define DISPLAYFUNC_SET_VEC(dst, vx, vy, vz) \
    do {                                     \
        (dst).x = (vx);                      \
        (dst).y = (vy);                      \
        (dst).z = (vz);                      \
    } while (0)

#if 0
asm void fn_80198038(void* dobj, void* mtx, void* renderState) {
#include "src/hsd/hsd_displayfunc_fn_80198038.inc"
}
#else
#pragma optimization_level 4
void fn_80198038(void* dobj, void* mtx, void* renderState)
{
    DisplayFuncVec col0;
    DisplayFuncVec col1;
    DisplayFuncVec col1Work;
    DisplayFuncVec col2;
    DisplayFuncVec trans;
    DisplayFuncVec basis;
    DisplayFuncVec cross;
    f32 col0Len;
    f32 col1Len;
    f32 col2Len;
    f32 crossLen;
    f32 denom;
    f32 scale;
    f32 horizLen;

    DISPLAYFUNC_LOAD_COLUMN(col0, mtx, 0);
    DISPLAYFUNC_LOAD_COLUMN(col1, mtx, 1);
    DISPLAYFUNC_LOAD_COLUMN(col2, mtx, 2);
    DISPLAYFUNC_LOAD_TRANSLATION(trans, mtx);

    col0Len = fn_800A3B38(&col0);
    col2Len = fn_800A3B38(&col2);
    col1Len = fn_800A3B38(&col1);
    col1Work = col1;

    if (((HSD_DObj*) dobj)->flags & DISPLAYFUNC_FLAG_2000) {
        scale = -1.0f / (lbl_80478ACC + fn_800A3B38(&trans));
        fn_800A3AC0(&trans, &basis, scale);
    } else {
        basis = lbl_802746D0;
    }

    scale = 1.0f / (lbl_80478ACC + col1Len);
    fn_800A3AC0(&col1Work, &col1Work, scale);
    fn_800A3B9C(&col1Work, &basis, &cross);
    crossLen = fn_800A3B38(&cross);

    if (crossLen >= lbl_80478ACC) {
        col0Len /= crossLen;
        fn_800A3B9C(&basis, &cross, &col1Work);
        col1Len /= lbl_80478ACC + fn_800A3B38(&col1Work);
    } else {
        DisplayFuncVec flat;

        DISPLAYFUNC_SET_VEC(flat, basis.x, 0.0f, basis.z);
        horizLen = fn_800A3B38(&flat);
        denom = lbl_80478ACC + horizLen;
        scale = -basis.y / denom;
        DISPLAYFUNC_SET_VEC(col1Work, basis.x * scale, denom, basis.z * scale);
        fn_800A3B9C(&col1Work, &basis, &cross);
        col0Len /= lbl_80478ACC + fn_800A3B38(&cross);
    }

    DISPLAYFUNC_STORE_COLUMN(renderState, 0, cross, col0Len);
    DISPLAYFUNC_STORE_COLUMN(renderState, 1, col1Work, col1Len);
    DISPLAYFUNC_STORE_COLUMN(renderState, 2, basis, col2Len);
    DISPLAYFUNC_STORE_TRANSLATION(renderState, trans);
}
#endif
#pragma pop

/* 0x801985E0 | 0x540 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_800A2D64(void*, void*);
extern void fn_800A3ADC(void*, void*);
#if 0
asm void fn_801985E0(void* dobj, void* mtx, void* renderState) {
#include "src/hsd/hsd_displayfunc_fn_801985E0.inc"
}
#else
#pragma optimization_level 4
void fn_801985E0(void* dobj, void* mtx, void* renderState)
{
    DisplayFuncVec col0;
    DisplayFuncVec col0Unit;
    DisplayFuncVec col1;
    DisplayFuncVec col2;
    DisplayFuncVec trans;
    DisplayFuncVec basis;
    DisplayFuncVec cross;
    f32 col1Len;
    f32 col2Len;
    f32 crossLen;
    f32 basisLen;
    f32 denom;
    f32 scale;

    DISPLAYFUNC_LOAD_COLUMN(col0, mtx, 0);
    DISPLAYFUNC_LOAD_COLUMN(col1, mtx, 1);
    DISPLAYFUNC_LOAD_COLUMN(col2, mtx, 2);
    DISPLAYFUNC_LOAD_TRANSLATION(trans, mtx);

    scale = 1.0f / (lbl_80478ACC + fn_800A3B38(&col0));
    fn_800A3AC0(&col0, &col0Unit, scale);
    col1Len = fn_800A3B38(&col1);
    col2Len = fn_800A3B38(&col2);

    if (((HSD_DObj*) dobj)->flags & DISPLAYFUNC_FLAG_2000) {
        DisplayFuncVec flat;

        DISPLAYFUNC_SET_VEC(flat, trans.x, 0.0f, trans.z);
        basisLen = fn_800A3B38(&flat);
        denom = lbl_80478ACC + basisLen;
        scale = -trans.y / denom;
        DISPLAYFUNC_SET_VEC(basis, trans.x * scale, denom, trans.z * scale);
        fn_800A3ADC(&basis, &basis);
    } else {
        DISPLAYFUNC_SET_VEC(basis, 0.0f, 1.0f, 0.0f);
    }

    fn_800A3B9C(&col0Unit, &basis, &cross);
    crossLen = fn_800A3B38(&cross);

    if (crossLen >= lbl_80478ACC) {
        col2Len /= crossLen;
        fn_800A3B9C(&cross, &col0Unit, &basis);
        col1Len /= lbl_80478ACC + fn_800A3B38(&basis);

        DISPLAYFUNC_STORE_COLUMN(renderState, 0, col0, 1.0f);
        DISPLAYFUNC_STORE_COLUMN(renderState, 1, basis, col1Len);
        DISPLAYFUNC_STORE_COLUMN(renderState, 2, cross, col2Len);
        DISPLAYFUNC_STORE_TRANSLATION(renderState, trans);
    } else {
        fn_800A2D64(mtx, renderState);
    }
}
#endif
#pragma pop

/* 0x80198B20 | 0x42C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80198B20(void* dobj, void* mtx, void* renderState) {
#include "src/hsd/hsd_displayfunc_fn_80198B20.inc"
}
#else
#pragma optimization_level 4
void fn_80198B20(void* dobj, void* mtx, void* renderState)
{
    DisplayFuncVec col0;
    DisplayFuncVec col1;
    DisplayFuncVec col1Unit;
    DisplayFuncVec col2;
    DisplayFuncVec trans;
    DisplayFuncVec basis;
    DisplayFuncVec cross;
    f32 col0Len;
    f32 col2Len;
    f32 crossLen;
    f32 basisLen;
    f32 scale;

    DISPLAYFUNC_LOAD_COLUMN(col1, mtx, 1);
    DISPLAYFUNC_LOAD_COLUMN(col0, mtx, 0);
    DISPLAYFUNC_LOAD_COLUMN(col2, mtx, 2);
    DISPLAYFUNC_LOAD_TRANSLATION(trans, mtx);

    scale = 1.0f / (lbl_80478ACC + fn_800A3B38(&col1));
    fn_800A3AC0(&col1, &col1Unit, scale);
    col0Len = fn_800A3B38(&col0);
    col2Len = fn_800A3B38(&col2);

    if (((HSD_DObj*) dobj)->flags & DISPLAYFUNC_FLAG_2000) {
        scale = -1.0f / (lbl_80478ACC + fn_800A3B38(&trans));
        fn_800A3AC0(&trans, &basis, scale);
    } else {
        basis = lbl_802746D0;
    }

    fn_800A3B9C(&col1Unit, &basis, &cross);
    crossLen = fn_800A3B38(&cross);

    if (crossLen >= lbl_80478ACC) {
        col0Len /= crossLen;
        fn_800A3B9C(&cross, &col1Unit, &basis);
        basisLen = fn_800A3B38(&basis);
        col2Len /= lbl_80478ACC + basisLen;

        DISPLAYFUNC_STORE_COLUMN(renderState, 0, cross, col0Len);
        DISPLAYFUNC_STORE_COLUMN(renderState, 1, col1, 1.0f);
        DISPLAYFUNC_STORE_COLUMN(renderState, 2, basis, col2Len);
        DISPLAYFUNC_STORE_TRANSLATION(renderState, trans);
    } else {
        fn_800A2D64(mtx, renderState);
    }
}
#endif
#pragma pop

/* 0x80198F4C | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void fn_801AA35C(void* list, u32 size, u32 alignment);
extern u8 lbl_80465348[];
#if 0
asm void fn_80198F4C(void) {
#include "src/hsd/hsd_displayfunc_fn_80198F4C.inc"
}
#else
#pragma optimization_level 4
void fn_80198F4C(void) {
    fn_801AA35C(lbl_80465348, 0x48, 4);
}
#endif
#pragma pop

/* =======================================================================
 *  HSD_DObjDisplayFunc1 / fn_80197A64
 *  Address: 0x80197A64, Size: 0x108
 *
 *  Billboard model-view matrix setup for DObj rendering.
 *
 *  r3 = dobj (the DObj to display)
 *  r4 = outputMtx (destination model-view matrix)
 *
 *  Algorithm:
 *    1. Check flags bit 1 (HIDDEN); if set, return 0
 *    2. If dobj is NULL, assert "displayfunc.c" line 0x184
 *    3. Walk dobj chain (via offset 0x0C) to find first non-hidden node
 *    4. If not found, assert "displayfunc.c" line 0x1D4
 *    5. If found node == dobj:
 *         Copy the world matrix (offset 0x78) directly to outputMtx
 *    6. Else if found node has HIDDEN flag:
 *         Concatenate found->jointMtx with dobj->jointMtx via inverse
 *    7. Else:
 *         Concatenate found->worldMtx with found->jointMtx, then
 *         inverse-concat with dobj->jointMtx
 *    8. Return outputMtx
 * ======================================================================= */
void* HSD_DObjDisplayFunc1(void* dobj, void* outputMtx) {
    u32* dobjPtr = (u32*)dobj;
    u32 flags;
    void* cur;
    void* found;
    f32 localMtx[3][4];

    /* Step 1: Check HIDDEN flag (bit 1 of flags at offset 0x14) */
    flags = dobjPtr[5]; /* offset 0x14 */
    if (flags & DOBJ_FLAG_HIDDEN) {
        return NULL;
    }

    /* Step 2: NULL check with assert */
    found = dobj;
    if (found == NULL) {
        __assert(lbl_802746DC, 0x184, lbl_8047D9E8);
    }

    /* Step 3: Walk chain to find first node without bit 0 of flags set */
    cur = dobj;
    while (cur != NULL) {
        u32* curPtr = (u32*)cur;
        u32 curFlags = curPtr[5]; /* flags at offset 0x14 */
        if ((curFlags & 0x01) == 0) {
            /* Not the flag we're looking for -- this IS our billboard */
            found = cur;
            break;
        }
        cur = (void*)curPtr[3]; /* next at offset 0x0C */
    }
    if (cur == NULL) {
        found = NULL;
    }

    /* Step 4: Assert if not found */
    if (found == NULL) {
        __assert(lbl_802746DC, 0x1D4, lbl_8047D9F0);
    }

    /* Step 5-7: Set up model-view matrix */
    if (found == dobj) {
        /* Same node: copy world matrix directly */
        u32* foundPtr = (u32*)found;
        void* worldMtx = (void*)foundPtr[30]; /* offset 0x78 */
        fn_800A2EB4(worldMtx, outputMtx);
    } else {
        u32* foundPtr = (u32*)found;
        u32 foundFlags = foundPtr[5];

        if (foundFlags & DOBJ_FLAG_HIDDEN) {
            /* Hidden node: inverse concatenation */
            void* foundJointMtx = (void*)((u8*)found + 0x44);
            void* dobjJointMtx = (void*)((u8*)dobj + 0x44);
            fn_801A9DF0(foundJointMtx, dobjJointMtx, outputMtx);
        } else {
            /* Normal case: concat world * joint, then inverse with target */
            void* foundWorldMtx = (void*)foundPtr[30]; /* offset 0x78 */
            void* foundJointMtx = (void*)((u8*)found + 0x44);
            void* dobjJointMtx = (void*)((u8*)dobj + 0x44);

            fn_800A2D98(foundJointMtx, foundWorldMtx, localMtx);
            fn_801A9DF0(localMtx, dobjJointMtx, outputMtx);
        }
    }

    return outputMtx;
}

/* =======================================================================
 *  HSD_DObjDisplayFunc2 / fn_80197B6C
 *  Address: 0x80197B6C, Size: 0x104
 *
 *  Render pass dispatch for DObj rendering.
 *
 *  r3 = dobj
 *  r4 = viewMtx (the camera/view matrix)
 *  r5 = renderState
 *
 *  Algorithm:
 *    1. Check render pass flags (bits 20-22 at offset 0x14, masked 0x700)
 *    2. If no render pass flags set:
 *         Simple case: just multiply viewMtx * dobj->jointMtx
 *    3. Else:
 *         Concat viewMtx with dobj->jointMtx into localMtx, then
 *         dispatch based on pass type:
 *           0x200 -> fn_80198038 (XLU pass)
 *           0x400 -> fn_80198B20 (OPA pass)
 *           0x600 -> fn_801985E0 (EFB pass)
 *           0x800 -> fn_80197C70 (billboard pass)
 *         If unknown pass type:
 *           HSD_Panic("displayfunc.c", 0x170, "unkown type of render pass.")
 * ======================================================================= */
void HSD_DObjDisplayFunc2(void* dobj, void* viewMtx, void* renderState) {
    u32* dobjPtr = (u32*)dobj;
    u32 flags;
    u32 passType;
    f32 localMtx[3][4];

    flags = dobjPtr[5]; /* offset 0x14 */
    passType = flags & 0xF00; /* mask for render pass bits (shifted) */

    if (passType == 0) {
        /* No render pass flags: simple matrix setup */
        void* dobjJointMtx = (void*)((u8*)dobj + 0x44);
        fn_800A2D98(viewMtx, dobjJointMtx, renderState);
        return;
    }

    /* Concat viewMtx with dobj->jointMtx */
    {
        void* dobjJointMtx = (void*)((u8*)dobj + 0x44);
        fn_800A2D98(viewMtx, dobjJointMtx, localMtx);
    }

    switch (passType) {
        case RENDERPASS_XLU:
            fn_80198038(dobj, localMtx, renderState);
            break;
        case RENDERPASS_OPA:
            fn_80198B20(dobj, localMtx, renderState);
            break;
        case RENDERPASS_EFB:
            fn_801985E0(dobj, localMtx, renderState);
            break;
        case RENDERPASS_BILLBOARD:
            fn_80197C70(dobj, localMtx, renderState);
            break;
        default:
            HSD_Panic(lbl_802746DC, 0x170, lbl_80274680);
            break;
    }
}
