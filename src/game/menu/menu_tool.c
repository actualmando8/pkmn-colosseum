/**
 * @file menu_tool.c
 * @brief Menu tool functions (0x80072A00-0x80075818)
 *
 * Address range: 0x80072A00 - 0x8007581C
 * Total functions: 24
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_800060F0();
extern void fn_8008ABE4();
extern void fn_800A501C();
extern void fn_800A50E4();
extern void fn_800A541C();
extern void fn_800A7BCC();
extern void fn_800CE148();
extern void fn_800D0F44();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800D59B8();
extern void fn_800D5CB8();
extern void fn_800D61E4();
extern void fn_800D6728();
extern void fn_800D67BC();
extern void fn_800D6A00();
extern void fn_800D7820();
extern void fn_800D85D4();
extern void fn_800D888C();
extern void fn_800D88DC();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800E3DC4();
extern void fn_800F0308();
extern void fn_800FF56C();
extern void fn_80102568();
extern void fn_80102620();
extern void fn_801026A4();
extern void fn_80109934();
extern void fn_80109B90();
extern void fn_80109C88();
extern void fn_8010A420();
extern void fn_8010A5BC();
extern void fn_801240C4();
extern void fn_80135938();
extern void fn_801CB9D8();
extern void fn_801DAC3C();
extern void fn_8025F350();
extern void fn_8025F3F4();
extern void fn_8025F484();
extern void fn_8025F584();
extern void fn_8025F648();
extern void OSGetTick();
extern void OSReport();
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047A5D0;
extern u8 lbl_8047A604;
extern u8 lbl_8047A608;
extern u8 lbl_8047A60C;
extern u8 lbl_8047A610;
extern u8 lbl_8047C098;
extern u8 lbl_8047C09C;
extern u8 lbl_8047C0A0;
extern u8 lbl_8047C0A4;
extern u8 lbl_8047C0A8;
extern u8 lbl_8047C0AC;
extern u8 lbl_8047C0B0;
extern u8 lbl_8047C0B8;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268780[];
extern u8 lbl_802EF0A8[];
extern u8 lbl_80314F98[];
extern u8 lbl_803B6E08[];
extern u8 lbl_803B6E18[];
extern u8 lbl_803B6E40[];
extern u8 lbl_803D6E40[];

/* ===== Forward declarations ===== */
s32 fn_80072A00(void);
void fn_80072C74(void);
s32 fn_80072D58(void);
s32 fn_80073034(void);
s32 fn_800730F8(void);
s32 fn_800733D0(void);
void fn_80073690(void);
s32 fn_80073700(void);
s32 fn_80073990(void);
s32 fn_80073A44(void);
s32 fn_80073C38(void);
s32 fn_80073E84(void);
s32 fn_80073E8C(void);
void fn_80074324(void);
s32 fn_80074360(void);
s32 fn_800745B4(void);
s32 fn_8007480C(void);
s32 fn_80075390(void);
void fn_800753D0(void);
s32 fn_80075518(void);
s32 fn_80075638(void);
s32 fn_8007565C(void);
s32 fn_800756C8(void);
void fn_800757F0(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80072A00 | size: 0x274 */
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80072A00(void) {
    extern void fn_80073C38();
    u8 sp[0x50];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    /* stmw r18, 0x18(r1) */;
    r27 = r3;
    r29 = r27 + 0x1;
    r4 = 0x2;
    r3 = r29;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r27;
    fn_80073C38();
    if ((s32)r3 == (s32)0x0) goto L_80072A3C;
    r28 = r3;
    goto L_80072C50;
L_80072A3C: ;
    r0 = 0x60;
    r3 = r27;
    *(u32*)(sp + 0x14) = r0;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    r28 = r3;
    if ((s32)r28 == (s32)0x0) goto L_80072A68;
    r28 = 0xb;
    goto L_80072C50;
L_80072A68: ;
    r3 = (0x8000 << 16);
    r0 = *(u32*)((u8*)r3 + 0xF8);
    r0 = (u32)r0 >> 2;
    r20 = r0 * 0x3;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r27 << 3;
    r21 = r3;
    r0 = (u32)&lbl_803B6E18;
    r22 = r27 << 2;
    r30 = r0 + r6;
    r23 = (u32)&lbl_803B6E08;
    r31 = r30 + 0x4;
    r3 = (0x1062 << 16);
    r25 = (0x8000 << 16);
    r24 = r3 + 0x4dd3;
L_80072AAC: ;
    OSGetTick();
    r0 = r3 - r21;
    if ((u32)r0 <= (u32)r20) goto L_80072AC4;
    r28 = 0x10;
    goto L_80072C50;
L_80072AC4: ;
    r0 = *(u32*)((u8*)r25 + 0xF8);
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r24 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r19 = r0 * 0x5;
    OSGetTick();
    r26 = r3;
L_80072AE0: ;
    OSGetTick();
    r4 = r3 - r26;
    r3 = r27;
    r0 = r4 ^ r19;
    r0 = __cntlzw(r0);
    r0 = r4 << r0;
    r18 = (u32)r0 >> 31;
    fn_80073C38();
    if ((s32)r3 != (s32)0x1) goto L_80072B10;
    if ((s32)r18 == (s32)0x0) goto L_80072AE0;
L_80072B10: ;
    if ((s32)r3 == (s32)0x0) goto L_80072B20;
    r18 = r3;
    goto L_80072C44;
L_80072B20: ;
    r0 = 0xaa;
    r3 = r27;
    *(u32*)(sp + 0xC) = r0;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == (s32)0x0) goto L_80072B48;
    r18 = 0xb;
    goto L_80072C1C;
L_80072B48: ;
    r4 = (0x8000 << 16);
    r3 = (0x1062 << 16);
    r0 = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    r0 = (u32)r0 >> 2;
    r0 = (u32)((u64)r3 * (u64)r0 >> 32);
    r0 = (u32)r0 >> 6;
    r19 = r0 * 0x64;
    OSGetTick();
    r26 = r3;
L_80072B70: ;
    OSGetTick();
    r0 = r3 - r26;
    if ((u32)r0 <= (u32)r19) goto L_80072B88;
    r3 = 0x1;
    goto L_80072C08;
L_80072B88: ;
    r3 = r27;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == (s32)0x0) goto L_80072BA4;
    r3 = 0x2;
    goto L_80072C08;
L_80072BA4: ;
    r0 = *(u8*)(sp + 0x8);
    r0 = r0 & 0xa;
    if ((s32)r0 == (s32)0x8) goto L_80072BE4;
    r12 = *(u32*)((u8*)r30 + 0x0);
    if ((u32)r12 == (u32)0x0) goto L_80072BD0;
    r3 = r27;
    r4 = *(u32*)((u8*)r31 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80072BD0: ;
    r0 = *(u32*)(r23 + r22);
    if ((s32)r0 == (s32)0x0) goto L_80072B70;
    r3 = 0x3e8;
    goto L_80072C08;
L_80072BE4: ;
    r3 = r27;
    r4 = r1 + 0x10;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == (s32)0x0) goto L_80072C04;
    r3 = 0x3;
    goto L_80072C08;
L_80072C04: ;
    r3 = 0x0;
L_80072C08: ;
    if ((s32)r3 == (s32)0x0) goto L_80072C18;
    r18 = r3 + 0xb;
    goto L_80072C1C;
L_80072C18: ;
    r18 = 0x0;
L_80072C1C: ;
    if ((s32)r18 == (s32)0x0) goto L_80072C28;
    goto L_80072C44;
L_80072C28: ;
    r0 = *(u32*)(sp + 0x10);
    r0 = (u32)r0 >> 24;
    if ((u32)r0 == (u32)0xaa) goto L_80072C40;
    r18 = 0xf;
    goto L_80072C44;
L_80072C40: ;
    r18 = 0x0;
L_80072C44: ;
    ((void(*)(void))fn_800F0308)();
    if ((s32)r18 != (s32)0x0) goto L_80072AAC;
L_80072C50: ;
    r3 = r29;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r28;
    /* lmw r18, 0x18(r1) */;
    return;
}

/* 0x80072C74 | size: 0xE4 */
void fn_80072C74(void) {
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_80072CAC;
    r31 = 0x1;
    goto L_80072D24;
L_80072CAC:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072CC8;
    r31 = 0x2;
    goto L_80072D24;
L_80072CC8:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_80072CF8;
    tmp = 0x11;
    r3 = r29;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F648)();
    r31 = -0x1;
    goto L_80072D24;
L_80072CF8:
    r3 = r29;
    r4 = r1 + 0x10;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80072D18;
    r31 = 0x3;
    goto L_80072D24;
L_80072D18:
    r31 = 0x0;
    *(u32*)((u8*)r30 + 0x0) = tmp;
L_80072D24:
    if ((s32)r31 < 0) goto L_80072D38;
    r3 = r29 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
L_80072D38:
    r3 = r31;
    return;
}

