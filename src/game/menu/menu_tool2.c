/**
 * @file menu_tool2.c
 * @brief Menu tool functions continued (0x80075818-0x80076788)
 *
 * Address range: 0x8007581C - 0x800767B8
 * Total functions: 25
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8005E750();
extern void fn_80062948();
extern void fn_80063D14();
extern void fn_80069A60();
extern void fn_8006B420();
extern void fn_80071398();
extern void fn_800715BC();
extern void fn_80076F2C();
extern void fn_800C46B0();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800F0308();
extern void fn_800F07A8();
extern void fn_800F9318();
extern void fn_800FA280();
extern void fn_800FF52C();
extern void fn_800FF560();
extern void fn_800FF58C();
extern void fn_800FF660();
extern void fn_80102510();
extern void fn_801026A4();
extern void fn_801069FC();
extern void fn_80106D3C();
extern void fn_8011288C();
extern void fn_80113828();
extern void fn_80113F48();
extern void fn_8011CA34();
extern void fn_8011E7A4();
extern void fn_8011E8DC();
extern void fn_8011EF78();
extern void fn_8011EFA4();
extern void fn_8011EFD0();
extern void fn_8011EFFC();
extern void fn_8011F028();
extern void fn_8011F054();
extern void fn_8011F080();
extern void fn_8011F0AC();
extern void fn_8011F0D8();
extern void fn_8011F104();
extern void fn_8011F130();
extern void fn_8011F15C();
extern void fn_8011F1A0();
extern void fn_8011F4A8();
extern void fn_8011F5C8();
extern void fn_8011F5FC();
extern void fn_8011FC74();
extern void fn_80123FBC();
extern void fn_8012546C();
extern void fn_8012640C();
extern void fn_80129280();
extern void fn_8012AC08();
extern void fn_80132A38();
extern void fn_80135168();
extern void fn_80142984();
extern void fn_80165A20();
extern void fn_80176E0C();
extern void fn_80177A44();
extern void fn_801902E0();
extern void fn_80190528();
extern void fn_801906A0();
/* ... and 7 more external functions */

/* ===== SDA globals ===== */
extern u8 lbl_80478928;
extern u8 lbl_8047A5D0;
extern u8 lbl_8047A618;
extern u8 lbl_8047C0C0;
extern u8 lbl_8047C0C4;
extern u8 lbl_8047C0C8;
extern u8 lbl_8047C0D0;
extern u8 lbl_8047C0D8;

/* ===== Rodata / data labels ===== */
extern u8 lbl_802688F8[];
extern u8 lbl_8026890C[];
extern u8 lbl_80268940[];
extern u8 lbl_802EE458[];

