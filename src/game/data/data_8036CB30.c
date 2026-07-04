#include "dolphin/types.h"

#pragma section ".data"

extern void* lbl_8036CB30[];
extern void* jumptable_8036CB84[];

extern u8 MObjInfoInit[];
extern u8 MObjUpdateFunc[];

/* Auto-carved .data unit 0x8036CB30..0x8036CBBC (2 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* lbl_8036CB30[21] = {
    (void*)((u8*)MObjInfoInit),
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
    (void*)0x00000000,
};

void* jumptable_8036CB84[14] = {
    (void*)((u8*)MObjUpdateFunc + 0x4C8),
    (void*)((u8*)MObjUpdateFunc + 0x2C),
    (void*)((u8*)MObjUpdateFunc + 0x88),
    (void*)((u8*)MObjUpdateFunc + 0xE4),
    (void*)((u8*)MObjUpdateFunc + 0x140),
    (void*)((u8*)MObjUpdateFunc + 0x19C),
    (void*)((u8*)MObjUpdateFunc + 0x1F8),
    (void*)((u8*)MObjUpdateFunc + 0x2A4),
    (void*)((u8*)MObjUpdateFunc + 0x300),
    (void*)((u8*)MObjUpdateFunc + 0x35C),
    (void*)((u8*)MObjUpdateFunc + 0x254),
    (void*)((u8*)MObjUpdateFunc + 0x3B8),
    (void*)((u8*)MObjUpdateFunc + 0x414),
    (void*)((u8*)MObjUpdateFunc + 0x470),
};

