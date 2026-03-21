/**
 * @file hsd_jobj_display.c
 * @brief HSD internal functions (0x801A1988-0x801A3FBC).
 *
 * Stub coverage for 20 functions.
 */

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

/* 0x78 | fn_801A1988 | generic */
u32 fn_801A1988(void) {
    fn_80196E10();
    fn_8019D9DC();
    return 1;
}

/* 0x801A1A00 | 0x140 */
void fn_801A1A00(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_800A2D98();
    extern void fn_800A2EB4();
    extern void fn_801942B8();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r5;
    /* mr. r29, r4 */;
    r28 = r3;
    if ((s32)tmp == 0) goto L_801A1A74;
    if (r29 != 0) goto L_801A1A44;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1A44:
    tmp = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r29 != 0) goto L_801A1A64;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00000040;
    if (r29 == 0) goto L_801A1A64;
    r3 = 0x1;
L_801A1A64:
    if ((s32)r3 == 0) goto L_801A1A74;
    r3 = r29;
    fn_8019D9DC();
L_801A1A74:
    r31 = *(u32*)((u8*)r29 + 0x10);
    if (r31 == 0) goto L_801A1AC8;
    if (r31 != 0) goto L_801A1A98;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1A98:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A1AB8;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A1AB8;
    r3 = 0x1;
L_801A1AB8:
    if ((s32)r3 == 0) goto L_801A1AC8;
    r3 = r31;
    fn_8019D9DC();
L_801A1AC8:
    r3 = *(u32*)((u8*)r29 + 0x10);
    r4 = r30;
    r3 = r3 + 0x44;
    fn_800A2EB4();
    r4 = r30;
    r5 = r30;
    r3 = r29 + 0x44;
    fn_800A2D98();
    if (r28 == 0) goto L_801A1B04;
    r3 = r28;
    r4 = r30;
    r5 = r30;
    fn_800A2D98();
    goto L_801A1B20;
L_801A1B04:
    fn_801942B8();
    if (r3 == 0) goto L_801A1B20;
    r3 = r3 + 0x54;
    r4 = r30;
    r5 = r30;
    fn_800A2D98();
L_801A1B20:
    return;
}

/* 0x801A1B40 | 0x3C */
void fn_801A1B40(void) {
    extern void fn_801A1B7C();
    extern void fn_801C2A04();
    extern void fn_801C2A60();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_801A1B68;
    fn_801C2A60();
    r3 = r31;
    fn_801A1B7C();
    fn_801C2A04();
L_801A1B68:
    return;
}

/* 0x801A1B7C | 0x3B0 */
void fn_801A1B7C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_80199568();
    extern void fn_8019D980();
    extern void fn_801A1B7C();
    extern void fn_801A1F2C();
    extern void fn_801A3D04();
    extern void fn_801B0040();
    extern void fn_801C27F4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_801A1F0C;
    if (r31 == 0) goto L_801A1D10;
    if (r31 == 0) goto L_801A1CD8;
    if (r31 != 0) goto L_801A1BC8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1BC8:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A1BE8;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A1BE8;
    r3 = 0x1;
L_801A1BE8:
    if ((s32)r3 != 0) goto L_801A1CD8;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00800000;
    if ((s32)r3 == 0) goto L_801A1C68;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x01000000;
    if ((s32)r3 != 0) goto L_801A1CD8;
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A1CD8;
    r30 = *(u32*)((u8*)r31 + 0xC);
    if (r30 != 0) goto L_801A1C30;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1C30:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A1C50;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A1C50;
    r3 = 0x1;
L_801A1C50:
    if ((s32)r3 == 0) goto L_801A1CD8;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    goto L_801A1CD8;
L_801A1C68:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A1C84;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000040;
    if (tmp != 0) goto L_801A1CCC;
L_801A1C84:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x20 */;
    if (tmp == 0) goto L_801A1CCC;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x40 */;
    if (tmp == 0) goto L_801A1CCC;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x60 */;
    if (tmp == 0) goto L_801A1CCC;
    tmp = *(u32*)((u8*)r31 + 0x80);
    if (tmp == 0) goto L_801A1CD8;
L_801A1CCC:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
L_801A1CD8:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801C27F4();
    r3 = *(u32*)((u8*)r31 + 0x80);
    fn_801B0040();
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp == 0) goto L_801A1D10;
    r3 = *(u32*)((u8*)r31 + 0x18);
    fn_80199568();
L_801A1D10:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (tmp != 0) goto L_801A1F0C;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A1F04;
L_801A1D24:
    if (r31 == 0) goto L_801A1F00;
    if (r31 == 0) goto L_801A1E38;
    if (r31 == 0) goto L_801A1E00;
    r3 = r31;
    fn_8019D980();
    if ((s32)r3 != 0) goto L_801A1E00;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00800000;
    if ((s32)r3 == 0) goto L_801A1D90;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x01000000;
    if ((s32)r3 != 0) goto L_801A1E00;
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A1E00;
    r3 = *(u32*)((u8*)r31 + 0xC);
    fn_8019D980();
    if ((s32)r3 == 0) goto L_801A1E00;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    goto L_801A1E00;
L_801A1D90:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A1DAC;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000040;
    if (tmp != 0) goto L_801A1DF4;
L_801A1DAC:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x20 */;
    if (tmp == 0) goto L_801A1DF4;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x40 */;
    if (tmp == 0) goto L_801A1DF4;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x60 */;
    if (tmp == 0) goto L_801A1DF4;
    tmp = *(u32*)((u8*)r31 + 0x80);
    if (tmp == 0) goto L_801A1E00;
L_801A1DF4:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
L_801A1E00:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801C27F4();
    r3 = *(u32*)((u8*)r31 + 0x80);
    fn_801B0040();
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp == 0) goto L_801A1E38;
    r3 = *(u32*)((u8*)r31 + 0x18);
    fn_80199568();
L_801A1E38:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (tmp != 0) goto L_801A1F00;
    r30 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A1EF8;
L_801A1E4C:
    if (r30 == 0) goto L_801A1EF4;
    if (r30 == 0) goto L_801A1E9C;
    r3 = r30;
    fn_801A3D04();
    r5 = *(u32*)((u8*)r30 + 0x0);
    r4 = r30;
    r3 = *(u32*)((u8*)r30 + 0x7C);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801C27F4();
    r3 = *(u32*)((u8*)r30 + 0x80);
    fn_801B0040();
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r30 == 0) goto L_801A1E9C;
    r3 = *(u32*)((u8*)r30 + 0x18);
    fn_80199568();
L_801A1E9C:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00001000;
    if (r30 != 0) goto L_801A1EF4;
    r28 = *(u32*)((u8*)r30 + 0x10);
    goto L_801A1EEC;
L_801A1EB0:
    if (r28 == 0) goto L_801A1EE8;
    r3 = r28;
    fn_801A1F2C();
    tmp = *(u32*)((u8*)r28 + 0x14);
    tmp = tmp & 0x00001000;
    if (r28 != 0) goto L_801A1EE8;
    r29 = *(u32*)((u8*)r28 + 0x10);
    goto L_801A1EE0;
L_801A1ED4:
    r3 = r29;
    fn_801A1B7C();
    r29 = *(u32*)((u8*)r29 + 0x8);
L_801A1EE0:
    if (r29 != 0) goto L_801A1ED4;
L_801A1EE8:
    r28 = *(u32*)((u8*)r28 + 0x8);
L_801A1EEC:
    if (r28 != 0) goto L_801A1EB0;
L_801A1EF4:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A1EF8:
    if (r30 != 0) goto L_801A1E4C;
L_801A1F00:
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A1F04:
    if (r31 != 0) goto L_801A1D24;
L_801A1F0C:
    return;
}

/* 0x801A1F2C | 0x19C */
void fn_801A1F2C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_80199568();
    extern void fn_801B0040();
    extern void fn_801C27F4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_801A20B0;
    if (r31 == 0) goto L_801A2078;
    if (r31 != 0) goto L_801A1F68;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1F68:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A1F88;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A1F88;
    r3 = 0x1;
