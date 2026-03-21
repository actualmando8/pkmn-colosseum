/**
 * @file menu_exdisc.c
 * @brief Extra disc shrine and related menus (0x80077A5C-0x80078D38)
 *
 * Address range: 0x80077A5C - 0x80078D38
 * Total functions: 20
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001E074();
extern void fn_8006B420();
extern void fn_80075B74();
extern void fn_80075BFC();
extern void fn_80092C90();
extern void fn_80093574();
extern void fn_80093610();
extern void fn_80093698();
extern void fn_800C80D0();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800F0308();
extern void fn_80103CC0();
extern void fn_801067E8();
extern void fn_801069FC();
extern void fn_80106D3C();
extern void fn_801159F0();
extern void fn_80115BD8();
extern void fn_80123FBC();
extern void fn_80124A60();
extern void fn_8012640C();
extern void fn_80129280();
extern void fn_8012A5B0();
extern void fn_8012AA2C();
extern void fn_8012AC08();
extern void fn_8012AC54();
extern void fn_80130660();
extern void fn_80132A38();
extern void fn_80142984();
extern void fn_80165668();
extern void fn_80166A28();
extern void fn_80196E10();
extern void fn_801C40F0();
extern void fn_801C41C8();
extern void fn_801CB708();
extern void fn_801CB834();
extern void fn_801D0314();
extern void fn_801D036C();
extern void fn_801D0748();
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478928;
extern u8 lbl_8047A620;
extern u8 lbl_8047C0E0;
extern u8 lbl_8047C0E4;
extern u8 lbl_8047C0E8;
extern u8 lbl_8047C0F0;
extern u8 lbl_8047C0F8;
extern u8 lbl_8047C100;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268940[];
extern u8 lbl_80268AB8[];
extern u8 lbl_802EE458[];
extern u8 lbl_803F6E40[];
extern u8 lbl_803F6F18[];

/* ===== Forward declarations ===== */
void fn_80077A5C(void);
void fn_80077AAC(void);
void fn_80077AD0(void);
void fn_80077AF4(void);
void fn_80077B18(void);
void fn_80077B3C(void);
void fn_80077B60(void);
void fn_80077B84(void);
s32 fn_80077BA8(void);
s32 fn_80077BD0(void);
s32 fn_80077C1C(void);
s32 fn_80077C68(void);
s32 fn_80077D88(void);
s32 fn_80077DB8(void);
s32 fn_80077E50(void);
void fn_80077E80(void);
void fn_80077EA4(void);
s32 fn_80077ED4(void);
s32 fn_80078390(void);
s32 fn_800788BC(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80077A5C | size: 0x50 */
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80077A5C(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = 0x0;
    if ((u32)r3 == (u32)0x0) goto L_80077A90;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x0) goto L_80077A94;
L_80077A90: ;
    r31 = 0x1;
L_80077A94: ;
    r3 = r31 & 0xFF;
    r31 = *(u32*)(sp + 0xC);
    return;
}

/* 0x80077AAC | size: 0x24 */
void fn_80077AAC(void) {
    fn_8006B420();
}

/* 0x80077AD0 | size: 0x24 */
void fn_80077AD0(void) {
    fn_8006B420();
}

/* 0x80077AF4 | size: 0x24 */
void fn_80077AF4(void) {
    fn_8006B420();
}

/* 0x80077B18 | size: 0x24 */
void fn_80077B18(void) {
    fn_8006B420();
}

/* 0x80077B3C | size: 0x24 */
void fn_80077B3C(void) {
    fn_8006B420();
}

/* 0x80077B60 | size: 0x24 */
void fn_80077B60(void) {
    fn_8006B420();
}

/* 0x80077B84 | size: 0x24 */
void fn_80077B84(void) {
    fn_8006B420();
}

/* 0x80077BA8 | size: 0x28 */
s32 fn_80077BA8(void) {
    fn_8006B420();
    return 0;
}

/* 0x80077BD0 | size: 0x4C */
s32 fn_80077BD0(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    tmp = *(u32*)((u8*)r3 + 0x8);
    if ((s32)tmp >= 3) goto L_80077C08;
    if ((s32)tmp >= 0) goto L_80077C00;
    goto L_80077C08;
L_80077C00:
    r3 = 0x1;
    goto L_80077C0C;
L_80077C08:
    r3 = 0x0;
L_80077C0C:
    return;
}

