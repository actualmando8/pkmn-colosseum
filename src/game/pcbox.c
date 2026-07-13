/**
 * @file pcbox.c
 * @brief Decompiled functions.
 *
 * Address range: 0x8013433C - 0x80135030
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x8013433C | 0xE4 */
#if 0
asm void pcboxSwapItemSlot(void) {
#include "src/game/effect/effect_util_pcboxSwapItemSlot.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 pcboxSwapItemSlot(void* base, s16 idx1, s16 idx2) {
    extern void fn_80140A9C(void*, void*);
    void* b;
    void* b2;
    void* entry1;
    void* entry2;
    s16 i;
    b = base;
    if (b == 0) {
        b = (void*)savedataGetStatus(0, 3);
    }
    i = idx1;
    if (i < 0 || i >= 0xeb) {
        entry1 = 0;
    } else {
        entry1 = (u8*)b + 0x6dec + (s32)i * 4;
    }
    if (entry1 == 0) return 0;
    b2 = base;
    if (base == 0) {
        b2 = (void*)savedataGetStatus(0, 3);
    }
    i = idx2;
    if (i < 0 || i >= 0xeb) {
        entry2 = 0;
    } else {
        entry2 = (u8*)b2 + 0x6dec + (s32)i * 4;
    }
    if (entry2 == 0) return 0;
    fn_80140A9C(entry1, entry2);
    return 1;
}
#pragma scheduling off
#endif