L_801A1F88:
    if ((s32)r3 != 0) goto L_801A2078;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00800000;
    if ((s32)r3 == 0) goto L_801A2008;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x01000000;
    if ((s32)r3 != 0) goto L_801A2078;
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A2078;
    r30 = *(u32*)((u8*)r31 + 0xC);
    if (r30 != 0) goto L_801A1FD0;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A1FD0:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A1FF0;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A1FF0;
    r3 = 0x1;
L_801A1FF0:
    if ((s32)r3 == 0) goto L_801A2078;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    goto L_801A2078;
L_801A2008:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A2024;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000040;
    if (tmp != 0) goto L_801A206C;
L_801A2024:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x20 */;
    if (tmp == 0) goto L_801A206C;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x40 */;
    if (tmp == 0) goto L_801A206C;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x60 */;
    if (tmp == 0) goto L_801A206C;
    tmp = *(u32*)((u8*)r31 + 0x80);
    if (tmp == 0) goto L_801A2078;
L_801A206C:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
L_801A2078:
    r5 = *(u32*)((u8*)r31 + 0x0);
    r4 = r31;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    r5 = *(u32*)((u8*)r5 + 0x50);
    fn_801C27F4();
    r3 = *(u32*)((u8*)r31 + 0x80);
    fn_801B0040();
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp == 0) goto L_801A20B0;
    r3 = *(u32*)((u8*)r31 + 0x18);
    fn_80199568();
L_801A20B0:
    return;
}

/* 0x801A20C8 | 0xA94 */
void fn_801A20C8(void) {
    extern u8 lbl_80274AA0[];
    extern u8 lbl_8047B29C[];
    extern u8 lbl_8047B2A0[];
    extern u8 lbl_8047B2A4[];
    extern u8 lbl_8047B2A8[];
    extern u8 lbl_8047DB20[];
    extern u8 lbl_8047DB30[];
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern u8 lbl_8047DB48[];
    extern u8 lbl_8047DB50[];
    extern u8 lbl_8047DB60[];
    extern u8 lbl_8047DB88[];
    extern u8 lbl_8047DB90[];
    extern u8 lbl_8047DB98[];
    extern void fn_800A2D64();
    extern void fn_80196E10();
    extern void fn_8019D620();
    extern void fn_8019F7F0();
    extern void fn_8019FB90();
    extern void fn_801A8D1C();
    extern void fn_801A9570();
    extern void fn_801A98CC();
    extern void fn_801A9DF0();
    extern void fn_801B00E0();
    extern void fn_801B1890();
    extern u8 jumptable_8036C934[];
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* mr. r30, r3 */;
    r6 = (u32)lbl_80274AA0;
    r27 = r4;
    r29 = r5;
    r31 = (u32)lbl_80274AA0;
    if ((s32)tmp == 0) goto L_801A2B40;
    if (r27 > 0x39) goto L_801A2B40;
    r3 = (u32)jumptable_8036C934;
    tmp = r27 << 2;
    r3 = (u32)jumptable_8036C934;
    r3 = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))r3;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f0 = *(f64*)lbl_8047DB60;
    if (f1 >= f0) goto L_801A2130;
    f0 = *(f32*)lbl_8047DB48;
    *(f32*)((u8*)r29 + 0x0) = f0;
L_801A2130:
    f1 = *(f64*)lbl_8047DB90;
    f0 = *(f32*)((u8*)r29 + 0x0);
    if (f1 >= f0) goto L_801A2148;
    f0 = *(f32*)lbl_8047DB30;
    *(f32*)((u8*)r29 + 0x0) = f0;
L_801A2148:
    tmp = *(u32*)((u8*)r30 + 0x7C);
    if (tmp != 0) goto L_801A2164;
    r5 = r31 + 0x260;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x24e;
    fn_80196E10();
L_801A2164:
    r3 = *(u32*)((u8*)r30 + 0x7C);
    r28 = *(u32*)((u8*)r3 + 0x18);
    if (r28 != 0) goto L_801A2184;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x250;
    r5 = (u32)lbl_8047DB88;
    fn_80196E10();
L_801A2184:
    tmp = *(u32*)((u8*)r28 + 0x18);
    if (tmp != 0) goto L_801A21A0;
    r5 = r31 + 0x26c;
    r3 = (u32)lbl_8047DB20;
    r4 = 0x251;
    fn_80196E10();
L_801A21A0:
    r4 = *(u32*)((u8*)r28 + 0x18);
    r3 = r1 + 0x8;
    f1 = *(f32*)((u8*)r29 + 0x0);
    fn_801B1890();
    f31 = *(f32*)(sp + 0x8);
    if (r30 != 0) goto L_801A21CC;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3b8;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A21CC:
    *(f32*)((u8*)r30 + 0x38) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A222C;
    if (r30 == 0) goto L_801A222C;
    if (r30 != 0) goto L_801A21FC;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A21FC:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A221C;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A221C;
    r3 = 0x1;
L_801A221C:
    if ((s32)r3 != 0) goto L_801A222C;
    r3 = r30;
    fn_8019D620();
L_801A222C:
    f31 = *(f32*)(sp + 0xC);
    if (r30 != 0) goto L_801A2248;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3c6;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2248:
    *(f32*)((u8*)r30 + 0x3C) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A22A8;
    if (r30 == 0) goto L_801A22A8;
    if (r30 != 0) goto L_801A2278;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2278:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2298;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2298;
    r3 = 0x1;
L_801A2298:
    if ((s32)r3 != 0) goto L_801A22A8;
    r3 = r30;
    fn_8019D620();
L_801A22A8:
    f31 = *(f32*)(sp + 0x10);
    if (r30 != 0) goto L_801A22C4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3d4;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A22C4:
    *(f32*)((u8*)r30 + 0x40) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A22F4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A22F4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2314;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2314;
    r3 = 0x1;
L_801A2314:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00200000;
    if ((s32)r3 == 0) goto L_801A2354;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = 0x40000000;
    r5 = 0x0;
    fn_801B00E0();
    if (r3 == 0) goto L_801A2354;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r3 + 0xC) = f0;
L_801A2354:
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A2370;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2a4;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2370:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00020000;
    if (r30 == 0) goto L_801A238C;
    r5 = r31 + 0x27c;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2a5;
    fn_80196E10();
L_801A238C:
    *(f32*)((u8*)r30 + 0x1C) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A23BC;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A23BC:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A23DC;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A23DC;
    r3 = 0x1;
L_801A23DC:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A240C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2b8;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A240C:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00020000;
    if (r30 == 0) goto L_801A2428;
    r5 = r31 + 0x27c;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2b9;
    fn_80196E10();
L_801A2428:
    *(f32*)((u8*)r30 + 0x20) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A2458;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2458:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2478;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2478;
    r3 = 0x1;
L_801A2478:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A24A8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2cc;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A24A8:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00020000;
    if (r30 == 0) goto L_801A24C4;
    r5 = r31 + 0x27c;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x2cd;
    fn_80196E10();
L_801A24C4:
    *(f32*)((u8*)r30 + 0x24) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A24F4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A24F4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2514;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2514;
    r3 = 0x1;
L_801A2514:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A2544;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3b8;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2544:
    *(f32*)((u8*)r30 + 0x38) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A2574;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2574:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2594;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2594;
    r3 = 0x1;
L_801A2594:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A25C4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3c6;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A25C4:
    *(f32*)((u8*)r30 + 0x3C) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A25F4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A25F4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2614;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2614;
    r3 = 0x1;
L_801A2614:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A2644;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x3d4;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2644:
    *(f32*)((u8*)r30 + 0x40) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A2674;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2674:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2694;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2694;
    r3 = 0x1;
L_801A2694:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A26C4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x325;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A26C4:
    *(f32*)((u8*)r30 + 0x2C) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A26F4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A26F4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2714;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2714;
    r3 = 0x1;
