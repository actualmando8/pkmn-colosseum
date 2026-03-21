#include "dolphin/types.h"

/*
 * math_longlong.c - CRT library functions.
 *
 * Stub implementations for function coverage.
 */

/* __div2u - 0x800C483C | size: 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __div2u(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800C4928 - 0x800C4928 | size: 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4928(void) {
    u8 sp[0x10];
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
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* clrrwi. r9, r3, 31 */;
    if ((s32)r0 == (s32)0) goto L_800C493C;
    r4 = 0x0 - r4;
    /* subfze r3, r3 */;
L_800C493C: ;
    /* clrrwi. r10, r5, 31 */;
    if ((s32)r0 == (s32)0) goto L_800C4950;
    r6 = 0x0 - r6;
    /* subfze r5, r5 */;
L_800C4950: ;
    r0 = __cntlzw(r3);
    r9 = __cntlzw(r4);
    if ((s32)r3 != (s32)0x0) goto L_800C4968;
    r0 = r9 + 0x20;
L_800C4968: ;
    r9 = __cntlzw(r5);
    r10 = __cntlzw(r6);
    if ((s32)r5 != (s32)0x0) goto L_800C497C;
    r9 = r10 + 0x20;
L_800C497C: ;
    r10 = 0x40 - r0;
    if ((s32)r0 > (s32)r9) goto L_800C4A50;
    r9 = r9 + 0x1;
    r9 = 0x40 - r9;
    r0 = r0 + r9;
    r9 = r10 - r9;
    ctr_fn = (void(*)(void))r9;
    /* subi r7, r9, 0x20 */;
    if ((s32)r9 < (s32)0x20) goto L_800C49B4;
    r8 = (u32)r3 >> r7;
    r7 = 0x0;
    goto L_800C49C8;
L_800C49B4: ;
    r8 = (u32)r4 >> r9;
    r7 = 0x20 - r9;
    r7 = r3 << r7;
    r8 = r8 | r7;
    r7 = (u32)r3 >> r9;
L_800C49C8: ;
    /* subic r9, r0, 0x20 */;
    if ((s32)r0 < (s32)0x20) goto L_800C49E0;
    r3 = r4 << r9;
    r4 = 0x0;
    goto L_800C49F4;
L_800C49E0: ;
    r3 = r3 << r0;
    r9 = 0x20 - r0;
    r9 = (u32)r4 >> r9;
    r3 = r3 | r9;
    r4 = r4 << r0;
L_800C49F4: ;
    r10 = -0x1;
    r7 = r7 + 0x0;
L_800C49FC: ;
    r4 = r4 + r4; /* +carry */;
    r3 = r3 + r3; /* +carry */;
    r8 = r8 + r8; /* +carry */;
    r7 = r7 + r7; /* +carry */;
    r0 = r8 - r6;
    /* subfe. r9, r5, r7 */;
    if ((s32)r0 < (s32)0x20) goto L_800C4A24;
    r8 = r0;
    r7 = r9;
    r0 = r10 + 0x1;
L_800C4A24: ;
    if (--ctr != 0) goto L_800C49FC;
    r4 = r4 + r4; /* +carry */;
    r3 = r3 + r3; /* +carry */;
    r9 = *(u32*)(sp + 0x8);
    r10 = *(u32*)(sp + 0xC);
    /* xor. r7, r9, r10 */;
    if ((s32)r0 == (s32)0x20) goto L_800C4A58;
    r4 = 0x0 - r4;
    /* subfze r3, r3 */;
    goto L_800C4A58;
L_800C4A50: ;
    r4 = 0x0;
    r3 = 0x0;
L_800C4A58: ;
    return;
}
#pragma pop

/* __mod2u - 0x800C4A60 | size: 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void __mod2u(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800C4B44 - 0x800C4B44 | size: 0x10C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4B44(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    if ((s32)r3 >= (s32)0x0) goto L_800C4B54;
    r4 = 0x0 - r4;
    /* subfze r3, r3 */;
