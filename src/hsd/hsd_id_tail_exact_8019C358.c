#include "hsd/hsd_id.h"
#include "hsd/hsd_objalloc.h"

extern void* memset(void* dst, int val, u32 size);
extern void HSD_ObjAllocInit(HSD_ObjAllocData* list, u32 size, u32 alignment);
extern HSD_IDTable lbl_804653A8;
extern HSD_ObjAllocData lbl_8046553C;

void HSD_IDSetup(void)
{
    memset(&lbl_804653A8, 0, sizeof(HSD_IDTable));
}

void HSD_IDInitAllocData(void)
{
    HSD_ObjAllocInit(&lbl_8046553C, sizeof(IDEntry), 4);
}

HSD_ObjAllocData* HSD_IDGetAllocData(void)
{
    return &lbl_8046553C;
}
