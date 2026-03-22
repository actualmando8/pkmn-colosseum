/**
 * @file menu_rule.c
 * @brief Menu rule handlers (0x800767B8-0x80077A5C)
 *
 * Address range: 0x800767B8 - 0x80077A5C
 * Total functions: 6
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8006B420();
extern void fn_80076398();
extern void fn_8011E8DC();
extern void fn_8011F1A0();
extern void fn_8011F4A8();
extern void fn_8011F5C8();
extern void fn_80123FBC();
extern void fn_8012640C();
extern void fn_8012AC08();
extern void fn_80142984();
extern void fn_80196E10();

/* ===== SDA globals ===== */
extern u8 lbl_80478928;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268A48[];
extern u8 lbl_80268A58[];
extern u8 lbl_802EE458[];

/* ===== Forward declarations ===== */
s32 fn_800767B8(void);
s32 fn_80076A8C(void);
s32 fn_80076F2C(void);
s32 fn_800772AC(void);
s32 fn_800774D4(void);
s32 fn_800776E4(void);

/* ===== Function implementations ===== */

/* 0x800767B8 | size: 0x2D4 */
s32 fn_800767B8(void) {
    extern void fn_80076F2C();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r25 = r3;
    r26 = r4;
    r28 = 0x0;
    r24 = 0x0;
    do {
        r3 = r25;
        r4 = r26;
        r5 = r24;
        fn_80076F2C();
        r0 = r3 & 0xFF;
        if (r0 == 0x0) {
            r0 = 0x0;
            break;
        }
        r24 = r24 + 0x1;
    } while ((s32)r24 < 0x4);
    r0 = 0x1;

    r0 = r0 & 0xFF;
    if (r0 == 0x0) {
        r3 = 0x0;
        return;
    }
    r29 = 0x0;
    do {
        r3 = r25;
        r4 = r29 & 0xFFFF;
        ((void(*)(void))fn_8012AC08)();
        r27 = r3;
        if (r27 == 0x0) goto L_80076A60;
        ((void(*)(void))fn_80123FBC)();
        r0 = r3 & 0xFF;
        if (r0 == 0x0) goto L_80076A60;
        r0 = __cntlzw(r27);
        r30 = 0x0;
        r31 = (u32)r0 >> 5;
        do {
            r24 = 0x0;
            if ((s32)r31 == 0x0) {
                r3 = r27;
                r4 = 0x0;
                r5 = 0x6e;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 == 0x0) {
                }
                r24 = 0x1;
                }
            do {
                if ((s32)r24 != 0x0) {
                    r0 = 0x1;
                    break;
                }
                if ((s32)r30 != 0x1) {
                    if ((s32)r30 < 0x1) {
                        if ((s32)r30 < 0x0) {
                            goto L_80076A08;
                        }
                        if ((s32)r30 >= 0x3) goto L_80076A08;
                        goto L_80076908;
                        }
                    r3 = r27;
                    ((void(*)(void))fn_8011F4A8)();
                    r5 = r3 & 0xFF;
                    r0 = *(s16*)((u8*)r26 + 0x0);
                    r4 = (s32)r5 >> 31;
                    r3 = (u32)r0 >> 31;
                    r0 = r5 - r0;
                    r0 = r4 + r3; /* +carry */;
                    r0 = r0 & 0xFF;
                    break;
                }
                r3 = r27;
                ((void(*)(void))fn_8011F4A8)();
                r0 = *(s16*)((u8*)r26 + 0x2);
                r5 = r3 & 0xFF;
                r3 = (u32)r5 >> 31;
                r4 = (s32)r0 >> 31;
                r0 = r0 - r5;
                r0 = r4 + r3; /* +carry */;
                r0 = r0 & 0xFF;
                break;
            L_80076908: ;
                r3 = r27;
                ((void(*)(void))fn_8011F1A0)();
                r23 = r3;
                ((void(*)(void))fn_8006B420)();
                r0 = r23 & 0xFFFF;
                r24 = r3;
                do {
                    if ((s32)r0 != 0xaf) {
                        if ((s32)r0 >= 0xaf) goto L_80076948;
                        if ((s32)r0 != 0x0) {
                            goto L_80076948;
                        }
                        r3 = 0x1;
                        break;
                    }
                    r3 = 0x0;
                    break;
                L_80076948: ;
                    r3 = r23;
                    ((void(*)(void))fn_80142984)();
                } while (0);

                r0 = r3 & 0xFF;
                if (r0 == 0x0) {
                    r0 = 0x0;
                    break;
                }
                r0 = *(u32*)((u8*)r24 + 0x8);
                if ((s32)r0 != 0x1) {
                    if ((s32)r0 < 0x1) {
                        if ((s32)r0 < 0x0) {
                            goto L_80076A00;
                        }
                        if ((s32)r0 >= 0x3) goto L_80076A00;
                        goto L_800769A8;
                        }
                    r0 = 0x1;
                    break;
                }
                r0 = r23 & 0xFFFF;
                r0 = __cntlzw(r0);
                r0 = (u32)r0 >> 5;
                r0 = r0 & 0xFF;
                break;
            L_800769A8: ;
                r3 = (u32)&lbl_802EE458;
                r0 = *(u32*)&lbl_80478928;
                r5 = (u32)&lbl_802EE458;
                r4 = 0x0;
                r3 = r23 & 0xFFFF;
                ctr_fn = (void(*)(void))r0;
                if (r0 > 0x0) {
                    do {
                        r0 = *(u16*)((u8*)r5 + 0x0);
                        if (r3 == r0) {
                            r0 = r4 + 0x18;
                            r0 = *(u8*)(r24 + r0);
                            r0 = __cntlzw(r0);
                            r0 = (u32)r0 >> 5;
                            r0 = r0 & 0xFF;
                            break;
                        }
                        r5 = r5 + 0x2;
                        r4 = r4 + 0x1;
                    } while (--ctr != 0);
                }
                r0 = 0x1;
                break;
            L_80076A00: ;
                r0 = 0x0;
                break;
            L_80076A08: ;
                r3 = (u32)&lbl_80268A48;
                r5 = (u32)&lbl_80268A58;
                r3 = (u32)&lbl_80268A48;
                r4 = 0xfb;
                r5 = (u32)&lbl_80268A58;
                ((void(*)(void))fn_80196E10)();
                r0 = 0x0;
            } while (0);

            r0 = r0 & 0xFF;
            if (r0 == 0x0) {
                r0 = 0x0;
                break;
            }
            r30 = r30 + 0x1;
        } while ((s32)r30 < 0x3);
        r0 = 0x1;

        r0 = r0 & 0xFF;
        if (r0 == 0x0) {
            r3 = 0x0;
            return;
        }
        r28 = r28 + 0x1;
    L_80076A60: ;
        r29 = r29 + 0x1;
    } while ((s32)r29 < 0x6);
    r0 = -r28;
    r0 = r0 & ~r28;
    r3 = (u32)r0 >> 31;

    return;
}

