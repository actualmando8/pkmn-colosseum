/**
 * @file hsd_tobj.h
 * @brief HSD TObj - Texture objects.
 *
 * TObj manages texture state including image data, texture coordinates,
 * filtering, wrapping, LOD, TEV color/alpha combine settings, and
 * texture animation.
 *
 * Colosseum address range: 0x801BBAC8 (HSD_TObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_TOBJ_H
#define HSD_TOBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  Texture animation attribute indices                                      */
/* ========================================================================= */

#define HSD_A_T_TIMG     1
#define HSD_A_T_TRAU     2
#define HSD_A_T_TRAV     3
#define HSD_A_T_SCAU     4
#define HSD_A_T_SCAV     5
#define HSD_A_T_ROTX     6
#define HSD_A_T_ROTY     7
#define HSD_A_T_ROTZ     8
#define HSD_A_T_BLEND    9
#define HSD_A_T_TCLT     10
#define HSD_A_T_LOD_BIAS 11

/* ========================================================================= */
/*  Texture coordinate source types                                          */
/* ========================================================================= */

#define TEX_COORD_UV         0
#define TEX_COORD_REFLECTION 1
#define TEX_COORD_HILIGHT    2
#define TEX_COORD_SHADOW     3
#define TEX_COORD_TOON       4
#define TEX_COORD_GRADATION  5
#define TEX_COORD_MASK       (0x0f)
#define tobj_coord(T) ((T)->flags & TEX_COORD_MASK)

/* ========================================================================= */
/*  Texture color mapping modes                                              */
/* ========================================================================= */

#define TEX_COLORMAP_NONE       (0 << 16)
#define TEX_COLORMAP_ALPHA_MASK (1 << 16)
#define TEX_COLORMAP_RGB_MASK   (2 << 16)
#define TEX_COLORMAP_BLEND      (3 << 16)
#define TEX_COLORMAP_MODULATE   (4 << 16)
#define TEX_COLORMAP_REPLACE    (5 << 16)
#define TEX_COLORMAP_PASS       (6 << 16)
#define TEX_COLORMAP_ADD        (7 << 16)
#define TEX_COLORMAP_SUB        (8 << 16)
#define TEX_COLORMAP_MASK       (0x0f << 16)
#define tobj_colormap(T) ((T)->flags & TEX_COLORMAP_MASK)

/* ========================================================================= */
/*  Texture alpha mapping modes                                              */
/* ========================================================================= */

#define TEX_ALPHAMAP_NONE       (0 << 20)
#define TEX_ALPHAMAP_ALPHA_MASK (1 << 20)
#define TEX_ALPHAMAP_BLEND      (2 << 20)
#define TEX_ALPHAMAP_MODULATE   (3 << 20)
#define TEX_ALPHAMAP_REPLACE    (4 << 20)
#define TEX_ALPHAMAP_PASS       (5 << 20)
#define TEX_ALPHAMAP_ADD        (6 << 20)
#define TEX_ALPHAMAP_SUB        (7 << 20)
#define TEX_ALPHAMAP_MASK       (0x0f << 20)
#define tobj_alphamap(T) ((T)->flags & TEX_ALPHAMAP_MASK)

/* ========================================================================= */
/*  Texture light map flags                                                  */
/* ========================================================================= */

#define TEX_LIGHTMAP_DIFFUSE  (0x1 << 4)
#define TEX_LIGHTMAP_SPECULAR (0x1 << 5)
#define TEX_LIGHTMAP_AMBIENT  (0x1 << 6)
#define TEX_LIGHTMAP_EXT      (0x1 << 7)
#define TEX_LIGHTMAP_SHADOW   (0x1 << 8)
#define TEX_LIGHTMAP_MASK \
    (TEX_LIGHTMAP_DIFFUSE | TEX_LIGHTMAP_SPECULAR | TEX_LIGHTMAP_AMBIENT | \
     TEX_LIGHTMAP_EXT | TEX_LIGHTMAP_SHADOW)
#define tobj_lightmap(T) ((T)->flags & TEX_LIGHTMAP_MASK)

#define TEX_BUMP       (0x1 << 24)
#define tobj_bump(T)   ((T)->flags & TEX_BUMP)
#define TEX_MTX_DIRTY  (1U << 31)

/* ========================================================================= */
/*  TObj structure                                                           */
/* ========================================================================= */

