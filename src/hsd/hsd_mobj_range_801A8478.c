#include "dolphin/types.h"

extern void fn_801AA35C(void* list, u32 size, u32 alignment);
extern u8 lbl_80465620[];

/* Initialize matrix allocation data. */
void HSD_MtxInitAllocData(void) {
    fn_801AA35C(lbl_80465620, 0x30, 4);
}