/* 0x80072D58 | size: 0x2DC */
s32 fn_80072D58(void) {
    extern void fn_80073C38();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r15 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r21 = r3;
    r22 = r4;
    r23 = r5;
    r26 = r21 + 0x1;
    r4 = 0x2;
    r3 = r26;
    ((void(*)(void))fn_8008ABE4)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r21 << 3;
    r29 = r3;
    tmp = (u32)&lbl_803B6E18;
    r30 = r21 << 2;
    r27 = tmp + r6;
    r31 = (u32)&lbl_803B6E08;
    r28 = r27 + 0x4;
L_80072DCC:
    OSGetTick();
    r4 = r3 - r29;
    r3 = r21;
    tmp = r4 ^ r25;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r24 = (u32)tmp >> 31;
    fn_80073C38();
    if ((s32)r3 == 0) goto L_80072DFC;
    r15 = r3;
    goto L_80072FF0;
L_80072DFC:
    tmp = 0x66;
    r3 = r21;
    *(u32*)(sp + 0x10) = tmp;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80072E24;
    r15 = 0xb;
    goto L_80072EF8;
L_80072E24:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r16 = tmp * 0x64;
    OSGetTick();
    r18 = r3;
L_80072E4C:
    OSGetTick();
    tmp = r3 - r18;
    if (tmp <= r16) goto L_80072E64;
    r3 = 0x1;
    goto L_80072EE4;
L_80072E64:
    r3 = r21;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072E80;
    r3 = 0x2;
    goto L_80072EE4;
L_80072E80:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80072EC0;
    r12 = *(u32*)((u8*)r27 + 0x0);
    if (r12 == 0) goto L_80072EAC;
    r3 = r21;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80072EAC:
    tmp = *(u32*)(r31 + r30);
    if ((s32)tmp == 0) goto L_80072E4C;
    r3 = 0x3e8;
    goto L_80072EE4;
L_80072EC0:
    r3 = r21;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80072EE0;
    r3 = 0x3;
    goto L_80072EE4;
L_80072EE0:
    r3 = 0x0;
L_80072EE4:
    if ((s32)r3 == 0) goto L_80072EF4;
    r15 = r3 + 0xb;
    goto L_80072EF8;
L_80072EF4:
    r15 = 0x0;
L_80072EF8:
    if ((s32)r15 == 0) goto L_80072F04;
    goto L_80072FF0;
L_80072F04:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x66) goto L_80072F1C;
    r15 = 0xf;
    goto L_80072FF0;
L_80072F1C:
    r17 = 0x0;
    r3 = 0x10620000;
    r15 = r22;
    r18 = r3 + 0x4dd3;
    r19 = 0x80000000;
    goto L_80072FE4;
L_80072F34:
    tmp = *(u32*)((u8*)r15 + 0x0);
    r3 = r21;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80072F5C;
    r15 = 0x10;
    goto L_80072FF0;
L_80072F5C:
    tmp = *(u32*)((u8*)r19 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r18 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r16 = tmp * 0x64;
    OSGetTick();
    r20 = r3;
L_80072F78:
    OSGetTick();
    tmp = r3 - r20;
    if (tmp <= r16) goto L_80072F90;
    r15 = 0x11;
    goto L_80072FF0;
L_80072F90:
    r3 = r21;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80072FAC;
    r15 = 0x12;
    goto L_80072FF0;
L_80072FAC:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000002;
    if ((s32)tmp != 0) goto L_80072F78;
    tmp = r17 << 26;
    r3 = (u32)r17 >> 31;
    tmp = tmp - r3;
    /* rotlwi tmp, tmp, 6 */;
    tmp = tmp + r3;
    if ((s32)tmp != 0) goto L_80072FDC;
    ((void(*)(void))fn_800F0308)();
L_80072FDC:
    r17 = r17 + 0x4;
    r15 = r15 + 0x4;
L_80072FE4:
    if ((s32)r17 < (s32)r23) goto L_80072F34;
    r15 = 0x0;
L_80072FF0:
    if ((s32)r15 != 1) goto L_80073000;
    if ((s32)r24 == 0) goto L_80072DCC;
L_80073000:
    r3 = r26;
    if ((s32)r15 == 0) goto L_80073014;
    r4 = 0x1;
    goto L_80073018;
L_80073014:
    r4 = 0x3;
L_80073018:
    ((void(*)(void))fn_8008ABE4)();
    r3 = r15;
    return;
}

/* 0x80073034 | size: 0xC4 */
s32 fn_80073034(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r4;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_80073068;
    r3 = 0x1;
    goto L_800730E0;
L_80073068:
    r3 = r30;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073084;
    r3 = 0x2;
    goto L_800730E0;
L_80073084:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_800730B4;
    tmp = 0x11;
    r3 = r30;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F648)();
    r3 = -0x1;
    goto L_800730E0;
L_800730B4:
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800730D4;
    r3 = 0x3;
    goto L_800730E0;
L_800730D4:
    r3 = 0x0;
    *(u32*)((u8*)r31 + 0x0) = tmp;
L_800730E0:
    return;
}

/* 0x800730F8 | size: 0x2D8 */
s32 fn_800730F8(void) {
    extern void fn_80073C38();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r12 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r22 = r3;
    r23 = r4;
    r25 = r22 + 0x1;
    r4 = 0x2;
    r3 = r25;
    ((void(*)(void))fn_8008ABE4)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r24 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)r23 >> 24;
    tmp = ((r23 << 24) | ((u32)r23 >> 8)) & 0x0000FF00;
    r7 = r22 << 3;
    r5 = (u32)&lbl_803B6E18;
    r6 = (u32)&lbl_803B6E08;
    r26 = r5 + r7;
    r5 = ((r23 << 8) | ((u32)r23 >> 24)) & 0x00FF0000;
    tmp = r4 | tmp;
    r4 = r23 << 24;
    tmp = r5 | tmp;
    r29 = r3;
    r28 = r26 + 0x4;
    r30 = r22 << 2;
    r31 = (u32)&lbl_803B6E08;
    r27 = r4 | tmp;
L_80073184:
    OSGetTick();
    r4 = r3 - r29;
    r3 = r22;
    tmp = r4 ^ r24;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r23 = (u32)tmp >> 31;
    fn_80073C38();
    if ((s32)r3 == 0) goto L_800731B4;
    r20 = r3;
    goto L_8007338C;
L_800731B4:
    tmp = 0x77;
    r3 = r22;
    *(u32*)(sp + 0x14) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800731E0;
    r20 = 0xb;
    goto L_800732B4;
L_800731E0:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r20 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_80073208:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r20) goto L_80073220;
    r3 = 0x1;
    goto L_800732A0;
L_80073220:
    r3 = r22;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_8007323C;
    r3 = 0x2;
    goto L_800732A0;
L_8007323C:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_8007327C;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 == 0) goto L_80073268;
    r3 = r22;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073268:
    tmp = *(u32*)(r31 + r30);
    if ((s32)tmp == 0) goto L_80073208;
    r3 = 0x3e8;
    goto L_800732A0;
L_8007327C:
    r3 = r22;
    r4 = r1 + 0x10;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_8007329C;
    r3 = 0x3;
    goto L_800732A0;
L_8007329C:
    r3 = 0x0;
L_800732A0:
    if ((s32)r3 == 0) goto L_800732B0;
    r20 = r3 + 0xb;
    goto L_800732B4;
L_800732B0:
    r20 = 0x0;
L_800732B4:
    if ((s32)r20 == 0) goto L_800732C0;
    goto L_8007338C;
L_800732C0:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x77) goto L_800732D8;
    r20 = 0xf;
    goto L_8007338C;
L_800732D8:
    r3 = r22;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r20 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_80073314:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r20) goto L_8007332C;
    r20 = 0x10;
    goto L_8007338C;
L_8007332C:
    r3 = r22;
    r4 = r1 + 0xa;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073348;
    r20 = 0x11;
    goto L_8007338C;
L_80073348:
    tmp = *(u8*)(sp + 0xA);
    tmp = tmp & 0x00000002;
    if ((s32)tmp == 0) goto L_80073388;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 == 0) goto L_80073374;
    r3 = r22;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073374:
    tmp = *(u32*)(r31 + r30);
    if ((s32)tmp == 0) goto L_80073314;
    r20 = 0x3e8;
    goto L_8007338C;
L_80073388:
    r20 = 0x0;
L_8007338C:
    if ((s32)r20 != 1) goto L_8007339C;
    if ((s32)r23 == 0) goto L_80073184;
L_8007339C:
    r3 = r25;
    if ((s32)r20 == 0) goto L_800733B0;
    r4 = 0x1;
    goto L_800733B4;
L_800733B0:
    r4 = 0x3;
L_800733B4:
    ((void(*)(void))fn_8008ABE4)();
    r3 = r20;
    return;
}

