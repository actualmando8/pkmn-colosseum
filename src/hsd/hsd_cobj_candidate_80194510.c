#include "hsd/hsd_cobj.h"

extern f64 tan(f64 value);
extern f32 lbl_8047D978;
extern f32 lbl_8047D97C;
extern f32 lbl_8047D980;

f32 HSD_CObjGetRight(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return lbl_8047D978;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
    {
        f32 inner = cobj->near *
                    (f32) tan(
                        lbl_8047D97C *
                        (lbl_8047D980 *
                         cobj->projection_param.perspective.fov));
        f32 aspect = cobj->projection_param.perspective.aspect;
        return aspect * inner;
    }
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.right;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.right;
    default:
        return lbl_8047D978;
    }
}

f32 HSD_CObjGetLeft(HSD_CObj* cobj)
{
    if (cobj == NULL) {
        return lbl_8047D978;
    }
    switch (cobj->projection_type) {
    case PROJ_PERSPECTIVE:
    {
        f32 inner = -cobj->near *
                    (f32) tan(
                        lbl_8047D97C *
                        (lbl_8047D980 *
                         cobj->projection_param.perspective.fov));
        f32 aspect = cobj->projection_param.perspective.aspect;
        return aspect * inner;
    }
    case PROJ_FRUSTUM:
        return cobj->projection_param.frustum.left;
    case PROJ_ORTHO:
        return cobj->projection_param.ortho.left;
    default:
        return lbl_8047D978;
    }
}