L_800C4B54: ;
    if ((s32)r5 >= (s32)0x0) goto L_800C4B64;
    r6 = 0x0 - r6;
    /* subfze r5, r5 */;
L_800C4B64: ;
    r0 = __cntlzw(r3);
    r9 = __cntlzw(r4);
    if ((s32)r3 != (s32)0x0) goto L_800C4B78;
    r0 = r9 + 0x20;
L_800C4B78: ;
    r9 = __cntlzw(r5);
    r10 = __cntlzw(r6);
    if ((s32)r5 != (s32)0x0) goto L_800C4B8C;
    r9 = r10 + 0x20;
L_800C4B8C: ;
    r10 = 0x40 - r0;
    if ((s32)r0 > (s32)r9) goto L_800C4C40;
    r9 = r9 + 0x1;
    r9 = 0x40 - r9;
    r0 = r0 + r9;
    r9 = r10 - r9;
    ctr_fn = (void(*)(void))r9;
    /* subi r7, r9, 0x20 */;
    if ((s32)r9 < (s32)0x20) goto L_800C4BC4;
    r8 = (u32)r3 >> r7;
    r7 = 0x0;
    goto L_800C4BD8;
L_800C4BC4: ;
    r8 = (u32)r4 >> r9;
    r7 = 0x20 - r9;
    r7 = r3 << r7;
    r8 = r8 | r7;
    r7 = (u32)r3 >> r9;
L_800C4BD8: ;
    /* subic r9, r0, 0x20 */;
    if ((s32)r0 < (s32)0x20) goto L_800C4BF0;
    r3 = r4 << r9;
    r4 = 0x0;
    goto L_800C4C04;
L_800C4BF0: ;
    r3 = r3 << r0;
    r9 = 0x20 - r0;
    r9 = (u32)r4 >> r9;
    r3 = r3 | r9;
    r4 = r4 << r0;
L_800C4C04: ;
    r10 = -0x1;
    r7 = r7 + 0x0;
L_800C4C0C: ;
    r4 = r4 + r4; /* +carry */;
    r3 = r3 + r3; /* +carry */;
    r8 = r8 + r8; /* +carry */;
    r7 = r7 + r7; /* +carry */;
    r0 = r8 - r6;
    /* subfe. r9, r5, r7 */;
    if ((s32)r0 < (s32)0x20) goto L_800C4C34;
    r8 = r0;
    r7 = r9;
    r0 = r10 + 0x1;
L_800C4C34: ;
    if (--ctr != 0) goto L_800C4C0C;
    r4 = r8;
    r3 = r7;
L_800C4C40: ;
    if ((s32)r0 >= (s32)0x20) return;
    r4 = 0x0 - r4;
    /* subfze r3, r3 */;
    return;
}
#pragma pop

/* fn_800C4C50 - 0x800C4C50 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4C50(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r8 = 0x20 - r5;
    /* subic r9, r5, 0x20 */;
    r3 = r3 << r5;
    r10 = (u32)r4 >> r8;
    r3 = r3 | r10;
    r10 = r4 << r9;
    r3 = r3 | r10;
    r4 = r4 << r5;
    return;
}
#pragma pop

/* fn_800C4C74 - 0x800C4C74 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4C74(void) {
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r8 = 0x20 - r5;
    /* subic r9, r5, 0x20 */;
    r4 = (u32)r4 >> r5;
    r10 = r3 << r8;
    r4 = r4 | r10;
    r10 = (u32)r3 >> r9;
    r4 = r4 | r10;
    r3 = (u32)r3 >> r5;
    return;
}
#pragma pop

/* fn_800C4C98 - 0x800C4C98 | size: 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4C98(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;

    r8 = 0x20 - r5;
    /* subic. r9, r5, 0x20 */;
    r4 = (u32)r4 >> r5;
    r10 = r3 << r8;
    r4 = r4 | r10;
    r10 = (s32)r3 >> r9;
    if ((s32)r0 <= (s32)0) goto L_800C4CB8;
    r4 = r4 | r10;
