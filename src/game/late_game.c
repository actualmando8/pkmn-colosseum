/**
 * @file late_game.c
 * @brief Late game code before SDK (0x800937F4-0x80097FFC)
 *
 * Address range: 0x800937F4 - 0x80098004
 * Total functions: 25
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001D624();
extern void fn_8001DA60();
extern void fn_8001E58C();
extern void fn_8005D858();
extern void fn_8005D934();
extern void fn_8006AEEC();
extern void fn_80071E34();
extern void fn_80073690();
extern void fn_80073E84();
extern void fn_80073E8C();
extern void fn_80074324();
extern void fn_800745B4();
extern void fn_80089380();
extern void fn_800895A4();
extern void fn_80089C84();
extern void fn_80089CA8();
extern void fn_80089D30();
extern void fn_8009F7B4();
extern void fn_8009F890();
extern void fn_8009F9E8();
extern void fn_800A13F8();
extern void fn_800A1990();
extern void fn_800A257C();
extern void fn_800C46B0();
extern void fn_800C4CC0();
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
extern void fn_800F0308();
extern void fn_800F9318();
extern void fn_800F9EE4();
extern void fn_800FA280();
extern void fn_800FA444();
extern void fn_800FAEF8();
extern void fn_800FB680();
extern void fn_800FB8C8();
extern void fn_800FBB34();
extern void fn_800FF660();
extern void fn_800FF730();
extern void fn_80102510();
extern void fn_80102568();
extern void fn_80102620();
extern void fn_8010264C();
extern void fn_801026A4();
extern void fn_80103484();
extern void fn_801040F0();
extern void fn_80104160();
extern void fn_80104704();
extern void fn_80105624();
extern void fn_801069FC();
extern void fn_80106D3C();
extern void fn_80109220();
extern void fn_80109934();
/* ... and 47 more external functions */
extern void OSGetTime();
extern void* memset(void* dst, int val, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_8047C1F0;
extern u8 lbl_8047C1F8;
extern u8 lbl_8047C200;
extern u8 lbl_8047C204;
extern u8 lbl_8047C208;
extern u8 lbl_8047C20C;
extern u8 lbl_8047C210;
extern u8 lbl_8047C214;
extern u8 lbl_8047C218;
extern u8 lbl_8047C21C;
extern u8 lbl_8047C220;
extern u8 lbl_8047C228;
extern u8 lbl_8047C230;
extern u8 lbl_8047C234;
extern u8 lbl_8047C238;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EECF0[];
extern u8 jumptable_802EF01C[];
extern u8 jumptable_802EF038[];
extern u8 jumptable_802EF05C[];
extern u8 jumptable_802EF080[];
extern u8 lbl_8026F5C0[];
extern u8 lbl_8026F5E4[];
extern u8 lbl_802EED28[];
extern u8 lbl_802EED44[];
extern u8 lbl_802EEEC4[];
extern u8 lbl_802EEFC4[];
extern u8 lbl_802EEFD8[];
extern u8 lbl_802EF000[];
extern u8 lbl_80314F98[];
extern u8 lbl_803FB338[];
extern u8 lbl_803FB380[];

/* ===== Forward declarations ===== */
s32 fn_800937F4(void);
void fn_80093B04(void);
s32 fn_80093B4C(void);
void fn_80093F2C(void);
s32 fn_80093F64(void);
s32 fn_80094650(void);
s32 fn_8009567C(void);
s32 fn_800965C8(void);
s32 fn_80096C48(void);
s32 fn_80096D54(void);
s32 fn_80096FA0(void);
s32 fn_800973EC(void);
s32 fn_8009769C(void);
void fn_800979EC(void);
s32 fn_80097A38(void);
s32 fn_80097B04(void);
s32 fn_80097BBC(void);
s32 fn_80097CD0(void);
s32 fn_80097D94(void);
s32 fn_80097E58(void);
s32 fn_80097F08(void);
void fn_80097FCC(void);
void fn_80097FD0(void);
void fn_80097FF8(void);
void PPCMfmsr(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x800937F4 | size: 0x310 */
s32 fn_800937F4(void) {
    /* TODO: decompile (0x310 bytes, ~196 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8009F7B4();
    fn_8009F9E8();
    fn_8009F890();
    fn_80073E8C();
    fn_800A13F8();
    fn_800A257C();
    fn_800A1990();
    fn_80073E84();
    OSGetTime();
    fn_800C4CC0();
    fn_80074324();
    fn_800745B4();
    return 0;
}

/* 0x80093B04 | size: 0x48 */
void fn_80093B04(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_800A13F8();
    fn_800A257C();
    fn_800A1990();
}

/* 0x80093B4C | size: 0x3E0 */
s32 fn_80093B4C(void) {
    /* TODO: decompile (0x3E0 bytes, ~248 instructions) */
    /* Uses many saved registers */
    fn_80104704();
    fn_80132A38();
    fn_800FBB34();
    fn_800FAEF8();
    fn_80265F14();
    fn_8005D934();
    fn_80104160();
    fn_8005D858();
    return -1;
}

/* 0x80093F2C | size: 0x38 */
void fn_80093F2C(void) {
    /* TODO: decompile (0x38 bytes) */
    fn_80093F64();
}

/* 0x80093F64 | size: 0x6EC */
s32 fn_80093F64(void) {
    /* TODO: decompile (0x6EC bytes, ~443 instructions) */
    /* Uses many saved registers */
    fn_8012640C();
    return 0;
}

/* 0x80094650 | size: 0x102C */
s32 fn_80094650(void) {
    /* TODO: decompile (0x102C bytes, ~1035 instructions) */
    /* Uses many saved registers */
    fn_80109220();
    fn_8001E58C();
    fn_8012640C();
    fn_80123CD4();
    fn_8011BEB4();
    fn_8010C46C();
    fn_801040F0();
    fn_800FBB34();
    fn_800FA280();
    fn_80132A38();
    fn_800FA444();
    fn_800FB680();
    return 0;
}

/* 0x8009567C | size: 0xF4C */
s32 fn_8009567C(void) {
    /* TODO: decompile (0xF4C bytes, ~979 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8012640C();
    fn_8011E778();
    fn_80109220();
    fn_8011E474();
    fn_8010C46C();
    fn_801040F0();
    fn_8011F77C();
    fn_8011CE18();
    fn_8011CE00();
    fn_80132A38();
    fn_80135938();
    fn_801F2A7C();
    return 0;
}

/* 0x800965C8 | size: 0x680 */
s32 fn_800965C8(void) {
    /* TODO: decompile (0x680 bytes, ~416 instructions) */
    fn_8012640C();
    fn_8011E778();
    fn_80109934();
    fn_800D88DC();
    fn_800D888C();
    fn_800D6A00();
    fn_800D7820();
    fn_800D85D4();
    fn_800D67BC();
    fn_800D61E4();
    fn_800D5CB8();
    fn_800D59B8();
    return 0;
}

/* 0x80096C48 | size: 0x10C */
s32 fn_80096C48(void) {
    /* TODO: decompile (0x10C bytes) */
    fn_80104704();
    return 0;
}

/* 0x80096D54 | size: 0x24C */
s32 fn_80096D54(void) {
    /* TODO: decompile (0x24C bytes, ~147 instructions) */
    /* Contains switch/jump table */
    fn_80105624();
    fn_8012640C();
    fn_80166A28();
    fn_80123C54();
    return -1;
}

/* 0x80096FA0 | size: 0x44C */
s32 fn_80096FA0(void) {
    /* TODO: decompile (0x44C bytes, ~275 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80105624();
    fn_80103484();
    fn_80109C88();
    fn_8012640C();
    fn_80123CD4();
    return 0;
}

/* 0x800973EC | size: 0x2B0 */
s32 fn_800973EC(void) {
    /* TODO: decompile (0x2B0 bytes, ~172 instructions) */
    fn_8012640C();
    fn_80102620();
    fn_8010264C();
    fn_80102510();
    return 0;
}

/* 0x8009769C | size: 0x350 */
s32 fn_8009769C(void) {
    /* TODO: decompile (0x350 bytes, ~212 instructions) */
    fn_8010A5BC();
    fn_80109C88();
    fn_801C40F0();
    fn_801C41C8();
    fn_801026A4();
    fn_8012640C();
    fn_80123CD4();
    fn_8011BEB4();
    fn_80106D3C();
    fn_801069FC();
    fn_80102620();
    fn_80102568();
    return 0;
}

/* 0x800979EC | size: 0x4C */
void fn_800979EC(void) {
    /* TODO: decompile (0x4C bytes) */
    fn_8009769C();
}

/* 0x80097A38 | size: 0xCC */
s32 fn_80097A38(void) {
    /* TODO: decompile (0xCC bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_800FF730();
    fn_8011288C();
    return 0;
}

/* 0x80097B04 | size: 0xB8 */
s32 fn_80097B04(void) {
    /* TODO: decompile (0xB8 bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_8009769C();
    return 0;
}

/* 0x80097BBC | size: 0x114 */
s32 fn_80097BBC(void) {
    /* TODO: decompile (0x114 bytes) */
    fn_80129280();
    fn_8012AC08();
    fn_80123FBC();
    fn_800F0308();
    fn_8010B560();
    fn_800FF730();
    fn_8011288C();
    return -1;
}

/* 0x80097CD0 | size: 0xC4 */
s32 fn_80097CD0(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_8009769C();
    return 0;
}

/* 0x80097D94 | size: 0xC4 */
s32 fn_80097D94(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_8009769C();
    return 0;
}

/* 0x80097E58 | size: 0xB0 */
s32 fn_80097E58(void) {
    /* TODO: decompile (0xB0 bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_8009769C();
    return 0;
}

/* 0x80097F08 | size: 0xC4 */
s32 fn_80097F08(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_800F0308();
    fn_8010B560();
    fn_8009769C();
    return 0;
}

/* 0x80097FCC | size: 0x4 */
void fn_80097FCC(void) {
}

/* 0x80097FD0 | size: 0x28 */
void fn_80097FD0(void) {
    fn_80113F48();
    fn_800F9318();
}

/* 0x80097FF8 | size: 0x4 */
void fn_80097FF8(void) {
}

/* 0x80097FFC | size: 0x8 */
void PPCMfmsr(void) {
}

#pragma pop
