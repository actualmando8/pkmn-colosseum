/**
 * @file gs_task_util.c
 * @brief Decompiled functions.
 *
 * Address range: 0x800FEBA0 - 0x800FF0A0
 */

#include "dolphin/types.h"

/* ===================================================================
 * Generated: 0 pattern-matched + 6 stubs
 * Range: 0x800FEBA0 - 0x800FF0A0
 * =================================================================== */

/* 0x800FEBA0 | 0x94 */
void fn_800FEBA0(void) {
    extern u8 lbl_8047AC7C[];
    extern u8 lbl_8047AC94[];
    extern u8 lbl_8047AC98[];
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r3 = 0xAAAB0000;
    r4 = *(u32*)lbl_8047AC98;
    goto L_800FEC0C;
L_800FEBC4:
    tmp = *(u32*)((u8*)r4 + 0x8);
    r30 = *(u32*)((u8*)r4 + 0x4);
    if ((s32)tmp != 3) goto L_800FEC08;
    tmp = *(u8*)((u8*)r4 + 0xD);
    if (tmp != 0) goto L_800FEC08;
    tmp = *(u32*)lbl_8047AC7C;
    *(u32*)lbl_8047AC94 = r4;
    tmp = r4 - tmp;
    tmp = (u32)((u64)r31 * (u64)tmp >> 32);
    r12 = *(u32*)((u8*)r4 + 0x14);
    r4 = *(u32*)((u8*)r4 + 0x10);
    r3 = (u32)tmp >> 4;
    r3 = r3 + 0x1;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800FEC08:
    r4 = r30;
L_800FEC0C:
    if (r4 != 0) goto L_800FEBC4;
    tmp = 0x0;
    *(u32*)lbl_8047AC94 = tmp;
    return;
}

/* 0x800FEC34 | 0x84 */
void fn_800FEC34(void) {
    extern u8 lbl_8047ACB0[];
    extern u8 lbl_8047ACC0[];
    extern void fn_800F0424();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    tmp = *(u32*)lbl_8047ACC0;
    r4 = *(u32*)lbl_8047ACB0;
    ctr_fn = (void(*)(void))tmp;
    if (tmp == 0) goto L_800FEC78;
L_800FEC54:
    tmp = *(u32*)((u8*)r4 + 0x8);
    if ((s32)tmp != 3) goto L_800FEC70;
    tmp = *(u32*)((u8*)r4 + 0x18);
    if (tmp != r3) goto L_800FEC70;
    goto L_800FEC7C;
L_800FEC70:
    r4 = r4 + 0x24;
    if (--ctr != 0) goto L_800FEC54;
L_800FEC78:
    r4 = 0x0;
L_800FEC7C:
    if (r4 == 0) goto L_800FECA8;
    tmp = 0x0;
    *(u8*)((u8*)r4 + 0x15) = tmp;
    tmp = *(u32*)((u8*)r4 + 0xC);
    if ((s32)tmp != 1) goto L_800FECA8;
    r3 = *(u32*)((u8*)r4 + 0x1C);
    if (r3 == 0) goto L_800FECA8;
    fn_800F0424();
L_800FECA8:
    return;
}

/* 0x800FECB8 | 0x84 */
void fn_800FECB8(void) {
    extern u8 lbl_8047ACB0[];
    extern u8 lbl_8047ACC0[];
    extern void fn_800F0438();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    tmp = *(u32*)lbl_8047ACC0;
    r4 = *(u32*)lbl_8047ACB0;
    ctr_fn = (void(*)(void))tmp;
    if (tmp == 0) goto L_800FECFC;
L_800FECD8:
    tmp = *(u32*)((u8*)r4 + 0x8);
    if ((s32)tmp != 3) goto L_800FECF4;
    tmp = *(u32*)((u8*)r4 + 0x18);
    if (tmp != r3) goto L_800FECF4;
    goto L_800FED00;
L_800FECF4:
    r4 = r4 + 0x24;
    if (--ctr != 0) goto L_800FECD8;
L_800FECFC:
    r4 = 0x0;
L_800FED00:
    if (r4 == 0) goto L_800FED2C;
    tmp = 0x1;
    *(u8*)((u8*)r4 + 0x15) = tmp;
    tmp = *(u32*)((u8*)r4 + 0xC);
    if ((s32)tmp != 1) goto L_800FED2C;
    r3 = *(u32*)((u8*)r4 + 0x1C);
    if (r3 == 0) goto L_800FED2C;
    fn_800F0438();
L_800FED2C:
    return;
}

