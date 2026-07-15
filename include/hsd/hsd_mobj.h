/**
 * @file hsd_mobj.h
 * @brief HSD MObj - Material objects.
 *
 * MObj manages material state: render mode flags, texture chain,
 * material colors (ambient/diffuse/specular/alpha), pixel engine
 * settings, and TEV (Texture Environment) configuration.
 *
 * Colosseum address range: 0x801A6A34 (HSD_MObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_MOBJ_H
#define HSD_MOBJ_H

#include "dolphin/gx/GX.h"
#include "dolphin/types.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_tobj.h"

/* ========================================================================= */
/*  Animation flag masks                                                     */
/* ========================================================================= */

#define MOBJ_ANIM 0x4
#define ALL_ANIM  0x7FF

/* ========================================================================= */
/*  Material animation attribute indices                                     */
/* ========================================================================= */

#define HSD_A_M_AMBIENT_R  1
#define HSD_A_M_AMBIENT_G  2
#define HSD_A_M_AMBIENT_B  3
#define HSD_A_M_DIFFUSE_R  4
#define HSD_A_M_DIFFUSE_G  5
#define HSD_A_M_DIFFUSE_B  6
#define HSD_A_M_SPECULAR_R 7
#define HSD_A_M_SPECULAR_G 8
#define HSD_A_M_SPECULAR_B 9
#define HSD_A_M_ALPHA      10
#define HSD_A_M_PE_REF0    11
#define HSD_A_M_PE_REF1    12
#define HSD_A_M_PE_DSTALPHA 13

/* ========================================================================= */
/*  Render mode flags                                                        */
/* ========================================================================= */

#define RENDER_DIFFUSE_SHIFT 0
#define RENDER_DIFFUSE_BITS  (3 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_MAT0  (0 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_MAT   (1 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_VTX   (2 << RENDER_DIFFUSE_SHIFT)
#define RENDER_DIFFUSE_BOTH  (3 << RENDER_DIFFUSE_SHIFT)

#define RENDER_ALPHA_SHIFT  13
#define RENDER_ALPHA_BITS   (3 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_COMPAT (0 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_MAT    (1 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_VTX    (2 << RENDER_ALPHA_SHIFT)
#define RENDER_ALPHA_BOTH   (3 << RENDER_ALPHA_SHIFT)

#define RENDER_CONSTANT   (1 << 0)
#define RENDER_VERTEX     (1 << 1)
#define RENDER_DIFFUSE    (1 << 2)
#define RENDER_SPECULAR   (1 << 3)
#define CHANNEL_FIELD \
    (RENDER_CONSTANT | RENDER_VERTEX | RENDER_DIFFUSE | RENDER_SPECULAR)

#define RENDER_TEX0       (1 << 4)
#define RENDER_TEX1       (1 << 5)
#define RENDER_TEX2       (1 << 6)
#define RENDER_TEX3       (1 << 7)
#define RENDER_TEX4       (1 << 8)
#define RENDER_TEX5       (1 << 9)
#define RENDER_TEX6       (1 << 10)
#define RENDER_TEX7       (1 << 11)
#define RENDER_TEXTURES \
    (RENDER_TEX0 | RENDER_TEX1 | RENDER_TEX2 | RENDER_TEX3 | \
     RENDER_TEX4 | RENDER_TEX5 | RENDER_TEX6 | RENDER_TEX7)

#define RENDER_TOON       (1 << 12)

#define RENDER_SHADOW     (1 << 26)
#define RENDER_ZMODE_ALWAYS (1 << 27)
#define RENDER_NO_ZUPDATE (1 << 29)
#define RENDER_XLU        (1 << 30)

#define RENDER_BLENDING   (RENDER_XLU | RENDER_NO_ZUPDATE)

/* ========================================================================= */
/*  MObj structure                                                           */
/* ========================================================================= */

typedef struct _HSD_TExpTevDesc HSD_TExpTevDesc;

struct HSD_MObj {
    /* +00 */ HSD_Class parent;
    /* +04 */ u32 rendermode;
    /* +08 */ HSD_TObj* tobj;
    /* +0C */ HSD_Material* mat;
    /* +10 */ HSD_PEDesc* pe;
    /* +14 */ HSD_AObj* aobj;
    /* +18 */ HSD_TExpTevDesc* tevdesc;
    /* +1C */ HSD_TExp* texp;
};

/* ========================================================================= */
/*  Material color structure                                                 */
/* ========================================================================= */

