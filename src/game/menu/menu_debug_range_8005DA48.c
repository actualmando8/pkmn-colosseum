/**
 * @file menu_debug_range_8005DA48.c
 * @brief debug menu TU, 0x8005DA48 - 0x8005DFC8.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. dbgMenuLog-family / dbgMenuFieldCamera-family /
 * menuDbgItem-family cluster in XD's dbg menu source (XD 0x8000DEE8-0x8000E53C). 2 XD anchors
 * (dbgMenuFieldCameraChangeDisp, menuDbgItemCreate), monotonic with exact
 * size matches. All functions asm-only.
 */
#include "dolphin/types.h"
