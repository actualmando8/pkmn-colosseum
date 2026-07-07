#include "dolphin/types.h"

#pragma section ".data"

extern void savedataGetStatus(void);

/*
 * Compiler-generated switch jump table for gs_field_world.c: savedataGetStatus switch dispatch (17 case handlers).
 * Entries are code-label addresses within the owning function, expressed
 * as (function symbol + byte offset) so MWCC emits the same R_PPC_ADDR32
 * relocations (with addend) as the retail table instead of dead literals.
 */
void* jumptable_803634A8[17] = {
    (void*) ((u8*) savedataGetStatus + 0x68),
    (void*) ((u8*) savedataGetStatus + 0x6C),
    (void*) ((u8*) savedataGetStatus + 0x74),
    (void*) ((u8*) savedataGetStatus + 0x7C),
    (void*) ((u8*) savedataGetStatus + 0x84),
    (void*) ((u8*) savedataGetStatus + 0x8C),
    (void*) ((u8*) savedataGetStatus + 0x94),
    (void*) ((u8*) savedataGetStatus + 0x9C),
    (void*) ((u8*) savedataGetStatus + 0xA4),
    (void*) ((u8*) savedataGetStatus + 0xAC),
    (void*) ((u8*) savedataGetStatus + 0xB4),
    (void*) ((u8*) savedataGetStatus + 0xBC),
    (void*) ((u8*) savedataGetStatus + 0xC4),
    (void*) ((u8*) savedataGetStatus + 0xCC),
    (void*) ((u8*) savedataGetStatus + 0xD4),
    (void*) ((u8*) savedataGetStatus + 0xDC),
    (void*) ((u8*) savedataGetStatus + 0xE4),
};
