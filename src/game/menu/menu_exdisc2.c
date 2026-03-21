/**
 * @file menu_exdisc2.c
 * @brief Extra disc coupon and related menus (0x80078D38-0x8007C2C0)
 *
 * Address range: 0x80078D38 - 0x8007C2C0
 * Total functions: 25
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001E184();
extern void fn_80029850();
extern void fn_800298DC();
extern void fn_8002DC6C();
extern void fn_80075A9C();
extern void fn_80075AC0();
extern void fn_80075AE4();
extern void fn_80075B08();
extern void fn_80075B2C();
extern void fn_80075B50();
extern void fn_80075BC4();
extern void fn_80075C20();
extern void fn_80075C44();
extern void fn_80077ED4();
extern void fn_80078390();
extern void fn_800788BC();
extern void fn_800849B4();
extern void fn_80092C90();
extern void fn_80093574();
extern void fn_80093610();
extern void fn_80093698();
extern void fn_8009A9D8();
extern void fn_8009AAD4();
extern void fn_8009F1D0();
extern void fn_800A19CC();
extern void fn_800A1E54();
extern void fn_800A1F94();
extern void fn_800A221C();
extern void fn_800A501C();
extern void fn_800A50E4();
extern void fn_800A541C();
extern void fn_800A7BCC();
extern void fn_800C8174();
extern void fn_800CA968();
extern void fn_800D0F44();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800E4014();
extern void fn_800F0308();
extern void fn_800F9318();
extern void fn_800FA444();
extern void fn_800FB680();
extern void fn_800FF58C();
extern void fn_80102510();
extern void fn_8010264C();
extern void fn_801067E8();
extern void fn_801069FC();
extern void fn_80106D3C();
extern void fn_80109220();
extern void fn_80113828();
extern void fn_80113F48();
extern void fn_801159F0();
extern void fn_80115BD8();
extern void fn_80123FBC();
extern void fn_80124A60();
extern void fn_80128DD4();
extern void fn_80129280();
extern void fn_8012A5B0();
extern void fn_8012AC08();
extern void fn_80130770();
/* ... and 24 more external functions */
extern void OSCreateAlarm();
extern void OSDisableInterrupts();
extern void OSGetTick();
extern void OSRestoreInterrupts();
extern void OSSetAlarm();
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_804788F0;
extern u8 lbl_80478930;
extern u8 lbl_80478940;
extern u8 lbl_80478980;
extern u8 lbl_8047A620;
extern u8 lbl_8047A628;
extern u8 lbl_8047A62C;
extern u8 lbl_8047A630;
extern u8 lbl_8047A631;
extern u8 lbl_8047A632;
extern u8 lbl_8047A633;
extern u8 lbl_8047A634;
extern u8 lbl_8047A635;
extern u8 lbl_8047A638;
extern u8 lbl_8047A640;
extern u8 lbl_8047A648;
extern u8 lbl_8047A64C;
extern u8 lbl_8047A650;
extern u8 lbl_8047C0E0;
extern u8 lbl_8047C0E4;
extern u8 lbl_8047C0F0;
extern u8 lbl_8047C0F8;
extern u8 lbl_8047C100;
extern u8 lbl_8047C104;
extern u8 lbl_8047C108;
extern u8 lbl_8047C10C;
extern u8 lbl_8047C114;
extern u8 lbl_8047C118;
extern u8 lbl_8047C120;
extern u8 lbl_8047C128;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EE4D8[];
extern u8 jumptable_802EE51C[];
extern u8 jumptable_802EE550[];
extern u8 jumptable_802EE594[];
extern u8 jumptable_802EE5C0[];
extern u8 lbl_80268AA8[];
extern u8 lbl_80268AD0[];
extern u8 lbl_80268AE0[];
extern u8 lbl_802E61D8[];
extern u8 lbl_802EE508[];
extern u8 lbl_802EE608[];
extern u8 lbl_803F6E40[];
extern u8 lbl_803F6F18[];
extern u8 lbl_803F7A30[];
extern u8 lbl_803FADF8[];
extern u8 lbl_803FAEF8[];

/* ===== Forward declarations ===== */
s32 fn_80078D38(void);
s32 fn_80078D5C(void);
void fn_8007926C(void);
s32 fn_800792D8(void);
s32 fn_800798E8(void);
s32 fn_80079C1C(void);
s32 fn_80079EF4(void);
s32 fn_8007A5E8(void);
void fn_8007A664(void);
s32 fn_8007A6F0(void);
s32 fn_8007A82C(void);
s32 fn_8007A850(void);
void fn_8007AA6C(void);
s32 fn_8007AAA8(void);
void fn_8007AAFC(void);
s32 fn_8007AB10(void);
s32 fn_8007B090(void);
void fn_8007B0D8(void);
s32 fn_8007B114(void);
void fn_8007B350(void);
void fn_8007B6A4(void);
s32 fn_8007B6D8(void);
void fn_8007C23C(void);
void fn_8007C260(void);
s32 fn_8007C26C(void);

/* ===== Function implementations ===== */


/* 0x80078D38 | size: 0x24 */
s32 fn_80078D38(void) {
    fn_801C40F0();
    return 0;
}

/* 0x80078D5C | size: 0x510 */
s32 fn_80078D5C(void) {
    extern void fn_80132A38();
    extern void fn_80166A28();
    extern void fn_80176E0C();
    extern void fn_80177A44();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801CB834();
    extern void fn_801CB9D8();
    extern void fn_801CBA0C();
    extern void fn_801D0314();
    extern void fn_801D036C();
    extern void fn_801EE398();
    extern u8 jumptable_802EE4D8[];
    u8 sp[0x90];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f9 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;

    fn_801D036C();
    r31 = r3;
    goto L_80079204;
L_80078DAC: ;
    if (r0 > 0xb) goto L_80079204;
    r3 = (u32)jumptable_802EE4D8;
    r0 = r0 << 2;
    r3 = (u32)jumptable_802EE4D8;
    r0 = *(u32*)(r3 + r0);
    ctr_fn = (void(*)(void))r0;
    ((void(*)(void))fn_80113F48)();
    r4 = (0x1095 << 16);
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    r3 = (0x104f << 16);
    r4 = 0x6;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    ((void(*)(void))fn_80075C44)();
    r0 = r3 & 0xFF;
    if (r0 != 0x1) goto L_80078E18;
    r0 = 0x2;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_80078E18: ;
    r0 = 0x3;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = (0x104f << 16);
    r4 = 0x0;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x43c3;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    ((void(*)(void))fn_80075C20)();
    r0 = r3 & 0xFF;
    if (r0 != 0x1) goto L_80078EA0;
    r0 = 0x5;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_80078EA0: ;
    r0 = 0x4;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = (0x104f << 16);
    r4 = 0x1;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    f1 = *(f32*)&lbl_8047C100;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x43c0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x3f9;
    fn_80166A28();
    f27 = *(f32*)&lbl_8047C0E0;
    f28 = *(f64*)&lbl_8047C0F0;
    r30 = (0x4330 << 16);
    f30 = *(f64*)&lbl_8047C0F8;
    f31 = *(f32*)&lbl_8047C0E4;
    goto L_80078F5C;
L_80078F24: ;
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x1C) = r0;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_80078F5C: ;
    if (f27 < f31) goto L_80078F24;
    r0 = 0x0;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    fn_801EE398();
    r0 = r3 & 0xFF;
    if (r0 != 0x1) goto L_80078F8C;
    r0 = 0x7;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_80078F8C: ;
    r0 = 0x6;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    r3 = 0x43af;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    ((void(*)(void))fn_80075BC4)();
    if (r3 >= 0x1) goto L_80078FE0;
    r3 = 0x43b2;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r0 = 0x0;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_80078FE0: ;
    r4 = r3;
    r3 = 0x2f;
    fn_80132A38();
    r3 = 0x43bb;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    ((void(*)(void))fn_8001E184)();
    r30 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r0 = (s8)r30;
    if ((s32)r0 == 0x0) goto L_8007904C;
    if ((s32)r0 >= 0x0) goto L_80079028;
    if ((s32)r0 >= (s32)-0x1) goto L_80079030;
    goto L_8007904C;
L_80079028: ;
    if ((s32)r0 >= 0x2) goto L_8007904C;
L_80079030: ;
    r3 = 0x43c1;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r0 = 0x0;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_8007904C: ;
    r3 = 0x43c4;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r0 = 0x8;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    r3 = 0x43c6;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r0 = 0x0;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    r3 = r31;
    ((void(*)(void))fn_800788BC)();
    goto L_80079204;
    r3 = (u32)&lbl_80268AA8;
    r6 = (u32)&lbl_80268AA8;
    r5 = *(u32*)((u8*)r6 + 0x0);
    r4 = *(u32*)((u8*)r6 + 0x4);
    r3 = *(u32*)((u8*)r6 + 0x8);
    r0 = *(u32*)((u8*)r6 + 0xC);
    *(u32*)(sp + 0x14) = r0;
    ((void(*)(void))fn_80113F48)();
    f1 = *(f32*)&lbl_8047C104;
    r28 = r3;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r4 = (0x104f << 16);
    r3 = r28;
    r4 = r4 + 0x1000;
    ((void(*)(void))fn_800F9318)();
    r30 = r3;
    if (r30 == 0x0) goto L_80079108;
    r4 = 0x0;
    ((void(*)(void))fn_800E4014)();
L_80079108: ;
    r3 = (0xffe << 16);
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r29 = r3;
    r3 = r28;
    r4 = r29;
    ((void(*)(void))fn_800F9318)();
    r4 = (0xfff << 16);
    r3 = 0x5d5;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x1;
    fn_80176E0C();
    r3 = 0x4;
    fn_80177A44();
    f1 = *(f32*)&lbl_8047C104;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r4 = (u32)&lbl_803F6F18;
    r3 = (u32)&lbl_803F6E40;
    r0 = (u32)&lbl_803F6F18;
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0xC) = r0;
    r6 = (u32)&lbl_803F6E40;
    r3 = 0x0;
    r4 = 0x20;
    ((void(*)(void))fn_800849B4)();
    if ((s32)r3 >= 0x0) goto L_80079190;
    r0 = 0x0;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
L_80079190: ;
    f1 = *(f32*)&lbl_8047C104;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r29;
    fn_801CB9D8();
    if (r30 == 0x0) goto L_800791D8;
    r3 = r30;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
    r4 = (0x1095 << 16);
    r3 = r28;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x0;
    fn_80176E0C();
L_800791D8: ;
    f1 = *(f32*)&lbl_8047C104;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r0 = 0xa;
    *(u32*)&lbl_8047A620 = r0;
    goto L_80079204;
    ((void(*)(void))fn_80078390)();
    goto L_80079204;
    ((void(*)(void))fn_80077ED4)();