/* 0x800733D0 | size: 0x2C0 */
s32 fn_800733D0(void) {
    extern void fn_80073C38();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r26 = r3;
    r28 = r4;
    r27 = r26 + 0x1;
    r4 = 0x2;
    r3 = r27;
    ((void(*)(void))fn_8008ABE4)();
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r30 = tmp * 0x5;
    OSGetTick();
    r25 = r3;
L_80073420:
    OSGetTick();
    r4 = r3 - r25;
    r3 = r26;
    tmp = r4 ^ r30;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r29 = (u32)tmp >> 31;
    fn_80073C38();
    if ((s32)r3 != 1) goto L_80073450;
    if ((s32)r29 == 0) goto L_80073420;
L_80073450:
    if ((s32)r3 == 0) goto L_80073460;
    r25 = r3;
    goto L_8007366C;
L_80073460:
    tmp = 0x88;
    r3 = r26;
    *(u32*)(sp + 0x10) = tmp;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80073488;
    r25 = 0xb;
    goto L_8007357C;
L_80073488:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r24 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r26 << 3;
    r30 = r3;
    tmp = (u32)&lbl_803B6E18;
    r29 = r26 << 2;
    r23 = tmp + r6;
    r25 = (u32)&lbl_803B6E08;
    r31 = r23 + 0x4;
L_800734D0:
    OSGetTick();
    tmp = r3 - r30;
    if (tmp <= r24) goto L_800734E8;
    r3 = 0x1;
    goto L_80073568;
L_800734E8:
    r3 = r26;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073504;
    r3 = 0x2;
    goto L_80073568;
L_80073504:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80073544;
    r12 = *(u32*)((u8*)r23 + 0x0);
    if (r12 == 0) goto L_80073530;
    r3 = r26;
    r4 = *(u32*)((u8*)r31 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073530:
    tmp = *(u32*)(r25 + r29);
    if ((s32)tmp == 0) goto L_800734D0;
    r3 = 0x3e8;
    goto L_80073568;
L_80073544:
    r3 = r26;
    r4 = r1 + 0x14;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80073564;
    r3 = 0x3;
    goto L_80073568;
L_80073564:
    r3 = 0x0;
L_80073568:
    if ((s32)r3 == 0) goto L_80073578;
    r25 = r3 + 0xb;
    goto L_8007357C;
L_80073578:
    r25 = 0x0;
L_8007357C:
    if ((s32)r25 == 0) goto L_80073588;
    goto L_8007366C;
L_80073588:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x88) goto L_800735A0;
    r25 = 0xf;
    goto L_8007366C;
L_800735A0:
    r29 = 0x0;
    r3 = 0x10620000;
    r31 = 0x80000000;
    r30 = r3 + 0x4dd3;
L_800735B0:
    tmp = *(u32*)((u8*)r28 + 0x0);
    r3 = r26;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800735D8;
    r25 = 0x10;
    goto L_8007366C;
L_800735D8:
    tmp = *(u32*)((u8*)r31 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r30 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r24 = tmp * 0x64;
    OSGetTick();
    r25 = r3;
L_800735F4:
    OSGetTick();
    tmp = r3 - r25;
    if (tmp <= r24) goto L_8007360C;
    r25 = 0x11;
    goto L_8007366C;
L_8007360C:
    r3 = r26;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073628;
    r25 = 0x12;
    goto L_8007366C;
L_80073628:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000002;
    if ((s32)tmp != 0) goto L_800735F4;
    tmp = r29 << 26;
    r3 = (u32)r29 >> 31;
    tmp = tmp - r3;
    /* rotlwi tmp, tmp, 6 */;
    tmp = tmp + r3;
    if ((s32)tmp != 0) goto L_80073658;
    ((void(*)(void))fn_800F0308)();
L_80073658:
    r28 = r28 + 0x4;
    r29 = r29 + 0x4;
    if ((s32)r29 < 0x780) goto L_800735B0;
    r25 = 0x0;
L_8007366C:
    r3 = r27;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r25;
    return;
}

/* 0x80073690 | size: 0x70 */
void fn_80073690(void) {
    extern void fn_80073700();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r31 = r29 + 0x1;
    r4 = 0x2;
    r3 = r31;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r29;
    r4 = r30;
    fn_80073700();
    tmp = r3;
    r3 = r31;
    r31 = tmp;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r31;
    return;
}

/* 0x80073700 | size: 0x290 */
s32 fn_80073700(void) {
    extern void fn_80073C38();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r24 = r3;
    r30 = r4;
    fn_80073C38();
    if ((s32)r3 == 0) goto L_80073728;
    goto L_8007397C;
L_80073728:
    tmp = 0x99;
    r3 = r24;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80073750;
    r3 = 0xb;
    goto L_80073844;
L_80073750:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r24 << 3;
    r25 = r3;
    tmp = (u32)&lbl_803B6E18;
    r23 = r24 << 2;
    r28 = tmp + r6;
    r22 = (u32)&lbl_803B6E08;
    r27 = r28 + 0x4;
L_80073798:
    OSGetTick();
    tmp = r3 - r25;
    if (tmp <= r26) goto L_800737B0;
    r3 = 0x1;
    goto L_80073830;
L_800737B0:
    r3 = r24;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800737CC;
    r3 = 0x2;
    goto L_80073830;
L_800737CC:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_8007380C;
    r12 = *(u32*)((u8*)r28 + 0x0);
    if (r12 == 0) goto L_800737F8;
    r3 = r24;
    r4 = *(u32*)((u8*)r27 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800737F8:
    tmp = *(u32*)(r22 + r23);
    if ((s32)tmp == 0) goto L_80073798;
    r3 = 0x3e8;
    goto L_80073830;
L_8007380C:
    r3 = r24;
    r4 = r1 + 0x10;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_8007382C;
    r3 = 0x3;
    goto L_80073830;
L_8007382C:
    r3 = 0x0;
L_80073830:
    if ((s32)r3 == 0) goto L_80073840;
    r3 = r3 + 0xb;
    goto L_80073844;
L_80073840:
    r3 = 0x0;
L_80073844:
    if ((s32)r3 == 0) goto L_80073850;
    goto L_8007397C;
L_80073850:
    tmp = (u32)tmp >> 24;
    if (tmp == 0x99) goto L_80073868;
    r3 = 0xf;
    goto L_8007397C;
L_80073868:
    r4 = (u32)&lbl_803B6E18;
    r3 = (u32)&lbl_803B6E08;
    r5 = r24 << 3;
    r28 = r24 << 2;
    tmp = (u32)&lbl_803B6E18;
    r29 = (u32)&lbl_803B6E08;
    r26 = tmp + r5;
    r25 = 0x0;
    r27 = r26 + 0x4;
    r3 = 0x10620000;
    r23 = r30;
    r30 = r3 + 0x4dd3;
    r31 = 0x80000000;
L_8007389C:
    tmp = *(u32*)((u8*)r31 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r30 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r21 = tmp * 0x64;
    OSGetTick();
    r22 = r3;
L_800738B8:
    OSGetTick();
    tmp = r3 - r22;
    if (tmp <= r21) goto L_800738D0;
    r3 = 0x10;
    goto L_8007397C;
L_800738D0:
    r3 = r24;
    r4 = r1 + 0xa;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800738EC;
    r3 = 0x11;
    goto L_8007397C;
L_800738EC:
    tmp = *(u8*)(sp + 0xA);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_8007392C;
    r12 = *(u32*)((u8*)r26 + 0x0);
    if (r12 == 0) goto L_80073918;
    r3 = r24;
    r4 = *(u32*)((u8*)r27 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073918:
    tmp = *(u32*)(r29 + r28);
    if ((s32)tmp == 0) goto L_800738B8;
    r3 = 0x3e8;
    goto L_8007397C;
L_8007392C:
    r3 = r24;
    r4 = r1 + 0x10;
    r5 = r1 + 0xa;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_8007394C;
    r3 = 0x12;
    goto L_8007397C;
L_8007394C:
    *(u32*)((u8*)r23 + 0x0) = tmp;
    tmp = *(u32*)(r29 + r28);
    if ((s32)tmp == 0) goto L_80073968;
    r3 = 0x3e8;
    goto L_8007397C;
L_80073968:
    r25 = r25 + 0x4;
    r23 = r23 + 0x4;
    if ((s32)r25 < 0x278) goto L_8007389C;
    r3 = 0x0;
L_8007397C:
    return;
}

/* 0x80073990 | size: 0xB4 */
s32 fn_80073990(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;

    r29 = r3;
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    r30 = (u32)tmp >> 6;
    OSGetTick();
    r31 = r3;
L_800739D0:
    OSGetTick();
    tmp = r3 - r31;
    if (tmp < r30) goto L_800739D0;
    r3 = r29;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_800739FC;
    r3 = 0x1;
    goto L_80073A28;
L_800739FC:
    tmp = 0x11;
    r3 = r29;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80073A24;
    r3 = 0x2;
    goto L_80073A28;
L_80073A24:
    r3 = 0x0;
L_80073A28:
    return;
}

/* 0x80073A44 | size: 0x1F4 */
s32 fn_80073A44(void) {
    extern void fn_80073C38();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r30 = r3;
    r31 = r4;
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r24 = tmp * 0x5;
    OSGetTick();
    r27 = r3;
L_80073A84:
    OSGetTick();
    r4 = r3 - r27;
    r3 = r30;
    tmp = r4 ^ r24;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r25 = (u32)tmp >> 31;
    fn_80073C38();
    if ((s32)r3 != 1) goto L_80073AB4;
    if ((s32)r25 == 0) goto L_80073A84;
L_80073AB4:
    if ((s32)r3 == 0) goto L_80073AC0;
    goto L_80073C24;
L_80073AC0:
    tmp = 0xaa;
    r3 = r30;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80073AE8;
    r3 = 0xb;
    goto L_80073BDC;
L_80073AE8:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r26 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r30 << 3;
    r27 = r3;
    tmp = (u32)&lbl_803B6E18;
    r28 = r30 << 2;
    r24 = tmp + r6;
    r29 = (u32)&lbl_803B6E08;
    r25 = r24 + 0x4;
L_80073B30:
    OSGetTick();
    tmp = r3 - r27;
    if (tmp <= r26) goto L_80073B48;
    r3 = 0x1;
    goto L_80073BC8;
L_80073B48:
    r3 = r30;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073B64;
    r3 = 0x2;
    goto L_80073BC8;
L_80073B64:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80073BA4;
    r12 = *(u32*)((u8*)r24 + 0x0);
    if (r12 == 0) goto L_80073B90;
    r3 = r30;
    r4 = *(u32*)((u8*)r25 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073B90:
    tmp = *(u32*)(r29 + r28);
    if ((s32)tmp == 0) goto L_80073B30;
    r3 = 0x3e8;
    goto L_80073BC8;
L_80073BA4:
    r3 = r30;
    r4 = r1 + 0x10;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80073BC4;
    r3 = 0x3;
    goto L_80073BC8;
L_80073BC4:
    r3 = 0x0;
L_80073BC8:
    if ((s32)r3 == 0) goto L_80073BD8;
    r3 = r3 + 0xb;
    goto L_80073BDC;
L_80073BD8:
    r3 = 0x0;
L_80073BDC:
    if ((s32)r3 == 0) goto L_80073BE8;
    goto L_80073C24;
L_80073BE8:
    r5 = (u32)r4 >> 24;
    if (r5 == 0xaa) goto L_80073C00;
    r3 = 0xf;
    goto L_80073C24;
L_80073C00:
    tmp = ((r4 << 24) | ((u32)r4 >> 8)) & 0x0000FF00;
    r3 = ((r4 << 8) | ((u32)r4 >> 24)) & 0x00FF0000;
    tmp = r5 | tmp;
    r4 = r4 << 24;
    tmp = r3 | tmp;
    r3 = 0x0;
    tmp = r4 | tmp;
    tmp = (u32)tmp >> 16;
    *(u16*)((u8*)r31 + 0x0) = tmp;
L_80073C24:
    return;
}

/* 0x80073C38 | size: 0x24C */
s32 fn_80073C38(void) {
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_80073C64;
    r3 = 0x1;
    goto L_80073E70;
L_80073C64:
    r3 = r29;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F484)();
    if ((s32)r3 == 0) goto L_80073C80;
    r3 = 0x2;
    goto L_80073E70;
L_80073C80:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x5;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r29 << 3;
    r28 = r3;
    tmp = (u32)&lbl_803B6E18;
    r27 = r29 << 2;
    r31 = tmp + r6;
    r26 = (u32)&lbl_803B6E08;
    r30 = r31 + 0x4;
L_80073CC8:
    OSGetTick();
    tmp = r3 - r28;
    if (tmp <= r25) goto L_80073CE0;
    r3 = 0x1;
    goto L_80073D60;
L_80073CE0:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073CFC;
    r3 = 0x2;
    goto L_80073D60;
L_80073CFC:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80073D3C;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 == 0) goto L_80073D28;
    r3 = r29;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073D28:
    tmp = *(u32*)(r26 + r27);
    if ((s32)tmp == 0) goto L_80073CC8;
    r3 = 0x3e8;
    goto L_80073D60;
L_80073D3C:
    r3 = r29;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80073D5C;
    r3 = 0x3;
    goto L_80073D60;
L_80073D5C:
    r3 = 0x0;
L_80073D60:
    if ((s32)r3 == 0) goto L_80073D70;
    r3 = r3 + 0x2;
    goto L_80073E70;
L_80073D70:
    tmp = *(u32*)&lbl_8047A60C;
    if (r3 == tmp) goto L_80073D88;
    r3 = 0x6;
    goto L_80073E70;
L_80073D88:
    r3 = r29;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073DA4;
    r3 = 0x7;
    goto L_80073E70;
L_80073DA4:
    r3 = r29;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80073DC4;
    r3 = 0x8;
    goto L_80073E70;
L_80073DC4:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x64;
    OSGetTick();
    r4 = (u32)&lbl_803B6E08;
    r26 = r3;
    r27 = r29 << 2;
    r28 = (u32)&lbl_803B6E08;
L_80073DF8:
    OSGetTick();
    tmp = r3 - r26;
    if (tmp <= r25) goto L_80073E10;
    r3 = 0x9;
    goto L_80073E70;
L_80073E10:
    r3 = r29;
    r4 = r1 + 0x9;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80073E2C;
    r3 = 0xa;
    goto L_80073E70;
L_80073E2C:
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000002;
    if ((s32)tmp == 0) goto L_80073E6C;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 == 0) goto L_80073E58;
    r3 = r29;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80073E58:
    tmp = *(u32*)(r28 + r27);
    if ((s32)tmp == 0) goto L_80073DF8;
    r3 = 0x3e8;
    goto L_80073E70;
L_80073E6C:
    r3 = 0x0;
L_80073E70:
    return;
}

/* 0x80073E84 | size: 0x8 */
s32 fn_80073E84(void) {
    return 0x1;
}

/* 0x80073E8C | size: 0x498 */
s32 fn_80073E8C(void) {
    u8 sp[0x90];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    r29 = r4;
    r3 = (u32)&lbl_80268780;
    r31 = (u32)&lbl_80268780;
    ((void(*)(void))fn_800A7BCC)();
    tmp = r3;
    r3 = (u32)&lbl_8047A60C;
    r4 = tmp;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    tmp = *(u32*)&lbl_8047A60C;
    tmp = tmp | (0x20 << 16);
    tmp = tmp | 0x2020;
    *(u32*)&lbl_8047A60C = tmp;
    ((void(*)(void))fn_8025F350)();
    r3 = r30;
    r4 = r1 + 0x44;
    ((void(*)(void))fn_800A501C)();
    if ((s32)r3 != 0) goto L_80073F08;
    r3 = r31 + 0x0;
    r5 = r31 + 0x10;
    r4 = 0x1d6;
    ((void(*)(void))fn_800060F0)();
L_80073F08:
    tmp = r3 + 0x7;
    /* clrrwi r3, tmp, 3 */;
    *(u32*)&lbl_8047A608 = r3;
    if (r3 == 0) goto L_80073F2C;
    tmp = 0x20000;
    if (r3 <= tmp) goto L_80073F40;
L_80073F2C:
    r3 = r31 + 0x0;
    r5 = r31 + 0x28;
    r4 = 0x1dc;
    ((void(*)(void))fn_800060F0)();
L_80073F40:
    r5 = *(u32*)&lbl_8047A608;
    r3 = (u32)&lbl_803D6E40;
    r4 = (u32)&lbl_803D6E40;
    r6 = 0x0;
    tmp = r5 + 0x1f;
    r3 = r1 + 0x44;
    /* clrrwi r5, tmp, 5 */;
    r7 = 0x2;
    *(u32*)&lbl_8047A608 = r5;
    ((void(*)(void))fn_800A541C)();
    if ((s32)r3 >= 0) goto L_80073F84;
    r3 = r31 + 0x0;
    r5 = r31 + 0x4c;
    r4 = 0x1e1;
    ((void(*)(void))fn_800060F0)();
L_80073F84:
    r3 = r1 + 0x44;
    ((void(*)(void))fn_800A50E4)();
    r30 = (u32)&lbl_8047A60C;
    r3 = (u32)&lbl_803D6E40;
    r8 = 0xa0;
    r6 = *(u8*)((u8*)r30 + 0x0);
    r5 = (u32)&lbl_803D6E40;
    r4 = *(u8*)((u8*)r30 + 0x1);
    r3 = *(u8*)((u8*)r30 + 0x2);
    tmp = *(u8*)((u8*)r30 + 0x3);
    r7 = 0xe7;
    *(u8*)((u8*)r5 + 0xAC) = r6;
    *(u8*)((u8*)r5 + 0xAD) = r4;
    *(u8*)((u8*)r5 + 0xAE) = r3;
    *(u8*)((u8*)r5 + 0xAF) = tmp;
    if ((s32)r8 >= 0xbd) goto L_800740E0;
    r4 = *(u8*)((u8*)r5 + 0xA0);
    r6 = r5 + 0xa7;
    r3 = *(u8*)((u8*)r5 + 0xA1);
    r8 = 0xbc;
    r7 = 0xe7 - r4;
    tmp = *(u8*)((u8*)r5 + 0xA2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r5 + 0xA3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r5 + 0xA4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r5 + 0xA5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r5 + 0xA6);
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r6 = r6 + 0x7;
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r6 = r6 + 0x7;
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r7 = r7 - r3;
    r7 = r7 - tmp;
    r3 = r5 + 0xbc;
    tmp = 0xbd - r8;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r8 >= 0xbd) goto L_800740E0;
L_800740CC:
    tmp = *(u8*)((u8*)r3 + 0x0);
    r3 = r3 + 0x1;
    r8 = r8 + 0x1;
    r7 = r7 - tmp;
    if (--ctr != 0) goto L_800740CC;
L_800740E0:
    r3 = (u32)&lbl_803D6E40;
    tmp = r7 & 0xFF;
    r3 = (u32)&lbl_803D6E40;
    *(u8*)(r3 + r8) = tmp;
    if (r29 == 0) goto L_80074304;
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_800A501C)();
    if ((s32)r3 != 0) goto L_80074120;
    r3 = r31 + 0x0;
    r5 = r31 + 0x10;
    r4 = 0x1d6;
    ((void(*)(void))fn_800060F0)();
L_80074120:
    tmp = r3 + 0x7;
    /* clrrwi r3, tmp, 3 */;
    *(u32*)&lbl_8047A604 = r3;
    if (r3 == 0) goto L_80074144;
    tmp = 0x20000;
    if (r3 <= tmp) goto L_80074158;
L_80074144:
    r3 = r31 + 0x0;
    r5 = r31 + 0x28;
    r4 = 0x1dc;
    ((void(*)(void))fn_800060F0)();
L_80074158:
    r5 = *(u32*)&lbl_8047A604;
    r3 = (u32)&lbl_803B6E40;
    r4 = (u32)&lbl_803B6E40;
    r6 = 0x0;
    tmp = r5 + 0x1f;
    r3 = r1 + 0x8;
    /* clrrwi r5, tmp, 5 */;
    r7 = 0x2;
    *(u32*)&lbl_8047A604 = r5;
    ((void(*)(void))fn_800A541C)();
    if ((s32)r3 >= 0) goto L_8007419C;
    r3 = r31 + 0x0;
    r5 = r31 + 0x4c;
    r4 = 0x1e1;
    ((void(*)(void))fn_800060F0)();
L_8007419C:
    r3 = r1 + 0x8;
    ((void(*)(void))fn_800A50E4)();
    r3 = (u32)&lbl_803B6E40;
    r8 = 0xa0;
    r6 = *(u8*)((u8*)r30 + 0x0);
    r5 = (u32)&lbl_803B6E40;
    r4 = *(u8*)((u8*)r30 + 0x1);
    r3 = *(u8*)((u8*)r30 + 0x2);
    r7 = 0xe7;
    tmp = *(u8*)((u8*)r30 + 0x3);
    *(u8*)((u8*)r5 + 0xAC) = r6;
    *(u8*)((u8*)r5 + 0xAD) = r4;
    *(u8*)((u8*)r5 + 0xAE) = r3;
    *(u8*)((u8*)r5 + 0xAF) = tmp;
    if ((s32)r8 >= 0xbd) goto L_800742F4;
    r4 = *(u8*)((u8*)r5 + 0xA0);
    r6 = r5 + 0xa7;
    r3 = *(u8*)((u8*)r5 + 0xA1);
    r8 = 0xbc;
    r7 = 0xe7 - r4;
    tmp = *(u8*)((u8*)r5 + 0xA2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r5 + 0xA3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r5 + 0xA4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r5 + 0xA5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r5 + 0xA6);
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r6 = r6 + 0x7;
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r6 = r6 + 0x7;
    r7 = r7 - r3;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r7 = r7 - tmp;
    r3 = *(u8*)((u8*)r6 + 0x1);
    r7 = r7 - r4;
    tmp = *(u8*)((u8*)r6 + 0x2);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x3);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x4);
    r7 = r7 - r3;
    r3 = *(u8*)((u8*)r6 + 0x5);
    r7 = r7 - tmp;
    tmp = *(u8*)((u8*)r6 + 0x6);
    r7 = r7 - r3;
    r7 = r7 - tmp;
    r3 = r5 + 0xbc;
    tmp = 0xbd - r8;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r8 >= 0xbd) goto L_800742F4;
