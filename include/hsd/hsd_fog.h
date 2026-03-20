/**
 * @file hsd_fog.h
 * @brief HSD Fog and FogAdj objects.
 *
 * Manages distance fog and fog adjustment (screen-space fog correction).
 *
 * Colosseum address range: 0x8019B7C0 (HSD_FogAdjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_FOG_H
#define HSD_FOG_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  FogAdj structure                                                         */
/* ========================================================================= */

struct HSD_FogAdj {
    /* 0x00 */ HSD_Obj parent;
    /* 0x08 */ s16 center;
    /* 0x0A */ u16 width;
    /* 0x0C */ f32 mtx[4][4];   /* Mtx44 (4x4 matrix) */
    /* 0x3C */ HSD_AObj* aobj;
};

/* ========================================================================= */
/*  Fog structure                                                            */
/* ========================================================================= */

struct HSD_Fog {
    /* 0x00 */ HSD_Obj parent;
    /* 0x08 */ u32 type;
    /* 0x0C */ HSD_FogAdj* fog_adj;
    /* 0x10 */ f32 start;
    /* 0x14 */ f32 end;
    /* 0x18 */ u32 color;       /* GXColor packed */
    /* 0x1C */ HSD_AObj* aobj;
};

/* ========================================================================= */
/*  Descriptors                                                              */
/* ========================================================================= */

struct HSD_FogAdjDesc {
    u16 center;
    u16 width;
    f32 mtx[4][4];
};

struct HSD_FogDesc {
    u32 type;
    HSD_FogAdjDesc* fogadjdesc;
    f32 start;
    f32 end;
    u32 color;
};

struct HSD_FogInfo {
    HSD_ObjInfo parent;
};

struct HSD_FogAdjInfo {
    HSD_ObjInfo parent;
};

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_FogSet(HSD_Fog* fog);
HSD_FogAdj* HSD_FogAdjLoadDesc(HSD_FogAdjDesc* desc);
void HSD_FogInit(HSD_Fog* fog, HSD_FogDesc* desc);
void HSD_FogAdjInit(HSD_FogAdj* fogadj, HSD_FogAdjDesc* desc);
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc);
HSD_Fog* HSD_FogAlloc(void);
HSD_FogAdj* HSD_FogAdjAlloc(void);
void HSD_FogReqAnim(HSD_Fog* fog, f32 frame);
void HSD_FogInterpretAnim(HSD_Fog* fog);

#endif /* HSD_FOG_H */