L_80079204: ;
    r0 = *(u32*)&lbl_8047A620;
    if ((s32)r0 > 0x0) goto L_80078DAC;
    r3 = r31;
    fn_801D0314();
    r3 = 0x321;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    return;
}

/* 0x8007926C | size: 0x6C */
void fn_8007926C(void) {
    extern void fn_801CB61C();
    extern void fn_801CB834();
    extern void fn_801CB954();
    extern void fn_801CBA0C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    tmp = 0x1;
    r3 = 0x10BD0000;
    *(u32*)&lbl_8047A620 = tmp;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r4 = 0x1;
    r31 = r3;
    fn_801CB954();
    r4 = 0x104F0000;
    r3 = r31;
    r4 = r4 + 0x1000;
    r5 = 0x207;
    fn_801CB61C();
    r3 = r31;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    return;
}

/* 0x800792D8 | size: 0x610 */
s32 fn_800792D8(void) {
    extern void fn_80079C1C();
    extern void fn_80079EF4();
    extern void fn_80176E0C();
    extern void fn_80177A44();
    extern void fn_80196E10();
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801CB9D8();
    extern void fn_801CBA0C();
    u8 sp[0xC20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;

    r3 = (u32)&lbl_80268AD0;
    f1 = *(f32*)&lbl_8047C108;
    r7 = (u32)&lbl_80268AD0;
    r31 = 0x0;
    r6 = *(u32*)((u8*)r7 + 0x0);
    r3 = 0x3;
    r5 = *(u32*)((u8*)r7 + 0x4);
    r4 = *(u32*)((u8*)r7 + 0x8);
    tmp = *(u32*)((u8*)r7 + 0xC);
    *(u32*)(sp + 0x14) = tmp;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0xe1;
    ((void(*)(void))fn_80102510)();
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1000;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079358;
    r4 = 0x0;
    ((void(*)(void))fn_800E4014)();
L_80079358:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1001;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079378;
    r4 = 0x0;
    ((void(*)(void))fn_800E4014)();
L_80079378:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1002;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079398;
    r4 = 0x0;
    ((void(*)(void))fn_800E4014)();
L_80079398:
    r3 = 0xFFE0000;
    r3 = r3 + 0x1000;
    fn_801CBA0C();
    r30 = r3;
    ((void(*)(void))fn_80113F48)();
    r4 = r30;
    ((void(*)(void))fn_800F9318)();
    r4 = 0xFFF0000;
    r3 = 0x5d4;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x1;
    fn_80176E0C();
    r3 = 0x4;
    fn_80177A44();
    f1 = *(f32*)&lbl_8047C108;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    tmp = (u32)sp + 0xf0;
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0xC) = tmp;
    r6 = (u32)sp + 0x18;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_800849B4)();
    if ((s32)r3 >= 0) goto L_800794C0;
    tmp = 0x1;
    f1 = *(f32*)&lbl_8047C108;
    *(u32*)&lbl_8047A638 = tmp;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r30;
    fn_801CB9D8();
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1000;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079450;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_80079450:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1001;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079470;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_80079470:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1002;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80079490;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_80079490:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x10940000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    f1 = *(f32*)&lbl_8047C108;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    goto L_800798CC;
L_800794C0:
    f1 = *(f32*)&lbl_8047C108;
    r3 = 0x3;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = r30;
    fn_801CB9D8();
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1000;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_800794FC;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_800794FC:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1001;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_8007951C;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_8007951C:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x104E0000;
    r4 = r4 + 0x1002;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_8007953C;
    r4 = 0x1;
    ((void(*)(void))fn_800E4014)();
L_8007953C:
    ((void(*)(void))fn_80113F48)();
    r4 = 0x10940000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    f1 = *(f32*)&lbl_8047C108;
    r3 = 0x2;
    fn_801C41C8();
    r3 = 0x1;
    fn_801C40F0();
    r3 = 0x0;
    r10 = tmp & 0x00000004;
    r8 = tmp & 0x00000002;
    r6 = tmp & 0x1;
    r9 = -r10;
    r7 = -r8;
    r5 = -r6;
    *(u32*)&lbl_8047A62C = tmp;
    tmp = r5 | r6;
    r9 = r9 | r10;
    r7 = r7 | r8;
    *(u32*)&lbl_8047A628 = r4;
    r6 = (u32)r9 >> 31;
    tmp = (u32)tmp >> 31;
    r5 = (u32)r7 >> 31;
    *(u8*)&lbl_8047A635 = r6;
    *(u8*)&lbl_8047A634 = r5;
    *(u8*)&lbl_8047A633 = tmp;
    fn_80079EF4();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_800798CC;
    tmp = *(u8*)&lbl_8047A632;
    r29 = 0x1;
    if (tmp == 0) goto L_8007962C;
    r30 = (u32)sp + 0x28;
    r4 = *(u16*)(sp + 0x26);
    r3 = r30;
    r5 = 0x47;
    r6 = 0x3e7;
    ((void(*)(void))fn_80029850)();
    if ((s32)r3 >= 1) goto L_80079604;
    r29 = 0x0;
    goto L_8007962C;
L_80079604:
    r4 = *(u16*)(sp + 0x26);
    r3 = r30;
    r5 = 0x47;
    r6 = 0x1;
    r7 = -0x1;
    r8 = 0x3e7;
    ((void(*)(void))fn_800298DC)();
    tmp = tmp | 0x4;
    *(u32*)(sp + 0x20) = tmp;
L_8007962C:
    tmp = *(u8*)&lbl_8047A630;
    if (tmp == 0) goto L_80079658;
    r4 = *(u16*)(sp + 0x26);
    r3 = (u32)sp + 0x28;
    r5 = 0x1;
    r6 = 0x3e7;
    ((void(*)(void))fn_80029850)();
    if ((s32)r3 >= 1) goto L_80079658;
    r29 = 0x0;
L_80079658:
    tmp = *(u8*)&lbl_8047A631;
    r30 = 0x1;
    if (tmp == 0) goto L_80079678;
    r3 = *(u16*)(sp + 0x24);
    tmp = (u32)tmp >> 31;
    r30 = tmp;
L_80079678:
    r3 = (u32)sp + 0xf0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    r6 = r3;
    r4 = r29;
    r5 = r30;
    r3 = 0x0;
    fn_80079C1C();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_800798CC;
    r3 = 0x2;
    r4 = 0x3d3b;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80106D3C)();
    tmp = *(u8*)&lbl_8047A631;
    if (tmp == 0) goto L_80079774;
    r29 = 0x0;
    goto L_800796E4;
L_800796D0:
    r4 = r29;
    r3 = (u32)sp + 0xf0;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_80124A60)();
    r29 = r29 + 0x1;
L_800796E4:
    tmp = r29 & 0xFFFF;
    if (tmp < 6) goto L_800796D0;
    ((void(*)(void))fn_80115BD8)();
    ((void(*)(void))fn_801159F0)();
    r4 = r3;
    r3 = (u32)sp + 0xf0;
    ((void(*)(void))fn_80130770)();
    r30 = 0x0;
    goto L_80079740;
L_8007970C:
    r4 = r30;
    r3 = (u32)sp + 0xf0;
    ((void(*)(void))fn_8012AC08)();
    ((void(*)(void))fn_80123FBC)();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_8007973C;
    r4 = r30;
    r3 = (u32)sp + 0xf0;
    ((void(*)(void))fn_8012AC08)();
    r31 = r3;
    goto L_8007974C;
L_8007973C:
    r30 = r30 + 0x1;
L_80079740:
    tmp = r30 & 0xFFFF;
    if (tmp < 6) goto L_8007970C;
L_8007974C:
    if (r31 != 0) goto L_80079768;
    r3 = (u32)&lbl_80268AE0;
    r4 = 0x460;
    r3 = (u32)&lbl_80268AE0;
    r5 = (u32)&lbl_8047C10C;
    fn_80196E10();
L_80079768:
    tmp = tmp | 0x2;
    *(u32*)(sp + 0x20) = tmp;
L_80079774:
    tmp = *(u8*)&lbl_8047A630;
    if (tmp == 0) goto L_800797A8;
    r4 = *(u16*)(sp + 0x26);
    r3 = (u32)sp + 0x28;
    r5 = 0x1;
    r6 = 0x1;
    r7 = -0x1;
    r8 = 0x3e7;
    ((void(*)(void))fn_800298DC)();
    tmp = tmp | 0x1;
    *(u32*)(sp + 0x20) = tmp;
L_800797A8:
    r3 = 0x1;
    ((void(*)(void))fn_80093574)();
    r5 = r31;
    r4 = (u32)sp + 0x18;
    r3 = 0x1;
    ((void(*)(void))fn_80092C90)();
    r3 = 0x1;
    ((void(*)(void))fn_80093574)();
    r3 = 0x1;
    ((void(*)(void))fn_80093610)();
    if ((s32)r3 == 0xc) goto L_8007982C;
    r3 = 0x1;
    ((void(*)(void))fn_80093698)();
    r3 = 0x2;
    r4 = 0x3d85;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_800798CC;
L_8007982C:
    r3 = 0x1;
    ((void(*)(void))fn_80093698)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x43c5;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    ((void(*)(void))fn_8001E184)();
    tmp = (s8)r3;
    if ((s32)tmp == 0) goto L_800798A0;
    if ((s32)tmp >= 0) goto L_8007986C;
    if ((s32)tmp >= (s32)-0x1) goto L_80079874;
    goto L_800798A0;
L_8007986C:
    if ((s32)tmp >= 2) goto L_800798A0;
L_80079874:
    r3 = 0x43ca;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_800798CC;
L_800798A0:
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
L_800798CC:
    return;
}

