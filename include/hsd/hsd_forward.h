/**
 * @file hsd_forward.h
 * @brief Forward declarations for all HSD (HAL SysDolphin) types.
 *
 * Adapted from the Super Smash Bros. Melee decompilation project
 * (doldecomp/melee) for use in the Pokemon Colosseum decompilation.
 *
 * HAL Laboratory's SysDolphin library provides the rendering and
 * scene graph infrastructure used by both Melee and Colosseum.
 */
#ifndef HSD_FORWARD_H
#define HSD_FORWARD_H

#include "dolphin/types.h"

/* ========================================================================= */
/*  Forward declarations - structs                                           */
/* ========================================================================= */

typedef struct HSD_AnimJoint HSD_AnimJoint;
typedef struct HSD_AObj HSD_AObj;
typedef struct HSD_AObjDesc HSD_AObjDesc;
typedef struct HSD_Archive HSD_Archive;
typedef struct HSD_CameraAnim HSD_CameraAnim;
typedef struct HSD_CameraDescCommon HSD_CameraDescCommon;
typedef struct HSD_CameraDescFrustum HSD_CameraDescFrustum;
typedef struct HSD_CameraDescPerspective HSD_CameraDescPerspective;
typedef struct HSD_CObj HSD_CObj;
typedef struct HSD_CObjInfo HSD_CObjInfo;
typedef struct HSD_DObj HSD_DObj;
typedef struct HSD_DObjDesc HSD_DObjDesc;
typedef struct HSD_DObjInfo HSD_DObjInfo;
typedef struct HSD_Envelope HSD_Envelope;
typedef struct HSD_EnvelopeDesc HSD_EnvelopeDesc;
typedef struct HSD_Exp HSD_Exp;
typedef struct HSD_ExpDesc HSD_ExpDesc;
typedef struct HSD_ByteCodeExpDesc HSD_ByteCodeExpDesc;
typedef struct _HSD_FObj HSD_FObj;
typedef struct HSD_Fog HSD_Fog;
typedef struct HSD_FogAdj HSD_FogAdj;
typedef struct HSD_FogAdjDesc HSD_FogAdjDesc;
typedef struct HSD_FogAdjInfo HSD_FogAdjInfo;
typedef struct HSD_FogDesc HSD_FogDesc;
typedef struct HSD_FogInfo HSD_FogInfo;
typedef struct HSD_GObj HSD_GObj;
typedef struct HSD_GObjProc HSD_GObjProc;
typedef struct HSD_IKHint HSD_IKHint;
typedef struct HSD_IKHintDesc HSD_IKHintDesc;
typedef struct HSD_ImageDesc HSD_ImageDesc;
typedef struct HSD_JObj HSD_JObj;
typedef struct HSD_Joint HSD_Joint;
typedef struct HSD_LightAnim HSD_LightAnim;
typedef struct HSD_LightAttn HSD_LightAttn;
typedef struct HSD_LightDesc HSD_LightDesc;
typedef struct HSD_LightPoint HSD_LightPoint;
typedef struct HSD_LightPointDesc HSD_LightPointDesc;
typedef struct HSD_LightSpot HSD_LightSpot;
typedef struct HSD_LightSpotDesc HSD_LightSpotDesc;
typedef struct HSD_LObj HSD_LObj;
typedef struct HSD_LObjInfo HSD_LObjInfo;
typedef struct HSD_MatAnimJoint HSD_MatAnimJoint;
typedef struct HSD_Material HSD_Material;
typedef struct HSD_MObj HSD_MObj;
typedef struct HSD_MObjInfo HSD_MObjInfo;
typedef struct HSD_Obj HSD_Obj;
typedef struct HSD_PEDesc HSD_PEDesc;
typedef struct HSD_PObj HSD_PObj;
typedef struct HSD_PObjDesc HSD_PObjDesc;
typedef struct HSD_PObjInfo HSD_PObjInfo;
typedef struct HSD_RObj HSD_RObj;
typedef struct HSD_RObjAnimJoint HSD_RObjAnimJoint;
typedef struct HSD_RObjDesc HSD_RObjDesc;
typedef struct HSD_Rvalue HSD_Rvalue;
typedef struct HSD_RvalueList HSD_RvalueList;
typedef struct HSD_Shadow HSD_Shadow;
typedef struct HSD_ShapeAnim HSD_ShapeAnim;
typedef struct HSD_ShapeAnimDObj HSD_ShapeAnimDObj;
typedef struct HSD_ShapeAnimJoint HSD_ShapeAnimJoint;
typedef struct HSD_ShapeSet HSD_ShapeSet;
typedef struct HSD_ShapeSetDesc HSD_ShapeSetDesc;
typedef struct HSD_Spline HSD_Spline;
typedef struct HSD_TObj HSD_TObj;
typedef struct HSD_VtxDescList HSD_VtxDescList;
typedef struct HSD_WObj HSD_WObj;
typedef struct HSD_WObjAnim HSD_WObjAnim;
typedef struct HSD_WObjDesc HSD_WObjDesc;
typedef struct HSD_WObjInfo HSD_WObjInfo;

