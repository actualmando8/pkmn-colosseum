/**
 * @file hsd_cobj.h
 * @brief HSD CObj - Camera objects.
 *
 * CObj manages camera state: viewport, scissor, projection type
 * (perspective/frustum/ortho), eye position, interest point,
 * up vector, and the resulting view/projection matrices.
 *
 * Colosseum address range: 0x80193C24 (HSD_CObjInit)
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_COBJ_H
#define HSD_COBJ_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"
#include "hsd/hsd_object.h"

/* ========================================================================= */
/*  Projection types                                                         */
/* ========================================================================= */

#define PROJ_PERSPECTIVE 1
#define PROJ_FRUSTUM     2
#define PROJ_ORTHO       3

/* ========================================================================= */
/*  Viewport / scissor types                                                 */
/* ========================================================================= */

typedef struct _Scissor {
    u16 left;
    u16 right;
    u16 top;
    u16 bottom;
} Scissor;

typedef struct _HSD_RectS16 {
    s16 xmin;
    s16 xmax;
    s16 ymin;
    s16 ymax;
} HSD_RectS16;

typedef struct _HSD_RectF32 {
    f32 xmin;
    f32 xmax;
    f32 ymin;
    f32 ymax;
} HSD_RectF32;

/* ========================================================================= */
/*  CObj structure                                                           */
/* ========================================================================= */

struct HSD_CObj {
    /*  +0 */ HSD_Obj parent;
    /*  +8 */ u32 flags;
    /*  +C */ HSD_RectF32 viewport;
    /* +1C */ Scissor scissor;
    /* +24 */ HSD_WObj* eyepos;
    /* +28 */ HSD_WObj* interest;
    /* +2C */ union {
        f32 roll;
        struct { f32 x; f32 y; f32 z; } up;
    } u;
    /* +38 */ f32 near;
    /* +3C */ f32 far;
    /* +40 */ union {
        struct {
            f32 fov;
            f32 aspect;
        } perspective;
        struct {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } frustum;
        struct {
            f32 top;
            f32 bottom;
            f32 left;
            f32 right;
        } ortho;
    } projection_param;
    /* +50 */ u8 projection_type;
    /* +54 */ f32 view_mtx[3][4];  /* View matrix (Mtx) */
    /* +84 */ HSD_AObj* aobj;
    /* +88 */ f32* proj_mtx;       /* Projection matrix pointer */
};

/* ========================================================================= */
/*  Camera descriptor types                                                  */
/* ========================================================================= */

struct HSD_CameraDescCommon {
    char* class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc* eyepos;
    HSD_WObjDesc* interest;
    f32 roll;
    void* up_vector;     /* Vec3* */
    f32 nnear;
    f32 ffar;
};

struct HSD_CameraDescPerspective {
    char* class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc* eyepos;
    HSD_WObjDesc* interest;
    f32 roll;
    void* up_vector;
    f32 nnear;
    f32 ffar;
    f32 fov;
    f32 aspect;
};

struct HSD_CameraDescFrustum {
    char* class_name;
    u16 flags;
    u16 projection_type;
    HSD_RectS16 viewport;
    Scissor scissor;
    HSD_WObjDesc* eyepos;
    HSD_WObjDesc* interest;
    f32 roll;
    void* up_vector;
    f32 nnear;
    f32 ffar;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
};

union HSD_CObjDesc {
    char* class_name;
    HSD_CameraDescCommon common;
    HSD_CameraDescFrustum frustum;
    HSD_CameraDescFrustum ortho;
    HSD_CameraDescPerspective perspective;
};

/* ========================================================================= */
/*  CObj class info                                                          */
/* ========================================================================= */

struct HSD_CObjInfo {
    HSD_ObjInfo parent;
    int (*load)(HSD_CObj* cobj, HSD_CObjDesc* desc);
};

/* ========================================================================= */
/*  Camera animation                                                         */
/* ========================================================================= */

struct HSD_CameraAnim {
    HSD_AObjDesc* aobjdesc;
    HSD_WObjAnim* eye_anim;
    HSD_WObjAnim* interest_anim;
};

/* ========================================================================= */
/*  Globals and macros                                                       */
/* ========================================================================= */

#define HSD_COBJ(o) ((HSD_CObj*) (o))
#define HSD_COBJ_INFO(i) ((HSD_CObjInfo*) (i))
#define HSD_COBJ_METHOD(o) HSD_COBJ_INFO(HSD_OBJECT_METHOD(o))

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void HSD_CObjEraseScreen(HSD_CObj* cobj, s32 enable_color, s32 enable_alpha,
                         s32 enable_depth);
void HSD_CObjRemoveAnim(HSD_CObj* cobj);
HSD_WObj* HSD_CObjGetInterestWObj(HSD_CObj* cobj);
void HSD_CObjSetInterestWObj(HSD_CObj* cobj, HSD_WObj* interest);
HSD_WObj* HSD_CObjGetEyePositionWObj(HSD_CObj* cobj);
void HSD_CObjSetEyePositionWObj(HSD_CObj* cobj, HSD_WObj* eyepos);
BOOL HSD_CObjSetCurrent(HSD_CObj* cobj);
void HSD_CObjEndCurrent(void);
void HSD_CObjGetEyePosition(HSD_CObj* cobj, f32* x, f32* y, f32* z);
HSD_CObj* HSD_CObjAlloc(void);
void HSD_CObjAddAnim(HSD_CObj* cobj, HSD_CameraAnim* canim);
void HSD_CObjAnim(HSD_CObj* cobj);
void HSD_CObjReqAnim(HSD_CObj* cobj, f32 startframe);
void HSD_CObjSetupViewingMtx(HSD_CObj* cobj);
f32 HSD_CObjGetEyeDistance(HSD_CObj* cobj);
f32 HSD_CObjGetFov(HSD_CObj* cobj);
void HSD_CObjSetFov(HSD_CObj* cobj, f32 fov);
f32 HSD_CObjGetAspect(HSD_CObj* cobj);
void HSD_CObjSetAspect(HSD_CObj* cobj, f32 aspect);
f32 HSD_CObjGetNear(HSD_CObj* cobj);
void HSD_CObjSetNear(HSD_CObj* cobj, f32 near);
f32 HSD_CObjGetFar(HSD_CObj* cobj);
void HSD_CObjSetFar(HSD_CObj* cobj, f32 far);
int HSD_CObjGetProjectionType(HSD_CObj* cobj);
void HSD_CObjSetProjectionType(HSD_CObj* cobj, u32 type);
void HSD_CObjSetPerspective(HSD_CObj* cobj, f32 fov, f32 aspect);
void HSD_CObjSetFrustum(HSD_CObj* cobj, f32 top, f32 bottom, f32 left,
                        f32 right);
void HSD_CObjSetOrtho(HSD_CObj* cobj, f32 top, f32 bottom, f32 left,
                      f32 right);
u32 HSD_CObjGetFlags(HSD_CObj* cobj);
void HSD_CObjSetFlags(HSD_CObj* cobj, u32 flags);
void HSD_CObjClearFlags(HSD_CObj* cobj, u32 flags);
HSD_CObj* HSD_CObjGetCurrent(void);
void HSD_CObjInit(HSD_CObj* cobj, HSD_CObjDesc* desc);
HSD_CObj* HSD_CObjLoadDesc(HSD_CObjDesc* desc);
void HSD_CObjSetDefaultClass(HSD_ClassInfo* info);

static inline f32* HSD_CObjGetViewingMtxPtrDirect(HSD_CObj* cobj)
{
    return (f32*)cobj->view_mtx;
}

#endif /* HSD_COBJ_H */