/* 0x800798E8 | size: 0x334 */
s32 fn_800798E8(void) {
    extern void fn_80079C1C();
    extern void fn_80079EF4();
    extern void fn_80134420();
    extern void fn_8013467C();
    extern void fn_80135168();
    extern void fn_801D0748();
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
    r3 = 0x2;
    r4 = 0x2;
    r5 = 0x0;
    fn_801D0748();
    r29 = r3;
    if ((s32)r29 != 3) goto L_8007992C;
    r3 = 0x0;
    r4 = 0x4;
    fn_80135168();
    if (r3 != 0) goto L_80079964;
L_8007992C:
    if ((s32)r29 == (s32)-0x1) goto L_80079950;
    r3 = 0x2;
    r4 = 0x44db;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079950:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079C08;
L_80079964:
    ((void(*)(void))fn_80075AC0)();
    *(u8*)&lbl_8047A635 = r3;
    ((void(*)(void))fn_80075B08)();
    *(u8*)&lbl_8047A634 = r3;
    ((void(*)(void))fn_80075B50)();
    *(u8*)&lbl_8047A633 = r3;
    r3 = 0x0;
    r4 = 0xe;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    tmp = r3;
    r3 = 0x1;
    r4 = tmp;
    fn_80079EF4();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_80079C08;
    r31 = 0x1;
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    ((void(*)(void))fn_80128DD4)();
    tmp = *(u8*)&lbl_8047A632;
    r30 = r3;
    if (tmp == 0) goto L_800799E4;
    r4 = 0x47;
    fn_80134420();
    tmp = r3 & 0xFFFF;
    if (tmp >= 1) goto L_800799E4;
    r31 = 0x0;
L_800799E4:
    tmp = *(u8*)&lbl_8047A630;
    if (tmp == 0) goto L_80079A0C;
    r3 = r30;
    r4 = 0x1;
    fn_80134420();
    tmp = r3 & 0xFFFF;
    if (tmp >= 1) goto L_80079A0C;
    r31 = 0x0;
L_80079A0C:
    r27 = 0x1;
    r3 = 0x0;
    r4 = 0x2;
    ((void(*)(void))fn_80129280)();
    tmp = *(u8*)&lbl_8047A631;
    r29 = r3;
    if (tmp == 0) goto L_80079A74;
    r27 = 0x0;
    goto L_80079A60;
L_80079A34:
    r3 = r29;
    r5 = r27 & 0xFF;
    r4 = 0x3;
    ((void(*)(void))fn_8012A5B0)();
    ((void(*)(void))fn_80123FBC)();
    tmp = r3 & 0xFF;
    if (tmp == 1) goto L_80079A5C;
    tmp = 0x1;
    goto L_80079A70;
L_80079A5C:
    r27 = r27 + 0x1;
L_80079A60:
    tmp = r27 & 0xFF;
    if (tmp < 6) goto L_80079A34;
    tmp = 0x0;
L_80079A70:
    r27 = tmp;
L_80079A74:
    r3 = 0x0;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
    tmp = r3;
    r3 = 0x0;
    r26 = tmp;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_80079AA8:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_80079AA8;
    r4 = r31;
    r5 = r27;
    r6 = r26;
    r3 = 0x1;
    fn_80079C1C();
    tmp = r3 & 0xFF;
    if (tmp == 0) goto L_80079C08;
    tmp = *(u8*)&lbl_8047A632;
    if (tmp == 0) goto L_80079AFC;
    r3 = r30;
    r4 = 0x47;
    r5 = 0x1;
    fn_8013467C();
    ((void(*)(void))fn_80075A9C)();
L_80079AFC:
    tmp = *(u8*)&lbl_8047A631;
    if (tmp == 0) goto L_80079B20;
    ((void(*)(void))fn_80115BD8)();
    ((void(*)(void))fn_801159F0)();
    r4 = r3;
    r3 = r29;
    ((void(*)(void))fn_80130770)();
    ((void(*)(void))fn_80075AE4)();
L_80079B20:
    tmp = *(u8*)&lbl_8047A630;
    if (tmp == 0) goto L_80079B40;
    r3 = r30;
    r4 = 0x1;
    r5 = 0x1;
    fn_8013467C();
    ((void(*)(void))fn_80075B2C)();
L_80079B40:
    r3 = 0x4;
    r4 = 0x2;
    r5 = 0x0;
    fn_801D0748();
    if ((s32)r3 == 4) goto L_80079B9C;
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    r3 = 0x0;
    r4 = 0x0;
    ((void(*)(void))fn_80129280)();
    tmp = 0x3bfa;
    ctr_fn = (void(*)(void))tmp;
L_80079B7C:
    r3 = *(u32*)((u8*)r4 + 0x4);
    tmp = *(u32*)((u8*)r4 + 0x8);
    *(u32*)((u8*)r5 + 0x4) = r3;
    r5 += 8; *(u32*)r5 = tmp;
    if (--ctr != 0) goto L_80079B7C;
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079C08;
L_80079B9C:
    r3 = 0x43c5;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    ((void(*)(void))fn_8001E184)();
    tmp = (s8)r3;
    if ((s32)tmp == 0) goto L_80079BF8;
    if ((s32)tmp >= 0) goto L_80079BCC;
    if ((s32)tmp >= (s32)-0x1) goto L_80079BD4;
    goto L_80079BF8;
L_80079BCC:
    if ((s32)tmp >= 2) goto L_80079BF8;
L_80079BD4:
    r3 = 0x43c8;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079C08;
L_80079BF8:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
L_80079C08:
    return;
}

/* 0x80079C1C | size: 0x2D8 */
s32 fn_80079C1C(void) {
    extern void fn_80132A38();
    extern void fn_80165668();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = r6;
    tmp = r4 & 0xFF;
    if (tmp != 0) goto L_80079CBC;
    tmp = r5 & 0xFF;
    if (tmp != 0) goto L_80079CBC;
    r3 = 0x43d2;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    r3 = 0x43d3;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r30 != 0) goto L_80079CA4;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079CA4:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079EDC;
L_80079CBC:
    tmp = r4 & 0xFF;
    if (tmp != 0) goto L_80079D1C;
    r3 = 0x43d2;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r30 != 0) goto L_80079D04;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079D04:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079EDC;
L_80079D1C:
    tmp = r5 & 0xFF;
    if (tmp != 0) goto L_80079D7C;
    r3 = 0x43d3;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r30 != 0) goto L_80079D64;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079D64:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_80079EDC;
L_80079D7C:
    tmp = *(u8*)&lbl_8047A632;
    if (tmp == 0) goto L_80079DF4;
    r3 = 0x2d;
    r4 = 0x47;
    fn_80132A38();
    r3 = 0x3ca;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    if ((s32)r30 == 1) goto L_80079DBC;
    if ((s32)r30 >= 1) goto L_80079DEC;
    if ((s32)r30 >= 0) goto L_80079DD0;
    goto L_80079DEC;
L_80079DBC:
    r3 = 0x43ad;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    goto L_80079DEC;
L_80079DD0:
    r4 = r31;
    r3 = 0x4d;
    fn_80132A38();
    r3 = 0x4436;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
L_80079DEC:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079DF4:
    tmp = *(u8*)&lbl_8047A631;
    if (tmp == 0) goto L_80079E60;
    r3 = 0x3d2;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    if ((s32)r30 == 1) goto L_80079E28;
    if ((s32)r30 >= 1) goto L_80079E58;
    if ((s32)r30 >= 0) goto L_80079E3C;
    goto L_80079E58;
L_80079E28:
    r3 = 0x4437;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    goto L_80079E58;
L_80079E3C:
    r4 = r31;
    r3 = 0x4d;
    fn_80132A38();
    r3 = 0x443b;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
L_80079E58:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079E60:
    tmp = *(u8*)&lbl_8047A630;
    if (tmp == 0) goto L_80079ED8;
    r3 = 0x2d;
    r4 = 0x1;
    fn_80132A38();
    r3 = 0x3ca;
    r4 = 0x0;
    r5 = 0xff;
    fn_80165668();
    if ((s32)r30 == 1) goto L_80079EA0;
    if ((s32)r30 >= 1) goto L_80079ED0;
    if ((s32)r30 >= 0) goto L_80079EB4;
    goto L_80079ED0;
L_80079EA0:
    r3 = 0x43ad;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    goto L_80079ED0;
L_80079EB4:
    r4 = r31;
    r3 = 0x4d;
    fn_80132A38();
    r3 = 0x4436;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
L_80079ED0:
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_80079ED8:
    r3 = 0x1;
L_80079EDC:
    return;
}

/* 0x80079EF4 | size: 0x6F4 */
s32 fn_80079EF4(void) {
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
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    r5 = *(u32*)&lbl_804788F0;
    r6 = 0x0;
    r3 = (u32)&lbl_802E61D8;
    *(u8*)&lbl_8047A630 = r6;
    *(u8*)&lbl_8047A631 = r6;
    r5 = r29 << 2;
    tmp = (u32)&lbl_802E61D8;
    *(u8*)&lbl_8047A632 = r6;
    r3 = tmp + r5;
    tmp = r29 + 0x1;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r29 < 0) goto L_80079F88;
L_80079F70:
    tmp = *(u32*)((u8*)r3 + 0x0);
    if (tmp <= r4) goto L_80079F88;
    if (--ctr != 0) goto L_80079F70;
L_80079F88:
    if ((s32)r29 >= 0) goto L_80079F94;
    r29 = 0x0;
L_80079F94:
    r3 = 0xe1;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C128;
    goto L_80079FEC;
L_80079FB4:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0xC) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_80079FEC:
    if (f27 < f28) goto L_80079FB4;
    r3 = 0xef;
    r4 = 0x0;
    ((void(*)(void))fn_8010264C)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A050;
L_8007A018:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A050:
    if (f27 < f28) goto L_8007A018;
    if ((s32)r29 >= 1) goto L_8007A10C;
    r3 = 0x43a7;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r31 != 0) goto L_8007A09C;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_8007A09C:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A0F4;
L_8007A0BC:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A0F4:
    if (f27 < f28) goto L_8007A0BC;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A10C:
    if ((s32)r29 == 2) goto L_8007A204;
    if ((s32)r29 >= 2) goto L_8007A124;
    if ((s32)r29 >= 1) goto L_8007A130;
    goto L_8007A450;
L_8007A124:
    if ((s32)r29 >= 4) goto L_8007A450;
    goto L_8007A30C;
L_8007A130:
    tmp = *(u8*)&lbl_8047A635;
    if (tmp == 0) goto L_8007A1E8;
    r3 = 0x43ae;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r31 != 0) goto L_8007A178;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_8007A178:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A1D0;
L_8007A198:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A1D0:
    if (f27 < f28) goto L_8007A198;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A1E8:
    r3 = 0x43b4;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A632 = tmp;
    goto L_8007A4C8;
L_8007A204:
    r3 = *(u8*)&lbl_8047A635;
    if (r3 == 0) goto L_8007A2C8;
    tmp = *(u8*)&lbl_8047A634;
    if (tmp == 0) goto L_8007A2C8;
    r3 = 0x43ab;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r31 != 0) goto L_8007A258;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_8007A258:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A2B0;
L_8007A278:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A2B0:
    if (f27 < f28) goto L_8007A278;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A2C8:
    if (r3 == 0) goto L_8007A2EC;
    r3 = 0x43b3;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A631 = tmp;
    goto L_8007A4C8;
L_8007A2EC:
    r3 = 0x43b6;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A632 = tmp;
    *(u8*)&lbl_8047A631 = tmp;
    goto L_8007A4C8;
