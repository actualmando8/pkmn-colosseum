#include "dolphin/types.h"

/*
 * extras.c - CRT library functions.
 *
 * Stub implementations for function coverage.
 */

/* fn_800CAAE0 - 0x800CAAE0 | size: 0xD0 */
void fn_800CAAE0(void) {
    extern u8 lbl_8047AA18[];
    extern void fn_800998B8();
    extern void fn_800C3A40();
    extern void fn_800CF47C();
    extern void fn_800CF4EC();
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
    f32 f0 = 0.0f;

    r31 = r6;
    r30 = r5;
    r29 = r4;
    r28 = r3;
    fn_800998B8();
    tmp = r3 & 0x20000000;
    if ((s32)tmp != 0) goto L_800CAB78;
    tmp = *(u32*)lbl_8047AA18;
    r3 = 0x0;
    if ((s32)tmp != 0) goto L_800CAB44;
    r3 = 0x10000;
    fn_800CF47C();
    if ((s32)r3 != 0) goto L_800CAB44;
    tmp = 0x1;
    *(u32*)lbl_8047AA18 = tmp;
L_800CAB44:
    if ((s32)r3 == 0) goto L_800CAB54;
    r3 = 0x1;
    goto L_800CAB90;
L_800CAB54:
    r4 = *(u32*)((u8*)r30 + 0x0);
    r3 = r29;
    fn_800CF4EC();
    if ((s32)r3 == 0) goto L_800CAB78;
    tmp = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r30 + 0x0) = tmp;
    goto L_800CAB90;
L_800CAB78:
    r3 = r28;
    r4 = r29;
    r5 = r30;
    r6 = r31;
    fn_800C3A40();
    r3 = 0x0;
L_800CAB90:
    return;
}

/* fn_800CABB0 - 0x800CABB0 | size: 0x23C */
void fn_800CABB0(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047C418[];
    extern u8 lbl_8047C420[];
    extern u8 lbl_8047C428[];
    extern u8 lbl_8047C430[];
    extern u8 lbl_8047C438[];
    extern u8 lbl_8047C440[];
    extern u8 lbl_8047C448[];
    extern u8 lbl_8047C450[];
    extern u8 lbl_8047C458[];
    extern u8 lbl_8047C460[];
    extern u8 lbl_8047C468[];
    extern u8 lbl_8047C470[];
    extern u8 lbl_8047C478[];
    extern u8 lbl_8047C480[];
    extern u8 lbl_8047C488[];
    extern u8 lbl_8047C490[];
    extern u8 lbl_8047C498[];
    extern void fn_800CE77C();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f31 = 0.0f;

    tmp = 0x3FF00000;
    r3 = r4 & 0x7FFFFFFF;
    if ((s32)r3 < (s32)tmp) goto L_800CAC10;
    /* subis r3, r3, 0x3ff0 */;
    /* or. tmp, r3, tmp */;
    if ((s32)r3 != (s32)tmp) goto L_800CAC04;
    if ((s32)r4 <= 0) goto L_800CABFC;
    f1 = *(f64*)lbl_8047C418;
    goto L_800CADD4;
L_800CABFC:
    f1 = *(f64*)lbl_8047C420;
    goto L_800CADD4;
L_800CAC04:
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_800CADD4;
L_800CAC10:
    tmp = 0x3FE00000;
    if ((s32)r3 >= (s32)tmp) goto L_800CACA4;
    tmp = 0x3C600000;
    if ((s32)r3 > (s32)tmp) goto L_800CAC30;
    f1 = *(f64*)lbl_8047C428;
    goto L_800CADD4;
L_800CAC30:
    f10 = f1 * f1;
    f2 = *(f64*)lbl_8047C460;
    f0 = *(f64*)lbl_8047C458;
    f3 = *(f64*)lbl_8047C450;
    f8 = *(f64*)lbl_8047C448;
    f4 = f2 * f10 + f0;
    f2 = *(f64*)lbl_8047C488;
    f0 = *(f64*)lbl_8047C480;
    f7 = *(f64*)lbl_8047C440;
    f9 = f10 * f4 + f3;
    f4 = *(f64*)lbl_8047C478;
    f6 = *(f64*)lbl_8047C438;
    f5 = f2 * f10 + f0;
    f3 = *(f64*)lbl_8047C470;
    f2 = *(f64*)lbl_8047C468;
    f8 = f10 * f9 + f8;
    f0 = *(f64*)lbl_8047C430;
    f9 = *(f64*)lbl_8047C428;
    f4 = f10 * f5 + f4;
    f5 = f10 * f8 + f7;
    f3 = f10 * f4 + f3;
    f4 = f10 * f5 + f6;
    f2 = f10 * f3 + f2;
    f3 = f10 * f4;
    f2 = f3 / f2;
    f0 = -(f1 * f2 - f0);
    f0 = f1 - f0;
    f1 = f9 - f0;
    goto L_800CADD4;
L_800CACA4:
    if ((s32)r4 >= 0) goto L_800CAD38;
    f0 = *(f64*)lbl_8047C468;
    f2 = *(f64*)lbl_8047C490;
    f0 = f0 + f1;
    f31 = f2 * f0;
    f1 = f31;
    fn_800CE77C();
    f3 = *(f64*)lbl_8047C460;
    f2 = *(f64*)lbl_8047C458;
    f0 = *(f64*)lbl_8047C450;
    f4 = f3 * f31 + f2;
    f5 = *(f64*)lbl_8047C448;
    f3 = *(f64*)lbl_8047C488;
    f2 = *(f64*)lbl_8047C480;
    f7 = *(f64*)lbl_8047C440;
    f8 = f31 * f4 + f0;
    f0 = *(f64*)lbl_8047C478;
    f2 = f3 * f31 + f2;
    f6 = *(f64*)lbl_8047C438;
    f4 = *(f64*)lbl_8047C470;
    f8 = f31 * f8 + f5;
    f3 = *(f64*)lbl_8047C468;
    f5 = f31 * f2 + f0;
    f2 = *(f64*)lbl_8047C430;
    f7 = f31 * f8 + f7;
    f8 = *(f64*)lbl_8047C498;
    f0 = *(f64*)lbl_8047C420;
    f4 = f31 * f5 + f4;
    f5 = f31 * f7 + f6;
    f3 = f31 * f4 + f3;
    f4 = f31 * f5;
    f3 = f4 / f3;
    f2 = f3 * f1 - f2;
    f1 = f1 + f2;
    f1 = -(f8 * f1 - f0);
    goto L_800CADD4;
L_800CAD38:
    f0 = *(f64*)lbl_8047C468;
    f2 = *(f64*)lbl_8047C490;
    f0 = f0 - f1;
    f31 = f2 * f0;
    f1 = f31;
    fn_800CE77C();
    f2 = *(f64*)lbl_8047C460;
    tmp = 0x0;
    f0 = *(f64*)lbl_8047C458;
    f3 = f2 * f31 + f0;
    f0 = *(f64*)lbl_8047C450;
    *(u32*)(sp + 0x14) = tmp;
    f2 = *(f64*)lbl_8047C448;
    f5 = f31 * f3 + f0;
    f4 = *(f64*)lbl_8047C488;
    f0 = *(f64*)lbl_8047C480;
    f3 = -(f9 * f9 - f31);
    f7 = *(f64*)lbl_8047C440;
    f8 = f31 * f5 + f2;
    f2 = *(f64*)lbl_8047C478;
    f5 = f4 * f31 + f0;
    f6 = *(f64*)lbl_8047C438;
    f0 = *(f64*)lbl_8047C470;
    f7 = f31 * f8 + f7;
    f4 = *(f64*)lbl_8047C468;
    f5 = f31 * f5 + f2;
    f2 = *(f64*)lbl_8047C498;
    f6 = f31 * f7 + f6;
    f5 = f31 * f5 + f0;
    f0 = f1 + f9;
    f6 = f31 * f6;
    f4 = f31 * f5 + f4;
    f0 = f3 / f0;
    f3 = f6 / f4;
    f0 = f3 * f1 + f0;
    f0 = f9 + f0;
    f1 = f2 * f0;
L_800CADD4:
    return;
}

/* fn_800CADEC - 0x800CADEC | size: 0x238 */
void fn_800CADEC(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047C4A0[];
    extern u8 lbl_8047C4A8[];
    extern u8 lbl_8047C4B0[];
    extern u8 lbl_8047C4B8[];
    extern u8 lbl_8047C4C0[];
    extern u8 lbl_8047C4C8[];
    extern u8 lbl_8047C4D0[];
    extern u8 lbl_8047C4D8[];
    extern u8 lbl_8047C4E0[];
    extern u8 lbl_8047C4E8[];
    extern u8 lbl_8047C4F0[];
    extern u8 lbl_8047C4F8[];
    extern u8 lbl_8047C500[];
    extern u8 lbl_8047C508[];
    extern u8 lbl_8047C510[];
    extern u8 lbl_8047C518[];
    extern u8 lbl_8047C520[];
    extern void fn_800CE77C();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    tmp = 0x3FF00000;
    r30 = r31 & 0x7FFFFFFF;
    if ((s32)r30 < (s32)tmp) goto L_800CAE60;
    /* subis r3, r30, 0x3ff0 */;
    /* or. tmp, r3, tmp */;
    if ((s32)r30 != (s32)tmp) goto L_800CAE54;
    f0 = *(f64*)lbl_8047C4A8;
    f2 = *(f64*)lbl_8047C4A0;
    f0 = f0 * f1;
    f1 = f2 * f1 + f0;
    goto L_800CAFF4;
L_800CAE54:
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_800CAFF4;
L_800CAE60:
    tmp = 0x3FE00000;
    if ((s32)r30 >= (s32)tmp) goto L_800CAEFC;
    tmp = 0x3E400000;
    if ((s32)r30 >= (s32)tmp) goto L_800CAE90;
    f2 = *(f64*)lbl_8047C4B0;
    f0 = *(f64*)lbl_8047C4B8;
    f2 = f2 + f1;
    if (f2 <= f0) goto L_800CAE94;
    goto L_800CAFF4;
L_800CAE90:
    f31 = f1 * f1;
L_800CAE94:
    f1 = *(f64*)lbl_8047C4E8;
    f0 = *(f64*)lbl_8047C4E0;
    f2 = *(f64*)lbl_8047C4D8;
    f3 = f1 * f31 + f0;
    f6 = *(f64*)lbl_8047C4D0;
    f1 = *(f64*)lbl_8047C508;
    f0 = *(f64*)lbl_8047C500;
    f5 = *(f64*)lbl_8047C4C8;
    f7 = f31 * f3 + f2;
    f2 = *(f64*)lbl_8047C4F8;
    f3 = f1 * f31 + f0;
    f4 = *(f64*)lbl_8047C4C0;
    f1 = *(f64*)lbl_8047C4F0;
    f6 = f31 * f7 + f6;
    f0 = *(f64*)lbl_8047C4B8;
    f2 = f31 * f3 + f2;
    f3 = f31 * f6 + f5;
    f1 = f31 * f2 + f1;
    f2 = f31 * f3 + f4;
    f0 = f31 * f1 + f0;
    f1 = f31 * f2;
    f0 = f1 / f0;
    f1 = f7 * f0 + f7;
    goto L_800CAFF4;
L_800CAEFC:
    /* fabs */ f1 = (f1 < 0) ? -f1 : f1;
    f9 = *(f64*)lbl_8047C4B8;
    f0 = *(f64*)lbl_8047C510;
    f7 = *(f64*)lbl_8047C4E8;
    f8 = f9 - f1;
    f3 = *(f64*)lbl_8047C4E0;
    f6 = *(f64*)lbl_8047C4D8;
    f5 = *(f64*)lbl_8047C4D0;
    f31 = f0 * f8;
    f2 = *(f64*)lbl_8047C508;
    f0 = *(f64*)lbl_8047C500;
    f4 = *(f64*)lbl_8047C4C8;
    f1 = *(f64*)lbl_8047C4F8;
    f7 = f7 * f31 + f3;
    f3 = *(f64*)lbl_8047C4C0;
    f2 = f2 * f31 + f0;
    f0 = *(f64*)lbl_8047C4F0;
    f6 = f31 * f7 + f6;
    f1 = f31 * f2 + f1;
    f2 = f31 * f6 + f5;
    f0 = f31 * f1 + f0;
    f1 = f31 * f2 + f4;
    f29 = f31 * f0 + f9;
    f0 = f31 * f1 + f3;
    f1 = f31;
    f30 = f31 * f0;
    fn_800CE77C();
    r3 = 0x3FEF0000;
    tmp = r3 + 0x3333;
    if ((s32)r30 < (s32)tmp) goto L_800CAFA0;
    f4 = f30 / f29;
    f2 = *(f64*)lbl_8047C518;
    f0 = *(f64*)lbl_8047C4A8;
    f3 = *(f64*)lbl_8047C4A0;
    f1 = f1 * f4 + f1;
    f0 = f2 * f1 - f0;
    f1 = f3 - f0;
    goto L_800CAFE4;
L_800CAFA0:
    tmp = 0x0;
    f7 = *(f64*)lbl_8047C518;
    f5 = f30 / f29;
    *(u32*)(sp + 0x14) = tmp;
    f0 = *(f64*)lbl_8047C4A8;
    f2 = *(f64*)lbl_8047C520;
    f4 = -(f8 * f8 - f31);
    f3 = f1 + f8;
    f6 = f7 * f1;
    f1 = f4 / f3;
    f1 = -(f7 * f1 - f0);
    f0 = -(f7 * f8 - f2);
    f1 = f6 * f5 - f1;
    f0 = f1 - f0;
    f1 = f2 - f0;
L_800CAFE4:
    if ((s32)r31 <= 0) goto L_800CAFF0;
    goto L_800CAFF4;
L_800CAFF0:
    f1 = -f1;
L_800CAFF4:
    return;
}

/* fn_800CB024 - 0x800CB024 | size: 0x290 */
void fn_800CB024(void) {
    extern u8 lbl_8047C528[];
    extern u8 lbl_8047C530[];
    extern u8 lbl_8047C538[];
    extern u8 lbl_8047C540[];
    extern u8 lbl_8047C548[];
    extern u8 lbl_8047C550[];
    extern u8 lbl_8047C558[];
    extern u8 lbl_8047C560[];
    extern u8 lbl_8047C568[];
    extern u8 lbl_8047C570[];
    extern u8 lbl_8047C578[];
    extern void fn_800CD85C();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = 0x7FF00000;
    tmp = -r8;
    tmp = r8 | tmp;
    r6 = r4 & 0x7FFFFFFF;
    tmp = (u32)tmp >> 31;
    tmp = r6 | tmp;
    r7 = r5 & 0x7FFFFFFF;
    if (tmp > r3) goto L_800CB088;
    tmp = -r9;
    tmp = r9 | tmp;
    tmp = (u32)tmp >> 31;
    tmp = r7 | tmp;
    if (tmp <= r3) goto L_800CB098;
L_800CB088:
    f1 = f1 + f0;
    goto L_800CB2A0;
L_800CB098:
    /* subis tmp, r4, 0x3ff0 */;
    /* or. tmp, tmp, r8 */;
    if (tmp != r3) goto L_800CB0AC;
    fn_800CD85C();
    goto L_800CB2A0;
L_800CB0AC:
    /* or. tmp, r7, r9 */;
    tmp = ((r4 << 2) | ((u32)r4 >> 30)) & 0x00000002;
    r31 = tmp;
    r31 = (r31 & ~0x00000001) | (((r5 << 1) | ((u32)r5 >> 31)) & 0x00000001);
    if (tmp != r3) goto L_800CB0F8;
    if ((s32)r31 == 2) goto L_800CB0E8;
    if ((s32)r31 >= 2) goto L_800CB0D8;
    if ((s32)r31 >= 0) goto L_800CB2A0;
    goto L_800CB0F8;
L_800CB0D8:
    if ((s32)r31 >= 4) goto L_800CB0F8;
    goto L_800CB0F0;
    goto L_800CB2A0;
L_800CB0E8:
    f1 = *(f64*)lbl_8047C528;
    goto L_800CB2A0;
L_800CB0F0:
    f1 = *(f64*)lbl_8047C530;
    goto L_800CB2A0;
L_800CB0F8:
    /* or. tmp, r6, r8 */;
    if ((s32)r31 != 4) goto L_800CB118;
    if ((s32)r5 >= 0) goto L_800CB110;
    f1 = *(f64*)lbl_8047C538;
    goto L_800CB2A0;
L_800CB110:
    f1 = *(f64*)lbl_8047C540;
    goto L_800CB2A0;
L_800CB118:
    /* subis tmp, r6, 0x7ff0 */;
    if (tmp != 0) goto L_800CB1C0;
    /* subis tmp, r7, 0x7ff0 */;
    if (tmp != 0) goto L_800CB178;
    if ((s32)r31 == 2) goto L_800CB168;
    if ((s32)r31 >= 2) goto L_800CB14C;
    if ((s32)r31 == 0) goto L_800CB158;
    if ((s32)r31 >= 0) goto L_800CB160;
    goto L_800CB1C0;
L_800CB14C:
    if ((s32)r31 >= 4) goto L_800CB1C0;
    goto L_800CB170;
L_800CB158:
    f1 = *(f64*)lbl_8047C548;
    goto L_800CB2A0;
L_800CB160:
    f1 = *(f64*)lbl_8047C550;
    goto L_800CB2A0;
L_800CB168:
    f1 = *(f64*)lbl_8047C558;
    goto L_800CB2A0;
L_800CB170:
    f1 = *(f64*)lbl_8047C560;
    goto L_800CB2A0;
L_800CB178:
    if ((s32)r31 == 2) goto L_800CB1B0;
    if ((s32)r31 >= 2) goto L_800CB194;
    if ((s32)r31 == 0) goto L_800CB1A0;
    if ((s32)r31 >= 0) goto L_800CB1A8;
    goto L_800CB1C0;
L_800CB194:
    if ((s32)r31 >= 4) goto L_800CB1C0;
    goto L_800CB1B8;
L_800CB1A0:
    f1 = *(f64*)lbl_8047C568;
    goto L_800CB2A0;
L_800CB1A8:
    f1 = *(f64*)lbl_8047C570;
    goto L_800CB2A0;
L_800CB1B0:
    f1 = *(f64*)lbl_8047C528;
    goto L_800CB2A0;
L_800CB1B8:
    f1 = *(f64*)lbl_8047C530;
    goto L_800CB2A0;
L_800CB1C0:
    /* subis tmp, r7, 0x7ff0 */;
    if (tmp != 0) goto L_800CB1E4;
    if ((s32)r5 >= 0) goto L_800CB1DC;
    f1 = *(f64*)lbl_8047C538;
    goto L_800CB2A0;
L_800CB1DC:
    f1 = *(f64*)lbl_8047C540;
    goto L_800CB2A0;
L_800CB1E4:
    tmp = r7 - r6;
    tmp = (s32)tmp >> 20;
    if ((s32)tmp <= 0x3c) goto L_800CB200;
    f0 = *(f64*)lbl_8047C540;
    goto L_800CB234;
L_800CB200:
    if ((s32)r4 >= 0) goto L_800CB21C;
    if ((s32)tmp >= (s32)-0x3c) goto L_800CB21C;
    f0 = *(f64*)lbl_8047C568;
    goto L_800CB234;
L_800CB21C:
    f0 = f1 / f0;
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    fn_800CD85C();
L_800CB234:
    if ((s32)r31 == 1) goto L_800CB260;
    if ((s32)r31 >= 1) goto L_800CB24C;
    if ((s32)r31 >= 0) goto L_800CB258;
    goto L_800CB28C;
L_800CB24C:
    if ((s32)r31 >= 3) goto L_800CB28C;
    goto L_800CB274;
L_800CB258:
    goto L_800CB2A0;
L_800CB260:
    *(u32*)(sp + 0x18) = tmp;
    goto L_800CB2A0;
L_800CB274:
    f0 = *(f64*)lbl_8047C578;
    f2 = *(f64*)lbl_8047C528;
    f0 = f1 - f0;
    f1 = f2 - f0;
    goto L_800CB2A0;
L_800CB28C:
    f1 = *(f64*)lbl_8047C578;
    f0 = *(f64*)lbl_8047C528;
    f1 = f2 - f1;
    f1 = f1 - f0;
L_800CB2A0:
    return;
}