/* 0x80134420 | 0x164 */
#if 0
asm void pcboxGetItemCapacity(void) {
#include "src/game/effect/effect_util_pcboxGetItemCapacity.inc"
}
#else
#pragma optimization_level 4
u16 pcboxGetItemCapacity(void* base, u16 effect_id) {
    extern u8 itemDataBiosGetPtr(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    void* cur;
    void* entry;
    s16 idx;
    u16 val;
    s32 i;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    if (!itemDataBiosGetPtr(effect_id)) return 0;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    if (idx < 0) return 0;
    entry = (idx >= 0 && idx < 0xeb) ? ((u8*)base + 0x6dec + (s32)idx * 4) : 0;
    if (entry == 0) {
        val = 0xFFFF;
    } else if (fn_801429E8(entry)) {
        val = itemGetStatus(entry, 0, 0x1c, 0) & 0xFFFF;
    } else {
        val = 0xFFFF;
    }
    if (val > 0x3e7) val = 0;
    return (u16)(0x3e7 - val);
}
#endif


/* 0x80134584 | 0xF8 */
#if 0
asm void pcboxAddItem(void) {
#include "src/game/effect/effect_util_pcboxAddItem.inc"
}
#else
#pragma scheduling on
u16 pcboxAddItem(void* base, u16 effect_id, u16 r5) {
    extern u8 itemDataBiosGetPtr(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    extern u16 fn_80140ACC(void*, u16, u16, u16, s16, u16, u32);
    void* cur; s16 idx; s32 i;
    if (base == 0) { base = (void*)savedataGetStatus(0, 3); }
    if (r5 == 0) return r5;
    if (!itemDataBiosGetPtr(effect_id)) return r5;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    if (idx < 0) return r5;
    return (u16)fn_80140ACC((u8*)base + 0x6dec, 0xeb, effect_id, r5, idx, 0x3e7, 0);
}
#pragma scheduling off
#endif


/* 0x8013467C | 0xEC */
#if 0
asm void pcboxDelItem(void) {
#include "src/game/effect/effect_util_pcboxDelItem.inc"
}
#else
#pragma scheduling on
u16 pcboxDelItem(void* base, u16 effect_id, u16 r5) {
    extern u8 itemDataBiosGetPtr(u16);
    extern u8 fn_801429E8(void*);
    extern u16 itemGetStatus(void*, u32, u32, u32);
    extern u16 fn_80141308(void*, u16, u16, u16, s16, u16, u32, u32);
    void* cur; s16 idx; s32 i;
    if (base == 0) { base = (void*)savedataGetStatus(0, 3); }
    if (r5 == 0) return r5;
    if (!itemDataBiosGetPtr(effect_id)) return r5;
    cur = (u8*)base;
    for (i = 0; i < 0xeb; i++, cur = (u8*)cur + 4) {
        if (fn_801429E8((u8*)cur + 0x6dec)) {
            if (itemGetStatus((u8*)cur + 0x6dec, 0, 0x1b, 0) == effect_id) break;
        }
    }
    idx = (i < 0xeb) ? (s16)i : -1;
    return (u16)fn_80141308((u8*)base + 0x6dec, 0xeb, effect_id, r5, idx, 0x3e7, 0, 0);
}
#pragma scheduling off
#endif

#if 0
asm void pcboxGetItem(void) {
#include "src/game/effect/effect_util_pcboxGetItem.inc"
}
#else
#pragma optimization_level 4
void* pcboxGetItem(void* base, s16 index) {
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    if ((s16)index >= 0 && (s16)index < 0xeb) {
        return (u8*)base + 0x6dec + (s32)(s16)index * 4;
    }
    return 0;
}
#endif


/* 0x801347D0 | 0x8 | return_const */
u32 pcboxGetNbItemSlot(void) { return 235; }


/* 0x801347D8 | 0x8 | return_const */
u32 fn_801347D8(void) { return 30; }


/* 0x801347E0 | 0x8 | return_const */
u32 pcboxGetNbPokemonBox(void) { return 3; }


/* 0x801347E8 | 0x104 */
#if 0
asm void fn_801347E8(void) {
#include "src/game/effect/effect_util_fn_801347E8.inc"
}
#else
#pragma optimization_level 4
s8 fn_801347E8(void* base, s8 slot) {
    extern u8 pokemonCheckValid(void*);
    u8* cur;
    s8 count;
    s8 i;
    count = 0;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    if (slot < 0 || slot >= 3) {
        count = -1;
    } else {
        cur = (u8*)base + (s32)slot * 0x24a4;
        i = 0;
        while (i < 0x1e) {
            if (pokemonCheckValid(cur + 0x14)) {
                count++;
            }
            cur += 0x138;
            i++;
        }
    }
    if (count < 0) {
        return -1;
    }
    return (s8)(0x1e - count);
}
#endif


/* 0x801348EC | 0xF0 */
#if 0
asm void getPokemonBoxNbUsedSlot__5PCBOXFSc(void) {
#include "src/game/effect/effect_util_getPokemonBoxNbUsedSlot__5PCBOXFSc.inc"
}
#else
#pragma optimization_level 4
s32 getPokemonBoxNbUsedSlot__5PCBOXFSc(void* base, void* src, s8 slot, s8 idx) {
    extern void pokemonAllKaihuku(void*);
    u8* entry;
    s8 s;
    s8 e;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    s = slot;
    e = idx;
    if (s < 0 || s >= 3 || e < 0 || e >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    pokemonAllKaihuku(entry);
    return 1;
}
#endif


/* 0x801349DC | 0xBC */
#if 0
asm void fn_801349DC(void) {
#include "src/game/effect/effect_util_fn_801349DC.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 fn_801349DC(void* base, s8 slot, u16* name) {
    extern void GScharCpy(void*, u16*);
    s32 len;
    u16* p;
    s8 s;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    s = slot;
    if (s < 0 || s >= 3) return 0;
    if (name == 0) return 0;
    p = name;
    len = 0;
    while (*p != 0) {
        p++;
        len++;
    }
    if (len > 8) return 0;
    GScharCpy((u8*)base + (s32)s * 0x24a4, name);
    return 1;
}
#pragma scheduling off
#endif

#if 0
asm void pcboxGetPokemonBoxName(void) {
#include "src/game/effect/effect_util_fn_80134A98.inc"
}
#else
#pragma scheduling on
void* pcboxGetPokemonBoxName(void* base, s8 index) {
    s8 i;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    i = index;
    if (i < 0) goto _ret0;
    i = index;
    if (i < 3) goto _compute;
_ret0:
    return 0;
_compute:
    return (u8*)base + (s32)i * 0x24a4;
}
#pragma scheduling off
#endif


/* 0x80134AF8 | 0xC8 */
#if 0
asm void delPokemon__5PCBOXFScSc(void) {
#include "src/game/effect/effect_util_delPokemon__5PCBOXFScSc.inc"
}
#else
#pragma optimization_level 4
#pragma scheduling on
s32 delPokemon__5PCBOXFScSc(void* base, s8 slot, s8 idx) {
    extern u8 pokemonCheckValid(void*);
    extern void pokemonInit(void*);
    u8* entry;
    s8 s;
    s8 e;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    s = slot;
    if (s < 0 || s >= 3) {
        entry = 0;
    } else {
        e = idx;
        if (e < 0 || e >= 0x1e) {
            entry = 0;
        } else {
            entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
        }
    }
    if (entry == 0) return 0;
    if (!pokemonCheckValid(entry)) return 0;
    pokemonInit(entry);
    return 1;
}
#pragma scheduling off
#endif


/* 0x80134BC0 | 0x250 */
#if 0
asm void pcboxAddPokemon(void) {
#include "src/game/effect/effect_util_fn_80134BC0.inc"
}
#else
#pragma optimization_level 2
s32 pcboxAddPokemon(void* base, void* src, s8 slot) {
    extern u8 pokemonCheckValid(void*);
    extern void pokemonAllKaihuku(void*);
    u8* slotbase;
    u8* entry;
    s8 found_slot;
    s8 found_entry;
    s8 si;
    s8 ei;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    if (slot < -1 || slot >= 3) return 0;
    if (src == 0) return 0;
    found_slot = slot;
    found_entry = -1;
    if (slot == -1) {
        /* Search all slots for first free entry */
        slotbase = (u8*)base;
        for (si = 0; si < 3; si++) {
            for (ei = 0; ei < 0x1e; ei++) {
                entry = (ei >= 0 && ei < 0x1e) ? (slotbase + (s32)ei * 0x138 + 0x14) : 0;
                if (entry != 0 && !pokemonCheckValid(entry)) {
                    found_entry = ei;
                    break;
                }
            }
            if (found_entry >= 0) {
                found_slot = si;
                break;
            }
            slotbase += 0x24a4;
        }
        if (si >= 3) return 0;
    } else {
        /* Search specific slot for first free entry */
        slotbase = (u8*)base + (s32)slot * 0x24a4;
        for (ei = 0; ei < 0x1e; ei++) {
            entry = (ei >= 0 && ei < 0x1e) ? (slotbase + (s32)ei * 0x138 + 0x14) : 0;
            if (entry != 0 && !pokemonCheckValid(entry)) {
                found_entry = ei;
                break;
            }
        }
        if (found_entry < 0) return 0;
    }
    /* Compute entry address */
    if (found_slot < 0 || found_slot >= 3 || found_entry < 0 || found_entry >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)found_slot * 0x24a4 + (s32)found_entry * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    pokemonAllKaihuku(entry);
    return 1;
}
#endif


/* 0x80134E10 | 0xE0 */
#if 0
asm void setPokemon__5PCBOXFP7PokemonScSc(void) {
#include "src/game/effect/effect_util_setPokemon__5PCBOXFP7PokemonScSc.inc"
}
#else
#pragma optimization_level 4
s32 setPokemon__5PCBOXFP7PokemonScSc(void* base, void* src, s8 slot, s8 idx) {
    extern void pokemonAllKaihuku(void*);
    u8* entry;
    s8 s;
    s8 e;
    u32 i;
    u32* dst32;
    u32* src32;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    s = slot;
    e = idx;
    if (s < 0 || s >= 3 || e < 0 || e >= 0x1e) {
        entry = 0;
    } else {
        entry = (u8*)base + (s32)s * 0x24a4 + (s32)e * 0x138 + 0x14;
    }
    if (entry == 0) return 0;
    dst32 = (u32*)entry;
    src32 = (u32*)src;
    for (i = 0; i < 0x27; i++) {
        dst32[0] = src32[0];
        dst32[1] = src32[1];
        dst32 += 2;
        src32 += 2;
    }
    pokemonAllKaihuku(entry);
    return 1;
}
#endif


/* 0x80134EF0 | 0x98 */
#if 0
asm void getPokemon__5PCBOXFScSc(void) {
#include "src/game/effect/effect_util_fn_80134EF0.inc"
}
#else
#pragma scheduling on
void* getPokemon__5PCBOXFScSc(void* base, s8 r4, s8 r5) {
    u8* new_var;
    s8 slot;
    s8 entry;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    slot = r4;
    if (slot < 0 || slot >= 3) {
        new_var = 0;
        goto done;
    }
    entry = r5;
    if (entry < 0 || entry >= 0x1e) {
        new_var = 0;
        goto done;
    }
    new_var = (u8*)base + (s32)slot * 0x24a4 +
              (s32)entry * 0x138 + 0x14;
done:
    return new_var;
}
#pragma scheduling off
#endif


/* 0x80134F88 | 0x9C */
#if 0
asm void pcboxInit(void) {
#include "src/game/effect/effect_util_pcboxInit.inc"
}
#else
#pragma scheduling on
void pcboxInit(void* base) {
    extern void msgctrlSetValue(u32, u32);
    extern void fn_800F96E4(void*, u32, u32);
    extern void pokemonInitAry(void*, u32);
    extern void fn_80142A88(void*, u32);
    u8* cur;
    s32 i;
    if (base == 0) {
        base = (void*)savedataGetStatus(0, 3);
    }
    i = 0;
    cur = (u8*)base;
    do {
        msgctrlSetValue(0x34, i + 1);
        fn_800F96E4(cur, 9, 0x32c9);
        pokemonInitAry(cur + 0x14, 0x1e);
        i++;
        cur += 0x24a4;
    } while (i < 3);
    fn_80142A88((u8*)base + 0x6dec, 0xeb);
}
#pragma scheduling off
#endif


/* 0x80135024 | 0x4 | void_stub */
#if 0
asm void pcboxSetStatus(void) {
#include "src/game/effect/effect_util_fn_80135024.inc"
}
#else
#pragma optimization_level 4
void pcboxSetStatus(void) {
}
#endif


/* 0x80135028 | 0x8 | return_const */
u32 pcboxGetStatus() { return 0; }