L_8007A30C:
    r3 = *(u8*)&lbl_8047A635;
    if (r3 == 0) goto L_8007A3DC;
    tmp = *(u8*)&lbl_8047A634;
    if (tmp == 0) goto L_8007A3DC;
    tmp = *(u8*)&lbl_8047A633;
    if (tmp == 0) goto L_8007A3DC;
    r3 = 0x43a9;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r31 != 0) goto L_8007A36C;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_8007A36C:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A3C4;
L_8007A38C:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A3C4:
    if (f27 < f28) goto L_8007A38C;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A3DC:
    tmp = *(u8*)&lbl_8047A634;
    if (tmp == 0) goto L_8007A404;
    r3 = 0x43b1;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A630 = tmp;
    goto L_8007A4C8;
L_8007A404:
    if (r3 == 0) goto L_8007A42C;
    r3 = 0x43b5;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A630 = tmp;
    *(u8*)&lbl_8047A631 = tmp;
    goto L_8007A4C8;
L_8007A42C:
    r3 = 0x43c2;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    tmp = 0x1;
    *(u8*)&lbl_8047A630 = tmp;
    *(u8*)&lbl_8047A631 = tmp;
    *(u8*)&lbl_8047A632 = tmp;
    goto L_8007A4C8;
L_8007A450:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    f27 = *(f32*)&lbl_8047C114;
    f31 = *(f64*)&lbl_8047C118;
    r30 = 0x43300000;
    f29 = *(f64*)&lbl_8047C120;
    f28 = *(f32*)&lbl_8047C108;
    goto L_8007A4B0;
L_8007A478:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f30 = f0 - f31;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f29;
    f0 = f0 / f30;
    f27 = f27 + f0;
L_8007A4B0:
    if (f27 < f28) goto L_8007A478;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A4C8:
    r3 = 0x43d1;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    ((void(*)(void))fn_8001E184)();
    r30 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    tmp = (s8)r30;
    if ((s32)tmp == 0) goto L_8007A5A0;
    if ((s32)tmp >= 0) goto L_8007A504;
    if ((s32)tmp >= (s32)-0x1) goto L_8007A50C;
    goto L_8007A5A0;
L_8007A504:
    if ((s32)tmp >= 2) goto L_8007A5A0;
L_8007A50C:
    if ((s32)r31 != 0) goto L_8007A530;
    r3 = 0x2;
    r4 = 0x44cf;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
L_8007A530:
    r3 = 0xef;
    ((void(*)(void))fn_80102510)();
    f27 = *(f32*)&lbl_8047C114;
    f28 = *(f64*)&lbl_8047C118;
    r31 = 0x43300000;
    f30 = *(f64*)&lbl_8047C120;
    f31 = *(f32*)&lbl_8047C108;
    goto L_8007A588;
L_8007A550:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0x14) = tmp;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_8007A588:
    if (f27 < f31) goto L_8007A550;
    tmp = 0x1;
    r3 = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007A5A4;
L_8007A5A0:
    r3 = 0x1;
L_8007A5A4:
    return;
}

/* 0x8007A5E8 | size: 0x7C */
s32 fn_8007A5E8(void) {
    extern void fn_80132A38();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    r31 = r4;
    tmp = *(u32*)&lbl_8047A638;
    if ((s32)tmp != 4) goto L_8007A610;
    r3 = *(u32*)&lbl_8047A62C;
    goto L_8007A620;
L_8007A610:
    r3 = 0x0;
    r4 = 0xd;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
L_8007A620:
    r4 = r3;
    r3 = 0x50;
    fn_80132A38();
    r3 = 0x153;
    ((void(*)(void))fn_800FA444)();
    tmp = *(s16*)((u8*)r31 + 0x54);
    r3 = (u32)r3 >> 16;
    r4 = 0x0;
    r5 = -0x1;
    r3 = tmp - r3;
    r6 = 0x153;
    ((void(*)(void))fn_800FB680)();
    return;
}

/* 0x8007A664 | size: 0x8C */
void fn_8007A664(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    tmp = *(u32*)&lbl_8047A638;
    if ((s32)tmp == 4) goto L_8007A68C;
    if ((s32)tmp >= 4) goto L_8007A6E0;
    if ((s32)tmp >= 3) goto L_8007A6B8;
    goto L_8007A6E0;
L_8007A68C:
    tmp = *(s16*)((u8*)r4 + 0x6);
    if ((s32)tmp != 0x10bf) goto L_8007A6A8;
    r3 = r4;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
    goto L_8007A6E0;
L_8007A6A8:
    r3 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_80109220)();
    goto L_8007A6E0;
L_8007A6B8:
    tmp = *(s16*)((u8*)r4 + 0x6);
    if ((s32)tmp != 0x10c0) goto L_8007A6D4;
    r3 = r4;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
    goto L_8007A6E0;
L_8007A6D4:
    r3 = r4;
    r4 = 0x0;
    ((void(*)(void))fn_80109220)();
L_8007A6E0:
    return;
}

/* 0x8007A6F0 | size: 0x13C */
s32 fn_8007A6F0(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r30 = r4;
    tmp = *(u32*)&lbl_8047A638;
    if ((s32)tmp != 4) goto L_8007A71C;
    r3 = *(u32*)&lbl_8047A628;
    goto L_8007A72C;
L_8007A71C:
    r3 = 0x0;
    r4 = 0xe;
    r5 = 0x0;
    ((void(*)(void))fn_8012A5B0)();
L_8007A72C:
    r5 = *(u32*)&lbl_804788F0;
    r4 = (u32)&lbl_802E61D8;
    tmp = (u32)&lbl_802E61D8;
    r4 = r31 << 2;
    r4 = tmp + r4;
    tmp = r31 + 0x1;
    ctr_fn = (void(*)(void))tmp;
    if ((s32)r31 < 0) goto L_8007A76C;
L_8007A754:
    tmp = *(u32*)((u8*)r4 + 0x0);
    if (tmp <= r3) goto L_8007A76C;
    if (--ctr != 0) goto L_8007A754;
L_8007A76C:
    if ((s32)r31 >= 0) goto L_8007A778;
    r31 = 0x0;
L_8007A778:
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))fn_80109220)();
    if ((s32)r31 == 2) goto L_8007A7C4;
    if ((s32)r31 >= 2) goto L_8007A79C;
    if ((s32)r31 >= 1) goto L_8007A7A8;
    goto L_8007A7FC;
L_8007A79C:
    if ((s32)r31 >= 4) goto L_8007A7FC;
    goto L_8007A7E0;
L_8007A7A8:
    tmp = *(s16*)((u8*)r30 + 0x6);
    if ((s32)tmp != 0x10c3) goto L_8007A814;
    r3 = r30;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
    goto L_8007A814;
L_8007A7C4:
    tmp = *(s16*)((u8*)r30 + 0x6);
    if ((s32)tmp != 0x10c4) goto L_8007A814;
    r3 = r30;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
    goto L_8007A814;
L_8007A7E0:
    tmp = *(s16*)((u8*)r30 + 0x6);
    if ((s32)tmp != 0x10c5) goto L_8007A814;
    r3 = r30;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
    goto L_8007A814;
L_8007A7FC:
    tmp = *(s16*)((u8*)r30 + 0x6);
    if ((s32)tmp != 0x10c2) goto L_8007A814;
    r3 = r30;
    r4 = 0x1;
    ((void(*)(void))fn_80109220)();
L_8007A814:
    return;
}

/* 0x8007A82C | size: 0x24 */
s32 fn_8007A82C(void) {
    fn_801C40F0();
    return 0;
}

/* 0x8007A850 | size: 0x21C */
s32 fn_8007A850(void) {
    extern void fn_800792D8();
    extern void fn_800798E8();
    extern void fn_801C40F0();
    extern void fn_801D0314();
    extern void fn_801D036C();
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f27 = 0.0f;
    f32 f28 = 0.0f;
    f32 f29 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    fn_801D036C();
    tmp = r3;
    r3 = 0x1;
    r30 = tmp;
    fn_801C40F0();
    goto L_8007AA0C;
L_8007A8A4:
    if ((s32)tmp == 3) goto L_8007A9FC;
    if ((s32)tmp >= 3) goto L_8007A8C0;
    if ((s32)tmp == 1) goto L_8007A8CC;
    if ((s32)tmp >= 1) goto L_8007A980;
    goto L_8007AA0C;
L_8007A8C0:
    if ((s32)tmp >= 5) goto L_8007AA0C;
    goto L_8007AA08;
L_8007A8CC:
    r3 = 0x43cf;
    r4 = 0x0;
    r5 = 0x1;
    ((void(*)(void))fn_801067E8)();
    r3 = 0xe1;
    r4 = 0x1;
    ((void(*)(void))fn_8010264C)();
    r31 = r3;
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    if ((s32)r31 == 1) goto L_8007A92C;
    if ((s32)r31 >= 1) goto L_8007A910;
    if ((s32)r31 == (s32)-0x1) goto L_8007A958;
    if ((s32)r31 >= (s32)-0x1) goto L_8007A920;
    goto L_8007A96C;
L_8007A910:
    if ((s32)r31 == 3) goto L_8007A944;
    if ((s32)r31 >= 3) goto L_8007A96C;
    goto L_8007A938;
L_8007A920:
    tmp = 0x3;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A92C:
    tmp = 0x4;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A938:
    tmp = 0x2;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A944:
    r3 = 0xe1;
    ((void(*)(void))fn_80102510)();
    tmp = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A958:
    r3 = 0xe1;
    ((void(*)(void))fn_80102510)();
    tmp = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A96C:
    r3 = 0xe1;
    ((void(*)(void))fn_80102510)();
    tmp = 0x0;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A980:
    r3 = 0x43a5;
    r4 = 0x1;
    r5 = 0x0;
    ((void(*)(void))fn_801067E8)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    f27 = *(f32*)&lbl_8047C114;
    f28 = *(f64*)&lbl_8047C118;
    r31 = 0x43300000;
    f30 = *(f64*)&lbl_8047C120;
    f31 = *(f32*)&lbl_8047C108;
    goto L_8007A9E8;
L_8007A9B0:
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D37CC)();
    *(u32*)(sp + 0xC) = tmp;
    f29 = f0 - f28;
    ((void(*)(void))fn_800D3088)();
    f0 = f0 - f30;
    f0 = f0 / f29;
    f27 = f27 + f0;
L_8007A9E8:
    if (f27 < f31) goto L_8007A9B0;
    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
    goto L_8007AA0C;
L_8007A9FC:
    r3 = r30;
    fn_800798E8();
    goto L_8007AA0C;
L_8007AA08:
    fn_800792D8();
L_8007AA0C:
    tmp = *(u32*)&lbl_8047A638;
    if ((s32)tmp > 0) goto L_8007A8A4;
    r3 = r30;
    fn_801D0314();
    r3 = 0x321;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    return;
}