/* fn_800CB2B4 - 0x800CB2B4 | size: 0x224 */
void fn_800CB2B4(void) {
    extern u8 lbl_80270008[];
    extern u8 lbl_8047C580[];
    extern u8 lbl_8047C588[];
    extern u8 lbl_8047C590[];
    extern u8 lbl_8047C598[];
    extern u8 lbl_8047C5A0[];
    extern u8 lbl_8047C5A8[];
    extern u8 lbl_8047C5B0[];
    extern u8 lbl_8047C5B8[];
    extern u8 lbl_8047C5C0[];
    extern u8 lbl_8047C5C8[];
    extern u8 lbl_8047C5D0[];
    extern u8 lbl_8047C5D8[];
    extern u8 lbl_8047C5E0[];
    extern u8 lbl_8047C5E8[];
    extern u8 lbl_8047C5F0[];
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    r3 = 0x40860000;
    r4 = (u32)lbl_80270008;
    tmp = r3 + 0x2e42;
    r5 = (u32)lbl_80270008;
    r4 = r8 & 0x7FFFFFFF;
    r7 = (u32)r8 >> 31;
    if (r4 < tmp) goto L_800CB340;
    tmp = 0x7FF00000;
    if (r4 < tmp) goto L_800CB318;
    r3 = r8 & 0xFFFFF;
    /* or. tmp, r3, tmp */;
    if (r4 == tmp) goto L_800CB304;
    f1 = f1 + f1;
    goto L_800CB4D0;
L_800CB304:
    if ((s32)r7 != 0) goto L_800CB310;
    goto L_800CB4D0;
L_800CB310:
    f1 = *(f64*)lbl_8047C580;
    goto L_800CB4D0;
L_800CB318:
    f0 = *(f64*)lbl_8047C588;
    if (f1 <= f0) goto L_800CB32C;
    f1 = *(f64*)lbl_8047C590;
    goto L_800CB4D0;
L_800CB32C:
    f0 = *(f64*)lbl_8047C598;
    if (f1 >= f0) goto L_800CB340;
    f1 = *(f64*)lbl_8047C580;
    goto L_800CB4D0;
L_800CB340:
    r3 = 0x3FD60000;
    tmp = r3 + 0x2e42;
    if (r4 <= tmp) goto L_800CB3E4;
    r3 = 0x3FF10000;
    if (r4 >= tmp) goto L_800CB388;
    r6 = r7 << 3;
    r4 = r5 + 0x10;
    r3 = r5 + 0x20;
    f0 = *(f64*)(r4 + r6);
    tmp = 0x1 - r7;
    f8 = *(f64*)(r3 + r6);
    r6 = tmp - r7;
    f7 = f1 - f0;
    goto L_800CB3D8;
L_800CB388:
    r4 = r7 << 3;
    r3 = r5 + 0x0;
    f1 = *(f64*)lbl_8047C5A0;
    tmp = 0x43300000;
    f0 = *(f64*)(r3 + r4);
    *(u32*)(sp + 0x20) = tmp;
    f2 = f1 * f4 + f0;
    f3 = *(f64*)lbl_8047C5F0;
    f1 = *(f64*)((u8*)r5 + 0x10);
    f0 = *(f64*)((u8*)r5 + 0x20);
    f2 = (f64)(s32)f2;
    *(u32*)(sp + 0x24) = tmp;
    f2 = f2 - f3;
    f7 = -(f2 * f1 - f4);
    f8 = f2 * f0;
L_800CB3D8:
    f0 = f7 - f8;
    goto L_800CB414;
L_800CB3E4:
    tmp = 0x3E300000;
    if (r4 >= tmp) goto L_800CB410;
    f1 = *(f64*)lbl_8047C5A8;
    f0 = *(f64*)lbl_8047C5B0;
    f1 = f1 + f2;
    if (f1 <= f0) goto L_800CB414;
    f1 = f0 + f2;
    goto L_800CB4D0;
L_800CB410:
    r6 = 0x0;
L_800CB414:
    f4 = *(f64*)lbl_8047C5D8;
    f6 = f5 * f5;
    f3 = *(f64*)lbl_8047C5D0;
    f2 = *(f64*)lbl_8047C5C8;
    f1 = *(f64*)lbl_8047C5C0;
    f0 = *(f64*)lbl_8047C5B8;
    f3 = f4 * f6 + f3;
    f2 = f6 * f3 + f2;
    f1 = f6 * f2 + f1;
    f0 = f6 * f1 + f0;
    f3 = -(f6 * f0 - f5);
    if ((s32)r6 != 0) goto L_800CB46C;
    f0 = *(f64*)lbl_8047C5E0;
    f1 = f5 * f3;
    f2 = *(f64*)lbl_8047C5B0;
    f0 = f3 - f0;
    f0 = f1 / f0;
    f0 = f0 - f5;
    f1 = f2 - f0;
    goto L_800CB4D0;
L_800CB46C:
    f0 = *(f64*)lbl_8047C5E0;
    f1 = f5 * f3;
    f2 = *(f64*)lbl_8047C5B0;
    f0 = f0 - f3;
    f0 = f1 / f0;
    f0 = f8 - f0;
    f0 = f0 - f7;
    f0 = f2 - f0;
    if ((s32)r6 < (s32)-0x3fd) goto L_800CB4B0;
    tmp = r6 << 20;
    tmp = r3 + tmp;
    *(u32*)(sp + 0x10) = tmp;
    goto L_800CB4D0;
L_800CB4B0:
    tmp = r6 + 0x3e8;
    tmp = tmp << 20;
    f1 = *(f64*)lbl_8047C5E8;
    tmp = r3 + tmp;
    *(u32*)(sp + 0x10) = tmp;
    f1 = f1 * f0;
L_800CB4D0:
    return;
}

/* fn_800CB4D8 - 0x800CB4D8 | size: 0x33C */
void fn_800CB4D8(void) {
    extern u8 lbl_80270038[];
    u8 sp[0x20];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r8 = r10 & 0x7FFFFFFF;
    /* clrrwi tmp, r6, 31 */;
    /* or. r3, r8, r5 */;
    r7 = r6 ^ tmp;
    if ((s32)tmp == 0) goto L_800CB52C;
    r6 = 0x7FF00000;
    if ((s32)r7 >= (s32)r6) goto L_800CB52C;
    r3 = -r5;
    r3 = r5 | r3;
    r3 = (u32)r3 >> 31;
    r3 = r8 | r3;
    if (r3 <= r6) goto L_800CB540;
L_800CB52C:
    f0 = f1 * f0;
    f1 = f0 / f0;
    goto L_800CB80C;
L_800CB540:
    if ((s32)r7 > (s32)r8) goto L_800CB574;
    if ((s32)r7 < (s32)r8) goto L_800CB554;
    if (r4 >= r5) goto L_800CB55C;
L_800CB554:
    goto L_800CB80C;
L_800CB55C:
    if (r4 != r5) goto L_800CB574;
    r3 = (u32)lbl_80270038;
    tmp = ((tmp << 4) | ((u32)tmp >> 28)) & 0x00000008;
    r3 = (u32)lbl_80270038;
    f1 = *(f64*)(r3 + tmp);
    goto L_800CB80C;
L_800CB574:
    r3 = 0x100000;
    if ((s32)r7 >= (s32)r3) goto L_800CB5C8;
    if ((s32)r7 != 0) goto L_800CB5A8;
    r3 = r4;
    r11 = -0x413;
    goto L_800CB59C;
L_800CB594:
    r3 = r3 << 1;
L_800CB59C:
    if ((s32)r3 > 0) goto L_800CB594;
    goto L_800CB5D0;
L_800CB5A8:
    r3 = r7 << 11;
    r11 = -0x3fe;
    goto L_800CB5BC;
L_800CB5B4:
    r3 = r3 << 1;
L_800CB5BC:
    if ((s32)r3 > 0) goto L_800CB5B4;
    goto L_800CB5D0;
L_800CB5C8:
    r3 = (s32)r7 >> 20;
L_800CB5D0:
    r3 = 0x100000;
    if ((s32)r8 >= (s32)r3) goto L_800CB624;
    if ((s32)r8 != 0) goto L_800CB604;
    r6 = r5;
    r3 = -0x413;
    goto L_800CB5F8;
L_800CB5F0:
    r6 = r6 << 1;
L_800CB5F8:
    if ((s32)r6 > 0) goto L_800CB5F0;
    goto L_800CB62C;
L_800CB604:
    r6 = r8 << 11;
    r3 = -0x3fe;
    goto L_800CB618;
L_800CB610:
    r6 = r6 << 1;
L_800CB618:
    if ((s32)r6 > 0) goto L_800CB610;
    goto L_800CB62C;
L_800CB624:
    r3 = (s32)r8 >> 20;
L_800CB62C:
    if ((s32)r11 < (s32)-0x3fe) goto L_800CB640;
    r6 = r7 & 0xFFFFF;
    r9 = r6 | (0x10 << 16);
    goto L_800CB670;
L_800CB640:
    r9 = -0x3fe - r11;
    if ((s32)r9 > 0x1f) goto L_800CB664;
    r6 = 0x20 - r9;
    r7 = r7 << r9;
    r6 = (u32)r4 >> r6;
    r4 = r4 << r9;
    r9 = r7 | r6;
    goto L_800CB670;
L_800CB664:
    r9 = r4 << r6;
    r4 = 0x0;
L_800CB670:
    if ((s32)r3 < (s32)-0x3fe) goto L_800CB684;
    r6 = r10 & 0xFFFFF;
    r7 = r6 | (0x10 << 16);
    goto L_800CB6B4;
L_800CB684:
    r10 = -0x3fe - r3;
    if ((s32)r10 > 0x1f) goto L_800CB6A8;
    r6 = 0x20 - r10;
    r7 = r8 << r10;
    r6 = (u32)r5 >> r6;
    r5 = r5 << r10;
    r7 = r7 | r6;
    goto L_800CB6B4;
L_800CB6A8:
    r7 = r5 << r6;
    r5 = 0x0;
L_800CB6B4:
    /* subf. r6, r3, r11 */;
    ctr_fn = (void(*)(void))r6;
    if ((s32)r10 == 0x1f) goto L_800CB720;
L_800CB6C0:
    r8 = r9 - r7;
    r10 = r4 - r5;
    if (r4 >= r5) goto L_800CB6D4;
L_800CB6D4:
    if ((s32)r8 >= 0) goto L_800CB6F0;
    r6 = (u32)r4 >> 31;
    r4 = r4 + r4;
    r6 = r9 + r6;
    r9 = r9 + r6;
    goto L_800CB71C;
L_800CB6F0:
    /* or. r4, r8, r10 */;
    if ((s32)r8 != 0) goto L_800CB70C;
    r3 = (u32)lbl_80270038;
    tmp = ((tmp << 4) | ((u32)tmp >> 28)) & 0x00000008;
    r3 = (u32)lbl_80270038;
    f1 = *(f64*)(r3 + tmp);
    goto L_800CB80C;
L_800CB70C:
    r6 = (u32)r10 >> 31;
    r4 = r10 + r10;
    r9 = r8 + r6;
    r9 = r8 + r9;
L_800CB71C:
    if (--ctr != 0) goto L_800CB6C0;
L_800CB720:
    r6 = r9 - r7;
    r5 = r4 - r5;
    if (r4 >= r5) goto L_800CB734;
L_800CB734:
    if ((s32)r6 < 0) goto L_800CB744;
    r9 = r6;
    r4 = r5;
L_800CB744:
    /* or. r5, r9, r4 */;
    if ((s32)r6 != 0) goto L_800CB760;
    r3 = (u32)lbl_80270038;
    tmp = ((tmp << 4) | ((u32)tmp >> 28)) & 0x00000008;
    r3 = (u32)lbl_80270038;
    f1 = *(f64*)(r3 + tmp);
    goto L_800CB80C;
L_800CB760:
    r5 = 0x100000;
    goto L_800CB77C;
L_800CB768:
    r6 = (u32)r4 >> 31;
    r4 = r4 + r4;
    r6 = r9 + r6;
    r9 = r9 + r6;
L_800CB77C:
    if ((s32)r9 < (s32)r5) goto L_800CB768;
    if ((s32)r3 < (s32)-0x3fe) goto L_800CB7AC;
    r3 = r3 + 0x3ff;
    /* subis r5, r9, 0x10 */;
    r3 = r3 << 20;
    r3 = r5 | r3;
    tmp = r3 | tmp;
    *(u32*)(sp + 0x8) = tmp;
    goto L_800CB808;
L_800CB7AC:
    r6 = -0x3fe - r3;
    if ((s32)r6 > 0x14) goto L_800CB7D0;
    r3 = 0x20 - r6;
    r4 = (u32)r4 >> r6;
    r3 = r9 << r3;
    r9 = (s32)r9 >> r6;
    r3 = r4 | r3;
    goto L_800CB7FC;
L_800CB7D0:
    if ((s32)r6 > 0x1f) goto L_800CB7F0;
    r5 = 0x20 - r6;
    r3 = (u32)r4 >> r6;
    r4 = r9 << r5;
    r9 = tmp;
    r3 = r4 | r3;
    goto L_800CB7FC;
L_800CB7F0:
    r3 = (s32)r9 >> r3;
    r9 = tmp;
L_800CB7FC:
    tmp = r9 | tmp;
    *(u32*)(sp + 0x8) = tmp;
L_800CB808:
L_800CB80C:
    return;
}

