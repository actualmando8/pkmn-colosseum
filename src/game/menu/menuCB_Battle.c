/**
 * @file menuCB_Battle.c
 * @brief menuCB_Battle.cpp, 0x80069A60 - 0x80069C0C.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments; this final range keeps the original bucket
 * name since it is the only segment with direct filename proof. XD:
 * menuCB_Battle / menuCB_InitBattle / _menuCBBattle_DeleteAllItem /
 * _menuCB_Flash* locals (XD 0x8004D394-0x8004DE74).
 *
 * DIRECT filename proof: fn_80069A60 calls __assert with file string
 * 'menuCB_Battle.c' (.data 0x80267C94) and conds
 * 'FIGHT_ENCOUNT_DATA_null != nFightEncountID',
 * '_LENGTH(staColosseum)>p->m_eColosseum'; calls
 * fightEncountDataBiosGetPtr/SetFightTrainerDataId = battle-encounter setup.
 * The function is asm-only (not yet matched as C).
 */
#include "dolphin/types.h"
