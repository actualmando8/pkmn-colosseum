/**
 * @file hsd_fog.h
 * @brief HSD Fog and FogAdj objects.
 *
 * Manages distance fog and fog adjustment (screen-space fog correction).
 *
 * Colosseum address range: 0x8019B7C0 (FogAdjInfoInit)
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

/**
 * Fog-range-adjustment object.
 *
 * @c flags selects which fields HSD_FogSet() honours (bits 29..31); if none
 * of them is set the adjustment is disabled entirely.
 *   bit 31 -> use @c center, else derive it from the viewport
 *   bit 30 -> use @c width,  else derive it from the viewport
 *   bit 29 -> use @c mtx,    else rebuild it from the current projection
 *
 * Layout verified against the binary (class size 0x54, see FogAdjInfoInit).
 */
struct HSD_FogAdj {
    /* 0x00 */ HSD_Obj parent;
    /* 0x08 */ u32 flags;
    /* 0x0C */ s16 center;
    /* 0x0E */ u16 width;
    /* 0x10 */ f32 mtx[4][4];   /* Mtx44 (4x4 matrix) */
    /* 0x50 */ HSD_AObj* aobj;
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
    /* 0x00 */ u32 flags;
    /* 0x04 */ s16 center;
    /* 0x06 */ u16 width;
    /* 0x08 */ f32 mtx[4][4];
};

struct HSD_FogDesc {
    /* 0x00 */ u32 type;
    /* 0x04 */ HSD_FogAdjDesc* fogadjdesc;
    /* 0x08 */ f32 start;
    /* 0x0C */ f32 end;
    /* 0x10 */ u32 color;
};

/**
 * Fog class info. Unlike Melee's, Colosseum's carries an extra virtual slot
 * at 0x3C (the AObj interpret callback), making the info 0x40 bytes.
 */
struct HSD_FogInfo {
    /* 0x00 */ HSD_ObjInfo parent;
    /* 0x3C */ void (*update)(HSD_Fog* fog, s32 type, f32* value);
};

struct HSD_FogAdjInfo {
    /* 0x00 */ HSD_ObjInfo parent;
};

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_FogSet(HSD_Fog* fog);
HSD_Fog* HSD_FogLoadDesc(HSD_FogDesc* desc);

#endif /* HSD_FOG_H */
