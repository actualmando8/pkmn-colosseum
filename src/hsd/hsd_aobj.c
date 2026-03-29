/**
 * @file hsd_aobj.c
 * @brief HSD AObj - Animation object implementation.
 *
 * AObj wraps FObj with playback state for driving animations.
 *
 * Adapted from doldecomp/melee src/sysdolphin/baselib/aobj.c
 */

#include "hsd/hsd_aobj.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_fobj.h"
#include "hsd/hsd_memory.h"

extern void* memset(void* dst, int val, u32 size);

/* ========================================================================= */
/*  Flag accessors                                                           */
/* ========================================================================= */

u32 HSD_AObjGetFlags(HSD_AObj* aobj)
{
    HSD_ASSERT(0, aobj);
    return aobj->flags;
}

void HSD_AObjSetFlags(HSD_AObj* aobj, u32 flags)
{
    HSD_ASSERT(0, aobj);
    aobj->flags |= flags;
}

void HSD_AObjClearFlags(HSD_AObj* aobj, u32 flags)
{
    HSD_ASSERT(0, aobj);
    aobj->flags &= ~flags;
}

/* ========================================================================= */
/*  Playback control                                                         */
/* ========================================================================= */

void HSD_AObjReqAnim(HSD_AObj* aobj, f32 frame)
{
    if (aobj != NULL) {
        aobj->curr_frame = frame;
        aobj->flags |= AOBJ_FIRST_PLAY;
        aobj->flags &= ~AOBJ_NO_ANIM;
        HSD_FObjReqAnimAll(aobj->fobj, frame);
    }
}

void HSD_AObjStopAnim(HSD_AObj* aobj, void* obj, HSD_ObjUpdateFunc func)
{
    if (aobj != NULL) {
        aobj->flags |= AOBJ_NO_ANIM;
    }
}

void HSD_AObjSetRate(HSD_AObj* aobj, f32 rate)
{
    HSD_ASSERT(0, aobj);
    aobj->framerate = rate;
}

void HSD_AObjSetEndFrame(HSD_AObj* aobj, f32 frame)
{
    HSD_ASSERT(0, aobj);
    aobj->end_frame = frame;
}

void HSD_AObjSetCurrentFrame(HSD_AObj* aobj, f32 frame)
{
    HSD_ASSERT(0, aobj);
    aobj->curr_frame = frame;
}

/* ========================================================================= */
/*  Interpret animation                                                      */
/* ========================================================================= */

void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj,
                           HSD_ObjUpdateFunc update_func)
{
    if (aobj == NULL) {
        return;
    }
    if (aobj->flags & AOBJ_NO_ANIM) {
        return;
    }
    HSD_FObjInterpretAnimAll(aobj->fobj, obj, update_func, aobj->framerate);
    aobj->curr_frame += aobj->framerate;
}

/* ========================================================================= */
/*  Load / Remove / Alloc                                                    */
/* ========================================================================= */

HSD_AObj* HSD_AObjLoadDesc(HSD_AObjDesc* aobjdesc)
{
    HSD_AObj* aobj;

    if (aobjdesc == NULL) {
        return NULL;
    }

    aobj = HSD_AObjAlloc();
    aobj->flags = aobjdesc->flags;
    aobj->end_frame = aobjdesc->end_frame;
    aobj->fobj = HSD_FObjLoadDesc(aobjdesc->fobjdesc);
    aobj->framerate = 1.0f;
    aobj->curr_frame = 0.0f;
    aobj->rewind_frame = 0.0f;

    return aobj;
}

void HSD_AObjRemove(HSD_AObj* aobj)
{
    if (aobj != NULL) {
        HSD_FObjRemoveAll(aobj->fobj);
        HSD_Free(aobj);
    }
}

HSD_AObj* HSD_AObjAlloc(void)
{
    HSD_AObj* aobj = (HSD_AObj*) HSD_MemAlloc(sizeof(HSD_AObj));
    if (aobj != NULL) {
        memset(aobj, 0, sizeof(HSD_AObj));
    }
    return aobj;
}

void HSD_AObjFree(HSD_AObj* aobj)
{
    if (aobj != NULL) {
        HSD_Free(aobj);
    }
}

/* 0x801920E4 | 0x1664 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern void OSReport(const char* fmt, ...);
extern void fn_800CD85C(void);
extern void fn_800CDBE0(void);
extern void fn_800CE148(void);
extern void fn_800CE220(void);
extern void fn_800CE298(void);
extern void fn_800CE2B8(void);
extern void fn_800CE2D8(void);
extern void fn_800CE2F8(void);
extern void fn_800CE318(void);
extern void fn_800CE338(void);
extern void fn_800CE358(void);
extern void fn_80196D78(void);
extern void fn_80196E10(void);
extern void fn_801A3E64(void);
extern void fn_801A3EB4(void);
extern void fn_801ADC3C(void);
extern void fn_801ADC7C(void);
#if 1
asm void fn_801920E4(void) {
#include "src/hsd/hsd_aobj_fn_801920E4.inc"
}
#else
void fn_801920E4(void) { /* TODO */ }
#endif
#pragma pop