L_801A2714:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A2744;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x333;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2744:
    *(f32*)((u8*)r30 + 0x30) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A2774;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2774:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2794;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2794;
    r3 = 0x1;
L_801A2794:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f31 = *(f32*)((u8*)r29 + 0x0);
    if (r30 != 0) goto L_801A27C4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x341;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A27C4:
    *(f32*)((u8*)r30 + 0x34) = f31;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x02000000;
    if (r30 != 0) goto L_801A2B40;
    if (r30 == 0) goto L_801A2B40;
    if (r30 != 0) goto L_801A27F4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A27F4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2814;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2814;
    r3 = 0x1;
L_801A2814:
    if ((s32)r3 != 0) goto L_801A2B40;
    r3 = r30;
    fn_8019D620();
    goto L_801A2B40;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f0 = *(f64*)lbl_8047DB50;
    if (f1 <= f0) goto L_801A2848;
    r3 = r30;
    r4 = 0x10;
    fn_8019F7F0();
    goto L_801A2B40;
L_801A2848:
    r3 = r30;
    r4 = 0x10;
    fn_8019FB90();
    goto L_801A2B40;
    f1 = *(f32*)((u8*)r29 + 0x0);
    f0 = *(f64*)lbl_8047DB50;
    if (f1 <= f0) goto L_801A28E0;
    if (r30 == 0) goto L_801A2B40;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp ^ 0x10;
    tmp = tmp & 0x00000008;
    if (r30 == 0) goto L_801A28D0;
    if (r30 == 0) goto L_801A28D0;
    if (r30 != 0) goto L_801A28A0;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A28A0:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A28C0;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A28C0;
    r3 = 0x1;
L_801A28C0:
    if ((s32)r3 != 0) goto L_801A28D0;
    r3 = r30;
    fn_8019D620();
L_801A28D0:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0xFFFFFFEF;
    *(u32*)((u8*)r30 + 0x14) = tmp;
    goto L_801A2B40;
L_801A28E0:
    if (r30 == 0) goto L_801A2B40;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp ^ 0x10;
    tmp = tmp & 0x00000008;
    if (r30 == 0) goto L_801A2948;
    if (r30 == 0) goto L_801A2948;
    if (r30 != 0) goto L_801A2918;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2918:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A2938;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A2938;
    r3 = 0x1;
L_801A2938:
    if ((s32)r3 != 0) goto L_801A2948;
    r3 = r30;
    fn_8019D620();
L_801A2948:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp | 0x10;
    *(u32*)((u8*)r30 + 0x14) = tmp;
    goto L_801A2B40;
    r28 = *(u32*)lbl_8047B29C;
    goto L_801A2998;
L_801A2960:
    r4 = *(u32*)((u8*)r29 + 0x0);
    tmp = 0x43300000;
    *(u32*)(sp + 0x48) = tmp;
    r3 = r30;
    f1 = *(f64*)lbl_8047DB98;
    *(u32*)(sp + 0x4C) = tmp;
    r4 = r27;
    r12 = *(u32*)((u8*)r28 + 0x4);
    f1 = f0 - f1;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r28 = *(u32*)((u8*)r28 + 0x0);
L_801A2998:
    if (r28 != 0) goto L_801A2960;
    goto L_801A2B40;
    r28 = *(u32*)lbl_8047B29C;
    goto L_801A29C8;
L_801A29AC:
    r12 = *(u32*)((u8*)r28 + 0x4);
    r3 = r30;
    r4 = r27;
    f1 = *(f32*)((u8*)r29 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r28 = *(u32*)((u8*)r28 + 0x0);
L_801A29C8:
    if (r28 != 0) goto L_801A29AC;
    goto L_801A2B40;
    tmp = *(u32*)lbl_8047B2A0;
    r3 = *(u32*)((u8*)r29 + 0x0);
    r4 = *(u32*)((u8*)r29 + 0x0);
    /* extrwi r5, r3, 24, 2 */;
    r4 = r4 & 0x3F;
    if (tmp == 0) goto L_801A2B40;
    r12 = *(u32*)lbl_8047B2A0;
    r6 = r30;
    r3 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_801A2B40;
    tmp = *(u32*)lbl_8047B2A4;
    if (tmp == 0) goto L_801A2B40;
    r12 = *(u32*)lbl_8047B2A4;
    r3 = *(u32*)((u8*)r29 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_801A2B40;
    tmp = *(u32*)lbl_8047B2A8;
    if (tmp == 0) goto L_801A2B40;
    r12 = *(u32*)lbl_8047B2A8;
    r3 = r30;
    r4 = *(u32*)((u8*)r29 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_801A2B40;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r30 + 0x44) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r30 + 0x54) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r30 + 0x64) = f0;
    goto L_801A2B40;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r30 + 0x48) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r30 + 0x58) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r30 + 0x68) = f0;
    goto L_801A2B40;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r30 + 0x4C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r30 + 0x5C) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r30 + 0x6C) = f0;
    goto L_801A2B40;
    f0 = *(f32*)((u8*)r29 + 0x0);
    *(f32*)((u8*)r30 + 0x50) = f0;
    f0 = *(f32*)((u8*)r29 + 0x4);
    *(f32*)((u8*)r30 + 0x60) = f0;
    f0 = *(f32*)((u8*)r29 + 0x8);
    *(f32*)((u8*)r30 + 0x70) = f0;
    goto L_801A2B40;
    tmp = *(u32*)((u8*)r30 + 0xC);
    if (tmp == 0) goto L_801A2AE0;
    r3 = *(u32*)((u8*)r30 + 0xC);
    r4 = r30 + 0x44;
    r5 = r1 + 0x14;
    r3 = r3 + 0x44;
    fn_801A9DF0();
    goto L_801A2AEC;
L_801A2AE0:
    r3 = r30 + 0x44;
    r4 = r1 + 0x14;
    fn_800A2D64();
L_801A2AEC:
    if (r27 == 0x36) goto L_801A2AFC;
    if (r27 != 0x38) goto L_801A2B08;
L_801A2AFC:
    r3 = r1 + 0x14;
    r4 = r30 + 0x38;
    fn_801A9570();
L_801A2B08:
    if (r27 == 0x36) goto L_801A2B18;
    if (r27 != 0x37) goto L_801A2B24;
L_801A2B18:
    r3 = r1 + 0x14;
    r4 = r30 + 0x1c;
    fn_801A98CC();
L_801A2B24:
    if (r27 == 0x36) goto L_801A2B34;
    if (r27 != 0x39) goto L_801A2B40;
L_801A2B34:
    r3 = r1 + 0x14;
    r4 = r30 + 0x2c;
    fn_801A8D1C();
L_801A2B40:
    return;
}

/* 0x801A2B5C | 0x4C0 */
void fn_801A2B5C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_80199654();
    extern void fn_8019D620();
    extern void fn_8019FAEC();
    extern void fn_8019FE8C();
    extern void fn_801A2B5C();
    extern void fn_801A301C();
    extern void fn_801A323C();
    extern void fn_801AFE68();
    extern void fn_801C25E4();
    extern void fn_801C2670();
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

    /* mr. r31, r3 */;
    r30 = r4;
    r29 = r5;
    r28 = r6;
    if ((s32)tmp == 0) goto L_801A3008;
    if (r31 == 0) goto L_801A2D58;
    if (r30 == 0) goto L_801A2D14;
    tmp = *(u32*)((u8*)r31 + 0x7C);
    if (tmp == 0) goto L_801A2BA4;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    fn_801C25E4();
L_801A2BA4:
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801C2670();
    *(u32*)((u8*)r31 + 0x7C) = r3;
    r6 = *(u32*)((u8*)r31 + 0x7C);
    if (r6 == 0) goto L_801A2C10;
    tmp = *(u32*)((u8*)r6 + 0x14);
    if (tmp == 0) goto L_801A2C10;
    r5 = r6 + 0x14;
    goto L_801A2C04;
