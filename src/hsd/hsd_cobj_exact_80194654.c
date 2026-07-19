/**
 * @file hsd_cobj_exact_80194654.c
 * @brief Camera projection extent accessors.
 */
#include "hsd/hsd_cobj.h"

extern f64 tan(f64);
extern const f32 lbl_8047D978;
extern const f32 lbl_8047D97C;
extern const f32 lbl_8047D980;

f32 HSD_CObjGetBottom(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return lbl_8047D978;
    }

    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return -cobj->near *
               (f32) tan(lbl_8047D97C *
                         (lbl_8047D980 *
                          cobj->projection_param.perspective.fov));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.bottom;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.bottom;
    default:
        return lbl_8047D978;
    }
}

f32 HSD_CObjGetTop(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return lbl_8047D978;
    }

    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
        return cobj->near *
               (f32) tan(lbl_8047D97C *
                         (lbl_8047D980 *
                          cobj->projection_param.perspective.fov));
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.top;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.top;
    default:
        return lbl_8047D978;
    }
}