/* 0x80077C1C | size: 0x4C */
s32 fn_80077C1C(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;

    tmp = r3 & 0xFFFF;
    if ((s32)tmp == 0xaf) goto L_80077C4C;
    if ((s32)tmp >= 0xaf) goto L_80077C54;
    if ((s32)tmp == 0) goto L_80077C44;
    goto L_80077C54;
L_80077C44:
    r3 = 0x1;
    goto L_80077C58;
L_80077C4C:
    r3 = 0x0;
    goto L_80077C58;
L_80077C54:
    ((void(*)(void))fn_80142984)();
L_80077C58:
    return;
}

/* 0x80077C68 | size: 0x120 */
s32 fn_80077C68(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r3;
    ((void(*)(void))fn_8006B420)();
    tmp = r30 & 0xFFFF;
    r31 = r3;
    if ((s32)tmp == 0xaf) goto L_80077CAC;
    if ((s32)tmp >= 0xaf) goto L_80077CB4;
    if ((s32)tmp == 0) goto L_80077CA4;
    goto L_80077CB4;
L_80077CA4:
    r3 = 0x1;
    goto L_80077CBC;
L_80077CAC:
    r3 = 0x0;
    goto L_80077CBC;
L_80077CB4:
    r3 = r30;
    ((void(*)(void))fn_80142984)();
L_80077CBC:
    tmp = r3 & 0xFF;
    if (tmp != 0) goto L_80077CD0;
    r3 = 0x0;
    goto L_80077D70;
L_80077CD0:
    tmp = *(u32*)((u8*)r31 + 0x8);
    if ((s32)tmp == 1) goto L_80077D00;
    if ((s32)tmp >= 1) goto L_80077CEC;
    if ((s32)tmp >= 0) goto L_80077CF8;
    goto L_80077D6C;
L_80077CEC:
    if ((s32)tmp >= 3) goto L_80077D6C;
    goto L_80077D14;
L_80077CF8:
    r3 = 0x1;
    goto L_80077D70;
L_80077D00:
    tmp = r30 & 0xFFFF;
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
    r3 = tmp & 0xFF;
    goto L_80077D70;
L_80077D14:
    r3 = (u32)&lbl_802EE458;
    tmp = *(u32*)&lbl_80478928;
    r4 = (u32)&lbl_802EE458;
    r5 = 0x0;
    r3 = r30 & 0xFFFF;
    ctr_fn = (void(*)(void))tmp;
    if (tmp <= 0) goto L_80077D64;
L_80077D34:
    tmp = *(u16*)((u8*)r4 + 0x0);
    if (r3 != tmp) goto L_80077D58;
    r3 = r31 + r5;
    tmp = *(u8*)((u8*)r3 + 0x18);
    tmp = __cntlzw(tmp);
    tmp = (u32)tmp >> 5;
    r3 = tmp & 0xFF;
    goto L_80077D70;
L_80077D58:
    r4 = r4 + 0x2;
    r5 = r5 + 0x1;
    if (--ctr != 0) goto L_80077D34;
L_80077D64:
    r3 = 0x1;
    goto L_80077D70;
L_80077D6C:
    r3 = 0x0;
L_80077D70:
    return;
}

/* 0x80077D88 | size: 0x30 */
s32 fn_80077D88(void) {
    return 0;
}

/* 0x80077DB8 | size: 0x98 */
s32 fn_80077DB8(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    tmp = *(u32*)((u8*)r3 + 0x0);
    if ((s32)tmp != 2) goto L_80077DFC;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    tmp = *(u32*)((u8*)r3 + 0x8);
    if ((s32)tmp != 0) goto L_80077DFC;
    r3 = 0x6;
    goto L_80077E40;
L_80077DFC:
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    tmp = *(u32*)((u8*)r3 + 0x4);
    if ((s32)tmp == 1) goto L_80077E34;
    if ((s32)tmp >= 1) goto L_80077E24;
    if ((s32)tmp >= 0) goto L_80077E2C;
    goto L_80077E3C;
L_80077E24:
    goto L_80077E3C;
L_80077E2C:
    r3 = 0x3;
    goto L_80077E40;
L_80077E34:
    r3 = 0x4;
    goto L_80077E40;
L_80077E3C:
    r3 = 0x2;
L_80077E40:
    return;
}

