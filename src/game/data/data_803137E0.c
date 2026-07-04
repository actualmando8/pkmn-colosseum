#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_803137E0[];
extern void* jumptable_803137FC[];

extern u8 TRKDoReadMemory[];
extern u8 TRKDoWriteMemory[];

/* Auto-carved .data unit 0x803137E0..0x80313818 (2 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_803137E0[7] = {
    (void*)((u8*)TRKDoWriteMemory + 0x194),
    (void*)((u8*)TRKDoWriteMemory + 0x1B4),
    (void*)((u8*)TRKDoWriteMemory + 0x18C),
    (void*)((u8*)TRKDoWriteMemory + 0x1B4),
    (void*)((u8*)TRKDoWriteMemory + 0x19C),
    (void*)((u8*)TRKDoWriteMemory + 0x1A4),
    (void*)((u8*)TRKDoWriteMemory + 0x1AC),
};

void* jumptable_803137FC[7] = {
    (void*)((u8*)TRKDoReadMemory + 0x19C),
    (void*)((u8*)TRKDoReadMemory + 0x1BC),
    (void*)((u8*)TRKDoReadMemory + 0x194),
    (void*)((u8*)TRKDoReadMemory + 0x1BC),
    (void*)((u8*)TRKDoReadMemory + 0x1A4),
    (void*)((u8*)TRKDoReadMemory + 0x1AC),
    (void*)((u8*)TRKDoReadMemory + 0x1B4),
};