L_800742E0:
    tmp = *(u8*)((u8*)r3 + 0x0);
    r3 = r3 + 0x1;
    r8 = r8 + 0x1;
    r7 = r7 - tmp;
    if (--ctr != 0) goto L_800742E0;
L_800742F4:
    r3 = (u32)&lbl_803B6E40;
    tmp = r7 & 0xFF;
    r3 = (u32)&lbl_803B6E40;
    *(u8*)(r3 + r8) = tmp;
L_80074304:
    r3 = 0x0;
    return;
}

/* 0x80074324 | size: 0x3C */
void fn_80074324(void) {
    extern void fn_80074360();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r31 = r3;
    r3 = r31 + 0x1;
    r4 = 0x0;
    ((void(*)(void))fn_8008ABE4)();
    r3 = r31;
    fn_80074360();
    return;
}

/* 0x80074360 | size: 0x254 */
s32 fn_80074360(void) {
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r29 = r3;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_8007438C;
    r3 = 0x1;
    goto L_800745A0;
L_8007438C:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F484)();
    if ((s32)r3 == 0) goto L_800743A8;
    r3 = 0x2;
    goto L_800745A0;
L_800743A8:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r29 << 3;
    r28 = r3;
    tmp = (u32)&lbl_803B6E18;
    r27 = r29 << 2;
    r31 = tmp + r6;
    r26 = (u32)&lbl_803B6E08;
    r30 = r31 + 0x4;
