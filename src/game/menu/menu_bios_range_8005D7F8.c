/**
 * @file menu_bios_range_8005D7F8.c
 * @brief menu core bios accessor farm, 0x8005D7F8 - 0x8005DA48.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. menuSeBios/menuSeqBios/menuSpriteBios/
 * menuItemBios/menuDataBios accessor farm; sits inside XD's menu-core TU
 * next to menuPanel-family / menuTool_imasugu-family (XD 0x8007C958-0x8007CD64). 8 XD
 * anchors, all monotonic. All functions asm-only.
 */
#include "dolphin/types.h"
