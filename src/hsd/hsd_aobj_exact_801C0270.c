#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"

extern HSD_SList* lbl_8047B388;

void _HSD_AObjForgetMemory(void* low, void* high)
{
    lbl_8047B388 = NULL;
}

void HSD_AObjSetRate(HSD_AObj* aobj, f32 rate)
{
    if (aobj != NULL) {
        aobj->framerate = rate;
    }
}
