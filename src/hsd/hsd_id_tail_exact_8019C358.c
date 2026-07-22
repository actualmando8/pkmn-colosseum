#include "hsd/hsd_id.h"

extern void* memset(void* dst, int val, u32 size);
extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);
extern HSD_IDTable lbl_804653A8;
extern u8 lbl_8046553C[];

void HSD_IDSetup(void)
{
    memset(&lbl_804653A8, 0, sizeof(HSD_IDTable));
}

void HSD_IDInitAllocData(void)
{
    HSD_ObjAllocInit(lbl_8046553C, 0xC, 4);
}

void* HSD_IDGetAllocData(void)
{
    return lbl_8046553C;
}