L_800C4CB8: ;
    r3 = (s32)r3 >> r5;
    return;
}
#pragma pop

/* fn_800C4CC0 - 0x800C4CC0 | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4CC0(void) {
    u8 sp[0x10];
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    f32 f1 = 0.0f;

    *(f64*)(sp + 0x8) = f1;
    r3 = *(u32*)(sp + 0x8);
    r4 = *(u32*)(sp + 0xC);
    /* extrwi r5, r3, 11, 1 */;
    if ((u32)r5 >= (u32)0x3ff) goto L_800C4CE8;
    r3 = 0x0;
    r4 = 0x0;
    goto L_800C4D84;
L_800C4CE8: ;
    r6 = r3;
    r3 = r3 & 0xFFFFF;
    r3 = r3 | (0x10 << 16);
    /* subi r5, r5, 0x433 */;
    if ((s32)r5 >= (s32)0x0) goto L_800C4D28;
    r5 = -r5;
    r8 = 0x20 - r5;
    /* subic r9, r5, 0x20 */;
    r4 = (u32)r4 >> r5;
    r10 = r3 << r8;
    r4 = r4 | r10;
    r10 = (u32)r3 >> r9;
    r4 = r4 | r10;
    r3 = (u32)r3 >> r5;
    goto L_800C4D74;
L_800C4D28: ;
    /* ble+ .L_800C4D54 */;
    /* clrrwi. r6, r6, 31 */;
    if ((s32)r5 == (s32)0xa) goto L_800C4D44;
    r3 = (0x8000 << 16);
    r4 = 0x0;
    goto L_800C4D84;
L_800C4D44: ;
    r3 = (0x7fff << 16);
    r3 = r3 | 0xffff;
    r4 = -0x1;
    goto L_800C4D84;
L_800C4D54: ;
    r8 = 0x20 - r5;
    /* subic r9, r5, 0x20 */;
    r3 = r3 << r5;
    r10 = (u32)r4 >> r8;
    r3 = r3 | r10;
    r10 = r4 << r9;
    r3 = r3 | r10;
    r4 = r4 << r5;
L_800C4D74: ;
    /* clrrwi. r6, r6, 31 */;
    if ((s32)r5 == (s32)0xa) goto L_800C4D84;
    r4 = 0x0 - r4;
    /* subfze r3, r3 */;
L_800C4D84: ;
    return;
}
#pragma pop

/* fn_800C4D8C - 0x800C4D8C | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C4D8C(void) {
    extern u8 lbl_8026FE70[];
    extern u8 lbl_8026FEA8[];
    extern u8 lbl_80478980[];
    extern void fn_8009AAD4();
    extern void fn_8009AB50();
    extern void fn_8009AB60();
    extern void fn_8009ABD0();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r0 = *(u32*)lbl_80478980;
    if ((s32)r0 != (s32)-0x1) goto L_800C4E1C;
    r3 = (u32)lbl_8026FE70;
    r3 = (u32)lbl_8026FE70;
    /* crclr cr1eq */;
    OSReport();
    r3 = (u32)lbl_8026FEA8;
    r3 = (u32)lbl_8026FEA8;
    /* crclr cr1eq */;
    OSReport();
    OSGetArenaLo();
    r31 = r3;
    OSGetArenaHi();
    r30 = r3;
    r3 = r31;
    r5 = 0x1;
    r4 = r30;
    fn_8009AB60();
    r31 = r3;
    OSSetArenaLo();
    r0 = r31 + 0x1f;
    /* clrrwi r30, r30, 5 */;
    /* clrrwi r3, r0, 5 */;
    r4 = r30;
    fn_8009ABD0();
    fn_8009AB50();
    r3 = r30;
    OSSetArenaLo();
L_800C4E1C: ;
    r3 = *(u32*)lbl_80478980;
    r4 = r29;
    fn_8009AAD4();
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

