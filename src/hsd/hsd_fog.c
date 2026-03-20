/**
 * @file hsd_fog.c
 * @brief HSD Fog and FogAdj implementation.
 *
 * Colosseum address: 0x8019B7C0 (HSD_FogAdjInit)
 * Adapted from doldecomp/melee src/sysdolphin/baselib/fog.c
 */

#include "hsd/hsd_fog.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_object.h"

extern void* memcpy(void* dst, const void* src, u32 size);

static void FogInfoInit(void);
static void FogAdjInfoInit(void);

static HSD_FogInfo hsdFog = { FogInfoInit };
static HSD_FogAdjInfo hsdFogAdj = { FogAdjInfoInit };

/* ========================================================================= */
/*  FogAdj                                                                   */
/* ========================================================================= */

void HSD_FogAdjInit(HSD_FogAdj* fogadj, HSD_FogAdjDesc* desc)
{
    if (fogadj == NULL || desc == NULL) {
        return;
    }
    fogadj->center = (s16) desc->center;
    fogadj->width = desc->width;
    memcpy(fogadj->mtx, desc->mtx, sizeof(f32) * 16);
}

HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc* desc)
{
    HSD_FogAdj* fogadj;

    if (desc == NULL) {
        return NULL;
    }

    fogadj = HSD_FogAdjAlloc();
    HSD_FogAdjInit(fogadj, desc);
    return fogadj;
}

HSD_FogAdj* HSD_FogAdjAlloc(void)
{
    HSD_FogAdj* fogadj;
    fogadj = (HSD_FogAdj*) hsdNew(&hsdFogAdj.parent.parent);
    HSD_ASSERT(0, fogadj);
    return fogadj;
}

/* ========================================================================= */
/*  Fog                                                                      */
/* ========================================================================= */

void HSD_FogInit(HSD_Fog* fog, HSD_FogDesc* desc)
{
    if (fog == NULL || desc == NULL) {
        return;
    }
    fog->type = desc->type;
    fog->start = desc->start;
    fog->end = desc->end;
    fog->color = desc->color;
    fog->fog_adj = HSD_FogAdjLoadDesc(desc->fogadjdesc);
}

HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc)
{
    HSD_Fog* fog;

    if (desc == NULL) {
        return NULL;
    }

    fog = HSD_FogAlloc();
    HSD_FogInit(fog, desc);
    return fog;
}

HSD_Fog* HSD_FogAlloc(void)
{
    HSD_Fog* fog;
    fog = (HSD_Fog*) hsdNew(&hsdFog.parent.parent);
    HSD_ASSERT(0, fog);
    return fog;
}

void HSD_FogReqAnim(HSD_Fog* fog, f32 frame)
{
    if (fog != NULL) {
        HSD_AObjReqAnim(fog->aobj, frame);
    }
}

void HSD_FogInterpretAnim(HSD_Fog* fog)
{
    if (fog == NULL) {
        return;
    }
    /* Interpret AObj -> update fog parameters */
}

/* ========================================================================= */
/*  Class init                                                               */
/* ========================================================================= */

static void FogRelease(HSD_Class* o)
{
    HSD_Fog* fog = (HSD_Fog*) o;
    HSD_AObjRemove(fog->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFog)->release(o);
}

static void FogInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFog), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fog",
                     sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    HSD_CLASS_INFO(&hsdFog)->release = FogRelease;
}

static void FogAdjRelease(HSD_Class* o)
{
    HSD_FogAdj* fogadj = (HSD_FogAdj*) o;
    HSD_AObjRemove(fogadj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdFogAdj)->release(o);
}

static void FogAdjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdFogAdj), HSD_CLASS_INFO(&hsdObj),
                     "sysdolphin_base_library", "hsd_fogadj",
                     sizeof(HSD_FogAdjInfo), sizeof(HSD_FogAdj));
    HSD_CLASS_INFO(&hsdFogAdj)->release = FogAdjRelease;
}

/* =========================================================================
 *  Internal stubs: 0x8019B490-0x8019C3C4 (20 functions)
 * ========================================================================= */

/* 0x8019B490 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B490(void) {
    /* TODO: match -- 0x98 bytes at 0x8019B490 */
}
#pragma pop

/* 0x8019B528 | 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B528(void) {
    /* TODO: match -- 0xC0 bytes at 0x8019B528 */
}
#pragma pop

/* 0x8019B5E8 | 0x168 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B5E8(void) {
    /* TODO: match -- 0x168 bytes at 0x8019B5E8 */
}
#pragma pop

/* 0x8019B750 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B750(void) {
    /* TODO: match -- 0x34 bytes at 0x8019B750 */
}
#pragma pop

/* 0x8019B784 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B784(void) {
    /* TODO: match -- 0x30 bytes at 0x8019B784 */
}
#pragma pop

/* 0x8019B7B4 | 0xC */
void fn_8019B7B4(void) {
}

/* 0x48 | fn_8019B7C0 | single_call_straight */
void fn_8019B7C0(void) {
    fn_80193B30();
}

/* 0x6C | fn_8019B808 | single_call_straight */
void fn_8019B808(void) {
    fn_80193B30();
}

/* 0x8019B874 | 0xD4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B874(void) {
    /* TODO: match -- 0xD4 bytes at 0x8019B874 */
}
#pragma pop

/* 0x8019B948 | 0x230 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019B948(void) {
    /* TODO: match -- 0x230 bytes at 0x8019B948 */
}
#pragma pop

/* 0x8019BB78 | 0x1A0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BB78(void) {
    /* TODO: match -- 0x1A0 bytes at 0x8019BB78 */
}
#pragma pop

/* 0x8019BD18 | 0x2D0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BD18(void) {
    /* TODO: match -- 0x2D0 bytes at 0x8019BD18 */
}
#pragma pop

/* 0x8019BFE8 | 0x110 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019BFE8(void) {
    /* TODO: match -- 0x110 bytes at 0x8019BFE8 */
}
#pragma pop

/* 0x8019C0F8 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C0F8(void) {
    /* TODO: match -- 0x30 bytes at 0x8019C0F8 */
}
#pragma pop

/* 0x8019C128 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C128(void) {
    /* TODO: match -- 0x88 bytes at 0x8019C128 */
}
#pragma pop

/* 0x8019C1B0 | 0xB4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C1B0(void) {
    /* TODO: match -- 0xB4 bytes at 0x8019C1B0 */
}
#pragma pop

/* 0x8019C264 | 0xF4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C264(void) {
    /* TODO: match -- 0xF4 bytes at 0x8019C264 */
}
#pragma pop

/* 0x8019C358 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C358(void) {
    /* TODO: match -- 0x30 bytes at 0x8019C358 */
}
#pragma pop

/* 0x8019C388 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8019C388(void) {
    /* TODO: match -- 0x30 bytes at 0x8019C388 */
}
#pragma pop

/* 0x8019C3B8 | 0xC */
void fn_8019C3B8(void) {
}
