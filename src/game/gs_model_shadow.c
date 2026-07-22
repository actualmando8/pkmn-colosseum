/**
 * @file gs_model_shadow.c
 * @brief GSmodel shadow (XD shadow.c / modelShadow*)
 *
 * Split from gs_range_800E202C.c (0x800E8684-0x800E9B2C), one XD source unit per
 * segment (Fable re-split, 2026-07-07). Functions asm-only until matched.
 */
#include "dolphin/types.h"

extern u32 lbl_8047AB84;
extern u32 lbl_8047AB80;
extern f32 lbl_8047AB88;

extern void fn_801B06D4(void);

typedef struct {
    u8 _pad[0x8];
    u16 flags;
    u16 _pad2;
} GSjobjClass;

typedef struct {
    u8 _pad[0xC];
    GSjobjClass* classObj;
} GSlight;

typedef struct {
    u8 _pad[0x158];
    u32 shadowVtxCount;    /* 0x158 */
    void* shadowVtxBuffer; /* 0x15c */
    GSlight* shadowLight;  /* 0x160 */
    u16 shadowVtxHandle;   /* 0x164 */
} GSmodel;

/* Slot table used by the debug shadow-texture pool: lbl_80401490, stride 0x58 */
typedef struct {
    u8 _pad[0x50];
    u8 flag;    /* 0x50 */
    u8 _pad2[3];
    void* obj;  /* 0x54 */
} GSshadowSlot;

extern GSshadowSlot lbl_80401490[6];

typedef struct GSmobjNode {
    u8 _pad0[0x4];
    u32 rendermode; /* 0x4 */
} GSmobjNode;

typedef struct GSdobjNode {
    u8 _pad0[0x4];
    struct GSdobjNode* next; /* 0x4 */
    GSmobjNode* mobj;        /* 0x8 */
} GSdobjNode;

typedef struct {
    u8 _pad[0x14];
    u32 flags;      /* 0x14 */
    GSdobjNode* child; /* 0x18 */
} GSjobjNode;

#if !defined(PR410_GS_MODEL_SHADOW_SPLIT) || defined(PR410_GS_MODEL_SHADOW_EXACT_8EFC)

void GSmodelFreeAllShadowTextures(void)
{
    extern void fn_801B06DC(void* obj);
    extern void fn_801B0880(void* obj, u32 flag);
    u32 i;

    for (i = 0; i < 6; i++) {
        fn_801B06DC(lbl_80401490[i].obj);
        fn_801B0880(lbl_80401490[i].obj, 0);
        lbl_80401490[i].flag = 0;
    }
}

void GSmodelSetShadowBoundExpansion(u32 extent, u32 state)
{
    lbl_8047AB84 = extent;
    lbl_8047AB80 = state;
}

void GSmaterialSetDistanceThreshold(f32 dist)
{
    lbl_8047AB88 = dist * dist;
}

#endif

#if !defined(PR410_GS_MODEL_SHADOW_SPLIT) || defined(PR410_GS_MODEL_SHADOW_MIDDLE)

void GSmodelSetShadowDebug(u32 val)
{
    (void)val;
    fn_801B06D4();
}

void GSmodelSetShadowTextureSize(s32 width, s32 height)
{
    extern u32 lbl_8047AB90;
    extern u32 lbl_8047AB8C;

    if (width & 1) {
        width += 1;
    }
    if (height & 1) {
        height += 1;
    }
    if (width < 2) {
        return;
    }
    if (height < 2) {
        return;
    }
    if (width > 0x280) {
        return;
    }
    if (height <= 0x1E0) {
        lbl_8047AB90 = width;
        lbl_8047AB8C = height;
    }
}

#endif

#if !defined(PR410_GS_MODEL_SHADOW_SPLIT) || defined(PR410_GS_MODEL_SHADOW_EXACT_8FE8)

void GSmodelSetShadowLight(GSmodel* model, GSlight* light)
{
    if ((light != NULL) && ((light->classObj->flags & 0x3) == 0U)) {
        light = NULL;
    }
    model->shadowLight = light;
}

