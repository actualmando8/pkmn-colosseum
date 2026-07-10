/**
 * @file gs_range_8008C7B0.c
 * @brief gs-engine code, 0x8008C7B0 - 0x800980E0 (84 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"
#include "game/gs_material.h"

#define GBA_DATA_OFFSET 0x20
#define GBA_STATE_PORT 0x4338
#define GBA_STATE_TIMEOUT 0x433C
#define GBA_STATE_PHASE 0x4340
#define GBA_THREAD_PRIORITY 8

extern u32 lbl_8047A690;
extern u32 lbl_8047A694;
extern f32 lbl_8047C1D0; /* 0.833333313f -- PAL-adjusted 1-unit wait */
extern f32 lbl_8047C1D4; /* 0.0f */
extern f32 lbl_8047C1D8; /* 1.0f */
extern f32 lbl_8047C1DC; /* 83.3333282f -- PAL-adjusted 100-unit wait */
extern f32 lbl_8047C1E0; /* {41.6666641f, 0.0f} -- PAL-adjusted 50-unit wait */

/* Additional data labels referenced by the ported gba_comm_ext.c /
 * late_game.c bodies below (GBA link-cable state machine + battle-status
 * window helpers living in this same address range per the current
 * object map). */
extern u8 lbl_803FB328[];
extern u8 lbl_803FB380[];
extern u8 lbl_8047C1E8;
extern u8 lbl_8026F5A8[];
extern u8 lbl_8026F5C0[];

/* Common callees needed by the ported bodies below that are not already
 * declared with a full prototype at the point of use. */
extern void _threadSwitch(void);
extern void* windowSearchID(u32 id);
extern void fn_8009F7B4(void *p);
extern void fn_8009F890(void *p);
extern void fn_800A257C(void *p, u32 b);
extern void fn_800716E8(u32 port, u32 val);
extern void fn_8009FABC(void *p);
extern void fn_800A1E54(void *p, u32 v);
extern void fn_800716C8(u32 port, void *a, void *b);
extern u32 fn_800E202C(void *p);
extern void __assert(const u8 *file, u32 line, const u8 *msg);
extern void fn_800E24B0(u32 status);
extern void fn_800E209C(u32 status);
extern u32 fn_800A13F8(void);
extern void OSYieldThread(void);
extern void fn_800FF730(u32 id);
extern void floorSetFadeScript(u32 a, u32 b);
extern u32 GSresGetResource(u32 ctx, u32 id);

/* Storage used by the battle-status window renderer below. */
extern u8 lbl_8047C200;
extern u8 lbl_8047C204;
extern u8 lbl_8047C208;
extern u8 lbl_8047C20C;
extern u8 lbl_8047C210;
extern u8 lbl_8047C214;
extern u8 lbl_8047C218;
extern u8 lbl_8047C21C;
extern u8 lbl_8047C220;
extern u8 lbl_8047C228;
extern u8 lbl_8047C230;
extern u8 lbl_8047C234;
extern u8 lbl_8047C238;
extern void fn_801040F0();
extern void winSpriteSetDisp();
extern void fn_8001E58C();
extern void fn_800FA280();
extern void fn_800FA444();
extern void fn_800FB680();
extern void fn_800FB8C8();
extern void fn_800FBB34();

