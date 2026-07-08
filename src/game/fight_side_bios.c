/**
 * @file fight_side_bios.c
 * @brief Fight-side low-level field accessors (BIOS layer).
 *
 * Split out of the former game/pokemon.c CodeCandidate bucket
 * (0x801F000C-0x801F7F80), which was mislabeled "pokemon" but is
 * entirely the XD-era fight-engine cluster. Address range covered by
 * this translation unit: 0x801F7798-0x801F7954 (13 functions), per
 * config/GC6E01/splits.txt.
 */

#include "game/pokemon_fight_types.h"

/* 0x801F7798 | size: 0x24 | small */
void fightSideDataBiosSetFightoutPokemonStatusMenuDataId(u8* ptr, u8 idx, u32 val) {
    if (ptr == NULL) return;
    if (idx >= 2) return;
    ptr += (u32)idx * 4;
    *(u32*)(ptr + 0xC) = val;
}

/* 0x801F77BC | size: 0x24 | small */
void fightSideDataBiosSetFightTrainerStatusMenuDataId(u8* ptr, u8 idx, u32 val) {
    if (ptr == NULL) return;
    if (idx >= 2) return;
    ptr += (u32)idx * 4;
    *(u32*)(ptr + 0x4) = val;
}

/* Address: 0x801F77E0 | Size: 0x10 | Pattern: nullcheck_setter */
void fightSideDataBiosSetYrot(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* 0x801F77F0 | size: 0x34 | small */
u32 fightSideDataBiosGetFightoutPokemonStatusMenuDataId(u8* ptr, u8 idx) {
    if (ptr == NULL) return 0;
    if (idx >= 2) return 0;
    ptr += (u32)idx * 4;
    return *(u32*)(ptr + 0xC);
}

/* 0x801F7824 | size: 0x34 | small */
u32 fightSideDataBiosGetFightTrainerStatusMenuDataId(u8* ptr, u8 idx) {
    if (ptr == NULL) return 0;
    if (idx >= 2) return 0;
    ptr += (u32)idx * 4;
    return *(u32*)(ptr + 0x4);
}

/* Address: 0x801F7858 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightSideDataBiosGetYrot(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* 0x801F7870 | size: 0x2C | small */
u8* fightSideDataBiosGetPtr(u16 idx) {
    extern u32 lbl_80478F38;
    extern u32 lbl_80478F3C;
    u32 count = *(u32*)lbl_80478F38;
    if ((u16)idx >= count) return NULL;
    return (u8*)(lbl_80478F3C + (u16)idx * 0x14);
}

/* Address: 0x801F789C | Size: 0x10 | Pattern: nullcheck_setter */
void fightSideBiosSetMakibisiCheckFlag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x522C]) = val;
}

/* Address: 0x801F78AC | Size: 0x10 | Pattern: nullcheck_setter */
void fightSideBiosSetFightSideDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x801F78BC | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightSideBiosGetMakibisiCheckFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x522C]);
}

/* 0x801F78D4 | size: 0x34 | small */
u8* fightSideBiosGetFightTrainerPtr(u8* base, u16 idx) {
    if (base == NULL) return NULL;
    if ((u16)idx >= 2) return NULL;
    return base + (u16)idx * 0x28e4 + 0x64;
}

/* 0x801F7908 | size: 0x34 | small */
u8* fightSideBiosGetJoutaiPtr(u8* base, u16 idx) {
    u8* r5;
    r5 = base;
    if (r5 == NULL) return NULL;
    if (idx >= 6) return NULL;
    return r5 + (u32)idx * 16 + 4;
}

/* Address: 0x801F793C | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightSideBiosGetFightSideDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}
