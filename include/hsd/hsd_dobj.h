/**
 * @file hsd_dobj.h
 * @brief HSD DObj - Display objects (visible geometry).
 *
 * DObj links a material (MObj) to polygon data (PObj) for rendering.
 * Multiple DObj can be chained together on a JObj to represent
 * multi-material meshes.
 *
 * Colosseum address range: 0x80198F7C (HSD_DObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_DOBJ_H
#define HSD_DOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_mobj.h"

/* ========================================================================= */
/*  DObj flags                                                               */
/* ========================================================================= */

#define DOBJ_HIDDEN 0x1

/* ========================================================================= */
/*  DObj structure                                                           */
/* ========================================================================= */

struct HSD_DObj {
    HSD_Class parent;
    HSD_DObj* next;     /* 0x04 */
    HSD_MObj* mobj;     /* 0x08 */
    HSD_PObj* pobj;     /* 0x0C */
    HSD_AObj* aobj;     /* 0x10 */
    u32 flags;          /* 0x14 */
};

/* ========================================================================= */
/*  DObj descriptor (data format)                                            */
/* ========================================================================= */

struct HSD_DObjDesc {
    char* class_name;
    HSD_DObjDesc* next;
    HSD_MObjDesc* mobjdesc;
    HSD_PObjDesc* pobjdesc;
};

/* ========================================================================= */
/*  DObj class info                                                          */
/* ========================================================================= */

struct HSD_DObjInfo {
    HSD_ClassInfo parent;
    void (*disp)(HSD_DObj* dobj, f32 vmtx[3][4], f32 pmtx[3][4],
                 u32 rendermode);
    int (*load)(HSD_DObj* dobj, HSD_DObjDesc* desc);
    void (*update)(HSD_DObj* dobj, u32 type, void* value);
};

/* ========================================================================= */
/*  Shape animation for DObj                                                 */
/* ========================================================================= */

struct HSD_ShapeAnimDObj {
    HSD_ShapeAnimDObj* next;
    HSD_ShapeAnim* shapeanim;
};

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

#define HSD_DOBJ(o) ((HSD_DObj*) (o))
#define HSD_DOBJ_INFO(i) ((HSD_DObjInfo*) (i))
#define HSD_DOBJ_METHOD(o) HSD_DOBJ_INFO(HSD_CLASS_METHOD(o))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_DObjSetCurrent(HSD_DObj* dobj);
u32 HSD_DObjGetFlags(HSD_DObj* dobj);
void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags);
void HSD_DObjCountVertices(HSD_DObj* dobj, s32* total_a, s32* total_b);
void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim,
                     HSD_ShapeAnimDObj* sh_anim);
void HSD_DObjAddAnimAll(HSD_DObj* dobj, void* matanim,
                        void* shapeanimdobj);
void HSD_DObjReqAnimAllByFlags(HSD_DObj* dobj, f32 startframe, void* flags);
void HSD_DObjReqAnimAll(HSD_DObj* dobj, f32 startframe);
void HSD_DObjAnim(HSD_DObj* dobj);
void HSD_DObjAnimAll(HSD_DObj* dobj);
HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc);
void HSD_DObjRemoveAll(HSD_DObj* dobj);
HSD_DObj* HSD_DObjAlloc(void);
void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc);
void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc);
void HSD_DObjDisp(HSD_DObj* dobj, f32 vmtx[3][4], f32 pmtx[3][4],
                  u32 rendermode);
void HSD_DObjRemove(HSD_DObj* dobj);
void HSD_DObjSetDefaultClass(HSD_ClassInfo* info);

#endif /* HSD_DOBJ_H */