struct HSD_Material {
    /* +00 */ GXColor ambient;
    /* +04 */ GXColor diffuse;
    /* +08 */ GXColor specular;
    /* +0C */ f32 alpha;
    /* +10 */ f32 shininess;
};

/* ========================================================================= */
/*  Pixel engine descriptor                                                  */
/* ========================================================================= */

struct HSD_PEDesc {
    u8 flags;
    u8 ref0;
    u8 ref1;
    u8 dst_alpha;
    u8 type;
    u8 src_factor;
    u8 dst_factor;
    u8 logic_op;
    u8 z_comp;
    u8 alpha_comp0;
    u8 alpha_op;
    u8 alpha_comp1;
};

/* ========================================================================= */
/*  MObj descriptor (data format)                                            */
/* ========================================================================= */

typedef struct _HSD_MObjDesc {
    char* class_name;
    u32 rendermode;
    HSD_TObjDesc* texdesc;
    HSD_Material* mat;
    void* renderdesc;
    HSD_PEDesc* pedesc;
} HSD_MObjDesc;

/* ========================================================================= */
/*  Material animation types                                                 */
/* ========================================================================= */

typedef struct _HSD_MatAnim {
    struct _HSD_MatAnim* next;
    HSD_AObjDesc* aobjdesc;
    HSD_TexAnim* texanim;
    void* renderanim;
} HSD_MatAnim;

struct HSD_MatAnimJoint {
    HSD_MatAnimJoint* child;
    HSD_MatAnimJoint* next;
    HSD_MatAnim* matanim;
};

/* ========================================================================= */
/*  MObj class info                                                          */
/* ========================================================================= */

struct HSD_MObjInfo {
    /* +00 */ HSD_ClassInfo parent;
    /* +3C */ HSD_MObjSetupFunc setup;
    /* +40 */ int (*load)(HSD_MObj* mobj, HSD_MObjDesc* desc);
    /* +44 */ HSD_TExp* (*make_texp)(HSD_MObj* mobj, HSD_TObj* tobj_top,
                                     HSD_TExp** list);
    /* +48 */ void (*setup_tev)(HSD_MObj* mobj, HSD_TObj* tobj,
                                u32 rendermode);
    /* +4C */ HSD_ObjUpdateFunc update;
    /* +50 */ void (*unset)(HSD_MObj* mobj, u32 rendermode);
};

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

extern HSD_MObjInfo hsdMObj;

#define HSD_MOBJ(o) ((HSD_MObj*) (o))
#define HSD_MOBJ_INFO(i) ((HSD_MObjInfo*) (i))
#define HSD_MOBJ_METHOD(o) HSD_MOBJ_INFO(HSD_CLASS_METHOD(o))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_MObjSetCurrent(HSD_MObj* mobj);
u32 HSD_MObjGetFlags(HSD_MObj* mobj);
void HSD_MObjSetFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjClearFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjRemoveAnimByFlags(HSD_MObj* mobj, u32 flags);
void HSD_MObjAddAnim(HSD_MObj* mobj, HSD_MatAnim* matanim);
void HSD_MObjReqAnim(HSD_MObj* mobj, f32 startframe);
void HSD_MObjAnim(HSD_MObj* mobj);
HSD_MObj* HSD_MObjLoadDesc(HSD_MObjDesc* mobjdesc);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
void HSD_MObjRemove(HSD_MObj* mobj);
void HSD_MObjAddShadowTexture(HSD_TObj* tobj);
void HSD_MObjAddTObjNext(HSD_MObj* mobj, HSD_TObj* tobj, HSD_TObj* next);
void HSD_MObjSetDefaultClass(HSD_ClassInfo* info);
void HSD_MObjCompileTev(HSD_MObj* mobj);
void HSD_MObjSetup(HSD_MObj* mobj, u32 rendermode);
void HSD_MObjUnset(HSD_MObj* mobj, u32 rendermode);
void HSD_MObjSetAlpha(HSD_MObj* mobj, f32 alpha);

HSD_TExp* MObjMakeTExp(HSD_MObj* mobj, HSD_TObj* tobj_top, HSD_TExp** list);
void MObjSetupTev(HSD_MObj* mobj, HSD_TObj* tobj, u32 rendermode);
int MObjLoad(HSD_MObj* mobj, HSD_MObjDesc* desc);
void MObjUpdateFunc(void* obj, u32 type, HSD_ObjData* val);

#endif /* HSD_MOBJ_H */
