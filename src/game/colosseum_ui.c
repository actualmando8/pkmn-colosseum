/**
 * @file colosseum_ui.c
 * @brief General Colosseum UI utility functions.
 *
 * This module contains UI helper routines that serve as common building blocks
 * for the menu/screen system. Functions here include sprite/graphic positioning,
 * text formatting helpers, and state-machine utilities used by the shop, summary
 * screen, and other UI subsystems.
 *
 * These functions form a transitional region between the scene init code and
 * the shop system, providing generic UI framework capabilities.
 *
 * Key behaviors:
 *   - Contains state machine logic invoked by scene_init (fn_8003A520 calls
 *     fn_80039498 and fn_8003A10C internally)
 *   - fn_8003A10C (0x414 bytes) is a significant UI framework function that
 *     dispatches to fn_8003ACE8 and fn_8003AE84
 *   - fn_8003A7F0 (0x460 bytes) is another large state machine
 *   - No BSS references, suggesting these operate on caller-provided data
 *
 * Address range: 0x80039A50 - 0x8003AEF0 (12 functions)
 */

#include "dolphin/types.h"

/* ===== GS Engine ===== */
extern void  fn_800F0308(void);          /* GSthread yield */
extern void  fn_801026A4(u32 sceneId, u32 a, u32 b, u32 c,
                         u32 d, u32 e, ...);
extern u32   fn_80102568(u32 a, u32 b, u32 c);
extern u32   fn_8010264C(u32 a, u32 b);
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800FB680(u32 a, u32 b, s32 c, u32 d);
extern void  fn_80132A38(u32 effectId, u32 param);

/*
 * Functions in this translation unit (12 total):
 *
 * fn_80039A50  0x034  Small utility
 * fn_80039A84  0x4C0  Large UI state machine
 * fn_80039F44  0x02C  Small accessor
 * fn_80039F70  0x19C  UI helper
 * fn_8003A10C  0x414  UI framework dispatcher (calls fn_8003ACE8, fn_8003AE84)
 * fn_8003A520  0x1A0  UI entry point (calls fn_80039498, fn_8003A10C)
 * fn_8003A6C0  0x130  UI helper
 * fn_8003A7F0  0x460  Large UI state machine
 * fn_8003AC50  0x098  UI accessor
 * fn_8003ACE8  0x19C  UI sub-handler A
 * fn_8003AE84  0x06C  UI sub-handler B
 */

#pragma push
#pragma force_active on

/* 0x80039A50 | size: 0x34 */
asm void fn_80039A50(void) { nofralloc
    #include "asm/GC6E01/nonmatching/colosseum_ui/fn_80039A50.s"
}

/* 0x80039A84 | size: 0x4C0 */
asm void fn_80039A84(void) { nofralloc
    #include "asm/GC6E01/nonmatching/colosseum_ui/fn_80039A84.s"
}

#pragma pop
