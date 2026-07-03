#include "dolphin/types.h"

#pragma section ".data"

extern void FogUpdateFunc(void);

/*
 * Compiler-generated switch jump table for hsd_fog.c: FogUpdateFunc switch dispatch (22 case handlers).
 * Entries are code-label addresses within the owning function, expressed
 * as (function symbol + byte offset) so MWCC emits the same R_PPC_ADDR32
 * relocations (with addend) as the retail table instead of dead literals.
 */
void* jumptable_8036C864[22] = {
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x2C),
    (void*) ((u8*) FogUpdateFunc + 0x38),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x44),
    (void*) ((u8*) FogUpdateFunc + 0x90),
    (void*) ((u8*) FogUpdateFunc + 0xDC),
    (void*) ((u8*) FogUpdateFunc + 0x128),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x228),
    (void*) ((u8*) FogUpdateFunc + 0x174),
    (void*) ((u8*) FogUpdateFunc + 0x1D0),
};