void fn_80094650(u32 r3, u32 r4) {
    extern void fn_8010C46C();
    extern void fn_8011BEB4();
    extern void fn_80123CD4();
    extern void fn_80123E70();
    extern void fn_8012640C();
    extern void fn_80132A38();
    extern void fn_801EE034();
    extern void fn_801EE04C();
    extern void fn_801EE064();
    extern void fn_801EE07C();
    extern void fn_801EE0A8();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;

    r27 = r3;
    r31 = r4;
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r29 = *(u32*)((u8*)r3 + 0xC);
    if (r29 == 0) return;
    tmp = *(s16*)((u8*)r31 + 0x6);
    r30 = 0x1;
    if ((s32)tmp < 0x1b8) {
        if ((s32)tmp < 0x18b) {
            if ((s32)tmp >= 0x182) goto L_8009473C;
            if ((s32)tmp < 0x170) {
                goto L_8009473C;
            }
            if ((s32)tmp < 0x191) {
            }
            goto L_800946D0;
        }
        if ((s32)tmp < 0x1d3) {
        }
        if ((s32)tmp < 0x1ca) {

        } else {
        }
        if ((s32)tmp >= 0x1d9) goto L_8009473C;
    }
L_800946D0:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    tmp = *(u8*)((u8*)r3 + 0x1);
    if ((s32)tmp != 7) {
        if ((s32)tmp >= 7 || (s32)tmp >= 5) goto L_8009472C;

        if ((s32)tmp < 3) {
            goto L_8009472C;
        }
        }
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    tmp = *(u8*)((u8*)r3 + 0x2);
    tmp = (s8)tmp;
    if ((s32)tmp >= 0 || (s32)tmp > 4) {

        r30 = 0x1;
        goto L_80094730;
    }
    r30 = 0x0;
    goto L_80094730;
L_8009472C:
    r30 = 0x0;
L_80094730:
    r3 = r31;
    r4 = r30;
    winSpriteSetDisp(r3, r4);
L_8009473C:
    tmp = r30 & 0xFF;
    if (tmp == 0) return;
    r4 = *(s16*)((u8*)r31 + 0x6);
    tmp = -0x100;
    r3 = *(u8*)((u8*)r27 + 0x8B);
    r30 = r3 | tmp;
    if ((s32)r4 < 0x1c1) {
        if ((s32)r4 != 0x18d) {
            if ((s32)r4 < 0x18d) {
                if ((s32)r4 == 0x181) return;
                if ((s32)r4 < 0x181) {
                    if ((s32)r4 == 0x170) goto L_80094F3C;
                    if ((s32)r4 < 0x170) return;
                    if ((s32)r4 >= 0x179) goto L_800952B4;
                    goto L_80095490;
                }
                if ((s32)r4 == 0x18b) goto L_80095010;
                if ((s32)r4 >= 0x18b) goto L_800950A8;
                if ((s32)r4 >= 0x187) return;
                goto L_800949F4;
            }
            if ((s32)r4 < 0x196) {
                if ((s32)r4 == 0x190) return;
                if ((s32)r4 >= 0x190) goto L_80094B58;
                if ((s32)r4 < 0x18f) return;

            }
            if ((s32)r4 == 0x1b8) goto L_80094F3C;
            if ((s32)r4 >= 0x1b8) goto L_80095490;
            if ((s32)r4 >= 0x19b) return;
            goto L_80094CB8;
        }
        if ((s32)r4 == 0x1d7) goto L_800951F4;
        if ((s32)r4 < 0x1d7) {
            if ((s32)r4 == 0x1d3) goto L_80095010;
            if ((s32)r4 < 0x1d3) {
                if ((s32)r4 == 0x1c9) return;
                if ((s32)r4 < 0x1c9) goto L_800952B4;
                if ((s32)r4 >= 0x1ce) return;
                goto L_800949F4;
            }
        }
        if ((s32)r4 == 0x1d5) goto L_80095134;
        if ((s32)r4 >= 0x1d5) return;
        goto L_800950A8;
    }
    if ((s32)r4 < 0x59b) {
        if ((s32)r4 < 0x1dd) {
            if ((s32)r4 < 0x1d9) return;

        }
        if ((s32)r4 >= 0x1e1) return;
        goto L_80094CB8;
    }
    if ((s32)r4 < 0x12b3) {
        if ((s32)r4 >= 0x59f) return;
    } else {

        if ((s32)r4 >= 0x12b8) return;
    }
    r3 = *(u32*)&lbl_8047C200;
    tmp = *(u32*)&lbl_8047C204;
    *(u32*)(sp + 0x14) = tmp;
    if ((s32)r4 != 0x12b3) {
        if ((s32)r4 < 0x12b3) {
            if ((s32)r4 != 0x59d) {
                if ((s32)r4 < 0x59d) {
                    if ((s32)r4 != 0x59b) {
                        if ((s32)r4 < 0x59b) {
                            goto L_80094910;
                        }
                        if ((s32)r4 >= 0x59f) goto L_80094910;
                        goto L_800948E4;
                    }
                    if ((s32)r4 == 0x12b6) goto L_8009490C;
                    if ((s32)r4 < 0x12b6) {
                        if ((s32)r4 >= 0x12b5) goto L_80094904;
                        goto L_800948FC;
                    }
                    if ((s32)r4 >= 0x12b8) goto L_80094910;
                    goto L_800948EC;
                        }
                r28 = 0x0;
                goto L_80094910;
                        }
            r28 = 0x1;
            goto L_80094910;
            }
        r28 = 0x2;
        goto L_80094910;
    L_800948E4:
        r28 = 0x3;
        goto L_80094910;
    L_800948EC:
        r28 = 0x0;
        goto L_80094910;
    }
    r28 = 0x1;
    goto L_80094910;
L_800948FC:
    r28 = 0x2;
    goto L_80094910;
L_80094904:
    r28 = 0x3;
    goto L_80094910;
L_8009490C:
    r28 = 0x4;
L_80094910:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    tmp = *(u8*)((u8*)r3 + 0x1);
    if ((s32)tmp != 4) {
        if ((s32)tmp < 4) {
            if ((s32)tmp < 3) return;

        }
        if ((s32)tmp != 7) return;

    }
    r3 = *(u8*)((u8*)r3 + 0x3);
    tmp = (s8)r28;
    r3 = (s8)r3;
    if ((s32)r3 == (s32)tmp) {
        r7 = (u32)sp + 0x10;
        r3 = 0x0;
        r4 = 0x0;
        *(u32*)(sp + 0x10) = tmp;
        r5 = *(s16*)((u8*)r31 + 0x54);
        r6 = *(s16*)((u8*)r31 + 0x56);
        ((void(*)(void))fn_8001E58C)();
    }
    r3 = (u32)&lbl_803FB380;
    tmp = (s8)r28;
    r3 = (u32)&lbl_803FB380;
    r3 = *(u8*)((u8*)r3 + 0x2);
    r3 = (s8)r3;
    if ((s32)r3 != (s32)tmp) return;
    r7 = (u32)sp + 0xc;
    r3 = 0x0;
    r4 = 0x0;
    *(u32*)(sp + 0xC) = tmp;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r6 = *(s16*)((u8*)r31 + 0x56);
    ((void(*)(void))fn_8001E58C)();
    return;

    r3 = (u32)&lbl_803FB380;
    tmp = (s8)r28;
    r3 = (u32)&lbl_803FB380;
    r3 = *(u8*)((u8*)r3 + 0x2);
    r3 = (s8)r3;
    if ((s32)r3 != (s32)tmp) return;
    r7 = (u32)sp + 0x8;
    r3 = 0x0;
    r4 = 0x0;
    *(u32*)(sp + 0x8) = tmp;
    r5 = *(s16*)((u8*)r31 + 0x54);
    r6 = *(s16*)((u8*)r31 + 0x56);
    ((void(*)(void))fn_8001E58C)();
    return;
L_800949F4:
do {
    if ((s32)r4 != 0x186) {
        if ((s32)r4 < 0x186) {
            if ((s32)r4 != 0x183) {
                if ((s32)r4 < 0x183) {
                    if ((s32)r4 < 0x182) {
                        break;
                    }
                    if ((s32)r4 < 0x185) {
                        goto L_80094A78;
                    }
                    if ((s32)r4 != 0x1cc) {
                        if ((s32)r4 < 0x1cc) {
                            if ((s32)r4 != 0x1ca) {
                                if ((s32)r4 < 0x1ca) {
                                    break;
                                }
                                if ((s32)r4 >= 0x1ce) break;
                                r28 = 0x0;
                                break;
                            }
                            r28 = 0x1;
                            break;
                                }
                        r28 = 0x2;
                        break;
                            }
                    r28 = 0x3;
                    break;
                }
                r28 = 0x0;
                break;
                    }
            r28 = 0x1;
            break;
        L_80094A78:
            r28 = 0x2;
            break;
                }
        r28 = 0x3;
        break;
                    }
    r28 = 0x4;
} while (0);
    r30 = r28 & 0xFFFF;
    if (r30 == 4) {
        r3 = (u32)&lbl_803FB380;
        r3 = (u32)&lbl_803FB380;
        r28 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r28 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r30;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r28 = 0x0;
        }
    }
    tmp = r28 & 0xFFFF;
    if ((s32)tmp != 0x164) {
        if ((s32)tmp < 0x164) {
            if ((s32)tmp != 0) {
                goto L_80094B14;
            }
            if ((s32)tmp >= 0x166) goto L_80094B14;
            goto L_80094B0C;
        }
            }
    tmp = 0x0;
    goto L_80094B34;
L_80094B0C:
    tmp = 0x5d;
    goto L_80094B34;
L_80094B14:
    r4 = r28;
    r3 = 0x0;
    r5 = 0x3;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    fn_8010C46C();
    tmp = r3 & 0xFFFF;
L_80094B34:
    if (tmp == 0) return;
    r5 = r27;
    r6 = tmp & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801040F0)();
    return;
