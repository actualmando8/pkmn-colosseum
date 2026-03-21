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

/* 0x8003AD6C | 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8003AD6C(void) {
    extern u8 lbl_80267140[];
    extern void fn_80109220();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r5 = (u32)lbl_80267140;
    r9 = r1 + 0x8;
    r8 = (u32)lbl_80267140;
    r11 = 0x0;
    r7 = *(u32*)((u8*)r8 + 0x0);
    r10 = 0x0;
    r6 = *(u32*)((u8*)r8 + 0x4);
    r5 = *(u32*)((u8*)r8 + 0x8);
    r0 = *(u32*)((u8*)r8 + 0xC);
    *(u32*)(sp + 0x14) = r0;
    r6 = r9;
    r7 = 0x0;
    goto L_8003ADD4;
L_8003ADB8: ;
    r5 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r5 != (s32)r0) goto L_8003ADCC;
    r11 = 0x1;
L_8003ADCC: ;
    r6 = r6 + 0x4;
    r7 = r7 + 0x1;
L_8003ADD4: ;
    if ((s32)r7 >= (s32)0x2) goto L_8003ADE4;
    if ((s32)r11 == (s32)0x0) goto L_8003ADB8;
L_8003ADE4: ;
    if ((s32)r11 != (s32)0x0) goto L_8003AE38;
    r9 = r9 + 0x8;
    r10 = 0x1;
    r6 = r9;
    r7 = 0x0;
    goto L_8003AE1C;
L_8003AE00: ;
    r5 = *(s16*)((u8*)r4 + 0x6);
    r0 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)r5 != (s32)r0) goto L_8003AE14;
    r11 = 0x1;
L_8003AE14: ;
    r6 = r6 + 0x4;
    r7 = r7 + 0x1;
L_8003AE1C: ;
    if ((s32)r7 >= (s32)0x2) goto L_8003AE2C;
    if ((s32)r11 == (s32)0x0) goto L_8003AE00;
L_8003AE2C: ;
    if ((s32)r11 != (s32)0x0) goto L_8003AE38;
    r10 = 0x2;
L_8003AE38: ;
    if ((s32)r11 != (s32)0x0) goto L_8003AE48;
    r3 = 0x0;
    goto L_8003AE74;
L_8003AE48: ;
    r0 = *(u8*)((u8*)r3 + 0x95);
    r3 = r4;
    r0 = (s8)r0;
    if ((s32)r10 != (s32)r0) goto L_8003AE64;
    r0 = 0x1;
    goto L_8003AE68;
L_8003AE64: ;
    r0 = 0x0;
L_8003AE68: ;
    r4 = r0 & 0xFF;
    fn_80109220();
    r3 = 0x0;
L_8003AE74: ;
    return;
}
#pragma pop
