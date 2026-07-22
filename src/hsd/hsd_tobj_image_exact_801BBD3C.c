#include "dolphin/types.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_tobj.h"

extern void fn_80193AF0(void* mem, s32 size);
extern void* fn_80193B10(s32 size);
extern void* memset(void* dst, int value, u32 size);
extern const char lbl_8047DEB0[7];
extern const char lbl_8047DEB8[6];

void HSD_ImageDescFree(HSD_ImageDesc* idesc)
{
    fn_80193AF0(idesc, sizeof(HSD_ImageDesc));
}

void HSD_ImageDescRemove(HSD_ImageDesc* idesc)
{
    fn_80193AF0(idesc, sizeof(HSD_ImageDesc));
}

HSD_ImageDesc* HSD_ImageDescAlloc(void)
{
    HSD_ImageDesc* idesc = fn_80193B10(sizeof(HSD_ImageDesc));
    if (idesc == NULL) {
        __assert(lbl_8047DEB0, 0x8F7, lbl_8047DEB8);
    }
    memset(idesc, 0, sizeof(HSD_ImageDesc));
    return idesc;
}
