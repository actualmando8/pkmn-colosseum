/**
 * @file pda_range_80037158.c
 * @brief PDA subsystem body, 0x80037158 - 0x8004B7EC.
 *
 * Colosseum's PDA: the People/party-select 3D screen (5 private
 * widget tables), a shared PDA scene/camera block, and the mail-fetch
 * helpers. A reduced/reordered subset of XD's PDA (ReliveHall and
 * PdaSearcher clusters were cut). Internal boundaries are fuzzy --
 * split further only with evidence. All functions asm-only until
 * matched.
 */
#include "dolphin/types.h"
