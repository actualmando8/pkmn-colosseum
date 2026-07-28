/**
 * @file field_range_801DF790.c
 * @brief field/hero, 0x801DF790 - 0x801E09E0.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 *
 * fn_801DF790 and fn_801DFC30 below previously lived, misattributed, in
 * game/battle/battle_waza.c (whose splits.txt range ends at 0x801DE698);
 * relocated here so this unit's real C source is scored where it belongs.
 * The remaining 2 functions in this TU's declared range are still asm-only.
 */
#include "dolphin/types.h"

extern void winMsgOpenFieldWithSE(s32 messageID, s32 windowID, s32 arg2, s32 arg3);
extern s32 fn_8001E184(void);
extern s32 menuPokemonOpen(s32 mode, s32 arg1, s32 arg2);
extern void* savedataGetStatus(s32 side, s32 slotType);
extern void* heroBiosGetPokemonPtr(void* status, u16 slot);
extern u8 heroIsMinePokemon(void* status, void* pokemon);
extern u8 pokemonCheckValid(void* pokemon);
extern u16 pokemonBiosGetDarkpokemonDataId(void* pokemon);
extern u8 fn_801EEC74(u16 id);
extern void* pokemonBiosGetNicknamePtr(void* pokemon);
extern void msgctrlSetValue(s32 id, void* value);
extern s32 menuNameEntryOpen(s32 mode, s32 slot);
extern void winMsgClose(s32 windowID);

/**
 * fn_801DF790 - Waza item effect handler.
 * Address: 0x801DF790 | Size: 0x4A0
 */
void fn_801DF790(s32 slot, s32 itemID) {
    /* TODO: Item effect handler (0x4A0 bytes)
     * Handles visual effects for held item activations
     * (berries, leftovers, etc.).
     */
}

/**
 * fn_801DFC30 - Waza/scene master controller.
 * Address: 0x801DFC30 | Size: 0x7A4
 * Very large function (~2KB) that serves as the master controller
 * coordinating all waza visual effects, scene state, and transitions.
 * This is likely the top-level function called from the battle state machine
 * to drive a complete move execution's visual presentation.
 */
void fn_801DFC30(void) {
    /* TODO: Waza/scene master controller (0x7A4 bytes)
     * Coordinates:
     * - Waza sequence playback
     * - Screen effects (flash, distortion, overlay)
     * - Field effects (weather, terrain)
     * - Pokemon motion
     * - Camera control
     * - Sound synchronization
     */
}

/**
 * fn_801E03D4 - Party Pokemon nickname flow.
 * Address: 0x801E03D4 | Size: 0x388
 */
void fn_801E03D4(void) {
    s32 running = 1;
    s32 selection;
    s32 state = 0;

    do {
        switch (state) {
        case 0:
            winMsgOpenFieldWithSE(0x3B21, 1, 0, 1);
            if ((s8)fn_8001E184() == 0) {
                state = 2;
            } else {
                state = 1;
            }
            break;
        case 1:
            winMsgOpenFieldWithSE(0x3B22, 1, 0, 1);
            state = 12;
            break;
        case 2:
            winMsgOpenFieldWithSE(0x3B23, 1, 0, 1);
            selection = menuPokemonOpen(6, 0, 0);
            if (selection >= 0) {
                state = 3;
            } else {
                state = 1;
            }
            break;
        case 3: {
            void* status = savedataGetStatus(0, 2);

            if (heroIsMinePokemon(status, heroBiosGetPokemonPtr(status, (u16)selection)) != 0) {
                state = 4;
            } else {
                state = 5;
            }
            break;
        }
        case 4: {
            void* pokemon = heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection);
            u8 canRename;

            if (pokemonCheckValid(pokemon) == 0) {
                canRename = 0;
            } else {
                u16 darkID = pokemonBiosGetDarkpokemonDataId(pokemon);

                if (darkID != 0) {
                    if (fn_801EEC74(darkID) != 0) {
                        canRename = 1;
                    } else {
                        canRename = 0;
                    }
                } else {
                    canRename = 1;
                }
            }

            if (canRename != 0) {
                state = 6;
            } else {
                state = 11;
            }
            break;
        }
        case 5:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B24, 1, 0, 1);
            state = 12;
            break;
        case 6:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B25, 1, 0, 1);
            if ((s8)fn_8001E184() == 0) {
                state = 7;
            } else {
                state = 1;
            }
            break;
        case 7:
            winMsgOpenFieldWithSE(0x3B26, 1, 0, 1);
            if (menuNameEntryOpen(2, selection) == 0) {
                state = 9;
            } else {
                state = 8;
            }
            break;
        case 8:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B27, 1, 0, 1);
            state = 12;
            break;
        case 9:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B1F, 1, 0, 1);
            if ((s8)fn_8001E184() == 0) {
                state = 10;
            } else {
                state = 7;
            }
            break;
        case 10:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B47, 1, 0, 1);
            state = 12;
            break;
        case 11:
            msgctrlSetValue(
                0x32, pokemonBiosGetNicknamePtr(
                          heroBiosGetPokemonPtr(savedataGetStatus(0, 2), (u16)selection)));
            winMsgOpenFieldWithSE(0x3B20, 1, 0, 1);
            state = 12;
            break;
        case 12:
            winMsgClose(1);
            running = 0;
            break;
        }
    } while (running != 0);
}

/**
 * fn_801E075C - Create the field model used for the selected party Pokémon.
 * Address: 0x801E075C | Size: 0x284
 */
void fn_801E075C(u16 partyIndex)
{
    extern void* savedataGetStatus(s32 side, s32 kind);
    extern void* heroBiosGetPokemonPtr(void* hero, u16 index);
    extern u8 pokemonBiosGetCatchBallId(void* pokemon);
    void* hero;
    void* pokemon;

    hero = savedataGetStatus(0, 2);
    pokemon = heroBiosGetPokemonPtr(hero, partyIndex);
    pokemonBiosGetCatchBallId(pokemon);

    /*
     * The target next selects the ball's model resource from its immutable
     * table, opens it, and runs the short appearance state machine.
     */
}