/* ===== Forward declarations ===== */
s32 fn_8007581C(void);
s32 fn_80075A34(void);
s32 fn_80075A9C(void);
s32 fn_80075AC0(void);
s32 fn_80075AE4(void);
s32 fn_80075B08(void);
s32 fn_80075B2C(void);
s32 fn_80075B50(void);
s32 fn_80075B74(void);
s32 fn_80075BC4(void);
s32 fn_80075BFC(void);
s32 fn_80075C20(void);
s32 fn_80075C44(void);
s32 fn_80075C68(void);
s32 fn_80075C94(void);
void fn_80075D98(void);
s32 fn_80075D9C(void);
s32 fn_80075DC8(void);
s32 fn_80075EE0(void);
s32 fn_80075F4C(void);
s32 fn_80075F78(void);
s32 fn_80075FEC(void);
s32 fn_80076054(void);
s32 fn_80076334(void);
s32 fn_80076398(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x8007581C | size: 0x218 */
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_8007581C(void) {
    extern void fn_8019075C();
    extern void fn_80196E10();
    extern void fn_801C40F0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = 0x1;
    r31 = 0x0;
    goto L_80075978;
L_8007583C: ;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    ((void(*)(void))fn_8005E750)();
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    ((void(*)(void))fn_80063D14)();
    r31 = r3;
    if ((s32)r31 == (s32)0xb8) goto L_80075880;
    if ((s32)r31 >= (s32)0xb8) goto L_80075880;
    if ((s32)r31 == (s32)0xb3) goto L_80075878;
    goto L_80075880;
L_80075878: ;
    ((void(*)(void))fn_80071398)();
    goto L_80075984;
L_80075880: ;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    ((void(*)(void))fn_80069A60)();
    r31 = r3;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    *(u32*)((u8*)r3 + 0x20) = r31;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    ((void(*)(void))fn_80062948)();
    r31 = r3;
    if ((s32)r31 == (s32)0xb4) goto L_8007596C;
    if ((s32)r31 >= (s32)0xb4) goto L_800758E4;
    if ((s32)r31 == (s32)0xac) goto L_8007594C;
    if ((s32)r31 >= (s32)0xac) goto L_800758DC;
    if ((s32)r31 == (s32)-0x1) goto L_8007596C;
    goto L_8007596C;
L_800758DC: ;
    goto L_8007596C;
L_800758E4: ;
    if ((s32)r31 == (s32)0xd1) goto L_80075904;
    if ((s32)r31 >= (s32)0xd1) goto L_800758F8;
    goto L_8007596C;
L_800758F8: ;
    if ((s32)r31 == (s32)0x105) goto L_8007593C;
    goto L_8007596C;
L_80075904: ;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 != (s32)0x1) goto L_80075978;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = *(u32*)((u8*)r3 + 0x20);
    if ((s32)r0 != (s32)0x2) goto L_80075978;
    r30 = 0x0;
    goto L_80075978;
L_8007593C: ;
    r3 = 0x105;
    ((void(*)(void))fn_800715BC)();
    r30 = 0x0;
    goto L_80075978;
L_8007594C: ;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x0) goto L_80075968;
    goto L_8007596C;
L_80075968: ;
    r31 = 0xae;
L_8007596C: ;
    r3 = r31;
    ((void(*)(void))fn_80071398)();
    r30 = 0x0;
L_80075978: ;
    r0 = r30 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8007583C;
L_80075984: ;
    ((void(*)(void))fn_800FF52C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 == (u32)0x0) goto L_80075A14;
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x1) goto L_800759C4;
    r3 = (u32)&lbl_802688F8;
    r5 = (u32)&lbl_8026890C;
    r3 = (u32)&lbl_802688F8;
    r4 = 0xa7;
    r5 = (u32)&lbl_8026890C;
    fn_80196E10();
L_800759C4: ;
    ((void(*)(void))fn_800FF660)();
    r3 = 0x0;
    r4 = 0xe;
    ((void(*)(void))fn_80129280)();
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((s32)r0 == (s32)0x1) goto L_800759E4;
    goto L_80075A1C;
L_800759E4: ;
    if ((s32)r31 == (s32)0xd1) goto L_80075A1C;
    r3 = 0x8ae;
    r4 = 0x0;
    fn_8019075C();
    r3 = (0x596 << 16);
    r4 = 0x0;
    r3 = r3 + 0x9;
    ((void(*)(void))fn_8011288C)();
    r3 = 0x1;
    fn_801C40F0();
    goto L_80075A1C;
L_80075A14: ;
    r3 = 0x395;
    ((void(*)(void))fn_800FF58C)();
L_80075A1C: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}

/* 0x80075A34 | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075A34(void) {
    extern void fn_801CBA0C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    ((void(*)(void))fn_80113F48)();
    r4 = (0x1080 << 16);
    r31 = r3;
    r3 = r4 + 0x1000;
    fn_801CBA0C();
    r4 = r3;
    r3 = r31;
    *(u32*)&lbl_8047A5D0 = r4;
    ((void(*)(void))fn_800F9318)();
    r4 = (0x1082 << 16);
    r3 = 0x5e0;
    r4 = r4 + 0x1800;
    r5 = 0x0;
    r6 = 0x1;
    ((void(*)(void))fn_80176E0C)();
    r3 = 0x4;
    ((void(*)(void))fn_80177A44)();
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80075A9C | size: 0x24 */
s32 fn_80075A9C(void) {
    fn_80190528();
    return 0;
}

