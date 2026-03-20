/**
 * @file hsd_wobj.h
 * @brief HSD WObj - World objects (3D position targets).
 *
 * WObj represents a position in world space, used as targets for
 * cameras (eye position, interest point) and lights. Supports
 * animation and constraint references.
 *
 * Colosseum address range: 0x801914F4 (HSD_WObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_WOBJ_H
#define HSD_WOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  WObj structure                                                           */
/* ========================================================================= */

struct HSD_WObj {
    HSD_Obj parent;
    u32 flags;
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    HSD_AObj* aobj;
    HSD_RObj* robj;
};

/* ========================================================================= */
/*  WObj descriptor (data format)                                            */
/* ========================================================================= */

struct HSD_WObjDesc {
    char* class_name;
    f32 pos_x;
    f32 pos_y;
    f32 pos_z;
    HSD_RObjDesc* robjdesc;
};

/* ========================================================================= */
/*  WObj class info                                                          */
/* ========================================================================= */

struct HSD_WObjInfo {
    HSD_ObjInfo parent;
    int (*load)(HSD_WObj* wobj, HSD_WObjDesc* desc);
};

/* ========================================================================= */
/*  WObj animation                                                           */
/* ========================================================================= */

struct HSD_WObjAnim {
    HSD_AObjDesc* aobjdesc;
    HSD_RObjAnimJoint* robjanim;
};

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

extern HSD_WObjInfo hsdWObj;

#define HSD_WOBJ_INFO(i) ((HSD_WObjInfo*) (i))
#define HSD_WOBJ_METHOD(o) HSD_WOBJ_INFO(HSD_OBJECT_METHOD(o))

/* ========================================================================= */
/*  Inline functions                                                         */
/* ========================================================================= */

static inline void HSD_WObjUnref(HSD_WObj* wobj)
{
    if (wobj == NULL) {
        return;
    }
    if (ref_DEC(wobj) != 0) {
        if (wobj != NULL) {
            HSD_OBJECT_METHOD(wobj)->release((HSD_Class*) wobj);
            HSD_OBJECT_METHOD(wobj)->destroy((HSD_Class*) wobj);
        }
    }
}

static inline void HSD_WObjClearFlags(HSD_WObj* wobj, u32 flags)
{
    wobj->flags &= ~flags;
}

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_WObjRemoveAnim(HSD_WObj* wobj);
void HSD_WObjReqAnim(HSD_WObj* wobj, f32 frame);
void HSD_WObjAddAnim(HSD_WObj* wobj, HSD_WObjAnim* anim);
void HSD_WObjInterpretAnim(HSD_WObj* wobj);
void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc);
HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc);
void HSD_WObjSetPosition(HSD_WObj* wobj, f32 x, f32 y, f32 z);
void HSD_WObjSetPositionX(HSD_WObj* wobj, f32 val);
void HSD_WObjSetPositionY(HSD_WObj* wobj, f32 val);
void HSD_WObjSetPositionZ(HSD_WObj* wobj, f32 val);
void HSD_WObjGetPosition(HSD_WObj* wobj, f32* x, f32* y, f32* z);
HSD_WObj* HSD_WObjAlloc(void);
void HSD_WObjSetDefaultClass(HSD_ClassInfo* info);

#endif /* HSD_WOBJ_H */
