#include "dolphin/types.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_objalloc.h"

extern HSD_ObjAllocData lbl_80466DB8;
extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);

HSD_ObjAllocData* HSD_AObjGetAllocData(void)
{
    return &lbl_80466DB8;
}

void HSD_AObjInitAllocData(void)
{
    HSD_ObjAllocInit(&lbl_80466DB8, sizeof(HSD_AObj), 4);
}
