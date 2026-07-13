#ifndef GAME_DATA_RODATA_80267398_H
#define GAME_DATA_RODATA_80267398_H

#include "dolphin/types.h"

/*
 * String block layout:
 *   +0x00 "MSG_ID_dummy != msg"
 *   +0x14 "BATTLETYPE_MULTI != _CB.m_eBattleType"
 *   +0x3C "!_menuCB_Temp"
 *   +0x4C "_menuCB_Temp"
 */
typedef u8 ColosseumBattleAssertStringBlock[96];

extern const ColosseumBattleAssertStringBlock lbl_80267A20;

#endif /* GAME_DATA_RODATA_80267398_H */
