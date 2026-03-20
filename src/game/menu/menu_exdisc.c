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
void fn_80077A5C(void) {
    /* TODO: decompile (0x50 bytes) */
    fn_8012640C();
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
    /* TODO: decompile (0x4C bytes) */
    fn_80129280();
    return 0;
}

/* 0x80077C1C | size: 0x4C */
s32 fn_80077C1C(void) {
    /* TODO: decompile (0x4C bytes) */
    fn_80142984();
    return 0;
}

/* 0x80077C68 | size: 0x120 */
s32 fn_80077C68(void) {
    /* TODO: decompile (0x120 bytes) */
    fn_8006B420();
    fn_80142984();
    return 0;
}

/* 0x80077D88 | size: 0x30 */
s32 fn_80077D88(void) {
    return 0;
}

/* 0x80077DB8 | size: 0x98 */
s32 fn_80077DB8(void) {
    /* TODO: decompile (0x98 bytes) */
    fn_80129280();
    return 0;
}

/* 0x80077E50 | size: 0x30 */
s32 fn_80077E50(void) {
    return 0;
}

/* 0x80077E80 | size: 0x24 */
void fn_80077E80(void) {
    memcpy();
}

/* 0x80077EA4 | size: 0x30 */
void fn_80077EA4(void) {
    fn_800C80D0();
}

/* 0x80077ED4 | size: 0x4BC */
s32 fn_80077ED4(void) {
    /* TODO: decompile (0x4BC bytes, ~303 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8012AC54();
    fn_801D036C();
    fn_80129280();
    fn_80075B74();
    fn_801D0748();
    fn_80166A28();
    fn_801CB834();
    fn_801CB708();
    fn_800F0308();
    fn_800D37CC();
    fn_800D3088();
    fn_80132A38();
    return 0;
}

/* 0x80078390 | size: 0x52C */
s32 fn_80078390(void) {
    /* TODO: decompile (0x52C bytes, ~331 instructions) */
    /* Uses floating point */
    fn_8012AA2C();
    fn_8012AC54();
    fn_80132A38();
    fn_80103CC0();
    fn_801067E8();
    fn_801069FC();
    fn_80106D3C();
    fn_801C41C8();
    fn_801C40F0();
    fn_801CB834();
    fn_80166A28();
    fn_801CB708();
    return 0;
}

/* 0x800788BC | size: 0x47C */
s32 fn_800788BC(void) {
    /* TODO: decompile (0x47C bytes, ~287 instructions) */
    /* Uses floating point */
    fn_801067E8();
    fn_801069FC();
    fn_801C41C8();
    fn_801C40F0();
    fn_801CB834();
    fn_80166A28();
    fn_801CB708();
    fn_80129280();
    fn_8012A5B0();
    fn_80123FBC();
    fn_800F0308();
    fn_800D37CC();
    return 0;
}

#pragma pop
