#include "dolphin/types.h"

#pragma section ".data"

extern void* lbl_8036CA20[];
extern void* jumptable_8036CA64[];
extern void* jumptable_8036CA88[];
extern void* lbl_8036CAA8[];
extern void* jumptable_8036CAAC[];
extern void* jumptable_8036CAD0[];
extern void* jumptable_8036CAF4[];

extern u8 HSD_Index2LightID[];
extern u8 HSD_LObjSetup[];
extern u8 LObjInfoInit[];
extern u8 LObjUpdateFunc[];

/* Auto-carved .data unit 0x8036CA20..0x8036CB2C (7 objects). Non-relocated data as byte-exact u8[]; pointer/jump tables as void*[] for R_PPC_ADDR32 relocations. */

void* lbl_8036CA20[17] = {
    (void*)((u8*)LObjInfoInit),
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

void* jumptable_8036CA64[9] = {
    (void*)((u8*)HSD_Index2LightID + 0x20),
    (void*)((u8*)HSD_Index2LightID + 0x28),
    (void*)((u8*)HSD_Index2LightID + 0x30),
    (void*)((u8*)HSD_Index2LightID + 0x38),
    (void*)((u8*)HSD_Index2LightID + 0x40),
    (void*)((u8*)HSD_Index2LightID + 0x48),
    (void*)((u8*)HSD_Index2LightID + 0x50),
    (void*)((u8*)HSD_Index2LightID + 0x58),
    (void*)((u8*)HSD_Index2LightID + 0x60),
};

void* jumptable_8036CA88[8] = {
    (void*)((u8*)HSD_LObjSetup + 0xA44),
    (void*)((u8*)HSD_LObjSetup + 0xA4C),
    (void*)((u8*)HSD_LObjSetup + 0xA54),
    (void*)((u8*)HSD_LObjSetup + 0xA5C),
    (void*)((u8*)HSD_LObjSetup + 0xA64),
    (void*)((u8*)HSD_LObjSetup + 0xA6C),
    (void*)((u8*)HSD_LObjSetup + 0xA74),
    (void*)((u8*)HSD_LObjSetup + 0xA7C),
};

void* lbl_8036CAA8[1] = {
    (void*)((u8*)HSD_LObjSetup + 0xA84),
};

void* jumptable_8036CAAC[9] = {
    (void*)((u8*)HSD_LObjSetup + 0x968),
    (void*)((u8*)HSD_LObjSetup + 0x970),
    (void*)((u8*)HSD_LObjSetup + 0x978),
    (void*)((u8*)HSD_LObjSetup + 0x980),
    (void*)((u8*)HSD_LObjSetup + 0x988),
    (void*)((u8*)HSD_LObjSetup + 0x990),
    (void*)((u8*)HSD_LObjSetup + 0x998),
    (void*)((u8*)HSD_LObjSetup + 0x9A0),
    (void*)((u8*)HSD_LObjSetup + 0x9A8),
};

void* jumptable_8036CAD0[9] = {
    (void*)((u8*)HSD_LObjSetup + 0x12C),
    (void*)((u8*)HSD_LObjSetup + 0x134),
    (void*)((u8*)HSD_LObjSetup + 0x13C),
    (void*)((u8*)HSD_LObjSetup + 0x144),
    (void*)((u8*)HSD_LObjSetup + 0x14C),
    (void*)((u8*)HSD_LObjSetup + 0x154),
    (void*)((u8*)HSD_LObjSetup + 0x15C),
    (void*)((u8*)HSD_LObjSetup + 0x164),
    (void*)((u8*)HSD_LObjSetup + 0x16C),
};

void* jumptable_8036CAF4[14] = {
    (void*)((u8*)LObjUpdateFunc + 0x118),
    (void*)((u8*)LObjUpdateFunc + 0x164),
    (void*)((u8*)LObjUpdateFunc + 0x1B0),
    (void*)((u8*)LObjUpdateFunc + 0x30),
    (void*)((u8*)LObjUpdateFunc + 0x64),
    (void*)((u8*)LObjUpdateFunc + 0x88),
    (void*)((u8*)LObjUpdateFunc + 0xAC),
    (void*)((u8*)LObjUpdateFunc + 0xD0),
    (void*)((u8*)LObjUpdateFunc + 0xE8),
    (void*)((u8*)LObjUpdateFunc + 0x100),
    (void*)((u8*)LObjUpdateFunc + 0x64),
    (void*)((u8*)LObjUpdateFunc + 0x88),
    (void*)((u8*)LObjUpdateFunc + 0xAC),
    (void*)((u8*)LObjUpdateFunc + 0x1FC),
};

