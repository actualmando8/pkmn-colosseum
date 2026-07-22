#include "dolphin/types.h"
#include "game/gs_render_util.h"
#include "hsd/hsd_cobj.h"

extern void HSD_CObjSetViewport(HSD_CObj* cobj, HSD_RectS16* rect);

void GScameraSetViewport(GSRenderCamera* camera, u32 x0, u32 y0, u32 x1,
                         u32 y1)
{
    HSD_RectS16 rect;

    if ((u16) x0 > 0x27E) {
        x0 = 0x27E;
    }
    if ((u16) y0 > 0x1DE) {
        y0 = 0x1DE;
    }
    if ((u16) x1 > 0x27F) {
        x1 = 0x27F;
    }
    if ((u16) y1 > 0x1DF) {
        y1 = 0x1DF;
    }
    rect.xmin = (s16) x0;
    rect.ymin = (s16) y0;
    rect.xmax = (s16) (x1 + 1);
    rect.ymax = (s16) (y1 + 1);
    HSD_CObjSetViewport((HSD_CObj*) camera->cobj, &rect);
}
