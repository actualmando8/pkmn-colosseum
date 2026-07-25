/**
 * @file hsd_range_801A69C0.c
 * @brief hsd code, 0x801A69C0 - 0x801A8428 (23 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct HSD_AmnesiaClass {
    u8 _00[0x14];
    struct HSD_AmnesiaClass* parent; /* 0x14 */
    u8 _18[0x20];
    void (*amnesia)(void* info);     /* 0x38 */
} HSD_AmnesiaClass;

extern u8 lbl_8036CB30[]; /* hsdMObj class info */
extern void* lbl_8047B2D0;
extern void* lbl_8047B2D8;
extern void* lbl_8047B2DC;

void MObjAmnesia(void* info)
{
    if (info == lbl_8047B2D0) {
        lbl_8047B2D0 = 0;
    }
    if (info == (void*)lbl_8036CB30) {
        lbl_8047B2D8 = 0;
        lbl_8047B2DC = 0;
    }
    ((HSD_AmnesiaClass*)lbl_8036CB30)->parent->amnesia(info);
}