L_80094B58:
do {
    if ((s32)r4 != 0x195) {
        if ((s32)r4 < 0x195) {
            if ((s32)r4 != 0x192) {
                if ((s32)r4 < 0x192) {
                    if ((s32)r4 < 0x191) {
                        break;
                    }
                    if ((s32)r4 < 0x194) {
                        goto L_80094BDC;
                    }
                    if ((s32)r4 != 0x1db) {
                        if ((s32)r4 < 0x1db) {
                            if ((s32)r4 != 0x1d9) {
                                if ((s32)r4 < 0x1d9) {
                                    break;
                                }
                                if ((s32)r4 >= 0x1dd) break;
                                r28 = 0x0;
                                break;
                            }
                            r28 = 0x1;
                            break;
                                }
                        r28 = 0x2;
                        break;
                            }
                    r28 = 0x3;
                    break;
                }
                r28 = 0x0;
                break;
                    }
            r28 = 0x1;
            break;
        L_80094BDC:
            r28 = 0x2;
            break;
                }
        r28 = 0x3;
        break;
                    }
    r28 = 0x4;
} while (0);
    r28 = r28 & 0xFFFF;
    if (r28 == 4) {
        r3 = (u32)&lbl_803FB380;
        r3 = (u32)&lbl_803FB380;
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r28;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    tmp = r27 & 0xFFFF;
    if (tmp == 0) {
        r5 = *(s16*)((u8*)r31 + 0x54);
        r7 = r30;
        r6 = *(s16*)((u8*)r31 + 0x56);
        r3 = 0x0;
        r4 = 0x0;
        r8 = 0x2be0;
        ((void(*)(void))fn_800FBB34)();
        return;
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x1;
    r6 = 0x0;
    fn_8011BEB4();
    if (r3 == 0) return;
    ((void(*)(void))fn_800FA280)();
    r4 = r3;
    r3 = 0x37;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xe7;
    ((void(*)(void))fn_800FBB34)();
    return;
L_80094CB8:
do {
    if ((s32)r4 != 0x19a) {
        if ((s32)r4 < 0x19a) {
            if ((s32)r4 != 0x197) {
                if ((s32)r4 < 0x197) {
                    if ((s32)r4 < 0x196) {
                        break;
                    }
                    if ((s32)r4 < 0x199) {
                        goto L_80094D3C;
                    }
                    if ((s32)r4 != 0x1df) {
                        if ((s32)r4 < 0x1df) {
                            if ((s32)r4 != 0x1dd) {
                                if ((s32)r4 < 0x1dd) {
                                    break;
                                }
                                if ((s32)r4 >= 0x1e1) break;
                                r28 = 0x0;
                                break;
                            }
                            r28 = 0x1;
                            break;
                                }
                        r28 = 0x2;
                        break;
                            }
                    r28 = 0x3;
                    break;
                }
                r28 = 0x0;
                break;
                    }
            r28 = 0x1;
            break;
        L_80094D3C:
            r28 = 0x2;
            break;
                }
        r28 = 0x3;
        break;
                    }
    r28 = 0x4;
} while (0);
    r26 = r28 & 0xFFFF;
    if (r26 == 4) {
        r3 = (u32)&lbl_803FB380;
        r3 = (u32)&lbl_803FB380;
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r26;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r26;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    r3 = 0x2bd4;
    ((void(*)(void))fn_800FA444)();
    r3 = (u32)r3 >> 16;
    tmp = *(s16*)((u8*)r31 + 0x54);
    r3 = (s16)r3;
    r5 = r30;
    r3 = tmp - r3;
    r4 = 0x0;
    tmp = (u32)r3 >> 31;
    r6 = 0x2bd4;
    tmp = tmp + r3;
    tmp = (s32)tmp >> 1;
    r25 = (s16)tmp;
    r3 = r25;
    ((void(*)(void))fn_800FB680)();
    tmp = r27 & 0xFFFF;
    if ((s32)tmp != 0x164) {
        if ((s32)tmp < 0x164) {
            if ((s32)tmp != 0) {
                goto L_80094E7C;
            }
            if ((s32)tmp >= 0x166) goto L_80094E7C;
            goto L_80094E40;
        }
            }
    r6 = *(s16*)((u8*)r31 + 0x56);
    r5 = r25;
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0x2be1;
    ((void(*)(void))fn_800FB8C8)();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0x2be1;
    ((void(*)(void))fn_800FB8C8)();
    return;
L_80094E40:
    r6 = *(s16*)((u8*)r31 + 0x56);
    r5 = r25;
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0x2b6d;
    ((void(*)(void))fn_800FB8C8)();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0x2b6d;
    ((void(*)(void))fn_800FB8C8)();
    return;
L_80094E7C:
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r4 = r27;
        r3 = 0x0;
        r5 = 0x2;
        r6 = 0x0;
        fn_8011BEB4();
    } else {

        r3 = r29;
        r6 = r26;
        r4 = 0x0;
        r5 = 0x80;
        fn_8012640C();
    }
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r6 = *(s16*)((u8*)r31 + 0x56);
    r5 = r25;
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xd2;
    ((void(*)(void))fn_800FB8C8)();
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r4 = r27;
        r3 = 0x0;
        r5 = 0x2;
        r6 = 0x0;
        fn_8011BEB4();
    } else {

        r3 = r29;
        r4 = r28;
        fn_80123E70();
        r3 = r3 & 0xFF;
    }
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xd2;
    ((void(*)(void))fn_800FB8C8)();
    return;
L_80094F3C:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r30 = *(u8*)((u8*)r3 + 0x2);
    r30 = (s8)r30;
    tmp = r30 & 0xFFFF;
    if (tmp == 4) {
        r28 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r30;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r28 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r30;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r28 = 0x0;
        }
    }
    tmp = r28 & 0xFFFF;
    if ((s32)tmp != 0x164) {
        if ((s32)tmp < 0x164) {
            if ((s32)tmp != 0) {
                goto L_80094FCC;
            }
            if ((s32)tmp >= 0x166) goto L_80094FCC;
            goto L_80094FC4;
        }
            }
    tmp = 0x0;
    goto L_80094FEC;
L_80094FC4:
    tmp = 0x5d;
    goto L_80094FEC;
L_80094FCC:
    r4 = r28;
    r3 = 0x0;
    r5 = 0x24;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFF;
    fn_801EE0A8();
    tmp = r3 & 0xFFFF;
L_80094FEC:
    if (tmp == 0) return;
    r5 = r27;
    r6 = tmp & 0xFFFF;
    r3 = 0x0;
    r4 = 0x0;
    r7 = 0x0;
    ((void(*)(void))fn_801040F0)();
    return;