/* 0x80077E50 | size: 0x30 */
s32 fn_80077E50(void) {
    return 0;
}

/* 0x80077E80 | size: 0x24 */
void fn_80077E80(void) {
}

/* 0x80077EA4 | size: 0x30 */
void fn_80077EA4(void) {
    fn_800C80D0();
}

/* 0x80077ED4 | size: 0x4BC */
s32 fn_80077ED4(void) {
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)&lbl_803F6F18;
    r3 = (u32)&lbl_803F6F18;
    ((void(*)(void))fn_8012AC54)();
    r30 = r3;
    ((void(*)(void))fn_801D036C)();
    tmp = r3;
    r3 = 0x0;
    r29 = tmp;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_80077F44:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_80077F44;
    ((void(*)(void))fn_80075B74)();
    r3 = 0x4;
    r4 = 0x2;
    r5 = 0x0;
    ((void(*)(void))fn_801D0748)();
    if ((s32)r3 == 4) goto L_80078048;
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f31 = *(f64*)&lbl_8047C0F0;
    r28 = 0x43300000;
    f29 = *(f64*)&lbl_8047C0F8;
    f28 = *(f32*)&lbl_8047C0E4;
    goto L_80077FF4;
L_80077FBC:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0xC) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80077FF4:
    if (f27 < f28) goto L_80077FBC;
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x44b0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    goto L_80078344;
L_80078048:
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x3d83;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106D3C)();
    ((void(*)(void))fn_800F0308)();
    r27 = 0x0;
    r3 = (u32)&lbl_803F6F18;
    r28 = (u32)&lbl_803F6F18;
    goto L_8007808C;
L_80078078:
    r3 = r28;
    r4 = r27;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_80124A60)();
    r27 = r27 + 0x1;
L_8007808C:
    tmp = r27 & 0xFFFF;
    if (tmp < 6) goto L_80078078;
    ((void(*)(void))fn_80115BD8)();
    ((void(*)(void))fn_801159F0)();
    r5 = (u32)&lbl_803F6F18;
    r4 = r3;
    r3 = (u32)&lbl_803F6F18;
    ((void(*)(void))fn_80130660)();
    r27 = 0x0;
    r3 = (u32)&lbl_803F6F18;
    r28 = (u32)&lbl_803F6F18;
    goto L_800780F8;
L_800780C0:
    r3 = r28;
    r4 = r27;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_80123FBC)();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_800780F4;
    r3 = (u32)&lbl_803F6F18;
    r4 = r27;
    r3 = (u32)&lbl_803F6F18;
    ((void(*)(void))fn_8012AC08)();
    r31 = r3;
    goto L_80078104;
L_800780F4:
    r27 = r27 + 0x1;
L_800780F8:
    tmp = r27 & 0xFFFF;
    if (tmp < 6) goto L_800780C0;
L_80078104:
    if (r31 != 0) goto L_80078120;
    r3 = (u32)&lbl_80268AB8;
    r4 = 0x42e;
    r3 = (u32)&lbl_80268AB8;
    r5 = (u32)&lbl_8047C0E8;
    ((void(*)(void))fn_80196E10)();
L_80078120:
    r4 = (u32)&lbl_803F6E40;
    r3 = 0x1;
    r4 = (u32)&lbl_803F6E40;
    tmp = *(u32*)((u8*)r4 + 0x8);
    tmp = tmp | 0x8;
    *(u32*)((u8*)r4 + 0x8) = tmp;
    ((void(*)(void))fn_80093574)();
    r3 = (u32)&lbl_803F6E40;
    r5 = r31;
    r4 = (u32)&lbl_803F6E40;
    r3 = 0x1;
    ((void(*)(void))fn_80092C90)();
    r3 = 0x1;
    ((void(*)(void))fn_80093574)();
    r3 = 0x1;
    ((void(*)(void))fn_80093610)();
    if ((s32)r3 == 0xc) goto L_800782B8;
    r3 = 0x1;
    ((void(*)(void))fn_80093698)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x3d85;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_800781B8:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_800781B8;
    r3 = 0x4;
    r4 = 0x2;
    r5 = 0x0;
    ((void(*)(void))fn_801D0748)();
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f28 = *(f64*)&lbl_8047C0F0;
    r31 = 0x43300000;
    f30 = *(f64*)&lbl_8047C0F8;
    f31 = *(f32*)&lbl_8047C0E4;
    goto L_8007825C;