/* 0x80076A8C | size: 0x4A0 */
s32 fn_80076A8C(void) {
    extern void fn_80076F2C();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r4;
    if ((s32)r6 != 2) {
        if ((s32)r6 < 2) {
            if ((s32)r6 != 0) {
                if ((s32)r6 < 0) {
                    r3 = 0x0;
                    return;
                }
                if ((s32)r6 >= 4) { r3 = 0x0; return; }

                } else {
                r4 = r5;
                r5 = r6;
                fn_80076F2C();
                return;
                    }
            tmp = *(u8*)((u8*)r5 + 0xC);
            if (tmp != 0) {
                r3 = 0x1;
                return;
            }
            tmp = __cntlzw(r31);
            r29 = 0x0;
            r28 = (u32)tmp >> 5;
            if ((s32)r28 == 0) {
                r3 = r31;
                r4 = 0x0;
                r5 = 0x6e;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 == 0) {
                }
                r29 = 0x1;
                }
            if ((s32)r29 != 0) { r3 = 0x1; return; }
            r28 = 0x0;
            if ((s32)r28 == 0) {
                r3 = r31;
                r4 = 0x0;
                r5 = 0x6e;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 == 0) {
                }
                r28 = 0x1;
                }
            if ((s32)r28 != 0) {
                tmp = 0x0;

            } else {
                r3 = r31;
                r28 = 0x0;
                ((void(*)(void))fn_8011E8DC)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    r3 = r31;
                    ((void(*)(void))fn_80123FBC)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                    }
                    r28 = 0x1;
                    }
                tmp = r28 & 0xFF;
            }
            tmp = tmp & 0xFF;
            if (tmp != 0) {

                r3 = 0x1;
                return;
            }
            r27 = 0x0;
            do {
                r3 = r30;
                r4 = r27 & 0xFFFF;
                ((void(*)(void))fn_8012AC08)();
                r26 = r3;
                do {
                if (r26 == r31) break;
                    tmp = __cntlzw(r26);
                    r29 = 0x0;
                    r28 = (u32)tmp >> 5;
                    if ((s32)r28 == 0) {
                        r4 = 0x0;
                        r5 = 0x6e;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        if ((s32)r3 == 0) {
                        }
                        r29 = 0x1;
                        }
                    if ((s32)r29 != 0) break;
                    r28 = 0x0;
                    if ((s32)r28 == 0) {
                        r3 = r26;
                        r4 = 0x0;
                        r5 = 0x6e;
                        r6 = 0x0;
                        ((void(*)(void))fn_8012640C)();
                        if ((s32)r3 == 0) {
                        }
                        r28 = 0x1;
                        }
                    if ((s32)r28 != 0) {
                        tmp = 0x0;

                    } else {
                        r3 = r26;
                        r28 = 0x0;
                        ((void(*)(void))fn_8011E8DC)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                            r3 = r26;
                            ((void(*)(void))fn_80123FBC)();
                            tmp = r3 & 0xFF;
                            if (tmp == 0) {
                            }
                            r28 = 0x1;
                            }
                        tmp = r28 & 0xFF;
                    }
                    tmp = tmp & 0xFF;
                    if (tmp != 0) break;
                    r3 = r31;
                    ((void(*)(void))fn_8011F5C8)();
                    r28 = r3 & 0xFFFF;
                    r3 = r26;
                    ((void(*)(void))fn_8011F5C8)();
                    tmp = r3 & 0xFFFF;
                    if (tmp != r28) break;
                    r3 = 0x0;
                    return;
                } while (0);

                r27 = r27 + 0x1;
            } while ((s32)r27 < 6);
            r3 = 0x1;
            return;
        }
        tmp = *(u8*)((u8*)r5 + 0xD);
        if (tmp != 0) {
            r3 = 0x1;
            return;
        }
        tmp = __cntlzw(r31);
        r29 = 0x0;
        r28 = (u32)tmp >> 5;
        if ((s32)r28 == 0) {
            r3 = r31;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r29 = 0x1;
            }
        if ((s32)r29 != 0) { r3 = 0x1; return; }
        r28 = 0x0;
        if ((s32)r28 == 0) {
            r3 = r31;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r28 = 0x1;
            }
        if ((s32)r28 != 0) {
            tmp = 0x0;

        } else {
            r3 = r31;
            r28 = 0x0;
            ((void(*)(void))fn_8011E8DC)();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                r3 = r31;
                ((void(*)(void))fn_80123FBC)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                }
                r28 = 0x1;
                }
            tmp = r28 & 0xFF;
        }
        tmp = tmp & 0xFF;
        if (tmp != 0) {

            r3 = 0x1;
            return;
        }
        r3 = r31;
        ((void(*)(void))fn_8011F1A0)();
        tmp = r3 & 0xFFFF;
        if (tmp == 0) {
            r3 = 0x1;
            return;
        }
        r27 = 0x0;
        do {
            r3 = r30;
            r4 = r27 & 0xFFFF;
            ((void(*)(void))fn_8012AC08)();
            r26 = r3;
            do {
            if (r26 == r31) break;
                tmp = __cntlzw(r26);
                r28 = 0x0;
                r29 = (u32)tmp >> 5;
                if ((s32)r29 == 0) {
                    r4 = 0x0;
                    r5 = 0x6e;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((s32)r3 == 0) {
                    }
                    r28 = 0x1;
                    }
                if ((s32)r28 != 0) break;
                r29 = 0x0;
                if ((s32)r29 == 0) {
                    r3 = r26;
                    r4 = 0x0;
                    r5 = 0x6e;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((s32)r3 == 0) {
                    }
                    r29 = 0x1;
                    }
                if ((s32)r29 != 0) {
                    tmp = 0x0;

                } else {
                    r3 = r26;
                    r29 = 0x0;
                    ((void(*)(void))fn_8011E8DC)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        r3 = r26;
                        ((void(*)(void))fn_80123FBC)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                        }
                        r29 = 0x1;
                        }
                    tmp = r29 & 0xFF;
                }
                tmp = tmp & 0xFF;
                if (tmp != 0) break;
                r3 = r31;
                ((void(*)(void))fn_8011F1A0)();
                r29 = r3 & 0xFFFF;
                r3 = r26;
                ((void(*)(void))fn_8011F1A0)();
                tmp = r3 & 0xFFFF;
                if (tmp != r29) break;
                r3 = 0x0;
                return;
            } while (0);

            r27 = r27 + 0x1;
        } while ((s32)r27 < 6);
        r3 = 0x1;
        return;
                }
    r4 = r5;
    r29 = 0x0;
    r5 = 0x3;
    fn_80076F2C();
    tmp = r3 & 0xFF;
    if (tmp == 0) {
        r30 = 0x0;
        if (r31 != 0) {
            r3 = r31;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r30 = 0x1;
            }
        if ((s32)r30 != 0) { r3 = r29 & 0xFF; return; }
    }
    r29 = 0x1;

    r3 = r29 & 0xFF;
    return;

    r3 = 0x0;

    return;
}

