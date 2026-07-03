#include "dolphin/types.h"

#pragma section ".data"

extern void inpTranslateExCtrl(void);

/*
 * Compiler-generated switch jump table for musyx_range_80157280.c: inpTranslateExCtrl switch dispatch (9 case handlers).
 * Entries are code-label addresses within the owning function, expressed
 * as (function symbol + byte offset) so MWCC emits the same R_PPC_ADDR32
 * relocations (with addend) as the retail table instead of dead literals.
 */
void* jumptable_80369CB0[9] = {
    (void*) ((u8*) inpTranslateExCtrl + 0x28),
    (void*) ((u8*) inpTranslateExCtrl + 0x30),
    (void*) ((u8*) inpTranslateExCtrl + 0x38),
    (void*) ((u8*) inpTranslateExCtrl + 0x40),
    (void*) ((u8*) inpTranslateExCtrl + 0x48),
    (void*) ((u8*) inpTranslateExCtrl + 0x50),
    (void*) ((u8*) inpTranslateExCtrl + 0x58),
    (void*) ((u8*) inpTranslateExCtrl + 0x60),
    (void*) ((u8*) inpTranslateExCtrl + 0x68),
};
