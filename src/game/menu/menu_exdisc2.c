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

#pragma push
#pragma force_active on

/* 0x80078D38 | size: 0x24 */
s32 fn_80078D38(void) {
    fn_801C40F0();
    return 0;
}

/* 0x80078D5C | size: 0x510 */
s32 fn_80078D5C(void) {
    /* TODO: decompile (0x510 bytes, ~324 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    fn_801D036C();
    fn_80113F48();
    fn_80176E0C();
    fn_801CB834();
    fn_80075C44();
    fn_801C41C8();
    fn_801C40F0();
    fn_801067E8();
    fn_801069FC();
    fn_80075C20();
    fn_80166A28();
    fn_800F0308();
    return 0;
}

/* 0x8007926C | size: 0x6C */
void fn_8007926C(void) {
    /* TODO: decompile (0x6C bytes) */
    fn_801CBA0C();
    fn_801CB954();
    fn_801CB61C();
    fn_801CB834();
}

/* 0x800792D8 | size: 0x610 */
s32 fn_800792D8(void) {
    /* TODO: decompile (0x610 bytes, ~388 instructions) */
    fn_801C41C8();
    fn_801C40F0();
    fn_80102510();
    fn_80113F48();
    fn_800F9318();
    fn_800E4014();
    fn_801CBA0C();
    fn_80176E0C();
    fn_80177A44();
    fn_800849B4();
    fn_801CB9D8();
    fn_80079EF4();
    return 0;
}

/* 0x800798E8 | size: 0x334 */
s32 fn_800798E8(void) {
    /* TODO: decompile (0x334 bytes, ~205 instructions) */
    /* Uses many saved registers */
    fn_801D0748();
    fn_80135168();
    fn_80106D3C();
    fn_801069FC();
    fn_80102510();
    fn_80075AC0();
    fn_80075B08();
    fn_80075B50();
    fn_8012A5B0();
    fn_80079EF4();
    fn_80129280();
    fn_80128DD4();
    return 0;
}

/* 0x80079C1C | size: 0x2D8 */
s32 fn_80079C1C(void) {
    /* TODO: decompile (0x2D8 bytes, ~182 instructions) */
    fn_801067E8();
    fn_801069FC();
    fn_80106D3C();
    fn_80102510();
    fn_80132A38();
    fn_80165668();
    return 0;
}

/* 0x80079EF4 | size: 0x6F4 */
s32 fn_80079EF4(void) {
    /* TODO: decompile (0x6F4 bytes, ~445 instructions) */
    /* Uses floating point */
    fn_80102510();
    fn_800F0308();
    fn_800D37CC();
    fn_800D3088();
    fn_8010264C();
    fn_801067E8();
    fn_801069FC();
    fn_80106D3C();
    fn_8001E184();
    return 0;
}

/* 0x8007A5E8 | size: 0x7C */
s32 fn_8007A5E8(void) {
    /* TODO: decompile (0x7C bytes) */
    fn_8012A5B0();
    fn_80132A38();
    fn_800FA444();
    fn_800FB680();
    return 0;
}

/* 0x8007A664 | size: 0x8C */
void fn_8007A664(void) {
    /* TODO: decompile (0x8C bytes) */
    fn_80109220();
}

/* 0x8007A6F0 | size: 0x13C */
s32 fn_8007A6F0(void) {
    /* TODO: decompile (0x13C bytes) */
    fn_8012A5B0();
    fn_80109220();
    return 0;
}

/* 0x8007A82C | size: 0x24 */
s32 fn_8007A82C(void) {
    fn_801C40F0();
    return 0;
}

/* 0x8007A850 | size: 0x21C */
s32 fn_8007A850(void) {
    /* TODO: decompile (0x21C bytes, ~135 instructions) */
    /* Uses floating point */
    fn_801D036C();
    fn_801C40F0();
    fn_801067E8();
    fn_8010264C();
    fn_801069FC();
    fn_80102510();
    fn_800F0308();
    fn_800D37CC();
    fn_800D3088();
    fn_800798E8();
    fn_800792D8();
    fn_801D0314();
    return 0;
}

/* 0x8007AA6C | size: 0x3C */
void fn_8007AA6C(void) {
    /* TODO: decompile (0x3C bytes) */
    fn_80113F48();
    fn_80176E0C();
}

/* 0x8007AAA8 | size: 0x54 */
s32 fn_8007AAA8(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_800A1E54();
    fn_8007B0D8();
    return 0;
}

/* 0x8007AAFC | size: 0x14 */
void fn_8007AAFC(void) {
}

/* 0x8007AB10 | size: 0x580 */
s32 fn_8007AB10(void) {
    /* TODO: decompile (0x580 bytes, ~352 instructions) */
    /* Contains switch/jump table */
    fn_800D0F44();
    fn_8007B350();
    fn_800A1E54();
    return 0;
}

/* 0x8007B090 | size: 0x48 */
s32 fn_8007B090(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_800A7BCC();
    fn_8007B114();
    return 0;
}

/* 0x8007B0D8 | size: 0x3C */
void fn_8007B0D8(void) {
    /* TODO: decompile (0x3C bytes) */
    fn_8009AAD4();
}

/* 0x8007B114 | size: 0x23C */
s32 fn_8007B114(void) {
    /* TODO: decompile (0x23C bytes, ~143 instructions) */
    OSGetTick();
    fn_800CA968();
    fn_800A501C();
    fn_8009AAD4();
    fn_8009A9D8();
    fn_800A50E4();
    fn_800A541C();
    fn_800C8174();
    return 0;
}

/* 0x8007B350 | size: 0x354 */
void fn_8007B350(void) {
    /* TODO: decompile (0x354 bytes, ~213 instructions) */
    /* Uses many saved registers */
    fn_8009F1D0();
    fn_800A19CC();
    fn_800A1F94();
}

/* 0x8007B6A4 | size: 0x34 */
void fn_8007B6A4(void) {
    /* TODO: decompile (0x34 bytes) */
    fn_8007B6D8();
}

/* 0x8007B6D8 | size: 0xB64 */
s32 fn_8007B6D8(void) {
    /* TODO: decompile (0xB64 bytes, ~729 instructions) */
    /* Uses many saved registers */
    fn_8025F484();
    fn_8025F3F4();
    fn_8025F584();
    fn_8025F648();
    OSCreateAlarm();
    OSDisableInterrupts();
    OSSetAlarm();
    fn_800A221C();
    OSRestoreInterrupts();
    OSGetTick();
    return 0;
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
    /* TODO: decompile (0x54 bytes) */
    fn_8002DC6C();
    fn_800FF58C();
    return 0;
}

#pragma pop
