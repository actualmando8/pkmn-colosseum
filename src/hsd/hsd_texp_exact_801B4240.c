#include "dolphin/types.h"

extern u8 lbl_80465728[];
extern u8 lbl_80465754[];
extern u8 lbl_80465780[];

void* HSD_ChanGetAllocData(void)
{
    return lbl_80465728;
}

void* HSD_TevRegGetAllocData(void)
{
    return lbl_80465754;
}

void* HSD_RenderGetAllocData(void)
{
    return lbl_80465780;
}