void GSmodelSetShadowSurface(GSmodel* model, s32 count, void* data)
{
    extern void fn_800E24B0(u16 handle);
    extern void fn_800E209C(u16 handle);
    extern u16 _toolentryAlloc__FUl(u32 size);
    extern void* fn_800E27B0(u16 handle);
    extern void* memcpy(void* dst, const void* src, u32 n);

    if (count != model->shadowVtxCount) {
        if (model->shadowVtxHandle != 0) {
            fn_800E24B0(model->shadowVtxHandle);
            fn_800E209C(model->shadowVtxHandle);
            model->shadowVtxHandle = 0;
            model->shadowVtxCount = 0;
            model->shadowVtxBuffer = NULL;
        }
        if (count == 0 || data == NULL) {
            return;
        }
        model->shadowVtxCount = count;
        model->shadowVtxHandle = _toolentryAlloc__FUl(model->shadowVtxCount * 4);
        model->shadowVtxBuffer = fn_800E27B0(model->shadowVtxHandle);
    }
    memcpy(model->shadowVtxBuffer, data, model->shadowVtxCount * 4);
}

#endif

#if !defined(PR410_GS_MODEL_SHADOW_SPLIT) || defined(PR410_GS_MODEL_SHADOW_SUFFIX)

void GSmodelClearShadowFlags(GSmodel* model, u32 flags)
{
    u32* modelFlags = (u32*) model;

    if ((flags & 1U) != 0U) {
        modelFlags[0] &= ~0x10000000U;
    }
    if ((flags & 2U) != 0U) {
        modelFlags[0] &= ~0x20000000U;
    }
    if ((flags & 4U) != 0U) {
        modelFlags[0] &= ~0x40000000U;
    }
}

void GSmodelSetShadowFlags(GSmodel* model, u32 flags)
{
    u32* modelFlags = (u32*) model;

    if ((flags & 1U) != 0U) {
        modelFlags[0] |= 0x10000000U;
    }
    if ((flags & 2U) != 0U) {
        modelFlags[0] |= 0x20000000U;
    }
    if ((flags & 4U) != 0U) {
        modelFlags[0] |= 0x40000000U;
    }
}

void modelShadowFreeModelList__FP8_GSmodel(GSmodel* model)
{
    extern void fn_800E24B0(u16 handle);
    extern void fn_800E209C(u16 handle);

    if (model->shadowVtxHandle != 0) {
        fn_800E24B0(model->shadowVtxHandle);
        fn_800E209C(model->shadowVtxHandle);
        model->shadowVtxHandle = 0;
        model->shadowVtxCount = 0;
        model->shadowVtxBuffer = NULL;
    }
}

void modelShadowInit__Fv(void)
{
    extern f32 lbl_8047CBC8;
    extern u8 lbl_8047AB94;
    extern u32 lbl_8047AB90;
    extern u32 lbl_8047AB8C;
    extern void* fn_801B1730(void);
    extern void fn_801B0880(void* obj, u32 flag);
    f32 dist;
    u32 i;

    dist = lbl_8047CBC8;
    lbl_8047AB94 = 0x80;
    lbl_8047AB90 = 0x180;
    lbl_8047AB8C = 0x180;
    lbl_8047AB88 = dist;
    lbl_8047AB84 = 0;

    for (i = 0; i < 6; i++) {
        lbl_80401490[i].obj = fn_801B1730();
        fn_801B0880(lbl_80401490[i].obj, 0);
    }
}

#pragma peephole off
void _modelShadowSetShadowFlag__FP9_HSD_JObjPPvi(GSjobjNode* jobj, void* arg, int unused)
{
    GSdobjNode* child;
    u8 flag = (u8)(u32)arg;

    (void)unused;

    if ((jobj->flags & 0x4020U) != 0U) {
        return;
    }

    for (child = jobj->child; child != NULL; child = child->next) {
        if (child->mobj != NULL) {
            if (flag != 0) {
                child->mobj->rendermode |= 0x04000000U;
            } else {
                child->mobj->rendermode &= ~0x04000000U;
            }
        }
    }
}
#pragma peephole reset

#endif
