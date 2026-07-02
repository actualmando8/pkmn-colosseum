#include "dolphin/types.h"

extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);
extern u8 lbl_80465620[];

/* Initialize matrix allocation data. */
void HSD_MtxInitAllocData(void) {
    HSD_ObjAllocInit(lbl_80465620, 0x30, 4);
}
