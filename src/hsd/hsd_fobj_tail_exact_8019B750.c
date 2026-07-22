#include "hsd/hsd_fobj.h"

extern void HSD_ObjFree(void* list, void* data);
extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);
extern u8 lbl_80465378[];

void HSD_FObjRemove(HSD_FObj* data)
{
    if (data != NULL) {
        HSD_ObjFree(lbl_80465378, data);
    }
}

void HSD_FObjInitAllocData(void)
{
    HSD_ObjAllocInit(lbl_80465378, 0x30, 4);
}

void* HSD_FObjGetAllocData(void)
{
    return lbl_80465378;
}