L_80078224:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_8007825C:
    if (f27 < f31) goto L_80078224;
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x44b0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    goto L_80078344;
L_800782B8:
    r3 = 0x1;
    ((void(*)(void))fn_80093698)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x3d84;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x3d2;
    r4 = 0x0;
    r5 = 0xff;
    ((void(*)(void))fn_80165668)();
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x4435;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x3d55;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80078344:
    r3 = r29;
    ((void(*)(void))fn_801D0314)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    return;
}

/* 0x80078390 | size: 0x52C */
s32 fn_80078390(void) {
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r3 = (u32)&lbl_803F6F18;
    r3 = (u32)&lbl_803F6F18;
    ((void(*)(void))fn_8012AA2C)();
    r4 = (u32)&lbl_803F6F18;
    r31 = r3;
    r3 = (u32)&lbl_803F6F18;
    ((void(*)(void))fn_8012AC54)();
    r4 = (u32)&lbl_803F6E40;
    r4 = (u32)&lbl_803F6E40;
    tmp = *(u32*)((u8*)r4 + 0x8);
    tmp = tmp & 0x00000008;
    if (tmp == 0) goto L_80078494;
    r4 = r3;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    tmp = r31 & 0xFF;
    if ((s32)tmp == 1) goto L_80078444;
    if ((s32)tmp >= 1) goto L_80078444;
    if ((s32)tmp >= 0) goto L_80078428;
    goto L_80078444;
L_80078428:
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x43cb;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    goto L_8007845C;
L_80078444:
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x43cd;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
L_8007845C:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    goto L_8007887C;
L_80078494:
    r30 = r3;
    r4 = r3;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    tmp = r31 & 0xFF;
    if ((s32)tmp == 1) goto L_800784DC;
    if ((s32)tmp >= 1) goto L_800784DC;
    if ((s32)tmp >= 0) goto L_800784C0;
    goto L_800784DC;
L_800784C0:
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x43c7;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    goto L_800784F4;
L_800784DC:
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x43c9;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
L_800784F4:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x3;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = 0x104F0000;
    r4 = 0x2;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x2;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    r3 = 0x104F0000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_801CB834)();
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x4434;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = (u32)&lbl_803F6E40;
    r3 = (u32)&lbl_803F6E40;
    tmp = *(u16*)((u8*)r3 + 0xC);
    if (tmp != 6) goto L_800786A0;
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x43a6;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f31 = *(f64*)&lbl_8047C0F0;
    r31 = 0x43300000;
    f29 = *(f64*)&lbl_8047C0F8;
    f28 = *(f32*)&lbl_8047C0E4;
    goto L_80078644;
L_8007860C:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0xC) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80078644:
    if (f27 < f28) goto L_8007860C;
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x44b0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    goto L_8007887C;
L_800786A0:
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x5;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f31 = *(f64*)&lbl_8047C0F0;
    r31 = 0x43300000;
    f29 = *(f64*)&lbl_8047C0F8;
    f28 = *(f32*)&lbl_8047C0E4;
    goto L_80078720;
L_800786E8:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80078720:
    if (f27 < f28) goto L_800786E8;
    r4 = r30;
    r3 = 0x4d;
    ((void(*)(void))fn_80132A38)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x43b0;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x0;
    r4 = -0x1;
    r5 = -0x1;
    r6 = 0x0;
    ((void(*)(void))fn_8001E074)();
    r31 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = (s8)r31;
    if ((s32)tmp == 0) goto L_80078874;
    if ((s32)tmp >= 0) goto L_80078794;
    if ((s32)tmp >= (s32)-0x1) goto L_8007879C;
    goto L_80078874;