/* 0x8007AA6C | size: 0x3C */
void fn_8007AA6C(void) {
    extern void fn_80176E0C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    tmp = 0x1;
    *(u32*)&lbl_8047A638 = tmp;
    ((void(*)(void))fn_80113F48)();
    r4 = 0x10940000;
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    fn_80176E0C();
    return;
}

/* 0x8007AAA8 | size: 0x54 */
s32 fn_8007AAA8(void) {
    extern void fn_8007B0D8();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r3 = (u32)&lbl_803F7A30;
    tmp = 0x1;
    r3 = (u32)&lbl_803F7A30;
    *(u8*)((u8*)r3 + 0x342) = tmp;
L_8007AAC4:
    tmp = *(u8*)((u8*)r3 + 0x345);
    if (tmp == 0) goto L_8007AAC4;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)sp + 0x8;
    r3 = (u32)&lbl_803F7A30;
    r3 = r3 + 0x28;
    ((void(*)(void))fn_800A1E54)();
    fn_8007B0D8();
    r3 = 0x0;
    return;
}

/* 0x8007AAFC | size: 0x14 */
void fn_8007AAFC(void) {
}

/* 0x8007AB10 | size: 0x580 */
s32 fn_8007AB10(void) {
    extern void fn_8007B350();
    extern u8 jumptable_802EE51C[];
    extern u8 jumptable_802EE550[];
    extern u8 jumptable_802EE594[];
    extern u8 jumptable_802EE5C0[];
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    r31 = r3;
    r30 = r4;
    if ((s32)r31 != 0) goto L_8007AB40;
    r3 = 0x0;
    goto L_8007B074;
L_8007AB40:
    tmp = *(u32*)((u8*)r30 + 0x0);
    if ((s32)tmp != 0) goto L_8007ABF0;
    r3 = 0x1;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_8007ABC0;
    if (r31 > 0x10) goto L_8007ABB8;
    r3 = (u32)jumptable_802EE5C0;
    tmp = r31 << 2;
    r3 = (u32)jumptable_802EE5C0;
    tmp = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))tmp;
    r3 = 0x2;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0x7;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0xb;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0x13;
    goto L_8007B074;
L_8007ABB8:
    r3 = 0x0;
    goto L_8007B074;
L_8007ABC0:
    tmp = 0x1;
    r3 = (u32)&lbl_803F7A30;
    *(u32*)((u8*)r30 + 0x0) = tmp;
    r5 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r3 = (u32)&lbl_803F7A30;
    r8 = *(u8*)&lbl_80478930;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
L_8007ABF0:
    r3 = (u32)&lbl_803F7A30;
    r29 = (u32)&lbl_803F7A30;
    tmp = *(u8*)((u8*)r29 + 0x345);
    if (tmp == 0) goto L_8007B054;
    r3 = r29 + 0x28;
    r4 = (u32)sp + 0x8;
    ((void(*)(void))fn_800A1E54)();
    tmp = 0x0;
    r3 = 0x1;
    *(u8*)((u8*)r29 + 0x345) = tmp;
    ((void(*)(void))fn_800D0F44)();
    /* subis tmp, r3, 0x4 */;
    if (tmp == 0) goto L_8007AC34;
    tmp = 0xa;
    goto L_8007AC44;
L_8007AC34:
    if ((s32)tmp != 0) goto L_8007AC44;
    tmp = 0x0;
L_8007AC44:
    if (tmp > 0xa) goto L_8007B070;
    r3 = (u32)jumptable_802EE594;
    tmp = tmp << 2;
    r3 = (u32)jumptable_802EE594;
    tmp = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))tmp;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if (r31 > 0x10) goto L_8007B070;
    r3 = (u32)jumptable_802EE550;
    tmp = r31 << 2;
    r3 = (u32)jumptable_802EE550;
    tmp = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))tmp;
    r3 = 0x2;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0x7;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0xb;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    r3 = 0x13;
    goto L_8007B074;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if ((s32)r31 != 0xf) goto L_8007B070;
    r3 = 0x10;
    goto L_8007B074;
    if ((s32)r31 != 0x12) goto L_8007AD64;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x2;
    fn_8007B350();
    if ((s32)r31 != 0x12) goto L_8007AD5C;
    r3 = 0x0;
    goto L_8007B074;
L_8007AD5C:
    r3 = 0x12;
    goto L_8007B074;
L_8007AD64:
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if (r31 > 0xc) goto L_8007B070;
    r3 = (u32)jumptable_802EE51C;
    tmp = r31 << 2;
    r3 = (u32)jumptable_802EE51C;
    tmp = *(u32*)(r3 + tmp);
    ctr_fn = (void(*)(void))tmp;
    r3 = 0x10;
    goto L_8007B074;
    r3 = 0x11;
    goto L_8007B074;
    r3 = 0xb;
    goto L_8007B074;
    r3 = 0x4;
    goto L_8007B074;
    if ((s32)r31 == 3) goto L_8007ADEC;
    if ((s32)r31 == 0xa) goto L_8007ADEC;
    if ((s32)r31 == 0xc) goto L_8007ADEC;
    if ((s32)r31 != 0xe) goto L_8007AE2C;
L_8007ADEC:
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x1;
    fn_8007B350();
    if ((s32)r31 != 0xe) goto L_8007AE24;
    r3 = 0x0;
    goto L_8007B074;
L_8007AE24:
    r3 = 0xe;
    goto L_8007B074;
L_8007AE2C:
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if ((s32)r31 == 6) goto L_8007AE80;
    if ((s32)r31 >= 6) goto L_8007AE6C;
    if ((s32)r31 == 1) goto L_8007AE78;
    goto L_8007B070;
L_8007AE6C:
    if ((s32)r31 == 8) goto L_8007AE88;
    goto L_8007B070;
L_8007AE78:
    r3 = 0x2;
    goto L_8007B074;
L_8007AE80:
    r3 = 0x7;
    goto L_8007B074;
L_8007AE88:
    r3 = 0x4;
    goto L_8007B074;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r7 = (u32)&lbl_802EE508;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    tmp = 0x0;
    if ((s32)r31 == 1) goto L_8007AF04;
    if ((s32)r31 == 3) goto L_8007AF04;
    if ((s32)r31 == 6) goto L_8007AF04;
    if ((s32)r31 == 8) goto L_8007AF04;
    if ((s32)r31 == 0xa) goto L_8007AF04;
    if ((s32)r31 == 0xc) goto L_8007AF04;
    if ((s32)r31 == 0xe) goto L_8007AF04;
    if ((s32)r31 == 0xf) goto L_8007AF04;
    if ((s32)r31 == 0x11) goto L_8007AF04;
    if ((s32)r31 != 0x12) goto L_8007AF08;
L_8007AF04:
    tmp = 0x1;
L_8007AF08:
    if ((s32)tmp == 0) goto L_8007AF18;
    r9 = 0x0;
    goto L_8007AF1C;
L_8007AF18:
    r9 = 0x3;
L_8007AF1C:
    fn_8007B350();
    goto L_8007B070;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    r3 = 0x14;
    goto L_8007B074;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if ((s32)r31 == 0x15) goto L_8007B070;
    r3 = 0x15;
    goto L_8007B074;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if ((s32)r31 == 0x16) goto L_8007B070;
    r3 = 0x16;
    goto L_8007B074;
    r3 = (u32)&lbl_803F7A30;
    r4 = (u32)&lbl_802EE508;
    r7 = (u32)&lbl_802EE508;
    r8 = *(u8*)&lbl_80478930;
    r3 = (u32)&lbl_803F7A30;
    r4 = 0x1;
    r5 = (u32)&lbl_8047A640;
    r6 = 0x4a;
    r9 = 0x3;
    fn_8007B350();
    if ((s32)r31 == 0x17) goto L_8007B070;
    r3 = 0x17;
    goto L_8007B074;
    r3 = 0x0;
    *(u32*)((u8*)r30 + 0x0) = r3;
    if (tmp <= 1) goto L_8007B020;
    if ((s32)r31 == 0x11) goto L_8007B020;
    if ((s32)r31 != 0x12) goto L_8007B070;
L_8007B020:
    r3 = 0x13;
    goto L_8007B074;
    r3 = 0x0;
    *(u32*)((u8*)r30 + 0x0) = r3;
    if (tmp <= 1) goto L_8007B04C;
    if ((s32)r31 == 0x11) goto L_8007B04C;
    if ((s32)r31 != 0x12) goto L_8007B070;
L_8007B04C:
    r3 = 0x13;
    goto L_8007B074;
L_8007B054:
    tmp = *(u8*)((u8*)r29 + 0x346);
    if ((s32)tmp != 2) goto L_8007B070;
    if ((s32)r31 != 0xe) goto L_8007B070;
    r3 = 0xf;
    goto L_8007B074;
L_8007B070:
    r3 = 0x0;
L_8007B074:
    return;
}

/* 0x8007B090 | size: 0x48 */
s32 fn_8007B090(void) {
    extern void fn_8007B114();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r31 = 0;

    r31 = r3;
    ((void(*)(void))fn_800A7BCC)();
    tmp = r3;
    r3 = (u32)&lbl_8047A640;
    r4 = tmp;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = r31;
    fn_8007B114();
    return;
}

/* 0x8007B0D8 | size: 0x3C */
void fn_8007B0D8(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;

    r4 = *(u32*)&lbl_8047A648;
    if (r4 == 0) goto L_8007B104;
    r3 = *(u32*)&lbl_80478980;
    ((void(*)(void))fn_8009AAD4)();
    tmp = 0x0;
    *(u32*)&lbl_8047A648 = tmp;
    *(u32*)&lbl_8047A64C = tmp;
L_8007B104:
    return;
}