L_80095010:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r28 = *(u8*)((u8*)r3 + 0x2);
    r28 = (s8)r28;
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r28;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x23;
    r6 = 0x0;
    fn_8011BEB4();
    r3 = r3 & 0xFFFF;
    fn_801EE07C();
    fn_801EE034();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r8 = r3;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_800FBB34)();
    return;
L_800950A8:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r28 = *(u8*)((u8*)r3 + 0x2);
    r28 = (s8)r28;
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r28;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x22;
    r6 = 0x0;
    fn_8011BEB4();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r8 = r3;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r7 = r30;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_800FBB34)();
    return;
L_80095134:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r28 = *(u8*)((u8*)r3 + 0x2);
    r28 = (s8)r28;
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r28;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x6;
    r6 = 0x0;
    fn_8011BEB4();
    if (r3 <= 1) {
        r5 = *(s16*)((u8*)r31 + 0x54);
        r7 = r30;
        r6 = *(s16*)((u8*)r31 + 0x56);
        r3 = 0x0;
        r4 = 0x0;
        r8 = 0x2be2;
        ((void(*)(void))fn_800FB8C8)();
        return;
    }
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xd2;
    ((void(*)(void))fn_800FB8C8)();
    return;
L_800951F4:
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r28 = *(u8*)((u8*)r3 + 0x2);
    r28 = (s8)r28;
    tmp = r28 & 0xFFFF;
    if (tmp == 4) {
        r27 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r28;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r27 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r28;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r27 = 0x0;
        }
    }
    r4 = r27;
    r3 = 0x0;
    r5 = 0x7;
    r6 = 0x0;
    fn_8011BEB4();
    if (r3 <= 1) {
        r5 = *(s16*)((u8*)r31 + 0x54);
        r7 = r30;
        r6 = *(s16*)((u8*)r31 + 0x56);
        r3 = 0x0;
        r4 = 0x0;
        r8 = 0x2be2;
        ((void(*)(void))fn_800FB8C8)();
        return;
    }
    r4 = r3;
    r3 = 0x34;
    fn_80132A38();
    r5 = *(s16*)((u8*)r31 + 0x54);
    r7 = r30;
    r6 = *(s16*)((u8*)r31 + 0x56);
    r3 = 0x0;
    r4 = 0x0;
    r8 = 0xd2;
    ((void(*)(void))fn_800FB8C8)();
    return;
L_800952B4:
do {
    if ((s32)r4 != 0x1c1) {
        if ((s32)r4 < 0x1c1) {
            if ((s32)r4 != 0x17d) {
                if ((s32)r4 < 0x17d) {
                    if ((s32)r4 != 0x17a) {
                        if ((s32)r4 < 0x17a) {
                            if ((s32)r4 < 0x179) {
                                break;
                            }
                            if ((s32)r4 < 0x17c) {
                                goto L_800953A4;
                            }
                            if ((s32)r4 != 0x180) {
                                if ((s32)r4 >= 0x180) break;
                                if ((s32)r4 < 0x17f) {
                                    goto L_8009538C;
                                }
                                if ((s32)r4 != 0x1c6) {
                                    if ((s32)r4 < 0x1c6) {
                                        if ((s32)r4 != 0x1c4) {
                                            if ((s32)r4 < 0x1c4) {
                                                if ((s32)r4 < 0x1c3) {
                                                    goto L_8009536C;
                                                }
                                                if ((s32)r4 != 0x1c8) {
                                                    if ((s32)r4 >= 0x1c8) break;

                                                } else {
                                                    r28 = 0x1;
                                                    break;
                                                }
                                                r28 = 0x2;
                                                break;
                                            }
                                            r28 = 0x3;
                                            break;
                                                }
                                        r28 = 0x4;
                                        break;
                                            }
                                    r28 = 0x5;
                                    break;
                                                }
                                r28 = 0x6;
                                break;
                            L_8009536C:
                                r28 = 0x7;
                                break;
                            }
                            r28 = 0x8;
                            break;
                                }
                        r28 = 0x1;
                        break;
                                }
                    r28 = 0x2;
                    break;
                L_8009538C:
                    r28 = 0x3;
                    break;
                        }
                r28 = 0x4;
                break;
                            }
            r28 = 0x5;
            break;
        L_800953A4:
            r28 = 0x6;
            break;
                        }
        r28 = 0x7;
        break;
                            }
    r28 = 0x8;
} while (0);
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r31 = *(u8*)((u8*)r3 + 0x2);
    r31 = (s8)r31;
    tmp = r31 & 0xFFFF;
    if (tmp == 4) {
        r30 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r30 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r31;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r30 = 0x0;
        }
    }
    tmp = r30 & 0xFFFF;
    if (tmp != 0) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x23;
        r6 = 0x0;
        fn_8011BEB4();
        r3 = r3 & 0xFFFF;
        fn_801EE07C();
        fn_801EE064();
        r4 = r3 & 0xFF;
    } else {

        r4 = 0x0;
    }
    r3 = 0x66660000;
    tmp = r28 & 0xFFFF;
    r3 = r3 + 0x6667;
    r5 = r27;
    r6 = (s32)((s64)r3 * (s64)r4 >> 32);
    r3 = 0x0;
    r4 = 0x0;
    r6 = (s32)r6 >> 2;
    r7 = (u32)r6 >> 31;
    r6 = r6 + r7;
    if ((s32)r6 >= (s32)tmp) {
        r6 = 0xf6;
    } else {

        r6 = 0xf5;
    }
    r7 = 0x0;
    ((void(*)(void))fn_801040F0)();
    return;
