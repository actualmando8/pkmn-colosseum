/**
 * @file menu_pda_mail.c
 * @brief PDA Mailbox reader UI, 0x8004B7EC - 0x8004EADC.
 *
 * XD-anchor-backed identity: menuPdaOpen (0x34) and pdaMailGetMailID
 * (0x50) byte-size-match XD's identically-named functions exactly;
 * calls mailGetMailIDInMailbox/mailGetReceiveNumber; owns 9 private
 * widget tables plus the 56-entry mail-list layout table. Positional
 * porting from XD's menuPdaMail* family failed (reordered subset) --
 * further naming needs byte-level comparison. All functions asm-only
 * until matched.
 */
#include "dolphin/types.h"
