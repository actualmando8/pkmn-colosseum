/**
 * @file menuCB_range_8005344C.c
 * @brief colosseum-battle Pokemon-select-from-PC screens, 0x8005344C - 0x80055E38.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. PCBOX-centric (getPokemon/setPokemon/delPokemon,
 * pcboxGetNbPokemonBox), menuModelCheck/Render, itemDataBiosGetName,
 * winSpriteSetDisp disp-subs + windowGetKeyInfo ctrl fns. Identity SPECULATIVE
 * (0 XD anchors; structural-family evidence only). All functions asm-only.
 */
#include "dolphin/types.h"