/* 0x8007B114 | size: 0x23C */
s32 fn_8007B114(void) {
    u8 sp[0x90];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r3 = (u32)&lbl_803FAEF8;
    r4 = 0x0;
    r6 = (u32)&lbl_803FAEF8;
L_8007B130:
    r5 = r4;
    r3 = 0x8;
    tmp = 0x2;
    ctr_fn = (void(*)(void))tmp;
L_8007B140:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007B15C;
    tmp = (u32)r5 >> 1;
    r5 = r5 ^ 0x8320;
    goto L_8007B160;
L_8007B15C:
    r5 = (u32)r5 >> 1;
L_8007B160:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007B17C;
    tmp = (u32)r5 >> 1;
    r5 = r5 ^ 0x8320;
    goto L_8007B180;
L_8007B17C:
    r5 = (u32)r5 >> 1;
L_8007B180:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007B19C;
    tmp = (u32)r5 >> 1;
    r5 = r5 ^ 0x8320;
    goto L_8007B1A0;
L_8007B19C:
    r5 = (u32)r5 >> 1;
L_8007B1A0:
    tmp = r5 & 0x1;
    if (tmp == 0) goto L_8007B1BC;
    tmp = (u32)r5 >> 1;
    r5 = r5 ^ 0x8320;
    goto L_8007B1C0;
L_8007B1BC:
    r5 = (u32)r5 >> 1;
L_8007B1C0:
    if (--ctr != 0) goto L_8007B140;
    *(u32*)((u8*)r6 + 0x0) = r5;
    r6 = r6 + 0x4;
    r4 = r4 + 0x1;
    if ((s32)r4 < 0x100) goto L_8007B130;
    tmp = 0x0;
    *(u32*)&lbl_8047A64C = tmp;
    OSGetTick();
    r5 = 0xAAAB0000;
    r4 = (u32)&lbl_802EE608;
    r5 = (u32)&lbl_803FADF8;
    r6 = (u32)((u64)tmp * (u64)r3 >> 32);
    r4 = (u32)&lbl_802EE608;
    tmp = (u32)&lbl_803FADF8;
    r6 = (u32)r6 >> 1;
    r5 = r6 * 0x3;
    r5 = r3 - r5;
    r3 = tmp;
    tmp = r5 << 2;
    r4 = *(u32*)(r4 + tmp);
    ((void(*)(void))fn_800CA968)();
    r3 = (u32)&lbl_803FADF8;
    r4 = (u32)sp + 0x44;
    r3 = (u32)&lbl_803FADF8;
    ((void(*)(void))fn_800A501C)();
    if ((s32)r3 == 0) goto L_8007B33C;
    if (r3 == 0) goto L_8007B33C;
    r4 = *(u32*)&lbl_8047A648;
    tmp = r3 + 0x5b;
    /* clrrwi tmp, tmp, 5 */;
    *(u32*)&lbl_8047A650 = tmp;
    if (r4 == 0) goto L_8007B264;
    r3 = *(u32*)&lbl_80478980;
    ((void(*)(void))fn_8009AAD4)();
L_8007B264:
    r3 = *(u32*)&lbl_80478980;
    r4 = *(u32*)&lbl_8047A650;
    ((void(*)(void))fn_8009A9D8)();
    *(u32*)&lbl_8047A648 = r3;
    if (r3 == 0) goto L_8007B33C;
    r5 = *(u32*)&lbl_8047A650;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = (u32)sp + 0x44;
    ((void(*)(void))fn_800A50E4)();
    tmp = *(u32*)&lbl_8047A64C;
    if ((s32)tmp != 0) goto L_8007B33C;
    r3 = (u32)&lbl_803FADF8;
    r4 = (u32)sp + 0x8;
    r3 = (u32)&lbl_803FADF8;
    ((void(*)(void))fn_800A501C)();
    if ((s32)r3 == 0) goto L_8007B33C;
    if (r31 == 0) goto L_8007B33C;
    tmp = r31 + 0x1f;
    r4 = *(u32*)&lbl_8047A648;
    r3 = (u32)sp + 0x8;
    r6 = 0x0;
    /* clrrwi r5, tmp, 5 */;
    r7 = 0x2;
    ((void(*)(void))fn_800A541C)();
    if ((s32)r3 < 0) goto L_8007B33C;
    if (r3 < r31) goto L_8007B33C;
    r3 = (u32)sp + 0x8;
    ((void(*)(void))fn_800A50E4)();
    r4 = *(u32*)&lbl_8047A648;
    r5 = r31;
    r3 = r4 + 0x34;
    ((void(*)(void))fn_800C8174)();
    r3 = *(u32*)&lbl_8047A648;
    r4 = 0x0;
    r5 = 0x34;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r31 + 0x34;
    r4 = *(u32*)&lbl_8047A648;
    r5 = r3;
    tmp = *(u32*)&lbl_8047A650;
    r3 = r4 + r3;
    r4 = 0x0;
    r5 = tmp - r5;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = 0x1;
    *(u32*)&lbl_8047A64C = tmp;
L_8007B33C:
    return;
}

/* 0x8007B350 | size: 0x354 */
void fn_8007B350(void) {
    extern void fn_8007B6A4();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
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
    r31 = r4;
    r26 = r5;
    r30 = r6;
    r27 = r7;
    r28 = r8;
    r29 = r9;
    tmp = r25 + 0x2388;
    r4 = 0x0;
    r5 = tmp - r25;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = r31 & 0xFF;
    r3 = r29 & 0x1;
    *(u8*)((u8*)r25 + 0x340) = tmp;
    tmp = r29 & 0x00000002;
    r5 = *(u8*)((u8*)r26 + 0x0);
    r4 = *(u8*)((u8*)r26 + 0x1);
    r6 = *(u8*)((u8*)r26 + 0x2);
    r5 = r5 << 24;
    r4 = r4 << 16;
    r7 = *(u8*)((u8*)r26 + 0x3);
    r6 = r6 << 8;
    r4 = r5 | r4;
    r4 = r6 | r4;
    r4 = r7 | r4;
    *(u32*)((u8*)r25 + 0x34C) = r4;
    *(u8*)((u8*)r25 + 0x344) = r3;
    *(u8*)((u8*)r25 + 0x343) = tmp;
    tmp = *(u32*)((u8*)r26 + 0x0);
    *(u32*)((u8*)r25 + 0x354) = tmp;
    if (r30 != 0x4a) goto L_8007B3F0;
    r3 = 0x50530000;
    tmp = r3 + 0x414a;
    *(u32*)((u8*)r25 + 0x350) = tmp;
    goto L_8007B3FC;
L_8007B3F0:
    r3 = 0x50530000;
    tmp = r3 + 0x4145;
    *(u32*)((u8*)r25 + 0x350) = tmp;
L_8007B3FC:
    r31 = *(u32*)&lbl_8047A648;
    r5 = 0x0;
    tmp = *(u32*)&lbl_8047A650;
    r29 = r31 + tmp;
    *(u32*)((u8*)r25 + 0x380) = r31;
    r30 = r29 - r31;
    *(u32*)((u8*)r25 + 0x384) = r29;
    r3 = r4 & 0xFF;
    tmp = (u32)r4 >> 8;
    *(u8*)((u8*)r31 + 0x2C) = r3;
    r3 = tmp & 0xFF;
    tmp = (u32)r4 >> 16;
    *(u8*)((u8*)r31 + 0x2D) = r3;
    r3 = tmp & 0xFF;
    tmp = (u32)r4 >> 24;
    *(u8*)((u8*)r31 + 0x2E) = r3;
    *(u8*)((u8*)r31 + 0x2F) = tmp;
    tmp = 0x4;
    r3 = r27;
    ctr_fn = (void(*)(void))tmp;
L_8007B450:
    r4 = *(u8*)((u8*)r3 + 0x0);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x1);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x2);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x3);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x4);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x5);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x6);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r4 = *(u8*)((u8*)r3 + 0x7);
    if (r4 == 0) goto L_8007B518;
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    r3 = r3 + 0x8;
    if (--ctr != 0) goto L_8007B450;
L_8007B518:
    r3 = 0x20 - r5;
    r4 = 0x0;
    if (r5 >= 0x20) goto L_8007B59C;
    tmp = (u32)r3 >> 3;
    ctr_fn = (void(*)(void))tmp;
    if (tmp == 0) goto L_8007B588;
L_8007B538:
    tmp = r5 + 0x4;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0x5;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0x6;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0x7;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0x8;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0x9;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0xa;
    *(u8*)(r31 + tmp) = r4;
    tmp = r5 + 0xb;
    r5 = r5 + 0x8;
    *(u8*)(r31 + tmp) = r4;
    if (--ctr != 0) goto L_8007B538;
    r3 = r3 & 0x7;
    if (tmp == 0) goto L_8007B59C;
L_8007B588:
    ctr_fn = (void(*)(void))r3;
L_8007B58C:
    tmp = r5 + 0x4;
    r5 = r5 + 0x1;
    *(u8*)(r31 + tmp) = r4;
    if (--ctr != 0) goto L_8007B58C;
L_8007B59C:
    tmp = 0x2;
    r3 = r28 & 0xFF;
    *(u8*)((u8*)r31 + 0x28) = tmp;
    r6 = 0x8;
    tmp = 0x0;
    r4 = r26;
    *(u8*)((u8*)r31 + 0x29) = r3;
    r5 = 0x4;
    *(u8*)((u8*)r31 + 0x30) = r6;
    *(u8*)((u8*)r31 + 0x31) = tmp;
    *(u8*)((u8*)r31 + 0x32) = tmp;
    *(u8*)((u8*)r31 + 0x33) = tmp;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r4 = r25 + 0x354;
    r5 = 0x4;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = 0xAA480000;
    r5 = 0x0;
    r3 = (u32)&lbl_803FAEF8;
    r3 = (u32)&lbl_803FAEF8;
L_8007B5FC:
    r4 = r5;
    r5 = r5 + 0x1;
    tmp = r4 + 0x4;
    r4 = (u32)r6 >> 8;
    tmp = *(u8*)(r31 + tmp);
    tmp = r6 ^ tmp;
    tmp = tmp & 0xFF;
    tmp = tmp << 2;
    tmp = *(u32*)(r3 + tmp);
    r6 = r4 ^ tmp;
    if (r5 != r7) goto L_8007B5FC;
    r3 = r6 & 0xFF;
    tmp = (u32)r6 >> 8;
    *(u8*)((u8*)r31 + 0x0) = r3;
    r4 = tmp & 0xFF;
    r3 = (u32)r6 >> 16;
    tmp = (u32)r6 >> 24;
    *(u8*)((u8*)r31 + 0x1) = r4;
    r5 = r3 & 0xFF;
    r3 = r25 + 0x35c;
    r4 = r25 + 0x37c;
    *(u8*)((u8*)r31 + 0x2) = r5;
    r5 = 0x1;
    *(u8*)((u8*)r31 + 0x3) = tmp;
    ((void(*)(void))fn_8009F1D0)();
    r3 = (u32)fn_8007B6A4;
    r5 = r25;
    r4 = (u32)fn_8007B6A4;
    r6 = r25 + 0x2388;
    r3 = r25 + 0x28;
    r7 = 0x2000;
    r8 = 0x8;
    r9 = 0x0;
    ((void(*)(void))fn_800A19CC)();
    r3 = r25 + 0x28;
    ((void(*)(void))fn_800A1F94)();
    return;
}

/* 0x8007B6A4 | size: 0x34 */
void fn_8007B6A4(void) {
    extern void fn_8007B6D8();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    fn_8007B6D8();
    tmp = 0x1;
    *(u8*)((u8*)r31 + 0x345) = tmp;
    return;
}