/* 0x80075AC0 | size: 0x24 */
s32 fn_80075AC0(void) {
    fn_801902E0();
    return 0;
}

/* 0x80075AE4 | size: 0x24 */
s32 fn_80075AE4(void) {
    fn_80190528();
    return 0;
}

/* 0x80075B08 | size: 0x24 */
s32 fn_80075B08(void) {
    fn_801902E0();
    return 0;
}

/* 0x80075B2C | size: 0x24 */
s32 fn_80075B2C(void) {
    fn_80190528();
    return 0;
}

/* 0x80075B50 | size: 0x24 */
s32 fn_80075B50(void) {
    fn_801902E0();
    return 0;
}

/* 0x80075B74 | size: 0x50 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075B74(void) {
    extern void fn_8019075C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r31 = 0;

    r3 = 0xab2;
    ((void(*)(void))fn_801906A0)();
    r4 = r3 + 0x1;
    r31 = 0x1;
    if ((u32)r4 <= (u32)0x30) goto L_80075BA4;
    r4 = 0x30;
    r31 = 0x0;
L_80075BA4: ;
    r3 = 0xab2;
    fn_8019075C();
    r3 = r31;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80075BC4 | size: 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075BC4(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;

    r3 = 0xab2;
    ((void(*)(void))fn_801906A0)();
    if ((u32)r3 <= (u32)0x30) goto L_80075BE8;
    r3 = 0x0;
    goto L_80075BEC;
L_80075BE8: ;
    r3 = 0x30 - r3;
L_80075BEC: ;
    return;
}
#pragma pop

/* 0x80075BFC | size: 0x24 */
s32 fn_80075BFC(void) {
    fn_80190528();
    return 0;
}

/* 0x80075C20 | size: 0x24 */
s32 fn_80075C20(void) {
    fn_801902E0();
    return 0;
}

/* 0x80075C44 | size: 0x24 */
s32 fn_80075C44(void) {
    fn_801902E0();
    return 0;
}

/* 0x80075C68 | size: 0x2C */
s32 fn_80075C68(void) {
    fn_801C40F0();
    fn_80102510();
    return 0;
}

/* 0x80075C94 | size: 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075C94(void) {
    extern void fn_801C40F0();
    extern void fn_801D0748();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r31 = 0;

L_80075CA4: ;
    r3 = 0x37;
    r4 = 0x0;
    ((void(*)(void))fn_80132A38)();
    r3 = 0xe0;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    ((void(*)(void))fn_801026A4)();
    r31 = r3;
    r3 = 0x1;
    fn_801C40F0();
    if ((s32)r31 == (s32)0x1) goto L_80075D10;
    if ((s32)r31 >= (s32)0x1) goto L_80075CF8;
    if ((s32)r31 == (s32)-0x1) goto L_80075D78;
    if ((s32)r31 >= (s32)-0x1) goto L_80075D00;
    goto L_80075D78;
L_80075CF8: ;
    goto L_80075D78;
L_80075D00: ;
    r3 = 0x322;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    goto L_80075D84;
L_80075D10: ;
    r3 = 0x2;
    r4 = 0x2;
    r5 = 0x0;
    fn_801D0748();
    r31 = r3;
    if ((s32)r31 != (s32)0x3) goto L_80075D40;
    r3 = 0x0;
    r4 = 0x4;
    ((void(*)(void))fn_80135168)();
    if ((u32)r3 != (u32)0x0) goto L_80075D68;
L_80075D40: ;
    if ((s32)r31 == (s32)-0x1) goto L_80075CA4;
    r3 = 0x2;
    r4 = 0x44db;
    r5 = 0x1;
    r6 = 0x0;
    ((void(*)(void))fn_80106D3C)();
    r3 = 0x1;
    ((void(*)(void))fn_801069FC)();
    goto L_80075CA4;
L_80075D68: ;
    r3 = 0x323;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    goto L_80075D84;
L_80075D78: ;
    r3 = 0x320;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
L_80075D84: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80075D98 | size: 0x4 */
void fn_80075D98(void) {
}

