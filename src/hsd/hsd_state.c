
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
    if ((s32)tmp == 0) goto L_80199B48;
    if (r29 != 0) goto L_80199B3C;
    r3 = 0x0;
    goto L_80199B4C;
L_80199B3C:
    tmp = *(u8*)((u8*)r29 + 0x10);
    r3 = tmp & 0xF;
    goto L_80199B4C;
L_80199B48:
    r3 = 0x0;
L_80199B4C:
    if (r3 == 0) return;
    f2 = *(f32*)((u8*)r29 + 0x1C);
    f0 = *(f64*)lbl_8047DA40;
    f1 = f2 + f1;
    *(f32*)((u8*)r29 + 0x1C) = f1;
    f1 = *(f32*)((u8*)r29 + 0x1C);
    if (f1 < f0) return;
L_80199B70:
    if ((s32)r3 == 4) goto L_80199FCC;
    if ((s32)r3 >= 4) goto L_80199B94;
    if ((s32)r3 == 0) return;
    if ((s32)r3 < 0) goto L_80199B70;
    if ((s32)r3 >= 3) goto L_80199D80;
    goto L_80199D74;
L_80199B94:
    if ((s32)r3 == 6) goto L_80199BA4;
    if ((s32)r3 >= 6) goto L_80199B70;
    goto L_8019A208;
L_80199BA4:
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
    if ((s32)tmp == 2) goto L_80199C7C;
    if ((s32)tmp >= 2) goto L_80199C08;
    if ((s32)tmp >= 1) goto L_80199C3C;
    goto L_80199D58;
L_80199C08:
    if ((s32)tmp == 6) goto L_80199C18;
    if ((s32)tmp >= 6) goto L_80199D58;
    goto L_80199CFC;
L_80199C18:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000080;
    if ((s32)tmp == 6) return;
    f0 = *(f32*)((u8*)r29 + 0x20);
    *(f32*)(sp + 0x20) = f0;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x7F;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    goto L_80199D58;
L_80199C3C:
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
L_80199C7C:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000020;
    if (f2 == f0) goto L_80199CE4;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0xFFFFFFDF;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_80199CD4;
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
L_80199CD4:
    f0 = *(f32*)lbl_8047DA3C;
    *(f32*)((u8*)r29 + 0x28) = f0;
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)((u8*)r29 + 0x20) = f0;
L_80199CE4:
    f2 = *(f32*)((u8*)r29 + 0x28);
    f1 = *(f32*)((u8*)r29 + 0x1C);
    f0 = *(f32*)((u8*)r29 + 0x20);
    f0 = f2 * f1 + f0;
    *(f32*)(sp + 0x20) = f0;
    goto L_80199D58;
L_80199CFC:
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_80199D50;
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
L_80199D50:
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
    goto L_80199B70;
L_80199D80:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000080;
    if (tmp == 0) goto L_80199F18;
    if (r31 == 0) goto L_80199F18;
    tmp = *(u8*)((u8*)r29 + 0x12);
    if ((s32)tmp == 2) goto L_80199E24;
    if ((s32)tmp >= 2) goto L_80199DB0;
    if ((s32)tmp >= 1) goto L_80199DE4;
    goto L_80199F00;
L_80199DB0:
    if ((s32)tmp == 6) goto L_80199DC0;
    if ((s32)tmp >= 6) goto L_80199F00;
    goto L_80199EA4;
L_80199DC0:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000080;
    if ((s32)tmp == 6) goto L_80199F18;
    f0 = *(f32*)((u8*)r29 + 0x20);
    *(f32*)(sp + 0x14) = f0;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x7F;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    goto L_80199F00;
L_80199DE4:
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
    goto L_80199F00;
L_80199E24:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000020;
    if (f2 == f0) goto L_80199E8C;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0xFFFFFFDF;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_80199E7C;
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
L_80199E7C:
    f0 = *(f32*)lbl_8047DA3C;
    *(f32*)((u8*)r29 + 0x28) = f0;
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)((u8*)r29 + 0x20) = f0;
L_80199E8C:
    f2 = *(f32*)((u8*)r29 + 0x28);
    f1 = *(f32*)((u8*)r29 + 0x1C);
    f0 = *(f32*)((u8*)r29 + 0x20);
    f0 = f2 * f1 + f0;
    *(f32*)(sp + 0x14) = f0;
    goto L_80199F00;
L_80199EA4:
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_80199EF8;
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
    goto L_80199F00;
L_80199EF8:
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)(sp + 0x14) = f0;
L_80199F00:
    r12 = r31;
    r3 = r30;
    r5 = (u32)sp + 0x14;
    r4 = *(u8*)((u8*)r29 + 0x13);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80199F18:
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
    if (r3 < tmp) goto L_80199F6C;
    r3 = 0x6;
    goto L_80199B70;
L_80199F6C:
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
    goto L_80199B70;
