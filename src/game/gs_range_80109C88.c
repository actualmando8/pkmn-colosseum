/**
 * @file gs_range_80109C88.c
 * @brief gs-engine code, 0x80109C88 - 0x8010C364 (19 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

extern u8 pokemonCheckValid(void* pokemon);
extern u8 pokemonBiosGetTamagoFlag(void* pokemon);
extern u16 pokemonGetStatus(void* pokemon, u32 index, u32 field, u32 rare);
extern u32 pokemonBiosGetRnd(void* pokemon);
extern u8 pokemonGetAnnonKatati(u32 personality);
extern u8 pokemonCheckRare(void* pokemon);
extern u16 lbl_8035B478[][2];

u16 fn_8010BBB8(void* pokemon)
{
    u16 species;
    u8 form;

    if (pokemon == NULL) {
        return 0;
    }
    if (!pokemonCheckValid(pokemon) || pokemonBiosGetTamagoFlag(pokemon)) {
        return 0x33D;
    }

    species = pokemonGetStatus(pokemon, 0, 0x6E, 0);
    if (species == 0xC9) {
        form = pokemonGetAnnonKatati(pokemonBiosGetRnd(pokemon));
        return lbl_8035B478[form][pokemonCheckRare(pokemon) != 0];
    }
    return pokemonGetStatus(NULL, species, 0x5B,
                            pokemonCheckRare(pokemon) != 0);
}

u32 fn_8010B560(void) {
    typedef struct Entry {
        void* data;
        u8 padding[2];
        u8 state;
        u8 padding2[9];
    } Entry;
    extern s32 lbl_8047AD48;
    extern Entry* lbl_8047AD4C;
    Entry* entry;
    s32 result;
    s32 i;

    result = 0;
    entry = lbl_8047AD4C;
    for (; result < lbl_8047AD48; entry++, result++) {
        if (entry->data == NULL) {
            break;
        }
    }

    entry = lbl_8047AD4C;
    for (i = 0; i < result; i++, entry++) {
        if (entry->state == 1) {
            result = 1;
            goto done;
        }
    }

    result = 0;
done:
    return result;
}

s8 fn_8010BCE4(void) {
    typedef struct Entry {
        void* data;
        u8 padding[2];
        u8 state;
        u8 padding2[9];
    } Entry;
    extern void* _menuFaceBiosGetPtr__FUs(void);
    extern s32 lbl_8047AD48;
    extern Entry* lbl_8047AD4C;
    void* data;
    Entry* entry;
    s32 i;

    data = _menuFaceBiosGetPtr__FUs();
    entry = lbl_8047AD4C;
    for (i = 0; i < lbl_8047AD48; entry++, i++) {
        if (data == entry->data) {
            goto found;
        }
    }
    i = -1;
found:
    if (i < 0) {
        return -1;
    }
    return lbl_8047AD4C[i].state == 2;
}

void fn_8010C220(void) {
}
