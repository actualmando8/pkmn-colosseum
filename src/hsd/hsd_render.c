
/* Forward declarations for converted functions */
void fn_80197650(void);

/**
 * @file hsd_render.c
 * @brief HSD internal functions (0x80197344-0x80197A64).
 *
 * Stub coverage for 7 functions.
 */

#include "dolphin/types.h"
#include "hsd/hsd_debug.h"

/* 0x80197344 | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80197344(void) {
    extern u8 lbl_8047B240[];
    extern void fn_80197784();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* mr. r31, r3 */;
    if ((s32)r0 == (s32)0) goto L_801973E8;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x4020;
    r0 = __cntlzw(r0);
    /* srwi. r0, r0, 5 */;
    if ((s32)r0 == (s32)0) goto L_80197380;
    r3 = r31;
    fn_80197784();
    goto L_801973E8;
L_80197380: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00000020;
    if ((s32)r0 == (s32)0) goto L_801973E8;
    r0 = *(u32*)lbl_8047B240;
    if ((u32)r0 == (u32)0x0) goto L_801973E8;
    r30 = *(u32*)((u8*)r31 + 0x18);
    goto L_801973E0;
L_801973A0: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    /* clrrwi. r0, r0, 31 */;
    if ((u32)r0 == (u32)0x0) goto L_801973D0;
    r0 = *(u32*)((u8*)r30 + 0x4);
    r6 = r31;
    r4 = *(u32*)((u8*)r30 + 0x4);
    r3 = 0x0;
    r12 = *(u32*)lbl_8047B240;
    /* extrwi r5, r0, 24, 2 */;
    r4 = r4 & 0x3F;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801973D0: ;
    r0 = *(u32*)((u8*)r30 + 0x4);
    r0 = r0 & 0x7FFFFFFF;
    *(u32*)((u8*)r30 + 0x4) = r0;
    r30 = *(u32*)((u8*)r30 + 0x0);
L_801973E0: ;
    if ((u32)r30 != (u32)0x0) goto L_801973A0;
