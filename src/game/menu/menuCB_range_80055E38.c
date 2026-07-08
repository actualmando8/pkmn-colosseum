/**
 * @file menuCB_range_80055E38.c
 * @brief colosseum-battle team/status display screens, 0x80055E38 - 0x80057B34.
 *
 * Split out of the former game/menu/menuCB_Battle.c bucket (2026-07-07) into
 * true XD source-unit segments. menuSetDisp/winSeq screens, pokemon status
 * drawing via windowDrawSprite2. Identity SPECULATIVE (0 XD anchors;
 * structural-family evidence only: distinct .data pool band and static bss
 * 0x803A9768 shared across this range).
 *
 * fn_80056A78 (0x80056A78): trivial sda_getter, ported from the previous
 * campaign's archive/previous_campaign/src/game/menu/menu_status.c.
 * Remainder of the range is asm-only.
 */
#include "dolphin/types.h"

/* ===== SDA globals ===== */
extern u32 lbl_8047A584;

/* ===== Function implementations ===== */

u32 fn_80056A78(void) {
    return lbl_8047A584;
}
