/**
 * @file gba_conv.c
 * @brief GBA data conversion and validation (0x80083AF4-0x80089048)
 *
 * Address range: 0x80083AF4 - 0x80089048
 * Total functions: 28
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001E074();
extern void fn_8005CF2C();
extern void fn_8006A76C();
extern void fn_8006A79C();
extern void fn_8006A7AC();
extern void fn_8006A7BC();
extern void fn_8006ADB4();
extern void fn_8006ADEC();
extern void fn_8006AE18();
extern void fn_8007109C();
extern void fn_80071104();
extern void fn_800776E4();
extern void fn_8008ABA0();
extern void fn_80092E38();
extern void fn_80092FC8();
extern void fn_80093160();
extern void fn_800932F0();
extern void fn_800934E4();
extern void fn_80093610();
extern void fn_80093698();
extern void fn_800D0F44();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800E202C();
extern void fn_800E209C();
extern void fn_800E24B0();
extern void fn_800E27B0();
extern void fn_800E2C04();
extern void fn_800EC960();
extern void fn_800EC990();
extern void fn_800EC9DC();
extern void fn_800ECA78();
extern void fn_800ECB74();
extern void fn_800ECCA8();
extern void fn_800F0308();
extern void fn_800F92D4();
extern void fn_800F9AEC();
extern void fn_800FA280();
extern void fn_800FF56C();
extern void fn_800FF660();
extern void fn_800FF730();
extern void fn_80102510();
extern void fn_80102568();
extern void fn_80102620();
extern void fn_8010264C();
extern void fn_80103CB0();
extern void fn_80103CC0();
extern void fn_801040A0();
extern void fn_801046C8();
extern void fn_80104704();
extern void fn_801054B8();
extern void fn_80105624();
extern void fn_80106934();
extern void fn_801069FC();
extern void fn_80106D3C();
extern void fn_801081F8();
extern void fn_80109220();
extern void fn_8011394C();
extern void fn_8011C7C0();
extern void fn_8011CA34();
/* ... and 27 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478950;
extern u8 lbl_80478954;
extern u8 lbl_80478958;
extern u8 lbl_8047A660;
extern u8 lbl_8047A664;
extern u8 lbl_8047A668;
extern u8 lbl_8047A66C;
extern u8 lbl_8047C190;
extern u8 lbl_8047C198;
extern u8 lbl_8047C1A0;
extern u8 lbl_8047C1A8;
extern u8 lbl_8047C1AC;
extern u8 lbl_8047C1B0;
extern u8 lbl_8047C1B8;
extern u8 lbl_8047C1C0;
extern u8 lbl_8047C1C4;
extern u8 lbl_8047C1C8;
extern u8 lbl_8047C1CC;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EEB78[];
extern u8 lbl_8026F2E8[];
extern u8 lbl_8026F488[];
extern u8 lbl_8026F4F8[];
extern u8 lbl_803FB2F8[];

/* ===== Forward declarations ===== */
s32 fn_80083AF4(void);
s32 fn_80083BF8(void);
s32 fn_80083CBC(void);
s32 fn_80083CFC(void);
s32 fn_80083D30(void);
s32 fn_80083ECC(void);
void fn_80084034(void);
s32 fn_80084038(void);
s32 fn_800849B4(void);
s32 fn_80084A8C(void);
s32 fn_80087AE8(void);
s32 fn_80087C64(void);
s32 fn_80088428(void);
s32 fn_800884BC(void);
s32 fn_800886D0(void);
s32 fn_80088964(void);
s32 fn_800889A4(void);
s32 fn_800889E4(void);
s32 fn_80088C60(void);
s32 fn_80088D84(void);
s32 fn_80088EA8(void);
s32 fn_80088F58(void);
s32 fn_80088F74(void);
s32 fn_80088F88(void);
s32 fn_80088FA4(void);
s32 fn_80088FF8(void);
void fn_80089028(void);
void fn_80089030(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80083AF4 | size: 0x104 */
s32 fn_80083AF4(void) {
    /* TODO: decompile (0x104 bytes) */
    fn_80129280();
    return 0;
}

/* 0x80083BF8 | size: 0xC4 */
s32 fn_80083BF8(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_80129280();
    return 0;
}

/* 0x80083CBC | size: 0x40 */
s32 fn_80083CBC(void) {
    /* TODO: decompile (0x40 bytes) */
    fn_80129280();
    return 0;
}

/* 0x80083CFC | size: 0x34 */
s32 fn_80083CFC(void) {
    /* TODO: decompile (0x34 bytes) */
    fn_80129280();
    return 0;
}

/* 0x80083D30 | size: 0x19C */
s32 fn_80083D30(void) {
    /* TODO: decompile (0x19C bytes) */
    fn_8012AC08();
    fn_8011F228();
    fn_8011CA34();
    fn_8011C7C0();
    fn_800FA280();
    fn_80135938();
    fn_800F9AEC();
    return 0;
}

/* 0x80083ECC | size: 0x168 */
s32 fn_80083ECC(void) {
    /* TODO: decompile (0x168 bytes) */
    fn_8011CA34();
    fn_8011C7C0();
    fn_800FA280();
    fn_80135938();
    fn_800F9AEC();
    return 0;
}

/* 0x80084034 | size: 0x4 */
void fn_80084034(void) {
}

/* 0x80084038 | size: 0x97C */
s32 fn_80084038(void) {
    /* TODO: decompile (0x97C bytes, ~607 instructions) */
    /* Uses many saved registers */
    fn_80104704();
    fn_801040A0();
    fn_800E2C04();
    fn_80196E10();
    fn_800E27B0();
    fn_801046C8();
    fn_801081F8();
    fn_800E202C();
    fn_800E24B0();
    fn_800E209C();
    fn_8008ABA0();
    fn_80103CB0();
    return 0;
}

/* 0x800849B4 | size: 0xD8 */
s32 fn_800849B4(void) {
    /* TODO: decompile (0xD8 bytes) */
    fn_80103CC0();
    fn_80084A8C();
    fn_801069FC();
    fn_80102620();
    fn_80102568();
    fn_80093698();
    return -1;
}

/* 0x80084A8C | size: 0x305C */
s32 fn_80084A8C(void) {
    /* TODO: decompile (0x305C bytes, ~3095 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_80093698();
    fn_800F0308();
    fn_800932F0();
    fn_8010264C();
    fn_80104704();
    fn_80196E10();
    fn_801040A0();
    fn_80129280();
    fn_80132A38();
    fn_80106D3C();
    fn_800D37CC();
    fn_800D3088();
    return 0;
}

/* 0x80087AE8 | size: 0x17C */
s32 fn_80087AE8(void) {
    /* TODO: decompile (0x17C bytes) */
    fn_800D0F44();
    fn_80102620();
    fn_800F0308();
    fn_80105624();
    fn_80106934();
    return 0;
}

/* 0x80087C64 | size: 0x7C4 */
s32 fn_80087C64(void) {
    /* TODO: decompile (0x7C4 bytes, ~497 instructions) */
    /* Uses many saved registers */
    fn_801054B8();
    fn_800F92D4();
    fn_800ECCA8();
    fn_800ECA78();
    fn_800EC9DC();
    fn_800EC990();
    fn_80166A28();
    fn_800F0308();
    fn_801666BC();
    fn_800ECB74();
    fn_800EC960();
    return 0;
}

/* 0x80088428 | size: 0x94 */
s32 fn_80088428(void) {
    /* TODO: decompile (0x94 bytes) */
    fn_80166A28();
    fn_80176E0C();
    fn_80176B48();
    fn_80087C64();
    fn_800FF660();
    return 0;
}

/* 0x800884BC | size: 0x214 */
s32 fn_800884BC(void) {
    /* TODO: decompile (0x214 bytes, ~133 instructions) */
    /* Uses many saved registers */
    fn_800FF730();
    fn_800F0308();
    return 0;
}

/* 0x800886D0 | size: 0x294 */
s32 fn_800886D0(void) {
    /* TODO: decompile (0x294 bytes, ~165 instructions) */
    fn_8006A76C();
    fn_8006AE18();
    fn_801EE398();
    fn_801902E0();
    fn_80071104();
    fn_80128E24();
    fn_801CADA8();
    fn_80266320();
    fn_8007109C();
    fn_8006ADEC();
    fn_8006ADB4();
    fn_801293FC();
    return 0;
}

/* 0x80088964 | size: 0x40 */
s32 fn_80088964(void) {
    /* TODO: decompile (0x40 bytes) */
    fn_801D0748();
    return -1;
}

/* 0x800889A4 | size: 0x40 */
s32 fn_800889A4(void) {
    /* TODO: decompile (0x40 bytes) */
    fn_801D0748();
    return -1;
}

/* 0x800889E4 | size: 0x27C */
s32 fn_800889E4(void) {
    /* TODO: decompile (0x27C bytes, ~159 instructions) */
    /* Uses many saved registers */
    fn_80129280();
    fn_8006A7BC();
    fn_80196E10();
    fn_8012D32C();
    fn_8012D2BC();
    fn_800FF56C();
    fn_8011394C();
    fn_801906A0();
    fn_8006A7AC();
    fn_8006A76C();
    fn_80071104();
    fn_80128E24();
    return 0;
}

/* 0x80088C60 | size: 0x124 */
s32 fn_80088C60(void) {
    /* TODO: decompile (0x124 bytes) */
    fn_8006A76C();
    fn_80071104();
    fn_80128E24();
    fn_80129280();
    fn_8019075C();
    fn_801D0748();
    fn_8007109C();
    return -1;
}

/* 0x80088D84 | size: 0x124 */
s32 fn_80088D84(void) {
    /* TODO: decompile (0x124 bytes) */
    fn_8006A76C();
    fn_80071104();
    fn_80128E24();
    fn_80129280();
    fn_8019075C();
    fn_801D0748();
    fn_8007109C();
    return -1;
}

/* 0x80088EA8 | size: 0xB0 */
s32 fn_80088EA8(void) {
    /* TODO: decompile (0xB0 bytes) */
    fn_8012D32C();
    fn_8012D2BC();
    fn_800FF56C();
    fn_8011394C();
    fn_801906A0();
    return 0;
}

/* 0x80088F58 | size: 0x1C */
s32 fn_80088F58(void) {
    return 0;
}

/* 0x80088F74 | size: 0x14 */
s32 fn_80088F74(void) {
    return 0;
}

/* 0x80088F88 | size: 0x1C */
s32 fn_80088F88(void) {
    return 0;
}

/* 0x80088FA4 | size: 0x54 */
s32 fn_80088FA4(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_8010264C();
    fn_80102510();
    return 0;
}

/* 0x80088FF8 | size: 0x30 */
s32 fn_80088FF8(void) {
    fn_80089030();
    return 0;
}

/* 0x80089028 | size: 0x8 */
void fn_80089028(void) {
}

/* 0x80089030 | size: 0x18 */
void fn_80089030(void) {
}

#pragma pop
