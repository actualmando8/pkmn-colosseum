/**
 * @file item_range_80144574.c
 * @brief item-use game code, 0x80144574 - 0x8014635C (1 fn).
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). fn_80144574
 * (size 0x1DE8) is item-use-on-Pokemon logic (calls hpRecover__FP20...);
 * it ends exactly at seqGetPrivateId (0x8014635C), the first MusyX seq.c
 * function. Asm-only until matched; the range name stays honest until the
 * function is decompiled.
 */
#include "dolphin/types.h"