L_800743F0:
    OSGetTick();
    tmp = r3 - r28;
    if (tmp <= r25) goto L_80074408;
    r3 = 0x3;
    goto L_800745A0;
L_80074408:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074424;
    r3 = 0x4;
    goto L_800745A0;
L_80074424:
    tmp = *(u8*)(sp + 0x8);
    if (tmp == 0x18) goto L_80074460;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 == 0) goto L_8007444C;
    r3 = r29;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8007444C:
    tmp = *(u32*)(r26 + r27);
    if ((s32)tmp == 0) goto L_800743F0;
    r3 = 0x3e8;
    goto L_800745A0;
L_80074460:
    r3 = r29;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80074480;
    r3 = 0x5;
    goto L_800745A0;
L_80074480:
    /* subis tmp, r3, 0x4158 */;
    if (tmp == 0x5645) goto L_80074498;
    r3 = 0x6;
    goto L_800745A0;
L_80074498:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_800744C0;
    tmp = *(u8*)(sp + 0x8);
    if (tmp == 0x10) goto L_800744C0;
    r3 = 0x7;
    goto L_800745A0;
L_800744C0:
    r3 = r29;
    r4 = r1 + 0xc;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800744E0;
    r3 = 0x8;
    goto L_800745A0;
L_800744E0:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r25 = tmp * 0x64;
    OSGetTick();
    r4 = (u32)&lbl_803B6E08;
    r26 = r3;
    r27 = r29 << 2;
    r28 = (u32)&lbl_803B6E08;
L_80074514:
    OSGetTick();
    tmp = r3 - r26;
    if (tmp <= r25) goto L_8007452C;
    r3 = 0x9;
    goto L_800745A0;
L_8007452C:
    r3 = r29;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074548;
    r3 = 0xa;
    goto L_800745A0;
L_80074548:
    r3 = *(u8*)(sp + 0x8);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x10) goto L_80074560;
    r3 = 0xb;
    goto L_800745A0;
L_80074560:
    tmp = r3 & 0x00000002;
    if ((s32)tmp == 0) goto L_8007459C;
    r12 = *(u32*)((u8*)r31 + 0x0);
    if (r12 == 0) goto L_80074588;
    r3 = r29;
    r4 = *(u32*)((u8*)r30 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80074588:
    tmp = *(u32*)(r28 + r27);
    if ((s32)tmp == 0) goto L_80074514;
    r3 = 0x3e8;
    goto L_800745A0;
L_8007459C:
    r3 = 0x0;
L_800745A0:
    return;
}

/* 0x800745B4 | size: 0x258 */
s32 fn_800745B4(void) {
    extern void fn_80073C38();
    extern void fn_8007480C();
    u8 sp[0x50];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;

    r27 = r3;
    fn_8007480C();
    r29 = r3;
    if ((s32)r29 != 0) goto L_800747F4;
    r3 = 0x80000000;
    tmp = *(u32*)((u8*)r3 + 0xF8);
    tmp = (u32)tmp >> 2;
    r28 = tmp << 3;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r27 << 3;
    r21 = r3;
    tmp = (u32)&lbl_803B6E18;
    r22 = r27 << 2;
    r30 = tmp + r6;
    r23 = (u32)&lbl_803B6E08;
    r31 = r30 + 0x4;
    r3 = 0x10620000;
    r25 = 0x80000000;
    r24 = r3 + 0x4dd3;
L_8007461C:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r28) goto L_80074634;
    r3 = 0x16;
    goto L_800747F8;