L_80095490:
do {
    if ((s32)r4 != 0x1b9) {
        if ((s32)r4 < 0x1b9) {
            if ((s32)r4 != 0x175) {
                if ((s32)r4 < 0x175) {
                    if ((s32)r4 != 0x172) {
                        if ((s32)r4 < 0x172) {
                            if ((s32)r4 < 0x171) {
                                break;
                            }
                            if ((s32)r4 < 0x174) {
                                goto L_80095580;
                            }
                            if ((s32)r4 != 0x178) {
                                if ((s32)r4 >= 0x178) break;
                                if ((s32)r4 < 0x177) {
                                    goto L_80095568;
                                }
                                if ((s32)r4 != 0x1be) {
                                    if ((s32)r4 < 0x1be) {
                                        if ((s32)r4 != 0x1bc) {
                                            if ((s32)r4 < 0x1bc) {
                                                if ((s32)r4 < 0x1bb) {
                                                    goto L_80095548;
                                                }
                                                if ((s32)r4 != 0x1c0) {
                                                    if ((s32)r4 >= 0x1c0) break;

                                                } else {
                                                    r28 = 0x1;
                                                    break;
                                                }
                                                r28 = 0x2;
                                                break;
                                            }
                                            r28 = 0x3;
                                            break;
                                                }
                                        r28 = 0x4;
                                        break;
                                            }
                                    r28 = 0x5;
                                    break;
                                                }
                                r28 = 0x6;
                                break;
                            L_80095548:
                                r28 = 0x7;
                                break;
                            }
                            r28 = 0x8;
                            break;
                                }
                        r28 = 0x1;
                        break;
                                }
                    r28 = 0x2;
                    break;
                L_80095568:
                    r28 = 0x3;
                    break;
                        }
                r28 = 0x4;
                break;
                            }
            r28 = 0x5;
            break;
        L_80095580:
            r28 = 0x6;
            break;
                        }
        r28 = 0x7;
        break;
                            }
    r28 = 0x8;
} while (0);
    r3 = (u32)&lbl_803FB380;
    r3 = (u32)&lbl_803FB380;
    r31 = *(u8*)((u8*)r3 + 0x2);
    r31 = (s8)r31;
    tmp = r31 & 0xFFFF;
    if (tmp == 4) {
        r30 = *(u16*)((u8*)r3 + 0x18);

    } else {
        r3 = r29;
        r6 = r31;
        r4 = 0x0;
        r5 = 0x7f;
        fn_8012640C();
        r30 = r3 & 0xFFFF;
        r3 = r29;
        r4 = r31;
        fn_80123CD4();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            r30 = 0x0;
        }
    }
    tmp = r30 & 0xFFFF;
    if (tmp != 0) {
        r4 = r30;
        r3 = 0x0;
        r5 = 0x23;
        r6 = 0x0;
        fn_8011BEB4();
        r3 = r3 & 0xFFFF;
        fn_801EE07C();
        fn_801EE04C();
        r4 = r3 & 0xFF;
    } else {

        r4 = 0x0;
    }
    r3 = 0x66660000;
    tmp = r28 & 0xFFFF;
    r3 = r3 + 0x6667;
    r5 = r27;
    r6 = (s32)((s64)r3 * (s64)r4 >> 32);
    r3 = 0x0;
    r4 = 0x0;
    r6 = (s32)r6 >> 2;
    r7 = (u32)r6 >> 31;
    r6 = r6 + r7;
    if ((s32)r6 >= (s32)tmp) {
        r6 = 0xf7;
    } else {

        r6 = 0xf5;
    }
    r7 = 0x0;
    ((void(*)(void))fn_801040F0)();

    return;
}



/* 0x80091564 | size: 0x210 */
void fn_80091564(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11261400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 4, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 7, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0C421800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3D1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x81);
    floorSetFadeScript(0, 0);
}

/* 0x80091774 | size: 0x210 */
void fn_80091774(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11251400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 3, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 6, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0C411800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3C1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    floorSetFadeScript(0, 0);
}

/* 0x80091984 | size: 0x210 */
void fn_80091984(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11241400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 2, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 5, 0, 1);

    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0C401800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0C3B1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    floorSetFadeScript(0, 0);
}

/* 0x80091B94 | size: 0x210 */
void fn_80091B94(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);

    u32 waitFrames;
    u32 elapsed;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x06DC1605);
    lbl_8047A694 = GSresGetResource(ctx, 0x06DC1001);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB7C4(0x06DC1000);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x06DC1000), GSresGetResource(ctx, 0x11221400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x06DC1000), 4);

    fn_801CB834(0x06DC1000, 0, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BD0400);
    fn_801CB834(iconHandle, 1, 0, 0);

    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0C3E1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x10491000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x82);
    floorSetFadeScript(0, 0);
}

/* 0x8008CACC | size: 0x30C */
void fn_8008CACC(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x0CE61000), GSresGetResource(ctx, 0x11211400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x0CE61000), 4);

    fn_801CB834(0x0CE61000, 2, 0, 0);
    waitFrames = 100;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1DC;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06BC0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0D021800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0C1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 4, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008FE94 | size: 0x26C */