/* 0x80076F2C | size: 0x380 */
s32 fn_80076F2C(void) {
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r20 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r22 = r3;
    r23 = r4;
    r24 = r5;
    r31 = 0x0;
    r30 = 0x1;
    r29 = 0x1;
    r28 = 0x0;
    do {
        r3 = r22;
        r4 = r28 & 0xFFFF;
        ((void(*)(void))fn_8012AC08)();
        r26 = r3;
        r21 = 0x0;
        tmp = __cntlzw(r26);
        r20 = (u32)tmp >> 5;
        if ((s32)r20 == 0) {
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r21 = 0x1;
            }
        if ((s32)r21 != 0) goto L_80077178;
        r20 = 0x0;
        if ((s32)r20 == 0) {
            r3 = r26;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r20 = 0x1;
            }
        if ((s32)r20 != 0) {
            tmp = 0x0;

        } else {
            r3 = r26;
            r20 = 0x0;
            ((void(*)(void))fn_8011E8DC)();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                r3 = r26;
                ((void(*)(void))fn_80123FBC)();
                tmp = r3 & 0xFF;
                if (tmp == 0) {
                }
                r20 = 0x1;
                }
            tmp = r20 & 0xFF;
        }
        tmp = tmp & 0xFF;
        if (tmp != 0) goto L_80077178;
        r3 = r26;
        ((void(*)(void))fn_8011F4A8)();
        tmp = r3 & 0xFF;
        r27 = r28 + 0x1;
        r31 = r31 + tmp;
        while (1) {
            if (r27 >= 6) break;
            r3 = r22;
            r4 = r27 & 0xFFFF;
            ((void(*)(void))fn_8012AC08)();
            r25 = r3;
            r20 = 0x0;
            tmp = __cntlzw(r25);
            r21 = (u32)tmp >> 5;
            if ((s32)r21 == 0) {
                r4 = 0x0;
                r5 = 0x6e;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 == 0) {
                }
                r20 = 0x1;
                }
            do {
                if ((s32)r20 != 0) break;
                r21 = 0x0;
                if ((s32)r21 == 0) {
                    r3 = r25;
                    r4 = 0x0;
                    r5 = 0x6e;
                    r6 = 0x0;
                    ((void(*)(void))fn_8012640C)();
                    if ((s32)r3 == 0) {
                    }
                    r21 = 0x1;
                    }
                if ((s32)r21 != 0) {
                    tmp = 0x0;

                } else {
                    r3 = r25;
                    r21 = 0x0;
                    ((void(*)(void))fn_8011E8DC)();
                    tmp = r3 & 0xFF;
                    if (tmp == 0) {
                        r3 = r25;
                        ((void(*)(void))fn_80123FBC)();
                        tmp = r3 & 0xFF;
                        if (tmp == 0) {
                        }
                        r21 = 0x1;
                        }
                    tmp = r21 & 0xFF;
                }
                tmp = tmp & 0xFF;
                if (tmp != 0) break;
                r3 = r26;
                ((void(*)(void))fn_8011F1A0)();
                tmp = r3 & 0xFFFF;
                if (tmp != 0) {
                    r3 = r25;
                    ((void(*)(void))fn_8011F1A0)();
                    r21 = r3 & 0xFFFF;
                    r3 = r26;
                    ((void(*)(void))fn_8011F1A0)();
                    tmp = r3 & 0xFFFF;
                    r3 = r21 - tmp;
                    tmp = tmp - r21;
                    tmp = r3 | tmp;
                    tmp = (u32)tmp >> 31;
                    tmp = r29 & tmp;
                    r29 = tmp & 0xFF;
                }
                r3 = r25;
                ((void(*)(void))fn_8011F5C8)();
                r25 = r3 & 0xFFFF;
                r3 = r26;
                ((void(*)(void))fn_8011F5C8)();
                tmp = r3 & 0xFFFF;
                r3 = r25 - tmp;
                tmp = tmp - r25;
                tmp = r3 | tmp;
                tmp = (u32)tmp >> 31;
                tmp = r30 & tmp;
                r30 = tmp & 0xFF;
            } while (0);

            r27 = r27 + 0x1;

        }
    L_80077178:
        r28 = r28 + 0x1;
    } while (r28 < 6);
    if ((s32)r24 != 2) {
        if ((s32)r24 < 2) {
            if ((s32)r24 != 0) {
                if ((s32)r24 < 0) {
                    r3 = 0x0;
                    return;
                }
                if ((s32)r24 >= 4) { r3 = 0x0; return; }

                } else {
                tmp = *(s16*)((u8*)r23 + 0x4);
                r3 = (u32)r31 >> 31;
                r4 = (s32)tmp >> 31;
                tmp = tmp - r31;
                tmp = r4 + r3; /* +carry */;
                r3 = tmp & 0xFF;
                return;
                    }
            tmp = *(u8*)((u8*)r23 + 0xC);
            r3 = 0x0;
            if (tmp == 0) {
                tmp = r30 & 0xFF;
                if (tmp == 0) { r3 = r3 & 0xFF; return; }
            }
            r3 = 0x1;

            r3 = r3 & 0xFF;
            return;
        }
        tmp = *(u8*)((u8*)r23 + 0xD);
        r3 = 0x0;
        if (tmp == 0) {
            tmp = r29 & 0xFF;
            if (tmp == 0) { r3 = r3 & 0xFF; return; }
        }
        r3 = 0x1;

        r3 = r3 & 0xFF;
        return;
                }
    r20 = 0x0;
    r21 = 0x0;
    while (1) {
        tmp = r21 & 0xFFFF;
        if (tmp >= 6) break;
        r3 = r22;
        r4 = r21;
        ((void(*)(void))fn_8012AC08)();
        r24 = 0x0;
        if (r3 != 0) {
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r24 = 0x1;
            }
        if ((s32)r24 == 0) {
            r20 = r20 + 0x1;
        }
        r21 = r21 + 0x1;

    }
    tmp = r20 & 0xFFFF;
    r5 = *(s16*)((u8*)r23 + 0x6);
    r4 = (s32)tmp >> 31;
    r3 = (u32)r5 >> 31;
    tmp = tmp - r5;
    tmp = r4 + r3; /* +carry */;
    r3 = tmp & 0xFF;
    return;

    r3 = 0x0;

    return;
}

