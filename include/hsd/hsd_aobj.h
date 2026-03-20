/**
 * @file hsd_aobj.h
 * @brief HSD AObj - Animation objects.
 *
 * AObj wraps FObj (keyframe data) with playback state: current frame,
 * end frame, frame rate, looping flags, etc. It drives the animation
 * system by interpreting FObj keyframes over time.
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_AOBJ_H
#define HSD_AOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_fobj.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  AObj flags                                                               */
/* ========================================================================= */

#define AOBJ_REWINDED    (1 << 26)
#define AOBJ_FIRST_PLAY  (1 << 27)
#define AOBJ_NO_UPDATE   (1 << 28)
#define AOBJ_LOOP        (1 << 29)
#define AOBJ_NO_ANIM     (1 << 30)

/* ========================================================================= */
/*  AObj structure                                                           */
/* ========================================================================= */

struct HSD_AObj {
    u32 flags;
    f32 curr_frame;
    f32 rewind_frame;
    f32 end_frame;
    f32 framerate;
    HSD_FObj* fobj;
    HSD_Obj* hsd_obj;
};

/* ========================================================================= */
/*  AObj descriptor (data format)                                            */
/* ========================================================================= */

struct HSD_AObjDesc {
    u32 flags;
    f32 end_frame;
    HSD_FObjDesc* fobjdesc;
    u32 obj_id;
};

/* ========================================================================= */
/*  AnimJoint - links animation data to joint hierarchy                      */
/* ========================================================================= */

struct HSD_AnimJoint {
    HSD_AnimJoint* child;
    HSD_AnimJoint* next;
    HSD_AObjDesc* aobjdesc;
    HSD_RObjAnimJoint* robj_anim;
    u32 flags;
};

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

u32 HSD_AObjGetFlags(HSD_AObj* aobj);
void HSD_AObjSetFlags(HSD_AObj* aobj, u32 flags);
void HSD_AObjClearFlags(HSD_AObj* aobj, u32 flags);
void HSD_AObjReqAnim(HSD_AObj* aobj, f32 frame);
void HSD_AObjStopAnim(HSD_AObj* aobj, void* obj, HSD_ObjUpdateFunc func);
void HSD_AObjInterpretAnim(HSD_AObj* aobj, void* obj,
                           HSD_ObjUpdateFunc update_func);
HSD_AObj* HSD_AObjLoadDesc(HSD_AObjDesc* aobjdesc);
void HSD_AObjRemove(HSD_AObj* aobj);
HSD_AObj* HSD_AObjAlloc(void);
void HSD_AObjFree(HSD_AObj* aobj);
void HSD_AObjSetRate(HSD_AObj* aobj, f32 rate);
void HSD_AObjSetEndFrame(HSD_AObj* aobj, f32 frame);
void HSD_AObjSetCurrentFrame(HSD_AObj* aobj, f32 frame);

/* ========================================================================= */
/*  Inline accessors                                                         */
/* ========================================================================= */

static inline f32 HSD_AObjGetCurrFrame(HSD_AObj* aobj)
{
    HSD_ASSERT(0x92, aobj);
    return aobj->curr_frame;
}

static inline f32 HSD_AObjGetEndFrame(HSD_AObj* aobj)
{
    HSD_ASSERT(0xAA, aobj);
    return aobj->end_frame;
}

#endif /* HSD_AOBJ_H */
