
/* Forward declarations for converted functions */
void fn_80199AF8(void);

/**
 * @file hsd_state.c
 * @brief HSD internal functions (0x80199A84-0x8019B490).
 *
 * Stub coverage for 4 functions.
 */

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

/* 0x80199A84 | 0x4 */
void fn_80199A84(void) {
}

/* 0x70 | fn_80199A88 | generic */
void fn_80199A88(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6) {
    fn_80199AF8();
}

/* 0x80199AF8 | 0x754 */
void fn_80199AF8(void) {
    extern u8 lbl_80274764[];
    extern u8 lbl_8047DA30[];
    extern u8 lbl_8047DA3C[];
    extern u8 lbl_8047DA40[];
    extern u8 lbl_8047DA48[];
    extern u8 lbl_8047DA50[];
    extern void fn_80196E10();
    extern void fn_8019A24C();
    extern void fn_801B2560();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r29, r3 */;
    f31 = *(f32*)lbl_8047DA3C;
    r30 = r4;
    r31 = r5;
    if ((s32)tmp != 0) {
        if (r29 == 0) {
            r3 = 0x0;
            goto L_80199B4C;
        }
        tmp = *(u8*)((u8*)r29 + 0x10);
        r3 = tmp & 0xF;
        goto L_80199B4C;
    }
    r3 = 0x0;
L_80199B4C:
    if (r3 == 0) return;
    f2 = *(f32*)((u8*)r29 + 0x1C);
    f0 = *(f64*)lbl_8047DA40;
    f1 = f2 + f1;
    *(f32*)((u8*)r29 + 0x1C) = f1;
    f1 = *(f32*)((u8*)r29 + 0x1C);
    if (f1 < f0) return;
while (1) {
        if ((s32)r3 != 4) {
            if ((s32)r3 < 4) {
                if ((s32)r3 == 0) return;
                if ((s32)r3 < 0) continue;
                if ((s32)r3 < 3) {
                    goto L_80199D74;
                }
                if ((s32)r3 != 6) {
                    if ((s32)r3 >= 6) continue;
                    goto L_8019A208;
                }
                f0 = *(f32*)((u8*)r29 + 0x1C);
                f0 = f0 + f31;
                *(f32*)((u8*)r29 + 0x1C) = f0;
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0x00000040;
                if ((s32)r3 != 6) {
                    tmp = *(u8*)((u8*)r29 + 0x11);
                    *(u8*)((u8*)r29 + 0x12) = tmp;
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp & 0xFFFFFFBF;
                    *(u8*)((u8*)r29 + 0x10) = tmp;
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp | 0x80;
                    *(u8*)((u8*)r29 + 0x10) = tmp;
                    f0 = *(f32*)((u8*)r29 + 0x24);
                    *(f32*)((u8*)r29 + 0x20) = f0;
                }
                if (r31 == 0) return;
                tmp = *(u8*)((u8*)r29 + 0x12);
                if ((s32)tmp != 2) {
                    if ((s32)tmp < 2) {
                        if ((s32)tmp < 1) {
                            goto L_80199D58;
                        }
                        if ((s32)tmp != 6) {
                            if ((s32)tmp >= 6) goto L_80199D58;
                            goto L_80199CFC;
                        }
                        tmp = *(u8*)((u8*)r29 + 0x10);
                        tmp = tmp & 0x00000080;
                        if ((s32)tmp == 6) return;
                        f0 = *(f32*)((u8*)r29 + 0x20);
                        *(f32*)(sp + 0x20) = f0;
                        tmp = *(u8*)((u8*)r29 + 0x10);
                        tmp = tmp & 0x7F;
                        *(u8*)((u8*)r29 + 0x10) = tmp;
                        goto L_80199D58;
                        }
                    r3 = *(u16*)((u8*)r29 + 0x1A);
                    tmp = 0x43300000;
                    *(u32*)(sp + 0x30) = tmp;
                    f1 = *(f64*)lbl_8047DA50;
                    f2 = *(f32*)((u8*)r29 + 0x1C);
                    f0 = f0 - f1;
                    /* cror eq, gt, eq */;
                    if (f2 == f0) {
                        f0 = *(f32*)((u8*)r29 + 0x24);
                    } else {

                        f0 = *(f32*)((u8*)r29 + 0x20);
                    }
                    *(f32*)(sp + 0x20) = f0;
                    goto L_80199D58;
                }
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0x00000020;
                if (f2 != f0) {
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp & 0xFFFFFFDF;
                    *(u8*)((u8*)r29 + 0x10) = tmp;
                    tmp = *(u16*)((u8*)r29 + 0x1A);
                    if (tmp != 0) {
                        r3 = *(u16*)((u8*)r29 + 0x1A);
                        tmp = 0x43300000;
                        *(u32*)(sp + 0x30) = tmp;
                        f3 = *(f32*)((u8*)r29 + 0x24);
                        f2 = *(f32*)((u8*)r29 + 0x20);
                        f1 = *(f64*)lbl_8047DA50;
                        f2 = f3 - f2;
                        f0 = f0 - f1;
                        f0 = f2 / f0;
                        *(f32*)((u8*)r29 + 0x28) = f0;
                        goto L_80199CE4;
                    }
                    f0 = *(f32*)lbl_8047DA3C;
                    *(f32*)((u8*)r29 + 0x28) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x24);
                    *(f32*)((u8*)r29 + 0x20) = f0;
                }
            L_80199CE4:
                f2 = *(f32*)((u8*)r29 + 0x28);
                f1 = *(f32*)((u8*)r29 + 0x1C);
                f0 = *(f32*)((u8*)r29 + 0x20);
                f0 = f2 * f1 + f0;
                *(f32*)(sp + 0x20) = f0;
                goto L_80199D58;
            L_80199CFC:
                tmp = *(u16*)((u8*)r29 + 0x1A);
                if (tmp != 0) {
                    r3 = *(u16*)((u8*)r29 + 0x1A);
                    tmp = 0x43300000;
                    *(u32*)(sp + 0x30) = tmp;
                    f1 = *(f64*)lbl_8047DA50;
                    f6 = *(f64*)lbl_8047DA48;
                    f2 = *(f32*)((u8*)r29 + 0x1C);
                    f0 = f0 - f1;
                    f3 = *(f32*)((u8*)r29 + 0x20);
                    f4 = *(f32*)((u8*)r29 + 0x24);
                    f5 = *(f32*)((u8*)r29 + 0x28);
                    f1 = f6 / f0;
                    f6 = *(f32*)((u8*)r29 + 0x2C);
                    f1 = (f32)f1;
                    fn_801B2560();
                    *(f32*)(sp + 0x20) = f1;
                    goto L_80199D58;
                }
                f0 = *(f32*)((u8*)r29 + 0x24);
                *(f32*)(sp + 0x20) = f0;
            L_80199D58:
                r12 = r31;
                r3 = r30;
                r5 = (u32)sp + 0x20;
                r4 = *(u8*)((u8*)r29 + 0x13);
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
                return;
            L_80199D74:
                r3 = r29;
                fn_8019A24C();
                continue;
                }
            tmp = *(u8*)((u8*)r29 + 0x10);
            tmp = tmp & 0x00000080;
            do {
            do {
                if (tmp == 0 || r31 == 0) break;

                tmp = *(u8*)((u8*)r29 + 0x12);
                if ((s32)tmp != 2) {
                    if ((s32)tmp < 2) {
                        if ((s32)tmp < 1) {
                            break;
                        }
                        if ((s32)tmp != 6) {
                            if ((s32)tmp >= 6) break;
                            goto L_80199EA4;
                        }
                        tmp = *(u8*)((u8*)r29 + 0x10);
                        tmp = tmp & 0x00000080;
                        if ((s32)tmp == 6) break;
                        f0 = *(f32*)((u8*)r29 + 0x20);
                        *(f32*)(sp + 0x14) = f0;
                        tmp = *(u8*)((u8*)r29 + 0x10);
                        tmp = tmp & 0x7F;
                        *(u8*)((u8*)r29 + 0x10) = tmp;
                        break;
                        }
                    r3 = *(u16*)((u8*)r29 + 0x1A);
                    tmp = 0x43300000;
                    *(u32*)(sp + 0x30) = tmp;
                    f1 = *(f64*)lbl_8047DA50;
                    f2 = *(f32*)((u8*)r29 + 0x1C);
                    f0 = f0 - f1;
                    /* cror eq, gt, eq */;
                    if (f2 == f0) {
                        f0 = *(f32*)((u8*)r29 + 0x24);
                    } else {

                        f0 = *(f32*)((u8*)r29 + 0x20);
                    }
                    *(f32*)(sp + 0x14) = f0;
                    break;
                }
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0x00000020;
                if (f2 != f0) {
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp & 0xFFFFFFDF;
                    *(u8*)((u8*)r29 + 0x10) = tmp;
                    tmp = *(u16*)((u8*)r29 + 0x1A);
                    if (tmp != 0) {
                        r3 = *(u16*)((u8*)r29 + 0x1A);
                        tmp = 0x43300000;
                        *(u32*)(sp + 0x30) = tmp;
                        f3 = *(f32*)((u8*)r29 + 0x24);
                        f2 = *(f32*)((u8*)r29 + 0x20);
                        f1 = *(f64*)lbl_8047DA50;
                        f2 = f3 - f2;
                        f0 = f0 - f1;
                        f0 = f2 / f0;
                        *(f32*)((u8*)r29 + 0x28) = f0;
                        goto L_80199E8C;
                    }
                    f0 = *(f32*)lbl_8047DA3C;
                    *(f32*)((u8*)r29 + 0x28) = f0;
                    f0 = *(f32*)((u8*)r29 + 0x24);
                    *(f32*)((u8*)r29 + 0x20) = f0;
                }
            L_80199E8C:
                f2 = *(f32*)((u8*)r29 + 0x28);
                f1 = *(f32*)((u8*)r29 + 0x1C);
                f0 = *(f32*)((u8*)r29 + 0x20);
                f0 = f2 * f1 + f0;
                *(f32*)(sp + 0x14) = f0;
                break;
            L_80199EA4:
                tmp = *(u16*)((u8*)r29 + 0x1A);
                if (tmp != 0) {
                    r3 = *(u16*)((u8*)r29 + 0x1A);
                    tmp = 0x43300000;
                    *(u32*)(sp + 0x30) = tmp;
                    f1 = *(f64*)lbl_8047DA50;
                    f6 = *(f64*)lbl_8047DA48;
                    f2 = *(f32*)((u8*)r29 + 0x1C);
                    f0 = f0 - f1;
                    f3 = *(f32*)((u8*)r29 + 0x20);
                    f4 = *(f32*)((u8*)r29 + 0x24);
                    f5 = *(f32*)((u8*)r29 + 0x28);
                    f1 = f6 / f0;
                    f6 = *(f32*)((u8*)r29 + 0x2C);
                    f1 = (f32)f1;
                    fn_801B2560();
                    *(f32*)(sp + 0x14) = f1;
                    break;
                }
                f0 = *(f32*)((u8*)r29 + 0x24);
                *(f32*)(sp + 0x14) = f0;
            } while (0);
                r12 = r31;
                r3 = r30;
                r5 = (u32)sp + 0x14;
                r4 = *(u8*)((u8*)r29 + 0x13);
                ctr_fn = (void(*)(void))r12;
                ctr_fn();
            } while (0);

            if (r29 == 0) {
                tmp = 0x0;
            } else {

                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0xF;
            }
            if (tmp != 3) {
                r4 = (u32)lbl_80274764;
                r3 = (u32)lbl_8047DA30;
                r5 = (u32)lbl_80274764;
                r4 = 0x16c;
                fn_80196E10();
            }
            r4 = *(u32*)((u8*)r29 + 0x8);
            r3 = *(u32*)((u8*)r29 + 0x4);
            tmp = *(u32*)((u8*)r29 + 0xC);
            r3 = r3 - r4;
            if (r3 >= tmp) {
                r3 = 0x6;
                continue;
            }
            r5 = 0x0;
            r4 = 0x0;
            do {
                r3 = *(u32*)((u8*)r29 + 0x4);
                tmp = r3 + 0x1;
                *(u32*)((u8*)r29 + 0x4) = tmp;
                r3 = *(u8*)((u8*)r3 + 0x0);
                tmp = r3 & 0x00000080;
                r3 = r3 & 0x7F;
                r3 = r3 << r4;
                r4 = r4 + 0x7;
                r5 = r5 | r3;
            } while (r3 != tmp);
            *(u16*)((u8*)r29 + 0x1A) = r5;
            tmp = *(u8*)((u8*)r29 + 0x10);
            tmp = tmp | 0x20;
            *(u8*)((u8*)r29 + 0x10) = tmp;
            if (r29 != 0) {
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0x000000F0;
                tmp = tmp | 0x2;
                *(u8*)((u8*)r29 + 0x10) = tmp;
            }
            r3 = 0x2;
            continue;
        }
        r3 = *(u16*)((u8*)r29 + 0x1A);
        tmp = 0x43300000;
        *(u32*)(sp + 0x30) = tmp;
        f2 = *(f64*)lbl_8047DA50;
        f0 = *(f32*)((u8*)r29 + 0x1C);
        f1 = f1 - f2;
        /* cror eq, lt, eq */;
        if (f1 == f0) {
            r3 = *(u16*)((u8*)r29 + 0x1A);
            tmp = 0x43300000;
            r5 = *(u16*)((u8*)r29 + 0x1A);
            r4 = 0x43300000;
            f1 = *(f64*)lbl_8047DA50;
            r3 = 0x3;
            *(u32*)(sp + 0x38) = tmp;
            f2 = *(f32*)((u8*)r29 + 0x1C);
            f0 = f0 - f1;
            f3 = *(f64*)lbl_8047DA50;
            f0 = f2 - f0;
            f31 = f1 - f3;
            *(f32*)((u8*)r29 + 0x1C) = f0;
            if (r29 == 0) continue;
            tmp = *(u8*)((u8*)r29 + 0x10);
            tmp = tmp & 0x000000F0;
            tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
            *(u8*)((u8*)r29 + 0x10) = tmp;
            continue;
        }
        do {
        do {
            if (r31 == 0) break;
            tmp = *(u8*)((u8*)r29 + 0x12);
            if ((s32)tmp != 2) {
                if ((s32)tmp < 2) {
                    if ((s32)tmp < 1) {
                        break;
                    }
                    if ((s32)tmp != 6) {
                        if ((s32)tmp >= 6) break;
                        goto L_8019A174;
                    }
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp & 0x00000080;
                    if ((s32)tmp == 6) break;
                    f0 = *(f32*)((u8*)r29 + 0x20);
                    *(f32*)(sp + 0x8) = f0;
                    tmp = *(u8*)((u8*)r29 + 0x10);
                    tmp = tmp & 0x7F;
                    *(u8*)((u8*)r29 + 0x10) = tmp;
                    break;
                    }
                r3 = *(u16*)((u8*)r29 + 0x1A);
                tmp = 0x43300000;
                *(u32*)(sp + 0x38) = tmp;
                f1 = *(f64*)lbl_8047DA50;
                f2 = *(f32*)((u8*)r29 + 0x1C);
                f0 = f0 - f1;
                /* cror eq, gt, eq */;
                if (f2 == f0) {
                    f0 = *(f32*)((u8*)r29 + 0x24);
                } else {

                    f0 = *(f32*)((u8*)r29 + 0x20);
                }
                *(f32*)(sp + 0x8) = f0;
                break;
            }
            tmp = *(u8*)((u8*)r29 + 0x10);
            tmp = tmp & 0x00000020;
            if (f2 != f0) {
                tmp = *(u8*)((u8*)r29 + 0x10);
                tmp = tmp & 0xFFFFFFDF;
                *(u8*)((u8*)r29 + 0x10) = tmp;
                tmp = *(u16*)((u8*)r29 + 0x1A);
                if (tmp != 0) {
                    r3 = *(u16*)((u8*)r29 + 0x1A);
                    tmp = 0x43300000;
                    *(u32*)(sp + 0x38) = tmp;
                    f3 = *(f32*)((u8*)r29 + 0x24);
                    f2 = *(f32*)((u8*)r29 + 0x20);
                    f1 = *(f64*)lbl_8047DA50;
                    f2 = f3 - f2;
                    f0 = f0 - f1;
                    f0 = f2 / f0;
                    *(f32*)((u8*)r29 + 0x28) = f0;
                    goto L_8019A15C;
                }
                f0 = *(f32*)lbl_8047DA3C;
                *(f32*)((u8*)r29 + 0x28) = f0;
                f0 = *(f32*)((u8*)r29 + 0x24);
                *(f32*)((u8*)r29 + 0x20) = f0;
            }
        L_8019A15C:
            f2 = *(f32*)((u8*)r29 + 0x28);
            f1 = *(f32*)((u8*)r29 + 0x1C);
            f0 = *(f32*)((u8*)r29 + 0x20);
            f0 = f2 * f1 + f0;
            *(f32*)(sp + 0x8) = f0;
            break;
        L_8019A174:
            tmp = *(u16*)((u8*)r29 + 0x1A);
            if (tmp != 0) {
                r3 = *(u16*)((u8*)r29 + 0x1A);
                tmp = 0x43300000;
                *(u32*)(sp + 0x38) = tmp;
                f1 = *(f64*)lbl_8047DA50;
                f6 = *(f64*)lbl_8047DA48;
                f2 = *(f32*)((u8*)r29 + 0x1C);
                f0 = f0 - f1;
                f3 = *(f32*)((u8*)r29 + 0x20);
                f4 = *(f32*)((u8*)r29 + 0x24);
                f5 = *(f32*)((u8*)r29 + 0x28);
                f1 = f6 / f0;
                f6 = *(f32*)((u8*)r29 + 0x2C);
                f1 = (f32)f1;
                fn_801B2560();
                *(f32*)(sp + 0x8) = f1;
                break;
            }
            f0 = *(f32*)((u8*)r29 + 0x24);
            *(f32*)(sp + 0x8) = f0;
        } while (0);
            r12 = r31;
            r3 = r30;
            r5 = (u32)sp + 0x8;
            r4 = *(u8*)((u8*)r29 + 0x13);
            ctr_fn = (void(*)(void))r12;
            ctr_fn();
        } while (0);

        r3 = 0x5;
        if (r29 == 0) return;
        tmp = *(u8*)((u8*)r29 + 0x10);
        tmp = tmp & 0x000000F0;
        tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
        *(u8*)((u8*)r29 + 0x10) = tmp;
        return;
    L_8019A208:
        r3 = 0x4;
        if (r29 == 0) continue;
        tmp = *(u8*)((u8*)r29 + 0x10);
        tmp = tmp & 0x000000F0;
        tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
        *(u8*)((u8*)r29 + 0x10) = tmp;
        continue;
}

    return;
}

