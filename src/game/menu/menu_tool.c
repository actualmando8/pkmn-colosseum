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
s32 fn_80072A00(void) {
    /* TODO: decompile (0x274 bytes, ~157 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    fn_80073C38();
    fn_8025F648();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    fn_800F0308();
    return 0;
}

/* 0x80072C74 | size: 0xE4 */
void fn_80072C74(void) {
    /* TODO: decompile (0xE4 bytes) */
    fn_800D0F44();
    fn_8025F3F4();
    fn_8025F648();
    fn_8025F584();
    fn_8008ABE4();
}

/* 0x80072D58 | size: 0x2DC */
s32 fn_80072D58(void) {
    /* TODO: decompile (0x2DC bytes, ~183 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    fn_800F0308();
    return 0;
}

/* 0x80073034 | size: 0xC4 */
s32 fn_80073034(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_800D0F44();
    fn_8025F3F4();
    fn_8025F648();
    fn_8025F584();
    return -1;
}

/* 0x800730F8 | size: 0x2D8 */
s32 fn_800730F8(void) {
    /* TODO: decompile (0x2D8 bytes, ~182 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x800733D0 | size: 0x2C0 */
s32 fn_800733D0(void) {
    /* TODO: decompile (0x2C0 bytes, ~176 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8008ABE4();
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    fn_800F0308();
    return 0;
}

/* 0x80073690 | size: 0x70 */
void fn_80073690(void) {
    /* TODO: decompile (0x70 bytes) */
    fn_8008ABE4();
    fn_80073700();
}

/* 0x80073700 | size: 0x290 */
s32 fn_80073700(void) {
    /* TODO: decompile (0x290 bytes, ~164 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80073C38();
    fn_8025F648();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x80073990 | size: 0xB4 */
s32 fn_80073990(void) {
    /* TODO: decompile (0xB4 bytes) */
    OSGetTick();
    fn_800D0F44();
    fn_8025F648();
    return 0;
}

/* 0x80073A44 | size: 0x1F4 */
s32 fn_80073A44(void) {
    /* TODO: decompile (0x1F4 bytes) */
    /* Contains switch statement */
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    return 0;
}

/* 0x80073C38 | size: 0x24C */
s32 fn_80073C38(void) {
    /* TODO: decompile (0x24C bytes, ~147 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_800D0F44();
    fn_8025F484();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    fn_8025F648();
    return 0;
}

/* 0x80073E84 | size: 0x8 */
s32 fn_80073E84(void) {
    return 0x1;
}

/* 0x80073E8C | size: 0x498 */
s32 fn_80073E8C(void) {
    /* TODO: decompile (0x498 bytes, ~294 instructions) */
    fn_800A7BCC();
    fn_8025F350();
    fn_800A501C();
    fn_800060F0();
    fn_800A541C();
    fn_800A50E4();
    return 0;
}

/* 0x80074324 | size: 0x3C */
void fn_80074324(void) {
    /* TODO: decompile (0x3C bytes) */
    fn_8008ABE4();
    fn_80074360();
}

/* 0x80074360 | size: 0x254 */
s32 fn_80074360(void) {
    /* TODO: decompile (0x254 bytes, ~149 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_800D0F44();
    fn_8025F484();
    OSGetTick();
    fn_8025F3F4();
    fn_8025F584();
    fn_8025F648();
    return 0;
}

/* 0x800745B4 | size: 0x258 */
s32 fn_800745B4(void) {
    /* TODO: decompile (0x258 bytes, ~150 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8007480C();
    OSGetTick();
    fn_80073C38();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    fn_8008ABE4();
    return 0;
}

/* 0x8007480C | size: 0xB84 */
s32 fn_8007480C(void) {
    /* TODO: decompile (0xB84 bytes, ~737 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    OSGetTick();
    fn_8025F648();
    fn_8025F3F4();
    fn_8025F584();
    OSReport();
    fn_800D0F44();
    return 0;
}

/* 0x80075390 | size: 0x40 */
s32 fn_80075390(void) {
    /* TODO: decompile (0x40 bytes) */
    fn_80075638();
    fn_8007565C();
    fn_800756C8();
    return 0;
}

/* 0x800753D0 | size: 0x148 */
void fn_800753D0(void) {
    /* TODO: decompile (0x148 bytes) */
    fn_800D37CC();
    fn_800D3088();
    fn_800CE148();
    fn_80109B90();
    fn_801DAC3C();
    fn_800E3DC4();
}

/* 0x80075518 | size: 0x120 */
s32 fn_80075518(void) {
    /* TODO: decompile (0x120 bytes) */
    fn_80109934();
    fn_800D88DC();
    fn_800D888C();
    fn_800D6A00();
    fn_800D7820();
    fn_800D85D4();
    fn_800D67BC();
    fn_800D61E4();
    return 0;
}

/* 0x80075638 | size: 0x24 */
s32 fn_80075638(void) {
    fn_80102620();
    return 0;
}

/* 0x8007565C | size: 0x6C */
s32 fn_8007565C(void) {
    /* TODO: decompile (0x6C bytes) */
    fn_8010A420();
    fn_80102568();
    fn_800E202C();
    fn_800E24B0();
    fn_800E209C();
    return 0;
}

/* 0x800756C8 | size: 0x128 */
s32 fn_800756C8(void) {
    /* TODO: decompile (0x128 bytes) */
    fn_800FF56C();
    fn_800E2C04();
    fn_800E27B0();
    fn_80135938();
    fn_801240C4();
    fn_800CE148();
    fn_8010A5BC();
    fn_80109C88();
    return 0;
}

/* 0x800757F0 | size: 0x2C */
void fn_800757F0(void) {
    fn_801CB9D8();
}

#pragma pop