void fn_8008FE94(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06BD0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0CF41800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CEB1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 2, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008CDD8 | size: 0x2C8 */
void fn_8008CDD8(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0D011800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0B1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 0xB, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 0xC, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008D0A0 | size: 0x2A8 */
void fn_8008D0A0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0D001800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D0A1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 9, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008EC28 | size: 0x2A8 */
void fn_8008EC28(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0CFB1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D051000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 7, 0, 1);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008EED0 | size: 0x2C0 */
void fn_8008EED0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 4, 0, 0);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0CFA1800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0D031000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 5, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008F190 | size: 0x394 */
void fn_8008F190(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, 0x0CE61000), GSresGetResource(ctx, 0x111B1400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, 0x0CE61000), 4);

    fn_801CB834(0x0CE61000, 3, 0, 0);
    waitFrames = 0x32;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1E0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 0, 0, 0);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0CF91800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CF81000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);
    fn_801CB834(iconHandle, 1, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 2, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 3, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008FBF4 | size: 0x2A0 */
void fn_8008FBF4(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    fn_801CB834(iconHandle, 0, 0, 1);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0CF51800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CEC1000);
    fn_801845E4(ctx, iconHandle, ctx, finalResult, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

static inline u32 fn_80090720_getHandle2(u32 ctx) {
    extern u32 GSresGetResource(u32 ctx, u32 id);
    return GSresGetResource(ctx, 0x0CE61004);
}

/* 0x80090720 | size: 0x2C4 */
void fn_80090720(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern void scriptWaitSyncMotion(u32 id, u32 val);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 new_var;
    u32 handle2;
    u32 iconHandle;
    u32 iconResult;
    u32 finalResult;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);

    frame = lbl_8047C1D4;
    handle2 = fn_80090720_getHandle2(ctx);
    GSmodelSetAnimIndex(handle2, 1);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 1);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    fn_801CB834(0x0CE61000, 0, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    iconHandle = fn_801CBA0C(0x06BC0400);
    iconResult = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(iconResult, 2);
    GSmodelSetShadowLight(iconResult, lbl_8047A690);
    GSmodelSetShadowSurface(iconResult, 1, &lbl_8047A694);

    cameraPlayAnime(ctx, 0x0CF21800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    finalResult = fn_801CBA0C(0x0CE91000);
    new_var = iconHandle;
    fn_801845E4(ctx, new_var, ctx, finalResult, 0);
    fn_801CB834(new_var, 1, 0, 0);
    fn_801CB834(0x0CE61004, 0, 0, 0);
    scriptWaitSyncMotion(iconHandle, 1);
    fn_801CB834(iconHandle, 2, 0, 0);
    scriptWaitSyncMotion(new_var, 1);
    fn_801CB834(new_var, 3, 0, 0);

    cameraWaitSyncAnime(1);
    fn_800FF58C(0x89);
    floorSetFadeScript(0, 0);
}

/* 0x8008C7B0 | size: 0x31C */
void fn_8008C7B0(u32 ctx) {
    #pragma peephole off
    extern u32 GSresGetResource(u32 ctx, u32 id);
    extern void GSmodelSetShadowTextureSize(u32 w, u32 h);
    extern void fn_801CB7C4(u32 id);
    extern void GSmodelLinkToGSparticleBank(u32 handle, u32 val);
    extern void GSmodelSetGSparticleLinkAttachMode(u32 handle, u32 val);
    extern void fn_801CB834(u32 id, u32 slot, u32 x, u32 y);
    extern s32 fn_800D37CC(void);
    extern void _threadSwitch(void);
    extern u32 fn_800D3088(void);
    extern u32 fn_801CBA0C(u32 id);
    extern u32 GSmodelSetShadowFlags(u32 handle, u32 val);
    extern void GSmodelSetShadowLight(u32 handle, u32 val);
    extern void GSmodelSetShadowSurface(u32 handle, u32 val, u32 *param);
    extern void cameraPlayAnime(u32 ctx, u32 id, u32 a, u32 b);
    extern void fn_801845E4(u32 ctx, u32 modelHandle, u32 ctx2, u32 handle, u32 flags);
    extern void cameraWaitSyncAnime(s32 sync);
    extern void fn_80190528(u32 id);
    extern void fn_800FF58C(u32 id);
    extern void floorSetFadeScript(u32 a, u32 b);
    extern void fn_80118874(void *texture, u32 flag);
    extern void GSmodelSetAnimIndex(u32 handle, u32 val);
    extern void GSmodelGetFrameCount(u32 handle, f32 *out, u32 flag);
    extern void GSmodelSetAnimFrame(u32 handle, f32 val);
    extern void GSmodelSetAnimType(u32 handle, u32 val);
    extern void GSmodelStartAnimation(u32 handle);

    u32 waitFrames;
    u32 elapsed;
    GSmaterialEntry *material;
    f32 frame;
    u32 handle2;
    u32 iconHandle;
    u32 iconHandle2;
    u32 b2;
    u32 tmpA;
    u32 tmpB;

    lbl_8047A690 = GSresGetResource(ctx, 0x0CE61602);
    lbl_8047A694 = GSresGetResource(ctx, 0x0CE61002);
    GSmodelSetShadowTextureSize(0x280, 0x1E0);
    fn_801CB834(0x0CE61000, 0, 0, 0);

    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    fn_801CB7C4(0x0CE61000);

    material = (GSmaterialEntry *)GSresGetResource(ctx, 0x0CE61000);
    fn_80118874(material->texture, 1);
    material->texture = NULL;

    frame = lbl_8047C1D4;
    handle2 = GSresGetResource(ctx, 0x0CE61004);
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelGetFrameCount(handle2, &frame, 0);
    frame = frame - lbl_8047C1D8;
    GSmodelSetAnimIndex(handle2, 0);
    GSmodelSetAnimFrame(handle2, frame);
    GSmodelSetAnimType(handle2, 0);
    GSmodelStartAnimation(handle2);

    iconHandle = fn_801CBA0C(0x06AF0400);
    iconHandle2 = fn_801CBA0C(0x0B720400);

    b2 = GSresGetResource(ctx, iconHandle);
    GSmodelSetShadowFlags(b2, 2);
    GSmodelSetShadowLight(b2, lbl_8047A690);
    GSmodelSetShadowSurface(b2, 1, &lbl_8047A694);

    b2 = GSresGetResource(ctx, iconHandle2);
    GSmodelSetShadowFlags(b2, 2);
    GSmodelSetShadowLight(b2, lbl_8047A690);
    GSmodelSetShadowSurface(b2, 1, &lbl_8047A694);

    GSmodelLinkToGSparticleBank(GSresGetResource(ctx, iconHandle), GSresGetResource(ctx, 0x11511400));
    GSmodelSetGSparticleLinkAttachMode(GSresGetResource(ctx, iconHandle), 4);

    cameraPlayAnime(ctx, 0x0D041800, 0, 0);
    waitFrames = 1;
    if (fn_800D37CC() == 0x32) {
        waitFrames = (u32)lbl_8047C1D0;
        if (waitFrames < 1) {
            waitFrames = 1;
        }
    }
    for (elapsed = 0; elapsed < waitFrames; ) {
        _threadSwitch();
        elapsed += fn_800D3088();
    }

    tmpA = fn_801CBA0C(0x0D0D1000);
    tmpB = fn_801CBA0C(0x0D0D1001);
    fn_801845E4(ctx, iconHandle, ctx, tmpA, 0);
    fn_801845E4(ctx, iconHandle2, ctx, tmpB, 0);
    fn_801CB834(iconHandle, 9, 0, 1);
    fn_801CB834(iconHandle2, 6, 0, 1);

    cameraWaitSyncAnime(1);
    fn_80190528(0x8D0);
    fn_800FF58C(1);
    floorSetFadeScript(0, 0);
}
/* 0x800934E4 | size: 0x90 */
s32 fn_800934E4(s32 channel)
{
#pragma peephole off
    s32 idle;
    u8* work;
    u32 slot;

    if (channel < 0 || channel > 3) {
        return 0;
    }

    slot = (u32)channel << 2;
    work = *(u8**)((u8*)lbl_803FB328 + slot);
    if (work != NULL) {
        fn_8009F7B4(work);
        idle = (*(u32*)(work + GBA_STATE_PHASE) == 0);
        fn_8009F890(work);
        fn_800A257C(work + GBA_DATA_OFFSET, GBA_THREAD_PRIORITY);
    } else {
        idle = 1;
    }

    return idle;
}

/* 0x80093574 | size: 0x9C */
u32 fn_80093574(s32 channel)
{
#pragma peephole off
    u32 status;
    u8* work;
    u32 slot;

    if (channel < 0 || channel > 3) {
        return 0x10000;
    }

    slot = (u32)channel << 2;
    work = *(u8**)((u8*)lbl_803FB328 + slot);
    if (work == NULL) {
        return 0;
    }

    while (1) {
        fn_8009F7B4(work);
        status = *(u32*)(work + GBA_STATE_TIMEOUT);
        fn_8009F890(work);
        fn_800A257C(work + GBA_DATA_OFFSET, GBA_THREAD_PRIORITY);
        if ((s32)(status >> 16) == 3) {
            _threadSwitch();
        } else {
            return status;
        }
    }
}

/* 0x80093610 | size: 0x88 */
u32 fn_80093610(s32 channel)
{
#pragma peephole off
    u32 status;
    u8* work;
    u32 slot;

    if (channel < 0 || channel > 3) {
        return 0x10000;
    }

    slot = (u32)channel << 2;
    work = *(u8**)((u8*)lbl_803FB328 + slot);
    if (work == NULL) {
        return 0;
    }

    fn_8009F7B4(work);
    status = *(u32*)(work + GBA_STATE_TIMEOUT);
    fn_8009F890(work);
    fn_800A257C(work + GBA_DATA_OFFSET, GBA_THREAD_PRIORITY);

    return status;
}

/* 0x80093698 | size: 0x15C */
s32 fn_80093698(s32 channel)
{
#pragma peephole off
    u32 slot;
    u32 status;
    u8* work;

    if (channel < 0 || channel > 3) {
        return 0;
    }

    slot = (u32)channel << 2;
    work = *(u8**)((u8*)lbl_803FB328 + slot);
    if (work == NULL) {
        return 1;
    }

    fn_800716E8(*(s32*)(work + GBA_STATE_PORT), 1);
    while (1) {
        fn_8009F7B4(work);
        status = *(u32*)(work + GBA_STATE_TIMEOUT);
        fn_8009F890(work);
        fn_800A257C(work + GBA_DATA_OFFSET, GBA_THREAD_PRIORITY);
        if ((s32)(status >> 16) == 3) {
            _threadSwitch();
        } else {
            break;
        }
    }

    fn_8009F7B4(work);
    *(u32*)(work + GBA_STATE_PHASE) = 0xD;
    *(u32*)(work + GBA_STATE_TIMEOUT) = 0x3000D;
    fn_8009F890(work);
    fn_800A257C(work + GBA_DATA_OFFSET, GBA_THREAD_PRIORITY);
    fn_8009FABC(work + 0x18);
    fn_800A1E54(work + GBA_DATA_OFFSET, 0);
    fn_800716C8(*(s32*)(work + GBA_STATE_PORT), NULL, NULL);
    fn_800716E8(*(s32*)(work + GBA_STATE_PORT), 0);

    status = fn_800E202C(*(u8**)((u8*)lbl_803FB328 + slot));
    if ((status & 0xFFFF) == 0) {
        __assert(lbl_8026F5A8, 0x1E6, &lbl_8047C1E8);
    }
    fn_800E24B0(status);
    fn_800E209C(status);
    *(u8**)((u8*)lbl_803FB328 + slot) = NULL;

    return 1;
}

/* 0x80093B04 | size: 0x48 */
void fn_80093B04(u32 a, u32 b) {
    u32 r31;
    u32 result;
    r31 = b;
    result = fn_800A13F8();
    if (r31 != 0) {
        if (r31 != result) return;
    }
    fn_800A257C((void*)result, 0x10);
    OSYieldThread();
    return;
}

/* 0x80093F2C | size: 0x38 */
#pragma push
#pragma scheduling off
void menuPokemonStatusCtrlRibbon(void) {
    extern void fn_80093F64();
    u8 *r4 = (u8*)&lbl_803FB380;
    u32 r3 = *(u32*)(r4 + 0xC);

    if (r3 != 0) {
        fn_80093F64(r3, r4 + 0x1c);
    }
    return;
}
#pragma pop

typedef struct GbaRibbonTableEntry {
    u16 status;
    s8 base;
    u8 max;
} GbaRibbonTableEntry;

extern GbaRibbonTableEntry lbl_802EEFD8[];
extern GbaRibbonTableEntry lbl_802EF000[];
extern s32 pokemonGetStatus(void* pokemon, u32 species, u32 status, u32 arg);

void fn_80093F64(void* pokemon, u8* out) {
    GbaRibbonTableEntry* entry;
    s32 value;
    s32 i;
    s32 j;
    s32 used;
    s32 count;

    for (i = 0; i < 9; i++) {
        out[4 + i * 4 + 0] = -1;
        out[4 + i * 4 + 1] = -1;
        out[4 + i * 4 + 2] = -1;
        out[4 + i * 4 + 3] = -1;
    }

    used = 0;
    entry = lbl_802EEFD8;
    for (i = 0; i < 10; i++, entry++) {
        value = pokemonGetStatus(pokemon, 0, entry->status, 0);
        if (value > entry->max) {
            value = entry->max;
        }
        for (j = 0; j < value; j++, used++) {
            out[4 + (used % 9) * 4 + used / 9] = (s8)(entry->base + j);
        }
    }

    used = 0;
    entry = lbl_802EF000;
    for (i = 0; i < 7; i++, entry++) {
        value = pokemonGetStatus(pokemon, 0, entry->status, 0);
        if (value > entry->max) {
            value = entry->max;
        }
        for (j = 0; j < value; j++, used++) {
            out[4 + used * 4 + 3] = (s8)(entry->base + j);
        }
    }

    count = 0;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 9; i++) {
            if ((s8)out[4 + i * 4 + j] >= 0) {
                count++;
            }
        }
    }
    *(u32*)out = count;
}

