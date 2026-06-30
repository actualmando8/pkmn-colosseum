#include "hsd/hsd_mobj.h"

u32 HSD_MObjGetFlags(HSD_MObj* mobj)
{
    if (mobj != NULL) {
        return mobj->rendermode;
    }
    return 0;
}