L_801A2BD0:
    r3 = *(u32*)((u8*)r5 + 0x0);
    tmp = *(u8*)((u8*)r3 + 0x13);
    if (tmp != 0xc) goto L_801A2C00;
    r3 = *(u32*)((u8*)r5 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x0);
    tmp = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r6 + 0x14);
    *(u32*)((u8*)r4 + 0x0) = tmp;
    *(u32*)((u8*)r6 + 0x14) = r4;
    goto L_801A2C10;
L_801A2C00:
    r5 = *(u32*)((u8*)r5 + 0x0);
L_801A2C04:
    tmp = *(u32*)((u8*)r5 + 0x0);
    if (tmp != 0) goto L_801A2BD0;
L_801A2C10:
    r3 = *(u32*)((u8*)r31 + 0x80);
    r4 = *(u32*)((u8*)r30 + 0xC);
    fn_801AFE68();
    tmp = *(u32*)((u8*)r30 + 0x10);
    tmp = tmp & 0x1;
    if (tmp == 0) goto L_801A2CA0;
    if (r31 == 0) goto L_801A2D14;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp ^ 0x8;
    tmp = tmp & 0x00000008;
    if (r31 == 0) goto L_801A2C90;
    if (r31 == 0) goto L_801A2C90;
    if (r31 != 0) goto L_801A2C60;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2C60:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A2C80;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A2C80;
    r3 = 0x1;
L_801A2C80:
    if ((s32)r3 != 0) goto L_801A2C90;
    r3 = r31;
    fn_8019D620();
L_801A2C90:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x8;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    goto L_801A2D14;
L_801A2CA0:
    if (r31 == 0) goto L_801A2D14;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp ^ 0x8;
    tmp = tmp & 0x00000008;
    if (r31 == 0) goto L_801A2D08;
    if (r31 == 0) goto L_801A2D08;
    if (r31 != 0) goto L_801A2CD8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A2CD8:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A2CF8;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A2CF8;
    r3 = 0x1;
L_801A2CF8:
    if ((s32)r3 != 0) goto L_801A2D08;
    r3 = r31;
    fn_8019D620();
L_801A2D08:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0xFFFFFFF7;
    *(u32*)((u8*)r31 + 0x14) = tmp;
L_801A2D14:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if ((s32)r3 == 0) goto L_801A2D58;
    if (r28 == 0) goto L_801A2D38;
    r5 = *(u32*)((u8*)r28 + 0x8);
    goto L_801A2D3C;
L_801A2D38:
    r5 = 0x0;
L_801A2D3C:
    r3 = *(u32*)((u8*)r31 + 0x18);
    if (r29 == 0) goto L_801A2D50;
    r4 = *(u32*)((u8*)r29 + 0x8);
    goto L_801A2D54;
L_801A2D50:
    r4 = 0x0;
L_801A2D54:
    fn_80199654();
L_801A2D58:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (r29 != 0) goto L_801A3008;
    r31 = *(u32*)((u8*)r31 + 0x10);
    if (r30 == 0) goto L_801A2D78;
    r30 = *(u32*)((u8*)r30 + 0x0);
    goto L_801A2D7C;
L_801A2D78:
    r30 = 0x0;
L_801A2D7C:
    if (r29 == 0) goto L_801A2D8C;
    r29 = *(u32*)((u8*)r29 + 0x0);
    goto L_801A2D90;
L_801A2D8C:
    r29 = 0x0;
L_801A2D90:
    if (r28 == 0) goto L_801A2DA0;
    r28 = *(u32*)((u8*)r28 + 0x0);
    goto L_801A3000;
L_801A2DA0:
    r28 = 0x0;
    goto L_801A3000;
L_801A2DA8:
    if (r31 == 0) goto L_801A2FC0;
    if (r31 == 0) goto L_801A2E60;
    if (r30 == 0) goto L_801A2E1C;
    tmp = *(u32*)((u8*)r31 + 0x7C);
    if (tmp == 0) goto L_801A2DD4;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    fn_801C25E4();
L_801A2DD4:
    r3 = *(u32*)((u8*)r30 + 0x8);
    fn_801C2670();
    *(u32*)((u8*)r31 + 0x7C) = r3;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    fn_801A323C();
    r3 = *(u32*)((u8*)r31 + 0x80);
    r4 = *(u32*)((u8*)r30 + 0xC);
    fn_801AFE68();
    tmp = *(u32*)((u8*)r30 + 0x10);
    tmp = tmp & 0x1;
    if (tmp == 0) goto L_801A2E10;
    r3 = r31;
    r4 = 0x8;
    fn_8019FE8C();
    goto L_801A2E1C;
L_801A2E10:
    r3 = r31;
    r4 = 0x8;
    fn_8019FAEC();
L_801A2E1C:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (tmp == 0) goto L_801A2E60;
    if (r28 == 0) goto L_801A2E40;
    r5 = *(u32*)((u8*)r28 + 0x8);
    goto L_801A2E44;
L_801A2E40:
    r5 = 0x0;
L_801A2E44:
    r3 = *(u32*)((u8*)r31 + 0x18);
    if (r29 == 0) goto L_801A2E58;
    r4 = *(u32*)((u8*)r29 + 0x8);
    goto L_801A2E5C;
L_801A2E58:
    r4 = 0x0;
L_801A2E5C:
    fn_80199654();
L_801A2E60:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (r29 != 0) goto L_801A2FC0;
    r23 = *(u32*)((u8*)r31 + 0x10);
    if (r30 == 0) goto L_801A2E80;
    r22 = *(u32*)((u8*)r30 + 0x0);
    goto L_801A2E84;
L_801A2E80:
    r22 = 0x0;
L_801A2E84:
    if (r29 == 0) goto L_801A2E94;
    r21 = *(u32*)((u8*)r29 + 0x0);
    goto L_801A2E98;
L_801A2E94:
    r21 = 0x0;
L_801A2E98:
    if (r28 == 0) goto L_801A2EA8;
    r20 = *(u32*)((u8*)r28 + 0x0);
    goto L_801A2FB8;
L_801A2EA8:
    r20 = 0x0;
    goto L_801A2FB8;
L_801A2EB0:
    if (r23 == 0) goto L_801A2F78;
    r3 = r23;
    r4 = r22;
    r5 = r21;
    r6 = r20;
    fn_801A301C();
    tmp = *(u32*)((u8*)r23 + 0x14);
    tmp = tmp & 0x00001000;
    if (r23 != 0) goto L_801A2F78;
    r24 = *(u32*)((u8*)r23 + 0x10);
    if (r22 == 0) goto L_801A2EEC;
    r25 = *(u32*)((u8*)r22 + 0x0);
    goto L_801A2EF0;
L_801A2EEC:
    r25 = 0x0;
L_801A2EF0:
    if (r21 == 0) goto L_801A2F00;
    r26 = *(u32*)((u8*)r21 + 0x0);
    goto L_801A2F04;
L_801A2F00:
    r26 = 0x0;
L_801A2F04:
    if (r20 == 0) goto L_801A2F14;
    r27 = *(u32*)((u8*)r20 + 0x0);
    goto L_801A2F70;
L_801A2F14:
    r27 = 0x0;
    goto L_801A2F70;
L_801A2F1C:
    r3 = r24;
    r4 = r25;
    r5 = r26;
    r6 = r27;
    fn_801A2B5C();
    r24 = *(u32*)((u8*)r24 + 0x8);
    if (r25 == 0) goto L_801A2F44;
    r25 = *(u32*)((u8*)r25 + 0x4);
    goto L_801A2F48;
L_801A2F44:
    r25 = 0x0;
L_801A2F48:
    if (r26 == 0) goto L_801A2F58;
    r26 = *(u32*)((u8*)r26 + 0x4);
    goto L_801A2F5C;
L_801A2F58:
    r26 = 0x0;
L_801A2F5C:
    if (r27 == 0) goto L_801A2F6C;
    r27 = *(u32*)((u8*)r27 + 0x4);
    goto L_801A2F70;
L_801A2F6C:
    r27 = 0x0;
