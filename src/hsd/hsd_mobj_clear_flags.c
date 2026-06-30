#include "hsd/hsd_mobj.h"

void HSD_MObjClearFlags(HSD_MObj* mobj, u32 flags)
{
    if (mobj == NULL) {
        return;
    }
    mobj->rendermode &= ~flags;
}
