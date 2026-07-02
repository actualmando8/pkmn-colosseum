/**
 * @file fight_range_80211A00.c
 * @brief Fight/battle-AI support layer, 0x80211A00 - 0x802405C0 (470 fns).
 *
 * Split out of the previously-unassigned auto block (audit 2026-07-01,
 * docs/fable5_audit_pass3_fight_engine.md). Contains fightSeqGetItemType,
 * fightTrainerAiGetValueAryMaxBanme, and the callee layer used by
 * colosseum_battle.c's WazaHit family. Range name kept honest until the
 * module's internal structure is proven.
 */
#include "dolphin/types.h"