/* 0x80096C48 | size: 0x10C */
#pragma peephole off
void fn_80096C48(u32 unused, u8* dst) {
    typedef struct {
        f32 x;
        f32 y;
        f32 z;
    } ColorTriple;

    ColorTriple state0;
    ColorTriple state1;
    ColorTriple state2;
    register u8* out;
    register ColorTriple* triple;
    u8* obj;
    s32 state;

    out = dst;
    state0 = *(ColorTriple*)(lbl_8026F5C0 + 0x00);
    state1 = *(ColorTriple*)(lbl_8026F5C0 + 0x0C);
    state2 = *(ColorTriple*)(lbl_8026F5C0 + 0x18);

    obj = windowSearchID(0x53);
    if (obj == NULL) {
        return;
    }

    state = (s8)obj[0x95];
    switch (state) {
    case 0:
        triple = &state0;
        break;
    case 1:
        triple = &state1;
        break;
    case 2:
        triple = &state2;
        break;
    }

    out[0x64] = triple->x;
    out[0x65] = triple->y;
    out[0x66] = triple->z;
}
#pragma peephole on

/* 0x80097BBC | size: 0x114 */
#pragma peephole off
s32 fn_80097BBC(u8 chan) {
    extern void* savedataGetStatus();
    extern void* heroBiosGetPokemonPtr();
    extern int pokemonCheckValid();
    extern int fn_8010B560();
    void* entity;
    void* mgr;

    entity = NULL;
    if (chan < 6) {
        mgr = savedataGetStatus(0, 2);
        if (mgr != 0) {
            entity = heroBiosGetPokemonPtr(mgr, chan);
            if ((u8)pokemonCheckValid() == 0) {
                entity = NULL;
            }
        }
    }
    if (entity == 0) {
        return -1;
    }
    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0x11;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = (u32)entity;
    *(u16*)(lbl_803FB380 + 0x18) = 0;
    *(u32*)(lbl_803FB380 + 0x10) = 0;
    *(u32*)(lbl_803FB380 + 0x14) = 0;
    *(u32*)(lbl_803FB380 + 0x4) = -1;
    fn_800FF730(0x39d);
    if (lbl_803FB380[0] & 8) {
        floorSetFadeScript(0, 0);
    }
    _threadSwitch();
    return *(s32*)(lbl_803FB380 + 0x4);
}
#pragma peephole on
#pragma peephole reset

