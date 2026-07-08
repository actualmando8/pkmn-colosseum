/**
 * @file menuCB_BattleStart.c
 * @brief menuCB_BattleStart.cpp, 0x8005DFC8 - 0x80062948.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. XD has __sinit_menuCB_BattleStart_cpp at
 * 0x800477F4; includes menuCBBattleStart* + menuCB_BattleResult* locals
 * (XD 0x80046594-0x800477F8). SMOKING GUN: fn_8005DFC8 takes the address of
 * local symbol _menuCBBattleStartDispTrainerTexCallBack__FlPvl (0x800626CC),
 * proving same-TU membership. All functions asm-only.
 */
#include "dolphin/types.h"
