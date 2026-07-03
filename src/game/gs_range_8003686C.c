/**
 * @file gs_range_8003686C.c
 * @brief Post-movie boot fragment, 0x8003686C - 0x80037158.
 *
 * Called from movie.c right after menuOpen(0x85, ...); spawns
 * _menuSoundReadWaveThread and runs filesystem init (_fsysInitTOC).
 * Identified during the PDA decomposition (2026-07-03); the range
 * name stays honest until the TU identity is proven.
 */
#include "dolphin/types.h"