/* 0x800772AC | size: 0x228 */
s32 fn_800772AC(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    tmp = __cntlzw(r28);
    r30 = 0x0;
    r31 = (u32)tmp >> 5;
    do {
        r27 = 0x0;
        if ((s32)r31 == 0) {
            r3 = r28;
            r4 = 0x0;
            r5 = 0x6e;
            r6 = 0x0;
            ((void(*)(void))fn_8012640C)();
            if ((s32)r3 == 0) {
            }
            r27 = 0x1;
            }
        do {
            if ((s32)r27 != 0) {
                tmp = 0x1;
                break;
            }
            if ((s32)r30 != 1) {
                if ((s32)r30 < 1) {
                    if ((s32)r30 < 0) {
                        goto L_80077480;
                    }
                    if ((s32)r30 >= 3) goto L_80077480;
                    goto L_80077380;
                    }
                r3 = r28;
                ((void(*)(void))fn_8011F4A8)();
                r5 = r3 & 0xFF;
                tmp = *(s16*)((u8*)r29 + 0x0);
                r4 = (s32)r5 >> 31;
                r3 = (u32)tmp >> 31;
                tmp = r5 - tmp;
                tmp = r4 + r3; /* +carry */;
                tmp = tmp & 0xFF;
                break;
            }
            r3 = r28;
            ((void(*)(void))fn_8011F4A8)();
            tmp = *(s16*)((u8*)r29 + 0x2);
            r5 = r3 & 0xFF;
            r3 = (u32)r5 >> 31;
            r4 = (s32)tmp >> 31;
            tmp = tmp - r5;
            tmp = r4 + r3; /* +carry */;
            tmp = tmp & 0xFF;
            break;
        L_80077380:
            r3 = r28;
            ((void(*)(void))fn_8011F1A0)();
            r26 = r3;
            ((void(*)(void))fn_8006B420)();
            tmp = r26 & 0xFFFF;
            r27 = r3;
            do {
                if ((s32)tmp != 0xaf) {
                    if ((s32)tmp >= 0xaf) goto L_800773C0;
                    if ((s32)tmp != 0) {
                        goto L_800773C0;
                    }
                    r3 = 0x1;
                    break;
                }
                r3 = 0x0;
                break;
            L_800773C0:
                r3 = r26;
                ((void(*)(void))fn_80142984)();
            } while (0);

            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = 0x0;
                break;
            }
            tmp = *(u32*)((u8*)r27 + 0x8);
            if ((s32)tmp != 1) {
                if ((s32)tmp < 1) {
                    if ((s32)tmp < 0) {
                        goto L_80077478;
                    }
                    if ((s32)tmp >= 3) goto L_80077478;
                    goto L_80077420;
                    }
                tmp = 0x1;
                break;
            }
            tmp = r26 & 0xFFFF;
            tmp = __cntlzw(tmp);
            tmp = (u32)tmp >> 5;
            tmp = tmp & 0xFF;
            break;
        L_80077420:
            r3 = (u32)&lbl_802EE458;
            tmp = *(u32*)&lbl_80478928;
            r5 = (u32)&lbl_802EE458;
            r4 = 0x0;
            r3 = r26 & 0xFFFF;
            ctr_fn = (void(*)(void))tmp;
            if (tmp > 0) {
                do {
                    tmp = *(u16*)((u8*)r5 + 0x0);
                    if (r3 == tmp) {
                        tmp = r4 + 0x18;
                        tmp = *(u8*)(r27 + tmp);
                        tmp = __cntlzw(tmp);
                        tmp = (u32)tmp >> 5;
                        tmp = tmp & 0xFF;
                        break;
                    }
                    r5 = r5 + 0x2;
                    r4 = r4 + 0x1;
                } while (--ctr != 0);
            }
            tmp = 0x1;
            break;
        L_80077478:
            tmp = 0x0;
            break;
        L_80077480:
            r3 = (u32)&lbl_80268A48;
            r5 = (u32)&lbl_80268A58;
            r3 = (u32)&lbl_80268A48;
            r4 = 0xfb;
            r5 = (u32)&lbl_80268A58;
            ((void(*)(void))fn_80196E10)();
            tmp = 0x0;
        } while (0);

        tmp = tmp & 0xFF;
        if (tmp == 0) {
            r3 = 0x0;
            return;
        }
        r30 = r30 + 0x1;
    } while ((s32)r30 < 3);
    r3 = 0x1;

    return;
}

