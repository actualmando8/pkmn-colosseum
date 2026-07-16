/**
 * @file sdk_range_800AA288.c
 * @brief dolphin-sdk code, 0x800AA288 - 0x800AA498 (4 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

/* The original VI translation unit owns these timing tables, strings, and
 * the switch table emitted by VIGetTvFormat. */
#pragma push
#pragma force_active on
#include "src/game/data/data_803120E8.c"
#pragma pop

u32 getCurrentFieldEvenOdd(void) {
    typedef struct VITiming {
        u8 _00[0x18];
        u16 nhlines;
        u16 hlw;
    } VITiming;
    extern VITiming* lbl_8047A888;
    volatile u16* hregs;
    u32 vcount;
    u32 previous;
    volatile u16* vregs = (volatile u16*)0xCC002000;
    u32 hcount;

    vcount = *(vregs += 22) & 0x7FF;
    hregs = (volatile u16*)0xCC002000;
    do {
        previous = vcount;
        hcount = hregs[23] & 0x7FF;
        vcount = vregs[0] & 0x7FF;
    } while (previous != vcount);

    if ((vcount - 1) * 2 + (hcount - 1) / lbl_8047A888->hlw <
        lbl_8047A888->nhlines) {
        return 1;
    }
    return 0;
}

u32 fn_800AA2F0(void) {
    typedef struct VITiming {
        u8 _00[0x18];
        u16 nhlines;
        u16 hlw;
    } VITiming;
    extern VITiming* lbl_8047A888;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern volatile u16 lbl_803FC578[];
    volatile u16* hregs;
    u32 vcount;
    u32 previous;
    volatile u16* vregs;
    u32 hcount;
    u32 field;
    BOOL enabled;
    u8 scratch[16];

    enabled = OSDisableInterrupts();
    vregs = (volatile u16*)0xCC002000;
    vcount = *(vregs += 22) & 0x7FF;
    hregs = (volatile u16*)0xCC002000;
    do {
        previous = vcount;
        hcount = hregs[23] & 0x7FF;
        vcount = vregs[0] & 0x7FF;
    } while (previous != vcount);

    if ((vcount - 1) * 2 + (hcount - 1) / lbl_8047A888->hlw <
        lbl_8047A888->nhlines) {
        field = 1;
    } else {
        field = 0;
    }
    OSRestoreInterrupts(enabled);
    return (field ^ 1) ^ (lbl_803FC578[5] & 1);
}

u32 VIGetCurrentLine(void) {
    typedef struct VITiming {
        u8 _00[0x18];
        u16 nhlines;
        u16 hlw;
    } VITiming;
    extern VITiming* lbl_8047A888;
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    VITiming* timing;
    volatile u16* hregs;
    u32 vcount;
    u32 previous;
    volatile u16* vregs;
    u32 hcount;
    u32 halfLine;
    BOOL enabled;

    timing = lbl_8047A888;
    enabled = OSDisableInterrupts();
    vregs = (volatile u16*)0xCC002000;
    vcount = *(vregs += 22) & 0x7FF;
    hregs = (volatile u16*)0xCC002000;
    do {
        previous = vcount;
        hcount = hregs[23] & 0x7FF;
        vcount = vregs[0] & 0x7FF;
    } while (previous != vcount);

    halfLine = (vcount - 1) * 2 + (hcount - 1) / lbl_8047A888->hlw;
    OSRestoreInterrupts(enabled);
    if (halfLine >= timing->nhlines) {
        halfLine -= timing->nhlines;
    }
    return halfLine >> 1;
}

u32 VIGetTvFormat(void) {
    extern BOOL OSDisableInterrupts(void);
    extern BOOL OSRestoreInterrupts(BOOL level);
    extern u32 CurrTvMode_8047A88C;
    BOOL enabled;
    u32 format;

    enabled = OSDisableInterrupts();
    switch (CurrTvMode_8047A88C) {
    case 0:
    case 3:
    case 6:
        format = 0;
        break;
    case 1:
    case 4:
        format = 1;
        break;
    case 2:
    case 5:
        format = CurrTvMode_8047A88C;
        break;
    }
    OSRestoreInterrupts(enabled);
    return format;
}