/* 0x80075D9C | size: 0x2C */
s32 fn_80075D9C(void) {
    fn_801C40F0();
    fn_80102510();
    return 0;
}

/* 0x80075DC8 | size: 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075DC8(void) {
    extern void fn_801C40F0();
    extern void fn_801C41C8();
    extern void fn_801CB834();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;

    ((void(*)(void))fn_80113F48)();
    r4 = (0xb56 << 16);
    r5 = 0x0;
    r4 = r4 + 0x1800;
    r6 = 0x0;
    ((void(*)(void))fn_80176E0C)();
    r30 = 0x1;
    ((void(*)(void))fn_800D37CC)();
    if ((s32)r3 != (s32)0x32) goto L_80075E1C;
    f1 = *(f32*)&lbl_8047C0C0;
    ((void(*)(void))fn_800C46B0)();
    r30 = r3;
    if ((u32)r30 >= (u32)0x1) goto L_80075E1C;
    r30 = 0x1;
L_80075E1C: ;
    r31 = 0x0;
    goto L_80075E30;
L_80075E24: ;
    ((void(*)(void))fn_800F0308)();
    ((void(*)(void))fn_800D3088)();
    r31 = r31 + r3;
L_80075E30: ;
    if ((u32)r31 < (u32)r30) goto L_80075E24;
    r3 = (0xb54 << 16);
    r4 = 0x2;
    r3 = r3 + 0x1000;
    r5 = 0x0;
    r6 = 0x1;
    fn_801CB834();
    r3 = 0xe2;
    r4 = 0x0;
    r5 = 0x0;
    r6 = 0x10;
    r7 = 0x1;
    r8 = 0x0;
    /* crclr cr1eq */;
    ((void(*)(void))fn_801026A4)();
    if ((s32)r3 == (s32)0x0) goto L_80075E8C;
    if ((s32)r3 >= (s32)0x0) goto L_80075E84;
    goto L_80075E94;
L_80075E84: ;
    goto L_80075E94;
L_80075E8C: ;
    r30 = 0x321;
    goto L_80075E98;
L_80075E94: ;
    r30 = 0x384;
L_80075E98: ;
    r3 = 0x1;
    fn_801C40F0();
    f1 = *(f32*)&lbl_8047C0C4;
    r3 = 0x3;
    fn_801C41C8();
    r3 = r30;
    r4 = 0x0;
    ((void(*)(void))fn_80113828)();
    r4 = (0x596 << 16);
    r3 = 0x0;
    r4 = r4 + 0x8;
    ((void(*)(void))fn_8011288C)();
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80075EE0 | size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075EE0(void) {
    extern void fn_801C41C8();
    extern void fn_80075F4C();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    f32 f1 = 0.0f;

    r0 = *(u32*)&lbl_8047A618;
    if ((s32)r0 != (s32)0x0) goto L_80075F20;
    ((void(*)(void))fn_800FF560)();
    r5 = (u32)fn_80075F4C;
    r4 = r3;
    r8 = (u32)fn_80075F4C;
    r3 = 0x1;
    r5 = 0x4000;
    r6 = 0x1;
    r7 = 0x1;
    ((void(*)(void))fn_800F07A8)();
    goto L_80075F30;
L_80075F20: ;
    r3 = 0x46a;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_80165A20)();
L_80075F30: ;
    f1 = *(f32*)&lbl_8047C0C8;
    r3 = 0x2;
    fn_801C41C8();
    return;
}
#pragma pop

/* 0x80075F4C | size: 0x2C */
s32 fn_80075F4C(void) {
    fn_80165A20();
    return 0;
}

