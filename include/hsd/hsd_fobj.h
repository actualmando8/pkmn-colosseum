/**
 * @file hsd_fobj.h
 * @brief HSD FObj - Function/animation keyframe objects.
 *
 * FObj stores animation keyframe data and interpolation state.
 * It is the lowest-level animation primitive in the HSD system.
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_FOBJ_H
#define HSD_FOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"

/* ========================================================================= */
/*  Animation operation types                                                */
/* ========================================================================= */

#define HSD_A_OP_NONE  0
#define HSD_A_OP_CON   1
#define HSD_A_OP_LIN   2
#define HSD_A_OP_SPL0  3
#define HSD_A_OP_SPL   4
#define HSD_A_OP_SLP   5
#define HSD_A_OP_KEY   6

/* Fractional data encoding types */
#define HSD_A_FRAC_FLOAT (0 << 5)
#define HSD_A_FRAC_S16   (1 << 5)
#define HSD_A_FRAC_U16   (2 << 5)
#define HSD_A_FRAC_S8    (3 << 5)
#define HSD_A_FRAC_U8    (4 << 5)

/* FObj load states */
#define FOBJ_LOAD_DATA0 1
#define FOBJ_LOAD_DATA  2
#define FOBJ_LOAD_WAIT  3

/* ========================================================================= */
/*  FObj structure                                                           */
/* ========================================================================= */

typedef struct _HSD_FObj {
    struct _HSD_FObj* next;
    u8* ad;
    u8* ad_head;
    u32 length;
    u8 flags;
    u8 op;
    u8 op_intrp;
    u8 obj_type;
    u8 frac_value;
    u8 frac_slope;
    u16 nb_pack;
    s16 startframe;
    u16 fterm;
    f32 time;
    f32 p0;
    f32 p1;
    f32 d0;
    f32 d1;
} HSD_FObj;

/* ========================================================================= */
/*  FObj descriptor (data format)                                            */
/* ========================================================================= */

typedef struct _HSD_FObjDesc {
    struct _HSD_FObjDesc* next;
    u32 length;
    f32 startframe;
    u8 type;
    u8 frac_value;
    u8 frac_slope;
    u8 dummy0;
    u8* ad;
} HSD_FObjDesc;

/* ========================================================================= */
/*  ObjData union - animation value container                                */
/* ========================================================================= */

union HSD_ObjData {
    f32 fv;
    s32 iv;
};

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_FObjRemove(HSD_FObj* fobj);
void HSD_FObjRemoveAll(HSD_FObj* fobj);
void HSD_FObjReqAnimAll(HSD_FObj* fobj, f32 startframe);
void HSD_FObjInterpretAnim(HSD_FObj* fobj, void* obj,
                           HSD_ObjUpdateFunc obj_update, f32 rate);
void HSD_FObjInterpretAnimAll(void* fobj, void* obj,
                              HSD_ObjUpdateFunc obj_update, f32 rate);
void HSD_FObjStopAnimAll(HSD_FObj* fobj, void* obj,
                         HSD_ObjUpdateFunc obj_update, f32 rate);
HSD_FObj* HSD_FObjLoadDesc(HSD_FObjDesc* desc);
HSD_FObj* HSD_FObjAlloc(void);
void HSD_FObjFree(HSD_FObj* fobj);

#endif /* HSD_FOBJ_H */