/* fn_800CB814 - 0x800CB814 | size: 0x27C */
void fn_800CB814(void) {
    extern u8 lbl_8047AA10[];
    extern u8 lbl_8047AA20[];
    extern u8 lbl_8047C5F8[];
    extern u8 lbl_8047C600[];
    extern u8 lbl_8047C608[];
    extern u8 lbl_8047C610[];
    extern u8 lbl_8047C618[];
    extern u8 lbl_8047C620[];
    extern u8 lbl_8047C628[];
    extern u8 lbl_8047C630[];
    extern u8 lbl_8047C638[];
    extern u8 lbl_8047C640[];
    extern u8 lbl_8047C648[];
    extern u8 lbl_8047C650[];
    extern u8 lbl_8047C658[];
    extern u8 lbl_8047C660[];
    extern u8 lbl_8047C668[];
    extern u8 lbl_8047C670[];
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;

    tmp = 0x100000;
    r8 = 0x0;
    if ((s32)r3 >= (s32)tmp) goto L_800CB884;
    tmp = r3 & 0x7FFFFFFF;
    /* or. tmp, tmp, r4 */;
    if ((s32)r3 != (s32)tmp) goto L_800CB850;
    f1 = *(f64*)lbl_8047C5F8;
    f0 = *(f64*)lbl_8047AA20;
    f1 = f1 / f0;
    goto L_800CBA88;
L_800CB850:
    if ((s32)r3 >= 0) goto L_800CB870;
    f1 = f1 - f1;
    f0 = *(f64*)lbl_8047AA20;
    tmp = 0x21;
    *(u32*)lbl_8047AA10 = tmp;
    f1 = f1 / f0;
    goto L_800CBA88;
L_800CB870:
    f0 = *(f64*)lbl_8047C600;
    r8 = -0x36;
    f0 = f1 * f0;
L_800CB884:
    tmp = 0x7FF00000;
    if ((s32)r3 < (s32)tmp) goto L_800CB89C;
    f1 = f0 + f0;
    goto L_800CBA88;
L_800CB89C:
    r6 = r3 & 0xFFFFF;
    r4 = (s32)r3 >> 20;
    r3 = r6 + (0x9 << 16);
    f0 = *(f64*)lbl_8047C608;
    r5 = r3 + 0x5f64;
    r8 = r4 + r8;
    r3 = r5 & 0x00100000;
    tmp = r6 + 0x2;
    r4 = r6 | r3;
    tmp = tmp & 0xFFFFF;
    /* extrwi r3, r5, 1, 11 */;
    r8 = r8 + r3;
    f0 = f1 - f0;
    if ((s32)tmp >= 3) goto L_800CB988;
    f1 = *(f64*)lbl_8047AA20;
    if (f0 != f1) goto L_800CB930;
    if ((s32)r8 != 0) goto L_800CB900;
    goto L_800CBA88;
L_800CB900:
    tmp = 0x43300000;
    f3 = *(f64*)lbl_8047C670;
    *(u32*)(sp + 0x10) = tmp;
    f0 = *(f64*)lbl_8047C618;
    f1 = *(f64*)lbl_8047C610;
    f2 = f2 - f3;
    f0 = f0 * f2;
    f1 = f1 * f2 + f0;
    goto L_800CBA88;
L_800CB930:
    f3 = *(f64*)lbl_8047C628;
    f1 = f0 * f0;
    f2 = *(f64*)lbl_8047C620;
    f2 = -(f3 * f0 - f2);
    f5 = f2 * f1;
    if ((s32)r8 != 0) goto L_800CB954;
    f1 = f0 - f5;
    goto L_800CBA88;
L_800CB954:
    tmp = 0x43300000;
    f4 = *(f64*)lbl_8047C670;
    *(u32*)(sp + 0x10) = tmp;
    f1 = *(f64*)lbl_8047C618;
    f2 = *(f64*)lbl_8047C610;
    f3 = f3 - f4;
    f1 = -(f1 * f3 - f5);
    f0 = f1 - f0;
    f1 = f2 * f3 - f0;
    goto L_800CBA88;
L_800CB988:
    f1 = *(f64*)lbl_8047C630;
    r4 = 0x43300000;
    r3 = 0x70000;
    f1 = f1 + f0;
    /* subis r7, r6, 0x6 */;
    f8 = *(f64*)lbl_8047C650;
    f7 = *(f64*)lbl_8047C648;
    tmp = tmp - r6;
    f1 = f0 / f1;
    f6 = *(f64*)lbl_8047C640;
    /* or. r7, r7, tmp */;
    f4 = *(f64*)lbl_8047C668;
    f3 = *(f64*)lbl_8047C660;
    f11 = f1 * f1;
    f5 = *(f64*)lbl_8047C638;
    f2 = *(f64*)lbl_8047C658;
    f10 = *(f64*)lbl_8047C670;
    f12 = f11 * f11;
    f7 = f8 * f12 + f7;
    f3 = f4 * f12 + f3;
    f4 = f12 * f7 + f6;
    f2 = f12 * f3 + f2;
    f3 = f12 * f4 + f5;
    f2 = f12 * f2;
    f3 = f11 * f3;
    f5 = f9 - f10;
    f3 = f3 + f2;
    if ((s32)r8 <= 0) goto L_800CBA58;
    f2 = *(f64*)lbl_8047C620;
    f2 = f2 * f0;
    f6 = f2 * f0;
    if ((s32)r8 != 0) goto L_800CBA34;
    f2 = f6 + f3;
    f1 = -(f1 * f2 - f6);
    f1 = f0 - f1;
    goto L_800CBA88;
L_800CBA34:
    f2 = *(f64*)lbl_8047C618;
    f3 = f6 + f3;
    f4 = *(f64*)lbl_8047C610;
    f2 = f2 * f5;
    f1 = f1 * f3 + f2;
    f1 = f6 - f1;
    f0 = f1 - f0;
    f1 = f4 * f5 - f0;
    goto L_800CBA88;
L_800CBA58:
    if ((s32)r8 != 0) goto L_800CBA6C;
    f2 = f0 - f3;
    f1 = -(f1 * f2 - f0);
    goto L_800CBA88;
L_800CBA6C:
    f2 = *(f64*)lbl_8047C618;
    f3 = f0 - f3;
    f4 = *(f64*)lbl_8047C610;
    f2 = f2 * f5;
    f1 = f1 * f3 - f2;
    f0 = f1 - f0;
    f1 = f4 * f5 - f0;
L_800CBA88:
    return;
}

/* fn_800CBA90 - 0x800CBA90 | size: 0x830 */
void fn_800CBA90(void) {
    extern u8 lbl_80270048[];
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047AA10[];
    extern u8 lbl_8047C678[];
    extern u8 lbl_8047C680[];
    extern u8 lbl_8047C688[];
    extern u8 lbl_8047C690[];
    extern u8 lbl_8047C698[];
    extern u8 lbl_8047C6A0[];
    extern u8 lbl_8047C6A8[];
    extern u8 lbl_8047C6B0[];
    extern u8 lbl_8047C6B8[];
    extern u8 lbl_8047C6C0[];
    extern u8 lbl_8047C6C8[];
    extern u8 lbl_8047C6D0[];
    extern u8 lbl_8047C6D8[];
    extern u8 lbl_8047C6E0[];
    extern u8 lbl_8047C6E8[];
    extern u8 lbl_8047C6F0[];
    extern u8 lbl_8047C6F8[];
    extern u8 lbl_8047C700[];
    extern u8 lbl_8047C708[];
    extern u8 lbl_8047C710[];
    extern u8 lbl_8047C718[];
    extern u8 lbl_8047C720[];
    extern u8 lbl_8047C728[];
    extern u8 lbl_8047C730[];
    extern u8 lbl_8047C738[];
    extern u8 lbl_8047C740[];
    extern u8 lbl_8047C748[];
    extern u8 lbl_8047C750[];
    extern u8 lbl_8047C758[];
    extern u8 lbl_8047C760[];
    extern u8 lbl_8047C768[];
    extern u8 lbl_8047C770[];
    extern u8 lbl_8047C778[];
    extern u8 lbl_8047C780[];
    extern void fn_800CDE88();
    extern void fn_800CE77C();
    u8 sp[0xB0];
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r3 = (u32)lbl_80270048;
    r3 = (u32)lbl_80270048;
    r7 = r5 & 0x7FFFFFFF;
    /* or. r4, r7, r11 */;
    r6 = tmp & 0x7FFFFFFF;
    if ((s32)tmp != 0) goto L_800CBAFC;
    f1 = *(f64*)lbl_8047C678;
    goto L_800CC288;
L_800CBAFC:
    r4 = 0x7FF00000;
    if ((s32)r6 > (s32)r4) goto L_800CBB3C;
    /* subis r4, r6, 0x7ff0 */;
    if (r4 != 0) goto L_800CBB1C;
    if (r10 != 0) goto L_800CBB3C;
L_800CBB1C:
    r4 = 0x7FF00000;
    if ((s32)r7 > (s32)r4) goto L_800CBB3C;
    /* subis r4, r7, 0x7ff0 */;
    if (r4 != 0) goto L_800CBB4C;
    if (r11 == 0) goto L_800CBB4C;
L_800CBB3C:
    f1 = f1 + f0;
    goto L_800CC288;
L_800CBB4C:
    r4 = 0x0;
    if ((s32)tmp >= 0) goto L_800CBBCC;
    r8 = 0x43400000;
    if ((s32)r7 < (s32)r8) goto L_800CBB6C;
    r4 = 0x2;
    goto L_800CBBCC;
L_800CBB6C:
    r8 = 0x3FF00000;
    if ((s32)r7 < (s32)r8) goto L_800CBBCC;
    r8 = (s32)r7 >> 20;
    if ((s32)r8 <= 0x14) goto L_800CBBA8;
    r8 = 0x34 - r8;
    r9 = (u32)r11 >> r8;
    r8 = r9 << r8;
    if (r11 != r8) goto L_800CBBCC;
    r4 = r9 & 0x1;
    r4 = 0x2 - r4;
    goto L_800CBBCC;
L_800CBBA8:
    if (r11 != 0) goto L_800CBBCC;
    r8 = 0x14 - r8;
    r9 = (s32)r7 >> r8;
    r8 = r9 << r8;
    if ((s32)r7 != (s32)r8) goto L_800CBBCC;
    r4 = r9 & 0x1;
    r4 = 0x2 - r4;
L_800CBBCC:
    if (r11 != 0) goto L_800CBC9C;
    /* subis r8, r7, 0x7ff0 */;
    if (r8 != 0) goto L_800CBC38;
    /* subis tmp, r6, 0x3ff0 */;
    /* or. tmp, tmp, r10 */;
    if (r8 != 0) goto L_800CBBF8;
    f1 = f0 - f0;
    goto L_800CC288;
L_800CBBF8:
    tmp = 0x3FF00000;
    if ((s32)r6 < (s32)tmp) goto L_800CBC1C;
    if ((s32)r5 < 0) goto L_800CBC14;
    goto L_800CC288;
L_800CBC14:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBC1C:
    if ((s32)r5 >= 0) goto L_800CBC30;
    f1 = -f0;
    goto L_800CC288;
L_800CBC30:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBC38:
    /* subis r8, r7, 0x3ff0 */;
    if (r8 != 0) goto L_800CBC64;
    if ((s32)r5 >= 0) goto L_800CBC5C;
    f1 = *(f64*)lbl_8047C678;
    f1 = f1 / f0;
    goto L_800CC288;
L_800CBC5C:
    goto L_800CC288;
L_800CBC64:
    /* subis r8, r5, 0x4000 */;
    if (r8 != 0) goto L_800CBC7C;
    f1 = f0 * f0;
    goto L_800CC288;
L_800CBC7C:
    /* subis r8, r5, 0x3fe0 */;
    if (r8 != 0) goto L_800CBC9C;
    if ((s32)tmp < 0) goto L_800CBC9C;
    fn_800CE77C();
    goto L_800CC288;
L_800CBC9C:
    /* fabs */ f1 = (f0 < 0) ? -f0 : f0;
    if (r10 != 0) goto L_800CBD2C;
    /* subis r8, r6, 0x7ff0 */;
    if (r8 == 0) goto L_800CBCD0;
    if ((s32)r6 == 0) goto L_800CBCD0;
    /* subis r8, r6, 0x3ff0 */;
    if (r8 != 0) goto L_800CBD2C;
L_800CBCD0:
    if ((s32)r5 >= 0) goto L_800CBCE8;
    f0 = *(f64*)lbl_8047C678;
    f0 = f0 / f1;
L_800CBCE8:
    if ((s32)tmp >= 0) goto L_800CBD24;
    /* subis tmp, r6, 0x3ff0 */;
    /* or. tmp, tmp, r4 */;
    if ((s32)tmp != 0) goto L_800CBD10;
    f0 = f0 - f0;
    f0 = f0 / f0;
    goto L_800CBD24;
L_800CBD10:
    if ((s32)r4 != 1) goto L_800CBD24;
    f0 = -f0;
L_800CBD24:
    goto L_800CC288;
L_800CBD2C:
    r8 = (s32)tmp >> 31;
    tmp = r8 + 0x1;
    /* or. r8, tmp, r4 */;
    if ((s32)r4 != 1) goto L_800CBD50;
    r3 = (u32)lbl_80478AC0;
    tmp = 0x21;
    *(u32*)lbl_8047AA10 = tmp;
    f1 = *(f32*)lbl_80478AC0;
    goto L_800CC288;
L_800CBD50:
    r8 = 0x41E00000;
    if ((s32)r7 <= (s32)r8) goto L_800CBE5C;
    r3 = 0x43F00000;
    if ((s32)r7 <= (s32)r3) goto L_800CBDB0;
    r3 = 0x3FF00000;
    if ((s32)r6 > (s32)r7) goto L_800CBD90;
    if ((s32)r5 >= 0) goto L_800CBD88;
    f1 = *(f64*)lbl_8047C688;
    goto L_800CC288;
L_800CBD88:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBD90:
    if ((s32)r6 < (s32)r3) goto L_800CBDB0;
    if ((s32)r5 <= 0) goto L_800CBDA8;
    f1 = *(f64*)lbl_8047C688;
    goto L_800CC288;
L_800CBDA8:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBDB0:
    r3 = 0x3FF00000;
    if ((s32)r6 >= (s32)r7) goto L_800CBDD8;
    if ((s32)r5 >= 0) goto L_800CBDD0;
    f1 = *(f64*)lbl_8047C688;
    goto L_800CC288;
L_800CBDD0:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBDD8:
    if ((s32)r6 <= (s32)r3) goto L_800CBDF8;
    if ((s32)r5 <= 0) goto L_800CBDF0;
    f1 = *(f64*)lbl_8047C688;
    goto L_800CC288;
L_800CBDF0:
    f1 = *(f64*)lbl_8047C680;
    goto L_800CC288;
L_800CBDF8:
    r3 = 0x0;
    f0 = *(f64*)lbl_8047C678;
    f1 = *(f64*)lbl_8047C6A0;
    f6 = f2 - f0;
    f0 = *(f64*)lbl_8047C698;
    f2 = *(f64*)lbl_8047C6A8;
    f3 = *(f64*)lbl_8047C690;
    f4 = -(f1 * f6 - f0);
    f0 = *(f64*)lbl_8047C6B8;
    f1 = *(f64*)lbl_8047C6B0;
    f5 = f6 * f6;
    f3 = -(f6 * f4 - f3);
    f2 = f2 * f6;
    f3 = f5 * f3;
    f0 = f0 * f3;
    f1 = f1 * f6 - f0;
    f0 = f2 + f1;
    f0 = f0 - f2;
    f0 = f1 - f0;
    goto L_800CC044;
L_800CBE5C:
    r5 = 0x100000;
    r10 = 0x0;
    if ((s32)r6 >= (s32)r5) goto L_800CBE84;
    r10 = -0x35;
    f0 = *(f64*)lbl_8047C6C0;
    f0 = f1 * f0;
L_800CBE84:
    r5 = 0x40000;
    r8 = r6 & 0xFFFFF;
    r6 = (s32)r6 >> 20;
    r7 = r8 | (0x3ff0 << 16);
    r10 = r6 + r10;
    if ((s32)r8 > (s32)r5) goto L_800CBEB0;
    r11 = 0x0;
    goto L_800CBED4;
L_800CBEB0:
    r5 = 0xC0000;
    if ((s32)r8 >= (s32)r5) goto L_800CBEC8;
    r11 = 0x1;
    goto L_800CBED4;
L_800CBEC8:
    /* subis r7, r7, 0x10 */;
    r11 = 0x0;
    r10 = r10 + 0x1;
L_800CBED4:
    r5 = (s32)r7 >> 1;
    r9 = r11 << 3;
    r6 = r3 + 0x0;
    r8 = r5 | (0x2000 << 16);
    f5 = *(f64*)(r6 + r9);
    r7 = r3 + 0x20;
    f1 = *(f64*)lbl_8047C680;
    f0 = f30 + f5;
    f2 = *(f64*)lbl_8047C678;
    r5 = 0x43300000;
    r10 = r3 + 0x10;
    f31 = f30 - f5;
    f4 = *(f64*)lbl_8047C6F0;
    f28 = f2 / f0;
    f0 = *(f64*)lbl_8047C6E8;
    f3 = *(f64*)lbl_8047C6E0;
    r8 = r8 + (0x8 << 16);
    r3 = r11 << 18;
    f1 = f31 * f28;
    r3 = r8 + r3;
    r3 = 0x0;
    f2 = *(f64*)lbl_8047C6D8;
    f27 = f1 * f1;
    f11 = *(f64*)lbl_8047C6D0;
    f13 = f12 - f5;
    f9 = *(f64*)lbl_8047C6C8;
    f4 = f4 * f27 + f0;
    f10 = *(f64*)lbl_8047C6F8;
    f0 = f29 * f29;
    f5 = *(f64*)lbl_8047C710;
    f6 = *(f64*)lbl_8047C708;
    f3 = f27 * f4 + f3;
    f8 = *(f64*)lbl_8047C700;
    f7 = *(f64*)(r7 + r9);
    f31 = -(f29 * f12 - f31);
    f4 = *(f64*)lbl_8047C780;
    f12 = f27 * f3 + f2;
    f2 = *(f64*)(r10 + r9);
    f30 = f30 - f13;
    f13 = f27 * f27;
    f11 = f27 * f12 + f11;
    f12 = -(f29 * f30 - f31);
    f9 = f27 * f11 + f9;
    f27 = f28 * f12;
    f12 = f13 * f9;
    f11 = f29 + f1;
    f9 = f10 + f0;
    f3 = f3 - f4;
    f12 = f27 * f11 + f12;
    f4 = f9 + f12;
    f4 = f9 - f10;
    f10 = f29 * f9;
    f0 = f4 - f0;
    f0 = f12 - f0;
    f0 = f0 * f1;
    f4 = f27 * f9 + f0;
    f0 = f10 + f4;
    f0 = f1 - f10;
    f8 = f8 * f1;
    f0 = f4 - f0;
    f0 = f5 * f0;
    f0 = f6 * f1 + f0;
    f1 = f7 + f0;
    f0 = f8 + f1;
    f0 = f0 + f2;
    f0 = f3 + f0;
    f0 = f0 - f3;
    f0 = f0 - f2;
    f0 = f0 - f8;
    f0 = f1 - f0;
L_800CC044:
    f31 = *(f64*)lbl_8047C678;
    /* or. tmp, tmp, r3 */;
    if ((s32)r8 != (s32)r5) goto L_800CC058;
    f31 = *(f64*)lbl_8047C718;
L_800CC058:
    r3 = 0x0;
    tmp = 0x40900000;
    f0 = f1 * f0;
    f1 = f1 - f2;
    f2 = f2 * f3;
    f12 = f3 * f1 + f0;
    f0 = f12 + f2;
    if ((s32)r6 < (s32)tmp) goto L_800CC0E0;
    /* subis tmp, r6, 0x4090 */;
    /* or. tmp, tmp, r5 */;
    if ((s32)r6 == (s32)tmp) goto L_800CC0BC;
    f1 = *(f64*)lbl_8047C720;
    f0 = f1 * f31;
    f1 = f1 * f0;
    goto L_800CC288;
L_800CC0BC:
    f1 = *(f64*)lbl_8047C728;
    f0 = f0 - f2;
    f1 = f1 + f12;
    if (f1 <= f0) goto L_800CC134;
    f1 = *(f64*)lbl_8047C720;
    f0 = f1 * f31;
    f1 = f1 * f0;
    goto L_800CC288;
L_800CC0E0:
    r3 = 0x40910000;
    r4 = r6 & 0x7FFFFFFF;
    if ((s32)r4 < (s32)tmp) goto L_800CC134;
    r3 = r6 + (0x3f6f << 16);
    tmp = r3 + 0x3400;
    /* or. tmp, tmp, r5 */;
    if ((s32)r4 == (s32)tmp) goto L_800CC114;
    f1 = *(f64*)lbl_8047C730;
    f0 = f1 * f31;
    f1 = f1 * f0;
    goto L_800CC288;
L_800CC114:
    f0 = f0 - f2;
    /* cror eq, lt, eq */;
    if (f12 != f0) goto L_800CC134;
    f1 = *(f64*)lbl_8047C730;
    f0 = f1 * f31;
    f1 = f1 * f0;
    goto L_800CC288;
L_800CC134:
    r3 = r6 & 0x7FFFFFFF;
    tmp = 0x3FE00000;
    /* extrwi r4, r6, 11, 1 */;
    r3 = 0x0;
    if ((s32)r3 <= (s32)tmp) goto L_800CC1AC;
    r3 = 0x100000;
    tmp = (s32)r3 >> tmp;
    f0 = *(f64*)lbl_8047C680;
    r7 = r6 + tmp;
    tmp = r7 & 0x7FFFFFFF;
    r4 = (s32)tmp >> 20;
    tmp = r7 & 0xFFFFF;
    r4 = (s32)r3 >> r5;
    r3 = tmp | (0x10 << 16);
    r4 = r7 & ~r4;
    tmp = 0x14 - r5;
    r3 = (s32)r3 >> tmp;
    if ((s32)r6 >= 0) goto L_800CC19C;
    r3 = -r3;
L_800CC19C:
    f0 = f1 - f0;
L_800CC1AC:
    tmp = 0x0;
    f1 = *(f64*)lbl_8047C748;
    r4 = r3 << 20;
    f0 = f12 + f2;
    f10 = *(f64*)lbl_8047C738;
    f9 = *(f64*)lbl_8047C740;
    f6 = *(f64*)lbl_8047C770;
    f5 = *(f64*)lbl_8047C768;
    *(u32*)(sp + 0x2C) = tmp;
    f0 = *(f64*)lbl_8047C760;
    f4 = *(f64*)lbl_8047C758;
    f8 = f11 - f2;
    f3 = *(f64*)lbl_8047C750;
    f7 = f1 * f11;
    f2 = *(f64*)lbl_8047C778;
    f1 = *(f64*)lbl_8047C678;
    f8 = f12 - f8;
    f10 = f10 * f11;
    f11 = f9 * f8 + f7;
    f9 = f10 + f11;
    f7 = f9 * f9;
    f8 = f9 - f10;
    f5 = f6 * f7 + f5;
    f6 = f11 - f8;
    f5 = f7 * f5 + f0;
    f0 = f9 * f6 + f6;
    f4 = f7 * f5 + f4;
    f3 = f7 * f4 + f3;
    f3 = f7 * f3;
    f4 = f9 - f3;
    f3 = f9 * f4;
    f2 = f4 - f2;
    f2 = f3 / f2;
    f0 = f2 - f0;
    f0 = f0 - f9;
    f1 = f1 - f0;
    tmp = tmp + r4;
    /* srawi. tmp, tmp, 20 */;
    if ((s32)r6 > 0) goto L_800CC274;
    fn_800CDE88();
    goto L_800CC280;
L_800CC274:
    tmp = tmp + r4;
    *(u32*)(sp + 0x50) = tmp;
L_800CC280:
    f1 = f31 * f0;
L_800CC288:
    return;
}

