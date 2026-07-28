/**
 * @file field_range_80089048.c
 * @brief field code, 0x80089048 - 0x800896B8 (3 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/* fn_800895A4 - 0x800895A4 | size: 0x114 */
void fn_800895A4(u8* hero, u8* source) {
    extern void fn_8008BBDC(void*, u8*);
    extern void heroBiosSetHomePlace(u8*, u8);
    extern void heroBiosSetSexDataId(u8*, u8);
    extern void heroBiosSetRnd(u8*, u32);
    extern void heroBiosSetNamePtr(u8*, void*);
    extern void* heroBiosGetPokemonPtr(u8*, u16);
    extern u32 fn_80135938(s32, s32);
    extern u32 fn_800F9C04(void*, u8*, u32, u32);
    extern void fn_8011D494(void*, u16);
    extern void exribbonSetNo(s32, u8);
    u8 name[0x28];
    u32 value;
    u32 i;
    void* pokemon;

    heroBiosSetHomePlace(hero, (source[0] & 4) != 0 ? 2 : 1);
    fn_800F9C04(name, source + 4, 7, fn_80135938(0, 5));
    heroBiosSetNamePtr(hero, name);
    heroBiosSetSexDataId(hero, source[0xC]);

    value = *(u32*)(source + 0x10);
    value = (value << 24) | ((value & 0xFF00) << 8)
          | ((value & 0xFF0000) >> 8) | (value >> 24);
    heroBiosSetRnd(hero, value);

    for (i = 0; i < 6; i++) {
        pokemon = heroBiosGetPokemonPtr(hero, (u16)i);
        fn_8008BBDC(pokemon, source + 0x14 + i * 0x64);
        fn_8011D494(pokemon, (u16)i);
    }
    for (i = 0; i < 11; i++) {
        exribbonSetNo(i, source[0x26C + i]);
    }
}
