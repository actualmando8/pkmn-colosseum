#include "dolphin/types.h"
#include "game/gs_render_util.h"
#include "hsd/hsd_object.h"

extern GSRenderCamera* lbl_8047AA74;

void fn_800D2738(GSRenderCamera* camera)
{
    void* cobj;

    if (camera == lbl_8047AA74) {
        lbl_8047AA74 = NULL;
    }
    cobj = camera->cobj;
    if (cobj != NULL && ref_DEC(cobj)) {
        hsdDelete(cobj);
    }
    camera->active = 0;
}