/* fn_800CC2C0 - 0x800CC2C0 | size: 0x3A0 */
void fn_800CC2C0(void) {
    extern u8 lbl_80270078[];
    extern u8 lbl_80270180[];
    extern u8 lbl_8047C788[];
    extern u8 lbl_8047C790[];
    extern u8 lbl_8047C798[];
    extern u8 lbl_8047C7A0[];
    extern u8 lbl_8047C7A8[];
    extern u8 lbl_8047C7B0[];
    extern u8 lbl_8047C7B8[];
    extern u8 lbl_8047C7C0[];
    extern u8 lbl_8047C7C8[];
    extern u8 lbl_8047C7D0[];
    extern u8 lbl_8047C7D8[];
    extern void fn_800CC754();
    u8 sp[0x60];
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
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f9 = 0.0f;

    r4 = 0x3FE90000;
    tmp = r4 + 0x21fb;
    r30 = r3;
    r6 = r31 & 0x7FFFFFFF;
    if ((s32)r6 > (s32)tmp) goto L_800CC308;
    *(f64*)((u8*)r30 + 0x0) = f1;
    r3 = 0x0;
    f0 = *(f64*)lbl_8047C788;
    *(f64*)((u8*)r30 + 0x8) = f0;
    goto L_800CC648;
L_800CC308:
    r3 = 0x40030000;
    if ((s32)r6 >= (s32)tmp) goto L_800CC3F0;
    if ((s32)r31 <= 0) goto L_800CC388;
    f0 = *(f64*)lbl_8047C790;
    /* subis tmp, r6, 0x3ff9 */;
    f2 = f1 - f0;
    if (tmp == 0x21fb) goto L_800CC358;
    f1 = *(f64*)lbl_8047C798;
    f0 = f2 - f1;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x0);
    f0 = f2 - f0;
    f0 = f0 - f1;
    *(f64*)((u8*)r30 + 0x8) = f0;
    goto L_800CC380;
L_800CC358:
    f0 = *(f64*)lbl_8047C7A0;
    f1 = *(f64*)lbl_8047C7A8;
    f2 = f2 - f0;
    f0 = f2 - f1;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x0);
    f0 = f2 - f0;
    f0 = f0 - f1;
    *(f64*)((u8*)r30 + 0x8) = f0;
L_800CC380:
    r3 = 0x1;
    goto L_800CC648;
L_800CC388:
    f0 = *(f64*)lbl_8047C790;
    /* subis tmp, r6, 0x3ff9 */;
    f2 = f0 + f1;
    if (tmp == 0x21fb) goto L_800CC3C0;
    f1 = *(f64*)lbl_8047C798;
    f0 = f1 + f2;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x0);
    f0 = f2 - f0;
    f0 = f1 + f0;
    *(f64*)((u8*)r30 + 0x8) = f0;
    goto L_800CC3E8;
L_800CC3C0:
    f0 = *(f64*)lbl_8047C7A0;
    f1 = *(f64*)lbl_8047C7A8;
    f2 = f2 + f0;
    f0 = f1 + f2;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x0);
    f0 = f2 - f0;
    f0 = f1 + f0;
    *(f64*)((u8*)r30 + 0x8) = f0;
L_800CC3E8:
    r3 = -0x1;
    goto L_800CC648;
L_800CC3F0:
    r3 = 0x41390000;
    tmp = r3 + 0x21fb;
    if ((s32)r6 > (s32)tmp) goto L_800CC538;
    /* fabs */ f4 = (f1 < 0) ? -f1 : f1;
    tmp = 0x43300000;
    f1 = *(f64*)lbl_8047C7B8;
    f0 = *(f64*)lbl_8047C7B0;
    *(u32*)(sp + 0x38) = tmp;
    f2 = f1 * f4 + f0;
    f3 = *(f64*)lbl_8047C7D8;
    f1 = *(f64*)lbl_8047C790;
    f0 = *(f64*)lbl_8047C798;
    f2 = (f64)(s32)f2;
    *(u32*)(sp + 0x3C) = tmp;
    f5 = f2 - f3;
    f4 = -(f1 * f5 - f4);
    f1 = f0 * f5;
    if ((s32)r3 >= 0x20) goto L_800CC478;
    r4 = (u32)lbl_80270180;
    tmp = r3 << 2;
    r4 = (u32)lbl_80270180;
    r4 = r4 + tmp;
    tmp = *(u32*)((u8*)r4 + (-4));
    if ((s32)r6 == (s32)tmp) goto L_800CC478;
    f0 = f4 - f1;
    *(f64*)((u8*)r30 + 0x0) = f0;
    goto L_800CC4FC;
L_800CC478:
    f0 = f4 - f1;
    r4 = (s32)r6 >> 20;
    *(f64*)((u8*)r30 + 0x0) = f0;
    tmp = *(u32*)((u8*)r30 + 0x0);
    /* extrwi tmp, tmp, 11, 1 */;
    tmp = r4 - tmp;
    if ((s32)tmp <= 0x10) goto L_800CC4FC;
    f0 = *(f64*)lbl_8047C7A0;
    f3 = f4;
    f1 = *(f64*)lbl_8047C7A8;
    f2 = f0 * f5;
    f4 = f4 - f2;
    f0 = f3 - f4;
    f0 = f0 - f2;
    f1 = f1 * f5 - f0;
    f0 = f4 - f1;
    *(f64*)((u8*)r30 + 0x0) = f0;
    tmp = *(u32*)((u8*)r30 + 0x0);
    /* extrwi tmp, tmp, 11, 1 */;
    tmp = r4 - tmp;
    if ((s32)tmp <= 0x31) goto L_800CC4FC;
    f0 = *(f64*)lbl_8047C7C0;
    f2 = f4;
    f1 = *(f64*)lbl_8047C7C8;
    f3 = f0 * f5;
    f4 = f4 - f3;
    f0 = f2 - f4;
    f0 = f0 - f3;
    f1 = f1 * f5 - f0;
    f0 = f4 - f1;
    *(f64*)((u8*)r30 + 0x0) = f0;
L_800CC4FC:
    f0 = *(f64*)((u8*)r30 + 0x0);
    f0 = f4 - f0;
    f0 = f0 - f1;
    *(f64*)((u8*)r30 + 0x8) = f0;
    if ((s32)r31 >= 0) goto L_800CC648;
    f0 = *(f64*)((u8*)r30 + 0x0);
    r3 = -r3;
    f0 = -f0;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x8);
    f0 = -f0;
    *(f64*)((u8*)r30 + 0x8) = f0;
    goto L_800CC648;
    goto L_800CC648;
L_800CC538:
    tmp = 0x7FF00000;
    if ((s32)r6 < (s32)tmp) goto L_800CC558;
    f0 = f1 - f1;
    r3 = 0x0;
    *(f64*)((u8*)r30 + 0x8) = f0;
    *(f64*)((u8*)r30 + 0x0) = f0;
    goto L_800CC648;
L_800CC558:
    r3 = (s32)r6 >> 20;
    tmp = 0x43300000;
    r3 = r5 << 20;
    *(u32*)(sp + 0x30) = tmp;
    r3 = r6 - r3;
    f5 = *(f64*)lbl_8047C7D8;
    r4 = (u32)sp + 0x30;
    f4 = *(f64*)lbl_8047C7D0;
    r6 = 0x3;
    f1 = *(f64*)lbl_8047C788;
    *(u32*)(sp + 0x48) = tmp;
    f0 = (f64)(s32)f3;
    *(u32*)(sp + 0x34) = tmp;
    f2 = f0 - f5;
    f0 = f3 - f2;
    f3 = f4 * f0;
    f0 = (f64)(s32)f3;
    *(u32*)(sp + 0x4C) = tmp;
    f2 = f0 - f5;
    f0 = f3 - f2;
    f0 = f4 * f0;
    goto L_800CC600;
L_800CC5F8:
L_800CC600:
    f0 = *(f64*)((u8*)r4 + (-8));
    if (f1 == f0) goto L_800CC5F8;
    r3 = (u32)lbl_80270078;
    r4 = r30;
    r8 = (u32)lbl_80270078;
    r7 = 0x2;
    r3 = (u32)sp + 0x18;
    fn_800CC754();
    if ((s32)r31 >= 0) goto L_800CC648;
    f0 = *(f64*)((u8*)r30 + 0x0);
    r3 = -r3;
    f0 = -f0;
    *(f64*)((u8*)r30 + 0x0) = f0;
    f0 = *(f64*)((u8*)r30 + 0x8);
    f0 = -f0;
    *(f64*)((u8*)r30 + 0x8) = f0;
L_800CC648:
    return;
}

/* fn_800CC660 - 0x800CC660 | size: 0xF4 */
void fn_800CC660(void) {
    extern u8 lbl_8047C7E0[];
    extern u8 lbl_8047C7E8[];
    extern u8 lbl_8047C7F0[];
    extern u8 lbl_8047C7F8[];
    extern u8 lbl_8047C800[];
    extern u8 lbl_8047C808[];
    extern u8 lbl_8047C810[];
    extern u8 lbl_8047C818[];
    extern u8 lbl_8047C820[];
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;

    tmp = 0x3E400000;
    r4 = r3 & 0x7FFFFFFF;
    if ((s32)r4 >= (s32)tmp) goto L_800CC698;
    f0 = (f64)(s32)f1;
    if ((s32)tmp != 0) goto L_800CC698;
    f1 = *(f64*)lbl_8047C7E0;
    goto L_800CC74C;
L_800CC698:
    r3 = 0x3FD30000;
    tmp = r3 + 0x3333;
    f5 = *(f64*)lbl_8047C810;
    f7 = f6 * f6;
    f0 = *(f64*)lbl_8047C808;
    f4 = *(f64*)lbl_8047C800;
    f3 = *(f64*)lbl_8047C7F8;
    f1 = *(f64*)lbl_8047C7F0;
    f5 = f5 * f7 + f0;
    f0 = *(f64*)lbl_8047C7E8;
    f4 = f7 * f5 + f4;
    f3 = f7 * f4 + f3;
    f1 = f7 * f3 + f1;
    f0 = f7 * f1 + f0;
    f4 = f7 * f0;
    if ((s32)r4 >= (s32)tmp) goto L_800CC6FC;
    f0 = f6 * f2;
    f1 = *(f64*)lbl_8047C818;
    f2 = *(f64*)lbl_8047C7E0;
    f0 = f7 * f4 - f0;
    f0 = f1 * f7 - f0;
    f1 = f2 - f0;
    goto L_800CC74C;
L_800CC6FC:
    tmp = 0x3FE90000;
    if ((s32)r4 <= (s32)tmp) goto L_800CC714;
    f0 = *(f64*)lbl_8047C820;
    goto L_800CC724;
L_800CC714:
    /* subis r3, r4, 0x20 */;
    tmp = 0x0;
    *(u32*)(sp + 0x14) = tmp;
L_800CC724:
    f0 = f0 * f2;
    f1 = *(f64*)lbl_8047C818;
    f2 = *(f64*)lbl_8047C7E0;
    f1 = f1 * f7 - f3;
    f0 = f7 * f4 - f0;
    f2 = f2 - f3;
    f0 = f1 - f0;
    f1 = f2 - f0;
L_800CC74C:
    return;
}

/* fn_800CC754 - 0x800CC754 | size: 0xE54 */
void fn_800CC754(void) {
    extern u8 lbl_80270200[];
    extern u8 lbl_80270210[];
    extern u8 lbl_8047C828[];
    extern u8 lbl_8047C830[];
    extern u8 lbl_8047C838[];
    extern u8 lbl_8047C840[];
    extern u8 lbl_8047C848[];
    extern u8 lbl_8047C850[];
    extern u8 lbl_8047C858[];
    extern u8 lbl_8047C860[];
    extern void fn_800CDCB4();
    extern void fn_800CDE88();
    u8 sp[0x2D0];
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
    u32 r12 = 0;
    u32 r16 = 0;
    u32 r17 = 0;
    u32 r18 = 0;
    u32 r19 = 0;
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
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f8 = 0.0f;
    f32 f25 = 0.0f;
    f32 f26 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r11 = (u32)sp + 0x2d0;
    r9 = 0x2AAB0000;
    r23 = r7;
    r10 = (u32)lbl_80270200;
    r9 = r23 << 2;
    tmp = (s32)((s64)r7 * (s64)tmp >> 32);
    r7 = (u32)lbl_80270200;
    r28 = *(u32*)(r7 + r9);
    r21 = r3;
    r22 = r4;
    r24 = r8;
    tmp = (s32)tmp >> 2;
    r3 = (u32)tmp >> 31;
    /* add. r29, tmp, r3 */;
    if ((s32)tmp >= 0) goto L_800CC7B4;
    r29 = 0x0;
L_800CC7B4:
    tmp = r29 + 0x1;
    /* add. r7, r30, r28 */;
    r4 = tmp * 0x18;
    r6 = r29 - r30;
    f1 = *(f64*)lbl_8047C860;
    r3 = r6 << 2;
    r26 = r5 - r4;
    tmp = r7 + 0x1;
    r4 = r24 + r3;
    r5 = (u32)sp + 0x198;
    r3 = 0x43300000;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)tmp < 0) goto L_800CC824;
L_800CC7E8:
    if ((s32)r6 >= 0) goto L_800CC7F8;
    f0 = *(f64*)lbl_8047C828;
    goto L_800CC810;
L_800CC7F8:
    tmp = *(u32*)((u8*)r4 + 0x0);
    *(u32*)(sp + 0x23C) = tmp;
    f0 = f0 - f1;
L_800CC810:
    *(f64*)((u8*)r5 + 0x0) = f0;
    r5 = r5 + 0x8;
    r4 = r4 + 0x4;
    r6 = r6 + 0x1;
    if (--ctr != 0) goto L_800CC7E8;
L_800CC824:
    r5 = (u32)sp + 0x58;
    r7 = 0x0;
    goto L_800CC988;
