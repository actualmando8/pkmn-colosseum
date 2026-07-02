/**
 * @file input.h
 * @brief Low-level PAD/WI input backend (src/game/input/input.c).
 *
 * This header used to declare an invented "PADInput_*" wrapper API
 * (PADInput_Init, PADInput_ReadButtons, PADInput_GetHeld, ...) along
 * with a PADInputState struct and PAD_* constants that duplicated the
 * real Dolphin SDK PAD.h definitions. None of those names appear in
 * config/GC6E01/symbols.txt and nothing outside input.c referenced
 * them, so they were removed along with the fictional definitions in
 * input.c. The real functions in input.c operate directly on a fixed
 * 4-slot pad-entry table (base lbl_80401C10) via raw offsets and do
 * not need any declarations from this header beyond the include guard.
 */

#ifndef GAME_INPUT_INPUT_H
#define GAME_INPUT_INPUT_H

#include "dolphin/types.h"

#endif /* GAME_INPUT_INPUT_H */
