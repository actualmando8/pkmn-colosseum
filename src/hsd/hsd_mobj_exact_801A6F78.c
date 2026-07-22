#include "hsd/hsd_mobj.h"
#include "hsd/hsd_tobj.h"

extern void __assert(const char* file, u32 line, const char* expr);
extern const char lbl_8047DC18[7];
extern const char lbl_80274E5C[];

#define MOBJ_ASSERT(line, cond, expr) \
    ((cond) ? ((void) 0) : __assert(lbl_8047DC18, line, expr))

void MObjSetupTev(HSD_MObj* mobj, HSD_TObj* tobj, u32 rendermode)
{
    MOBJ_ASSERT(798, mobj->tevdesc, lbl_80274E5C);
    fn_801B45A4(mobj->tevdesc, mobj->texp);
    HSD_TObjSetupVolatileTev(tobj, rendermode);
}