L_800CC830:
    f4 = *(f64*)lbl_8047C828;
    r6 = 0x0;
    if ((s32)r30 < 0) goto L_800CC97C;
    tmp = r30 + 0x1;
    if ((s32)tmp <= 8) goto L_800CC938;
    r8 = r9 + 0x8;
    r4 = r21;
    r8 = (u32)r8 >> 3;
    tmp = r30 + r7;
    r3 = (u32)sp + 0x198;
    ctr_fn = (void(*)(void))r8;
    if ((s32)r9 < 0) goto L_800CC938;
L_800CC870:
    r8 = tmp - r6;
    r9 = r6 + 0x1;
    r10 = r8 << 3;
    f1 = *(f64*)((u8*)r4 + 0x0);
    f0 = *(f64*)(r3 + r10);
    r9 = tmp - r9;
    r10 = r9 << 3;
    r8 = r6 + 0x2;
    f4 = f1 * f0 + f4;
    r9 = tmp - r8;
    r8 = r6 + 0x3;
    f1 = *(f64*)((u8*)r4 + 0x8);
    f0 = *(f64*)(r3 + r10);
    r8 = tmp - r8;
    f4 = f1 * f0 + f4;
    r9 = r9 << 3;
    f1 = *(f64*)((u8*)r4 + 0x10);
    r10 = r8 << 3;
    f0 = *(f64*)(r3 + r9);
    r8 = r6 + 0x4;
    f4 = f1 * f0 + f4;
    f1 = *(f64*)((u8*)r4 + 0x18);
    f0 = *(f64*)(r3 + r10);
    r9 = tmp - r8;
    r10 = r9 << 3;
    r8 = r6 + 0x5;
    f4 = f1 * f0 + f4;
    f2 = *(f64*)((u8*)r4 + 0x20);
    f0 = *(f64*)(r3 + r10);
    r8 = tmp - r8;
    r10 = r8 << 3;
    r9 = r6 + 0x6;
    f4 = f2 * f0 + f4;
    r9 = tmp - r9;
    r9 = r9 << 3;
    f1 = *(f64*)((u8*)r4 + 0x28);
    f0 = *(f64*)(r3 + r10);
    r8 = r6 + 0x7;
    f4 = f1 * f0 + f4;
    r8 = tmp - r8;
    r8 = r8 << 3;
    f3 = *(f64*)((u8*)r4 + 0x30);
    f2 = *(f64*)(r3 + r9);
    r6 = r6 + 0x8;
    f1 = *(f64*)((u8*)r4 + 0x38);
    f4 = f3 * f2 + f4;
    f0 = *(f64*)(r3 + r8);
    r4 = r4 + 0x40;
    f4 = f1 * f0 + f4;
    if (--ctr != 0) goto L_800CC870;
L_800CC938:
    tmp = r30 + 0x1;
    r3 = r6 << 3;
    tmp = tmp - r6;
    r8 = r30 + r7;
    r4 = r21 + r3;
    r3 = (u32)sp + 0x198;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r6 > (s32)r30) goto L_800CC97C;
L_800CC95C:
    tmp = r8 - r6;
    f1 = *(f64*)((u8*)r4 + 0x0);
    tmp = tmp << 3;
    r4 = r4 + 0x8;
    f0 = *(f64*)(r3 + tmp);
    r6 = r6 + 0x1;
    f4 = f1 * f0 + f4;
    if (--ctr != 0) goto L_800CC95C;
L_800CC97C:
    *(f64*)((u8*)r5 + 0x0) = f4;
    r5 = r5 + 0x8;
    r7 = r7 + 0x1;
L_800CC988:
    if ((s32)r7 <= (s32)r28) goto L_800CC830;
    r18 = 0x18 - r26;
    f26 = *(f64*)lbl_8047C830;
    f27 = *(f64*)lbl_8047C860;
    r16 = (u32)sp + 0x8;
    f28 = *(f64*)lbl_8047C838;
    r31 = r28;
    f29 = *(f64*)lbl_8047C848;
    r17 = 0x17 - r26;
    f30 = *(f64*)lbl_8047C840;
    r20 = (u32)sp + 0x198;
    f31 = *(f64*)lbl_8047C828;
    r19 = 0x43300000;
L_800CC9C0:
    tmp = r31 << 3;
    r5 = (u32)sp + 0x58;
    r5 = r5 + tmp;
    f1 = *(f64*)((u8*)r5 + 0x0);
    r4 = r16;
    r3 = r31;
    if ((s32)r31 <= 0) goto L_800CCAC8;
    /* srwi. tmp, r31, 1 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 0) goto L_800CCA7C;
L_800CC9EC:
    f3 = f26 * f1;
    f0 = *(f64*)((u8*)r5 + (-8));
    f2 = (f64)(s32)f3;
    *(u32*)(sp + 0x244) = tmp;
    f3 = f2 - f27;
    f2 = -(f28 * f3 - f1);
    f1 = f3 + f0;
    /* lfdu f0, -0x10(r5) */;
    f2 = (f64)(s32)f2;
    f3 = f26 * f1;
    f2 = (f64)(s32)f3;
    *(u32*)((u8*)r4 + 0x0) = tmp;
    *(u32*)(sp + 0x244) = tmp;
    f3 = f2 - f27;
    f2 = -(f28 * f3 - f1);
    f1 = f3 + f0;
    f2 = (f64)(s32)f2;
    *(u32*)((u8*)r4 + 0x4) = tmp;
    r4 = r4 + 0x8;
    if (--ctr != 0) goto L_800CC9EC;
    r3 = r3 & 0x1;
    if ((s32)r31 == 0) goto L_800CCAC8;
L_800CCA7C:
    ctr_fn = (void(*)(void))r3;
L_800CCA80:
    f3 = f26 * f1;
    /* lfdu f0, -0x8(r5) */;
    f2 = (f64)(s32)f3;
    *(u32*)(sp + 0x244) = tmp;
    f3 = f2 - f27;
    f2 = -(f28 * f3 - f1);
    f1 = f3 + f0;
    f2 = (f64)(s32)f2;
    *(u32*)((u8*)r4 + 0x0) = tmp;
    r4 = r4 + 0x4;
    if (--ctr != 0) goto L_800CCA80;
L_800CCAC8:
    r3 = r26;
    fn_800CDE88();
    f25 = f1;
    f1 = f29 * f25;
    fn_800CDCB4();
    f25 = -(f30 * f1 - f25);
    r25 = 0x0;
    f0 = (f64)(s32)f25;
    *(u32*)(sp + 0x244) = tmp;
    f0 = f0 - f27;
    f25 = f25 - f0;
    if ((s32)r26 <= 0) goto L_800CCB40;
    r3 = r31 << 2;
    r4 = (u32)sp + 0x8;
    r3 = *(u32*)(r4 + r5);
    r6 = (s32)r3 >> r18;
    tmp = r6 << r18;
    tmp = r3 - tmp;
    r27 = r27 + r6;
    *(u32*)(r4 + r5) = tmp;
    tmp = *(u32*)(r4 + r5);
    r25 = (s32)tmp >> r17;
    goto L_800CCB70;
L_800CCB40:
    if ((s32)r26 != 0) goto L_800CCB5C;
    r4 = r31 << 2;
    r3 = (u32)sp + 0x8;
    tmp = *(u32*)(r3 + tmp);
    r25 = (s32)tmp >> 23;
    goto L_800CCB70;
L_800CCB5C:
    f0 = *(f64*)lbl_8047C850;
    /* cror eq, gt, eq */;
    if (f25 != f0) goto L_800CCB70;
    r25 = 0x2;
L_800CCB70:
    if ((s32)r25 <= 0) goto L_800CCC44;
    r5 = 0x1000000;
    r6 = r16;
    tmp = 0x0;
    ctr_fn = (void(*)(void))r31;
    r27 = r27 + 0x1;
    if ((s32)r31 <= 0) goto L_800CCBCC;
L_800CCB98:
    r3 = *(u32*)((u8*)r6 + 0x0);
    if ((s32)tmp != 0) goto L_800CCBBC;
    if ((s32)r3 == 0) goto L_800CCBC4;
    r3 = r5 - r3;
    tmp = 0x1;
    *(u32*)((u8*)r6 + 0x0) = r3;
    goto L_800CCBC4;
L_800CCBBC:
    r3 = r4 - r3;
    *(u32*)((u8*)r6 + 0x0) = r3;
L_800CCBC4:
    r6 = r6 + 0x4;
    if (--ctr != 0) goto L_800CCB98;
L_800CCBCC:
    if ((s32)r26 <= 0) goto L_800CCC20;
    if ((s32)r26 == 2) goto L_800CCC08;
    if ((s32)r26 >= 2) goto L_800CCC20;
    if ((s32)r26 >= 1) goto L_800CCBEC;
    goto L_800CCC20;
L_800CCBEC:
    r3 = r31 << 2;
    r5 = (u32)sp + 0x8;
    r3 = *(u32*)(r5 + r4);
    r3 = r3 & 0x7FFFFF;
    *(u32*)(r5 + r4) = r3;
    goto L_800CCC20;
L_800CCC08:
    r3 = r31 << 2;
    r5 = (u32)sp + 0x8;
    r3 = *(u32*)(r5 + r4);
    r3 = r3 & 0x3FFFFF;
    *(u32*)(r5 + r4) = r3;
L_800CCC20:
    if ((s32)r25 != 2) goto L_800CCC44;
    f1 = *(f64*)lbl_8047C858;
    f25 = f1 - f25;
    if ((s32)tmp == 0) goto L_800CCC44;
    r3 = r26;
    fn_800CDE88();
    f25 = f25 - f1;
L_800CCC44:
    if (f31 != f25) goto L_800CCEAC;
    r4 = (u32)sp + 0x8;
    r5 = 0x0;
    tmp = r3 << 2;
    r3 = r3 + 0x1;
    r4 = r4 + tmp;
    r3 = r3 - r28;
    if ((s32)r3 < (s32)r28) goto L_800CCCE0;
    /* srwi. tmp, r3, 3 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r3 == (s32)r28) goto L_800CCCCC;
L_800CCC7C:
    tmp = *(u32*)((u8*)r4 + 0x0);
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-4));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-8));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-12));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-16));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-20));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-24));
    r5 = r5 | tmp;
    tmp = *(u32*)((u8*)r4 + (-28));
    r5 = r5 | tmp;
    if (--ctr != 0) goto L_800CCC7C;
    r3 = r3 & 0x7;
    if ((s32)r3 == (s32)r28) goto L_800CCCE0;
L_800CCCCC:
    ctr_fn = (void(*)(void))r3;
L_800CCCD0:
    tmp = *(u32*)((u8*)r4 + 0x0);
    r5 = r5 | tmp;
    if (--ctr != 0) goto L_800CCCD0;
L_800CCCE0:
    if ((s32)r5 != 0) goto L_800CCEAC;
    r10 = 0x1;
    goto L_800CCCF4;
L_800CCCF0:
    r10 = r10 + 0x1;
L_800CCCF4:
    tmp = r28 - r10;
    tmp = tmp << 2;
    tmp = *(u32*)(r16 + tmp);
    if ((s32)tmp == 0) goto L_800CCCF0;
    r9 = r31 + 0x1;
    r5 = (u32)sp + 0x58;
    tmp = r9 << 3;
    r6 = r31 + r10;
    r5 = r5 + tmp;
    goto L_800CCE9C;
L_800CCD20:
    tmp = r29 + r9;
    r7 = r30 + r9;
    tmp = tmp << 2;
    r3 = *(u32*)(r24 + tmp);
    tmp = r7 << 3;
    f4 = *(f64*)lbl_8047C828;
    r8 = 0x0;
    f0 = f0 - f27;
    *(f64*)(r20 + tmp) = f0;
    if ((s32)r30 < 0) goto L_800CCE90;
    tmp = r30 + 0x1;
    if ((s32)tmp <= 8) goto L_800CCE50;
    tmp = r11 + 0x8;
    r4 = r21;
    tmp = (u32)tmp >> 3;
    r3 = (u32)sp + 0x198;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r11 < 0) goto L_800CCE50;
L_800CCD88:
    r11 = r7 - r8;
    tmp = r8 + 0x1;
    r12 = r11 << 3;
    f1 = *(f64*)((u8*)r4 + 0x0);
    f0 = *(f64*)(r3 + r12);
    tmp = r7 - tmp;
    r11 = r8 + 0x2;
    f2 = *(f64*)((u8*)r4 + 0x20);
    f4 = f1 * f0 + f4;
    r12 = r7 - r11;
    tmp = tmp << 3;
    r11 = r8 + 0x3;
    f0 = *(f64*)(r3 + tmp);
    r25 = r12 << 3;
    f1 = *(f64*)((u8*)r4 + 0x8);
    r11 = r7 - r11;
    r12 = r11 << 3;
    tmp = r8 + 0x4;
    f4 = f1 * f0 + f4;
    f1 = *(f64*)((u8*)r4 + 0x10);
    f0 = *(f64*)(r3 + r25);
    r11 = r7 - tmp;
    tmp = r8 + 0x5;
    f3 = *(f64*)((u8*)r4 + 0x30);
    f4 = f1 * f0 + f4;
    f0 = *(f64*)(r3 + r12);
    f1 = *(f64*)((u8*)r4 + 0x18);
    r12 = r11 << 3;
    r11 = r8 + 0x6;
    tmp = r7 - tmp;
    f4 = f1 * f0 + f4;
    f0 = *(f64*)(r3 + r12);
    r12 = tmp << 3;
    f1 = *(f64*)((u8*)r4 + 0x28);
    tmp = r8 + 0x7;
    r11 = r7 - r11;
    f4 = f2 * f0 + f4;
    f0 = *(f64*)(r3 + r12);
    r11 = r11 << 3;
    tmp = r7 - tmp;
    f2 = *(f64*)(r3 + r11);
    tmp = tmp << 3;
    f4 = f1 * f0 + f4;
    f1 = *(f64*)((u8*)r4 + 0x38);
    f0 = *(f64*)(r3 + tmp);
    r4 = r4 + 0x40;
    r8 = r8 + 0x8;
    f4 = f3 * f2 + f4;
    f4 = f1 * f0 + f4;
    if (--ctr != 0) goto L_800CCD88;
L_800CCE50:
    tmp = r30 + 0x1;
    r4 = r8 << 3;
    tmp = tmp - r8;
    r3 = (u32)sp + 0x198;
    r4 = r21 + r4;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r8 > (s32)r30) goto L_800CCE90;
L_800CCE70:
    tmp = r7 - r8;
    f1 = *(f64*)((u8*)r4 + 0x0);
    tmp = tmp << 3;
    r4 = r4 + 0x8;
    f0 = *(f64*)(r3 + tmp);
    r8 = r8 + 0x1;
    f4 = f1 * f0 + f4;
    if (--ctr != 0) goto L_800CCE70;
L_800CCE90:
    *(f64*)((u8*)r5 + 0x0) = f4;
    r5 = r5 + 0x8;
    r9 = r9 + 0x1;
L_800CCE9C:
    if ((s32)r9 <= (s32)r6) goto L_800CCD20;
    r31 = r31 + r10;
    goto L_800CC9C0;
L_800CCEAC:
    f0 = *(f64*)lbl_8047C828;
    if (f0 != f25) goto L_800CCEEC;
    r3 = (u32)sp + 0x8;
    tmp = r31 << 2;
    r3 = r3 + tmp;
    goto L_800CCEDC;
L_800CCED0:
L_800CCEDC:
    tmp = *(u32*)((u8*)r3 + 0x0);
    if ((s32)tmp == 0) goto L_800CCED0;
    goto L_800CCF8C;
L_800CCEEC:
    f1 = f25;
    r3 = -r26;
    fn_800CDE88();
    f3 = *(f64*)lbl_8047C838;
    /* cror eq, gt, eq */;
    if (f1 != f3) goto L_800CCF74;
    f0 = *(f64*)lbl_8047C830;
    tmp = 0x43300000;
    r5 = r31 << 2;
    *(u32*)(sp + 0x240) = tmp;
    f0 = f0 * f1;
    r31 = r31 + 0x1;
    f2 = *(f64*)lbl_8047C860;
    r4 = (u32)sp + 0x8;
    tmp = r31 << 2;
    r26 = r26 + 0x18;
    f0 = (f64)(s32)f0;
    f0 = f0 - f2;
    f1 = -(f3 * f0 - f1);
    f0 = (f64)(s32)f0;
    f1 = (f64)(s32)f1;
    *(u32*)(r4 + r5) = r6;
    *(u32*)(r4 + tmp) = r3;
    goto L_800CCF8C;
L_800CCF74:
    f0 = (f64)(s32)f1;
    tmp = r31 << 2;
    r3 = (u32)sp + 0x8;
    *(u32*)(r3 + tmp) = r4;
