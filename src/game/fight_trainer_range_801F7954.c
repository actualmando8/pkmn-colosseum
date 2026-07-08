/**
 * @file fight_trainer_range_801F7954.c
 * @brief Head of fightTrainer.cpp: menu ball status and item helpers.
 *
 * Split out of the former game/pokemon.c CodeCandidate bucket
 * (0x801F000C-0x801F7F80), which was mislabeled "pokemon" but is
 * entirely the XD-era fight-engine cluster. Address range covered by
 * this translation unit: 0x801F7954-0x801F7F80 (5 functions), per
 * config/GC6E01/splits.txt.
 */

#include "game/pokemon_fight_types.h"

/* 0x801F7954 | size: 0x21C | large */
void fightTrainerToMenuBallStatus(u8* ptr, u8* arr) {
    extern u16 fn_801EF634(void);
    extern u8* fightTrainerGetStatus(u8*, u32, u16, u32);
    extern u8 heroCheckValid(void);
    extern u8* heroGetStatus(u8*, u32, u16);
    extern u8 pokemonCheckValid(void);
    extern u32 pokemonGetStatus(u8*, u32, u16, u32);
    extern u8 fightPokemonCheckValid(u8*);
    extern u8 fightPokemonCheckFightOut(u8*);
    extern u8* fightPokemonGetPokemonPtr(u8*);
    extern u8 pokemonIsJoutaiNormal(u8*);
    u8 r0;
    u8* r30;
    u8* r27;
    u8* r26;
    u16 r31;
    u16 r25;

    if (ptr == NULL) return;
    if ((u16)fn_801EF634() == 1) { r0 = 0; goto L_check; }
    if ((s32)fightTrainerGetStatus(ptr, 0, 0x43, 0) == 0) { r0 = 0; goto L_check; }
    if (fightTrainerGetStatus(ptr, 0, 0x44, 0) == NULL) { r0 = 0; goto L_check; }
    if (heroCheckValid() == 0) { r0 = 0; goto L_check; }
    r0 = 1;
L_check:
    if ((u8)r0 == 0) return;
    r30 = fightTrainerGetStatus(ptr, 0, 0x44, 0);
    {
        u16 j;
        j = 0;
        while ((u16)j < 6) {
            arr[(u16)j] = 0;
            j = j + 1;
        }
    }
    r31 = 0;
    while ((u16)r31 < 6) {
        r27 = heroGetStatus(r30, 3, (u16)r31);
        r0 = pokemonCheckValid();
        if ((u8)r0 != 0) {
            r25 = 0;
            while ((u16)r25 < 6) {
                r26 = fightTrainerGetStatus(ptr, 0, 0x45, (u32)(u16)r25);
                r0 = fightPokemonCheckValid(r26);
                if ((u8)r0 != 0) {
                    u32 cb = pokemonGetStatus(r26, 0, 0xcb, 0);
                    if (cb != 0 && r27 == (u8*)cb) {
                        break;
                    }
                }
                r25 = r25 + 1;
            }
            if (r26 == NULL) goto L_next;
            if (fightPokemonCheckValid(r26) == 0) goto L_next;
            if ((s32)pokemonGetStatus(r26, 0, 0xd2, 0) == 1) goto L_next;
            if (fightPokemonCheckFightOut(r26) != 0) {
                arr[(u16)r31] = 3;
                goto L_next;
            }
            fightPokemonGetPokemonPtr(r26);
            if (pokemonIsJoutaiNormal(r26) != 0) {
                arr[(u16)r31] = 2;
                goto L_next;
            }
            arr[(u16)r31] = 1;
        }
L_next:
        r31 = r31 + 1;
    }
}

/* 0x801F7B70 | size: 0xE4 | medium */
s32 fn_801F7B70(u8* ptr) {
    extern u8* fightTrainerGetStatus(u8*, u32, u16, u32);
    extern u8* heroItemGetItemKindToItemAryPtr(u8*, u32, u16*, u32, u32, u32);
    extern u8 fn_801429E8(u8*);
    extern u16 itemGetStatus(u8*, u32, u16, u32);
    u16 count;
    u8* r31;
    u8* r30;
    u32 r29;
    u8* p44;
    p44 = fightTrainerGetStatus(ptr, 0, 0x44, 0);
    if (p44 == NULL) return 0;
    r31 = heroItemGetItemKindToItemAryPtr(p44, 1, &count, 0, 0, 0);
    if (r31 == NULL) return 0;
    r29 = 0;
    while ((u16)r29 < count) {
        r30 = r31 + (u16)r29 * 4;
        if (fn_801429E8(r30) != 0) {
            if (itemGetStatus(r30, 0, 0x1b, 0) != 0) {
                if (itemGetStatus(r30, 0, 0x1c, 0) != 0) {
                    return 1;
                }
            }
        }
        r29++;
    }
    return 0;
}

