/**
 * @file stream.c
 * @brief MusyX runtime streaming (musyx/runtime/stream.c),
 * 0x8014DDD8 - 0x80150C78.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/stream.c`. Boundary evidence: streamInit
 * confirmed at 0x8014DDD8 (simindex seq=1.0 vs MP4/Prime matched copies);
 * sndStreamDeactivate (0x80150564 + 0x714) is reference stream.c's last
 * function and ends exactly at dataInsertKeymap (0x80150C78), synthdata.c's
 * first. All functions asm-only until matched.
 */
#include "dolphin/types.h"