typedef union HSD_CObjDesc HSD_CObjDesc;
typedef union HSD_ObjData HSD_ObjData;
typedef union HSD_TExp HSD_TExp;

/* ========================================================================= */
/*  Forward declarations - linked list types                                 */
/* ========================================================================= */

typedef struct _HSD_SList {
    struct _HSD_SList* next;
    void* data;
} HSD_SList;

/* ========================================================================= */
/*  Function pointer types                                                   */
/* ========================================================================= */

typedef void (*GObj_RenderFunc)(HSD_GObj* gobj, int code);
typedef void (*HSD_ObjUpdateFunc)(void* obj, u32 type, HSD_ObjData* fval);
typedef void (*HSD_GObjEvent)(HSD_GObj* gobj);
typedef void (*HSD_MObjSetupFunc)(HSD_MObj* mobj, u32 rendermode);

/* ========================================================================= */
/*  PObj constants                                                           */
/* ========================================================================= */

#define HSD_A_S_W0 2
#define HSD_DEFAULT_MAX_SHAPE_VERTICES 2000
#define HSD_DEFAULT_MAX_SHAPE_NORMALS 2000

#define POBJ_ANIM       (1 << 3)
#define POBJ_SKIN       (0 << 12)
#define POBJ_SHAPEANIM  (1 << 12)
#define POBJ_ENVELOPE   (2 << 12)

#define pobj_type(o) ((o)->flags & 0x3000)

#define POBJ_CULLFRONT  (1 << 14)
#define POBJ_CULLBACK   (1 << 15)

#define SHAPESET_AVERAGE  1
#define SHAPESET_ADDITIVE (1 << 1)

/* ========================================================================= */
/*  Render / display constants                                               */
/* ========================================================================= */

typedef enum HSD_TrspMask {
    HSD_TRSP_OPA     = 1,
    HSD_TRSP_XLU     = 2,
    HSD_TRSP_TEXEDGE = 4,
    HSD_TRSP_ALL     = 7,
} HSD_TrspMask;

/* ========================================================================= */
/*  Light attribute animation indices                                        */
/* ========================================================================= */

#define HSD_A_L_LITC_R    9
#define HSD_A_L_LITC_G    10
#define HSD_A_L_LITC_B    11
#define HSD_A_L_VIS       12
#define HSD_A_L_A0        13
#define HSD_A_L_A1        14
#define HSD_A_L_A2        15
#define HSD_A_L_K0        16
#define HSD_A_L_K1        17
#define HSD_A_L_K2        18
#define HSD_A_L_CUTOFF    19
#define HSD_A_L_REFDIST   20
#define HSD_A_L_REFBRIGHT 21
#define HSD_A_L_LITC_A    22

/* ========================================================================= */
/*  Light object flags                                                       */
/* ========================================================================= */

#define LOBJ_AMBIENT    (0 << 0)
#define LOBJ_INFINITE   (1 << 0)
#define LOBJ_POINT      (2 << 0)
#define LOBJ_SPOT       (3 << 0)
#define LOBJ_DIFFUSE    (1 << 2)
#define LOBJ_SPECULAR   (1 << 3)
#define LOBJ_ALPHA      (1 << 4)
#define LOBJ_HIDDEN     (1 << 5)
#define LOBJ_RAW_PARAM  (1 << 6)
#define LOBJ_DIFF_DIRTY (1 << 7)
#define LOBJ_SPEC_DIRTY (1 << 8)

#define LOBJ_TYPE_MASK  3

#define LOBJ_LIGHT_ATTN_NONE 0
#define LOBJ_LIGHT_ATTN      1

#endif /* HSD_FORWARD_H */
