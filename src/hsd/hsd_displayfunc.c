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
extern void fn_80196E10(const char* file, u32 line, const char* msg); /* HSD_Halt / assert */
extern void fn_80196D78(const char* file, u32 line, const char* msg); /* HSD_Panic */
extern void fn_800A2EB4(void* worldMtx, void* dstMtx);               /* MTXCopy (3x4 matrix) */
extern void fn_800A2D98(void* srcMtx, void* jointMtx, void* dstMtx); /* MTXConcat */
extern void fn_801A9DF0(void* a, void* b, void* c);                  /* HSD_MtxInverseConcat */

/* Render pass subroutines */
extern void fn_80198038(void* dobj, void* mtx, void* renderState); /* render pass: XLU */
extern void fn_80198B20(void* dobj, void* mtx, void* renderState); /* render pass: OPA */
extern void fn_801985E0(void* dobj, void* mtx, void* renderState); /* render pass: EFB */
extern void fn_80197C70(void* dobj, void* mtx, void* renderState); /* render pass: billboard */

/* ===== String constants (rodata) ===== */
extern const char lbl_802746DC[]; /* "displayfunc.c" */
extern const char lbl_802746EC[]; /* "unkown type of billboard.\n" */
extern const char lbl_80274680[]; /* "unkown type of render pass.\n" */

/* ===== SDA2 assertion expression strings ===== */
extern const char lbl_8047D9E8[]; /* "jobj" -- assertion expression for billboard null check */
extern const char lbl_8047D9F0[]; /* "x" -- assertion expression for billboard found check */

/* Forward declarations for converted functions */
void fn_80197A64(void);
void fn_80197B6C(void);


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
        fn_80196E10(lbl_802746DC, 0x184, lbl_8047D9E8);
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
        fn_80196E10(lbl_802746DC, 0x1D4, lbl_8047D9F0);
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
            fn_80196D78(lbl_802746DC, 0x170, lbl_80274680);
            break;
    }
}

/* =========================================================================
 *  Internal stubs: 0x80197A64-0x80198F4C (2 functions)
 * ========================================================================= */

/* 0x80197A64 | 0x108 */
void fn_80197A64(void) {
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r4;
    r29 = r3;
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000002;
    if ((s32)tmp == 0) goto L_80197A98;
    r3 = 0x0;
    goto L_80197B50;
L_80197A98:
    /* mr. r31, r29 */;
    if ((s32)tmp != 0) goto L_80197ACC;
    r3 = (u32)&lbl_802746DC;
    r4 = 0x184;
    r3 = (u32)&lbl_802746DC;
    r5 = (u32)&lbl_8047D9E8;
    ((void(*)(void))fn_80196E10)();
    goto L_80197ACC;
L_80197AB8:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x3;
    if ((s32)tmp == 0) goto L_80197AC8;
    goto L_80197AD8;
L_80197AC8:
    r31 = *(u32*)((u8*)r31 + 0xC);
L_80197ACC:
    if (r31 != 0) goto L_80197AB8;
    r31 = 0x0;
L_80197AD8:
    if (r31 != 0) goto L_80197AF4;
    r3 = (u32)&lbl_802746DC;
    r4 = 0x1d4;
    r3 = (u32)&lbl_802746DC;
    r5 = (u32)&lbl_8047D9F0;
    ((void(*)(void))fn_80196E10)();
L_80197AF4:
    if (r31 != r29) goto L_80197B0C;
    r3 = *(u32*)((u8*)r31 + 0x78);
    r4 = r30;
    ((void(*)(void))fn_800A2EB4)();
    goto L_80197B4C;
L_80197B0C:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000002;
    if (r31 == r29) goto L_80197B2C;
    r5 = r30;
    r3 = r31 + 0x44;
    r4 = r29 + 0x44;
    ((void(*)(void))fn_801A9DF0)();
    goto L_80197B4C;
L_80197B2C:
    r4 = *(u32*)((u8*)r31 + 0x78);
    r3 = r31 + 0x44;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_800A2D98)();
    r5 = r30;
    r3 = r1 + 0x8;
    r4 = r29 + 0x44;
    ((void(*)(void))fn_801A9DF0)();
L_80197B4C:
    r3 = r30;
L_80197B50:
    return;
}

/* 0x80197B6C | 0x104 */
void fn_80197B6C(void) {
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = r5;
    r30 = r3;
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000E00;
    if ((s32)tmp == 0) goto L_80197C48;
    r3 = r4;
    r4 = r30 + 0x44;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_800A2D98)();
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000E00;
    if ((s32)tmp == 0x600) goto L_80197C04;
    if ((s32)tmp >= 0x600) goto L_80197BD0;
    if ((s32)tmp == 0x400) goto L_80197BF0;
    if ((s32)tmp >= 0x400) goto L_80197C2C;
    if ((s32)tmp == 0x200) goto L_80197BDC;
    goto L_80197C2C;
L_80197BD0:
    if ((s32)tmp == 0x800) goto L_80197C18;
    goto L_80197C2C;
L_80197BDC:
    r3 = r30;
    r5 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_80198038)();
    goto L_80197C58;
L_80197BF0:
    r3 = r30;
    r5 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_80198B20)();
    goto L_80197C58;
L_80197C04:
    r3 = r30;
    r5 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_801985E0)();
    goto L_80197C58;
L_80197C18:
    r3 = r30;
    r5 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_80197C70)();
    goto L_80197C58;
L_80197C2C:
    r3 = (u32)&lbl_802746DC;
    r5 = (u32)&lbl_802746EC;
    r3 = (u32)&lbl_802746DC;
    r4 = 0x170;
    r5 = (u32)&lbl_802746EC;
    ((void(*)(void))fn_80196D78)();
    goto L_80197C58;
L_80197C48:
    r3 = r4;
    r5 = r31;
    r4 = r30 + 0x44;
    ((void(*)(void))fn_800A2D98)();
L_80197C58:
    return;
}