L_80199FCC:
    r3 = *(u16*)((u8*)r29 + 0x1A);
    tmp = 0x43300000;
    *(u32*)(sp + 0x30) = tmp;
    f2 = *(f64*)lbl_8047DA50;
    f0 = *(f32*)((u8*)r29 + 0x1C);
    f1 = f1 - f2;
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_8019A05C;
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
    if (r29 == 0) goto L_80199B70;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x000000F0;
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
    *(u8*)((u8*)r29 + 0x10) = tmp;
    goto L_80199B70;
L_8019A05C:
    if (r31 == 0) goto L_8019A1E8;
    tmp = *(u8*)((u8*)r29 + 0x12);
    if ((s32)tmp == 2) goto L_8019A0F4;
    if ((s32)tmp >= 2) goto L_8019A080;
    if ((s32)tmp >= 1) goto L_8019A0B4;
    goto L_8019A1D0;
L_8019A080:
    if ((s32)tmp == 6) goto L_8019A090;
    if ((s32)tmp >= 6) goto L_8019A1D0;
    goto L_8019A174;
L_8019A090:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000080;
    if ((s32)tmp == 6) goto L_8019A1E8;
    f0 = *(f32*)((u8*)r29 + 0x20);
    *(f32*)(sp + 0x8) = f0;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x7F;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    goto L_8019A1D0;
L_8019A0B4:
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
    goto L_8019A1D0;
L_8019A0F4:
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x00000020;
    if (f2 == f0) goto L_8019A15C;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0xFFFFFFDF;
    *(u8*)((u8*)r29 + 0x10) = tmp;
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_8019A14C;
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
L_8019A14C:
    f0 = *(f32*)lbl_8047DA3C;
    *(f32*)((u8*)r29 + 0x28) = f0;
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)((u8*)r29 + 0x20) = f0;
L_8019A15C:
    f2 = *(f32*)((u8*)r29 + 0x28);
    f1 = *(f32*)((u8*)r29 + 0x1C);
    f0 = *(f32*)((u8*)r29 + 0x20);
    f0 = f2 * f1 + f0;
    *(f32*)(sp + 0x8) = f0;
    goto L_8019A1D0;
L_8019A174:
    tmp = *(u16*)((u8*)r29 + 0x1A);
    if (tmp == 0) goto L_8019A1C8;
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
    goto L_8019A1D0;
L_8019A1C8:
    f0 = *(f32*)((u8*)r29 + 0x24);
    *(f32*)(sp + 0x8) = f0;
L_8019A1D0:
    r12 = r31;
    r3 = r30;
    r5 = (u32)sp + 0x8;
    r4 = *(u8*)((u8*)r29 + 0x13);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8019A1E8:
    r3 = 0x5;
    if (r29 == 0) return;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x000000F0;
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
    *(u8*)((u8*)r29 + 0x10) = tmp;
    return;
L_8019A208:
    r3 = 0x4;
    if (r29 == 0) goto L_80199B70;
    tmp = *(u8*)((u8*)r29 + 0x10);
    tmp = tmp & 0x000000F0;
    tmp = (tmp & ~0x0000000F) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x0000000F);
    *(u8*)((u8*)r29 + 0x10) = tmp;
    goto L_80199B70;

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
    if (tmp != 0) goto L_8019A2FC;
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
    if (tmp != 0) goto L_8019A2D0;
    goto L_8019A2F8;
L_8019A2D0:
    r3 = *(u32*)((u8*)r31 + 0x4);
    tmp = r3 + 0x1;
    *(u32*)((u8*)r31 + 0x4) = tmp;
    r3 = *(u8*)((u8*)r3 + 0x0);
    tmp = r3 & 0x00000080;
    r3 = r3 & 0x7F;
    r3 = r3 << r4;
    r4 = r4 + 0x7;
    r5 = r5 + r3;
    if (tmp != 0) goto L_8019A2D0;
L_8019A2F8:
    *(u16*)((u8*)r31 + 0x16) = r5;
L_8019A2FC:
    r3 = *(u16*)((u8*)r31 + 0x16);
    *(u16*)((u8*)r31 + 0x16) = tmp;
    tmp = *(u8*)((u8*)r31 + 0x11);
    if ((s32)tmp == 4) goto L_8019AAFC;
    if ((s32)tmp >= 4) goto L_8019A330;
    if ((s32)tmp == 2) goto L_8019A5D8;
    if ((s32)tmp >= 2) goto L_8019A870;
    if ((s32)tmp >= 1) goto L_8019A340;
    r3 = 0x0;
    return;
L_8019A330:
    if ((s32)tmp == 6) goto L_8019B1C0;
    if ((s32)tmp >= 6) { r3 = 0x0; return; }
    goto L_8019AF60;
L_8019A340:
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
    if ((s32)r5 != 0) goto L_8019A42C;
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
L_8019A42C:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019A46C;
    if ((s32)r3 >= 0x60) goto L_8019A460;
    if ((s32)r3 == 0x40) goto L_8019A518;
    if ((s32)r3 >= 0x40) goto L_8019A558;
    if ((s32)r3 == 0x20) goto L_8019A4D4;
    goto L_8019A558;