L_800CCF8C:
    f1 = *(f64*)lbl_8047C858;
    r3 = r26;
    fn_800CDE88();
    tmp = r31 << 2;
    r6 = (u32)sp + 0x8;
    r8 = r31 << 3;
    r7 = (u32)sp + 0x58;
    f5 = *(f64*)lbl_8047C860;
    f0 = *(f64*)lbl_8047C830;
    r6 = r6 + tmp;
    r7 = r7 + r8;
    r3 = r31 + 0x1;
    r4 = 0x43300000;
    if ((s32)r31 < 0) goto L_800CD0AC;
    /* srwi. tmp, r3, 2 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 0) goto L_800CD078;
L_800CCFD4:
    r5 = *(u32*)((u8*)r6 + 0x0);
    r5 = *(u32*)((u8*)r6 + (-4));
    *(u32*)(sp + 0x254) = tmp;
    r5 = *(u32*)((u8*)r6 + (-8));
    *(u32*)(sp + 0x254) = tmp;
    f3 = f4 - f5;
    r5 = *(u32*)((u8*)r6 + (-12));
    f2 = f1 * f3;
    f1 = f1 * f0;
    *(u32*)(sp + 0x254) = tmp;
    f3 = f4 - f5;
    *(f64*)((u8*)r7 + 0x0) = f2;
    f2 = f1 * f3;
    f1 = f1 * f0;
    *(u32*)(sp + 0x254) = tmp;
    f3 = f4 - f5;
    *(f64*)((u8*)r7 + (-8)) = f2;
    f2 = f1 * f3;
    f1 = f1 * f0;
    f3 = f4 - f5;
    *(f64*)((u8*)r7 + (-16)) = f2;
    f2 = f1 * f3;
    f1 = f1 * f0;
    *(f64*)((u8*)r7 + (-24)) = f2;
    if (--ctr != 0) goto L_800CCFD4;
    r3 = r3 & 0x3;
    if ((s32)r31 == 0) goto L_800CD0AC;
L_800CD078:
    ctr_fn = (void(*)(void))r3;
L_800CD07C:
    r5 = *(u32*)((u8*)r6 + 0x0);
    *(u32*)(sp + 0x254) = tmp;
    f3 = f4 - f5;
    f2 = f1 * f3;
    f1 = f1 * f0;
    *(f64*)((u8*)r7 + 0x0) = f2;
    if (--ctr != 0) goto L_800CD07C;
L_800CD0AC:
    r3 = (u32)lbl_80270210;
    tmp = r31 + 0x1;
    r9 = r31;
    r4 = (u32)sp + 0x58;
    r5 = (u32)lbl_80270210;
    r3 = (u32)sp + 0xf8;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 < 0) goto L_800CD120;
L_800CD0D0:
    f2 = *(f64*)lbl_8047C828;
    r6 = r5;
    r7 = r31 - r9;
    r10 = 0x0;
    goto L_800CD100;
L_800CD0E4:
    tmp = r9 + r10;
    f1 = *(f64*)((u8*)r6 + 0x0);
    tmp = tmp << 3;
    r6 = r6 + 0x8;
    f0 = *(f64*)(r4 + tmp);
    r10 = r10 + 0x1;
    f2 = f1 * f0 + f2;
L_800CD100:
    if ((s32)r10 > (s32)r28) goto L_800CD110;
    if ((s32)r10 <= (s32)r7) goto L_800CD0E4;
L_800CD110:
    tmp = r7 << 3;
    *(f64*)(r3 + tmp) = f2;
    if (--ctr != 0) goto L_800CD0D0;
L_800CD120:
    if ((s32)r23 == 3) goto L_800CD348;
    if ((s32)r23 >= 3) goto L_800CD588;
    if ((s32)r23 == 0) goto L_800CD13C;
    if ((s32)r23 >= 0) goto L_800CD1DC;
    goto L_800CD588;
L_800CD13C:
    r4 = (u32)sp + 0xf8;
    f1 = *(f64*)lbl_8047C828;
    r4 = r4 + r8;
    r3 = r31 + 0x1;
    if ((s32)r31 < 0) goto L_800CD1C4;
    /* srwi. tmp, r3, 3 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 0) goto L_800CD1B0;
L_800CD160:
    f0 = *(f64*)((u8*)r4 + 0x0);
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-8));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-16));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-24));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-32));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-40));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-48));
    f1 = f1 + f0;
    f0 = *(f64*)((u8*)r4 + (-56));
    f1 = f1 + f0;
    if (--ctr != 0) goto L_800CD160;
    r3 = r3 & 0x7;
    if ((s32)r31 == 0) goto L_800CD1C4;
L_800CD1B0:
    ctr_fn = (void(*)(void))r3;
L_800CD1B4:
    f0 = *(f64*)((u8*)r4 + 0x0);
    f1 = f1 + f0;
    if (--ctr != 0) goto L_800CD1B4;
L_800CD1C4:
    if ((s32)r25 != 0) goto L_800CD1D0;
    goto L_800CD1D4;
L_800CD1D0:
    f1 = -f1;
L_800CD1D4:
    *(f64*)((u8*)r22 + 0x0) = f1;
    goto L_800CD588;
L_800CD1DC:
    r4 = (u32)sp + 0xf8;
    f2 = *(f64*)lbl_8047C828;
    r4 = r4 + r8;
    r3 = r31 + 0x1;
    if ((s32)r31 < 0) goto L_800CD264;
    /* srwi. tmp, r3, 3 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 0) goto L_800CD250;
L_800CD200:
    f0 = *(f64*)((u8*)r4 + 0x0);
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-8));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-16));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-24));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-32));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-40));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-48));
    f2 = f2 + f0;
    f0 = *(f64*)((u8*)r4 + (-56));
    f2 = f2 + f0;
    if (--ctr != 0) goto L_800CD200;
    r3 = r3 & 0x7;
    if ((s32)r31 == 0) goto L_800CD264;
L_800CD250:
    ctr_fn = (void(*)(void))r3;
L_800CD254:
    f0 = *(f64*)((u8*)r4 + 0x0);
    f2 = f2 + f0;
    if (--ctr != 0) goto L_800CD254;
L_800CD264:
    if ((s32)r25 != 0) goto L_800CD274;
    f1 = f2;
    goto L_800CD278;
L_800CD274:
    f1 = -f2;
L_800CD278:
    *(f64*)((u8*)r22 + 0x0) = f1;
    r5 = 0x1;
    f6 = f0 - f2;
    if ((s32)r31 < 1) goto L_800CD330;
    if ((s32)r31 <= 8) goto L_800CD300;
    tmp = r3 + 0x7;
    r4 = (u32)sp + 0x100;
    tmp = (u32)tmp >> 3;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r3 < 1) goto L_800CD300;
L_800CD2B4:
    f1 = *(f64*)((u8*)r4 + 0x0);
    r5 = r5 + 0x8;
    f0 = *(f64*)((u8*)r4 + 0x8);
    f6 = f6 + f1;
    f5 = *(f64*)((u8*)r4 + 0x10);
    f4 = *(f64*)((u8*)r4 + 0x18);
    f3 = *(f64*)((u8*)r4 + 0x20);
    f6 = f6 + f0;
    f2 = *(f64*)((u8*)r4 + 0x28);
    f1 = *(f64*)((u8*)r4 + 0x30);
    f0 = *(f64*)((u8*)r4 + 0x38);
    r4 = r4 + 0x40;
    f6 = f6 + f5;
    f6 = f6 + f4;
    f6 = f6 + f3;
    f6 = f6 + f2;
    f6 = f6 + f1;
    f6 = f6 + f0;
    if (--ctr != 0) goto L_800CD2B4;
L_800CD300:
    tmp = r31 + 0x1;
    r3 = r5 << 3;
    r4 = (u32)sp + 0xf8;
    tmp = tmp - r5;
    r4 = r4 + r3;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r5 > (s32)r31) goto L_800CD330;
L_800CD320:
    f0 = *(f64*)((u8*)r4 + 0x0);
    r4 = r4 + 0x8;
    f6 = f6 + f0;
    if (--ctr != 0) goto L_800CD320;
L_800CD330:
    if ((s32)r25 != 0) goto L_800CD33C;
    goto L_800CD340;
L_800CD33C:
    f6 = -f6;
L_800CD340:
    *(f64*)((u8*)r22 + 0x8) = f6;
    goto L_800CD588;
L_800CD348:
    r5 = (u32)sp + 0xf8;
    r5 = r5 + r8;
    r3 = r31;
    r4 = r5;
    if ((s32)r31 <= 0) goto L_800CD40C;
    /* srwi. tmp, r31, 2 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 0) goto L_800CD3E8;
L_800CD36C:
    f0 = *(f64*)((u8*)r4 + (-8));
    f1 = *(f64*)((u8*)r4 + 0x0);
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + 0x0) = f0;
    *(f64*)((u8*)r4 + (-8)) = f2;
    f0 = *(f64*)((u8*)r4 + (-16));
    f1 = *(f64*)((u8*)r4 + (-8));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-8)) = f0;
    *(f64*)((u8*)r4 + (-16)) = f2;
    f0 = *(f64*)((u8*)r4 + (-24));
    f1 = *(f64*)((u8*)r4 + (-16));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-16)) = f0;
    *(f64*)((u8*)r4 + (-24)) = f2;
    f0 = *(f64*)((u8*)r4 + (-32));
    f1 = *(f64*)((u8*)r4 + (-24));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-24)) = f0;
    /* stfdu f2, -0x20(r4) */;
    if (--ctr != 0) goto L_800CD36C;
    r3 = r3 & 0x3;
    if ((s32)r31 == 0) goto L_800CD40C;
L_800CD3E8:
    ctr_fn = (void(*)(void))r3;
L_800CD3EC:
    f0 = *(f64*)((u8*)r4 + (-8));
    f1 = *(f64*)((u8*)r4 + 0x0);
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + 0x0) = f0;
    /* stfdu f2, -0x8(r4) */;
    if (--ctr != 0) goto L_800CD3EC;
L_800CD40C:
    r4 = r5;
    if ((s32)r31 <= 1) goto L_800CD4C8;
    /* srwi. tmp, r3, 2 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 1) goto L_800CD4A4;
L_800CD428:
    f0 = *(f64*)((u8*)r4 + (-8));
    f1 = *(f64*)((u8*)r4 + 0x0);
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + 0x0) = f0;
    *(f64*)((u8*)r4 + (-8)) = f2;
    f0 = *(f64*)((u8*)r4 + (-16));
    f1 = *(f64*)((u8*)r4 + (-8));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-8)) = f0;
    *(f64*)((u8*)r4 + (-16)) = f2;
    f0 = *(f64*)((u8*)r4 + (-24));
    f1 = *(f64*)((u8*)r4 + (-16));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-16)) = f0;
    *(f64*)((u8*)r4 + (-24)) = f2;
    f0 = *(f64*)((u8*)r4 + (-32));
    f1 = *(f64*)((u8*)r4 + (-24));
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + (-24)) = f0;
    /* stfdu f2, -0x20(r4) */;
    if (--ctr != 0) goto L_800CD428;
    r3 = r3 & 0x3;
    if ((s32)r31 == 1) goto L_800CD4C8;
L_800CD4A4:
    ctr_fn = (void(*)(void))r3;
L_800CD4A8:
    f0 = *(f64*)((u8*)r4 + (-8));
    f1 = *(f64*)((u8*)r4 + 0x0);
    f2 = f0 + f1;
    f0 = f0 - f2;
    f0 = f1 + f0;
    *(f64*)((u8*)r4 + 0x0) = f0;
    /* stfdu f2, -0x8(r4) */;
    if (--ctr != 0) goto L_800CD4A8;
L_800CD4C8:
    f3 = *(f64*)lbl_8047C828;
    if ((s32)r31 < 2) goto L_800CD548;
    /* srwi. tmp, r3, 3 */;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 == 2) goto L_800CD534;
L_800CD4E4:
    f0 = *(f64*)((u8*)r5 + 0x0);
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-8));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-16));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-24));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-32));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-40));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-48));
    f3 = f3 + f0;
    f0 = *(f64*)((u8*)r5 + (-56));
    f3 = f3 + f0;
    if (--ctr != 0) goto L_800CD4E4;
    r3 = r3 & 0x7;
    if ((s32)r31 == 2) goto L_800CD548;
L_800CD534:
    ctr_fn = (void(*)(void))r3;
L_800CD538:
    f0 = *(f64*)((u8*)r5 + 0x0);
    f3 = f3 + f0;
    if (--ctr != 0) goto L_800CD538;
L_800CD548:
    if ((s32)r25 != 0) goto L_800CD568;
    *(f64*)((u8*)r22 + 0x0) = f1;
    *(f64*)((u8*)r22 + 0x8) = f0;
    *(f64*)((u8*)r22 + 0x10) = f3;
    goto L_800CD588;
L_800CD568:
    f0 = -f3;
    f2 = -f2;
    f1 = -f1;
    *(f64*)((u8*)r22 + 0x0) = f2;
    *(f64*)((u8*)r22 + 0x8) = f1;
    *(f64*)((u8*)r22 + 0x10) = f0;
L_800CD588:
    r3 = r27 & 0x7;
    r11 = (u32)sp + 0x2d0;
    return;
}

/* fn_800CD5A8 - 0x800CD5A8 | size: 0xA0 */
void fn_800CD5A8(void) {
    extern u8 lbl_8047C868[];
    extern u8 lbl_8047C870[];
    extern u8 lbl_8047C878[];
    extern u8 lbl_8047C880[];
    extern u8 lbl_8047C888[];
    extern u8 lbl_8047C890[];
    extern u8 lbl_8047C898[];
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;

    tmp = 0x3E400000;
    r4 = r4 & 0x7FFFFFFF;
    if ((s32)r4 >= (s32)tmp) goto L_800CD5DC;
    f0 = (f64)(s32)f1;
    if ((s32)tmp != 0) goto L_800CD5DC;
    goto L_800CD640;
L_800CD5DC:
    f5 = *(f64*)lbl_8047C888;
    f7 = f6 * f6;
    f4 = *(f64*)lbl_8047C880;
    f3 = *(f64*)lbl_8047C878;
    f1 = *(f64*)lbl_8047C870;
    f0 = *(f64*)lbl_8047C868;
    f4 = f5 * f7 + f4;
    f5 = f7 * f6;
    f3 = f7 * f4 + f3;
    f1 = f7 * f3 + f1;
    f1 = f7 * f1 + f0;
    if ((s32)r3 != 0) goto L_800CD624;
    f0 = *(f64*)lbl_8047C890;
    f0 = f7 * f1 + f0;
    f1 = f5 * f0 + f6;
    goto L_800CD640;
L_800CD624:
    f0 = f5 * f1;
    f1 = *(f64*)lbl_8047C898;
    f3 = *(f64*)lbl_8047C890;
    f0 = f1 * f2 - f0;
    f0 = f7 * f0 - f2;
    f0 = -(f3 * f5 - f0);
    f1 = f6 - f0;
L_800CD640:
    return;
}

/* fn_800CD648 - 0x800CD648 | size: 0x214 */
void fn_800CD648(void) {
    extern u8 lbl_80270250[];
    extern u8 lbl_8047C8A0[];
    extern u8 lbl_8047C8A8[];
    extern u8 lbl_8047C8B0[];
    extern u8 lbl_8047C8B8[];
    extern u8 lbl_8047C8C0[];
    extern u8 lbl_8047C8C8[];
    extern u8 lbl_8047C8D0[];
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;
    f32 f12 = 0.0f;
    f32 f13 = 0.0f;
    f32 f31 = 0.0f;

    tmp = 0x3E300000;
    r6 = r7 & 0x7FFFFFFF;
    if ((s32)r6 >= (s32)tmp) goto L_800CD6BC;
    f0 = (f64)(s32)f1;
    if ((s32)tmp != 0) goto L_800CD6BC;
    r4 = r3 + 0x1;
    tmp = r6 | tmp;
    /* or. tmp, r4, tmp */;
    if ((s32)tmp != 0) goto L_800CD6A4;
    /* fabs */ f1 = (f1 < 0) ? -f1 : f1;
    f0 = *(f64*)lbl_8047C8A0;
    f1 = f0 / f1;
    goto L_800CD84C;
L_800CD6A4:
    if ((s32)r3 != 1) goto L_800CD6B0;
    goto L_800CD84C;
L_800CD6B0:
    f0 = *(f64*)lbl_8047C8A8;
    f1 = f0 / f1;
    goto L_800CD84C;
L_800CD6BC:
    r4 = 0x3FE60000;
    if ((s32)r6 < (s32)tmp) goto L_800CD708;
    if ((s32)r7 >= 0) goto L_800CD6E4;
    f2 = -f2;
    f0 = -f0;
L_800CD6E4:
    f0 = *(f64*)lbl_8047C8B8;
    f3 = *(f64*)lbl_8047C8B0;
    f0 = f0 - f2;
    f2 = *(f64*)lbl_8047C8C0;
    f1 = f3 - f1;
    f0 = f1 + f0;
L_800CD708:
    r4 = (u32)lbl_80270250;
    r5 = (u32)lbl_80270250;
    r4 = 0x3FE60000;
    f13 = f0 * f0;
    f5 = *(f64*)((u8*)r5 + 0x60);
    f4 = *(f64*)((u8*)r5 + 0x50);
    f9 = *(f64*)((u8*)r5 + 0x58);
    f31 = f13 * f13;
    f8 = *(f64*)((u8*)r5 + 0x48);
    f3 = *(f64*)((u8*)r5 + 0x40);
    f11 = *(f64*)((u8*)r5 + 0x38);
    f1 = f13 * f0;
    f6 = *(f64*)((u8*)r5 + 0x30);
    f7 = f31 * f5 + f4;
    f10 = *(f64*)((u8*)r5 + 0x28);
    f5 = *(f64*)((u8*)r5 + 0x20);
    f12 = f31 * f9 + f8;
    f9 = *(f64*)((u8*)r5 + 0x18);
    f4 = *(f64*)((u8*)r5 + 0x10);
    f7 = f31 * f7 + f3;
    f8 = *(f64*)((u8*)r5 + 0x8);
    f3 = *(f64*)((u8*)r5 + 0x0);
    f11 = f31 * f12 + f11;
    f6 = f31 * f7 + f6;
    f7 = f31 * f11 + f10;
    f5 = f31 * f6 + f5;
    f6 = f31 * f7 + f9;
    f4 = f31 * f5 + f4;
    f5 = f31 * f6 + f8;
    f4 = f13 * f4;
    f4 = f5 + f4;
    f4 = f1 * f4 + f2;
    f6 = f13 * f4 + f2;
    f6 = f3 * f1 + f6;
    f1 = f0 + f6;
    if ((s32)r6 < (s32)tmp) goto L_800CD804;
    r4 = 0x43300000;
    *(u32*)(sp + 0x24) = tmp;
    tmp = ((r7 << 2) | ((u32)r7 >> 30)) & 0x00000002;
    tmp = 0x1 - tmp;
    f5 = *(f64*)lbl_8047C8D0;
    f2 = f1 * f1;
    f3 = *(f64*)lbl_8047C8C8;
    *(u32*)(sp + 0x2C) = tmp;
    f7 = f4 - f5;
    f1 = f1 + f7;
    f4 = f4 - f5;
    f1 = f2 / f1;
    f1 = f1 - f6;
    f0 = f0 - f1;
    f0 = -(f3 * f0 - f7);
    f1 = f4 * f0;
    goto L_800CD84C;
L_800CD804:
    if ((s32)r3 != 1) goto L_800CD810;
    goto L_800CD84C;
L_800CD810:
    f2 = *(f64*)lbl_8047C8A8;
    tmp = 0x0;
    f4 = f2 / f1;
    f1 = *(f64*)lbl_8047C8A0;
    *(u32*)(sp + 0x1C) = tmp;
    f0 = f2 - f0;
    *(u32*)(sp + 0x14) = tmp;
    f0 = f6 - f0;
    f1 = f3 * f2 + f1;
    f0 = f3 * f0 + f1;
    f1 = f4 * f0 + f3;
L_800CD84C:
    return;
}