struct HSD_TObj {
    HSD_Obj parent;
    HSD_TObj* next;
    u32 id;           /* GXTexMapID */
    u32 src;          /* GXTexGenSrc */
    u32 mtxid;
    f32 rotate_x;     /* Quaternion rotation */
    f32 rotate_y;
    f32 rotate_z;
    f32 rotate_w;
    f32 scale_x;
    f32 scale_y;
    f32 scale_z;
    f32 translate_x;
    f32 translate_y;
    f32 translate_z;
    u32 wrap_s;       /* GXTexWrapMode */
    u32 wrap_t;       /* GXTexWrapMode */
    u8 repeat_s;
    u8 repeat_t;
    u32 flags;
    f32 blending;
    u32 magFilt;      /* GXTexFilter */
    HSD_ImageDesc* imagedesc;
    void* tlut;
    void* lod;
    HSD_AObj* aobj;
    HSD_ImageDesc** imagetbl;
    void** tluttbl;
    u8 tlut_no;
    f32 mtx[3][4];    /* Mtx (3x4 matrix) */
    u32 coord;        /* GXTexCoordID */
    void* tev;
};

/* ========================================================================= */
/*  TObj descriptor (data format)                                            */
/* ========================================================================= */

typedef struct _HSD_TObjDesc {
    char* class_name;
    struct _HSD_TObjDesc* next;
    u32 id;            /* GXTexMapID */
    u32 src;           /* GXTexGenSrc */
    f32 rotate_x;
    f32 rotate_y;
    f32 rotate_z;
    f32 scale_x;
    f32 scale_y;
    f32 scale_z;
    f32 translate_x;
    f32 translate_y;
    f32 translate_z;
    u32 wrap_s;
    u32 wrap_t;
    u8 repeat_s;
    u8 repeat_t;
    u32 blend_flags;
    f32 blending;
    u32 magFilt;
    HSD_ImageDesc* imagedesc;
    void* tlutdesc;
    void* lod;
    void* tev;
} HSD_TObjDesc;

/* ========================================================================= */
/*  Image descriptor                                                         */
/* ========================================================================= */

struct HSD_ImageDesc {
    void* image_ptr;
    u16 width;
    u16 height;
    u32 format;        /* GXTexFmt */
    u32 mipmap;
    f32 minLOD;
    f32 maxLOD;
};

/* ========================================================================= */
/*  TObj class info                                                          */
/* ========================================================================= */

typedef struct _HSD_TObjInfo {
    HSD_ObjInfo parent;
    void (*make_mtx)(HSD_TObj* tobj);
    int (*load)(HSD_TObj* tobj, HSD_TObjDesc* desc);
    void (*make_texp)(HSD_TObj* tobj, u32 lightmap, u32 lightmap_done,
                      HSD_TExp** c, HSD_TExp** a, HSD_TExp** list);
} HSD_TObjInfo;

/* ========================================================================= */
/*  Texture animation                                                        */
/* ========================================================================= */

typedef struct _HSD_TexAnim {
    struct _HSD_TexAnim* next;
    u32 id;            /* GXTexMapID */
    HSD_AObjDesc* aobjdesc;
    HSD_ImageDesc** imagetbl;
    void** tluttbl;
    u16 n_imagetbl;
    u16 n_tluttbl;
} HSD_TexAnim;

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

extern HSD_TObjInfo hsdTObj;

#define HSD_TOBJ(o) ((HSD_TObj*) (o))
#define HSD_TOBJ_INFO(i) ((HSD_TObjInfo*) (i))
#define HSD_TOBJ_METHOD(o) HSD_TOBJ_INFO(HSD_OBJECT_METHOD(o))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_TObjRemoveAnimAll(HSD_TObj* tobj);
void HSD_TObjAddAnim(HSD_TObj* tobj, HSD_TexAnim* texanim);
void HSD_TObjAddAnimAll(HSD_TObj* tobj, HSD_TexAnim* texanim);
void HSD_TObjReqAnim(HSD_TObj* tobj, f32 startframe);
void HSD_TObjReqAnimAll(HSD_TObj* tobj, f32 startframe);
void HSD_TObjAnim(HSD_TObj* tobj);
void HSD_TObjAnimAll(HSD_TObj* tobj);
HSD_TObj* HSD_TObjLoadDesc(HSD_TObjDesc* td);
void HSD_TObjRemove(HSD_TObj* tobj);
void HSD_TObjRemoveAll(HSD_TObj* tobj);
HSD_TObj* HSD_TObjGetNext(HSD_TObj* tobj);
HSD_TObj* HSD_TObjAlloc(void);
void HSD_TObjSetup(HSD_TObj* tobj);

#endif /* HSD_TOBJ_H */