L_8019A460:
    if ((s32)r3 == 0x80) goto L_8019A4A4;
    goto L_8019A558;
L_8019A46C:
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
L_8019A4A4:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019A560;
L_8019A4D4:
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
L_8019A518:
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
L_8019A5D8:
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
    if ((s32)r5 != 0) goto L_8019A6C4;
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
L_8019A6C4:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019A704;
    if ((s32)r3 >= 0x60) goto L_8019A6F8;
    if ((s32)r3 == 0x40) goto L_8019A7B0;
    if ((s32)r3 >= 0x40) goto L_8019A7F0;
    if ((s32)r3 == 0x20) goto L_8019A76C;
    goto L_8019A7F0;
L_8019A6F8:
    if ((s32)r3 == 0x80) goto L_8019A73C;
    goto L_8019A7F0;
L_8019A704:
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
L_8019A73C:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019A7F8;
L_8019A76C:
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
L_8019A7B0:
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
L_8019A870:
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
    if ((s32)r5 != 0) goto L_8019A964;
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
L_8019A964:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019A9A4;
    if ((s32)r3 >= 0x60) goto L_8019A998;
    if ((s32)r3 == 0x40) goto L_8019AA50;
    if ((s32)r3 >= 0x40) goto L_8019AA90;
    if ((s32)r3 == 0x20) goto L_8019AA0C;
    goto L_8019AA90;
L_8019A998:
    if ((s32)r3 == 0x80) goto L_8019A9DC;
    goto L_8019AA90;
L_8019A9A4:
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
L_8019A9DC:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019AA98;
L_8019AA0C:
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
L_8019AA50:
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
L_8019AAFC:
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
    if ((s32)r5 != 0) goto L_8019ABE8;
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
L_8019ABE8:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019AC28;
    if ((s32)r3 >= 0x60) goto L_8019AC1C;
    if ((s32)r3 == 0x40) goto L_8019ACD4;
    if ((s32)r3 >= 0x40) goto L_8019AD14;
    if ((s32)r3 == 0x20) goto L_8019AC90;
    goto L_8019AD14;
L_8019AC1C:
    if ((s32)r3 == 0x80) goto L_8019AC60;
    goto L_8019AD14;
L_8019AC28:
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
L_8019AC60:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019AD1C;
L_8019AC90:
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
L_8019ACD4:
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
    if ((s32)r3 != 0x80) goto L_8019ADD0;
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
L_8019ADD0:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019AE10;
    if ((s32)r3 >= 0x60) goto L_8019AE04;
    if ((s32)r3 == 0x40) goto L_8019AEBC;
    if ((s32)r3 >= 0x40) goto L_8019AEFC;
    if ((s32)r3 == 0x20) goto L_8019AE78;
    goto L_8019AEFC;
L_8019AE04:
    if ((s32)r3 == 0x80) goto L_8019AE48;
    goto L_8019AEFC;
L_8019AE10:
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
L_8019AE48:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019AF04;
L_8019AE78:
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
L_8019AEBC:
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
    if ((s32)r5 != 0) goto L_8019B04C;
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
L_8019B04C:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019B08C;
    if ((s32)r3 >= 0x60) goto L_8019B080;
    if ((s32)r3 == 0x40) goto L_8019B138;
    if ((s32)r3 >= 0x40) goto L_8019B178;
    if ((s32)r3 == 0x20) goto L_8019B0F4;
    goto L_8019B178;
L_8019B080:
    if ((s32)r3 == 0x80) goto L_8019B0C4;
    goto L_8019B178;
L_8019B08C:
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
L_8019B0C4:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019B180;
L_8019B0F4:
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
L_8019B138:
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
L_8019B1C0:
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
    if ((s32)r5 != 0) goto L_8019B2D8;
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
L_8019B2D8:
    r3 = r5 & 0x000000E0;
    r4 = 0x1;
    tmp = r5 & 0x1F;
    tmp = r4 << tmp;
    if ((s32)r3 == 0x60) goto L_8019B318;
    if ((s32)r3 >= 0x60) goto L_8019B30C;
    if ((s32)r3 == 0x40) goto L_8019B3C4;
    if ((s32)r3 >= 0x40) goto L_8019B404;
    if ((s32)r3 == 0x20) goto L_8019B380;
    goto L_8019B404;
L_8019B30C:
    if ((s32)r3 == 0x80) goto L_8019B350;
    goto L_8019B404;
L_8019B318:
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
L_8019B350:
    r5 = *(u32*)((u8*)r31 + 0x4);
    r4 = 0x43300000;
    r3 = *(u32*)((u8*)r31 + 0x4);
    r5 = *(u8*)((u8*)r5 + 0x0);
    r3 = r3 + 0x1;
    f1 = *(f64*)lbl_8047DA50;
    *(u32*)((u8*)r31 + 0x4) = r3;
    f2 = f0 - f1;
    goto L_8019B40C;
L_8019B380:
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
L_8019B3C4:
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