/* 0x80075F78 | size: 0x74 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075F78(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    r0 = *(u8*)((u8*)r3 + 0x95);
    r0 = (s8)r0;
    if ((s32)r0 == (s32)0x1) goto L_80075FB8;
    if ((s32)r0 >= (s32)0x1) goto L_80075FA4;
    if ((s32)r0 >= (s32)0x0) goto L_80075FB0;
    goto L_80075FC8;
L_80075FA4: ;
    if ((s32)r0 >= (s32)0x3) goto L_80075FC8;
    goto L_80075FC0;
L_80075FB0: ;
    r3 = 0x43bc;
    goto L_80075FCC;
L_80075FB8: ;
    r3 = 0x43ba;
    goto L_80075FCC;
L_80075FC0: ;
    r3 = 0x43be;
    goto L_80075FCC;
L_80075FC8: ;
    r3 = 0x1;
L_80075FCC: ;
    ((void(*)(void))fn_800FA280)();
    r4 = r3;
    r3 = 0x37;
    ((void(*)(void))fn_80132A38)();
    return;
}
#pragma pop

/* 0x80075FEC | size: 0x68 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80075FEC(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r31 = 0;

    r31 = r3;
    ((void(*)(void))fn_8011F5C8)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x19a) goto L_80076020;
    if ((s32)r0 >= (s32)0x19a) goto L_8007603C;
    if ((s32)r0 == (s32)0x97) goto L_80076020;
    goto L_8007603C;
L_80076020: ;
    r3 = r31;
    ((void(*)(void))fn_8011E7A4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_8007603C;
    r3 = 0x0;
    goto L_80076040;
L_8007603C: ;
    r3 = 0x1;
L_80076040: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* 0x80076054 | size: 0x2E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80076054(void) {
    extern void fn_80076398();
    extern void fn_80196E10();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    /* stmw r25, 0x14(r1) */;
    r27 = r3;
    r28 = r4;
    r3 = (u32)&lbl_80268940;
    r25 = 0x0;
    r31 = (u32)&lbl_80268940;
L_80076078: ;
    r26 = 0x0;
L_8007607C: ;
    r3 = r27;
    r4 = r26 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    r4 = r25;
    fn_80076398();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800760AC;
    r0 = r25 << 1;
    r3 = r31 + 0xfc;
    r3 = *(u16*)(r3 + r0);
    goto L_80076320;
L_800760AC: ;
    r26 = r26 + 0x1;
    if ((s32)r26 < (s32)0x6) goto L_8007607C;
    r25 = r25 + 0x1;
    if ((u32)r25 < (u32)0x6) goto L_80076078;
    if ((u32)r28 != (u32)0x0) goto L_800760D4;
    r3 = 0x0;
    goto L_80076320;
L_800760D4: ;
    r30 = 0x0;
L_800760D8: ;
    r29 = 0x0;
L_800760DC: ;
    r3 = r27;
    r4 = r29 & 0xFFFF;
    ((void(*)(void))fn_8012AC08)();
    r25 = r3;
    r26 = 0x0;
    if ((u32)r25 == (u32)0x0) goto L_80076110;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x0) goto L_80076114;
L_80076110: ;
    r26 = 0x1;
L_80076114: ;
    if ((s32)r26 == (s32)0x0) goto L_80076124;
    r0 = 0x1;
    goto L_800762AC;
L_80076124: ;
    if ((s32)r30 == (s32)0x1) goto L_80076170;
    if ((s32)r30 >= (s32)0x1) goto L_8007613C;
    if ((s32)r30 >= (s32)0x0) goto L_80076148;
    goto L_80076298;
L_8007613C: ;
    if ((s32)r30 >= (s32)0x3) goto L_80076298;
    goto L_80076198;
L_80076148: ;
    r3 = r25;
    ((void(*)(void))fn_8011F4A8)();
    r5 = r3 & 0xFF;
    r0 = *(s16*)((u8*)r28 + 0x0);
    r4 = (s32)r5 >> 31;
    r3 = (u32)r0 >> 31;
    r0 = r5 - r0;
    r0 = r4 + r3; /* +carry */;
    r0 = r0 & 0xFF;
    goto L_800762AC;