/* 0x800FED3C | 0x12C */
void fn_800FED3C(void) {
    extern u8 lbl_8047ACB0[];
    extern u8 lbl_8047ACB4[];
    extern u8 lbl_8047ACB8[];
    extern u8 lbl_8047ACBC[];
    extern u8 lbl_8047ACCC[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = *(u32*)lbl_8047ACB4;
    tmp = *(u32*)lbl_8047ACB8;
    r7 = *(u32*)lbl_8047ACB0;
    tmp = r6 + tmp;
    r6 = *(u32*)lbl_8047ACBC;
    tmp = tmp * 0x24;
    r8 = r7 + tmp;
    ctr_fn = (void(*)(void))r6;
    if (r6 == 0) goto L_800FED7C;
L_800FED64:
    tmp = *(u32*)((u8*)r8 + 0x8);
    if ((s32)tmp != 0) goto L_800FED74;
    goto L_800FED80;
L_800FED74:
    r8 = r8 + 0x24;
    if (--ctr != 0) goto L_800FED64;
L_800FED7C:
    r8 = 0x0;
L_800FED80:
    if ((u32)r8 == (u32)0x0) return;
    r7 = 0x0;
    r6 = 0x5;
    *(u32*)((u8*)r8 + 0x0) = r7;
    tmp = 0x1;
    *(u32*)((u8*)r8 + 0x4) = r7;
    *(u32*)((u8*)r8 + 0x8) = r6;
    *(u32*)((u8*)r8 + 0xC) = tmp;
    *(u32*)((u8*)r8 + 0x10) = r4;
    *(u8*)((u8*)r8 + 0x14) = r3;
    *(u8*)((u8*)r8 + 0x15) = r7;
    tmp = *(u32*)((u8*)r8 + 0xC);
    if ((s32)tmp != 0) goto L_800FEDC4;
    *(u32*)((u8*)r8 + 0x18) = r5;
    goto L_800FEDD0;
L_800FEDC4:
    *(u32*)((u8*)r8 + 0x18) = r5;
    *(u16*)((u8*)r8 + 0x20) = r7;
    *(u32*)((u8*)r8 + 0x1C) = r7;
L_800FEDD0:
    r4 = *(u32*)lbl_8047ACCC;
    if (r4 != 0) goto L_800FEDEC;
    *(u32*)lbl_8047ACCC = r8;
    return;
    goto L_800FEDEC;
L_800FEDE8:
    r4 = r5;
L_800FEDEC:
    r5 = *(u32*)((u8*)r4 + 0x4);
    if (r5 == 0) goto L_800FEE08;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r8 + 0x14);
    if (r3 < tmp) goto L_800FEDE8;
L_800FEE08:
    if (r5 != 0) goto L_800FEE34;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r8 + 0x14);
    if (r3 >= tmp) goto L_800FEE34;
    *(u32*)((u8*)r8 + 0x0) = r4;
    tmp = 0x0;
    *(u32*)((u8*)r8 + 0x4) = tmp;
    *(u32*)((u8*)r4 + 0x4) = r8;
    return;
L_800FEE34:
    r3 = *(u32*)((u8*)r4 + 0x0);
    if (r3 == 0) goto L_800FEE44;
    *(u32*)((u8*)r3 + 0x4) = r8;
L_800FEE44:
    tmp = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r8 + 0x0) = tmp;
    *(u32*)((u8*)r8 + 0x4) = r4;
    *(u32*)((u8*)r4 + 0x0) = r8;
    tmp = *(u32*)lbl_8047ACCC;
    if ((u32)tmp != (u32)r4) return;
    *(u32*)lbl_8047ACCC = r8;
    return;
}

/* 0x800FEE68 | 0x124 */
void fn_800FEE68(void) {
    extern u8 lbl_8047ACB0[];
    extern u8 lbl_8047ACB4[];
    extern u8 lbl_8047ACB8[];
    extern u8 lbl_8047ACCC[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    tmp = *(u32*)lbl_8047ACB4;
    r6 = *(u32*)lbl_8047ACB0;
    tmp = tmp * 0x24;
    r7 = *(u32*)lbl_8047ACB8;
    r8 = r6 + tmp;
    ctr_fn = (void(*)(void))r7;
    if (r7 == 0) goto L_800FEEA0;
L_800FEE88:
    tmp = *(u32*)((u8*)r8 + 0x8);
    if ((s32)tmp != 0) goto L_800FEE98;
    goto L_800FEEA4;
L_800FEE98:
    r8 = r8 + 0x24;
    if (--ctr != 0) goto L_800FEE88;
L_800FEEA0:
    r8 = 0x0;
L_800FEEA4:
    if ((u32)r8 == (u32)0x0) return;
    r7 = 0x0;
    r6 = 0x3;
    *(u32*)((u8*)r8 + 0x0) = r7;
    tmp = 0x1;
    *(u32*)((u8*)r8 + 0x4) = r7;
    *(u32*)((u8*)r8 + 0x8) = r6;
    *(u32*)((u8*)r8 + 0xC) = tmp;
    *(u32*)((u8*)r8 + 0x10) = r4;
    *(u8*)((u8*)r8 + 0x14) = r3;
    *(u8*)((u8*)r8 + 0x15) = r7;
    tmp = *(u32*)((u8*)r8 + 0xC);
    if ((s32)tmp != 0) goto L_800FEEE8;
    *(u32*)((u8*)r8 + 0x18) = r5;
    goto L_800FEEF4;
L_800FEEE8:
    *(u32*)((u8*)r8 + 0x18) = r5;
    *(u16*)((u8*)r8 + 0x20) = r7;
    *(u32*)((u8*)r8 + 0x1C) = r7;
L_800FEEF4:
    r4 = *(u32*)lbl_8047ACCC;
    if (r4 != 0) goto L_800FEF10;
    *(u32*)lbl_8047ACCC = r8;
    return;
    goto L_800FEF10;
L_800FEF0C:
    r4 = r5;
L_800FEF10:
    r5 = *(u32*)((u8*)r4 + 0x4);
    if (r5 == 0) goto L_800FEF2C;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r8 + 0x14);
    if (r3 < tmp) goto L_800FEF0C;
L_800FEF2C:
    if (r5 != 0) goto L_800FEF58;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r8 + 0x14);
    if (r3 >= tmp) goto L_800FEF58;
    *(u32*)((u8*)r8 + 0x0) = r4;
    tmp = 0x0;
    *(u32*)((u8*)r8 + 0x4) = tmp;
    *(u32*)((u8*)r4 + 0x4) = r8;
    return;