L_801A2F70:
    if (r24 != 0) goto L_801A2F1C;
L_801A2F78:
    r23 = *(u32*)((u8*)r23 + 0x8);
    if (r22 == 0) goto L_801A2F8C;
    r22 = *(u32*)((u8*)r22 + 0x4);
    goto L_801A2F90;
L_801A2F8C:
    r22 = 0x0;
L_801A2F90:
    if (r21 == 0) goto L_801A2FA0;
    r21 = *(u32*)((u8*)r21 + 0x4);
    goto L_801A2FA4;
L_801A2FA0:
    r21 = 0x0;
L_801A2FA4:
    if (r20 == 0) goto L_801A2FB4;
    r20 = *(u32*)((u8*)r20 + 0x4);
    goto L_801A2FB8;
L_801A2FB4:
    r20 = 0x0;
L_801A2FB8:
    if (r23 != 0) goto L_801A2EB0;
L_801A2FC0:
    r31 = *(u32*)((u8*)r31 + 0x8);
    if (r30 == 0) goto L_801A2FD4;
    r30 = *(u32*)((u8*)r30 + 0x4);
    goto L_801A2FD8;
L_801A2FD4:
    r30 = 0x0;
L_801A2FD8:
    if (r29 == 0) goto L_801A2FE8;
    r29 = *(u32*)((u8*)r29 + 0x4);
    goto L_801A2FEC;
L_801A2FE8:
    r29 = 0x0;
L_801A2FEC:
    if (r28 == 0) goto L_801A2FFC;
    r28 = *(u32*)((u8*)r28 + 0x4);
    goto L_801A3000;
L_801A2FFC:
    r28 = 0x0;
L_801A3000:
    if (r31 != 0) goto L_801A2DA8;
L_801A3008:
    return;
}

/* 0x801A301C | 0x220 */
void fn_801A301C(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    extern void fn_80199654();
    extern void fn_8019D620();
    extern void fn_801AFE68();
    extern void fn_801C25E4();
    extern void fn_801C2670();
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

    r31 = r6;
    r30 = r5;
    /* mr. r29, r3 */;
    r28 = r4;
    if ((s32)tmp == 0) goto L_801A321C;
    if (r28 == 0) goto L_801A31D8;
    tmp = *(u32*)((u8*)r29 + 0x7C);
    if (tmp == 0) goto L_801A3068;
    r3 = *(u32*)((u8*)r29 + 0x7C);
    fn_801C25E4();
L_801A3068:
    r3 = *(u32*)((u8*)r28 + 0x8);
    fn_801C2670();
    *(u32*)((u8*)r29 + 0x7C) = r3;
    r6 = *(u32*)((u8*)r29 + 0x7C);
    if (r6 == 0) goto L_801A30D4;
    tmp = *(u32*)((u8*)r6 + 0x14);
    if (tmp == 0) goto L_801A30D4;
    r5 = r6 + 0x14;
    goto L_801A30C8;
L_801A3094:
    r3 = *(u32*)((u8*)r5 + 0x0);
    tmp = *(u8*)((u8*)r3 + 0x13);
    if (tmp != 0xc) goto L_801A30C4;
    r3 = *(u32*)((u8*)r5 + 0x0);
    r4 = *(u32*)((u8*)r5 + 0x0);
    tmp = *(u32*)((u8*)r3 + 0x0);
    *(u32*)((u8*)r5 + 0x0) = tmp;
    tmp = *(u32*)((u8*)r6 + 0x14);
    *(u32*)((u8*)r4 + 0x0) = tmp;
    *(u32*)((u8*)r6 + 0x14) = r4;
    goto L_801A30D4;
L_801A30C4:
    r5 = *(u32*)((u8*)r5 + 0x0);
L_801A30C8:
    tmp = *(u32*)((u8*)r5 + 0x0);
    if (tmp != 0) goto L_801A3094;
L_801A30D4:
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = *(u32*)((u8*)r28 + 0xC);
    fn_801AFE68();
    tmp = *(u32*)((u8*)r28 + 0x10);
    tmp = tmp & 0x1;
    if (tmp == 0) goto L_801A3164;
    if (r29 == 0) goto L_801A31D8;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp ^ 0x8;
    tmp = tmp & 0x00000008;
    if (r29 == 0) goto L_801A3154;
    if (r29 == 0) goto L_801A3154;
    if (r29 != 0) goto L_801A3124;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A3124:
    tmp = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r29 != 0) goto L_801A3144;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00000040;
    if (r29 == 0) goto L_801A3144;
    r3 = 0x1;
L_801A3144:
    if ((s32)r3 != 0) goto L_801A3154;
    r3 = r29;
    fn_8019D620();
L_801A3154:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp | 0x8;
    *(u32*)((u8*)r29 + 0x14) = tmp;
    goto L_801A31D8;
L_801A3164:
    if (r29 == 0) goto L_801A31D8;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp ^ 0x8;
    tmp = tmp & 0x00000008;
    if (r29 == 0) goto L_801A31CC;
    if (r29 == 0) goto L_801A31CC;
    if (r29 != 0) goto L_801A319C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A319C:
    tmp = *(u32*)((u8*)r29 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r29 != 0) goto L_801A31BC;
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00000040;
    if (r29 == 0) goto L_801A31BC;
    r3 = 0x1;
L_801A31BC:
    if ((s32)r3 != 0) goto L_801A31CC;
    r3 = r29;
    fn_8019D620();
L_801A31CC:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0xFFFFFFF7;
    *(u32*)((u8*)r29 + 0x14) = tmp;
L_801A31D8:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if ((s32)r3 == 0) goto L_801A321C;
    if (r31 == 0) goto L_801A31FC;
    r5 = *(u32*)((u8*)r31 + 0x8);
    goto L_801A3200;
L_801A31FC:
    r5 = 0x0;
L_801A3200:
    r3 = *(u32*)((u8*)r29 + 0x18);
    if (r30 == 0) goto L_801A3214;
    r4 = *(u32*)((u8*)r30 + 0x8);
    goto L_801A3218;
L_801A3214:
    r4 = 0x0;
L_801A3218:
    fn_80199654();
L_801A321C:
    return;
}

/* 0x64 | fn_801A323C | generic */
void fn_801A323C(u32 arg1) {

}

/* 0x801A32A0 | 0x2D4 */
void fn_801A32A0(void) {
    extern void fn_801995D4();
    extern void fn_801A32A0();
    extern void fn_801A3574();
    extern void fn_801AFF64();
    extern void fn_801C29C4();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    f31 = f1;
    /* mr. r25, r3 */;
    r28 = r4;
    if ((s32)tmp == 0) goto L_801A355C;
    if (r25 == 0) goto L_801A3314;
    tmp = r28 & 0x1;
    if (r25 == 0) goto L_801A32E0;
    f1 = f31;
    r3 = *(u32*)((u8*)r25 + 0x7C);
    fn_801C29C4();
L_801A32E0:
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r25 == 0) goto L_801A3304;
    f1 = f31;
    r3 = *(u32*)((u8*)r25 + 0x18);
    r4 = r28;
    fn_801995D4();
L_801A3304:
    f1 = f31;
    r3 = *(u32*)((u8*)r25 + 0x80);
    r4 = r28;
    fn_801AFF64();
L_801A3314:
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x00001000;
    if (r25 != 0) goto L_801A355C;
    r29 = *(u32*)((u8*)r25 + 0x10);
    goto L_801A3554;
L_801A3328:
    if (r29 == 0) goto L_801A3550;
    if (r29 == 0) goto L_801A3380;
    tmp = r28 & 0x1;
    if (r29 == 0) goto L_801A334C;
    f1 = f31;
    r3 = *(u32*)((u8*)r29 + 0x7C);
    fn_801C29C4();
L_801A334C:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r29 == 0) goto L_801A3370;
    f1 = f31;
    r3 = *(u32*)((u8*)r29 + 0x18);
    r4 = r28;
    fn_801995D4();
