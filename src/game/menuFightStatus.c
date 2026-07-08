/**
 * @file menuFightStatus.c
 * @brief menuFightStatus -- XD translation unit: fight-menu HP/EXP bar
 *        animation state (wait/start-anim pairs for EXP and HP gauges,
 *        active-slot bookkeeping).
 *
 * Address range: 0x80011B4C - 0x80011EA4 (6 functions)
 *
 * Split from game/gs_npc_interact.c (a CodeCandidate bucket spanning two
 * XD-era translation units) into its true source file. See
 * game/menuFight.c for the sibling TU (0x8000DAA8-0x80011B4C). The split
 * point 0x80011B4C is high-confidence: menuFightStatusWaitAnimeEXP/menuFightStatusWaitAnimeEXP
 * and menuFightStatusWaitAnimeHP/menuFightStatusWaitAnimeHP are instruction-identical except
 * for the polled halfword offset, mirroring XD's WaitAnimeEXP/WaitAnimeHP
 * twin pair that opens menuFightStatus.cpp.
 *
 * fn_80011EA4 is physically included at the end of this file even though
 * its address (0x80011EA4) sits at/above this TU's declared end; it belongs
 * to the following range unit (game/gs_range_80011EA4.c), which XD evidence
 * suggests is itself a continuation of menuFightStatus.cpp (DrawParam /
 * DrawDarkPanel / DrawCheckDark / StatusCtrl). It is carried over unmodified
 * from the original monolithic file pending a follow-up merge/rename.
 */

#include "dolphin/types.h"

/* =========================================================================
 * External declarations
 * ========================================================================= */

/* renamed symbols referenced by asm incs (symbolmap port) */
extern u32 _menuFightIsUse__FP16MENU_WAZA_STATUSUs();
extern void* menuSubCalcColor(void*, void*);

/* NPC/People system */
extern void  winSpriteSetDisp(void* npc, s32 direction);  /* Set NPC facing */
extern void* menuItemBiosGetPtr(s16 npcId);                  /* Lookup NPC data by ID */

/* Text/dialog system */
extern void  windowDrawSprite();
extern void  winSeqSetMenu(void* ctx, s32 state);       /* Set dialog state */
extern void  fn_801081F8(void* ctx, s32 msgId, s32 flags); /* Display message */

/* Rendering */
extern u32   GSmsgGetRect();                           /* Get model dimensions */
extern void  fn_800FB680();

/* Map/warp */
extern u8    fn_801F2020(s32 p1, void* warpId, void* outDest);
extern void* fightOutPokemonGetNicknamePtr(void* mapData);              /* Get map name string */
extern void  winMsgOpen(s32 slot, s32 msgId, s32 p3, s32 p4);
extern void  winMsgClose(s32 slot);                   /* Close message box */

/* Input/frame */
extern u8    fn_801F18DC(s32 controller);             /* Check input ready */
extern u8    fn_801F1700(s32 controller);             /* Check button pressed */
extern u8    fightTimerCommandIsOver(void);                       /* Check A button */
extern u16   fn_801EF634(void);                       /* Get input state */
extern void  _threadSwitch(void);                       /* Frame advance */
extern u32   fn_800F7AF0(s32 slot);                   /* Get render flags */
extern u32   fn_800F7BC4(s32 slot);                   /* Get VSync flags */

/* Battle bridge */
extern void  fn_80132A38();                           /* Set battle parameter */
extern void  fn_801F4C14();                           /* Configure map object */
extern u32   windowGetParam();                           /* Get participant data */

/* =========================================================================
 * SDA globals
 * ========================================================================= */

/* NPC interaction state variables are scattered across the SDA region.
 * The exact mapping is determined by the lbz/sth instructions that
 * reference r0+offset or r13+offset addressing modes. */

