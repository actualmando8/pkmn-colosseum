/**
 * @file snd_synthapi.c
 * @brief MusyX runtime synth API (musyx/runtime/snd_synthapi.c),
 * 0x8014D75C - 0x8014DDD8.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/snd_synthapi.c`. Boundary evidence:
 * fn_8014DCA8 is synthDeactivateStudio (simindex seq=1.0 vs the MP4
 * matched copy); the two 0x20 wrappers fn_8014DD98/fn_8014DDB8 are
 * snd_synthapi tail one-call wrappers (stream.c has no functions before
 * streamInit, which is confirmed at 0x8014DDD8 at seq=1.0).
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"
