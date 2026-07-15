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
    u32 shadowSurfaceCount;
    void** shadowSurfaces;
    GSlight* shadowLight;
    u16 shadowSurfaceHandle;
} GSmodel;

typedef struct {
    u8 _pad[0x50];
    u8 active;
    u8 _pad2[3];
    void* texture;
} GSshadowTexture;

typedef struct {
    u8 _pad[4];
    u32 renderMode;
} GSshadowMObj;

typedef struct GSshadowDObj {
    u8 _pad[4];
    struct GSshadowDObj* next;
    GSshadowMObj* mobj;
} GSshadowDObj;

typedef struct {
    u8 _pad[0x14];
    u32 flags;
    GSshadowDObj* dobj;
} GSshadowJObj;

static inline void GSshadowMObjSetFlags(GSshadowMObj* mobj, u32 flags)
{
    mobj->renderMode |= flags;
}

static inline void GSshadowMObjClearFlags(GSshadowMObj* mobj, u32 flags)
{
    mobj->renderMode &= ~flags;
}

void GSmodelFreeAllShadowTextures(void)
{
    extern GSshadowTexture lbl_80401490[6];
    extern void fn_801B06DC(void* texture);
    extern void fn_801B0880(void* texture, u32 state);
    u32 i;

    for (i = 0; i < 6; i++) {
        fn_801B06DC(lbl_80401490[i].texture);
        fn_801B0880(lbl_80401490[i].texture, 0);
        lbl_80401490[i].active = 0;
    }
}

    u8 _pad[0x160];
    GSlight* shadowLight;
} GSmodel;

void GSmodelSetShadowBoundExpansion(u32 callback, u32 state)
{
    lbl_8047AB84 = callback;
    lbl_8047AB80 = state;
}

void GSmaterialSetDistanceThreshold(f32 dist)
{
    lbl_8047AB88 = dist * dist;
}

void GSmodelSetShadowDebug(u32 val)
{
    (void)val;
    fn_801B06D4();
}

void GSmodelSetShadowTextureSize(s32 width, s32 height)
{
    extern s32 lbl_8047AB90;
    extern s32 lbl_8047AB8C;

    if ((width & 1) != 0) {
        width++;
    }
    if ((height & 1) != 0) {
        height++;
    }
    if (width < 2 || height < 2 || width > 640 || height > 480) {
        return;
    }
    lbl_8047AB90 = width;
    lbl_8047AB8C = height;
}

void GSmodelSetShadowLight(GSmodel* model, GSlight* light)
{
    if ((light != NULL) && ((light->classObj->flags & 0x3) == 0U)) {
        light = NULL;
    }
    model->shadowLight = light;
}

void GSmodelSetShadowSurface(GSmodel* model, s32 count, void** surfaces)
{
    extern void fn_800E24B0(u16 handle);
    extern void fn_800E209C(u16 handle);
    extern u16 _toolentryAlloc__FUl(u32 size);
    extern void* fn_800E27B0(u16 handle);
    extern void* memcpy(void* dst, const void* src, u32 size);

    if (count != model->shadowSurfaceCount) {
        if (model->shadowSurfaceHandle != 0) {
            fn_800E24B0(model->shadowSurfaceHandle);
            fn_800E209C(model->shadowSurfaceHandle);
            model->shadowSurfaceHandle = 0;
            model->shadowSurfaceCount = 0;
            model->shadowSurfaces = NULL;
        }

        if (count == 0 || surfaces == NULL) {
            return;
        }

        model->shadowSurfaceCount = count;
        model->shadowSurfaceHandle = _toolentryAlloc__FUl(model->shadowSurfaceCount * sizeof(void*));
        model->shadowSurfaces = fn_800E27B0(model->shadowSurfaceHandle);
    }

    memcpy(model->shadowSurfaces, surfaces, model->shadowSurfaceCount * sizeof(void*));
}

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

    if (model->shadowSurfaceHandle != 0) {
        fn_800E24B0(model->shadowSurfaceHandle);
        fn_800E209C(model->shadowSurfaceHandle);
        model->shadowSurfaceHandle = 0;
        model->shadowSurfaceCount = 0;
        model->shadowSurfaces = NULL;
    }
}

#pragma peephole off
void modelShadowInit__Fv(void)
{
    extern GSshadowTexture lbl_80401490[6];
    extern void* fn_801B1730(void);
    extern void fn_801B0880(void* texture, u32 state);
    extern f32 lbl_8047CBC8;
    extern u8 lbl_8047AB94;
    extern s32 lbl_8047AB90;
    extern s32 lbl_8047AB8C;
    GSshadowTexture* slot = lbl_80401490;
    u32 i;

    lbl_8047AB94 = 0x80;
    lbl_8047AB90 = 0x180;
    lbl_8047AB8C = 0x180;
    lbl_8047AB88 = lbl_8047CBC8;
    lbl_8047AB84 = 0;
    for (i = 0; i < 6; i++) {
        slot->texture = fn_801B1730();
        fn_801B0880(slot->texture, 0);
        slot++;
    }
}
#pragma peephole reset

void _modelShadowSetShadowFlag__FP9_HSD_JObjPPvi(GSshadowJObj* jobj, void** data, s32 unused)
{
    GSshadowDObj* dobj;
    u8 enable = (u32) data;

    (void) unused;

    if ((jobj->flags & 0x4020) ? FALSE : TRUE) {
        for (dobj = jobj->dobj; dobj != NULL; dobj = dobj->next) {
            if (dobj->mobj != NULL) {
                if (enable != 0) {
                    GSshadowMObjSetFlags(dobj->mobj, 0x04000000);
                } else {
                    GSshadowMObjClearFlags(dobj->mobj, 0x04000000);
                }
            }
        }
    }
}