/* 0x78 | menuFightStatusWaitAnimeEXP | generic */
extern u32 windowSearchID();
extern u8* windowGetFreeWork();
#pragma peephole off
#pragma peephole off
u32 menuFightStatusWaitAnimeEXP(u32 arg1, u8 arg2) {
    u32 r;
    while (1) {
        if (!(r = windowSearchID(arg1))) return 0;
        if (*(s16*)((u8*)windowGetFreeWork(r) + 0xc) == 0) return 0;
        if (arg2 != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
#pragma peephole on
#pragma peephole on

/* 0x80011BC4 | 0xB4 */
extern u8* windowGetAllocPtr();
extern void fn_80166A28(u32 val);
#if 0
asm void menuFightStatusStartAnimEXP(void) {
#include "src/game/gs_npc_interact_fn_80011BC4.inc"
}
#else
#pragma peephole off
#pragma peephole off
#pragma peephole off
void menuFightStatusStartAnimEXP(u32 arg1, u32 target) {
    u32 ptr;
    u32 state;
    u32 data;
    u32 diff;
    s16 score;
    ptr = windowSearchID(arg1);
    if (!ptr) { return; }
    state = (u32)windowGetAllocPtr(ptr);
    data = (u32)windowGetFreeWork(ptr);
    if (target > *(u32*)(state + 0x20)) {
        diff = target - *(u32*)(state + 0x20);
    } else {
        diff = *(u32*)(state + 0x20) - target;
    }
    score = diff * 100 / *(u32*)(state + 0x1c);
    *(s16*)(data + 0xc) = score;
    if (*(s16*)(data + 0xc) < 0xf) { *(s16*)(data + 0xc) = 0xf; }
    *(u32*)(data + 0x8) = *(u32*)(state + 0x20);
    *(u32*)(state + 0x20) = target;
    *(s16*)(data + 0xe) = 0;
    fn_80166A28(0x4d0);
}
#pragma peephole on
#pragma peephole on
#pragma peephole on
#endif

/* 0x78 | menuFightStatusWaitAnimeHP | generic */
#pragma peephole off
#pragma peephole off
u32 menuFightStatusWaitAnimeHP(u32 arg1, u8 arg2) {
    u32 r;
    while (1) {
        if (!(r = windowSearchID(arg1))) return 0;
        if (*(s16*)((u8*)windowGetFreeWork(r) + 0x2) == 0) return 0;
        if (arg2 != 0) {
            _threadSwitch();
        } else {
            return 1;
        }
    }
}
#pragma peephole on
#pragma peephole on

/* 0x80011CF0 | 0xAC */
#if 0
asm void menuFightStatusStartAnimHP(void) {
#include "src/game/gs_npc_interact_fn_80011CF0.inc"
}
#else
#pragma peephole off
void menuFightStatusStartAnimHP(u32 arg1, s16 target) {
    u32 ptr;
    u32 state;
    u32 data;
    s32 diff;

    ptr = windowSearchID(arg1);
    if (ptr == 0) {
        return;
    }
    state = (u32)windowGetAllocPtr(ptr);
    data = (u32)windowGetFreeWork(ptr);
    diff = *(s16*)(state + 0x1a) - (s16)target;
    if (diff < 0) {
        diff = -diff;
    }
    *(s16*)(data + 2) = (s16)((diff * 100) / *(s16*)(state + 0x18));
    if (*(s16*)(data + 2) <= 0) {
        *(s16*)(data + 2) = 1;
    }
    *(s16*)(data + 0) = *(s16*)(state + 0x1a);
    *(s16*)(state + 0x1a) = target;
    *(s16*)(data + 4) = 0;
}
#pragma peephole on
#endif

/* 0x80011D9C | 0xCC - item-kind resolver + dispatch (same switch as fn_800129A8) */
#if 0
asm void menuFightStatusSetActive(void) {
#include "src/game/gs_npc_interact_fn_80011D9C.inc"
}
#else
#pragma peephole off
#pragma peephole off
void menuFightStatusSetActive(s32 id, s32 do_extra) {
    u32 resolved;
    s32 kind;
    kind = 0;
    resolved = windowSearchID(id);
    if (resolved == 0) return;
    switch (id) {
    case 0x45: case 0x46: case 0x49:
        kind = 0x538;
        break;
    case 0x47: case 0x48: case 0x4a:
        kind = 0x540;
        break;
    }
    if (do_extra != 0) {
        fn_80103F74(resolved, kind, 1);
        fn_801081F8((void*)resolved, kind, 0x2d);
    } else {
        fn_80103F74(resolved, kind, 0);
    }
}
#pragma peephole on
#pragma peephole on
#endif

/* 0x80011E68 | 0x3C */
/* Set an NPC's facing direction. */
void menuFightStatusSetHP(u32 npcId, u16 direction) {
    extern void* windowGetAllocPtr(void* obj);
    extern void* windowSearchID(u32 npcId);
    void* npc;

    npc = windowSearchID(npcId);
    if (npc != NULL) {
        u8* obj = (u8*)windowGetAllocPtr(npc);
        *(u16*)(obj + 0x1A) = direction;
    }
}

/* 0x80011EA4 | 0x9B4 -- GSnpc_WarpToLocation continued */
extern void winSpriteGetDisp();
extern void fn_8001DACC();
extern void fn_8010B9E8();
extern void fn_801F54A4();
extern void fn_800FA280();
extern void windowDrawSprite2();
extern void fn_800D88DC();
extern void fn_800D888C();
extern void fn_800D6A00();
extern void fn_800D7820();
extern void fn_800D67BC();
extern void fn_800D61E4();
extern void fn_800D5BA0();
extern void fn_800D6728();
extern void __cvt_fp2unsigned();
extern f64 lbl_8047B730;
extern f32 lbl_8047B720;
extern f64 lbl_8047B738;
extern f32 lbl_8047B718;
extern f32 lbl_8047B724;
extern f32 lbl_8047B71C;
extern f32 lbl_8047B728;
extern f32 lbl_8047B72C;
extern u8 lbl_80314E08[];
#if 0
asm void fn_80011EA4(void) {
#include "src/game/gs_npc_interact_fn_80011EA4.inc"
}
#else
void fn_80011EA4(void) {
    extern u8 lbl_80314E08[];
    extern f32 lbl_8047B718;
    extern f32 lbl_8047B71C;
    extern f32 lbl_8047B720;
    extern f32 lbl_8047B724;
    extern f32 lbl_8047B728;
    extern f32 lbl_8047B72C;
    extern f64 lbl_8047B730;
    extern f64 lbl_8047B738;
    extern void fn_8001DACC();
    extern void __cvt_fp2unsigned();
    extern void fn_800D5BA0();
    extern void fn_800D61E4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800FA280();
    extern void windowGetAllocPtr();
    extern void windowGetFreeWork();
    extern void windowDrawSprite2();
    extern void winSpriteGetDisp();
    extern void fn_8010B9E8();
    extern void fn_801F54A4();
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r27 = r3;
    r28 = r4;
    windowGetAllocPtr();
    tmp = r3;
    r3 = r27;
    r30 = tmp;
    windowGetFreeWork();
    r31 = r3;
    r3 = r27;
    windowGetAllocPtr();
    r6 = *(s16*)((u8*)r28 + 0x6);
    r4 = 0x1;
    r5 = *(u8*)((u8*)r3 + 0x16);
    if ((s32)r6 < 0xa8) {
        if ((s32)r6 < 0x9f) {
            if ((s32)r6 == 0x9a) goto L_80011FC0;
            if ((s32)r6 < 0x9a) {
                if ((s32)r6 < 0x99) {
                    goto L_80011FC0;
                }
                if ((s32)r6 < 0x9d) {
                    goto L_80011F8C;
                }
                if ((s32)r6 != 0xa5) {
                    if ((s32)r6 < 0xa5) {
            }
                }
                if ((s32)r6 < 0xa4) {
                }
                goto L_80011FC0;
            }
            if ((s32)r6 < 0x538) {
                if ((s32)r6 < 0x534) {
                    if ((s32)r6 >= 0xaa) goto L_80011FC0;
                }
                goto L_80011FA8;
            }
            if ((s32)r6 >= 0x536) goto L_80011FA8;
        }

    } else {
        if ((s32)r6 < 0x53e) {
            if ((s32)r6 >= 0x53c) goto L_80011F8C;
            goto L_80011FC0;
        }
        if ((s32)r6 >= 0x540) goto L_80011FC0;
        goto L_80011FA8;
    }
L_80011F8C:
    tmp = *(u8*)((u8*)r3 + 0x29);
    if (tmp != 0) {
        r4 = 0x0;
        goto L_80011FC0;
    }
    r4 = 0x1;
    goto L_80011FC0;
L_80011FA8:
    tmp = *(u8*)((u8*)r3 + 0x29);
    if (tmp != 0) {
        r4 = 0x1;

    } else {
        r4 = 0x0;
    }
L_80011FC0:
do {
    if ((s32)r6 < 0x534) {
        if ((s32)r6 != 0xa4) {
            if ((s32)r6 >= 0xa4) break;
            if ((s32)r6 != 0x99) {
                break;
            }
            if ((s32)r6 < 0x53c) {
                if ((s32)r6 >= 0x538) break;
        }
        }

    } else {
        if ((s32)r6 >= 0x540) break;
    }
    if (r5 == 1) {
        r4 = 0x0;
    }
} while (0);
    r3 = r28;
    ((void(*)(void))winSpriteSetDisp)();
    r3 = r28;
    winSpriteGetDisp();
    tmp = r3 & 0xFF;
    if (tmp == 0) return;
    tmp = *(s16*)((u8*)r28 + 0x6);
    r3 = -0x100;
    r8 = *(u8*)((u8*)r27 + 0x8B);
    r29 = r8 | r3;
    if ((s32)tmp != 0xae) {
        if ((s32)tmp < 0xae) {
            if ((s32)tmp != 0xa3) {
                if ((s32)tmp < 0xa3) {
                    if ((s32)tmp < 0x9f) {
                        if ((s32)tmp == 0x9a) goto L_8001215C;
                        if ((s32)tmp < 0x9a) return;
                        if ((s32)tmp < 0x9d) return;

                    }
                    if ((s32)tmp == 0xa1) goto L_80012170;
                    if ((s32)tmp < 0xa1) return;

                }
                if ((s32)tmp < 0xaa) {
                    if ((s32)tmp == 0xa5) goto L_8001215C;
                    if ((s32)tmp < 0xa5) return;
                    if ((s32)tmp < 0xa8) return;

                }
                if ((s32)tmp == 0xac) goto L_80012170;
                if ((s32)tmp < 0xac) return;

            }
            if ((s32)tmp == 0x53a) goto L_800123A4;
            if ((s32)tmp < 0x53a) {
                if ((s32)tmp != 0x535) {
                    if ((s32)tmp < 0x535) {
                        if ((s32)tmp == 0x533) goto L_800123CC;
                        if ((s32)tmp >= 0x533) return;
                        if ((s32)tmp < 0x532) return;

                    }
                    if ((s32)tmp != 0x537) return;

                }
                if ((s32)tmp == 0x53e) return;
                if ((s32)tmp < 0x53e) {
                    if ((s32)tmp == 0x53c) return;
                }
                if ((s32)tmp >= 0x53c) goto L_80012680;
                goto L_800123CC;
            }
            if ((s32)tmp >= 0x540) return;
            goto L_80012758;

            tmp = *(u8*)((u8*)r30 + 0x29);
            if (tmp != 2) return;
            r5 = *(u16*)((u8*)r31 + 0x6);
            tmp = 0x43300000;
            *(u32*)(sp + 0x8) = tmp;
            r3 = r27;
            f2 = lbl_8047B730;
            r4 = r28;
            f0 = lbl_8047B720;
            f1 = f1 - f2;
            f1 = f1 / f0;
            fn_8001DACC();
            r3 = r28;
            r4 = 0x0;
            ((void(*)(void))winSpriteSetDisp)();
            return;
        L_8001215C:
            r5 = *(u16*)((u8*)r30 + 0x26);
            r3 = r27;
            r4 = r28;
            fn_8010B9E8();
            return;
        L_80012170:
        do {
            r3 = 0x0;
            r4 = 0x0;
            r5 = 0x32;
            r6 = 0x0;
            fn_801F54A4();
            if ((s32)r3 != 0) {
                tmp = *(u8*)((u8*)r30 + 0x16);
                if (tmp != 0) return;
            }
            r3 = *(s16*)((u8*)r31 + 0x2);
            if ((s32)r3 != 0) {
                tmp = *(s16*)((u8*)r31 + 0x4);
                r5 = 0x43300000;
                r6 = *(s16*)((u8*)r31 + 0x0);
                r3 = *(s16*)((u8*)r30 + 0x1A);
                *(u32*)(sp + 0xC) = tmp;
                r3 = r3 - r6;
                f3 = lbl_8047B738;
                f1 = f0 - f3;
                f0 = f0 - f3;
                f2 = f1 / f0;
                *(u32*)(sp + 0x24) = tmp;
                f1 = f1 - f3;
                f0 = f0 - f3;
                f1 = f2 * f1 + f0;
                f0 = (f64)(s32)f1;
                tmp = (s16)r27;
                if ((s32)tmp != 0) break;
                f0 = lbl_8047B718;
                if (f1 <= f0) break;
                r27 = 0x1;
                break;
            }
            r27 = *(s16*)((u8*)r30 + 0x1A);
        } while (0);
            r5 = r29;
            r3 = 0x20;
            r4 = -0x2;
            r6 = 0x195;
            ((void(*)(void))fn_800FB680)();
            r4 = (s16)r27;
            r3 = 0x34;
            ((void(*)(void))fn_80132A38)();
            r3 = 0xcb;
            ((void(*)(void))GSmsgGetRect)();
            tmp = (u32)r3 >> 16;
            r5 = r29;
            tmp = (s16)tmp;
            r4 = -0x1;
            tmp = 0x18 - tmp;
            r6 = 0xcb;
            r3 = (s16)tmp;
            ((void(*)(void))fn_800FB680)();
            r4 = *(s16*)((u8*)r30 + 0x18);
            r3 = 0x34;
            ((void(*)(void))fn_80132A38)();
            r3 = 0xcb;
            ((void(*)(void))GSmsgGetRect)();
            r3 = (u32)r3 >> 16;
            tmp = *(s16*)((u8*)r28 + 0x54);
            r3 = (s16)r3;
            r5 = r29;
            tmp = tmp - r3;
            r4 = -0x1;
            r3 = (s16)tmp;
            r6 = 0xcb;
            ((void(*)(void))fn_800FB680)();
            return;

            r4 = *(u8*)((u8*)r30 + 0x17);
            r3 = 0x34;
            ((void(*)(void))fn_80132A38)();
            r3 = 0xcb;
            ((void(*)(void))GSmsgGetRect)();
            r3 = (u32)r3 >> 16;
            tmp = *(s16*)((u8*)r28 + 0x54);
            r3 = (s16)r3;
            r5 = *(u8*)((u8*)r27 + 0x8B);
            r3 = tmp - r3;
            tmp = -0x100;
            r3 = (s16)r3;
            r4 = -0x1;
            r5 = r5 | tmp;
            r6 = 0xcb;
            ((void(*)(void))fn_800FB680)();
            return;
        }
            }
    r4 = r30;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r5 = r29;
    r3 = 0x0;
    r4 = -0x1;
    r6 = 0xe9;
    ((void(*)(void))fn_800FB680)();
    r3 = 0xe9;
    ((void(*)(void))GSmsgGetRect)();
    tmp = *(u8*)((u8*)r30 + 0x28);
    r3 = (u32)r3 >> 16;
    r27 = (s16)r3;
    do {
        if ((s32)tmp != 1) {
            if ((s32)tmp < 1) {
                if ((s32)tmp < 0) {
                    goto L_80012370;
                }
                goto L_80012370;
                }
            r3 = 0xd67;
            break;
        }
        r3 = 0xd68;
        break;
    L_80012370:
        r3 = 0x0;
    } while (0);

    if (r3 == 0) return;
    fn_800FA280();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    r3 = r27;
    r5 = r29;
    r4 = -0x1;
    r6 = 0xd0;
    ((void(*)(void))fn_800FB680)();
    return;
L_800123A4:
    r9 = *(u16*)((u8*)r30 + 0x24);
    r7 = r29;
    r5 = *(s16*)((u8*)r28 + 0x54);
    r8 = r27;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    windowDrawSprite2();
    return;
L_800123CC:
    r6 = *(s16*)((u8*)r31 + 0x2);
    if ((s32)r6 != 0) {
        tmp = *(s16*)((u8*)r31 + 0x4);
        r5 = 0x43300000;
        r7 = *(s16*)((u8*)r31 + 0x0);
        r3 = *(s16*)((u8*)r30 + 0x1A);
        *(u32*)(sp + 0x2C) = tmp;
        r3 = r3 - r7;
        f3 = lbl_8047B738;
        f1 = f0 - f3;
        f0 = f0 - f3;
        f2 = f1 / f0;
        *(u32*)(sp + 0x14) = tmp;
        f1 = f1 - f3;
        f0 = f0 - f3;
        f31 = f2 * f1 + f0;
    } else {

        r3 = *(s16*)((u8*)r30 + 0x1A);
        tmp = 0x43300000;
        *(u32*)(sp + 0x8) = tmp;
        f1 = lbl_8047B738;
        *(u32*)(sp + 0xC) = tmp;
        f31 = f0 - f1;
    }
    tmp = *(s16*)((u8*)r30 + 0x18);
    r3 = 0x43300000;
    f3 = lbl_8047B738;
    *(u32*)(sp + 0x34) = tmp;
    f30 = f0 - f3;
    if ((s32)r6 != 0) {
        r4 = *(s16*)((u8*)r31 + 0x0);
        do {
            if ((s32)r4 <= 0) {
                r31 = 0x0;
                break;
            }
            f0 = lbl_8047B724;
            f1 = lbl_8047B71C;
            f0 = f0 * f30;
            *(u32*)(sp + 0x34) = tmp;
            f0 = f0 / f1;
            f2 = f2 - f3;
            /* cror eq, lt, eq */;
            if (f2 == f0) {
                r31 = 0x80000000;
                break;
            }
            f0 = lbl_8047B728;
            *(u32*)(sp + 0x34) = tmp;
            f0 = f0 * f30;
            f0 = f0 / f1;
            f1 = f1 - f3;
            /* cror eq, lt, eq */;
            if (f1 == f0) {
                r31 = 0x64640000;
                break;
            }
            r31 = 0x800000;
        } while (0);

        r3 = *(s16*)((u8*)r28 + 0x54);
        tmp = 0x43300000;
        f0 = lbl_8047B72C;
        r31 = r31 | r8;
        r4 = r4 * r3;
        *(u32*)(sp + 0x30) = tmp;
        f1 = lbl_8047B738;
        f2 = f30 - f0;
        r3 = 0x1;
        *(u32*)(sp + 0x34) = tmp;
        f0 = f0 - f1;
        f0 = f2 + f0;
        f0 = f0 / f30;
        f0 = (f64)(s32)f0;
        fn_800D88DC();
        r3 = 0x6;
        fn_800D888C();
        r3 = 0x7;
        fn_800D6A00();
        r3 = (u32)lbl_80314E08;
        r3 = (u32)lbl_80314E08;
        fn_800D7820();
        r3 = 0x2;
        fn_800D67BC();
        r3 = 0x0;
        r4 = 0x0;
        fn_800D61E4();
        r4 = r31;
        r3 = 0x0;
        fn_800D5BA0();
        r4 = *(s16*)((u8*)r28 + 0x56);
        r3 = r30;
        fn_800D61E4();
        r4 = r31;
        r3 = 0x0;
        fn_800D5BA0();
        fn_800D6728();
    }
    f0 = lbl_8047B718;
    /* cror eq, lt, eq */;
    do {
        if (f31 == f0) {
            r9 = 0x0;
            break;
        }
        f0 = lbl_8047B724;
        f1 = lbl_8047B71C;
        f0 = f0 * f30;
        f0 = f0 / f1;
        /* cror eq, lt, eq */;
        if (f31 == f0) {
            r9 = 0x1ad;
            break;
        }
        f0 = lbl_8047B728;
        f0 = f0 * f30;
        f0 = f0 / f1;
        /* cror eq, lt, eq */;
        if (f31 == f0) {
            r9 = 0x1b0;
            break;
        }
        r9 = 0x1b1;
    } while (0);

    r4 = *(s16*)((u8*)r28 + 0x54);
    r3 = 0x43300000;
    f0 = lbl_8047B72C;
    tmp = r9 & 0xFFFF;
    f2 = lbl_8047B738;
    f0 = f30 - f0;
    f1 = f1 - f2;
    f0 = f31 * f1 + f0;
    f0 = f0 / f30;
    f0 = (f64)(s32)f0;
    if (tmp == 0) return;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r10 = 0x0;
    windowDrawSprite2();
    return;
L_80012680:
    r3 = *(s16*)((u8*)r31 + 0xC);
    if ((s32)r3 != 0) {
        tmp = *(s16*)((u8*)r31 + 0xE);
        r4 = 0x43300000;
        r6 = *(u32*)((u8*)r31 + 0x8);
        tmp = *(u32*)((u8*)r30 + 0x20);
        tmp = tmp - r6;
        f3 = lbl_8047B738;
        f2 = lbl_8047B730;
        f1 = f0 - f3;
        *(u32*)(sp + 0x24) = tmp;
        f0 = f0 - f3;
        f3 = f1 / f0;
        f1 = f1 - f2;
        f0 = f0 - f2;
        f1 = f3 * f1 + f0;
        __cvt_fp2unsigned();
    } else {

        r3 = *(u32*)((u8*)r30 + 0x20);
    }
    r11 = *(u32*)((u8*)r30 + 0x1C);
    if (r3 > r11) {
        r3 = r11;
    }
    if (r11 == 0) return;
    tmp = *(s16*)((u8*)r28 + 0x54);
    r7 = r29;
    r6 = *(s16*)((u8*)r28 + 0x56);
    r8 = r27;
    tmp = r3 * tmp;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ac;
    r10 = 0x0;
    r5 = r11 + tmp;
    tmp = (u32)tmp / (u32)r11;
    r5 = (s16)tmp;
    windowDrawSprite2();
    return;
L_80012758:
    tmp = *(s16*)((u8*)r31 + 0xC);
    if ((s32)tmp != 0) {
        r4 = *(s16*)((u8*)r31 + 0xE);
        r3 = 0x43300000;
        f3 = lbl_8047B738;
        f4 = *(f32*)((u8*)r31 + 0x8);
        *(u32*)(sp + 0x2C) = tmp;
        f2 = f0 - f3;
        f0 = *(f32*)((u8*)r30 + 0x20);
        f0 = f0 - f4;
        f1 = f1 - f3;
        f1 = f2 / f1;
        f2 = f1 * f0 + f4;
    } else {

        f2 = *(f32*)((u8*)r30 + 0x20);
    }
    f3 = *(f32*)((u8*)r30 + 0x1C);
    if (f2 > f3) {
        f2 = f3;
    }
    r3 = *(s16*)((u8*)r28 + 0x54);
    tmp = 0x43300000;
    f0 = lbl_8047B718;
    *(u32*)(sp + 0x20) = tmp;
    f1 = lbl_8047B738;
    f0 = f0 - f1;
    f0 = f2 * f0;
    f0 = f0 / f3;
    f0 = (f64)(s32)f0;
    if (f2 > f0) {
        tmp = (s16)r5;
        if ((s32)tmp == 0) {
            r5 = 0x1;
    }
    }
    r6 = *(s16*)((u8*)r28 + 0x56);
    r7 = r29;
    r8 = r27;
    r3 = 0x0;
    r4 = 0x0;
    r9 = 0x1ab;
    r10 = 0x0;
    windowDrawSprite2();

    return;
}
#endif