L_80074634:
    r12 = *(u32*)((u8*)r30 + 0x0);
    if (r12 == 0) goto L_80074650;
    r3 = r27;
    r4 = *(u32*)((u8*)r31 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80074650:
    tmp = *(u32*)(r23 + r22);
    if ((s32)tmp == 0) goto L_80074664;
    r3 = 0x3e8;
    goto L_800747F8;
L_80074664:
    tmp = *(u32*)((u8*)r25 + 0xF8);
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r24 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r20 = tmp * 0x5;
    OSGetTick();
    r26 = r3;
L_80074680:
    OSGetTick();
    r4 = r3 - r26;
    r3 = r27;
    tmp = r4 ^ r20;
    tmp = __cntlzw(tmp);
    tmp = r4 << tmp;
    r19 = (u32)tmp >> 31;
    fn_80073C38();
    if ((s32)r3 != 1) goto L_800746B0;
    if ((s32)r19 == 0) goto L_80074680;
L_800746B0:
    if ((s32)r3 == 0) goto L_800746BC;
    goto L_800747E0;
L_800746BC:
    tmp = 0xaa;
    r3 = r27;
    *(u32*)(sp + 0xC) = tmp;
    r4 = r1 + 0xc;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800746E4;
    r3 = 0xb;
    goto L_800747B8;
L_800746E4:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r19 = tmp * 0x64;
    OSGetTick();
    r20 = r3;
L_8007470C:
    OSGetTick();
    tmp = r3 - r20;
    if (tmp <= r19) goto L_80074724;
    r3 = 0x1;
    goto L_800747A4;
L_80074724:
    r3 = r27;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074740;
    r3 = 0x2;
    goto L_800747A4;
L_80074740:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0xa;
    if ((s32)tmp == 8) goto L_80074780;
    r12 = *(u32*)((u8*)r30 + 0x0);
    if (r12 == 0) goto L_8007476C;
    r3 = r27;
    r4 = *(u32*)((u8*)r31 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_8007476C:
    tmp = *(u32*)(r23 + r22);
    if ((s32)tmp == 0) goto L_8007470C;
    r3 = 0x3e8;
    goto L_800747A4;
L_80074780:
    r3 = r27;
    r4 = r1 + 0x10;
    r5 = r1 + 0x9;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_800747A0;
    r3 = 0x3;
    goto L_800747A4;
L_800747A0:
    r3 = 0x0;
L_800747A4:
    if ((s32)r3 == 0) goto L_800747B4;
    r3 = r3 + 0xb;
    goto L_800747B8;
L_800747B4:
    r3 = 0x0;
L_800747B8:
    if ((s32)r3 == 0) goto L_800747C4;
    goto L_800747E0;
L_800747C4:
    tmp = (u32)tmp >> 24;
    if (tmp == 0xaa) goto L_800747DC;
    r3 = 0xf;
    goto L_800747E0;
L_800747DC:
    r3 = 0x0;
L_800747E0:
    if ((s32)r3 != 0) goto L_8007461C;
    r3 = r27 + 0x1;
    r4 = 0x1;
    ((void(*)(void))fn_8008ABE4)();
L_800747F4:
    r3 = r29;
L_800747F8:
    return;
}

/* 0x8007480C | size: 0xB84 */
s32 fn_8007480C(void) {
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
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
    f32 f8 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r3 = (u32)&lbl_80268780;
    r30 = (u32)&lbl_80268780;
    if ((s32)r4 == 0) goto L_80074844;
    r3 = (u32)&lbl_803B6E40;
    r18 = *(u32*)&lbl_8047A604;
    tmp = (u32)&lbl_803B6E40;
    r22 = tmp;
    goto L_80074854;
L_80074844:
    r3 = (u32)&lbl_803D6E40;
    r18 = *(u32*)&lbl_8047A608;
    tmp = (u32)&lbl_803D6E40;
    r22 = tmp;
L_80074854:
    r16 = 0x0;
L_80074858:
    if (r16 <= 0x20) goto L_8007486C;
    r3 = 0xDD650000;
    r20 = r3 + 0x4321;
    goto L_80074878;
L_8007486C:
    OSGetTick();
    tmp = r3 & 0xFFFFFF;
    r20 = tmp | (0xdd00 << 16);
L_80074878:
    r3 = 0x0;
    r5 = r20;
    r4 = r3;
    tmp = 0x4;
    ctr_fn = (void(*)(void))tmp;
L_8007488C:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007489C;
    r3 = r3 + 0x1;
L_8007489C:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800748B0;
    r3 = r3 + 0x1;
L_800748B0:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800748C4;
    r3 = r3 + 0x1;
L_800748C4:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800748D8;
    r3 = r3 + 0x1;
L_800748D8:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800748EC;
    r3 = r3 + 0x1;
L_800748EC:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80074900;
    r3 = r3 + 0x1;
L_80074900:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80074914;
    r3 = r3 + 0x1;
L_80074914:
    r5 = (u32)r5 >> 1;
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80074928;
    r3 = r3 + 0x1;
L_80074928:
    r5 = (u32)r5 >> 1;
    r4 = r4 + 0x7;
    if (--ctr != 0) goto L_8007488C;
    r16 = r16 + 0x1;
    if (r3 < 0xa) goto L_80074858;
    if (r3 > 0x18) goto L_80074858;
    r3 = (u32)r20 >> 24;
    tmp = ((r20 << 24) | ((u32)r20 >> 8)) & 0x0000FF00;
    r4 = ((r20 << 8) | ((u32)r20 >> 24)) & 0x00FF0000;
    r5 = r20 << 24;
    tmp = r3 | tmp;
    r3 = r31;
    tmp = r4 | tmp;
    r4 = r1 + 0xc;
    tmp = r5 | tmp;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80074988;
    r3 = 0x1;
    goto L_8007537C;
L_80074988:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r16 = tmp * 0x64;
    OSGetTick();
    r5 = (u32)&lbl_803B6E18;
    r4 = (u32)&lbl_803B6E08;
    r6 = r31 << 3;
    r17 = r3;
    tmp = (u32)&lbl_803B6E18;
    r21 = r31 << 2;
    r29 = tmp + r6;
    r19 = (u32)&lbl_803B6E08;
    r28 = r29 + 0x4;
L_800749D0:
    OSGetTick();
    tmp = r3 - r17;
    if (tmp <= r16) goto L_800749E8;
    r3 = 0x2;
    goto L_8007537C;
L_800749E8:
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074A04;
    r3 = 0x3;
    goto L_8007537C;
L_80074A04:
    tmp = *(u8*)(sp + 0x8);
    if (tmp == 0x38) goto L_80074A40;
    r12 = *(u32*)((u8*)r29 + 0x0);
    if (r12 == 0) goto L_80074A2C;
    r3 = r31;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80074A2C:
    tmp = *(u32*)(r19 + r21);
    if ((s32)tmp == 0) goto L_800749D0;
    r3 = 0x3e8;
    goto L_8007537C;
L_80074A40:
    r3 = r31;
    r4 = r1 + 0x10;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80074A60;
    r3 = 0x4;
    goto L_8007537C;
L_80074A60:
    r3 = (u32)r5 >> 24;
    tmp = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    r4 = r5 | tmp;
    tmp = r4 & 0xFF;
    if (tmp == 0xee) goto L_80074AA0;
    r3 = r30 + 0x68;
    OSReport();
    r21 = 0x0;
    goto L_80074B80;
L_80074AA0:
    r3 = 0x0;
    /* clrrwi r21, r4, 8 */;
    r5 = r3;
    tmp = 0x3;
    ctr_fn = (void(*)(void))tmp;
L_80074AB4:
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074AC4;
    r3 = r3 + 0x1;
L_80074AC4:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074AD8;
    r3 = r3 + 0x1;
L_80074AD8:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074AEC;
    r3 = r3 + 0x1;
L_80074AEC:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074B00;
    r3 = r3 + 0x1;
L_80074B00:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074B14;
    r3 = r3 + 0x1;
L_80074B14:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074B28;
    r3 = r3 + 0x1;
L_80074B28:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074B3C;
    r3 = r3 + 0x1;
L_80074B3C:
    r4 = r4 << 1;
    /* clrrwi tmp, r4, 31 */;
    if (tmp == 0) goto L_80074B50;
    r3 = r3 + 0x1;
L_80074B50:
    r4 = r4 << 1;
    r5 = r5 + 0x7;
    if (--ctr != 0) goto L_80074AB4;
    if (r3 < 7) goto L_80074B6C;
    if (r3 <= 0xe) goto L_80074B80;
L_80074B6C:
    r4 = r21;
    r3 = r30 + 0x98;
    OSReport();
    r21 = 0x0;
L_80074B80:
    if (r21 != 0) goto L_80074B90;
    r3 = 0x5;
    goto L_8007537C;
L_80074B90:
    tmp = r18 + 0x7;
    r3 = r31;
    /* clrrwi r27, tmp, 3 */;
    r4 = r1 + 0xc;
    r6 = (u32)r27 >> 3;
    r5 = r1 + 0x8;
    r6 = (u32)r8 >> 24;
    tmp = ((r8 << 24) | ((u32)r8 >> 8)) & 0x0000FF00;
    r7 = ((r8 << 8) | ((u32)r8 >> 24)) & 0x00FF0000;
    tmp = r6 | tmp;
    r6 = r8 << 24;
    tmp = r7 | tmp;
    tmp = r6 | tmp;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80074BE0;
    r3 = 0x6;
    goto L_8007537C;
L_80074BE0:
    r4 = 0x80000000;
    r3 = 0x10620000;
    tmp = *(u32*)((u8*)r4 + 0xF8);
    r3 = r3 + 0x4dd3;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r3 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r16 = tmp * 0x64;
    OSGetTick();
    r4 = (u32)&lbl_803B6E08;
    r19 = r3;
    r18 = r31 << 2;
    r17 = (u32)&lbl_803B6E08;
L_80074C14:
    OSGetTick();
    tmp = r3 - r19;
    if (tmp <= r16) goto L_80074C2C;
    r3 = 0x7;
    goto L_8007537C;
L_80074C2C:
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074C48;
    r3 = 0x8;
    goto L_8007537C;
L_80074C48:
    r3 = *(u8*)(sp + 0x8);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x30) goto L_80074C60;
    r3 = 0x9;
    goto L_8007537C;
L_80074C60:
    tmp = r3 & 0x00000002;
    if ((s32)tmp == 0) goto L_80074C9C;
    r12 = *(u32*)((u8*)r29 + 0x0);
    if (r12 == 0) goto L_80074C88;
    r3 = r31;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80074C88:
    tmp = *(u32*)(r17 + r18);
    if ((s32)tmp == 0) goto L_80074C14;
    r3 = 0x3e8;
    goto L_8007537C;
L_80074C9C:
    r3 = 0x61770000;
    r5 = r20 ^ r21;
    tmp = r3 + 0x614b;
    r3 = (u32)&lbl_803B6E08;
    r4 = r5 * tmp;
    r24 = r5;
    r17 = r31 << 2;
    r18 = (u32)&lbl_803B6E08;
    r25 = 0x30;
    r23 = 0x0;
    r26 = r4 + 0x1;
    r3 = 0x10620000;
    r20 = 0x80000000;
    r19 = r3 + 0x4dd3;
    goto L_80074F34;
L_80074CD8:
    r5 = *(u32*)((u8*)r22 + 0x0);
    r3 = (u32)r5 >> 24;
    tmp = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r5 = tmp;
    if (r23 < 0xa0) goto L_80074E18;
    r5 = tmp - r24;
    r4 = r24 ^ tmp;
    r5 = r5 ^ r26;
    r3 = 0x20;
    tmp = 0x4;
    ctr_fn = (void(*)(void))tmp;
L_80074D1C:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074D34;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074D38;
L_80074D34:
    r4 = (u32)r4 >> 1;
L_80074D38:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074D50;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074D54;
L_80074D50:
    r4 = (u32)r4 >> 1;
L_80074D54:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074D6C;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074D70;
L_80074D6C:
    r4 = (u32)r4 >> 1;
L_80074D70:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074D88;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074D8C;
L_80074D88:
    r4 = (u32)r4 >> 1;
L_80074D8C:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074DA4;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074DA8;
L_80074DA4:
    r4 = (u32)r4 >> 1;
L_80074DA8:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074DC0;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074DC4;
L_80074DC0:
    r4 = (u32)r4 >> 1;
L_80074DC4:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074DDC;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074DE0;
L_80074DDC:
    r4 = (u32)r4 >> 1;
L_80074DE0:
    tmp = r4 & 0x1;
    if (tmp == 0) goto L_80074DF8;
    r4 = (u32)r4 >> 1;
    r4 = r4 ^ 0xa1c1;
    goto L_80074DFC;
L_80074DF8:
    r4 = (u32)r4 >> 1;
L_80074DFC:
    if (--ctr != 0) goto L_80074D1C;
    r3 = 0x61770000;
    r24 = r4;
    tmp = r3 + 0x614b;
    r3 = r26 * tmp;
    r26 = r3 + 0x1;
L_80074E18:
    r3 = (u32)r5 >> 24;
    tmp = ((r5 << 24) | ((u32)r5 >> 8)) & 0x0000FF00;
    r4 = ((r5 << 8) | ((u32)r5 >> 24)) & 0x00FF0000;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    r3 = r31;
    tmp = r4 | tmp;
    r4 = r1 + 0xc;
    tmp = r5 | tmp;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_80074E58;
    r3 = 0xa;
    goto L_8007537C;
L_80074E58:
    tmp = *(u8*)(sp + 0x8);
    tmp = tmp & 0x00000030;
    if (tmp == r25) goto L_80074E70;
    r3 = 0x12;
    goto L_8007537C;
L_80074E70:
    tmp = *(u32*)((u8*)r20 + 0xF8);
    r25 = r25 ^ 0x10;
    tmp = (u32)tmp >> 2;
    tmp = (u32)((u64)r19 * (u64)tmp >> 32);
    tmp = (u32)tmp >> 6;
    r16 = tmp * 0x64;
    OSGetTick();
    r21 = r3;
L_80074E90:
    OSGetTick();
    tmp = r3 - r21;
    if (tmp <= r16) goto L_80074EA8;
    r3 = 0xb;
    goto L_8007537C;
L_80074EA8:
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074EC4;
    r3 = 0xc;
    goto L_8007537C;
L_80074EC4:
    r3 = *(u8*)(sp + 0x8);
    tmp = r3 & 0x00000020;
    if ((s32)tmp != 0) goto L_80074EDC;
    r3 = 0xd;
    goto L_8007537C;
L_80074EDC:
    tmp = r3 & 0x00000002;
    if ((s32)tmp == 0) goto L_80074F18;
    r12 = *(u32*)((u8*)r29 + 0x0);
    if (r12 == 0) goto L_80074F04;
    r3 = r31;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80074F04:
    tmp = *(u32*)(r18 + r17);
    if ((s32)tmp == 0) goto L_80074E90;
    r3 = 0x3e8;
    goto L_8007537C;
L_80074F18:
    tmp = *(u32*)(r18 + r17);
    if ((s32)tmp == 0) goto L_80074F2C;
    r3 = 0x3e8;
    goto L_8007537C;
L_80074F2C:
    r23 = r23 + 0x4;
    r22 = r22 + 0x4;
L_80074F34:
    if (r23 < r27) goto L_80074CD8;
    r3 = 0x80000000;
    tmp = *(u32*)((u8*)r3 + 0xF8);
    tmp = (u32)tmp >> 2;
    r17 = tmp * 0xa;
    OSGetTick();
    r4 = (u32)&lbl_803B6E08;
    r19 = r3;
    r18 = r31 << 2;
    r16 = (u32)&lbl_803B6E08;
L_80074F60:
    OSGetTick();
    tmp = r3 - r19;
    if (tmp <= r17) goto L_80074F78;
    r3 = 0xe;
    goto L_8007537C;
L_80074F78:
    r3 = r31;
    r4 = r1 + 0x8;
    ((void(*)(void))fn_8025F3F4)();
    if ((s32)r3 == 0) goto L_80074F94;
    r3 = 0xf;
    goto L_8007537C;
L_80074F94:
    r3 = *(u8*)(sp + 0x8);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x30) goto L_80074FAC;
    r3 = 0x10;
    goto L_8007537C;
