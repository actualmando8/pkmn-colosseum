/**
 * @file gs_thread_hi_range_800FEC34.c
 * @brief GSthread (upper half) -- unnamed/unclassified trailing block.
 *
 * Address range: 0x800FEC34 - 0x800FF0A0 (nominally 5 functions per
 * symbols.txt: fn_800FEC34, fn_800FECB8, fn_800FED3C, fn_800FEE68,
 * fn_800FEF8C). None of these are actually implemented anywhere in the
 * original monolithic gs_thread_hi.c -- the file's own internal
 * "Generated" block comment documented real coverage stopping at
 * 0x800FEBA0 (end of gappBackgroundCallback), and grepping the repo
 * shows these five symbols are only ever referenced via extern
 * (game/gs_party_access.c, game/data/data_8027A500.c), never defined.
 * This file is therefore an intentionally-empty CodeCandidate
 * placeholder that preserves splits.txt address-range tiling; per the
 * r3 spec's caveat, this block's own globals (lbl_8047ACB0-ACCC
 * cluster) are self-contained and not shared with the neighbouring
 * GSres/GSmsg/GSgapp/sprite blocks, so it was not matched to any XD TU.
 *
 * Split out of the former monolithic game/gs_thread_hi.c
 * (0x800F8268-0x800FF0A0 per config/GC6E01/splits.txt).
 */
#include "dolphin/types.h"
#include "game/gs_floor.h"

/* 0x800FEC34 | 0x84 */
void fn_800FEC34(u32 callback) {
    extern GSFloorResource* lbl_8047ACB0;
    extern u32 lbl_8047ACC0;
    extern void GSthreadUnblock(void* thread);
    GSFloorResource* resource;
    u32 resourceCallback;
    u32 i;

    resource = lbl_8047ACB0;
    for (i = lbl_8047ACC0; i > 0; i--) {
        if ((s32)resource->active == 3) {
            resourceCallback = (u32)resource->callback;
            if (resourceCallback == callback) {
                if (resource->active) {
                    goto found;
                }
                goto found;
            }
        }
        resource++;
    }
    resource = NULL;

found:
    if (resource != NULL) {
        resource->pending = 0;
        if ((s32)resource->status == 1 && resource->modelHandle != NULL) {
            GSthreadUnblock(resource->modelHandle);
        }
    }
}

/* 0x800FECB8 | 0x84 */
void fn_800FECB8(u32 callback) {
    extern GSFloorResource* lbl_8047ACB0;
    extern u32 lbl_8047ACC0;
    extern void GSthreadBlock(void* thread);
    GSFloorResource* resource;
    u32 i;

    resource = lbl_8047ACB0;
    for (i = lbl_8047ACC0; i > 0; i--) {
        if ((s32)resource->active == 3) {
            if ((u32)resource->callback == callback) {
                if (resource->active) {
                    goto found;
                }
                goto found;
            }
        }
        resource++;
    }
    resource = NULL;

found:
    if (resource != NULL) {
        resource->pending = 1;
        if ((s32)resource->status == 1 && resource->modelHandle != NULL) {
            GSthreadBlock(resource->modelHandle);
        }
    }
}