/* 0x800774D4 | size: 0x210 */
s32 fn_800774D4(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r28 = r3;
    r29 = r4;
    r30 = r5;
    r31 = 0x0;
    if (r28 != 0) {
        r4 = 0x0;
        r5 = 0x6e;
        r6 = 0x0;
        ((void(*)(void))fn_8012640C)();
        if ((s32)r3 == 0) {
        }
        r31 = 0x1;
        }
    if ((s32)r31 != 0) {
        r3 = 0x1;
        return;
    }
    if ((s32)r30 != 1) {
        if ((s32)r30 < 1) {
            if ((s32)r30 < 0) {
                goto L_800776A8;
            }
            if ((s32)r30 >= 3) goto L_800776A8;

            } else {
            r3 = r28;
            ((void(*)(void))fn_8011F4A8)();
            r5 = r3 & 0xFF;
            tmp = *(s16*)((u8*)r29 + 0x0);
            r4 = (s32)r5 >> 31;
            r3 = (u32)tmp >> 31;
            tmp = r5 - tmp;
            tmp = r4 + r3; /* +carry */;
            r3 = tmp & 0xFF;
            return;
        }
        r3 = r28;
        ((void(*)(void))fn_8011F4A8)();
        tmp = *(s16*)((u8*)r29 + 0x2);
        r5 = r3 & 0xFF;
        r3 = (u32)r5 >> 31;
        r4 = (s32)tmp >> 31;
        tmp = tmp - r5;
        tmp = r4 + r3; /* +carry */;
        r3 = tmp & 0xFF;
        return;
            }
    r3 = r28;
    ((void(*)(void))fn_8011F1A0)();
    r30 = r3;
    ((void(*)(void))fn_8006B420)();
    tmp = r30 & 0xFFFF;
    r31 = r3;
    do {
        if ((s32)tmp != 0xaf) {
            if ((s32)tmp >= 0xaf) goto L_800775E8;
            if ((s32)tmp != 0) {
                goto L_800775E8;
            }
            r3 = 0x1;
            break;
        }
        r3 = 0x0;
        break;
    L_800775E8:
        r3 = r30;
        ((void(*)(void))fn_80142984)();
    } while (0);

    tmp = r3 & 0xFF;
    if (tmp == 0) {
        r3 = 0x0;
        return;
    }
    tmp = *(u32*)((u8*)r31 + 0x8);
    if ((s32)tmp != 1) {
        if ((s32)tmp < 1) {
            if ((s32)tmp >= 0) { r3 = 0x1; return; }
            r3 = 0x0;
            return;
        }
        if ((s32)tmp >= 3) { r3 = 0x0; return; }
        goto L_80077648;

        r3 = 0x1;
        return;
    }
    tmp = r30 & 0xFFFF;
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
    r3 = tmp & 0xFF;
    return;
L_80077648:
    r3 = (u32)&lbl_802EE458;
    tmp = *(u32*)&lbl_80478928;
    r5 = (u32)&lbl_802EE458;
    r4 = 0x0;
    r3 = r30 & 0xFFFF;
    ctr_fn = (void(*)(void))tmp;
    if (tmp > 0) {
        do {
            tmp = *(u16*)((u8*)r5 + 0x0);
            if (r3 == tmp) {
                r3 = r31 + r4;
                tmp = *(u8*)((u8*)r3 + 0x18);
                tmp = __cntlzw(tmp);
                tmp = (u32)tmp >> 5;
                r3 = tmp & 0xFF;
                return;
            }
            r5 = r5 + 0x2;
            r4 = r4 + 0x1;
        } while (--ctr != 0);
    }
    r3 = 0x1;
    return;

    r3 = 0x0;
    return;
L_800776A8:
    r3 = (u32)&lbl_80268A48;
    r5 = (u32)&lbl_80268A58;
    r3 = (u32)&lbl_80268A48;
    r4 = 0xfb;
    r5 = (u32)&lbl_80268A58;
    ((void(*)(void))fn_80196E10)();
    r3 = 0x0;

    return;
}

