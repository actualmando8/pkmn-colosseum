/**
 * @file hsd_lobj.h
 * @brief HSD LObj - Light objects.
 *
 * LObj manages hardware lights for the GX graphics pipeline.
 * Supports ambient, infinite (directional), point, and spot lights
 * with attenuation, color, and animation.
 *
 * Colosseum address range: 0x801A4000 (HSD_LObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_LOBJ_H
#define HSD_LOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  Constants                                                                */
/* ========================================================================= */

#define MAX_GXLIGHT 9

/* ========================================================================= */
/*  Light parameter structures                                               */
/* ========================================================================= */

struct HSD_LightPoint {
    f32 cutoff;
    u32 point_func;
    f32 ref_br;
    f32 ref_dist;
    u32 dist_func;
};

struct HSD_LightPointDesc {
    f32 ref_br;
    f32 ref_dist;
    u32 dist_func;
};

struct HSD_LightSpot {
    f32 cutoff;
    u32 spot_func;
    f32 ref_br;
    f32 ref_dist;
    u32 dist_func;
};

struct HSD_LightSpotDesc {
    f32 cutoff;
    u32 spot_func;
    f32 ref_br;
    f32 ref_dist;
    u32 dist_func;
};

struct HSD_LightAttn {
    f32 a0;
    f32 a1;
    f32 a2;
    f32 k0;
    f32 k1;
    f32 k2;
};

/* ========================================================================= */
/*  LObj structure                                                           */
/* ========================================================================= */

struct HSD_LObj {
    /* 0x00 */ HSD_Obj parent;
    /* 0x08 */ u16 flags;
    /* 0x0A */ u16 priority;
    /* 0x0C */ HSD_LObj* next;
    /* 0x10 */ u32 color;          /* GXColor packed as u32 */
    /* 0x14 */ u32 hw_color;       /* Hardware-set color */
    /* 0x18 */ HSD_WObj* position;
    /* 0x1C */ HSD_WObj* interest;
    /* 0x20 */ union {
        HSD_LightPoint point;
        HSD_LightSpot spot;
        HSD_LightAttn attn;
    } u;
    /* 0x38 */ f32 shininess;
    /* 0x3C */ f32 lvec_x;
    /* 0x40 */ f32 lvec_y;
    /* 0x44 */ f32 lvec_z;
    /* 0x48 */ HSD_AObj* aobj;
    /* 0x4C */ u32 id;             /* GXLightID */
    /* 0x50 */ u8 lightobj[0x40];  /* GXLightObj (64 bytes) */
    /* 0x90 */ u32 spec_id;        /* GXLightID for specular */
    /* 0x94 */ u8 spec_lightobj[0x40]; /* GXLightObj for specular */
};

/* ========================================================================= */
/*  Light descriptor (data format)                                           */
/* ========================================================================= */

struct HSD_LightDesc {
    char* class_name;
    HSD_LightDesc* next;
    u16 flags;
    u16 attnflags;
    u32 color;         /* GXColor */
    HSD_WObjDesc* position;
    HSD_WObjDesc* interest;
    union {
        void* p;
        f32* shininess;
        HSD_LightPointDesc* point;
        HSD_LightSpotDesc* spot;
        HSD_LightAttn* attn;
    } u;
};

/* ========================================================================= */
/*  Light animation                                                          */
/* ========================================================================= */

struct HSD_LightAnim {
    HSD_LightAnim* next;
    HSD_AObjDesc* aobjdesc;
    HSD_WObjAnim* position_anim;
    HSD_WObjAnim* interest_anim;
};

/* ========================================================================= */
/*  LObj class info                                                          */
/* ========================================================================= */

struct HSD_LObjInfo {
    HSD_ObjInfo parent;
    int (*load)(HSD_LObj* lobj, HSD_LightDesc* ldesc);
};

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

extern HSD_LObjInfo hsdLObj;

#define HSD_LOBJ(o) ((HSD_LObj*) (o))
#define HSD_LOBJ_INFO(i) ((HSD_LObjInfo*) (i))
#define HSD_LOBJ_METHOD(o) HSD_LOBJ_INFO(HSD_OBJECT_METHOD(o))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

u32 HSD_LObjGetFlags(HSD_LObj* lobj);
void HSD_LObjSetFlags(HSD_LObj* lobj, u32 flags);
void HSD_LObjClearFlags(HSD_LObj* lobj, u32 flags);
void HSD_LObjSetActive(HSD_LObj* lobj);
s32 HSD_LObjGetNbActive(void);
void HSD_LObjClearActive(void);
void HSD_LObjAddAnim(HSD_LObj* lobj, HSD_LightAnim* lanim);
void HSD_LObjAddAnimAll(HSD_LObj* lobj, HSD_LightAnim* lanim);
void HSD_LObjAnim(HSD_LObj* lobj);
void HSD_LObjAnimAll(HSD_LObj* lobj);
void HSD_LObjReqAnim(HSD_LObj* lobj, f32 startframe);
void HSD_LObjReqAnimAll(HSD_LObj* lobj, f32 startframe);
void HSD_LObjSetup(HSD_LObj* lobj, u32 color, f32 shininess);
void HSD_LObjSetColor(HSD_LObj* lobj, u32 color);
void HSD_LObjSetPosition(HSD_LObj* lobj, f32 x, f32 y, f32 z);
void HSD_LObjSetInterest(HSD_LObj* lobj, f32 x, f32 y, f32 z);
void HSD_LObjRemoveAll(HSD_LObj* lobj);
HSD_LObj* HSD_LObjAlloc(void);
HSD_LObj* HSD_LObjLoadDesc(HSD_LightDesc* ldesc);

static inline HSD_LObj* HSD_LObjGetNext(HSD_LObj* lobj)
{
    if (lobj == NULL) {
        return NULL;
    }
    return lobj->next;
}

#endif /* HSD_LOBJ_H */