L_801973E8: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80197400 | 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80197400(void) {
    extern u8 lbl_80465348[];
    extern u8 lbl_80478C64[];
    extern u8 lbl_80478C68[];
    extern u8 lbl_80478C6C[];
    extern u8 lbl_8047B24C[];
    extern u8 lbl_8047B250[];
    extern u8 lbl_8047B254[];
    extern u8 lbl_8047B258[];
    extern u8 lbl_8047B25C[];
    extern void fn_801A84F0();
    extern void fn_801AA498();
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
    u32 r30 = 0;
    u32 r31 = 0;

    r31 = *(u32*)lbl_8047B24C;
    goto L_80197448;
L_8019741C: ;
    r0 = *(u32*)((u8*)r31 + 0x30);
    r30 = *(u32*)((u8*)r31 + 0x44);
    if ((u32)r0 == (u32)0x0) goto L_80197434;
    r3 = *(u32*)((u8*)r31 + 0x30);
    fn_801A84F0();
L_80197434: ;
    r3 = (u32)lbl_80465348;
    r4 = r31;
    r3 = (u32)lbl_80465348;
    fn_801AA498();
    r31 = r30;
L_80197448: ;
    if ((u32)r31 != (u32)0x0) goto L_8019741C;
    r9 = 0x0;
    r8 = (u32)lbl_8047B24C;
    r7 = 0x0;
    r6 = (u32)lbl_8047B250;
    r5 = 0x0;
    r4 = 0x0;
    r3 = (u32)lbl_8047B258;
    r0 = 0x0;
    *(u32*)lbl_8047B24C = r9;
    *(u32*)lbl_80478C64 = r8;
    *(u32*)lbl_8047B250 = r7;
    *(u32*)lbl_80478C68 = r6;
    *(u32*)lbl_8047B254 = r5;
    *(u32*)lbl_8047B258 = r4;
    *(u32*)lbl_80478C6C = r3;
    *(u32*)lbl_8047B25C = r0;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x801974A8 | 0x154 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801974A8(void) {
    extern u8 lbl_80465348[];
    extern u8 lbl_80478C64[];
    extern u8 lbl_80478C68[];
    extern u8 lbl_80478C6C[];
    extern u8 lbl_8047B24C[];
    extern u8 lbl_8047B250[];
    extern u8 lbl_8047B254[];
    extern u8 lbl_8047B258[];
    extern u8 lbl_8047B25C[];
    extern void fn_801942B8();
    extern void fn_801A84F0();
    extern void fn_801AA498();
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
    u32 r12 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    fn_801942B8();
    r0 = r3 + 0x54;
    r30 = *(u32*)lbl_8047B250;
    r31 = r0;
    goto L_80197510;
L_801974D0: ;
    r0 = *(u32*)((u8*)r30 + 0x30);
    r3 = *(u32*)((u8*)r30 + 0x34);
    if ((u32)r0 == (u32)0x0) goto L_801974E8;
    r4 = *(u32*)((u8*)r30 + 0x30);
    goto L_801974EC;
L_801974E8: ;
    r4 = r31;
L_801974EC: ;
    r8 = *(u32*)((u8*)r30 + 0x34);
    r5 = r30;
    r7 = *(u32*)((u8*)r30 + 0x38);
    r6 = 0x4;
    r8 = *(u32*)((u8*)r8 + 0x0);
    r12 = *(u32*)((u8*)r8 + 0x48);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r30 = *(u32*)((u8*)r30 + 0x3C);
L_80197510: ;
    if ((u32)r30 != (u32)0x0) goto L_801974D0;
    r30 = *(u32*)lbl_8047B258;
    goto L_80197560;
L_80197520: ;
    r0 = *(u32*)((u8*)r30 + 0x30);
    r3 = *(u32*)((u8*)r30 + 0x34);
    if ((u32)r0 == (u32)0x0) goto L_80197538;
    r4 = *(u32*)((u8*)r30 + 0x30);
    goto L_8019753C;
L_80197538: ;
    r4 = r31;
L_8019753C: ;
    r8 = *(u32*)((u8*)r30 + 0x34);
    r5 = r30;
    r7 = *(u32*)((u8*)r30 + 0x38);
    r6 = 0x2;
    r8 = *(u32*)((u8*)r8 + 0x0);
    r12 = *(u32*)((u8*)r8 + 0x48);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r30 = *(u32*)((u8*)r30 + 0x40);
L_80197560: ;
    if ((u32)r30 != (u32)0x0) goto L_80197520;
    r30 = *(u32*)lbl_8047B24C;
    goto L_8019759C;
L_80197570: ;
    r0 = *(u32*)((u8*)r30 + 0x30);
    r31 = *(u32*)((u8*)r30 + 0x44);
    if ((u32)r0 == (u32)0x0) goto L_80197588;
    r3 = *(u32*)((u8*)r30 + 0x30);
    fn_801A84F0();
L_80197588: ;
    r3 = (u32)lbl_80465348;
    r4 = r30;
    r3 = (u32)lbl_80465348;
    fn_801AA498();
    r30 = r31;
L_8019759C: ;
    if ((u32)r30 != (u32)0x0) goto L_80197570;
    r9 = 0x0;
    r8 = (u32)lbl_8047B24C;
    r7 = 0x0;
    r6 = (u32)lbl_8047B250;
    r5 = 0x0;
    r4 = 0x0;
    r3 = (u32)lbl_8047B258;
    r0 = 0x0;
    *(u32*)lbl_8047B24C = r9;
    *(u32*)lbl_80478C64 = r8;
    *(u32*)lbl_8047B250 = r7;
    *(u32*)lbl_80478C68 = r6;
    *(u32*)lbl_8047B254 = r5;
    *(u32*)lbl_8047B258 = r4;
    *(u32*)lbl_80478C6C = r3;
    *(u32*)lbl_8047B25C = r0;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x54 | fn_801975FC | two_call_arg_check */
void fn_801975FC(u32 arg1) {
    if (arg1 == 0) { return; }
    fn_80197650();
    fn_80197650();
}

/* 0x80197650 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80197650(void) {
    extern void fn_80197650();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r28 = r5;
    if ((s32)r4 > (s32)0x1) goto L_8019769C;
    r0 = *(u32*)(sp + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_80197694;
    r3 = *(u32*)(sp + 0x8);
    r0 = 0x0;
    *(u32*)(r3 + r28) = r0;
L_80197694: ;
    r3 = *(u32*)(sp + 0x8);
    goto L_80197764;
L_8019769C: ;
    r0 = (u32)r4 >> 31;
    r30 = *(u32*)(sp + 0x8);
    r0 = r0 + r4;
    r3 = 0x0;
    r0 = (s32)r0 >> 1;
    r29 = r4 - r0;
    goto L_801976C0;
L_801976B8: ;
    r30 = *(u32*)(r30 + r28);
    r3 = r3 + 0x1;
L_801976C0: ;
    if ((s32)r3 < (s32)r0) goto L_801976B8;
    r3 = *(u32*)(sp + 0x8);
    r4 = r0;
    r5 = r28;
    fn_80197650();
    r0 = r3;
    r3 = r30;
    r31 = r0;
    r4 = r29;
    r5 = r28;
    fn_80197650();
    r0 = 0x0;
    r30 = r3;
    *(u32*)(sp + 0x8) = r0;
    r3 = r1 + 0x8;
    goto L_80197734;
L_80197704: ;
    f1 = *(f32*)((u8*)r31 + 0x2C);
    f0 = *(f32*)((u8*)r30 + 0x2C);
    /* cror eq, lt, eq */;
    if (f1 != f0) goto L_80197724;
    *(u32*)((u8*)r3 + 0x0) = r31;
    r31 = *(u32*)(r31 + r28);
    goto L_8019772C;
L_80197724: ;
    *(u32*)((u8*)r3 + 0x0) = r30;
    r30 = *(u32*)(r30 + r28);
L_8019772C: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    r3 = r0 + r28;
L_80197734: ;
    if ((u32)r31 == (u32)0x0) goto L_80197744;
    if ((u32)r30 != (u32)0x0) goto L_80197704;
L_80197744: ;
    if ((u32)r31 == (u32)0x0) goto L_80197754;
    *(u32*)((u8*)r3 + 0x0) = r31;
    goto L_80197760;
L_80197754: ;
    if ((u32)r30 == (u32)0x0) goto L_80197760;
    *(u32*)((u8*)r3 + 0x0) = r30;
L_80197760: ;
    r3 = *(u32*)(sp + 0x8);
L_80197764: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    r28 = *(u32*)(sp + 0x10);
    return;
}
#pragma pop

/* 0x80197784 | 0x214 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80197784(void) {
    extern u8 lbl_80465348[];
    extern u8 lbl_80478C64[];
    extern u8 lbl_80478C68[];
    extern u8 lbl_80478C6C[];
    extern u8 lbl_8047B244[];
    extern u8 lbl_8047B254[];
    extern u8 lbl_8047B25C[];
    extern u8 lbl_8047D9E0[];
    extern u8 lbl_8047D9E8[];
    extern void fn_800A2D64();
    extern void fn_801942B8();
    extern void fn_80196E10();
    extern void fn_8019D9DC();
    extern void fn_801A8524();
    extern void fn_801AA4CC();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0x3c(r1) */;
    r28 = r3;
    r29 = r4;
    r30 = r6;
    r0 = *(u32*)((u8*)r3 + 0x14);
    r0 = r0 & 0x00000010;
    if ((s32)r0 != (s32)0) goto L_80197984;
    r3 = *(u32*)((u8*)r28 + 0x14);
    r0 = r5 << 18;
    /* and. r31, r3, r0 */;
    if ((s32)r0 == (s32)0) goto L_80197984;
    if ((u32)r28 == (u32)0x0) goto L_8019780C;
    if ((u32)r28 != (u32)0x0) goto L_801977DC;
    r3 = (u32)lbl_8047D9E0;
    r4 = 0x25d;
    r5 = (u32)lbl_8047D9E8;
    fn_80196E10();
L_801977DC: ;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r3 = 0x0;
    r0 = r0 & 0x00800000;
    if ((u32)r28 != (u32)0x0) goto L_801977FC;
    r0 = *(u32*)((u8*)r28 + 0x14);
    r0 = r0 & 0x00000040;
    if ((u32)r28 == (u32)0x0) goto L_801977FC;
    r3 = 0x1;
L_801977FC: ;
    if ((s32)r3 == (s32)0x0) goto L_8019780C;
    r3 = r28;
    fn_8019D9DC();
L_8019780C: ;
    if ((u32)r29 != (u32)0x0) goto L_80197820;
    fn_801942B8();
    r0 = r3 + 0x54;
    r29 = r0;
L_80197820: ;
    r6 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r5 = r1 + 0x8;
    r12 = *(u32*)((u8*)r6 + 0x44);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    r0 = r31 & 0x00040000;
    if ((u32)r29 == (u32)0x0) goto L_80197868;
    r5 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r7 = r30;
    r12 = *(u32*)((u8*)r5 + 0x48);
    r5 = r1 + 0x8;
    r6 = 0x1;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80197868: ;
    r0 = *(u32*)lbl_8047B244;
    if ((s32)r0 != (s32)0x0) goto L_801978D0;
    r0 = r31 & 0x00100000;
    if ((s32)r0 == (s32)0x0) goto L_801978A0;
    r5 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r7 = r30;
    r12 = *(u32*)((u8*)r5 + 0x48);
    r5 = r1 + 0x8;
    r6 = 0x4;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_801978A0: ;
    r0 = r31 & 0x00080000;
    if ((s32)r0 == (s32)0x0) goto L_80197984;
    r5 = *(u32*)((u8*)r28 + 0x0);
    r3 = r28;
    r4 = r29;
    r7 = r30;
    r12 = *(u32*)((u8*)r5 + 0x48);
    r5 = r1 + 0x8;
    r6 = 0x2;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    goto L_80197984;
L_801978D0: ;
    r0 = r31 & 0x00180000;
    if ((s32)r0 == (s32)0x0) goto L_80197984;
    r3 = (u32)lbl_80465348;
    r3 = (u32)lbl_80465348;
    fn_801AA4CC();
    r27 = r3;
    r4 = 0x0;
    r5 = 0x18;
    r3 = r27 + 0x30;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r1 + 0x8;
    r4 = r27;
    fn_800A2D64();
    if ((u32)r29 == (u32)0x0) goto L_80197924;
    fn_801A8524();
    r0 = r3;
    r3 = r29;
    *(u32*)((u8*)r27 + 0x30) = r0;
    r4 = *(u32*)((u8*)r27 + 0x30);
    fn_800A2D64();
L_80197924: ;
    *(u32*)((u8*)r27 + 0x34) = r28;
    r0 = r31 & 0x00100000;
    r0 = r27 + 0x44;
    *(u32*)((u8*)r27 + 0x38) = r30;
    r3 = *(u32*)lbl_80478C64;
    *(u32*)((u8*)r3 + 0x0) = r27;
    *(u32*)lbl_80478C64 = r0;
    if ((u32)r29 == (u32)0x0) goto L_80197960;
    r3 = *(u32*)lbl_80478C68;
    r0 = r27 + 0x3c;
    *(u32*)((u8*)r3 + 0x0) = r27;
    r3 = *(u32*)lbl_8047B254;
    *(u32*)lbl_80478C68 = r0;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B254 = r0;
L_80197960: ;
    r0 = r31 & 0x00080000;
    if ((u32)r29 == (u32)0x0) goto L_80197984;
    r3 = *(u32*)lbl_80478C6C;
    r0 = r27 + 0x40;
    *(u32*)((u8*)r3 + 0x0) = r27;
    r3 = *(u32*)lbl_8047B25C;
    *(u32*)lbl_80478C6C = r0;
    r0 = r3 + 0x1;
    *(u32*)lbl_8047B25C = r0;
L_80197984: ;
    /* lmw r27, 0x3c(r1) */;
    return;
}
#pragma pop

