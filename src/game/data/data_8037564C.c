#include "dolphin/types.h"

#pragma section ".data"

extern void fightSideGetStatus(void);

/*
 * Compiler-generated switch jump table for pokemon.c: fightSideGetStatus slotType switch dispatch (9 case handlers).
 * Entries are code-label addresses within the owning function, expressed
 * as (function symbol + byte offset) so MWCC emits the same R_PPC_ADDR32
 * relocations (with addend) as the retail table instead of dead literals.
 */
void* jumptable_8037564C[9] = {
    (void*) ((u8*) fightSideGetStatus + 0xC8),
    (void*) ((u8*) fightSideGetStatus + 0x74),
    (void*) ((u8*) fightSideGetStatus + 0x80),
    (void*) ((u8*) fightSideGetStatus + 0x8C),
    (void*) ((u8*) fightSideGetStatus + 0xC8),
    (void*) ((u8*) fightSideGetStatus + 0x98),
    (void*) ((u8*) fightSideGetStatus + 0xA4),
    (void*) ((u8*) fightSideGetStatus + 0xB0),
    (void*) ((u8*) fightSideGetStatus + 0xBC),
};
