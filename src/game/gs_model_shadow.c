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

void GSmodelSetShadowLight(GSmodel* model, GSlight* light)
{
    if ((light != NULL) && ((light->classObj->flags & 0x3) == 0U)) {
        light = NULL;
    }
    model->shadowLight = light;
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