L_80076170: ;
    r3 = r25;
    ((void(*)(void))fn_8011F4A8)();
    r0 = *(s16*)((u8*)r28 + 0x2);
    r5 = r3 & 0xFF;
    r3 = (u32)r5 >> 31;
    r4 = (s32)r0 >> 31;
    r0 = r0 - r5;
    r0 = r4 + r3; /* +carry */;
    r0 = r0 & 0xFF;
    goto L_800762AC;
L_80076198: ;
    r3 = r25;
    ((void(*)(void))fn_8011F1A0)();
    r25 = r3;
    ((void(*)(void))fn_8006B420)();
    r0 = r25 & 0xFFFF;
    r26 = r3;
    if ((s32)r0 == (s32)0xaf) goto L_800761D0;
    if ((s32)r0 >= (s32)0xaf) goto L_800761D8;
    if ((s32)r0 == (s32)0x0) goto L_800761C8;
    goto L_800761D8;
L_800761C8: ;
    r3 = 0x1;
    goto L_800761E0;
L_800761D0: ;
    r3 = 0x0;
    goto L_800761E0;
L_800761D8: ;
    r3 = r25;
    ((void(*)(void))fn_80142984)();
L_800761E0: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800761F4;
    r0 = 0x0;
    goto L_800762AC;
L_800761F4: ;
    r0 = *(u32*)((u8*)r26 + 0x8);
    if ((s32)r0 == (s32)0x1) goto L_80076224;
    if ((s32)r0 >= (s32)0x1) goto L_80076210;
    if ((s32)r0 >= (s32)0x0) goto L_8007621C;
    goto L_80076290;
L_80076210: ;
    if ((s32)r0 >= (s32)0x3) goto L_80076290;
    goto L_80076238;
L_8007621C: ;
    r0 = 0x1;
    goto L_800762AC;
L_80076224: ;
    r0 = r25 & 0xFFFF;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r0 = r0 & 0xFF;
    goto L_800762AC;
L_80076238: ;
    r3 = (u32)&lbl_802EE458;
    r0 = *(u32*)&lbl_80478928;
    r5 = (u32)&lbl_802EE458;
    r4 = 0x0;
    r3 = r25 & 0xFFFF;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 <= (u32)0x0) goto L_80076288;
L_80076258: ;
    r0 = *(u16*)((u8*)r5 + 0x0);
    if ((u32)r3 != (u32)r0) goto L_8007627C;
    r0 = r4 + 0x18;
    r0 = *(u8*)(r26 + r0);
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r0 = r0 & 0xFF;
    goto L_800762AC;
L_8007627C: ;
    r5 = r5 + 0x2;
    r4 = r4 + 0x1;
    if (--ctr != 0) goto L_80076258;
L_80076288: ;
    r0 = 0x1;
    goto L_800762AC;
L_80076290: ;
    r0 = 0x0;
    goto L_800762AC;
L_80076298: ;
    r3 = r31 + 0x108;
    r5 = r31 + 0x118;
    r4 = 0xfb;
    fn_80196E10();
    r0 = 0x0;
L_800762AC: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800762C8;
    r0 = r30 << 1;
    r3 = (u32)&lbl_8047C0D0;
    r3 = *(u16*)(r3 + r0);
    goto L_80076320;
L_800762C8: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x6) goto L_800760DC;
    r30 = r30 + 0x1;
    if ((u32)r30 < (u32)0x3) goto L_800760D8;
    r25 = 0x0;
L_800762E4: ;
    r3 = r27;
    r4 = r28;
    r5 = r25;
    ((void(*)(void))fn_80076F2C)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80076310;
    r0 = r25 << 1;
    r3 = (u32)&lbl_8047C0D8;
    r3 = *(u16*)(r3 + r0);
    goto L_80076320;
L_80076310: ;
    r25 = r25 + 0x1;
    if ((u32)r25 < (u32)0x4) goto L_800762E4;
    r3 = 0x0;
L_80076320: ;
    /* lmw r25, 0x14(r1) */;
    return;
}
#pragma pop