/* 0x800776E4 | size: 0x378 */
s32 fn_800776E4(void) {
    extern void fn_80076F2C();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r25 = r3;
    r26 = 0x0;
    while (1) {
        tmp = r26 & 0xFFFF;
        if (tmp >= 6) break;
        r3 = r25;
        r4 = r26;
        ((void(*)(void))fn_8012AC08)();
        r27 = 0x0;
        r24 = r3;
        do {
            r3 = r24;
            r4 = r27;
            ((void(*)(void))fn_80076398)();
            tmp = r3 & 0xFF;
            if (tmp == 0) {
                tmp = 0x0;
                break;
            }
            r27 = r27 + 0x1;
        } while ((s32)r27 < 6);
        tmp = 0x1;

        tmp = tmp & 0xFF;
        if (tmp == 0) {
            tmp = 0x0;
            break;
        }
        r26 = r26 + 0x1;

    }
    tmp = 0x1;

    r3 = tmp & 0xFF;
    tmp = -r3;
    tmp = tmp | r3;
    tmp = (u32)tmp >> 31;
    if ((s32)tmp == 0) { r3 = tmp & 0xFF; return; }
    ((void(*)(void))fn_8006B420)();
    r29 = 0x0;
    r27 = r3;
    r24 = r29;
    do {
        r3 = r25;
        r4 = r27;
        r5 = r24;
        fn_80076F2C();
        tmp = r3 & 0xFF;
        if (tmp == 0) {
            tmp = 0x0;
            break;
        }
        r24 = r24 + 0x1;
    } while ((s32)r24 < 4);
    tmp = 0x1;

    tmp = tmp & 0xFF;
    if (tmp == 0) {
        tmp = 0x0;
        goto L_80077A34;
    }
    r28 = 0x0;
    do {
        r3 = r25;
        r4 = r28 & 0xFFFF;
        ((void(*)(void))fn_8012AC08)();
        r30 = r3;
        if (r30 == 0) goto L_80077A1C;
        ((void(*)(void))fn_80123FBC)();
        tmp = r3 & 0xFF;
        if (tmp == 0) goto L_80077A1C;
        tmp = __cntlzw(r30);
        r26 = 0x0;
        r31 = (u32)tmp >> 5;
        do {
            r24 = 0x0;
            if ((s32)r31 == 0) {
                r3 = r30;
                r4 = 0x0;
                r5 = 0x6e;
                r6 = 0x0;
                ((void(*)(void))fn_8012640C)();
                if ((s32)r3 == 0) {
                }
                r24 = 0x1;
                }
            do {
                if ((s32)r24 != 0) {
                    tmp = 0x1;
                    break;
                }
                if ((s32)r26 != 1) {
                    if ((s32)r26 < 1) {
                        if ((s32)r26 < 0) {
                            goto L_800779C4;
                        }
                        if ((s32)r26 >= 3) goto L_800779C4;
                        goto L_800778C4;
                        }
                    r3 = r30;
                    ((void(*)(void))fn_8011F4A8)();
                    r5 = r3 & 0xFF;
                    tmp = *(s16*)((u8*)r27 + 0x0);
                    r4 = (s32)r5 >> 31;
                    r3 = (u32)tmp >> 31;
                    tmp = r5 - tmp;
                    tmp = r4 + r3; /* +carry */;
                    tmp = tmp & 0xFF;
                    break;
                }
                r3 = r30;
                ((void(*)(void))fn_8011F4A8)();
                tmp = *(s16*)((u8*)r27 + 0x2);
                r5 = r3 & 0xFF;
                r3 = (u32)r5 >> 31;
                r4 = (s32)tmp >> 31;
                tmp = tmp - r5;
                tmp = r4 + r3; /* +carry */;
                tmp = tmp & 0xFF;
                break;
            L_800778C4:
                r3 = r30;
                ((void(*)(void))fn_8011F1A0)();
                r23 = r3;
                ((void(*)(void))fn_8006B420)();
                tmp = r23 & 0xFFFF;
                r24 = r3;
                do {
                    if ((s32)tmp != 0xaf) {
                        if ((s32)tmp >= 0xaf) goto L_80077904;
                        if ((s32)tmp != 0) {
                            goto L_80077904;
                        }
                        r3 = 0x1;
                        break;
                    }
                    r3 = 0x0;
                    break;
                L_80077904:
                    r3 = r23;
                    ((void(*)(void))fn_80142984)();
                } while (0);

                tmp = r3 & 0xFF;
                if (tmp == 0) {
                    tmp = 0x0;
                    break;
                }
                tmp = *(u32*)((u8*)r24 + 0x8);
                if ((s32)tmp != 1) {
                    if ((s32)tmp < 1) {
                        if ((s32)tmp < 0) {
                            goto L_800779BC;
                        }
                        if ((s32)tmp >= 3) goto L_800779BC;
                        goto L_80077964;
                        }
                    tmp = 0x1;
                    break;
                }
                tmp = r23 & 0xFFFF;
                tmp = __cntlzw(tmp);
                tmp = (u32)tmp >> 5;
                tmp = tmp & 0xFF;
                break;
            L_80077964:
                r3 = (u32)&lbl_802EE458;
                tmp = *(u32*)&lbl_80478928;
                r5 = (u32)&lbl_802EE458;
                r4 = 0x0;
                r3 = r23 & 0xFFFF;
                ctr_fn = (void(*)(void))tmp;
                if (tmp > 0) {
                    do {
                        tmp = *(u16*)((u8*)r5 + 0x0);
                        if (r3 == tmp) {
                            tmp = r4 + 0x18;
                            tmp = *(u8*)(r24 + tmp);
                            tmp = __cntlzw(tmp);
                            tmp = (u32)tmp >> 5;
                            tmp = tmp & 0xFF;
                            break;
                        }
                        r5 = r5 + 0x2;
                        r4 = r4 + 0x1;
                    } while (--ctr != 0);
                }
                tmp = 0x1;
                break;
            L_800779BC:
                tmp = 0x0;
                break;
            L_800779C4:
                r3 = (u32)&lbl_80268A48;
                r5 = (u32)&lbl_80268A58;
                r3 = (u32)&lbl_80268A48;
                r4 = 0xfb;
                r5 = (u32)&lbl_80268A58;
                ((void(*)(void))fn_80196E10)();
                tmp = 0x0;
            } while (0);

            tmp = tmp & 0xFF;
            if (tmp == 0) {
                tmp = 0x0;
                break;
            }
            r26 = r26 + 0x1;
        } while ((s32)r26 < 3);
        tmp = 0x1;

        tmp = tmp & 0xFF;
        if (tmp == 0) {
            tmp = 0x0;
            break;
        }
        r29 = r29 + 0x1;
    L_80077A1C:
        r28 = r28 + 0x1;
    } while ((s32)r28 < 6);
    tmp = -r29;
    tmp = tmp & ~r29;
    tmp = (u32)tmp >> 31;
L_80077A34:
    r3 = tmp & 0xFF;
    tmp = -r3;
    tmp = tmp | r3;
    tmp = (u32)tmp >> 31;

    r3 = tmp & 0xFF;
    return;
}
