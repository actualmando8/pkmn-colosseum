/**
 * @file seq_api.c
 * @brief MusyX runtime sequencer API wrappers (musyx/runtime/seq_api.c),
 * 0x8014D598 - 0x8014D75C.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/seq_api.c`. Boundary evidence: fn_8014D598
 * is sndSeqGetValid (calls seqGetPrivateId, returns id != -1); fn_8014D740
 * is seqGetMIDIPriority (simindex seq=1.0 vs MP4/Prime matched copies),
 * ending at sndFXCtrl (0x8014D75C), snd_synthapi.c's territory.
 * All functions asm-only until matched.
 */
#include "dolphin/types.h"