L_80074FAC:
    tmp = r3 & 0xa;
    if ((s32)tmp != 8) goto L_800752CC;
    r3 = r31;
    r4 = r1 + 0x10;
    r5 = r1 + 0x8;
    ((void(*)(void))fn_8025F584)();
    if ((s32)r3 == 0) goto L_80074FD8;
    r3 = 0x11;
    goto L_8007537C;
L_80074FD8:
    r5 = (u32)r4 >> 24;
    if (r5 != 0xff) goto L_800752A8;
    tmp = ((r4 << 24) | ((u32)r4 >> 8)) & 0x0000FF00;
    r3 = ((r4 << 8) | ((u32)r4 >> 24)) & 0x00FF0000;
    tmp = r5 | tmp;
    r4 = r4 << 24;
    tmp = r3 | tmp;
    r5 = r24;
    r16 = r4 | tmp;
    r3 = r30 + 0xcc;
    r4 = r16;
    OSReport();
    r7 = (u32)r16 >> 8;
    r3 = 0x0;
    r4 = -0x1;
L_80075020:
    r6 = r4 ^ r24;
    r5 = 0x20;
    tmp = 0x4;
    ctr_fn = (void(*)(void))tmp;
L_80075030:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_80075048;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_8007504C;
L_80075048:
    r6 = (u32)r6 >> 1;
L_8007504C:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_80075064;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_80075068;
L_80075064:
    r6 = (u32)r6 >> 1;
L_80075068:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_80075080;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_80075084;
L_80075080:
    r6 = (u32)r6 >> 1;
L_80075084:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_8007509C;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_800750A0;
L_8007509C:
    r6 = (u32)r6 >> 1;
L_800750A0:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_800750B8;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_800750BC;
L_800750B8:
    r6 = (u32)r6 >> 1;
L_800750BC:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_800750D4;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_800750D8;
L_800750D4:
    r6 = (u32)r6 >> 1;
L_800750D8:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_800750F0;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_800750F4;
L_800750F0:
    r6 = (u32)r6 >> 1;
L_800750F4:
    tmp = r6 & 0x1;
    if (tmp == 0) goto L_8007510C;
    r6 = (u32)r6 >> 1;
    r6 = r6 ^ 0xa1c1;
    goto L_80075110;
L_8007510C:
    r6 = (u32)r6 >> 1;
L_80075110:
    if (--ctr != 0) goto L_80075030;
    if (r7 != r6) goto L_80075230;
    r5 = r4 ^ 0xbb;
    r3 = 0x20;
    tmp = 0x4;
    ctr_fn = (void(*)(void))tmp;
L_80075130:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80075148;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_8007514C;
L_80075148:
    r5 = (u32)r5 >> 1;
L_8007514C:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80075164;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_80075168;
L_80075164:
    r5 = (u32)r5 >> 1;
L_80075168:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_80075180;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_80075184;
L_80075180:
    r5 = (u32)r5 >> 1;
L_80075184:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007519C;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_800751A0;
L_8007519C:
    r5 = (u32)r5 >> 1;
L_800751A0:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800751B8;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_800751BC;
L_800751B8:
    r5 = (u32)r5 >> 1;
L_800751BC:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800751D4;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_800751D8;
L_800751D4:
    r5 = (u32)r5 >> 1;
L_800751D8:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_800751F0;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_800751F4;
L_800751F0:
    r5 = (u32)r5 >> 1;
L_800751F4:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007520C;
    r5 = (u32)r5 >> 1;
    r5 = r5 ^ 0xa1c1;
    goto L_80075210;
L_8007520C:
    r5 = (u32)r5 >> 1;
L_80075210:
    if (--ctr != 0) goto L_80075130;
    r16 = r5 | (0xbb00 << 16);
    r3 = r30 + 0x100;
    r5 = r16;
    OSReport();
    goto L_80075258;
L_80075230:
    r4 = r4 + (0x100 << 16);
    r3 = r3 + 0x1;
    if (r3 < 0x100) goto L_80075020;
    r4 = r16;
    r5 = r24;
    r3 = r30 + 0x130;
    OSReport();
    r16 = 0x0;
L_80075258:
    if (r16 != 0) goto L_80075268;
    r3 = 0x12;
    goto L_8007537C;
L_80075268:
    r3 = (u32)r16 >> 24;
    tmp = ((r16 << 24) | ((u32)r16 >> 8)) & 0x0000FF00;
    r4 = ((r16 << 8) | ((u32)r16 >> 24)) & 0x00FF0000;
    r5 = r16 << 24;
    tmp = r3 | tmp;
    r3 = r31;
    tmp = r4 | tmp;
    r4 = r1 + 0xc;
    tmp = r5 | tmp;
    r5 = r1 + 0x8;
    *(u32*)(sp + 0xC) = tmp;
    ((void(*)(void))fn_8025F648)();
    if ((s32)r3 == 0) goto L_800752FC;
    r3 = 0x13;
    goto L_8007537C;
L_800752A8:
    if (r5 == 0xcc) goto L_800752B8;
    r3 = 0x14;
    goto L_8007537C;
L_800752B8:
    tmp = *(u32*)(r16 + r18);
    if ((s32)tmp == 0) goto L_80074F60;
    r3 = 0x3e8;
    goto L_8007537C;
L_800752CC:
    r12 = *(u32*)((u8*)r29 + 0x0);
    if (r12 == 0) goto L_800752E8;
    r3 = r31;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_800752E8:
    tmp = *(u32*)(r16 + r18);
    if ((s32)tmp == 0) goto L_80074F60;
    r3 = 0x3e8;
    goto L_8007537C;
L_800752FC:
    r3 = 0x80000000;
    tmp = *(u32*)((u8*)r3 + 0xF8);
    r19 = (u32)tmp >> 2;
    OSGetTick();
    r4 = (u32)&lbl_803B6E08;
    r18 = r3;
    r17 = r31 << 2;
    r16 = (u32)&lbl_803B6E08;
L_8007531C:
    OSGetTick();
    tmp = r3 - r18;
    if (tmp <= r19) goto L_80075334;
    r3 = 0x15;
    goto L_8007537C;
L_80075334:
    r3 = r31;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp != 0) goto L_80075378;
    r12 = *(u32*)((u8*)r29 + 0x0);
    if (r12 == 0) goto L_80075364;
    r3 = r31;
    r4 = *(u32*)((u8*)r28 + 0x0);
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
L_80075364:
    tmp = *(u32*)(r16 + r17);
    if ((s32)tmp == 0) goto L_8007531C;
    r3 = 0x3e8;
    goto L_8007537C;
L_80075378:
    r3 = 0x0;
L_8007537C:
    return;
}