L_801A3370:
    f1 = f31;
    r3 = *(u32*)((u8*)r29 + 0x80);
    r4 = r28;
    fn_801AFF64();
L_801A3380:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00001000;
    if (r29 != 0) goto L_801A3550;
    r31 = *(u32*)((u8*)r29 + 0x10);
    goto L_801A3548;
L_801A3394:
    if (r31 == 0) goto L_801A3544;
    if (r31 == 0) goto L_801A33EC;
    tmp = r28 & 0x1;
    if (r31 == 0) goto L_801A33B8;
    f1 = f31;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    fn_801C29C4();
L_801A33B8:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r31 == 0) goto L_801A33DC;
    f1 = f31;
    r3 = *(u32*)((u8*)r31 + 0x18);
    r4 = r28;
    fn_801995D4();
L_801A33DC:
    f1 = f31;
    r3 = *(u32*)((u8*)r31 + 0x80);
    r4 = r28;
    fn_801AFF64();
L_801A33EC:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3544;
    r30 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A353C;
L_801A3400:
    if (r30 == 0) goto L_801A3538;
    if (r30 == 0) goto L_801A3458;
    tmp = r28 & 0x1;
    if (r30 == 0) goto L_801A3424;
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x7C);
    fn_801C29C4();
L_801A3424:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r30 == 0) goto L_801A3448;
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r28;
    fn_801995D4();
L_801A3448:
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = r28;
    fn_801AFF64();
L_801A3458:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00001000;
    if (r30 != 0) goto L_801A3538;
    r27 = *(u32*)((u8*)r30 + 0x10);
    goto L_801A3530;
L_801A346C:
    if (r27 == 0) goto L_801A352C;
    if (r27 == 0) goto L_801A34C4;
    tmp = r28 & 0x1;
    if (r27 == 0) goto L_801A3490;
    f1 = f31;
    r3 = *(u32*)((u8*)r27 + 0x7C);
    fn_801C29C4();
L_801A3490:
    tmp = *(u32*)((u8*)r27 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if (r27 == 0) goto L_801A34B4;
    f1 = f31;
    r3 = *(u32*)((u8*)r27 + 0x18);
    r4 = r28;
    fn_801995D4();
L_801A34B4:
    f1 = f31;
    r3 = *(u32*)((u8*)r27 + 0x80);
    r4 = r28;
    fn_801AFF64();
L_801A34C4:
    tmp = *(u32*)((u8*)r27 + 0x14);
    tmp = tmp & 0x00001000;
    if (r27 != 0) goto L_801A352C;
    r25 = *(u32*)((u8*)r27 + 0x10);
    goto L_801A3524;
L_801A34D8:
    if (r25 == 0) goto L_801A3520;
    f1 = f31;
    r3 = r25;
    r4 = r28;
    fn_801A3574();
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x00001000;
    if (r25 != 0) goto L_801A3520;
    r26 = *(u32*)((u8*)r25 + 0x10);
    goto L_801A3518;
L_801A3504:
    f1 = f31;
    r3 = r26;
    r4 = r28;
    fn_801A32A0();
    r26 = *(u32*)((u8*)r26 + 0x8);
L_801A3518:
    if (r26 != 0) goto L_801A3504;
L_801A3520:
    r25 = *(u32*)((u8*)r25 + 0x8);
L_801A3524:
    if (r25 != 0) goto L_801A34D8;
L_801A352C:
    r27 = *(u32*)((u8*)r27 + 0x8);
L_801A3530:
    if (r27 != 0) goto L_801A346C;
L_801A3538:
    r30 = *(u32*)((u8*)r30 + 0x8);
L_801A353C:
    if (r30 != 0) goto L_801A3400;
L_801A3544:
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A3548:
    if (r31 != 0) goto L_801A3394;
L_801A3550:
    r29 = *(u32*)((u8*)r29 + 0x8);
L_801A3554:
    if (r29 != 0) goto L_801A3328;
L_801A355C:
    return;
}

/* 0x801A3574 | 0x8C */
void fn_801A3574(void) {
    extern void fn_801995D4();
    extern void fn_801AFF64();
    extern void fn_801C29C4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f31 = 0.0f;

    f31 = f1;
    r31 = r4;
    /* mr. r30, r3 */;
    if ((s32)tmp == 0) goto L_801A35E4;
    tmp = r31 & 0x1;
    if ((s32)tmp == 0) goto L_801A35B0;
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x7C);
    fn_801C29C4();
L_801A35B0:
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x4020;
    tmp = __cntlzw(tmp);
    /* srwi. tmp, tmp, 5 */;
    if ((s32)tmp == 0) goto L_801A35D4;
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x18);
    r4 = r31;
    fn_801995D4();
L_801A35D4:
    f1 = f31;
    r3 = *(u32*)((u8*)r30 + 0x80);
    r4 = r31;
    fn_801AFF64();
L_801A35E4:
    return;
}

/* 0x801A3600 | 0x318 */
void fn_801A3600(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_800A2D98();
    extern void fn_800A37CC();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    extern void fn_801A8570();
    extern void fn_801A85A4();
    extern void fn_801A86B4();
    extern void fn_801A8884();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r31 = r3;
    r30 = *(u32*)((u8*)r3 + 0xC);
    if (r30 == 0) goto L_801A366C;
    if (r30 != 0) goto L_801A363C;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A363C:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A365C;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A365C;
    r3 = 0x1;
L_801A365C:
    if ((s32)r3 == 0) goto L_801A366C;
    r3 = r30;
    fn_8019D9DC();
L_801A366C:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000008;
    if ((s32)r3 == 0) goto L_801A36F0;
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A36D0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x74);
    if (tmp == 0) goto L_801A36D0;
    tmp = *(u32*)((u8*)r31 + 0x74);
    if (tmp != 0) goto L_801A36A8;
    fn_801A85A4();
    *(u32*)((u8*)r31 + 0x74) = r3;
L_801A36A8:
    r3 = *(u32*)((u8*)r31 + 0xC);
    r4 = *(u32*)((u8*)r31 + 0x74);
    r5 = *(u32*)((u8*)r3 + 0x74);
    r3 = *(u32*)((u8*)r5 + 0x0);
    tmp = *(u32*)((u8*)r5 + 0x4);
    *(u32*)((u8*)r4 + 0x0) = r3;
    *(u32*)((u8*)r4 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r5 + 0x8);
    *(u32*)((u8*)r4 + 0x8) = tmp;
    goto L_801A3794;
L_801A36D0:
    tmp = *(u32*)((u8*)r31 + 0x74);
    if (tmp == 0) goto L_801A3794;
    r3 = *(u32*)((u8*)r31 + 0x74);
    fn_801A8570();
    tmp = 0x0;
    *(u32*)((u8*)r31 + 0x74) = tmp;
    goto L_801A3794;
L_801A36F0:
    tmp = *(u32*)((u8*)r31 + 0x74);
    if (tmp != 0) goto L_801A3704;
    fn_801A85A4();
    *(u32*)((u8*)r31 + 0x74) = r3;
L_801A3704:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A3778;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x74);
    if (tmp == 0) goto L_801A3778;
    r3 = *(u32*)((u8*)r31 + 0xC);
    f1 = *(f32*)((u8*)r31 + 0x2C);
    r4 = *(u32*)((u8*)r3 + 0x74);
    r3 = *(u32*)((u8*)r31 + 0x74);
    f0 = *(f32*)((u8*)r4 + 0x0);
    f0 = f1 * f0;
    *(f32*)((u8*)r3 + 0x0) = f0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    f1 = *(f32*)((u8*)r31 + 0x30);
    r4 = *(u32*)((u8*)r3 + 0x74);
    r3 = *(u32*)((u8*)r31 + 0x74);
    f0 = *(f32*)((u8*)r4 + 0x4);
    f0 = f1 * f0;
    *(f32*)((u8*)r3 + 0x4) = f0;
    r3 = *(u32*)((u8*)r31 + 0xC);
    f1 = *(f32*)((u8*)r31 + 0x34);
    r4 = *(u32*)((u8*)r3 + 0x74);
    r3 = *(u32*)((u8*)r31 + 0x74);
    f0 = *(f32*)((u8*)r4 + 0x8);
    f0 = f1 * f0;
    *(f32*)((u8*)r3 + 0x8) = f0;
    goto L_801A3794;