L_800FEF58:
    r3 = *(u32*)((u8*)r4 + 0x0);
    if (r3 == 0) goto L_800FEF68;
    *(u32*)((u8*)r3 + 0x4) = r8;
L_800FEF68:
    tmp = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r8 + 0x0) = tmp;
    *(u32*)((u8*)r8 + 0x4) = r4;
    *(u32*)((u8*)r4 + 0x0) = r8;
    tmp = *(u32*)lbl_8047ACCC;
    if ((u32)tmp != (u32)r4) return;
    *(u32*)lbl_8047ACCC = r8;
    return;
}

/* 0x800FEF8C | 0x114 */
void fn_800FEF8C(void) {
    extern u8 lbl_8047ACB0[];
    extern u8 lbl_8047ACB4[];
    extern u8 lbl_8047ACCC[];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    tmp = *(u32*)lbl_8047ACB4;
    r7 = *(u32*)lbl_8047ACB0;
    ctr_fn = (void(*)(void))tmp;
    if (tmp == 0) goto L_800FEFB8;
L_800FEFA0:
    tmp = *(u32*)((u8*)r7 + 0x8);
    if ((s32)tmp != 0) goto L_800FEFB0;
    goto L_800FEFBC;
L_800FEFB0:
    r7 = r7 + 0x24;
    if (--ctr != 0) goto L_800FEFA0;
L_800FEFB8:
    r7 = 0x0;
L_800FEFBC:
    if ((u32)r7 == (u32)0x0) return;
    r6 = 0x0;
    tmp = 0x1;
    *(u32*)((u8*)r7 + 0x0) = r6;
    *(u32*)((u8*)r7 + 0x4) = r6;
    *(u32*)((u8*)r7 + 0x8) = tmp;
    *(u32*)((u8*)r7 + 0xC) = tmp;
    *(u32*)((u8*)r7 + 0x10) = r4;
    *(u8*)((u8*)r7 + 0x14) = r3;
    *(u8*)((u8*)r7 + 0x15) = r6;
    tmp = *(u32*)((u8*)r7 + 0xC);
    if ((s32)tmp != 0) goto L_800FEFFC;
    *(u32*)((u8*)r7 + 0x18) = r5;
    goto L_800FF008;
L_800FEFFC:
    *(u32*)((u8*)r7 + 0x18) = r5;
    *(u16*)((u8*)r7 + 0x20) = r6;
    *(u32*)((u8*)r7 + 0x1C) = r6;
L_800FF008:
    r4 = *(u32*)lbl_8047ACCC;
    if (r4 != 0) goto L_800FF024;
    *(u32*)lbl_8047ACCC = r7;
    return;
    goto L_800FF024;
L_800FF020:
    r4 = r5;
L_800FF024:
    r5 = *(u32*)((u8*)r4 + 0x4);
    if (r5 == 0) goto L_800FF040;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r7 + 0x14);
    if (r3 < tmp) goto L_800FF020;
L_800FF040:
    if (r5 != 0) goto L_800FF06C;
    r3 = *(u8*)((u8*)r4 + 0x14);
    tmp = *(u8*)((u8*)r7 + 0x14);
    if (r3 >= tmp) goto L_800FF06C;
    *(u32*)((u8*)r7 + 0x0) = r4;
    tmp = 0x0;
    *(u32*)((u8*)r7 + 0x4) = tmp;
    *(u32*)((u8*)r4 + 0x4) = r7;
    return;
L_800FF06C:
    r3 = *(u32*)((u8*)r4 + 0x0);
    if (r3 == 0) goto L_800FF07C;
    *(u32*)((u8*)r3 + 0x4) = r7;
L_800FF07C:
    tmp = *(u32*)((u8*)r4 + 0x0);
    *(u32*)((u8*)r7 + 0x0) = tmp;
    *(u32*)((u8*)r7 + 0x4) = r4;
    *(u32*)((u8*)r4 + 0x0) = r7;
    tmp = *(u32*)lbl_8047ACCC;
    if ((u32)tmp != (u32)r4) return;
    *(u32*)lbl_8047ACCC = r7;
    return;
}