/* fn_800CD85C - 0x800CD85C | size: 0x218 */
void fn_800CD85C(void) {
    extern u8 lbl_802702B8[];
    extern u8 lbl_8047C8D8[];
    extern u8 lbl_8047C8E0[];
    extern u8 lbl_8047C8E8[];
    extern u8 lbl_8047C8F0[];
    extern u8 lbl_8047C8F8[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;
    f32 f9 = 0.0f;
    f32 f10 = 0.0f;
    f32 f11 = 0.0f;

    r3 = (u32)lbl_802702B8;
    tmp = 0x44100000;
    r5 = (u32)lbl_802702B8;
    r4 = r6 & 0x7FFFFFFF;
    if ((s32)r4 < (s32)tmp) goto L_800CD8EC;
    tmp = 0x7FF00000;
    if ((s32)r4 > (s32)tmp) goto L_800CD8A4;
    /* subis tmp, r4, 0x7ff0 */;
    if (tmp != 0) goto L_800CD8B0;
    if ((s32)tmp == 0) goto L_800CD8B0;
L_800CD8A4:
    f1 = f0 + f0;
    goto L_800CDA6C;
L_800CD8B0:
    if ((s32)r6 <= 0) goto L_800CD8D0;
    r4 = r5 + 0x0;
    r3 = r5 + 0x20;
    f1 = *(f64*)((u8*)r4 + 0x18);
    f0 = *(f64*)((u8*)r3 + 0x18);
    f1 = f1 + f0;
    goto L_800CDA6C;
L_800CD8D0:
    r4 = r5 + 0x0;
    r3 = r5 + 0x20;
    f1 = *(f64*)((u8*)r4 + 0x18);
    f0 = *(f64*)((u8*)r3 + 0x18);
    f1 = -f1;
    f1 = f1 - f0;
    goto L_800CDA6C;
L_800CD8EC:
    tmp = 0x3FDC0000;
    if ((s32)r4 >= (s32)tmp) goto L_800CD924;
    tmp = 0x3E200000;
    if ((s32)r4 >= (s32)tmp) goto L_800CD91C;
    f2 = *(f64*)lbl_8047C8D8;
    f0 = *(f64*)lbl_8047C8E0;
    f2 = f2 + f1;
    if (f2 <= f0) goto L_800CD91C;
    goto L_800CDA6C;
L_800CD91C:
    tmp = -0x1;
    goto L_800CD9C0;
L_800CD924:
    /* fabs */ f3 = (f1 < 0) ? -f1 : f1;
    tmp = 0x3FF30000;
    if ((s32)r4 >= (s32)tmp) goto L_800CD980;
    tmp = 0x3FE60000;
    if ((s32)r4 >= (s32)tmp) goto L_800CD964;
    f2 = *(f64*)lbl_8047C8E8;
    tmp = 0x0;
    f1 = *(f64*)lbl_8047C8E0;
    f0 = f2 + f3;
    f1 = f2 * f3 - f1;
    f0 = f1 / f0;
    goto L_800CD9C0;
L_800CD964:
    f0 = *(f64*)lbl_8047C8E0;
    tmp = 0x1;
    f1 = f3 - f0;
    f0 = f0 + f3;
    f0 = f1 / f0;
    goto L_800CD9C0;
L_800CD980:
    r3 = 0x40040000;
    tmp = r3 + -0x8000;
    if ((s32)r4 >= (s32)tmp) goto L_800CD9B0;
    f2 = *(f64*)lbl_8047C8F0;
    tmp = 0x2;
    f0 = *(f64*)lbl_8047C8E0;
    f1 = f3 - f2;
    f0 = f2 * f3 + f0;
    f0 = f1 / f0;
    goto L_800CD9C0;
L_800CD9B0:
    f0 = *(f64*)lbl_8047C8F8;
    tmp = 0x3;
    f0 = f0 / f3;
L_800CD9C0:
    r3 = r5 + 0x40;
    f4 = *(f64*)((u8*)r3 + 0x50);
    f11 = f9 * f9;
    f1 = *(f64*)((u8*)r3 + 0x40);
    f7 = *(f64*)((u8*)r3 + 0x30);
    f3 = *(f64*)((u8*)r3 + 0x48);
    f0 = *(f64*)((u8*)r3 + 0x38);
    f10 = f11 * f11;
    f6 = *(f64*)((u8*)r3 + 0x20);
    f2 = *(f64*)((u8*)r3 + 0x28);
    f5 = *(f64*)((u8*)r3 + 0x10);
    f8 = f10 * f4 + f1;
    f1 = *(f64*)((u8*)r3 + 0x18);
    f4 = *(f64*)((u8*)r5 + 0x40);
    f3 = f10 * f3 + f0;
    f0 = *(f64*)((u8*)r3 + 0x8);
    f7 = f10 * f8 + f7;
    f2 = f10 * f3 + f2;
    f3 = f10 * f7 + f6;
    f1 = f10 * f2 + f1;
    f2 = f10 * f3 + f5;
    f0 = f10 * f1 + f0;
    f1 = f10 * f2 + f4;
    f2 = f10 * f0;
    f0 = f11 * f1;
    if ((s32)tmp >= 0) goto L_800CDA3C;
    f0 = f0 + f2;
    f1 = -(f9 * f0 - f9);
    goto L_800CDA6C;
L_800CDA3C:
    tmp = tmp << 3;
    r3 = r5 + 0x20;
    f1 = f0 + f2;
    f0 = *(f64*)(r3 + tmp);
    r3 = r5 + 0x0;
    f2 = *(f64*)(r3 + tmp);
    f0 = f9 * f1 - f0;
    f0 = f0 - f9;
    f1 = f2 - f0;
    if ((s32)r6 >= 0) goto L_800CDA6C;
    f1 = -f1;
L_800CDA6C:
    return;
}

/* fn_800CDA74 - 0x800CDA74 | size: 0x144 */
void fn_800CDA74(void) {
    extern u8 lbl_8047C900[];
    extern u8 lbl_8047C908[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* extrwi r3, r5, 11, 1 */;
    if ((s32)r7 >= 0x14) goto L_800CDB24;
    if ((s32)r7 >= 0) goto L_800CDAD8;
    f2 = *(f64*)lbl_8047C900;
    f0 = *(f64*)lbl_8047C908;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDBA4;
    if ((s32)r5 >= 0) goto L_800CDAC4;
    r5 = 0x80000000;
    r6 = 0x0;
    goto L_800CDBA4;
L_800CDAC4:
    /* or. tmp, r5, r6 */;
    if ((s32)r5 == 0) goto L_800CDBA4;
    r5 = 0x3FF00000;
    r6 = 0x0;
    goto L_800CDBA4;
L_800CDAD8:
    r3 = 0x100000;
    r4 = (s32)tmp >> r7;
    tmp = r5 & r4;
    /* or. tmp, r6, tmp */;
    if ((s32)r5 != 0) goto L_800CDAF4;
    goto L_800CDBB0;
L_800CDAF4:
    f2 = *(f64*)lbl_8047C900;
    f0 = *(f64*)lbl_8047C908;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDBA4;
    if ((s32)r5 <= 0) goto L_800CDB18;
    tmp = (s32)r3 >> r7;
    r5 = r5 + tmp;
L_800CDB18:
    r5 = r5 & ~r4;
    r6 = 0x0;
    goto L_800CDBA4;
L_800CDB24:
    if ((s32)r7 <= 0x33) goto L_800CDB3C;
    if ((s32)r7 != 0x400) goto L_800CDBB0;
    f1 = f1 + f1;
    goto L_800CDBB0;
L_800CDB3C:
    r3 = -0x1;
    r4 = (u32)r3 >> tmp;
    /* and. tmp, r6, r4 */;
    if ((s32)r7 != 0x400) goto L_800CDB54;
    goto L_800CDBB0;
L_800CDB54:
    f2 = *(f64*)lbl_8047C900;
    f0 = *(f64*)lbl_8047C908;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDBA4;
    if ((s32)r5 <= 0) goto L_800CDBA0;
    if ((s32)r7 != 0x14) goto L_800CDB80;
    r5 = r5 + 0x1;
    goto L_800CDBA0;
L_800CDB80:
    tmp = 0x34 - r7;
    r3 = 0x1;
    tmp = r3 << tmp;
    tmp = r6 + tmp;
    if (tmp >= r6) goto L_800CDB9C;
    r5 = r5 + 0x1;
L_800CDB9C:
    r6 = tmp;
L_800CDBA0:
    r6 = r6 & ~r4;
L_800CDBA4:
L_800CDBB0:
    return;
}

/* fn_800CDBB8 - 0x800CDBB8 | size: 0x28 */
void fn_800CDBB8(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    tmp = (tmp & ~0x7FFFFFFF) | (((r3 << 0) | ((u32)r3 >> 32)) & 0x7FFFFFFF);
    *(u32*)(sp + 0x8) = tmp;
    return;
}

/* fn_800CDBE0 - 0x800CDBE0 | size: 0xD4 */
void fn_800CDBE0(void) {
    extern u8 lbl_8047C910[];
    extern void fn_800CC2C0();
    extern void fn_800CC660();
    extern void fn_800CD5A8();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = 0x3FE90000;
    tmp = r3 + 0x21fb;
    r3 = r3 & 0x7FFFFFFF;
    if ((s32)r3 > (s32)tmp) goto L_800CDC14;
    f2 = *(f64*)lbl_8047C910;
    fn_800CC660();
    goto L_800CDCA4;
L_800CDC14:
    tmp = 0x7FF00000;
    if ((s32)r3 < (s32)tmp) goto L_800CDC28;
    f1 = f1 - f1;
    goto L_800CDCA4;
L_800CDC28:
    r3 = (u32)sp + 0x10;
    fn_800CC2C0();
    tmp = r3 & 0x3;
    if ((s32)tmp == 1) goto L_800CDC68;
    if ((s32)tmp >= 1) goto L_800CDC4C;
    if ((s32)tmp >= 0) goto L_800CDC58;
    goto L_800CDC94;
L_800CDC4C:
    if ((s32)tmp >= 3) goto L_800CDC94;
    goto L_800CDC80;
L_800CDC58:
    fn_800CC660();
    goto L_800CDCA4;
L_800CDC68:
    r3 = 0x1;
    fn_800CD5A8();
    f1 = -f1;
    goto L_800CDCA4;
L_800CDC80:
    fn_800CC660();
    f1 = -f1;
    goto L_800CDCA4;
L_800CDC94:
    r3 = 0x1;
    fn_800CD5A8();
L_800CDCA4:
    return;
}

/* fn_800CDCB4 - 0x800CDCB4 | size: 0x148 */
void fn_800CDCB4(void) {
    extern u8 lbl_8047C918[];
    extern u8 lbl_8047C920[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    /* extrwi r3, r5, 11, 1 */;
    if ((s32)r7 >= 0x14) goto L_800CDD68;
    if ((s32)r7 >= 0) goto L_800CDD1C;
    f2 = *(f64*)lbl_8047C918;
    f0 = *(f64*)lbl_8047C920;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDDE8;
    if ((s32)r5 < 0) goto L_800CDD04;
    r6 = 0x0;
    r5 = 0x0;
    goto L_800CDDE8;
L_800CDD04:
    tmp = r5 & 0x7FFFFFFF;
    /* or. tmp, tmp, r6 */;
    if ((s32)r5 == 0) goto L_800CDDE8;
    r5 = 0xBFF00000;
    r6 = 0x0;
    goto L_800CDDE8;
L_800CDD1C:
    r3 = 0x100000;
    r4 = (s32)tmp >> r7;
    tmp = r5 & r4;
    /* or. tmp, r6, tmp */;
    if ((s32)r5 != 0) goto L_800CDD38;
    goto L_800CDDF4;
L_800CDD38:
    f2 = *(f64*)lbl_8047C918;
    f0 = *(f64*)lbl_8047C920;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDDE8;
    if ((s32)r5 >= 0) goto L_800CDD5C;
    tmp = (s32)r3 >> r7;
    r5 = r5 + tmp;
L_800CDD5C:
    r5 = r5 & ~r4;
    r6 = 0x0;
    goto L_800CDDE8;
L_800CDD68:
    if ((s32)r7 <= 0x33) goto L_800CDD80;
    if ((s32)r7 != 0x400) goto L_800CDDF4;
    f1 = f1 + f1;
    goto L_800CDDF4;
L_800CDD80:
    r3 = -0x1;
    r4 = (u32)r3 >> tmp;
    /* and. tmp, r6, r4 */;
    if ((s32)r7 != 0x400) goto L_800CDD98;
    goto L_800CDDF4;
L_800CDD98:
    f2 = *(f64*)lbl_8047C918;
    f0 = *(f64*)lbl_8047C920;
    f1 = f2 + f1;
    if (f1 <= f0) goto L_800CDDE8;
    if ((s32)r5 >= 0) goto L_800CDDE4;
    if ((s32)r7 != 0x14) goto L_800CDDC4;
    r5 = r5 + 0x1;
    goto L_800CDDE4;
L_800CDDC4:
    tmp = 0x34 - r7;
    r3 = 0x1;
    tmp = r3 << tmp;
    tmp = r6 + tmp;
    if (tmp >= r6) goto L_800CDDE0;
    r5 = r5 + 0x1;
L_800CDDE0:
    r6 = tmp;
L_800CDDE4:
    r6 = r6 & ~r4;
L_800CDDE8:
L_800CDDF4:
    return;
}

/* fn_800CDDFC - 0x800CDDFC | size: 0x8C */
void fn_800CDDFC(void) {
    extern u8 lbl_8047C928[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r4 = 0x0;
    tmp = 0x7FF00000;
    *(u32*)((u8*)r3 + 0x0) = r4;
    r4 = r5 & 0x7FFFFFFF;
    if ((s32)r4 >= (s32)tmp) goto L_800CDE2C;
    /* or. tmp, r4, r6 */;
    if ((s32)r4 != (s32)tmp) goto L_800CDE34;
L_800CDE2C:
    goto L_800CDE80;
L_800CDE34:
    tmp = 0x100000;
    if ((s32)r4 >= (s32)tmp) goto L_800CDE5C;
    f0 = *(f64*)lbl_8047C928;
    tmp = -0x36;
    *(u32*)((u8*)r3 + 0x0) = tmp;
    f0 = f1 * f0;
    r4 = r5 & 0x7FFFFFFF;
L_800CDE5C:
    tmp = r5 & 0x800FFFFF;
    r5 = *(u32*)((u8*)r3 + 0x0);
    r4 = (s32)r4 >> 20;
    tmp = tmp | (0x3fe0 << 16);
    *(u32*)(sp + 0x8) = tmp;
    r4 = r4 + r5;
    *(u32*)((u8*)r3 + 0x0) = tmp;
L_800CDE80:
    return;
}

/* fn_800CDE88 - 0x800CDE88 | size: 0x1C4 */
void fn_800CDE88(void) {
    extern u8 lbl_8047C930[];
    extern u8 lbl_8047C938[];
    extern u8 lbl_8047C940[];
    extern u8 lbl_8047C948[];
    extern u8 lbl_8047C950[];
    extern void fn_800CDBB8();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    tmp = 0x7FF00000;
    r4 = r5 & 0x7FF00000;
    if ((s32)r4 == (s32)tmp) goto L_800CDEC0;
    if ((s32)r4 >= (s32)tmp) goto L_800CDF08;
    if ((s32)r4 == 0) goto L_800CDEE4;
    goto L_800CDF08;
L_800CDEC0:
    tmp = r5 & 0xFFFFF;
    if ((s32)r4 != 0) goto L_800CDED4;
    if ((s32)tmp == 0) goto L_800CDEDC;
L_800CDED4:
    tmp = 0x1;
    goto L_800CDF0C;
L_800CDEDC:
    tmp = 0x2;
    goto L_800CDF0C;
L_800CDEE4:
    tmp = r5 & 0xFFFFF;
    if ((s32)tmp != 0) goto L_800CDEF8;
    if ((s32)tmp == 0) goto L_800CDF00;
L_800CDEF8:
    tmp = 0x5;
    goto L_800CDF0C;
L_800CDF00:
    tmp = 0x3;
    goto L_800CDF0C;
L_800CDF08:
    tmp = 0x4;
L_800CDF0C:
    if ((s32)tmp <= 2) goto L_800CE03C;
    f0 = *(f64*)lbl_8047C930;
    if (f0 != f1) goto L_800CDF24;
    goto L_800CE03C;
L_800CDF24:
    /* extrwi. r4, r5, 11, 1 */;
    if (f0 != f1) goto L_800CDF7C;
    tmp = r5 & 0x7FFFFFFF;
    /* or. tmp, r6, tmp */;
    if (f0 != f1) goto L_800CDF44;
    goto L_800CE03C;
L_800CDF44:
    r4 = 0xFFFF0000;
    f0 = *(f64*)lbl_8047C938;
    tmp = r4 + 0x3cb0;
    f1 = f1 * f0;
    /* extrwi r4, r5, 11, 1 */;
    if ((s32)r3 >= (s32)tmp) goto L_800CDF7C;
    f0 = *(f64*)lbl_8047C940;
    f1 = f0 * f1;
    goto L_800CE03C;
L_800CDF7C:
    if ((s32)r4 != 0x7ff) goto L_800CDF90;
    f1 = f0 + f0;
    goto L_800CE03C;
L_800CDF90:
    r4 = r4 + r3;
    if ((s32)r4 <= 0x7fe) goto L_800CDFB4;
    f1 = *(f64*)lbl_8047C948;
    fn_800CDBB8();
    f0 = *(f64*)lbl_8047C948;
    f1 = f0 * f1;
    goto L_800CE03C;
L_800CDFB4:
    if ((s32)r4 <= 0) goto L_800CDFD4;
    r3 = r5 & 0x800FFFFF;
    tmp = r4 << 20;
    tmp = r3 | tmp;
    *(u32*)(sp + 0x8) = tmp;
    goto L_800CE03C;
L_800CDFD4:
    if ((s32)r4 > (s32)-0x36) goto L_800CE01C;
    r4 = 0x10000;
    if ((s32)r3 <= (s32)tmp) goto L_800CE004;
    f1 = *(f64*)lbl_8047C948;
    fn_800CDBB8();
    f0 = *(f64*)lbl_8047C948;
    f1 = f0 * f1;
    goto L_800CE03C;
L_800CE004:
    f1 = *(f64*)lbl_8047C940;
    fn_800CDBB8();
    f0 = *(f64*)lbl_8047C940;
    f1 = f0 * f1;
    goto L_800CE03C;
L_800CE01C:
    tmp = r4 + 0x36;
    r3 = r5 & 0x800FFFFF;
    tmp = tmp << 20;
    f1 = *(f64*)lbl_8047C950;
    tmp = r3 | tmp;
    *(u32*)(sp + 0x8) = tmp;
    f1 = f1 * f0;
L_800CE03C:
    return;
}

/* fn_800CE04C - 0x800CE04C | size: 0xFC */
void fn_800CE04C(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    /* extrwi r4, r5, 11, 1 */;
    if ((s32)r7 >= 0x14) goto L_800CE0D8;
    if ((s32)r7 >= 0) goto L_800CE088;
    /* clrrwi r4, r5, 31 */;
    tmp = 0x0;
    *(u32*)((u8*)r3 + 0x0) = r4;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    goto L_800CE140;
L_800CE088:
    r4 = 0x100000;
    r4 = (s32)tmp >> r7;
    tmp = r5 & r4;
    /* or. tmp, r6, tmp */;
    if ((s32)r7 != 0) goto L_800CE0BC;
    /* clrrwi r4, r5, 31 */;
    tmp = 0x0;
    *(f64*)((u8*)r3 + 0x0) = f1;
    goto L_800CE140;
L_800CE0BC:
    r4 = r5 & ~r4;
    tmp = 0x0;
    *(u32*)((u8*)r3 + 0x0) = r4;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    f0 = *(f64*)((u8*)r3 + 0x0);
    f1 = f1 - f0;
    goto L_800CE140;
L_800CE0D8:
    if ((s32)r7 <= 0x33) goto L_800CE0FC;
    /* clrrwi r4, r5, 31 */;
    tmp = 0x0;
    *(f64*)((u8*)r3 + 0x0) = f1;
    goto L_800CE140;
L_800CE0FC:
    r4 = -0x1;
    r4 = (u32)r4 >> tmp;
    /* and. tmp, r6, r4 */;
    if ((s32)r7 != 0x33) goto L_800CE12C;
    /* clrrwi r4, r5, 31 */;
    tmp = 0x0;
    *(f64*)((u8*)r3 + 0x0) = f1;
    goto L_800CE140;
L_800CE12C:
    *(u32*)((u8*)r3 + 0x0) = r5;
    tmp = r6 & ~r4;
    *(u32*)((u8*)r3 + 0x4) = tmp;
    f0 = *(f64*)((u8*)r3 + 0x0);
    f1 = f1 - f0;
L_800CE140:
    return;
}

/* fn_800CE148 - 0x800CE148 | size: 0xD8 */
void fn_800CE148(void) {
    extern u8 lbl_8047C958[];
    extern void fn_800CC2C0();
    extern void fn_800CC660();
    extern void fn_800CD5A8();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = 0x3FE90000;
    tmp = r3 + 0x21fb;
    r3 = r3 & 0x7FFFFFFF;
    if ((s32)r3 > (s32)tmp) goto L_800CE180;
    f2 = *(f64*)lbl_8047C958;
    r3 = 0x0;
    fn_800CD5A8();
    goto L_800CE210;
L_800CE180:
    tmp = 0x7FF00000;
    if ((s32)r3 < (s32)tmp) goto L_800CE194;
    f1 = f1 - f1;
    goto L_800CE210;
L_800CE194:
    r3 = (u32)sp + 0x10;
    fn_800CC2C0();
    tmp = r3 & 0x3;
    if ((s32)tmp == 1) goto L_800CE1D8;
    if ((s32)tmp >= 1) goto L_800CE1B8;
    if ((s32)tmp >= 0) goto L_800CE1C4;
    goto L_800CE200;
L_800CE1B8:
    if ((s32)tmp >= 3) goto L_800CE200;
    goto L_800CE1E8;
L_800CE1C4:
    r3 = 0x1;
    fn_800CD5A8();
    goto L_800CE210;
L_800CE1D8:
    fn_800CC660();
    goto L_800CE210;
L_800CE1E8:
    r3 = 0x1;
    fn_800CD5A8();
    f1 = -f1;
    goto L_800CE210;
L_800CE200:
    fn_800CC660();
    f1 = -f1;
L_800CE210:
    return;
}

/* fn_800CE220 - 0x800CE220 | size: 0x78 */
void fn_800CE220(void) {
    extern u8 lbl_8047C960[];
    extern void fn_800CC2C0();
    extern void fn_800CD648();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r3 = 0x3FE90000;
    tmp = r3 + 0x21fb;
    r3 = r3 & 0x7FFFFFFF;
    if ((s32)r3 > (s32)tmp) goto L_800CE258;
    f2 = *(f64*)lbl_8047C960;
    r3 = 0x1;
    fn_800CD648();
    goto L_800CE288;
L_800CE258:
    tmp = 0x7FF00000;
    if ((s32)r3 < (s32)tmp) goto L_800CE26C;
    f1 = f1 - f1;
    goto L_800CE288;
L_800CE26C:
    r3 = (u32)sp + 0x10;
    fn_800CC2C0();
    r3 = 0x1 - tmp;
    fn_800CD648();
L_800CE288:
    return;
}

/* fn_800CE298 - 0x800CE298 | size: 0x20 */
void fn_800CE298(void) {
    extern void fn_800CABB0();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CABB0();
    return;
}

/* fn_800CE2B8 - 0x800CE2B8 | size: 0x20 */
void fn_800CE2B8(void) {
    extern void fn_800CADEC();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CADEC();
    return;
}

/* fn_800CE2D8 - 0x800CE2D8 | size: 0x20 */
void fn_800CE2D8(void) {
    extern void fn_800CB024();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CB024();
    return;
}

/* fn_800CE2F8 - 0x800CE2F8 | size: 0x20 */
void fn_800CE2F8(void) {
    extern void fn_800CB2B4();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CB2B4();
    return;
}

/* fn_800CE318 - 0x800CE318 | size: 0x20 */
void fn_800CE318(void) {
    extern void fn_800CB4D8();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CB4D8();
    return;
}

/* fn_800CE338 - 0x800CE338 | size: 0x20 */
void fn_800CE338(void) {
    extern void fn_800CB814();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CB814();
    return;
}

/* fn_800CE358 - 0x800CE358 | size: 0x20 */
void fn_800CE358(void) {
    extern void fn_800CBA90();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CBA90();
    return;
}

/* fn_800CE378 - 0x800CE378 | size: 0x224 */
void fn_800CE378(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047AA10[];
    extern u8 lbl_8047C968[];
    u8 sp[0x20];
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
    u32 r12 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r3 = r6 & 0x7FF00000;
    /* subis r3, r3, 0x7ff0 */;
    if (r3 != 0) goto L_800CE3A8;
    f1 = f1 * f1 + f1;
    tmp = 0x21;
    *(u32*)lbl_8047AA10 = tmp;
    goto L_800CE594;
L_800CE3A8:
    if ((s32)r6 > 0) goto L_800CE3DC;
    r3 = r6 & 0x7FFFFFFF;
    /* or. r3, tmp, r3 */;
    if ((s32)r6 != 0) goto L_800CE3C0;
    goto L_800CE594;
L_800CE3C0:
    if ((s32)r6 >= 0) goto L_800CE3DC;
    r3 = (u32)lbl_80478AC0;
    tmp = 0x21;
    *(u32*)lbl_8047AA10 = tmp;
    f1 = *(f32*)lbl_80478AC0;
    goto L_800CE594;
L_800CE3DC:
    /* srawi. r3, r6, 20 */;
    if ((s32)r6 != 0) goto L_800CE430;
    goto L_800CE3F8;
L_800CE3E8:
    r4 = (u32)tmp >> 11;
    tmp = tmp << 21;
    r6 = r6 | r4;
L_800CE3F8:
    if ((s32)r6 == 0) goto L_800CE3E8;
    r7 = 0x0;
    goto L_800CE410;
L_800CE408:
    r6 = r6 << 1;
    r7 = r7 + 0x1;
L_800CE410:
    r4 = r6 & 0x00100000;
    if ((s32)r6 == 0) goto L_800CE408;
    r4 = 0x20 - r7;
    r4 = (u32)tmp >> r4;
    tmp = tmp << r7;
    r3 = r3 - r5;
    r6 = r6 | r4;
L_800CE430:
    r5 = r6 & 0xFFFFF;
    r4 = r4 & 0x1;
    r5 = r5 | (0x10 << 16);
    if ((s32)r6 == 0) goto L_800CE454;
    r4 = (u32)tmp >> 31;
    tmp = tmp + tmp;
    r4 = r4 + r5;
    r5 = r5 + r4;
L_800CE454:
    r4 = (u32)tmp >> 31;
    tmp = tmp + tmp;
    r4 = r4 + r5;
    r9 = 0x0;
    r5 = r5 + r4;
    r11 = 0x0;
    r10 = 0x0;
    r12 = 0x0;
    r6 = 0x200000;
    goto L_800CE4A8;
L_800CE47C:
    r4 = r11 + r6;
    if ((s32)r4 > (s32)r5) goto L_800CE494;
    r11 = r4 + r6;
    r5 = r5 - r4;
    r12 = r12 + r6;
L_800CE494:
    r4 = (u32)tmp >> 31;
    tmp = tmp + tmp;
    r4 = r4 + r5;
    r6 = (u32)r6 >> 1;
    r5 = r5 + r4;
L_800CE4A8:
    if (r6 != 0) goto L_800CE47C;
    r6 = 0x80000000;
    goto L_800CE520;
L_800CE4B8:
    r7 = r11;
    r8 = r9 + r6;
    if ((s32)r11 < (s32)r5) goto L_800CE4D4;
    if ((s32)r11 != (s32)r5) goto L_800CE50C;
    if (r8 > tmp) goto L_800CE50C;
L_800CE4D4:
    /* clrrwi r4, r8, 31 */;
    r9 = r8 + r6;
    r4 = r4 + (0x8000 << 16);
    if (r4 != 0) goto L_800CE4F4;
    /* clrrwi. r4, r9, 31 */;
    if (r4 != 0) goto L_800CE4F4;
    r11 = r11 + 0x1;
L_800CE4F4:
    r5 = r5 - r7;
    if (tmp >= r8) goto L_800CE504;
L_800CE504:
    tmp = tmp - r8;
    r10 = r10 + r6;
L_800CE50C:
    r4 = (u32)tmp >> 31;
    tmp = tmp + tmp;
    r4 = r4 + r5;
    r6 = (u32)r6 >> 1;
    r5 = r5 + r4;
L_800CE520:
    if (r6 != 0) goto L_800CE4B8;
    /* or. tmp, r5, tmp */;
    if (r6 == 0) goto L_800CE55C;
    f0 = *(f64*)lbl_8047C968;
    tmp = r10 + (0x1 << 16);
    if (tmp != 0xffff) goto L_800CE554;
    r10 = 0x0;
    r12 = r12 + 0x1;
    goto L_800CE55C;
L_800CE554:
    tmp = r10 & 0x1;
    r10 = r10 + tmp;
L_800CE55C:
    tmp = r12 & 0x1;
    r4 = (s32)r12 >> 1;
    r5 = (u32)r10 >> 1;
    r4 = r4 + (0x3fe0 << 16);
    if ((s32)tmp != 1) goto L_800CE578;
    r5 = r5 | (0x8000 << 16);
L_800CE578:
    tmp = (s32)tmp >> 1;
    tmp = tmp << 20;
    r4 = r4 + tmp;
L_800CE594:
    return;
}

/* fn_800CE59C - 0x800CE59C | size: 0x8 */
/* Empty function (blr) - no-op placeholder */
void fn_800CE59C(void) {
}

/* fn_800CE5A4 - 0x800CE5A4 | size: 0xE4 */
void fn_800CE5A4(void) {
    extern u8 lbl_80478AC0[];
    extern u8 lbl_8047C970[];
    extern u8 lbl_8047C978[];
    extern u8 lbl_8047C980[];
    extern u8 lbl_8047C988[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    f0 = *(f32*)lbl_8047C970;
    if (f1 <= f0) goto L_800CE5FC;
    /* frsqrte f2, f1 */;
    f4 = *(f64*)lbl_8047C978;
    f3 = *(f64*)lbl_8047C980;
    f0 = f2 * f2;
    f2 = f4 * f2;
    f0 = -(f1 * f0 - f3);
    f2 = f2 * f0;
    f0 = f2 * f2;
    f2 = f4 * f2;
    f0 = -(f1 * f0 - f3);
    f2 = f2 * f0;
    f0 = f2 * f2;
    f2 = f4 * f2;
    f0 = -(f1 * f0 - f3);
    f0 = f2 * f0;
    f1 = f1 * f0;
    f1 = (f32)f1;
    goto L_800CE680;
L_800CE5FC:
    f0 = *(f64*)lbl_8047C988;
    if (f1 >= f0) goto L_800CE614;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
    goto L_800CE680;
L_800CE614:
    *(f32*)(sp + 0x8) = f1;
    tmp = 0x7F800000;
    r3 = r4 & 0x7F800000;
    if ((s32)r3 == (s32)tmp) goto L_800CE63C;
    if ((s32)r3 >= (s32)tmp) goto L_800CE66C;
    if ((s32)r3 == 0) goto L_800CE654;
    goto L_800CE66C;
L_800CE63C:
    tmp = r4 & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_800CE64C;
    tmp = 0x1;
    goto L_800CE670;
L_800CE64C:
    tmp = 0x2;
    goto L_800CE670;
L_800CE654:
    tmp = r4 & 0x7FFFFF;
    if ((s32)r3 == 0) goto L_800CE664;
    tmp = 0x5;
    goto L_800CE670;
L_800CE664:
    tmp = 0x3;
    goto L_800CE670;
L_800CE66C:
    tmp = 0x4;
L_800CE670:
    if ((s32)tmp != 1) goto L_800CE680;
    r3 = (u32)lbl_80478AC0;
    f1 = *(f32*)lbl_80478AC0;
L_800CE680:
    return;
}

/* fn_800CE688 - 0x800CE688 | size: 0x24 */
void fn_800CE688(void) {
    extern void fn_800CE220();
    u8 sp[0x10];
    u32 tmp = 0;
    f32 f1 = 0.0f;

    fn_800CE220();
    f1 = (f32)f1;
    return;
}

/* fn_800CE6AC - 0x800CE6AC | size: 0x24 */
void fn_800CE6AC(void) {
    extern void fn_800CE148();
    u8 sp[0x10];
    u32 tmp = 0;
    f32 f1 = 0.0f;

    fn_800CE148();
    f1 = (f32)f1;
    return;
}

/* fn_800CE6D0 - 0x800CE6D0 | size: 0x24 */
void fn_800CE6D0(void) {
    extern void fn_800CDBE0();
    u8 sp[0x10];
    u32 tmp = 0;
    f32 f1 = 0.0f;

    fn_800CDBE0();
    f1 = (f32)f1;
    return;
}

/* fn_800CE6F4 - 0x800CE6F4 | size: 0x24 */
void fn_800CE6F4(void) {
    extern void fn_800CE298();
    u8 sp[0x10];
    u32 tmp = 0;
    f32 f1 = 0.0f;

    fn_800CE298();
    f1 = (f32)f1;
    return;
}

/* fn_800CE718 - 0x800CE718 | size: 0x64 */
void fn_800CE718(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    f32 f1 = 0.0f;

    tmp = 0x7F800000;
    *(f32*)(sp + 0x8) = f1;
    r3 = r4 & 0x7F800000;
    if ((s32)r3 == (s32)tmp) goto L_800CE744;
    if ((s32)r3 >= (s32)tmp) goto L_800CE770;
    if ((s32)r3 == 0) goto L_800CE75C;
    goto L_800CE770;
L_800CE744:
    r3 = r4 & 0x7FFFFF;
    tmp = -r3;
    tmp = tmp | r3;
    r3 = (s32)tmp >> 31;
    r3 = r3 + 0x2;
    goto L_800CE774;
L_800CE75C:
    tmp = r4 & 0x7FFFFF;
    r3 = 0x3;
    if ((s32)r3 == 0) goto L_800CE774;
    r3 = 0x5;
    goto L_800CE774;
L_800CE770:
    r3 = 0x4;
L_800CE774:
    return;
}

/* fn_800CE77C - 0x800CE77C | size: 0x20 */
void fn_800CE77C(void) {
    extern void fn_800CE378();
    u8 sp[0x10];
    u32 tmp = 0;

    fn_800CE378();
    return;
}