L_801A3778:
    r4 = *(u32*)((u8*)r31 + 0x74);
    r3 = *(u32*)((u8*)r31 + 0x2C);
    tmp = *(u32*)((u8*)r31 + 0x30);
    *(u32*)((u8*)r4 + 0x0) = r3;
    *(u32*)((u8*)r4 + 0x4) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x34);
    *(u32*)((u8*)r4 + 0x8) = tmp;
L_801A3794:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00020000;
    if (tmp == 0) goto L_801A37F4;
    tmp = *(u32*)((u8*)r31 + 0xC);
    r3 = r31 + 0x44;
    r4 = r31 + 0x2c;
    r5 = r31 + 0x1c;
    r6 = r31 + 0x38;
    r8 = 0x0;
    if (tmp == 0) goto L_801A37D4;
    r7 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r7 + 0x74);
    if (tmp == 0) goto L_801A37D4;
    r8 = 0x1;
L_801A37D4:
    if ((s32)r8 == 0) goto L_801A37E8;
    r7 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r7 + 0x74);
    goto L_801A37EC;
L_801A37E8:
    r7 = 0x0;
L_801A37EC:
    fn_801A86B4();
    goto L_801A3844;
L_801A37F4:
    tmp = *(u32*)((u8*)r31 + 0xC);
    r3 = r31 + 0x44;
    r4 = r31 + 0x2c;
    r5 = r31 + 0x1c;
    r6 = r31 + 0x38;
    r8 = 0x0;
    if (tmp == 0) goto L_801A3828;
    r7 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r7 + 0x74);
    if (tmp == 0) goto L_801A3828;
    r8 = 0x1;
L_801A3828:
    if ((s32)r8 == 0) goto L_801A383C;
    r7 = *(u32*)((u8*)r31 + 0xC);
    r7 = *(u32*)((u8*)r7 + 0x74);
    goto L_801A3840;
L_801A383C:
    r7 = 0x0;
L_801A3840:
    fn_801A8884();
L_801A3844:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A3864;
    r3 = *(u32*)((u8*)r31 + 0xC);
    r4 = r31 + 0x44;
    r5 = r31 + 0x44;
    r3 = r3 + 0x44;
    fn_800A2D98();
L_801A3864:
    tmp = *(u32*)((u8*)r31 + 0x7C);
    if (tmp == 0) goto L_801A3900;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    tmp = *(u32*)((u8*)r3 + 0x18);
    if (tmp == 0) goto L_801A3900;
    r3 = *(u32*)((u8*)r31 + 0x7C);
    r30 = *(u32*)((u8*)r3 + 0x18);
    if (r30 == 0) goto L_801A38D8;
    if (r30 != 0) goto L_801A38A8;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A38A8:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A38C8;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A38C8;
    r3 = 0x1;
L_801A38C8:
    if ((s32)r3 == 0) goto L_801A38D8;
    r3 = r30;
    fn_8019D9DC();
L_801A38D8:
    r3 = r30 + 0x44;
    r4 = r31 + 0x38;
    r5 = r1 + 0x8;
    fn_800A37CC();
    f0 = *(f32*)(sp + 0x8);
    *(f32*)((u8*)r31 + 0x50) = f0;
    f0 = *(f32*)(sp + 0xC);
    *(f32*)((u8*)r31 + 0x60) = f0;
    f0 = *(f32*)(sp + 0x10);
    *(f32*)((u8*)r31 + 0x70) = f0;
L_801A3900:
    return;
}

/* 0x801A3918 | 0x94 */
void fn_801A3918(void) {
    extern void fn_801A39AC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    r30 = r5;
    r29 = r4;
    if ((s32)tmp == 0) goto L_801A3990;
    if (r29 == 0) goto L_801A3960;
    r12 = r29;
    r3 = r31;
    r4 = r30;
    r5 = 0x0;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3960:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00001000;
    if (r29 != 0) goto L_801A3990;
    r31 = *(u32*)((u8*)r31 + 0x10);
    goto L_801A3988;
L_801A3974:
    r3 = r31;
    r4 = r29;
    r5 = r30;
    fn_801A39AC();
    r31 = *(u32*)((u8*)r31 + 0x8);
L_801A3988:
    if (r31 != 0) goto L_801A3974;
L_801A3990:
    return;
}