/* 0x8019A24C | 0x1244 */
void fn_8019A24C(void) {
    extern u8 lbl_8027477C[];
    extern u8 lbl_8047DA30[];
    extern u8 lbl_8047DA3C[];
    extern u8 lbl_8047DA50[];
    extern u8 lbl_8047DA58[];
    extern void fn_80196E10();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r31 = r3;
    r4 = *(u32*)((u8*)r3 + 0x8);
    r3 = *(u32*)((u8*)r3 + 0x4);
    tmp = *(u32*)((u8*)r31 + 0xC);
    r3 = r3 - r4;
    if (r3 >= tmp) {
        r3 = 0x6;
        return;
    }
    tmp = *(u8*)((u8*)r31 + 0x11);
    *(u8*)((u8*)r31 + 0x12) = tmp;
    tmp = *(u16*)((u8*)r31 + 0x16);
    if (tmp == 0) {
        r3 = *(u32*)((u8*)r31 + 0x4);
        r4 = 0x3;
        tmp = *(u8*)((u8*)r3 + 0x0);
        tmp = tmp & 0xF;
        *(u8*)((u8*)r31 + 0x11) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x4);
        tmp = r3 + 0x1;
        *(u32*)((u8*)r31 + 0x4) = tmp;
        r3 = *(u8*)((u8*)r3 + 0x0);
        tmp = r3 & 0x00000080;
        /* extrwi r3, r3, 3, 25 */;
        r5 = r3 + 0x1;
        if (tmp == 0) {
            goto L_8019A2F8;
        }
    do {
            r3 = *(u32*)((u8*)r31 + 0x4);
            tmp = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r3 = *(u8*)((u8*)r3 + 0x0);
            tmp = r3 & 0x00000080;
            r3 = r3 & 0x7F;
            r3 = r3 << r4;
            r4 = r4 + 0x7;
            r5 = r5 + r3;
    } while (tmp != 0);
    L_8019A2F8:
        *(u16*)((u8*)r31 + 0x16) = r5;
    }
    r3 = *(u16*)((u8*)r31 + 0x16);
    *(u16*)((u8*)r31 + 0x16) = tmp;
    tmp = *(u8*)((u8*)r31 + 0x11);
    if ((s32)tmp != 4) {
        if ((s32)tmp < 4) {
            if ((s32)tmp != 2) {
                if ((s32)tmp < 2) {
                    if ((s32)tmp < 1) {
                        r3 = 0x0;
                        return;
                    }
                    if ((s32)tmp != 6) {
                        if ((s32)tmp >= 6) { r3 = 0x0; return; }
                        goto L_8019AF60;
                        }
                    if (r31 == 0) {
                        r30 = 0x0;
                    } else {

                        tmp = *(u8*)((u8*)r31 + 0x10);
                        r30 = tmp & 0xF;
                    }
                    r3 = 0x1;
                    tmp = r3 - r4;
                    r5 = 0x1;
                    r3 = r3 | ~r4;
                    tmp = (u32)tmp >> 1;
                    tmp = r3 - tmp;
                    /* srwi. tmp, tmp, 31 */;
                    if (r31 == 0) {
                        r5 = 0x0;
                    }
                    if ((s32)r5 == 0) {
                        r4 = (u32)lbl_8027477C;
                        r3 = (u32)lbl_8047DA30;
                        r5 = (u32)lbl_8027477C;
                        r4 = 0x17f;
                        fn_80196E10();
                    }
                    f0 = *(f32*)((u8*)r31 + 0x24);
                    *(f32*)((u8*)r31 + 0x20) = f0;
                    r5 = *(u8*)((u8*)r31 + 0x14);
                    tmp = r5 & 0xFF;
                    if ((s32)r5 == 0) {
                        r4 = *(u32*)((u8*)r31 + 0x4);
                        tmp = r4 + 0x1;
                        *(u32*)((u8*)r31 + 0x4) = tmp;
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r5 = *(u8*)((u8*)r4 + 0x0);
                        tmp = r3 + 0x1;
                        *(u32*)((u8*)r31 + 0x4) = tmp;
                        r4 = *(u32*)((u8*)r31 + 0x4);
                        r3 = *(u8*)((u8*)r3 + 0x0);
                        tmp = r4 + 0x1;
                        *(u32*)((u8*)r31 + 0x4) = tmp;
                        tmp = r3 << 8;
                        tmp = r5 | tmp;
                        *(u32*)(sp + 0x20) = tmp;
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r4 = *(u8*)((u8*)r4 + 0x0);
                        tmp = r3 + 0x1;
                        r4 = r4 << 16;
                        *(u32*)((u8*)r31 + 0x4) = tmp;
                        tmp = r5 | r4;
                        *(u32*)(sp + 0x20) = tmp;
                        tmp = *(u8*)((u8*)r3 + 0x0);
                        tmp = tmp << 24;
                        tmp = r3 | tmp;
                        *(u32*)(sp + 0x20) = tmp;
                        f0 = *(f32*)(sp + 0x20);
                        goto L_8019A580;
                    }
                    r3 = r5 & 0x000000E0;
                    r4 = 0x1;
                    tmp = r5 & 0x1F;
                    tmp = r4 << tmp;
                    if ((s32)r3 != 0x60) {
                        if ((s32)r3 < 0x60) {
                            if ((s32)r3 != 0x40) {
                                if ((s32)r3 >= 0x40) goto L_8019A558;
                                if ((s32)r3 != 0x20) {
                                    goto L_8019A558;
                                }
                                if ((s32)r3 != 0x80) {
                                    goto L_8019A558;
                                }
                                r5 = *(u32*)((u8*)r31 + 0x4);
                                r4 = 0x43300000;
                                r3 = *(u32*)((u8*)r31 + 0x4);
                                r5 = *(u8*)((u8*)r5 + 0x0);
                                r3 = r3 + 0x1;
                                r4 = (s8)r5;
                                f1 = *(f64*)lbl_8047DA58;
                                *(u32*)((u8*)r31 + 0x4) = r3;
                                f2 = f0 - f1;
                                goto L_8019A560;
                                }
                            r5 = *(u32*)((u8*)r31 + 0x4);
                            r4 = 0x43300000;
                            r3 = *(u32*)((u8*)r31 + 0x4);
                            r5 = *(u8*)((u8*)r5 + 0x0);
                            r3 = r3 + 0x1;
                            f1 = *(f64*)lbl_8047DA50;
                            *(u32*)((u8*)r31 + 0x4) = r3;
                            f2 = f0 - f1;
                            goto L_8019A560;
                                }
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r4 = 0x43300000;
                        r5 = *(u32*)((u8*)r31 + 0x4);
                        r6 = *(u8*)((u8*)r3 + 0x1);
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r6 = (s8)r6;
                        r5 = *(u8*)((u8*)r5 + 0x0);
                        r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
                        r3 = r3 + 0x2;
                        f1 = *(f64*)lbl_8047DA58;
                        *(u32*)((u8*)r31 + 0x4) = r3;
                        f2 = f0 - f1;
                        goto L_8019A560;
                            }
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r6 = *(u8*)((u8*)r5 + 0x1);
                    r5 = *(u8*)((u8*)r3 + 0x0);
                    r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r3 = r3 + 0x2;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019A560;
                L_8019A558:
                    f0 = *(f32*)lbl_8047DA3C;
                    goto L_8019A580;
                L_8019A560:
                    tmp = 0x43300000;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)(sp + 0x28) = tmp;
                    f0 = f0 - f1;
                    f0 = f2 / f0;
                L_8019A580:
                    *(f32*)((u8*)r31 + 0x24) = f0;
                    tmp = *(u8*)((u8*)r31 + 0x12);
                    if (tmp != 5) {
                        f1 = *(f32*)((u8*)r31 + 0x2C);
                        f0 = *(f32*)lbl_8047DA3C;
                        *(f32*)((u8*)r31 + 0x28) = f1;
                        *(f32*)((u8*)r31 + 0x2C) = f0;
                    }
                    tmp = 0x1;
                    r4 = r30 - tmp;
                    r3 = 0x4;
                    tmp = tmp - r30;
                    tmp = ~(r4 | tmp);
                    tmp = (s32)tmp >> 31;
                    r3 = tmp + r3;
                    if (r31 == 0) return;
                    tmp = *(u8*)((u8*)r31 + 0x10);
                    tmp = tmp & 0x000000F0;
                    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
                    *(u8*)((u8*)r31 + 0x10) = tmp;
                    return;
                    }
                if (r31 == 0) {
                    r30 = 0x0;
                } else {

                    tmp = *(u8*)((u8*)r31 + 0x10);
                    r30 = tmp & 0xF;
                }
                r3 = 0x1;
                tmp = r3 - r4;
                r5 = 0x1;
                r3 = r3 | ~r4;
                tmp = (u32)tmp >> 1;
                tmp = r3 - tmp;
                /* srwi. tmp, tmp, 31 */;
                if (r31 == 0) {
                    r5 = 0x0;
                }
                if ((s32)r5 == 0) {
                    r4 = (u32)lbl_8027477C;
                    r3 = (u32)lbl_8047DA30;
                    r5 = (u32)lbl_8027477C;
                    r4 = 0x193;
                    fn_80196E10();
                }
                f0 = *(f32*)((u8*)r31 + 0x24);
                *(f32*)((u8*)r31 + 0x20) = f0;
                r5 = *(u8*)((u8*)r31 + 0x14);
                tmp = r5 & 0xFF;
                if ((s32)r5 == 0) {
                    r4 = *(u32*)((u8*)r31 + 0x4);
                    tmp = r4 + 0x1;
                    *(u32*)((u8*)r31 + 0x4) = tmp;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r5 = *(u8*)((u8*)r4 + 0x0);
                    tmp = r3 + 0x1;
                    *(u32*)((u8*)r31 + 0x4) = tmp;
                    r4 = *(u32*)((u8*)r31 + 0x4);
                    r3 = *(u8*)((u8*)r3 + 0x0);
                    tmp = r4 + 0x1;
                    *(u32*)((u8*)r31 + 0x4) = tmp;
                    tmp = r3 << 8;
                    tmp = r5 | tmp;
                    *(u32*)(sp + 0x1C) = tmp;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r4 = *(u8*)((u8*)r4 + 0x0);
                    tmp = r3 + 0x1;
                    r4 = r4 << 16;
                    *(u32*)((u8*)r31 + 0x4) = tmp;
                    tmp = r5 | r4;
                    *(u32*)(sp + 0x1C) = tmp;
                    tmp = *(u8*)((u8*)r3 + 0x0);
                    tmp = tmp << 24;
                    tmp = r3 | tmp;
                    *(u32*)(sp + 0x1C) = tmp;
                    f0 = *(f32*)(sp + 0x1C);
                    goto L_8019A818;
                }
                r3 = r5 & 0x000000E0;
                r4 = 0x1;
                tmp = r5 & 0x1F;
                tmp = r4 << tmp;
                if ((s32)r3 != 0x60) {
                    if ((s32)r3 < 0x60) {
                        if ((s32)r3 != 0x40) {
                            if ((s32)r3 >= 0x40) goto L_8019A7F0;
                            if ((s32)r3 != 0x20) {
                                goto L_8019A7F0;
                            }
                            if ((s32)r3 != 0x80) {
                                goto L_8019A7F0;
                            }
                            r5 = *(u32*)((u8*)r31 + 0x4);
                            r4 = 0x43300000;
                            r3 = *(u32*)((u8*)r31 + 0x4);
                            r5 = *(u8*)((u8*)r5 + 0x0);
                            r3 = r3 + 0x1;
                            r4 = (s8)r5;
                            f1 = *(f64*)lbl_8047DA58;
                            *(u32*)((u8*)r31 + 0x4) = r3;
                            f2 = f0 - f1;
                            goto L_8019A7F8;
                            }
                        r5 = *(u32*)((u8*)r31 + 0x4);
                        r4 = 0x43300000;
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r5 = *(u8*)((u8*)r5 + 0x0);
                        r3 = r3 + 0x1;
                        f1 = *(f64*)lbl_8047DA50;
                        *(u32*)((u8*)r31 + 0x4) = r3;
                        f2 = f0 - f1;
                        goto L_8019A7F8;
                            }
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r6 = *(u8*)((u8*)r3 + 0x1);
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r6 = (s8)r6;
                    r5 = *(u8*)((u8*)r5 + 0x0);
                    r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
                    r3 = r3 + 0x2;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019A7F8;
                        }
                r5 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r6 = *(u8*)((u8*)r5 + 0x1);
                r5 = *(u8*)((u8*)r3 + 0x0);
                r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
                r3 = *(u32*)((u8*)r31 + 0x4);
                r3 = r3 + 0x2;
                f1 = *(f64*)lbl_8047DA58;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019A7F8;
            L_8019A7F0:
                f0 = *(f32*)lbl_8047DA3C;
                goto L_8019A818;
            L_8019A7F8:
                tmp = 0x43300000;
                f1 = *(f64*)lbl_8047DA58;
                *(u32*)(sp + 0x28) = tmp;
                f0 = f0 - f1;
                f0 = f2 / f0;
            L_8019A818:
                *(f32*)((u8*)r31 + 0x24) = f0;
                tmp = *(u8*)((u8*)r31 + 0x12);
                if (tmp != 5) {
                    f1 = *(f32*)((u8*)r31 + 0x2C);
                    f0 = *(f32*)lbl_8047DA3C;
                    *(f32*)((u8*)r31 + 0x28) = f1;
                    *(f32*)((u8*)r31 + 0x2C) = f0;
                }
                tmp = 0x1;
                r4 = r30 - tmp;
                r3 = 0x4;
                tmp = tmp - r30;
                tmp = ~(r4 | tmp);
                tmp = (s32)tmp >> 31;
                r3 = tmp + r3;
                if (r31 == 0) return;
                tmp = *(u8*)((u8*)r31 + 0x10);
                tmp = tmp & 0x000000F0;
                tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
                *(u8*)((u8*)r31 + 0x10) = tmp;
                return;
                    }
            if (r31 == 0) {
                r30 = 0x0;
            } else {

                tmp = *(u8*)((u8*)r31 + 0x10);
                r30 = tmp & 0xF;
            }
            r3 = 0x1;
            tmp = r3 - r4;
            r5 = 0x1;
            r3 = r3 | ~r4;
            tmp = (u32)tmp >> 1;
            tmp = r3 - tmp;
            /* srwi. tmp, tmp, 31 */;
            if (r31 == 0) {
                r5 = 0x0;
            }
            if ((s32)r5 == 0) {
                r4 = (u32)lbl_8027477C;
                r3 = (u32)lbl_8047DA30;
                r5 = (u32)lbl_8027477C;
                r4 = 0x1a7;
                fn_80196E10();
            }
            f0 = *(f32*)((u8*)r31 + 0x24);
            *(f32*)((u8*)r31 + 0x20) = f0;
            f0 = *(f32*)((u8*)r31 + 0x2C);
            *(f32*)((u8*)r31 + 0x28) = f0;
            r5 = *(u8*)((u8*)r31 + 0x14);
            tmp = r5 & 0xFF;
            if ((s32)r5 == 0) {
                r4 = *(u32*)((u8*)r31 + 0x4);
                tmp = r4 + 0x1;
                *(u32*)((u8*)r31 + 0x4) = tmp;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r5 = *(u8*)((u8*)r4 + 0x0);
                tmp = r3 + 0x1;
                *(u32*)((u8*)r31 + 0x4) = tmp;
                r4 = *(u32*)((u8*)r31 + 0x4);
                r3 = *(u8*)((u8*)r3 + 0x0);
                tmp = r4 + 0x1;
                *(u32*)((u8*)r31 + 0x4) = tmp;
                tmp = r3 << 8;
                tmp = r5 | tmp;
                *(u32*)(sp + 0x18) = tmp;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r4 = *(u8*)((u8*)r4 + 0x0);
                tmp = r3 + 0x1;
                r4 = r4 << 16;
                *(u32*)((u8*)r31 + 0x4) = tmp;
                tmp = r5 | r4;
                *(u32*)(sp + 0x18) = tmp;
                tmp = *(u8*)((u8*)r3 + 0x0);
                tmp = tmp << 24;
                tmp = r3 | tmp;
                *(u32*)(sp + 0x18) = tmp;
                f0 = *(f32*)(sp + 0x18);
                goto L_8019AAB8;
            }
            r3 = r5 & 0x000000E0;
            r4 = 0x1;
            tmp = r5 & 0x1F;
            tmp = r4 << tmp;
            if ((s32)r3 != 0x60) {
                if ((s32)r3 < 0x60) {
                    if ((s32)r3 != 0x40) {
                        if ((s32)r3 >= 0x40) goto L_8019AA90;
                        if ((s32)r3 != 0x20) {
                            goto L_8019AA90;
                        }
                        if ((s32)r3 != 0x80) {
                            goto L_8019AA90;
                        }
                        r5 = *(u32*)((u8*)r31 + 0x4);
                        r4 = 0x43300000;
                        r3 = *(u32*)((u8*)r31 + 0x4);
                        r5 = *(u8*)((u8*)r5 + 0x0);
                        r3 = r3 + 0x1;
                        r4 = (s8)r5;
                        f1 = *(f64*)lbl_8047DA58;
                        *(u32*)((u8*)r31 + 0x4) = r3;
                        f2 = f0 - f1;
                        goto L_8019AA98;
                        }
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r5 = *(u8*)((u8*)r5 + 0x0);
                    r3 = r3 + 0x1;
                    f1 = *(f64*)lbl_8047DA50;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019AA98;
                        }
                r3 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r5 = *(u32*)((u8*)r31 + 0x4);
                r6 = *(u8*)((u8*)r3 + 0x1);
                r3 = *(u32*)((u8*)r31 + 0x4);
                r6 = (s8)r6;
                r5 = *(u8*)((u8*)r5 + 0x0);
                r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
                r3 = r3 + 0x2;
                f1 = *(f64*)lbl_8047DA58;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019AA98;
                    }
            r5 = *(u32*)((u8*)r31 + 0x4);
            r4 = 0x43300000;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r6 = *(u8*)((u8*)r5 + 0x1);
            r5 = *(u8*)((u8*)r3 + 0x0);
            r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
            r3 = *(u32*)((u8*)r31 + 0x4);
            r3 = r3 + 0x2;
            f1 = *(f64*)lbl_8047DA58;
            *(u32*)((u8*)r31 + 0x4) = r3;
            f2 = f0 - f1;
            goto L_8019AA98;
        L_8019AA90:
            f0 = *(f32*)lbl_8047DA3C;
            goto L_8019AAB8;
        L_8019AA98:
            tmp = 0x43300000;
            f1 = *(f64*)lbl_8047DA58;
            *(u32*)(sp + 0x28) = tmp;
            f0 = f0 - f1;
            f0 = f2 / f0;
        L_8019AAB8:
            tmp = 0x1;
            *(f32*)((u8*)r31 + 0x24) = f0;
            r3 = r30 - tmp;
            f0 = *(f32*)lbl_8047DA3C;
            tmp = tmp - r30;
            tmp = ~(r3 | tmp);
            r3 = 0x4;
            tmp = (s32)tmp >> 31;
            *(f32*)((u8*)r31 + 0x2C) = f0;
            r3 = tmp + r3;
            if (r31 == 0) return;
            tmp = *(u8*)((u8*)r31 + 0x10);
            tmp = tmp & 0x000000F0;
            tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
            *(u8*)((u8*)r31 + 0x10) = tmp;
            return;
        }
        if (r31 == 0) {
            r30 = 0x0;
        } else {

            tmp = *(u8*)((u8*)r31 + 0x10);
            r30 = tmp & 0xF;
        }
        r3 = 0x1;
        tmp = r3 - r4;
        r5 = 0x1;
        r3 = r3 | ~r4;
        tmp = (u32)tmp >> 1;
        tmp = r3 - tmp;
        /* srwi. tmp, tmp, 31 */;
        if (r31 == 0) {
            r5 = 0x0;
        }
        if ((s32)r5 == 0) {
            r4 = (u32)lbl_8027477C;
            r3 = (u32)lbl_8047DA30;
            r5 = (u32)lbl_8027477C;
            r4 = 0x1b9;
            fn_80196E10();
        }
        f0 = *(f32*)((u8*)r31 + 0x24);
        *(f32*)((u8*)r31 + 0x20) = f0;
        r5 = *(u8*)((u8*)r31 + 0x14);
        tmp = r5 & 0xFF;
        if ((s32)r5 == 0) {
            r4 = *(u32*)((u8*)r31 + 0x4);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r5 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r4 = *(u32*)((u8*)r31 + 0x4);
            r3 = *(u8*)((u8*)r3 + 0x0);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r3 << 8;
            tmp = r5 | tmp;
            *(u32*)(sp + 0x14) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            r4 = r4 << 16;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r5 | r4;
            *(u32*)(sp + 0x14) = tmp;
            tmp = *(u8*)((u8*)r3 + 0x0);
            tmp = tmp << 24;
            tmp = r3 | tmp;
            *(u32*)(sp + 0x14) = tmp;
            f0 = *(f32*)(sp + 0x14);
            goto L_8019AD3C;
        }
        r3 = r5 & 0x000000E0;
        r4 = 0x1;
        tmp = r5 & 0x1F;
        tmp = r4 << tmp;
        if ((s32)r3 != 0x60) {
            if ((s32)r3 < 0x60) {
                if ((s32)r3 != 0x40) {
                    if ((s32)r3 >= 0x40) goto L_8019AD14;
                    if ((s32)r3 != 0x20) {
                        goto L_8019AD14;
                    }
                    if ((s32)r3 != 0x80) {
                        goto L_8019AD14;
                    }
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r5 = *(u8*)((u8*)r5 + 0x0);
                    r3 = r3 + 0x1;
                    r4 = (s8)r5;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019AD1C;
                    }
                r5 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r5 = *(u8*)((u8*)r5 + 0x0);
                r3 = r3 + 0x1;
                f1 = *(f64*)lbl_8047DA50;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019AD1C;
                    }
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = 0x43300000;
            r5 = *(u32*)((u8*)r31 + 0x4);
            r6 = *(u8*)((u8*)r3 + 0x1);
            r3 = *(u32*)((u8*)r31 + 0x4);
            r6 = (s8)r6;
            r5 = *(u8*)((u8*)r5 + 0x0);
            r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
            r3 = r3 + 0x2;
            f1 = *(f64*)lbl_8047DA58;
            *(u32*)((u8*)r31 + 0x4) = r3;
            f2 = f0 - f1;
            goto L_8019AD1C;
                }
        r5 = *(u32*)((u8*)r31 + 0x4);
        r4 = 0x43300000;
        r3 = *(u32*)((u8*)r31 + 0x4);
        r6 = *(u8*)((u8*)r5 + 0x1);
        r5 = *(u8*)((u8*)r3 + 0x0);
        r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
        r3 = *(u32*)((u8*)r31 + 0x4);
        r3 = r3 + 0x2;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)((u8*)r31 + 0x4) = r3;
        f2 = f0 - f1;
        goto L_8019AD1C;
    L_8019AD14:
        f0 = *(f32*)lbl_8047DA3C;
        goto L_8019AD3C;
    L_8019AD1C:
        tmp = 0x43300000;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)(sp + 0x28) = tmp;
        f0 = f0 - f1;
        f0 = f2 / f0;
    L_8019AD3C:
        *(f32*)((u8*)r31 + 0x24) = f0;
        f0 = *(f32*)((u8*)r31 + 0x2C);
        *(f32*)((u8*)r31 + 0x28) = f0;
        r5 = *(u8*)((u8*)r31 + 0x15);
        tmp = r5 & 0xFF;
        if ((s32)r3 == 0x80) {
            r4 = *(u32*)((u8*)r31 + 0x4);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r5 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r4 = *(u32*)((u8*)r31 + 0x4);
            r3 = *(u8*)((u8*)r3 + 0x0);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r3 << 8;
            tmp = r5 | tmp;
            *(u32*)(sp + 0x10) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            r4 = r4 << 16;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r5 | r4;
            *(u32*)(sp + 0x10) = tmp;
            tmp = *(u8*)((u8*)r3 + 0x0);
            tmp = tmp << 24;
            tmp = r3 | tmp;
            *(u32*)(sp + 0x10) = tmp;
            f0 = *(f32*)(sp + 0x10);
            goto L_8019AF24;
        }
        r3 = r5 & 0x000000E0;
        r4 = 0x1;
        tmp = r5 & 0x1F;
        tmp = r4 << tmp;
        if ((s32)r3 != 0x60) {
            if ((s32)r3 < 0x60) {
                if ((s32)r3 != 0x40) {
                    if ((s32)r3 >= 0x40) goto L_8019AEFC;
                    if ((s32)r3 != 0x20) {
                        goto L_8019AEFC;
                    }
                    if ((s32)r3 != 0x80) {
                        goto L_8019AEFC;
                    }
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r5 = *(u8*)((u8*)r5 + 0x0);
                    r3 = r3 + 0x1;
                    r4 = (s8)r5;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019AF04;
                    }
                r5 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r5 = *(u8*)((u8*)r5 + 0x0);
                r3 = r3 + 0x1;
                f1 = *(f64*)lbl_8047DA50;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019AF04;
                    }
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = 0x43300000;
            r5 = *(u32*)((u8*)r31 + 0x4);
            r6 = *(u8*)((u8*)r3 + 0x1);
            r3 = *(u32*)((u8*)r31 + 0x4);
            r6 = (s8)r6;
            r5 = *(u8*)((u8*)r5 + 0x0);
            r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
            r3 = r3 + 0x2;
            f1 = *(f64*)lbl_8047DA58;
            *(u32*)((u8*)r31 + 0x4) = r3;
            f2 = f0 - f1;
            goto L_8019AF04;
                }
        r5 = *(u32*)((u8*)r31 + 0x4);
        r4 = 0x43300000;
        r3 = *(u32*)((u8*)r31 + 0x4);
        r6 = *(u8*)((u8*)r5 + 0x1);
        r5 = *(u8*)((u8*)r3 + 0x0);
        r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
        r3 = *(u32*)((u8*)r31 + 0x4);
        r3 = r3 + 0x2;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)((u8*)r31 + 0x4) = r3;
        f2 = f0 - f1;
        goto L_8019AF04;
    L_8019AEFC:
        f0 = *(f32*)lbl_8047DA3C;
        goto L_8019AF24;
    L_8019AF04:
        tmp = 0x43300000;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)(sp + 0x28) = tmp;
        f0 = f0 - f1;
        f0 = f2 / f0;
    L_8019AF24:
        tmp = 0x1;
        r4 = r30 - tmp;
        r3 = 0x4;
        tmp = tmp - r30;
        *(f32*)((u8*)r31 + 0x2C) = f0;
        tmp = ~(r4 | tmp);
        tmp = (s32)tmp >> 31;
        r3 = tmp + r3;
        if (r31 == 0) return;
        tmp = *(u8*)((u8*)r31 + 0x10);
        tmp = tmp & 0x000000F0;
        tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
        *(u8*)((u8*)r31 + 0x10) = tmp;
        return;
    L_8019AF60:
        if (r31 == 0) {
            r3 = 0x0;
        } else {

            tmp = *(u8*)((u8*)r31 + 0x10);
            r3 = tmp & 0xF;
        }
        r3 = 0x1;
        tmp = r3 - r4;
        r5 = 0x1;
        r3 = r3 | ~r4;
        tmp = (u32)tmp >> 1;
        tmp = r3 - tmp;
        /* srwi. tmp, tmp, 31 */;
        if (r31 == 0) {
            r5 = 0x0;
        }
        if ((s32)r5 == 0) {
            r4 = (u32)lbl_8027477C;
            r3 = (u32)lbl_8047DA30;
            r5 = (u32)lbl_8027477C;
            r4 = 0x1cc;
            fn_80196E10();
        }
        f0 = *(f32*)((u8*)r31 + 0x2C);
        *(f32*)((u8*)r31 + 0x28) = f0;
        r5 = *(u8*)((u8*)r31 + 0x15);
        tmp = r5 & 0xFF;
        if ((s32)r5 == 0) {
            r4 = *(u32*)((u8*)r31 + 0x4);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r5 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            r4 = *(u32*)((u8*)r31 + 0x4);
            r3 = *(u8*)((u8*)r3 + 0x0);
            tmp = r4 + 0x1;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r3 << 8;
            tmp = r5 | tmp;
            *(u32*)(sp + 0xC) = tmp;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = *(u8*)((u8*)r4 + 0x0);
            tmp = r3 + 0x1;
            r4 = r4 << 16;
            *(u32*)((u8*)r31 + 0x4) = tmp;
            tmp = r5 | r4;
            *(u32*)(sp + 0xC) = tmp;
            tmp = *(u8*)((u8*)r3 + 0x0);
            tmp = tmp << 24;
            tmp = r3 | tmp;
            *(u32*)(sp + 0xC) = tmp;
            f0 = *(f32*)(sp + 0xC);
            goto L_8019B1A0;
        }
        r3 = r5 & 0x000000E0;
        r4 = 0x1;
        tmp = r5 & 0x1F;
        tmp = r4 << tmp;
        if ((s32)r3 != 0x60) {
            if ((s32)r3 < 0x60) {
                if ((s32)r3 != 0x40) {
                    if ((s32)r3 >= 0x40) goto L_8019B178;
                    if ((s32)r3 != 0x20) {
                        goto L_8019B178;
                    }
                    if ((s32)r3 != 0x80) {
                        goto L_8019B178;
                    }
                    r5 = *(u32*)((u8*)r31 + 0x4);
                    r4 = 0x43300000;
                    r3 = *(u32*)((u8*)r31 + 0x4);
                    r5 = *(u8*)((u8*)r5 + 0x0);
                    r3 = r3 + 0x1;
                    r4 = (s8)r5;
                    f1 = *(f64*)lbl_8047DA58;
                    *(u32*)((u8*)r31 + 0x4) = r3;
                    f2 = f0 - f1;
                    goto L_8019B180;
                    }
                r5 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r5 = *(u8*)((u8*)r5 + 0x0);
                r3 = r3 + 0x1;
                f1 = *(f64*)lbl_8047DA50;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019B180;
                    }
            r3 = *(u32*)((u8*)r31 + 0x4);
            r4 = 0x43300000;
            r5 = *(u32*)((u8*)r31 + 0x4);
            r6 = *(u8*)((u8*)r3 + 0x1);
            r3 = *(u32*)((u8*)r31 + 0x4);
            r6 = (s8)r6;
            r5 = *(u8*)((u8*)r5 + 0x0);
            r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
            r3 = r3 + 0x2;
            f1 = *(f64*)lbl_8047DA58;
            *(u32*)((u8*)r31 + 0x4) = r3;
            f2 = f0 - f1;
            goto L_8019B180;
                }
        r5 = *(u32*)((u8*)r31 + 0x4);
        r4 = 0x43300000;
        r3 = *(u32*)((u8*)r31 + 0x4);
        r6 = *(u8*)((u8*)r5 + 0x1);
        r5 = *(u8*)((u8*)r3 + 0x0);
        r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
        r3 = *(u32*)((u8*)r31 + 0x4);
        r3 = r3 + 0x2;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)((u8*)r31 + 0x4) = r3;
        f2 = f0 - f1;
        goto L_8019B180;
    L_8019B178:
        f0 = *(f32*)lbl_8047DA3C;
        goto L_8019B1A0;
    L_8019B180:
        tmp = 0x43300000;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)(sp + 0x28) = tmp;
        f0 = f0 - f1;
        f0 = f2 / f0;
    L_8019B1A0:
        *(f32*)((u8*)r31 + 0x2C) = f0;
        if (r31 == 0) {
            r3 = 0x0;
            return;
        }
        tmp = *(u8*)((u8*)r31 + 0x10);
        r3 = tmp & 0xF;
        return;
                    }
    if (r31 == 0) {
        r30 = 0x0;
    } else {

        tmp = *(u8*)((u8*)r31 + 0x10);
        r30 = tmp & 0xF;
    }
    r3 = 0x1;
    tmp = r3 - r4;
    r5 = 0x1;
    r3 = r3 | ~r4;
    tmp = (u32)tmp >> 1;
    tmp = r3 - tmp;
    /* srwi. tmp, tmp, 31 */;
    if (r31 == 0) {
        r5 = 0x0;
    }
    if ((s32)r5 == 0) {
        r4 = (u32)lbl_8027477C;
        r3 = (u32)lbl_8047DA30;
        r5 = (u32)lbl_8027477C;
        r4 = 0x1e9;
        fn_80196E10();
    }
    tmp = *(u8*)((u8*)r31 + 0x10);
    tmp = tmp & 0x00000040;
    if ((s32)r5 != 0) {
        tmp = *(u8*)((u8*)r31 + 0x11);
        *(u8*)((u8*)r31 + 0x12) = tmp;
        tmp = *(u8*)((u8*)r31 + 0x10);
        tmp = tmp & 0xFFFFFFBF;
        *(u8*)((u8*)r31 + 0x10) = tmp;
        tmp = *(u8*)((u8*)r31 + 0x10);
        tmp = tmp | 0x80;
        *(u8*)((u8*)r31 + 0x10) = tmp;
        f0 = *(f32*)((u8*)r31 + 0x24);
        *(f32*)((u8*)r31 + 0x20) = f0;
    }
    r5 = *(u8*)((u8*)r31 + 0x14);
    tmp = r5 & 0xFF;
    if ((s32)r5 == 0) {
        r4 = *(u32*)((u8*)r31 + 0x4);
        tmp = r4 + 0x1;
        *(u32*)((u8*)r31 + 0x4) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x4);
        r5 = *(u8*)((u8*)r4 + 0x0);
        tmp = r3 + 0x1;
        *(u32*)((u8*)r31 + 0x4) = tmp;
        r4 = *(u32*)((u8*)r31 + 0x4);
        r3 = *(u8*)((u8*)r3 + 0x0);
        tmp = r4 + 0x1;
        *(u32*)((u8*)r31 + 0x4) = tmp;
        tmp = r3 << 8;
        tmp = r5 | tmp;
        *(u32*)(sp + 0x8) = tmp;
        r3 = *(u32*)((u8*)r31 + 0x4);
        r4 = *(u8*)((u8*)r4 + 0x0);
        tmp = r3 + 0x1;
        r4 = r4 << 16;
        *(u32*)((u8*)r31 + 0x4) = tmp;
        tmp = r5 | r4;
        *(u32*)(sp + 0x8) = tmp;
        tmp = *(u8*)((u8*)r3 + 0x0);
        tmp = tmp << 24;
        tmp = r3 | tmp;
        *(u32*)(sp + 0x8) = tmp;
        f0 = *(f32*)(sp + 0x8);
        goto L_8019B42C;
    }
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 != 0x60) {
        if ((s32)r3 < 0x60) {
            if ((s32)r3 != 0x40) {
                if ((s32)r3 >= 0x40) goto L_8019B404;
                if ((s32)r3 != 0x20) {
                    goto L_8019B404;
                }
                if ((s32)r3 != 0x80) {
                    goto L_8019B404;
                }
                r5 = *(u32*)((u8*)r31 + 0x4);
                r4 = 0x43300000;
                r3 = *(u32*)((u8*)r31 + 0x4);
                r5 = *(u8*)((u8*)r5 + 0x0);
                r3 = r3 + 0x1;
                r4 = (s8)r5;
                f1 = *(f64*)lbl_8047DA58;
                *(u32*)((u8*)r31 + 0x4) = r3;
                f2 = f0 - f1;
                goto L_8019B40C;
                }
            r5 = *(u32*)((u8*)r31 + 0x4);
            r4 = 0x43300000;
            r3 = *(u32*)((u8*)r31 + 0x4);
            r5 = *(u8*)((u8*)r5 + 0x0);
            r3 = r3 + 0x1;
            f1 = *(f64*)lbl_8047DA50;
            *(u32*)((u8*)r31 + 0x4) = r3;
            f2 = f0 - f1;
            goto L_8019B40C;
                }
        r3 = *(u32*)((u8*)r31 + 0x4);
        r4 = 0x43300000;
        r5 = *(u32*)((u8*)r31 + 0x4);
        r6 = *(u8*)((u8*)r3 + 0x1);
        r3 = *(u32*)((u8*)r31 + 0x4);
        r6 = (s8)r6;
        r5 = *(u8*)((u8*)r5 + 0x0);
        r5 = (r5 & ~0xFFFFFF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0xFFFFFF00);
        r3 = r3 + 0x2;
        f1 = *(f64*)lbl_8047DA58;
        *(u32*)((u8*)r31 + 0x4) = r3;
        f2 = f0 - f1;
        goto L_8019B40C;
            }
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r6 = *(u8*)((u8*)r5 + 0x1);
    r5 = *(u8*)((u8*)r3 + 0x0);
    r5 = (r5 & ~0x0000FF00) | (((r6 << 8) | ((u32)r6 >> 24)) & 0x0000FF00);
    r3 = *(u32*)((u8*)r31 + 0x4);
    r3 = r3 + 0x2;
    f1 = *(f64*)lbl_8047DA58;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019B40C;
L_8019B404:
    f0 = *(f32*)lbl_8047DA3C;
    goto L_8019B42C;
L_8019B40C:
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047DA58;
    *(u32*)(sp + 0x28) = tmp;
    f0 = f0 - f1;
    f0 = f2 / f0;
L_8019B42C:
    *(f32*)((u8*)r31 + 0x24) = f0;
    tmp = 0x1;
    r3 = r30 - tmp;
    r4 = *(u8*)((u8*)r31 + 0x10);
    tmp = tmp - r30;
    tmp = ~(r3 | tmp);
    r3 = 0x4;
    r4 = r4 | 0x40;
    tmp = (s32)tmp >> 31;
    *(u8*)((u8*)r31 + 0x10) = r4;
    r3 = tmp + r3;
    if (r31 == 0) return;
    tmp = *(u8*)((u8*)r31 + 0x10);
    tmp = tmp & 0x000000F0;
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
    *(u8*)((u8*)r31 + 0x10) = tmp;
    return;

    r3 = 0x0;

    return;
}
