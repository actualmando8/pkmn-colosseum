#include "dolphin/types.h"

#pragma section ".data"

extern void* jumptable_80369CD4[];
extern void* jumptable_80369CF8[];

extern u8 inpGetExCtrl[];
extern u8 inpSetExCtrl[];

/* Auto-carved .data unit 0x80369CD4..0x80369D1C (2 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* jumptable_80369CD4[9] = {
    (void*)((u8*)inpGetExCtrl + 0x38),
    (void*)((u8*)inpGetExCtrl + 0x40),
    (void*)((u8*)inpGetExCtrl + 0x48),
    (void*)((u8*)inpGetExCtrl + 0x50),
    (void*)((u8*)inpGetExCtrl + 0x58),
    (void*)((u8*)inpGetExCtrl + 0x60),
    (void*)((u8*)inpGetExCtrl + 0x68),
    (void*)((u8*)inpGetExCtrl + 0x70),
    (void*)((u8*)inpGetExCtrl + 0x78),
};

void* jumptable_80369CF8[9] = {
    (void*)((u8*)inpSetExCtrl + 0x70),
    (void*)((u8*)inpSetExCtrl + 0x78),
    (void*)((u8*)inpSetExCtrl + 0x80),
    (void*)((u8*)inpSetExCtrl + 0x88),
    (void*)((u8*)inpSetExCtrl + 0x90),
    (void*)((u8*)inpSetExCtrl + 0x98),
    (void*)((u8*)inpSetExCtrl + 0xA0),
    (void*)((u8*)inpSetExCtrl + 0xA8),
    (void*)((u8*)inpSetExCtrl + 0xB0),
};

