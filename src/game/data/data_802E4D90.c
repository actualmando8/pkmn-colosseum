#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_802E4D90[];

extern u8 fn_8001329C[];

/* Auto-carved .data unit 0x802E4D90..0x802E4DAC (1 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_802E4D90[7] = {
    (void*)((u8*)fn_8001329C + 0x50),
    (void*)((u8*)fn_8001329C + 0x98),
    (void*)((u8*)fn_8001329C + 0x11C),
    (void*)((u8*)fn_8001329C + 0x1A0),
    (void*)((u8*)fn_8001329C + 0x224),
    (void*)((u8*)fn_8001329C + 0x2A8),
    (void*)((u8*)fn_8001329C + 0x32C),
};

