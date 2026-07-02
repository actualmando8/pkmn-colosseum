#include "dolphin/types.h"

extern void HSD_ObjAllocInit(void* list, u32 size, u32 alignment);
extern u8 lbl_8046564C[];

/* Initialize vector allocation data. */
void HSD_VecInitAllocData(void) {
    HSD_ObjAllocInit(lbl_8046564C, 0xC, 4);
}