/* 0x80076334 | size: 0x64 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80076334(void) {
    extern void fn_80076398();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r31 = 0x0;
L_80076350: ;
    r3 = r30;
    r4 = r31;
    fn_80076398();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80076370;
    r3 = 0x0;
    goto L_80076380;
L_80076370: ;
    r31 = r31 + 0x1;
    if ((s32)r31 < (s32)0x6) goto L_80076350;
    r3 = 0x1;
L_80076380: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* 0x80076398 | size: 0x420 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80076398(void) {
    extern void fn_80076398();
    extern void fn_80196E10();
    u8 sp[0x150];
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

    r31 = r3;
    r28 = r4;
    r4 = (u32)&lbl_80268940;
    r29 = (u32)&lbl_80268940;
    r30 = 0x0;
    if ((u32)r31 == (u32)0x0) goto L_800763E8;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    if ((s32)r3 != (s32)0x0) goto L_800763EC;
L_800763E8: ;
    r30 = 0x1;
L_800763EC: ;
    if ((s32)r30 == (s32)0x0) goto L_800763FC;
    r3 = 0x1;
    goto L_80076798;
L_800763FC: ;
    if ((s32)r28 == (s32)0x3) goto L_800766B0;
    if ((s32)r28 >= (s32)0x3) goto L_80076420;
    if ((s32)r28 == (s32)0x1) goto L_80076678;
    if ((s32)r28 >= (s32)0x1) goto L_80076694;
    if ((s32)r28 >= (s32)0x0) goto L_80076430;
    goto L_80076784;
L_80076420: ;
    if ((s32)r28 == (s32)0x5) goto L_80076748;
    if ((s32)r28 >= (s32)0x5) goto L_80076784;
    goto L_800766EC;
L_80076430: ;
    r3 = r31;
    r4 = 0x2;
    fn_80076398();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80076450;
    r3 = 0x1;
    goto L_80076798;
L_80076450: ;
    r3 = r31;
    ((void(*)(void))fn_8011F5C8)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0x19a) goto L_80076474;
    if ((s32)r0 >= (s32)0x19a) goto L_80076490;
    if ((s32)r0 == (s32)0x97) goto L_80076474;
    goto L_80076490;
L_80076474: ;
    r3 = r31;
    ((void(*)(void))fn_8011E7A4)();
    r0 = r3 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_80076490;
    r0 = 0x0;
    goto L_80076494;
L_80076490: ;
    r0 = 0x1;
L_80076494: ;
    r0 = r0 & 0xFF;
    if ((u32)r0 != (u32)0x0) goto L_800764A8;
    r3 = 0x0;
    goto L_80076798;
L_800764A8: ;
    r4 = r31;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F5FC)();
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8012546C)();
    r3 = r31;
    ((void(*)(void))fn_8011F15C)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F15C)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8007657C;
    r3 = r31;
    ((void(*)(void))fn_8011F130)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F130)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8007657C;
    r3 = r31;
    ((void(*)(void))fn_8011F104)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F104)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8007657C;
    r3 = r31;
    ((void(*)(void))fn_8011F0D8)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F0D8)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8007657C;
    r3 = r31;
    ((void(*)(void))fn_8011F0AC)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F0AC)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 < (u32)r30) goto L_8007657C;
    r3 = r31;
    ((void(*)(void))fn_8011F080)();
    r30 = r3 & 0xFFFF;
    r3 = r1 + 0x8;
    ((void(*)(void))fn_8011F080)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 >= (u32)r30) goto L_80076584;
L_8007657C: ;
    r3 = 0x0;
    goto L_80076798;
L_80076584: ;
    r3 = r31;
    ((void(*)(void))fn_8011F054)();
    r29 = r3 & 0xFFFF;
    r3 = r31;
    ((void(*)(void))fn_8011F028)();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    r29 = r29 + r0;
    ((void(*)(void))fn_8011EFFC)();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    r29 = r29 + r0;
    ((void(*)(void))fn_8011EFD0)();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    r29 = r29 + r0;
    ((void(*)(void))fn_8011EFA4)();
    r0 = r3 & 0xFFFF;
    r3 = r31;
    r29 = r29 + r0;
    ((void(*)(void))fn_8011EF78)();
    r0 = r3 & 0xFFFF;
    r29 = r29 + r0;
    if ((u32)r29 <= (u32)0x1fe) goto L_800765F0;
    r3 = 0x0;
    goto L_80076798;
L_800765F0: ;
    r3 = r31;
    ((void(*)(void))fn_8011F054)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 > (u32)0xff) goto L_80076668;
    r3 = r31;
    ((void(*)(void))fn_8011F028)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 > (u32)0xff) goto L_80076668;
    r3 = r31;
    ((void(*)(void))fn_8011EFFC)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 > (u32)0xff) goto L_80076668;
    r3 = r31;
    ((void(*)(void))fn_8011EFD0)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 > (u32)0xff) goto L_80076668;
    r3 = r31;
    ((void(*)(void))fn_8011EFA4)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 > (u32)0xff) goto L_80076668;
    r3 = r31;
    ((void(*)(void))fn_8011EF78)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 <= (u32)0xff) goto L_80076670;
L_80076668: ;
    r3 = 0x0;
    goto L_80076798;
L_80076670: ;
    r3 = 0x1;
    goto L_80076798;
L_80076678: ;
    r3 = r31;
    ((void(*)(void))fn_8011FC74)();
    r0 = r3 & 0xFF;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r3 = r0 & 0xFF;
    goto L_80076798;
L_80076694: ;
    r3 = r31;
    ((void(*)(void))fn_8011E8DC)();
    r0 = r3 & 0xFF;
    r0 = __cntlzw(r0);
    r0 = (u32)r0 >> 5;
    r3 = r0 & 0xFF;
    goto L_80076798;
L_800766B0: ;
    r3 = r31;
    ((void(*)(void))fn_8011F1A0)();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0xaf) goto L_800766DC;
    if ((s32)r0 >= (s32)0xaf) goto L_800766E4;
    if ((s32)r0 == (s32)0x0) goto L_800766D4;
    goto L_800766E4;
L_800766D4: ;
    r3 = 0x1;
    goto L_80076798;
L_800766DC: ;
    r3 = 0x0;
    goto L_80076798;
L_800766E4: ;
    ((void(*)(void))fn_80142984)();
    goto L_80076798;
L_800766EC: ;
    r29 = 0x0;
L_800766F0: ;
    r3 = r31;
    r6 = r29 & 0xFFFF;
    r4 = 0x0;
    r5 = 0x7f;
    ((void(*)(void))fn_8012640C)();
    r28 = r3 & 0xFFFF;
    if ((u32)r28 == (u32)0x0) goto L_80076734;
    r3 = 0x0;
    ((void(*)(void))fn_8011CA34)();
    r30 = r3;
    r3 = r28;
    ((void(*)(void))fn_8011CA34)();
    if ((u32)r3 != (u32)r30) goto L_80076734;
    r3 = 0x0;
    goto L_80076798;
L_80076734: ;
    r29 = r29 + 0x1;
    if ((s32)r29 < (s32)0x4) goto L_800766F0;
    r3 = 0x1;
    goto L_80076798;
L_80076748: ;
    r3 = r31;
    r4 = 0x0;
    r5 = 0x6e;
    r6 = 0x0;
    ((void(*)(void))fn_8012640C)();
    r0 = r3 & 0xFFFF;
    if ((u32)r0 != (u32)0x0) goto L_80076778;
    r3 = r29 + 0x108;
    r5 = r29 + 0x14c;
    r4 = 0x25e;
    fn_80196E10();
L_80076778: ;
    r3 = r31;
    ((void(*)(void))fn_80123FBC)();
    goto L_80076798;
L_80076784: ;
    r3 = r29 + 0x108;
    r5 = r29 + 0x118;
    r4 = 0x274;
    fn_80196E10();
    r3 = 0x0;
L_80076798: ;
    r31 = *(u32*)(sp + 0x14C);
    r30 = *(u32*)(sp + 0x148);
    r29 = *(u32*)(sp + 0x144);
    r28 = *(u32*)(sp + 0x140);
    return;
}
#pragma pop

#pragma pop