/* 0x800979EC | size: 0x4C */
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
#pragma scheduling off
void menuPokemonStatus(void) {
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    *(u32*)(lbl_803FB380 + 4) = fn_8009769C(
        lbl_803FB380[0],
        *(u32*)(lbl_803FB380 + 8),
        *(u32*)(lbl_803FB380 + 0xC),
        *(u16*)(lbl_803FB380 + 0x18),
        *(u32*)(lbl_803FB380 + 0x10),
        *(u32*)(lbl_803FB380 + 0x14));
}
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on
#pragma scheduling on

/* 0x80097FCC | size: 0x4 */
void fn_80097FCC(void) {
}

/* 0x80097FD0 | size: 0x28 */
void fn_80097FD0(void) {
    extern int fn_80113F48();
    GSresGetResource(fn_80113F48(), 0x12670000);
}

/* 0x80097FF8 | size: 0x4 */
void fn_80097FF8(void) {
}

/* 0x80097A38 | size: 0xCC */
s32 fn_80097A38(u32 arg0, u16 arg1) {
    extern int fn_8010B560();

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0x59;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = arg0;
    *(u16*)(lbl_803FB380 + 0x18) = arg1;
    *(u32*)(lbl_803FB380 + 0x10) = 0;
    *(u32*)(lbl_803FB380 + 0x14) = 0;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_800FF730(0x39d);
    if (lbl_803FB380[0] & 8) {
        floorSetFadeScript(0, 0);
    }
    _threadSwitch();
    return *(s32*)(lbl_803FB380 + 0x4);
}

/* 0x80097B04 | size: 0xB8 */
s32 fn_80097B04(u32 arg0, u16 arg1) {
    extern int fn_8010B560();
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0x58;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = arg0;
    *(u16*)(lbl_803FB380 + 0x18) = arg1;
    *(u32*)(lbl_803FB380 + 0x10) = 0;
    *(u32*)(lbl_803FB380 + 0x14) = 0;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_8009769C(lbl_803FB380[0], *(u32*)(lbl_803FB380 + 0x8), *(u32*)(lbl_803FB380 + 0xC),
                arg1, *(u32*)(lbl_803FB380 + 0x10), *(u32*)(lbl_803FB380 + 0x14));
    return *(s32*)(lbl_803FB380 + 0x4);
}

/* 0x80097CD0 | size: 0xC4 */
s32 fn_80097CD0(u32 arg0, u32 arg1, u32 arg2) {
    extern int fn_8010B560();
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0xc;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = arg0;
    *(u16*)(lbl_803FB380 + 0x18) = 0;
    *(u32*)(lbl_803FB380 + 0x10) = arg1;
    *(u32*)(lbl_803FB380 + 0x14) = arg2;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_8009769C(lbl_803FB380[0], *(u32*)(lbl_803FB380 + 0x8), *(u32*)(lbl_803FB380 + 0xC),
                *(u16*)(lbl_803FB380 + 0x18), *(u32*)(lbl_803FB380 + 0x10), *(u32*)(lbl_803FB380 + 0x14));
    return *(s32*)(lbl_803FB380 + 0x4);
}

/* 0x80097D94 | size: 0xC4 */
s32 fn_80097D94(u32 arg0, u32 arg1, u32 arg2) {
    extern int fn_8010B560();
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0xe;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = arg0;
    *(u16*)(lbl_803FB380 + 0x18) = 0;
    *(u32*)(lbl_803FB380 + 0x10) = arg1;
    *(u32*)(lbl_803FB380 + 0x14) = arg2;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_8009769C(lbl_803FB380[0], *(u32*)(lbl_803FB380 + 0x8), *(u32*)(lbl_803FB380 + 0xC),
                *(u16*)(lbl_803FB380 + 0x18), *(u32*)(lbl_803FB380 + 0x10), *(u32*)(lbl_803FB380 + 0x14));
    return *(s32*)(lbl_803FB380 + 0x4);
}

/* 0x80097E58 | size: 0xB0 */
s32 fn_80097E58(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    extern int fn_8010B560();
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0xac;
    *(u32*)(lbl_803FB380 + 0x8) = arg0;
    *(u32*)(lbl_803FB380 + 0xC) = arg1;
    *(u16*)(lbl_803FB380 + 0x18) = 0;
    *(u32*)(lbl_803FB380 + 0x10) = arg2;
    *(u32*)(lbl_803FB380 + 0x14) = arg3;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_8009769C(lbl_803FB380[0], *(u32*)(lbl_803FB380 + 0x8), *(u32*)(lbl_803FB380 + 0xC),
                *(u16*)(lbl_803FB380 + 0x18), *(u32*)(lbl_803FB380 + 0x10), *(u32*)(lbl_803FB380 + 0x14));
    return *(s32*)(lbl_803FB380 + 0x4);
}

/* 0x80097F08 | size: 0xC4 */
s32 fn_80097F08(u32 arg0, u32 arg1, u32 arg2) {
    extern int fn_8010B560();
    extern u32 fn_8009769C(u8, u32, u32, u16, u32, u32);

    while ((u8)fn_8010B560() != 0) {
        _threadSwitch();
    }
    memset(lbl_803FB380, 0, 0x44);
    *(u8*)(lbl_803FB380 + 0x0) = 0x8e;
    *(u32*)(lbl_803FB380 + 0x8) = 0;
    *(u32*)(lbl_803FB380 + 0xC) = arg0;
    *(u16*)(lbl_803FB380 + 0x18) = 0;
    *(u32*)(lbl_803FB380 + 0x10) = arg1;
    *(u32*)(lbl_803FB380 + 0x14) = arg2;
    *(s32*)(lbl_803FB380 + 0x4) = -1;
    fn_8009769C(lbl_803FB380[0], *(u32*)(lbl_803FB380 + 0x8), *(u32*)(lbl_803FB380 + 0xC),
                *(u16*)(lbl_803FB380 + 0x18), *(u32*)(lbl_803FB380 + 0x10), *(u32*)(lbl_803FB380 + 0x14));
    return *(s32*)(lbl_803FB380 + 0x4);
}
