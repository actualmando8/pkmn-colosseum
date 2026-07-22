#include "hsd/hsd_fobj.h"
#include "hsd/hsd_objalloc.h"

extern void HSD_ObjFree(HSD_ObjAllocData* list, void* data);
extern void HSD_ObjAllocInit(HSD_ObjAllocData* list, u32 size, u32 alignment);
extern HSD_ObjAllocData lbl_80465378;

void HSD_FObjRemove(HSD_FObj* data)
{
    if (data != NULL) {
        HSD_ObjFree(&lbl_80465378, data);
    }
}

void HSD_FObjInitAllocData(void)
{
    HSD_ObjAllocInit(&lbl_80465378, sizeof(HSD_FObj), 4);
}

HSD_ObjAllocData* HSD_FObjGetAllocData(void)
{
    return &lbl_80465378;
}