L_80078794:
    if ((s32)tmp >= 2) goto L_80078874;
L_8007879C:
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f28 = *(f64*)&lbl_8047C0F0;
    r31 = 0x43300000;
    f30 = *(f64*)&lbl_8047C0F8;
    f31 = *(f32*)&lbl_8047C0E4;
    goto L_8007881C;
L_800787E4:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_8007881C:
    if (f27 < f31) goto L_800787E4;
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x44b0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x1;
    ((void(*)(void))fn_80103CC0)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    goto L_8007887C;
L_80078874:
    tmp = 0xb;
    *(u32*)&lbl_8047A620 = tmp;
L_8007887C:
    return;
}

/* 0x800788BC | size: 0x47C */
s32 fn_800788BC(void) {
    u8 sp[0x80];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r3 = 0x43a1;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x43a2;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x3;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = 0x104F0000;
    r4 = 0x2;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x2;
    ((void(*)(void))fn_801C41C8)();
    r3 = 0x1;
    ((void(*)(void))fn_801C40F0)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    r3 = 0x104F0000;
    r4 = 0x3;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x43a3;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x43a4;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    r30 = 0x0;
    r29 = r3;
    goto L_80078A14;
L_800789E8:
    r3 = r29;
    r5 = r30 & 0xFF;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    ((void(*)(void))fn_80123FBC)();
    tmp = r3 & 0xFF;
    if (tmp == 1) goto L_80078A10;
    tmp = 0x1;
    goto L_80078A24;
L_80078A10:
    r30 = r30 + 0x1;
L_80078A14:
    tmp = r30 & 0xFF;
    if (tmp < 6) goto L_800789E8;
    tmp = 0x0;
L_80078A24:
    tmp = tmp & 0xFF;
    if (tmp != 0) goto L_80078AEC;
    r3 = 0x43a6;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f31 = *(f64*)&lbl_8047C0F0;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C0F8;
    f28 = *(f32*)&lbl_8047C0E4;
    goto L_80078AC0;
L_80078A88:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0xC) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80078AC0:
    if (f27 < f28) goto L_80078A88;
    r3 = 0x43ac;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    goto L_80078CF4;
L_80078AEC:
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x5;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f31 = *(f64*)&lbl_8047C0F0;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C0F8;
    f28 = *(f32*)&lbl_8047C0E4;
    goto L_80078B6C;
L_80078B34:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80078B6C:
    if (f27 < f28) goto L_80078B34;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_80078B90:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_80078B90;
    ((void(*)(void))fn_80075BFC)();
    ((void(*)(void))fn_80115BD8)();
    ((void(*)(void))fn_801159F0)();
    r4 = r3;
    r3 = r29;
    ((void(*)(void))fn_80130660)();
    r3 = 0x4;
    r4 = 0x2;
    r5 = 0x0;
    ((void(*)(void))fn_801D0748)();
    tmp = (s8)r3;
    if ((s32)tmp == 4) goto L_80078CAC;
    r3 = 0x4c7;
    ((void(*)(void))fn_80166A28)();
    r3 = 0x104F0000;
    r4 = 0x4;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_801CB834)();
    r3 = 0x104F0000;
    r4 = 0x1;
    r3 = r3 + 0x1000;
    ((void(*)(void))fn_801CB708)();
    f27 = *(f32*)&lbl_8047C0E0;
    f28 = *(f64*)&lbl_8047C0F0;
    r30 = 0x43300000;
    f30 = *(f64*)&lbl_8047C0F8;
    f31 = *(f32*)&lbl_8047C0E4;
    goto L_80078C58;
L_80078C20:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_80078C58:
    if (f27 < f31) goto L_80078C20;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_80078C7C:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_80078C7C;
    r3 = 0x43ac;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
    goto L_80078CF4;
L_80078CAC:
    r3 = 0x3d2;
    r4 = 0x0;
    r5 = 0xff;
    ((void(*)(void))fn_80165668)();
    r3 = 0x43a8;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x43aa;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = 0x0;
    *(u32*)&lbl_8047A620 = tmp;
L_80078CF4:
    return;
}

#pragma pop