/* 0x80075390 | size: 0x40 */
s32 fn_80075390(void) {
    extern void fn_80075638();
    extern void fn_8007565C();
    extern void fn_800756C8();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;

    fn_80075638();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_800753B4;
    fn_8007565C();
    goto L_800753BC;
L_800753B4:
    r3 = 0x3;
    fn_800756C8();
L_800753BC:
    r3 = 0x0;
    return;
}

/* 0x800753D0 | size: 0x148 */
void fn_800753D0(void) {
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f31 = 0.0f;

    ((void(*)(void))fn_800D37CC)();
    tmp = 0x43300000;
    f1 = *(f64*)&lbl_8047C0B0;
    *(u32*)(sp + 0x18) = tmp;
    f31 = f0 - f1;
    ((void(*)(void))fn_800D3088)();
    tmp = 0x43300000;
    f1 = *(f64*)&lbl_8047C0B8;
    *(u32*)(sp + 0x20) = tmp;
    r3 = *(u32*)&lbl_8047A610;
    f0 = f0 - f1;
    f0 = f0 / f31;
    *(f32*)((u8*)r3 + 0x0) = f0;
    r3 = *(u32*)&lbl_8047A610;
    f1 = *(f32*)((u8*)r3 + 0x4);
    ((void(*)(void))fn_800CE148)();
    f2 = (f32)f1;
    f1 = *(f32*)&lbl_8047C09C;
    f0 = *(f32*)&lbl_8047C098;
    r3 = *(u32*)&lbl_8047A610;
    f1 = f1 * f2 + f0;
    f0 = *(f32*)&lbl_8047C0A0;
    *(f32*)((u8*)r3 + 0x18C) = f1;
    r3 = *(u32*)&lbl_8047A610;
    f1 = *(f32*)((u8*)r3 + 0x18C);
    if (f1 <= f0) goto L_80075468;
    *(f32*)((u8*)r3 + 0x18C) = f0;
L_80075468:
    r3 = *(u32*)&lbl_8047A610;
    r4 = 0x0;
    r3 = r3 + 0x144;
    ((void(*)(void))fn_80109B90)();
    tmp = r3 & 0xFF;
    if (tmp != 0) goto L_800754CC;
    r3 = *(u32*)&lbl_8047A610;
    r3 = *(u32*)((u8*)r3 + 0x168);
    ((void(*)(void))fn_801DAC3C)();
    if (r3 == 0) goto L_800754CC;
    r5 = *(u32*)&lbl_8047A610;
    r4 = r1 + 0x8;
    f2 = *(f32*)&lbl_8047C0A4;
    f0 = *(f32*)((u8*)r5 + 0x0);
    f1 = *(f32*)((u8*)r5 + 0x8);
    f2 = f2 * f0;
    f0 = *(f32*)&lbl_8047C0A8;
    f1 = f1 + f2;
    *(f32*)((u8*)r5 + 0x8) = f1;
    *(f32*)(sp + 0x8) = f0;
    *(f32*)(sp + 0x10) = f0;
    *(f32*)(sp + 0xC) = f2;
    ((void(*)(void))fn_800E3DC4)();
L_800754CC:
    r3 = *(u32*)&lbl_8047A610;
    f0 = *(f32*)&lbl_8047C0AC;
    f2 = *(f32*)((u8*)r3 + 0x4);
    f1 = *(f32*)((u8*)r3 + 0x0);
    f1 = f2 + f1;
    *(f32*)((u8*)r3 + 0x4) = f1;
    r3 = *(u32*)&lbl_8047A610;
    f1 = *(f32*)((u8*)r3 + 0x4);
    /* cror eq, gt, eq */;
    if (f1 != f0) goto L_80075500;
    f0 = *(f32*)&lbl_8047C0A8;
    *(f32*)((u8*)r3 + 0x4) = f0;
L_80075500:
    return;
}

/* 0x80075518 | size: 0x120 */
s32 fn_80075518(void) {
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;

    r29 = r4;
    tmp = *(s16*)((u8*)r29 + 0x6);
    if ((s32)tmp == 0xd3d) goto L_8007561C;
    if ((s32)tmp >= 0xd3d) goto L_8007561C;
    if ((s32)tmp >= 0xd3c) goto L_80075550;
    goto L_8007561C;
L_80075550:
    r3 = *(u32*)&lbl_8047A610;
    f0 = *(f32*)((u8*)r3 + 0x18C);
    r3 = r3 + 0x144;
    f0 = (f64)(s32)f0;
    ((void(*)(void))fn_80109934)();
    r30 = r3;
    if (r30 == 0) goto L_8007561C;
    r3 = 0x3;
    ((void(*)(void))fn_800D88DC)();
    r3 = 0x4;
    ((void(*)(void))fn_800D888C)();
    r3 = 0x7;
    ((void(*)(void))fn_800D6A00)();
    r3 = (u32)&lbl_80314F98;
    r3 = (u32)&lbl_80314F98;
    ((void(*)(void))fn_800D7820)();
    r4 = r30;
    r3 = 0x0;
    ((void(*)(void))fn_800D85D4)();
    r3 = 0x2;
    ((void(*)(void))fn_800D67BC)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_800D61E4)();
    r7 = r31;
    r3 = 0x0;
    r4 = 0x28;
    r5 = 0x3e;
    r6 = 0xc8;
    ((void(*)(void))fn_800D5CB8)();
    f1 = *(f32*)&lbl_8047C0A8;
    r3 = 0x0;
    f2 = f1;
    ((void(*)(void))fn_800D59B8)();
    r3 = *(s16*)((u8*)r29 + 0x54);
    r4 = *(s16*)((u8*)r29 + 0x56);
    ((void(*)(void))fn_800D61E4)();
    r7 = r31;
    r3 = 0x0;
    r4 = 0x28;
    r5 = 0x3e;
    r6 = 0xc8;
    ((void(*)(void))fn_800D5CB8)();
    f1 = *(f32*)&lbl_8047C0AC;
    r3 = 0x0;
    f2 = f1;
    ((void(*)(void))fn_800D59B8)();
    ((void(*)(void))fn_800D6728)();
L_8007561C:
    return;
}

/* 0x80075638 | size: 0x24 */
s32 fn_80075638(void) {
    fn_80102620();
    return 0;
}

/* 0x8007565C | size: 0x6C */
s32 fn_8007565C(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r3 = *(u32*)&lbl_8047A610;
    r3 = r3 + 0x144;
    ((void(*)(void))fn_8010A420)();
    r3 = 0xd8;
    r4 = 0x0;
    r5 = 0x1;
    ((void(*)(void))fn_80102568)();
    r3 = *(u32*)&lbl_8047A610;
    ((void(*)(void))fn_800E202C)();
    r31 = r3;
    tmp = r31 & 0xFFFF;
    if (tmp == 0) goto L_800756AC;
    ((void(*)(void))fn_800E24B0)();
    r3 = r31;
    ((void(*)(void))fn_800E209C)();
L_800756AC:
    tmp = 0x0;
    *(u32*)&lbl_8047A610 = tmp;
    return;
}

/* 0x800756C8 | size: 0x128 */
s32 fn_800756C8(void) {
    u8 sp[0x10];
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

    r30 = r3;
    ((void(*)(void))fn_800FF56C)();
    if (r3 != 0x43) goto L_800757D8;
    r3 = 0x1a0;
    r4 = 0x20;
    ((void(*)(void))fn_800E2C04)();
    tmp = r3 & 0xFFFF;
    if (tmp == 0) goto L_8007570C;
    ((void(*)(void))fn_800E27B0)();
    goto L_80075710;
L_8007570C:
    r3 = 0x0;
L_80075710:
    *(u32*)&lbl_8047A610 = r3;
    r31 = r3 + 0xc;
    r3 = 0x0;
    r4 = 0x1;
    ((void(*)(void))fn_80135938)();
    r6 = r3;
    r3 = r31;
    r4 = r30;
    r5 = 0xa;
    ((void(*)(void))fn_801240C4)();
    f0 = *(f32*)&lbl_8047C0A8;
    r3 = *(u32*)&lbl_8047A610;
    *(f32*)((u8*)r3 + 0x4) = f0;
    r3 = *(u32*)&lbl_8047A610;
    *(f32*)((u8*)r3 + 0x8) = f0;
    r3 = *(u32*)&lbl_8047A610;
    f1 = *(f32*)((u8*)r3 + 0x4);
    ((void(*)(void))fn_800CE148)();
    f2 = (f32)f1;
    f1 = *(f32*)&lbl_8047C09C;
    f0 = *(f32*)&lbl_8047C098;
    r3 = *(u32*)&lbl_8047A610;
    f1 = f1 * f2 + f0;
    f0 = *(f32*)&lbl_8047C0A0;
    *(f32*)((u8*)r3 + 0x18C) = f1;
    r3 = *(u32*)&lbl_8047A610;
    f1 = *(f32*)((u8*)r3 + 0x18C);
    if (f1 <= f0) goto L_80075788;
    *(f32*)((u8*)r3 + 0x18C) = f0;
L_80075788:
    r4 = (u32)&lbl_802EF0A8;
    r3 = *(u32*)&lbl_8047A610;
    r4 = (u32)&lbl_802EF0A8;
    r5 = r4 + (0x1 << 16);
    r3 = r3 + 0x144;
    r4 = *(s16*)((u8*)r5 + 0x7296);
    r5 = *(s16*)((u8*)r5 + 0x7298);
    ((void(*)(void))fn_8010A5BC)();
    r4 = *(u32*)&lbl_8047A610;
    r3 = r4 + 0x144;
    r4 = r4 + 0xc;
    ((void(*)(void))fn_80109C88)();
    r3 = 0xd8;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x0;
    r8 = 0x0;
    ((void(*)(void))fn_801026A4)();
L_800757D8:
    return;
}

/* 0x800757F0 | size: 0x2C */
void fn_800757F0(void) {
    fn_801CB9D8();
}

#pragma pop
