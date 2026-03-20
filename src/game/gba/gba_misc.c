/**
 * @file gba_misc.c
 * @brief GBA miscellaneous communication support (0x800895A4-0x80092C90)
 *
 * Address range: 0x800895A4 - 0x80092C90
 * Total functions: 69
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001E184();
extern void fn_80071700();
extern void fn_800719A8();
extern void fn_80071AE4();
extern void fn_800722A0();
extern void fn_80072548();
extern void fn_800726A8();
extern void fn_80072A00();
extern void fn_80072C74();
extern void fn_80072D58();
extern void fn_80073034();
extern void fn_800730F8();
extern void fn_800733D0();
extern void fn_80073990();
extern void fn_80073A44();
extern void fn_800830A4();
extern void fn_80083BF8();
extern void fn_80083CFC();
extern void fn_80083D30();
extern void fn_80083ECC();
extern void fn_800C46B0();
extern void fn_800C8174();
extern void fn_800D3088();
extern void fn_800D37CC();
extern void fn_800E3C00();
extern void fn_800E3C08();
extern void fn_800E8FA0();
extern void fn_800E8FE8();
extern void fn_800E900C();
extern void fn_800E9108();
extern void fn_800EC4D0();
extern void fn_800EC990();
extern void fn_800ECA78();
extern void fn_800ECB74();
extern void fn_800ECCA8();
extern void fn_800F0308();
extern void fn_800F9318();
extern void fn_800F9AEC();
extern void fn_800F9C04();
extern void fn_800FF58C();
extern void fn_8011288C();
extern void fn_80113F48();
extern void fn_80118874();
extern void fn_8011D480();
extern void fn_8011D494();
extern void fn_8011D4A4();
extern void fn_8011D4B4();
extern void fn_8011D4C4();
extern void fn_8011D4D4();
extern void fn_8011D4E4();
extern void fn_8011D4F4();
extern void fn_8011D504();
extern void fn_8011D56C();
extern void fn_8011D57C();
extern void fn_8011D58C();
extern void fn_8011D5B0();
extern void fn_8011D5D4();
extern void fn_8011D5F8();
extern void fn_8011D61C();
extern void fn_8011D640();
/* ... and 210 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478960;
extern u8 lbl_8047A670;
extern u8 lbl_8047A674;
extern u8 lbl_8047A678;
extern u8 lbl_8047A67C;
extern u8 lbl_8047A684;
extern u8 lbl_8047A690;
extern u8 lbl_8047A694;
extern u8 lbl_8047C1D0;
extern u8 lbl_8047C1D4;
extern u8 lbl_8047C1D8;
extern u8 lbl_8047C1DC;
extern u8 lbl_8047C1E0;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EEBB8[];
extern u8 jumptable_802EEBE0[];
extern u8 jumptable_802EEC10[];
extern u8 jumptable_802EEC30[];
extern u8 lbl_802EEB98[];
extern u8 lbl_802EEC70[];
extern u8 lbl_803FB308[];
extern u8 lbl_803FB318[];

/* ===== Forward declarations ===== */
s32 fn_800895A4(void);
void fn_800896B8(void);
void fn_800896C0(void);
void fn_800896C8(void);
void fn_800896D0(void);
void fn_800896D8(void);
void fn_800896E0(void);
s32 fn_800896E8(void);
s32 fn_80089978(void);
s32 fn_80089B8C(void);
s32 fn_80089C10(void);
s32 fn_80089C54(void);
void fn_80089C84(void);
s32 fn_80089CA8(void);
void fn_80089D30(void);
void fn_80089D74(void);
s32 fn_80089D98(void);
void fn_80089E20(void);
void fn_80089F58(void);
void fn_80089F60(void);
void fn_80089F68(void);
void fn_80089F70(void);
s32 fn_80089F78(void);
s32 fn_8008A99C(void);
s32 fn_8008A9AC(void);
s32 fn_8008A9E4(void);
void fn_8008AB20(void);
void fn_8008AB4C(void);
s32 fn_8008AB8C(void);
void fn_8008ABA0(void);
void fn_8008ABE4(void);
s32 fn_8008AC34(void);
s32 fn_8008AE18(void);
void fn_8008BBDC(void);
void fn_8008C5D4(void);
void fn_8008C6FC(void);
s32 fn_8008C700(void);
s32 fn_8008C78C(void);
s32 fn_8008C7B0(void);
s32 fn_8008CACC(void);
s32 fn_8008CDD8(void);
s32 fn_8008D0A0(void);
s32 fn_8008D348(void);
s32 fn_8008D938(void);
s32 fn_8008E320(void);
s32 fn_8008E7D4(void);
s32 fn_8008EC28(void);
s32 fn_8008EED0(void);
s32 fn_8008F190(void);
s32 fn_8008F524(void);
s32 fn_8008F91C(void);
s32 fn_8008FBF4(void);
s32 fn_8008FE94(void);
s32 fn_80090100(void);
s32 fn_80090720(void);
s32 fn_800909E4(void);
s32 fn_80090D34(void);
s32 fn_8009100C(void);
s32 fn_80091564(void);
s32 fn_80091774(void);
s32 fn_80091984(void);
s32 fn_80091B94(void);
s32 fn_80091DA4(void);
s32 fn_80091F48(void);
s32 fn_80092140(void);
s32 fn_80092498(void);
s32 fn_80092664(void);
s32 fn_800929BC(void);
s32 fn_80092B2C(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x800895A4 | size: 0x114 */
s32 fn_800895A4(void) {
    /* TODO: decompile (0x114 bytes) */
    fn_8012A774();
    fn_80135938();
    fn_800F9C04();
    fn_8012AA64();
    fn_8012AA44();
    fn_8012AA54();
    fn_8012AC08();
    fn_8008BBDC();
    return 0;
}

/* 0x800896B8 | size: 0x8 */
void fn_800896B8(void) {
}

/* 0x800896C0 | size: 0x8 */
void fn_800896C0(void) {
}

/* 0x800896C8 | size: 0x8 */
void fn_800896C8(void) {
}

/* 0x800896D0 | size: 0x8 */
void fn_800896D0(void) {
}

/* 0x800896D8 | size: 0x8 */
void fn_800896D8(void) {
}

/* 0x800896E0 | size: 0x8 */
void fn_800896E0(void) {
}

/* 0x800896E8 | size: 0x290 */
s32 fn_800896E8(void) {
    /* TODO: decompile (0x290 bytes, ~164 instructions) */
    /* Uses many saved registers */
    fn_80083CFC();
    fn_80132A38();
    fn_80189990();
    fn_8001E184();
    fn_8020E0F8();
    fn_801FCCC4();
    fn_801FCB94();
    fn_801FCC54();
    fn_801FCB84();
    fn_801FCAFC();
    fn_801FCB40();
    fn_801FCC3C();
    return 0;
}

/* 0x80089978 | size: 0x214 */
s32 fn_80089978(void) {
    /* TODO: decompile (0x214 bytes, ~133 instructions) */
    fn_801EEAD0();
    fn_801FC794();
    fn_801FC828();
    fn_801FC7B4();
    fn_801EEE6C();
    fn_801FC7D4();
    fn_801FC744();
    fn_801FC784();
    fn_801FC7A4();
    fn_801FC808();
    fn_801FC7E4();
    fn_801FC6F4();
    return 0;
}

/* 0x80089B8C | size: 0x84 */
s32 fn_80089B8C(void) {
    /* TODO: decompile (0x84 bytes) */
    fn_80083CFC();
    return 0;
}

/* 0x80089C10 | size: 0x44 */
s32 fn_80089C10(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_80083CFC();
    return 0;
}

/* 0x80089C54 | size: 0x30 */
s32 fn_80089C54(void) {
    fn_80083BF8();
    return 0;
}

/* 0x80089C84 | size: 0x24 */
void fn_80089C84(void) {
    fn_80071700();
}

/* 0x80089CA8 | size: 0x88 */
s32 fn_80089CA8(void) {
    /* TODO: decompile (0x88 bytes) */
    fn_800719A8();
    return -1;
}

/* 0x80089D30 | size: 0x44 */
void fn_80089D30(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_80071AE4();
}

/* 0x80089D74 | size: 0x24 */
void fn_80089D74(void) {
    fn_800722A0();
}

/* 0x80089D98 | size: 0x88 */
s32 fn_80089D98(void) {
    /* TODO: decompile (0x88 bytes) */
    fn_80072548();
    return -1;
}

/* 0x80089E20 | size: 0x138 */
void fn_80089E20(void) {
    /* TODO: decompile (0x138 bytes) */
    fn_8011F5C8();
    fn_8011E868();
    fn_8011D504();
    fn_8008AE18();
    fn_80265F14();
    fn_800726A8();
}

/* 0x80089F58 | size: 0x8 */
void fn_80089F58(void) {
}

/* 0x80089F60 | size: 0x8 */
void fn_80089F60(void) {
}

/* 0x80089F68 | size: 0x8 */
void fn_80089F68(void) {
}

/* 0x80089F70 | size: 0x8 */
void fn_80089F70(void) {
}

/* 0x80089F78 | size: 0xA24 */
s32 fn_80089F78(void) {
    /* TODO: decompile (0xA24 bytes, ~649 instructions) */
    /* Uses many saved registers */
    fn_801F54A4();
    fn_8020E204();
    fn_8020E1D4();
    fn_8020E1BC();
    fn_8020E1A4();
    fn_801F02AC();
    fn_801F981C();
    fn_801FCDB4();
    fn_801F986C();
    fn_80205BE8();
    fn_8011E7C0();
    fn_801F8C00();
    return 0;
}

/* 0x8008A99C | size: 0x10 */
s32 fn_8008A99C(void) {
    return 0;
}

/* 0x8008A9AC | size: 0x38 */
s32 fn_8008A9AC(void) {
    /* TODO: decompile (0x38 bytes) */
    return 0;
}

/* 0x8008A9E4 | size: 0x13C */
s32 fn_8008A9E4(void) {
    /* TODO: decompile (0x13C bytes) */
    fn_80073034();
    return 0;
}

/* 0x8008AB20 | size: 0x2C */
void fn_8008AB20(void) {
    fn_800730F8();
}

/* 0x8008AB4C | size: 0x40 */
void fn_8008AB4C(void) {
    /* TODO: decompile (0x40 bytes) */
    fn_80083D30();
    fn_800733D0();
}

/* 0x8008AB8C | size: 0x14 */
s32 fn_8008AB8C(void) {
    return 0;
}

/* 0x8008ABA0 | size: 0x44 */
void fn_8008ABA0(void) {
    /* TODO: decompile (0x44 bytes) */
}

/* 0x8008ABE4 | size: 0x50 */
void fn_8008ABE4(void) {
    /* TODO: decompile (0x50 bytes) */
}

/* 0x8008AC34 | size: 0x1E4 */
s32 fn_8008AC34(void) {
    /* TODO: decompile (0x1E4 bytes) */
    fn_80073A44();
    fn_80073990();
    return 0;
}

/* 0x8008AE18 | size: 0xDC4 */
s32 fn_8008AE18(void) {
    /* TODO: decompile (0xDC4 bytes, ~881 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80123FBC();
    fn_8011F5B0();
    fn_8011F520();
    fn_8011F598();
    fn_80135AB8();
    fn_80135A70();
    fn_8011E8DC();
    fn_8011E838();
    fn_8011E850();
    fn_8011F4D8();
    fn_800F9AEC();
    fn_8011F508();
    return 0;
}

/* 0x8008BBDC | size: 0x9F8 */
void fn_8008BBDC(void) {
    /* TODO: decompile (0x9F8 bytes, ~638 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80124A60();
    fn_800C8174();
    fn_8011DFE0();
    fn_8011DF90();
    fn_8011F598();
    fn_801353C0();
    fn_800F9C04();
    fn_8011DEE4();
    fn_8011D4E4();
    fn_8011D4F4();
    fn_8011F508();
    fn_8011D4D4();
}

/* 0x8008C5D4 | size: 0x128 */
void fn_8008C5D4(void) {
    /* TODO: decompile (0x128 bytes) */
    fn_80121ADC();
    fn_80121984();
    fn_8012189C();
}

/* 0x8008C6FC | size: 0x4 */
void fn_8008C6FC(void) {
}

/* 0x8008C700 | size: 0x8C */
s32 fn_8008C700(void) {
    /* TODO: decompile (0x8C bytes) */
    /* Contains switch statement */
    fn_80113F48();
    fn_801906A0();
    fn_8019075C();
    return 0;
}

/* 0x8008C78C | size: 0x24 */
s32 fn_8008C78C(void) {
    fn_801906A0();
    return 0;
}

/* 0x8008C7B0 | size: 0x31C */
s32 fn_8008C7B0(void) {
    /* TODO: decompile (0x31C bytes, ~199 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008CACC | size: 0x30C */
s32 fn_8008CACC(void) {
    /* TODO: decompile (0x30C bytes, ~195 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008CDD8 | size: 0x2C8 */
s32 fn_8008CDD8(void) {
    /* TODO: decompile (0x2C8 bytes, ~178 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008D0A0 | size: 0x2A8 */
s32 fn_8008D0A0(void) {
    /* TODO: decompile (0x2A8 bytes, ~170 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008D348 | size: 0x5F0 */
s32 fn_8008D348(void) {
    /* TODO: decompile (0x5F0 bytes, ~380 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008D938 | size: 0x9E8 */
s32 fn_8008D938(void) {
    /* TODO: decompile (0x9E8 bytes, ~634 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008E320 | size: 0x4B4 */
s32 fn_8008E320(void) {
    /* TODO: decompile (0x4B4 bytes, ~301 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008E7D4 | size: 0x454 */
s32 fn_8008E7D4(void) {
    /* TODO: decompile (0x454 bytes, ~277 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008EC28 | size: 0x2A8 */
s32 fn_8008EC28(void) {
    /* TODO: decompile (0x2A8 bytes, ~170 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008EED0 | size: 0x2C0 */
s32 fn_8008EED0(void) {
    /* TODO: decompile (0x2C0 bytes, ~176 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008F190 | size: 0x394 */
s32 fn_8008F190(void) {
    /* TODO: decompile (0x394 bytes, ~229 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008F524 | size: 0x3F8 */
s32 fn_8008F524(void) {
    /* TODO: decompile (0x3F8 bytes, ~254 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008F91C | size: 0x2D8 */
s32 fn_8008F91C(void) {
    /* TODO: decompile (0x2D8 bytes, ~182 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008FBF4 | size: 0x2A0 */
s32 fn_8008FBF4(void) {
    /* TODO: decompile (0x2A0 bytes, ~168 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x8008FE94 | size: 0x26C */
s32 fn_8008FE94(void) {
    /* TODO: decompile (0x26C bytes, ~155 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x80090100 | size: 0x620 */
s32 fn_80090100(void) {
    /* TODO: decompile (0x620 bytes, ~392 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    fn_80118874();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    return 0;
}

/* 0x80090720 | size: 0x2C4 */
s32 fn_80090720(void) {
    /* TODO: decompile (0x2C4 bytes, ~177 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    fn_800ECB74();
    fn_800EC990();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    return 0;
}

/* 0x800909E4 | size: 0x350 */
s32 fn_800909E4(void) {
    /* TODO: decompile (0x350 bytes, ~212 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    fn_800ECB74();
    fn_800EC990();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    return 0;
}

/* 0x80090D34 | size: 0x2D8 */
s32 fn_80090D34(void) {
    /* TODO: decompile (0x2D8 bytes, ~182 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_800ECCA8();
    fn_800EC4D0();
    fn_800ECA78();
    fn_800ECB74();
    fn_800EC990();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    return 0;
}

/* 0x8009100C | size: 0x558 */
s32 fn_8009100C(void) {
    /* TODO: decompile (0x558 bytes, ~342 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CBA0C();
    fn_800E9108();
    fn_800E8FE8();
    fn_800E900C();
    fn_80176E0C();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801845E4();
    return 0;
}

/* 0x80091564 | size: 0x210 */
s32 fn_80091564(void) {
    /* TODO: decompile (0x210 bytes, ~132 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB7C4();
    fn_800E3C08();
    fn_800E3C00();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CBA0C();
    fn_800E9108();
    return 0;
}

/* 0x80091774 | size: 0x210 */
s32 fn_80091774(void) {
    /* TODO: decompile (0x210 bytes, ~132 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB7C4();
    fn_800E3C08();
    fn_800E3C00();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CBA0C();
    fn_800E9108();
    return 0;
}

/* 0x80091984 | size: 0x210 */
s32 fn_80091984(void) {
    /* TODO: decompile (0x210 bytes, ~132 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB7C4();
    fn_800E3C08();
    fn_800E3C00();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CBA0C();
    fn_800E9108();
    return 0;
}

/* 0x80091B94 | size: 0x210 */
s32 fn_80091B94(void) {
    /* TODO: decompile (0x210 bytes, ~132 instructions) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB7C4();
    fn_800E3C08();
    fn_800E3C00();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CBA0C();
    fn_800E9108();
    return 0;
}

/* 0x80091DA4 | size: 0x1A4 */
s32 fn_80091DA4(void) {
    /* TODO: decompile (0x1A4 bytes) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB7C4();
    fn_801CB834();
    fn_800E3C08();
    fn_800E3C00();
    fn_800D37CC();
    fn_800C46B0();
    return 0;
}

/* 0x80091F48 | size: 0x1F8 */
s32 fn_80091F48(void) {
    /* TODO: decompile (0x1F8 bytes) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CBA0C();
    fn_800E9108();
    fn_800E8FE8();
    fn_800E900C();
    fn_80176E0C();
    fn_800D37CC();
    return 0;
}

/* 0x80092140 | size: 0x358 */
s32 fn_80092140(void) {
    /* TODO: decompile (0x358 bytes, ~214 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CBA0C();
    fn_800E9108();
    fn_800E8FE8();
    fn_800E900C();
    fn_80176E0C();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801845E4();
    return 0;
}

/* 0x80092498 | size: 0x1CC */
s32 fn_80092498(void) {
    /* TODO: decompile (0x1CC bytes) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    return 0;
}

/* 0x80092664 | size: 0x358 */
s32 fn_80092664(void) {
    /* TODO: decompile (0x358 bytes, ~214 instructions) */
    /* Uses many saved registers */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CBA0C();
    fn_800E9108();
    fn_800E8FE8();
    fn_800E900C();
    fn_80176E0C();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801845E4();
    return 0;
}

/* 0x800929BC | size: 0x170 */
s32 fn_800929BC(void) {
    /* TODO: decompile (0x170 bytes) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CB834();
    fn_800D37CC();
    fn_800C46B0();
    fn_800F0308();
    fn_800D3088();
    fn_801CB7C4();
    return 0;
}

/* 0x80092B2C | size: 0x164 */
s32 fn_80092B2C(void) {
    /* TODO: decompile (0x164 bytes) */
    fn_800F9318();
    fn_800E8FA0();
    fn_801CBA0C();
    fn_801CB834();
    fn_800E9108();
    fn_800E8FE8();
    fn_800E900C();
    fn_80113F48();
    return 0;
}

#pragma pop
