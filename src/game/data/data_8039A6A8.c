#include "dolphin/types.h"

#pragma section ".data"

extern u8 lbl_8039A6A8[];
extern void* lbl_8039A6B8[];

extern u8 fn_8025F514[];

/* Auto-carved .data unit 0x8039A6A8..0x8039A6C8 (2 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

u8 lbl_8039A6A8[16] = {
    0x00, 0x4B, 0x00, 0x04, 0x00, 0x4C, 0x00, 0x04, 0x00, 0x4D, 0x00, 0x04,
    0x00, 0x7A, 0x00, 0x04,
};

void* lbl_8039A6B8[4] = {
    (void*)((u8*)fn_8025F514),
    (void*)0x0000007F,
    (void*)0x00000000,
    (void*)0x00000000,
};

