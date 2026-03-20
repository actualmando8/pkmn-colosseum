/**
 * @file hsd_gobj.h
 * @brief HSD GObj - Game objects (top-level container).
 *
 * GObj is the top-level container in the HSD scene graph. It holds
 * an HSD object (JObj, CObj, LObj, etc.), user data, render callback,
 * process list, and priority/linking for the render and process queues.
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_GOBJ_H
#define HSD_GOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"

/* ========================================================================= */
/*  GObj link constants                                                      */
/* ========================================================================= */

#define HSD_GOBJ_GXLINK_NONE ((u8) 0xFF)
#define HSD_GOBJ_OBJ_NONE    0xFF

/* ========================================================================= */
/*  GObj structure                                                           */
/* ========================================================================= */

struct HSD_GObj {
    /*  +0 */ u16 classifier;
    /*  +2 */ u8 p_link;
    /*  +3 */ u8 gx_link;
    /*  +4 */ u8 p_priority;
    /*  +5 */ u8 render_priority;
    /*  +6 */ u8 obj_kind;
    /*  +7 */ u8 user_data_kind;
    /*  +8 */ HSD_GObj* next;
    /*  +C */ HSD_GObj* prev;
    /* +10 */ HSD_GObj* next_gx;
    /* +14 */ HSD_GObj* prev_gx;
    /* +18 */ HSD_GObjProc* proc;
    /* +1C */ GObj_RenderFunc render_cb;
    /* +20 */ u64 gxlink_prios;
    /* +28 */ void* hsd_obj;
    /* +2C */ void* user_data;
    /* +30 */ void (*user_data_remove_func)(void* data);
    /* +34 */ void* x34_unk;
};

/* ========================================================================= */
/*  GObj process structure                                                   */
/* ========================================================================= */

struct HSD_GObjProc {
    HSD_GObjProc* child;
    HSD_GObjProc* next;
    HSD_GObjProc* prev;
    u8 s_link;
    u8 flags;
    HSD_GObj* gobj;
    void (*callback)(HSD_GObj*);
};

/* ========================================================================= */
/*  Inline accessors                                                         */
/* ========================================================================= */

static inline void* HSD_GObjGetUserData(HSD_GObj* gobj)
{
    return gobj->user_data;
}

static inline void* HSD_GObjGetHSDObj(HSD_GObj* gobj)
{
    return gobj->hsd_obj;
}

static inline u16 HSD_GObjGetClassifier(HSD_GObj* gobj)
{
    return gobj->classifier;
}

static inline HSD_GObj* HSD_GObjGetNext(HSD_GObj* gobj)
{
    return gobj->next;
}

/* ========================================================================= */
/*  Convenience object accessors                                             */
/* ========================================================================= */

#define GET_COBJ(gobj) ((HSD_CObj*) HSD_GObjGetHSDObj(gobj))
#define GET_FOG(gobj)  ((HSD_Fog*)  HSD_GObjGetHSDObj(gobj))
#define GET_JOBJ(gobj) ((HSD_JObj*) HSD_GObjGetHSDObj(gobj))
#define GET_LOBJ(gobj) ((HSD_LObj*) HSD_GObjGetHSDObj(gobj))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

HSD_GObj* GObj_Create(u16 classifier, u8 p_link, u8 priority);

#endif /* HSD_GOBJ_H */