/* 0x80197998 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80197998(void) {
    extern void fn_80199704();
    extern void fn_8019F024();
    extern void fn_801A5DCC();
    extern void fn_801AB63C();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r27, 0xc(r1) */;
    r31 = r3;
    r27 = r4;
    r28 = r5;
    r30 = r6;
    r29 = r7;
    fn_8019F024();
    r0 = r29 & 0x04000000;
    r30 = r30 << 1;
    if ((s32)r0 != (s32)0) goto L_801979E0;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x00010000;
    if ((s32)r0 == (s32)0) goto L_801979E0;
    r3 = r28;
    fn_801A5DCC();
L_801979E0: ;
    r3 = 0x0;
    r4 = 0x0;
    fn_801AB63C();
    r31 = *(u32*)((u8*)r31 + 0x18);
    goto L_80197A38;
L_801979F4: ;
    r0 = *(u32*)((u8*)r31 + 0x14);
    r0 = r0 & 0x1;
    if ((s32)r0 != (s32)0) goto L_80197A34;
    r0 = *(u32*)((u8*)r31 + 0x14);
    /* and. r0, r0, r30 */;
    if ((s32)r0 == (s32)0) goto L_80197A34;
    r3 = r31;
    fn_80199704();
    r6 = *(u32*)((u8*)r31 + 0x0);
    r3 = r31;
    r4 = r27;
    r5 = r28;
    r12 = *(u32*)((u8*)r6 + 0x3C);
    r6 = r29;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80197A34: ;
    r31 = *(u32*)((u8*)r31 + 0x4);
L_80197A38: ;
    if ((u32)r31 != (u32)0x0) goto L_801979F4;
    r3 = 0x0;
    fn_80199704();
    r3 = 0x0;
    fn_8019F024();
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