/* 0x8007B6D8 | size: 0xB64 */
s32 fn_8007B6D8(void) {
    extern void fn_8025F3F4();
    extern void fn_8025F484();
    extern void fn_8025F584();
    extern void fn_8025F648();
    extern void fn_8007C23C();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r31 = r3;
    tmp = 0x0;
    r30 = *(u8*)((u8*)r31 + 0x340);
    r4 = (u32)sp + 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    r3 = r30;
    fn_8025F484();
    if ((s32)r3 == 0) goto L_8007B714;
    r3 = 0x0;
    goto L_8007C228;
L_8007B714:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007BC18;
    tmp = *(u8*)(sp + 0x9);
    if (tmp != 0x28) goto L_8007BC18;
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
    if ((s32)r3 == 0) goto L_8007B754;
    r3 = 0x0;
    goto L_8007C228;
L_8007B754:
    tmp = *(u32*)((u8*)r31 + 0x350);
    r3 = *(u8*)(sp + 0x9);
    if (r4 != tmp) goto L_8007B770;
    r4 = 0x1;
    goto L_8007B7E0;
L_8007B770:
    tmp = r3 & 0x00000030;
    if (tmp == 0) goto L_8007B784;
    r4 = 0x0;
    goto L_8007B7E0;
L_8007B784:
    tmp = *(u32*)((u8*)r31 + 0x34C);
    r5 = tmp ^ r4;
    /* subis tmp, r5, 0x2000 */;
    if (tmp != 0) goto L_8007B7A4;
    tmp = 0x100;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007B7DC;
L_8007B7A4:
    /* subis tmp, r5, 0x20 */;
    if (tmp != 0) goto L_8007B7BC;
    tmp = 0x200;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007B7DC;
L_8007B7BC:
    r3 = 0xDFE00000;
    r4 = 0x0;
    *(u16*)((u8*)r31 + 0x348) = r4;
    tmp = r5 & tmp;
    if (tmp == 0) goto L_8007B7DC;
    goto L_8007B7E0;
L_8007B7DC:
    r4 = 0x2;
L_8007B7E0:
    if (r4 == 1) goto L_8007B7F0;
    r3 = 0x0;
    goto L_8007C228;
L_8007B7F0:
    tmp = *(u8*)((u8*)r31 + 0x343);
    if (tmp == 0) goto L_8007B804;
    r3 = 0x3;
    goto L_8007C228;
L_8007B804:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007B824;
    tmp = *(u8*)(sp + 0x9);
    if (tmp == 0x20) goto L_8007B82C;
L_8007B824:
    r3 = 0x0;
    goto L_8007C228;
L_8007B82C:
    r3 = r30;
    r4 = r31 + 0x350;
    r5 = (u32)sp + 0x9;
    fn_8025F648();
    if ((s32)r3 == 0) goto L_8007B84C;
    r3 = 0x0;
    goto L_8007C228;
L_8007B84C:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007B86C;
    tmp = *(u8*)(sp + 0x9);
    if (tmp == 0x30) goto L_8007B874;
L_8007B86C:
    r3 = 0x0;
    goto L_8007C228;
L_8007B874:
    r6 = *(u32*)((u8*)r31 + 0x380);
    r3 = r30;
    tmp = *(u32*)((u8*)r31 + 0x384);
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    r29 = tmp - r6;
    /* clrrwi r6, r29, 24 */;
    tmp = r29 & 0x00FF0000;
    r7 = r29 & 0x0000FF00;
    r8 = r29 & 0xFF;
    r6 = (u32)r6 >> 24;
    tmp = (u32)tmp >> 8;
    r7 = r7 << 8;
    tmp = r6 | tmp;
    r6 = r8 << 24;
    tmp = r7 | tmp;
    tmp = r6 | tmp;
    *(u32*)(sp + 0xC) = tmp;
    fn_8025F648();
    if ((s32)r3 == 0) goto L_8007B8D0;
    r3 = 0x0;
    goto L_8007C228;
L_8007B8D0:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
    if ((s32)r3 != 0) goto L_8007B8F8;
    if (r3 == tmp) goto L_8007B900;
L_8007B8F8:
    r3 = 0x0;
    goto L_8007C228;
L_8007B900:
    tmp = 0x1;
    r26 = 0x0;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    r3 = (u32)fn_8007C23C;
    r28 = (u32)fn_8007C23C;
    goto L_8007BA30;
L_8007B918:
    r25 = 0x0;
L_8007B91C:
    tmp = *(u8*)((u8*)r31 + 0x342);
    if (tmp == 0) goto L_8007B938;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007B938:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 == 0) goto L_8007B95C;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007B95C:
    r3 = *(u8*)(sp + 0x9);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x30) goto L_8007B998;
    tmp = r3 & 0x00000008;
    if ((s32)tmp == 0) goto L_8007B988;
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
L_8007B988:
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007B998:
    tmp = r3 & 0x00000002;
    if ((s32)tmp == 0) goto L_8007B9F8;
    r25 = r25 + 0x1;
    if (r25 <= 9) goto L_8007B9C0;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007B9C0:
    r3 = r31;
    OSCreateAlarm();
    OSDisableInterrupts();
    r27 = r3;
    r3 = r31;
    r7 = r28;
    r6 = 0x10;
    r5 = 0x0;
    OSSetAlarm();
    r3 = r31 + 0x28;
    ((void(*)(void))fn_800A221C)();
    r3 = r27;
    OSRestoreInterrupts();
    goto L_8007B91C;
L_8007B9F8:
    r6 = *(u32*)((u8*)r31 + 0x380);
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    tmp = *(u32*)(r6 + r26);
    *(u32*)(sp + 0xC) = tmp;
    fn_8025F648();
    if ((s32)r3 == 0) goto L_8007BA2C;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BA2C:
    r26 = r26 + 0x4;
L_8007BA30:
    if (r26 < r29) goto L_8007B918;
    tmp = 0x2;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    r3 = (u32)fn_8007C23C;
    r29 = (u32)fn_8007C23C;
L_8007BA48:
    tmp = *(u8*)((u8*)r31 + 0x342);
    if (tmp == 0) goto L_8007BA64;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BA64:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007BAC0;
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000008;
    if ((s32)tmp != 0) goto L_8007BAC0;
    r3 = r31;
    OSCreateAlarm();
    OSDisableInterrupts();
    r27 = r3;
    r3 = r31;
    r7 = r29;
    r6 = 0x10;
    r5 = 0x0;
    OSSetAlarm();
    r3 = r31 + 0x28;
    ((void(*)(void))fn_800A221C)();
    r3 = r27;
    OSRestoreInterrupts();
    goto L_8007BA48;
L_8007BAC0:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F484();
    if ((s32)r3 == 0) goto L_8007BAE4;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BAE4:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007BB04;
    tmp = *(u8*)(sp + 0x9);
    if (tmp == 8) goto L_8007BB14;
L_8007BB04:
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BB14:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
    if ((s32)r3 == 0) goto L_8007BB3C;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BB3C:
    tmp = *(u32*)((u8*)r31 + 0x350);
    r3 = *(u8*)(sp + 0x9);
    if (r4 != tmp) goto L_8007BB58;
    r4 = 0x1;
    goto L_8007BBC8;
L_8007BB58:
    tmp = r3 & 0x00000030;
    if (tmp == 0) goto L_8007BB6C;
    r4 = 0x0;
    goto L_8007BBC8;
L_8007BB6C:
    tmp = *(u32*)((u8*)r31 + 0x34C);
    r5 = tmp ^ r4;
    /* subis tmp, r5, 0x2000 */;
    if (tmp != 0) goto L_8007BB8C;
    tmp = 0x100;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007BBC4;
L_8007BB8C:
    /* subis tmp, r5, 0x20 */;
    if (tmp != 0) goto L_8007BBA4;
    tmp = 0x200;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007BBC4;
L_8007BBA4:
    r3 = 0xDFE00000;
    r4 = 0x0;
    *(u16*)((u8*)r31 + 0x348) = r4;
    tmp = r5 & tmp;
    if (tmp == 0) goto L_8007BBC4;
    goto L_8007BBC8;
L_8007BBC4:
    r4 = 0x2;
L_8007BBC8:
    if (r4 == 2) goto L_8007BBE0;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BBE0:
    r3 = r30;
    r4 = r31 + 0x34c;
    r5 = (u32)sp + 0x9;
    fn_8025F648();
    if ((s32)r3 == 0) goto L_8007BC08;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BC08:
    tmp = 0x3;
    r3 = 0x1;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BC18:
    if ((s32)r3 != 0) goto L_8007C224;
    tmp = *(u8*)(sp + 0x9);
    if (tmp != 8) goto L_8007C224;
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
    if ((s32)r3 == 0) goto L_8007BC4C;
    r3 = 0x0;
    goto L_8007C228;
L_8007BC4C:
    tmp = *(u32*)((u8*)r31 + 0x350);
    r3 = *(u8*)(sp + 0x9);
    if (r4 != tmp) goto L_8007BC68;
    r4 = 0x1;
    goto L_8007BCD8;
L_8007BC68:
    tmp = r3 & 0x00000030;
    if (tmp == 0) goto L_8007BC7C;
    r4 = 0x0;
    goto L_8007BCD8;
L_8007BC7C:
    tmp = *(u32*)((u8*)r31 + 0x34C);
    r5 = tmp ^ r4;
    /* subis tmp, r5, 0x2000 */;
    if (tmp != 0) goto L_8007BC9C;
    tmp = 0x100;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007BCD4;
L_8007BC9C:
    /* subis tmp, r5, 0x20 */;
    if (tmp != 0) goto L_8007BCB4;
    tmp = 0x200;
    *(u16*)((u8*)r31 + 0x348) = tmp;
    goto L_8007BCD4;
L_8007BCB4:
    r3 = 0xDFE00000;
    r4 = 0x0;
    *(u16*)((u8*)r31 + 0x348) = r4;
    tmp = r5 & tmp;
    if (tmp == 0) goto L_8007BCD4;
    goto L_8007BCD8;
L_8007BCD4:
    r4 = 0x2;
L_8007BCD8:
    if (r4 == 2) goto L_8007BCE8;
    r3 = 0x0;
    goto L_8007C228;
L_8007BCE8:
    tmp = *(u8*)((u8*)r31 + 0x344);
    if (tmp == 0) goto L_8007BD00;
    tmp = *(u32*)((u8*)r31 + 0x34C);
    *(u32*)(sp + 0xC) = tmp;
    goto L_8007BD18;
L_8007BD00:
    tmp = 0x4;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x34C);
    tmp = tmp | (0x2020 << 16);
    tmp = tmp | 0x2020;
    *(u32*)(sp + 0xC) = tmp;
L_8007BD18:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007BD38;
    tmp = *(u8*)(sp + 0x9);
    if (tmp == 0) goto L_8007BD48;