/* 0x801F7C54 | size: 0x20C | large */
void fightTrainerGetTemotiNormalItemDataIdAry(u8* ptr, u16* out, u16 count, u8 mode) {
    extern u8* fightTrainerGetStatus(u8*, u32, u16, u32);
    extern u8* heroItemGetItemKindToItemAryPtr(u8*, u32, u16*, u32, u32, u32);
    extern u8 fn_801429E8(u8*);
    extern u16 itemGetStatus(u8*, u32, u16, u32);
    extern u8 fightOutPokemonCheckFightOut(u8*);
    extern u16 fightOutPokemonGetFightActionUseItemDataId(u8*);
    u16 sp8;
    u16 spc[2];
    u8* r31;
    u8* r29;
    u8* r23;
    u16 r27;
    u16 r5v;
    u16 r30;
    u16 r28;
    u16 r23i;
    u16 r29i;
    u16 r6i;

    {
        u8* p44 = fightTrainerGetStatus(ptr, 0, 0x44, 0);
        if (p44 == NULL) { return; }
        r31 = heroItemGetItemKindToItemAryPtr(p44, 2, &sp8, 0, 0, 0);
    }
    if (r31 == NULL) { return; }
    for (r23i = 0; (u16)r23i < 2; r23i = r23i + 1) {
        spc[r23i] = 0;
    }
    r30 = 0;
    for (r23i = 0; (u16)r23i < 2; r23i = r23i + 1) {
        r29 = fightTrainerGetStatus(ptr, 0, 0x46, (u32)(u16)r23i);
        if (fightOutPokemonCheckFightOut(r29) != 0) {
            r5v = fightOutPokemonGetFightActionUseItemDataId(r29);
            if ((u16)r5v != 0) {
                spc[r30] = r5v;
                r30 = r30 + 1;
            }
        }
    }
    for (r28 = 0; (u16)r28 < (u16)count; r28 = r28 + 1) {
        out[r28] = 0;
    }
    r28 = 0;
    for (r29i = 0; (u16)r29i < sp8; r29i = r29i + 1) {
        r23 = r31 + (u32)(u16)r29i * 4;
        if (fn_801429E8(r23) != 0) {
            r27 = itemGetStatus(r23, 0, 0x1b, 0);
            if ((u16)r27 != 0) {
                r5v = itemGetStatus(r23, 0, 0x1c, 0);
                if ((u16)r5v != 0) {
                    if ((u8)mode == 1) {
                        for (r6i = 0; (u16)r6i < (u16)r30; r6i = r6i + 1) {
                            if (r27 == spc[r6i]) {
                                if (r27 != (u16)r5v) {
                                    r5v = r5v - 1;
                                }
                            }
                        }
                        if ((u16)r5v == 0) goto next_item;
                    }
                    if ((u16)r28 < (u16)count) {
                        out[r28] = r27;
                        r28 = r28 + 1;
                    }
                }
            }
        }
next_item:;
    }
    (void)r28;
}

/* 0x801F7E60 | size: 0x90 | medium */
s32 fightTrainerIsUsedItem(u8* ptr) {
    extern u8* fightTrainerGetStatus(u8*, u32, u16, u32);
    extern u8 fightOutPokemonCheckFightOut(u8*);
    extern u8 fightOutPokemonIsFightActionUseItemKind(u8*, u32, u32, u32);
    u8* r29;
    u8* r31;
    u32 r30;
    r29 = ptr;
    for (r30 = 0; (u16)r30 < 2; r30++) {
        r31 = fightTrainerGetStatus(r29, 0, 0x46, r30);
        if (fightOutPokemonCheckFightOut(r31) != 0) {
            if (fightOutPokemonIsFightActionUseItemKind(r31, 1, 0, 0) == 1) {
                return 1;
            }
        }
    }
    return 0;
}

/* 0x801F7EF0 | size: 0x90 | medium */
s32 fightTrainerIsSelectedItemBall(u8* ptr) {
    extern u8* fightTrainerGetStatus(u8*, u32, u16, u32);
    extern u8 fightOutPokemonCheckFightOut(u8*);
    extern u8 fightOutPokemonIsFightActionUseItemKind(u8*, u32, u32, u32);
    u8* r29;
    u8* r31;
    u32 r30;
    r29 = ptr;
    for (r30 = 0; (u16)r30 < 2; r30++) {
        r31 = fightTrainerGetStatus(r29, 0, 0x46, r30);
        if (fightOutPokemonCheckFightOut(r31) != 0) {
            if (fightOutPokemonIsFightActionUseItemKind(r31, 1, 1, 0) == 1) {
                return 1;
            }
        }
    }
    return 0;
}