/* 0x801A39AC | 0x358 */
void fn_801A39AC(void) {
    extern u8 lbl_80274D44[];
    extern u8 lbl_8047DB20[];
    extern void fn_80196E10();
    extern void fn_801A39AC();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r24, r3 */;
    r31 = r4;
    r30 = r5;
    if ((s32)tmp == 0) goto L_801A3CF0;
    tmp = *(u32*)((u8*)r24 + 0xC);
    if (tmp != 0) goto L_801A39EC;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A39EC:
    r3 = *(u32*)((u8*)r24 + 0xC);
    tmp = 0x2;
    r3 = *(u32*)((u8*)r3 + 0x10);
    r4 = r3 - r24;
    r3 = r24 - r3;
    r3 = ~(r4 | r3);
    r3 = (s32)r3 >> 31;
    tmp = r3 + tmp;
    r5 = tmp;
    if (r31 == 0) goto L_801A3A2C;
    r12 = r31;
    r3 = r24;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3A2C:
    tmp = *(u32*)((u8*)r24 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CF0;
    r28 = *(u32*)((u8*)r24 + 0x10);
    goto L_801A3CE8;
L_801A3A40:
    if (r28 == 0) goto L_801A3CE4;
    tmp = *(u32*)((u8*)r28 + 0xC);
    if (tmp != 0) goto L_801A3A68;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A3A68:
    r3 = *(u32*)((u8*)r28 + 0xC);
    r5 = 0x2;
    tmp = *(u32*)((u8*)r3 + 0x10);
    r3 = tmp - r28;
    tmp = r28 - tmp;
    tmp = ~(r3 | tmp);
    tmp = (s32)tmp >> 31;
    r5 = tmp + r5;
    if (r31 == 0) goto L_801A3AA4;
    r12 = r31;
    r3 = r28;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3AA4:
    tmp = *(u32*)((u8*)r28 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CE4;
    r29 = *(u32*)((u8*)r28 + 0x10);
    goto L_801A3CDC;
L_801A3AB8:
    if (r29 == 0) goto L_801A3CD8;
    tmp = *(u32*)((u8*)r29 + 0xC);
    if (tmp != 0) goto L_801A3AE0;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A3AE0:
    r3 = *(u32*)((u8*)r29 + 0xC);
    r5 = 0x2;
    tmp = *(u32*)((u8*)r3 + 0x10);
    r3 = tmp - r29;
    tmp = r29 - tmp;
    tmp = ~(r3 | tmp);
    tmp = (s32)tmp >> 31;
    r5 = tmp + r5;
    if (r31 == 0) goto L_801A3B1C;
    r12 = r31;
    r3 = r29;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3B1C:
    tmp = *(u32*)((u8*)r29 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CD8;
    r27 = *(u32*)((u8*)r29 + 0x10);
    goto L_801A3CD0;
L_801A3B30:
    if (r27 == 0) goto L_801A3CCC;
    tmp = *(u32*)((u8*)r27 + 0xC);
    if (tmp != 0) goto L_801A3B58;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A3B58:
    r3 = *(u32*)((u8*)r27 + 0xC);
    r5 = 0x2;
    tmp = *(u32*)((u8*)r3 + 0x10);
    r3 = tmp - r27;
    tmp = r27 - tmp;
    tmp = ~(r3 | tmp);
    tmp = (s32)tmp >> 31;
    r5 = tmp + r5;
    if (r31 == 0) goto L_801A3B94;
    r12 = r31;
    r3 = r27;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3B94:
    tmp = *(u32*)((u8*)r27 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CCC;
    r26 = *(u32*)((u8*)r27 + 0x10);
    goto L_801A3CC4;
L_801A3BA8:
    if (r26 == 0) goto L_801A3CC0;
    tmp = *(u32*)((u8*)r26 + 0xC);
    if (tmp != 0) goto L_801A3BD0;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A3BD0:
    r3 = *(u32*)((u8*)r26 + 0xC);
    r5 = 0x2;
    tmp = *(u32*)((u8*)r3 + 0x10);
    r3 = tmp - r26;
    tmp = r26 - tmp;
    tmp = ~(r3 | tmp);
    tmp = (s32)tmp >> 31;
    r5 = tmp + r5;
    if (r31 == 0) goto L_801A3C0C;
    r12 = r31;
    r3 = r26;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3C0C:
    tmp = *(u32*)((u8*)r26 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CC0;
    r25 = *(u32*)((u8*)r26 + 0x10);
    goto L_801A3CB8;
L_801A3C20:
    if (r25 == 0) goto L_801A3CB4;
    tmp = *(u32*)((u8*)r25 + 0xC);
    if (tmp != 0) goto L_801A3C48;
    r4 = (u32)lbl_80274D44;
    r3 = (u32)lbl_8047DB20;
    r5 = (u32)lbl_80274D44;
    r4 = 0xad;
    fn_80196E10();
L_801A3C48:
    r3 = *(u32*)((u8*)r25 + 0xC);
    r5 = 0x2;
    tmp = *(u32*)((u8*)r3 + 0x10);
    r3 = tmp - r25;
    tmp = r25 - tmp;
    tmp = ~(r3 | tmp);
    tmp = (s32)tmp >> 31;
    r5 = tmp + r5;
    if (r31 == 0) goto L_801A3C84;
    r12 = r31;
    r3 = r25;
    r4 = r30;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801A3C84:
    tmp = *(u32*)((u8*)r25 + 0x14);
    tmp = tmp & 0x00001000;
    if (r31 != 0) goto L_801A3CB4;
    r24 = *(u32*)((u8*)r25 + 0x10);
    goto L_801A3CAC;
L_801A3C98:
    r3 = r24;
    r4 = r31;
    r5 = r30;
    fn_801A39AC();
    r24 = *(u32*)((u8*)r24 + 0x8);
L_801A3CAC:
    if (r24 != 0) goto L_801A3C98;
L_801A3CB4:
    r25 = *(u32*)((u8*)r25 + 0x8);
L_801A3CB8:
    if (r25 != 0) goto L_801A3C20;
L_801A3CC0:
    r26 = *(u32*)((u8*)r26 + 0x8);
L_801A3CC4:
    if (r26 != 0) goto L_801A3BA8;
L_801A3CCC:
    r27 = *(u32*)((u8*)r27 + 0x8);
L_801A3CD0:
    if (r27 != 0) goto L_801A3B30;
L_801A3CD8:
    r29 = *(u32*)((u8*)r29 + 0x8);
L_801A3CDC:
    if (r29 != 0) goto L_801A3AB8;
L_801A3CE4:
    r28 = *(u32*)((u8*)r28 + 0x8);
L_801A3CE8:
    if (r28 != 0) goto L_801A3A40;
L_801A3CF0:
    return;
}

/* 0x801A3D04 | 0x160 */
void fn_801A3D04(void) {
    extern u8 lbl_8047DB34[];
    extern u8 lbl_8047DB3C[];
    extern void fn_80196E10();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    /* mr. r31, r3 */;
    if ((s32)tmp == 0) goto L_801A3E4C;
    if (r31 != 0) goto L_801A3D38;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A3D38:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r31 != 0) goto L_801A3D58;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00000040;
    if (r31 == 0) goto L_801A3D58;
    r3 = 0x1;
L_801A3D58:
    if ((s32)r3 == 0) goto L_801A3D64;
    goto L_801A3E4C;
L_801A3D64:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x00800000;
    if ((s32)r3 == 0) goto L_801A3DDC;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp & 0x01000000;
    if ((s32)r3 != 0) goto L_801A3E4C;
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A3E4C;
    r30 = *(u32*)((u8*)r31 + 0xC);
    if (r30 != 0) goto L_801A3DA4;
    r3 = (u32)lbl_8047DB34;
    r4 = 0x25d;
    r5 = (u32)lbl_8047DB3C;
    fn_80196E10();
L_801A3DA4:
    tmp = *(u32*)((u8*)r30 + 0x14);
    r3 = 0x0;
    tmp = tmp & 0x00800000;
    if (r30 != 0) goto L_801A3DC4;
    tmp = *(u32*)((u8*)r30 + 0x14);
    tmp = tmp & 0x00000040;
    if (r30 == 0) goto L_801A3DC4;
    r3 = 0x1;
L_801A3DC4:
    if ((s32)r3 == 0) goto L_801A3E4C;
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
    goto L_801A3E4C;
L_801A3DDC:
    tmp = *(u32*)((u8*)r31 + 0xC);
    if (tmp == 0) goto L_801A3DF8;
    r3 = *(u32*)((u8*)r31 + 0xC);
    tmp = *(u32*)((u8*)r3 + 0x14);
    tmp = tmp & 0x00000040;
    if (tmp != 0) goto L_801A3E40;
L_801A3DF8:
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x20 */;
    if (tmp == 0) goto L_801A3E40;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x40 */;
    if (tmp == 0) goto L_801A3E40;
    tmp = *(u32*)((u8*)r31 + 0x14);
    r3 = tmp & 0x00600000;
    /* subis tmp, r3, 0x60 */;
    if (tmp == 0) goto L_801A3E40;
    tmp = *(u32*)((u8*)r31 + 0x80);
    if (tmp == 0) goto L_801A3E4C;
L_801A3E40:
    tmp = *(u32*)((u8*)r31 + 0x14);
    tmp = tmp | 0x40;
    *(u32*)((u8*)r31 + 0x14) = tmp;
L_801A3E4C:
    return;
}

/* 0x50 | fn_801A3E64 | global_cond_call */
u32 fn_801A3E64(void) {
    /* uses lbl_804655B4 */
    if (0 /* field check */) { return 0; }
    fn_801AA498();
    return 0;
}

/* 0x801A3EB4 | 0x94 */
void fn_801A3EB4(void) {
    extern u8 lbl_804655B4[];
    extern u8 lbl_8047DBA0[];
    extern u8 lbl_8047DBA8[];
    extern u8 lbl_8047DBB0[];
    extern void fn_80196E10();
    extern void fn_801AA4CC();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r5 = (u32)lbl_804655B4;
    r30 = r4;
    r29 = r3;
    r3 = (u32)lbl_804655B4;
    fn_801AA4CC();
    /* mr. r31, r3 */;
    if ((s32)tmp != 0) goto L_801A3EF8;
    r3 = (u32)lbl_8047DBA0;
    r4 = 0x4c;
    r5 = (u32)lbl_8047DBB0;
    fn_80196E10();
L_801A3EF8:
    r3 = r31;
    r4 = 0x0;
    r5 = 0x8;
    memset((void*)r3, (int)r4, (u32)r5);
    *(u32*)((u8*)r31 + 0x4) = r30;
    if (r31 != 0) goto L_801A3F24;
    r3 = (u32)lbl_8047DBA0;
    r4 = 0xca;
    r5 = (u32)lbl_8047DBA8;
    fn_80196E10();
L_801A3F24:
    *(u32*)((u8*)r31 + 0x0) = r29;
    r3 = r31;
    return;
}

/* 0x5C | fn_801A3F48 | multi_call_guarded */
void fn_801A3F48(void) {
    { fn_801AA4CC(); return; }
    fn_80196E10();
    memset();
}

/* 0x801A3FA4 | 0xC */
void fn_801A3FA4(void) {
}

/* 0x801A3FB0 | 0xC */
void fn_801A3FB0(void) {
}