L_8007BD38:
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BD48:
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    fn_8025F648();
    if ((s32)r3 == 0) goto L_8007BD70;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BD70:
    tmp = *(u8*)((u8*)r31 + 0x344);
    if (tmp == 0) goto L_8007BDA4;
    tmp = 0x0;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    tmp = *(u16*)((u8*)r31 + 0x348);
    tmp = tmp & 0x1;
    if ((s32)tmp == 0) goto L_8007BD9C;
    r3 = 0x4;
    goto L_8007C228;
L_8007BD9C:
    r3 = 0x2;
    goto L_8007C228;
L_8007BDA4:
    r3 = (u32)fn_8007C23C;
    r29 = (u32)fn_8007C23C;
L_8007BDAC:
    tmp = *(u8*)((u8*)r31 + 0x342);
    if (tmp == 0) goto L_8007BDC8;
    tmp = 0x6;
    r3 = 0x6;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BDC8:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007C19C;
    r3 = *(u8*)(sp + 0x9);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x30) goto L_8007C19C;
    if ((s32)tmp == 0) goto L_8007C19C;
    tmp = r3 & 0x32;
    if ((s32)tmp != 0x20) goto L_8007C164;
    OSGetTick();
    *(u32*)((u8*)r31 + 0x358) = r3;
    r3 = r30;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x9;
    tmp = *(u32*)((u8*)r31 + 0x358);
    *(u32*)(sp + 0xC) = tmp;
    fn_8025F648();
    if ((s32)r3 != 0) goto L_8007BE38;
    tmp = *(u8*)(sp + 0x9);
    tmp = tmp & 0x00000030;
    if ((s32)tmp == 0x20) goto L_8007BE48;
L_8007BE38:
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BE48:
    r28 = 0x0;
    r3 = (u32)fn_8007C23C;
    r29 = (u32)fn_8007C23C;
L_8007BE54:
    r26 = 0x0;
L_8007BE58:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 == 0) goto L_8007BE7C;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BE7C:
    r3 = *(u8*)(sp + 0x9);
    tmp = r3 & 0x00000030;
    if ((s32)tmp == 0x20) goto L_8007BE9C;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BE9C:
    tmp = r3 & 0xa;
    if ((s32)tmp == 8) goto L_8007BEFC;
    r26 = r26 + 0x1;
    if (r26 <= 9) goto L_8007BEC4;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BEC4:
    r3 = r31;
    OSCreateAlarm();
    OSDisableInterrupts();
    r27 = r3;
    r3 = r31;
    r7 = r29;
    r6 = 0x10;
    r5 = 0x0;
    OSSetAlarm();
    r3 = r31 + 0x28;
    ((void(*)(void))fn_800A221C)();
    r3 = r27;
    OSRestoreInterrupts();
    goto L_8007BE58;
L_8007BEFC:
    r3 = r30;
    r4 = (u32)sp + 0x10;
    r5 = (u32)sp + 0x9;
    fn_8025F584();
    if ((s32)r3 == 0) goto L_8007BF24;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007BF24:
    tmp = r28 + 0x2388;
    r3 = *(u32*)((u8*)r31 + 0x358);
    r28 = r28 + 0x4;
    r3 = r4 ^ r3;
    *(u32*)(r31 + tmp) = r3;
    if (r28 < 0x1040) goto L_8007BE54;
    r6 = *(u32*)((u8*)r31 + 0x33C4);
    r3 = 0x0;
    /* clrrwi r4, r6, 24 */;
    tmp = r6 & 0x00FF0000;
    r5 = r6 & 0x0000FF00;
    r6 = r6 & 0xFF;
    r4 = (u32)r4 >> 24;
    tmp = (u32)tmp >> 8;
    r5 = r5 << 8;
    r6 = r6 << 24;
    tmp = r4 | tmp;
    tmp = r5 | tmp;
    r9 = r6 | tmp;
    r4 = (u32)&lbl_803FAEF8;
    tmp = 0x80;
    r5 = (u32)sp + 0x8;
    r4 = (u32)&lbl_803FAEF8;
    ctr_fn = (void(*)(void))tmp;
L_8007BF8C:
    tmp = r3 & 0xFF;
    r8 = *(u32*)((u8*)r31 + 0x354);
    *(u8*)(sp + 0x8) = tmp;
    r7 = 0x0;
L_8007BF9C:
    tmp = r7;
    r7 = r7 + 0x1;
    tmp = *(u8*)(r5 + tmp);
    r6 = (u32)r8 >> 8;
    tmp = r8 ^ tmp;
    tmp = tmp & 0xFF;
    tmp = tmp << 2;
    tmp = *(u32*)(r4 + tmp);
    r8 = r6 ^ tmp;
    if (r7 != 1) goto L_8007BF9C;
    if (r9 == r8) goto L_8007C020;
    r3 = r3 + 0x1;
    tmp = r3 & 0xFF;
    r8 = *(u32*)((u8*)r31 + 0x354);
    *(u8*)(sp + 0x8) = tmp;
    r7 = 0x0;
L_8007BFE4:
    tmp = r7;
    r7 = r7 + 0x1;
    tmp = *(u8*)(r5 + tmp);
    r6 = (u32)r8 >> 8;
    tmp = r8 ^ tmp;
    tmp = tmp & 0xFF;
    tmp = tmp << 2;
    tmp = *(u32*)(r4 + tmp);
    r8 = r6 ^ tmp;
    if (r7 != 1) goto L_8007BFE4;
    if (r9 == r8) goto L_8007C020;
    r3 = r3 + 0x1;
    if (--ctr != 0) goto L_8007BF8C;
L_8007C020:
    if (r3 != 0x100) goto L_8007C038;
    tmp = 0x7;
    r3 = 0x7;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C038:
    r3 = 0xAA480000;
    r6 = 0x0;
    r3 = (u32)&lbl_803FAEF8;
    r4 = (u32)sp + 0x8;
    r3 = (u32)&lbl_803FAEF8;
L_8007C050:
    tmp = r6;
    r6 = r6 + 0x1;
    tmp = *(u8*)(r4 + tmp);
    r5 = (u32)r7 >> 8;
    tmp = r7 ^ tmp;
    tmp = tmp & 0xFF;
    tmp = tmp << 2;
    tmp = *(u32*)(r3 + tmp);
    r7 = r5 ^ tmp;
    if (r6 != 1) goto L_8007C050;
    /* clrrwi r3, r7, 24 */;
    tmp = r7 & 0x00FF0000;
    r4 = r7 & 0x0000FF00;
    r5 = r7 & 0xFF;
    r3 = (u32)r3 >> 24;
    tmp = (u32)tmp >> 8;
    r4 = r4 << 8;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    r6 = 0x0;
    tmp = r4 | tmp;
    r4 = r5 | tmp;
    tmp = 0x40f;
    ctr_fn = (void(*)(void))tmp;
L_8007C0B4:
    r3 = r6 + 0x2388;
    r6 = r6 + 0x4;
    tmp = *(u32*)(r31 + r3);
    tmp = tmp ^ r4;
    *(u32*)(r31 + r3) = tmp;
    if (--ctr != 0) goto L_8007C0B4;
    r5 = *(u32*)((u8*)r31 + 0x33C0);
    r6 = 0x0;
    r7 = *(u32*)((u8*)r31 + 0x354);
    /* clrrwi r3, r5, 24 */;
    tmp = r5 & 0x00FF0000;
    r4 = r5 & 0x0000FF00;
    r5 = r5 & 0xFF;
    r3 = (u32)r3 >> 24;
    tmp = (u32)tmp >> 8;
    r4 = r4 << 8;
    r5 = r5 << 24;
    tmp = r3 | tmp;
    tmp = r4 | tmp;
    r5 = r5 | tmp;
    r3 = (u32)&lbl_803FAEF8;
    r3 = (u32)&lbl_803FAEF8;
L_8007C10C:
    r4 = r6;
    r6 = r6 + 0x1;
    tmp = r4 + 0x2388;
    r4 = (u32)r7 >> 8;
    tmp = *(u8*)(r31 + tmp);
    tmp = r7 ^ tmp;
    tmp = tmp & 0xFF;
    tmp = tmp << 2;
    tmp = *(u32*)(r3 + tmp);
    r7 = r4 ^ tmp;
    if (r6 != 0x1038) goto L_8007C10C;
    if (r7 == r5) goto L_8007C154;
    tmp = 0x7;
    r3 = 0x7;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C154:
    tmp = 0x5;
    r3 = 0x5;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C164:
    r3 = r31;
    OSCreateAlarm();
    OSDisableInterrupts();
    r27 = r3;
    r3 = r31;
    r7 = r29;
    r6 = 0x10;
    r5 = 0x0;
    OSSetAlarm();
    r3 = r31 + 0x28;
    ((void(*)(void))fn_800A221C)();
    r3 = r27;
    OSRestoreInterrupts();
    goto L_8007BDAC;
L_8007C19C:
    r3 = r30;
    r4 = (u32)sp + 0x9;
    fn_8025F3F4();
    if ((s32)r3 != 0) goto L_8007C1C0;
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C1C0:
    r3 = *(u8*)(sp + 0x9);
    tmp = r3 & 0x00000030;
    if ((s32)tmp != 0) goto L_8007C1E0;
    tmp = 0x6;
    r3 = 0x6;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C1E0:
    if ((s32)tmp != 0x30) goto L_8007C214;
    tmp = r3 & 0x00000008;
    if ((s32)tmp == 0) goto L_8007C204;
    tmp = 0x7;
    r3 = 0x7;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C204:
    tmp = 0x8;
    r3 = 0x8;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C214:
    tmp = 0x9;
    r3 = 0x9;
    *(u8*)((u8*)r31 + 0x346) = tmp;
    goto L_8007C228;
L_8007C224:
    r3 = 0x0;
L_8007C228:
    return;
}

/* 0x8007C23C | size: 0x24 */
void fn_8007C23C(void) {
    fn_800A1F94();
}

/* 0x8007C260 | size: 0xC */
void fn_8007C260(void) {
}

/* 0x8007C26C | size: 0x54 */
s32 fn_8007C26C(void) {
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;

    tmp = *(u32*)&lbl_80478940;
    if ((s32)tmp == 1) goto L_8007C2A0;
    if ((s32)tmp >= 1) goto L_8007C2A8;
    if ((s32)tmp >= 0) goto L_8007C294;
    goto L_8007C2A8;
L_8007C294:
    r3 = 0xb;
    ((void(*)(void))fn_8002DC6C)();
    goto L_8007C2A8;
L_8007C2A0:
    r3 = 0xc;
    ((void(*)(void))fn_8002DC6C)();
L_8007C2A8:
    r3 = 0x395;
    ((void(*)(void))fn_800FF58C)();
    return;
}

