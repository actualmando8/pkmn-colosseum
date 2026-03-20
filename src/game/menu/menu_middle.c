/**
 * @file menu_middle.c
 * @brief Menu middle code between battle and common (0x80069C0C-0x8007109C)
 *
 * Address range: 0x80069C0C - 0x8007109C
 * Total functions: 100
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001DA60();
extern void fn_8005D830();
extern void fn_8005D858();
extern void fn_8005D8F8();
extern void fn_80071160();
extern void fn_80071208();
extern void fn_80071318();
extern void fn_8007162C();
extern void fn_80071644();
extern void fn_80076334();
extern void fn_80076398();
extern void fn_800767B8();
extern void fn_80076A8C();
extern void fn_80076F2C();
extern void fn_800772AC();
extern void fn_800774D4();
extern void fn_80077A5C();
extern void fn_80077BD0();
extern void fn_80077C1C();
extern void fn_80077C68();
extern void fn_80077D88();
extern void fn_80077DB8();
extern void fn_80077E50();
extern void fn_80088EA8();
extern void fn_800C8520();
extern void fn_800CE318();
extern void fn_800D5648();
extern void fn_800D5BA0();
extern void fn_800D61E4();
extern void fn_800D6728();
extern void fn_800D67BC();
extern void fn_800D6A00();
extern void fn_800D7820();
extern void fn_800D888C();
extern void fn_800D88DC();
extern void fn_800E0C54();
extern void fn_800F0308();
extern void fn_800F9D04();
extern void fn_800FA280();
extern void fn_800FA444();
extern void fn_800FB680();
extern void fn_800FE35C();
extern void fn_800FE38C();
extern void fn_800FF730();
extern void fn_80102138();
extern void fn_801022B8();
extern void fn_80102398();
extern void fn_80102868();
extern void fn_80102ED4();
extern void fn_80102F38();
extern void fn_801040D0();
extern void fn_801044D0();
extern void fn_801046B8();
extern void fn_801046C8();
extern void fn_80104704();
extern void fn_80104CA0();
extern void fn_80105624();
extern void fn_80107F38();
extern void fn_801081F8();
extern void fn_80108518();
/* ... and 50 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478938;
extern u8 lbl_80478F20;
extern u8 lbl_8047A5A4;
extern u8 lbl_8047A5D8;
extern u8 lbl_8047A5E0;
extern u8 lbl_8047A5E8;
extern u8 lbl_8047A5EC;
extern u8 lbl_8047A5F0;
extern u8 lbl_8047A5F4;
extern u8 lbl_8047A5F8;
extern u8 lbl_8047A5FC;
extern u8 lbl_8047C028;
extern u8 lbl_8047C030;
extern u8 lbl_8047C038;
extern u8 lbl_8047C040;
extern u8 lbl_8047C048;
extern u8 lbl_8047C050;
extern u8 lbl_8047C058;
extern u8 lbl_8047C060;
extern u8 lbl_8047C064;
extern u8 lbl_8047C068;
extern u8 lbl_8047C070;
extern u8 lbl_8047C078;
extern u8 lbl_8047C080;
extern u8 lbl_8047C088;
extern u8 lbl_8047C08C;
extern u8 lbl_8047E708;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802EDE78[];
extern u8 jumptable_802EDEFC[];
extern u8 jumptable_802EDF20[];
extern u8 jumptable_802EDFB0[];
extern u8 jumptable_802EDFCC[];
extern u8 jumptable_802EE06C[];
extern u8 jumptable_802EE0F0[];
extern u8 jumptable_802EE20C[];
extern u8 jumptable_802EE31C[];
extern u8 lbl_80267C18[];
extern u8 lbl_80267DD8[];
extern u8 lbl_80267DE8[];
extern u8 lbl_80267E70[];
extern u8 lbl_80267EA8[];
extern u8 lbl_80267F68[];
extern u8 lbl_80267FE8[];
extern u8 lbl_80268184[];
extern u8 lbl_802681B4[];
extern u8 lbl_80268234[];
extern u8 lbl_80268424[];
extern u8 lbl_80268560[];
extern u8 lbl_80268574[];
extern u8 lbl_8026858C[];
extern u8 lbl_8026860C[];
extern u8 lbl_8026864C[];
extern u8 lbl_80268674[];
extern u8 lbl_80268680[];
extern u8 lbl_802686D0[];
extern u8 lbl_802EDE58[];
extern u8 lbl_802EE618[];
extern u8 lbl_80314E08[];
extern u8 lbl_803B6D68[];

/* ===== Forward declarations ===== */
s32 fn_80069C0C(void);
s32 fn_8006A65C(void);
s32 fn_8006A718(void);
void fn_8006A76C(void);
void fn_8006A79C(void);
void fn_8006A7AC(void);
void fn_8006A7BC(void);
void fn_8006A7C8(void);
void fn_8006A7D0(void);
void fn_8006A7D8(void);
void fn_8006A7E0(void);
void fn_8006A7E8(void);
void fn_8006A7F0(void);
void fn_8006A814(void);
void fn_8006A81C(void);
void fn_8006A824(void);
void fn_8006A990(void);
void fn_8006AABC(void);
void fn_8006AC28(void);
s32 fn_8006AC6C(void);
s32 fn_8006ACCC(void);
s32 fn_8006ADB4(void);
s32 fn_8006ADEC(void);
s32 fn_8006AE18(void);
s32 fn_8006AEEC(void);
s32 fn_8006AF44(void);
s32 fn_8006AFC4(void);
s32 fn_8006AFE4(void);
s32 fn_8006B09C(void);
s32 fn_8006B0F8(void);
s32 fn_8006B154(void);
void fn_8006B1C0(void);
void fn_8006B1D4(void);
s32 fn_8006B1F4(void);
s32 fn_8006B2A4(void);
s32 fn_8006B354(void);
s32 fn_8006B3C8(void);
s32 fn_8006B420(void);
s32 fn_8006B4AC(void);
s32 fn_8006B51C(void);
s32 fn_8006B57C(void);
s32 fn_8006B5A8(void);
s32 fn_8006B5D0(void);
s32 fn_8006B6B4(void);
void fn_8006B8E8(void);
void fn_8006B8F0(void);
void fn_8006B8FC(void);
void fn_8006B908(void);
s32 fn_8006B930(void);
s32 fn_8006B9B8(void);
s32 fn_8006BB34(void);
void fn_8006C018(void);
void fn_8006C0DC(void);
s32 fn_8006C164(void);
s32 fn_8006C5D8(void);
s32 fn_8006C7D4(void);
s32 fn_8006CCC0(void);
s32 fn_8006D550(void);
void fn_8006D940(void);
void fn_8006D98C(void);
s32 fn_8006DAE4(void);
s32 fn_8006DC28(void);
void fn_8006E0CC(void);
s32 fn_8006E128(void);
void fn_8006E160(void);
void fn_8006E188(void);
void fn_8006E18C(void);
s32 fn_8006E258(void);
s32 fn_8006E338(void);
s32 fn_8006E798(void);
s32 fn_8006E9A4(void);
void fn_8006EE7C(void);
void fn_8006EF24(void);
s32 fn_8006EFF8(void);
s32 fn_8006F284(void);
s32 fn_8006F720(void);
void fn_8006FBFC(void);
void fn_8006FCF8(void);
void fn_8006FD24(void);
void fn_8006FD4C(void);
void fn_8006FD74(void);
void fn_8006FD9C(void);
void fn_8006FDC4(void);
void fn_8006FDEC(void);
void fn_8006FE14(void);
void fn_8006FE3C(void);
s32 fn_8006FE64(void);
s32 fn_8006FEE4(void);
void fn_80070274(void);
void fn_8007029C(void);
void fn_800702C8(void);
void fn_800702F0(void);
void fn_80070318(void);
s32 fn_80070428(void);
void fn_800704A4(void);
void fn_800704A8(void);
s32 fn_800704AC(void);
s32 fn_800706C4(void);
s32 fn_80070A9C(void);
s32 fn_80070D84(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80069C0C | size: 0xA50 */
s32 fn_80069C0C(void) {
    /* TODO: decompile (0xA50 bytes, ~660 instructions) */
    /* Uses many saved registers */
    fn_80196E10();
    fn_8020E0F8();
    fn_8020DF00();
    fn_8020DF90();
    fn_8020DFB0();
    fn_8020DEF0();
    fn_80077DB8();
    fn_8020DFA0();
    fn_800E0C54();
    fn_8006AABC();
    fn_8006A81C();
    fn_8012AC08();
    return 0;
}

/* 0x8006A65C | size: 0xBC */
s32 fn_8006A65C(void) {
    /* TODO: decompile (0xBC bytes) */
    fn_80129280();
    fn_801657D0();
    fn_80088EA8();
    fn_801906A0();
    fn_80069C0C();
    fn_800FF730();
    fn_800F0308();
    return 0;
}

/* 0x8006A718 | size: 0x54 */
s32 fn_8006A718(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006A76C | size: 0x30 */
void fn_8006A76C(void) {
    fn_801D04E8();
}

/* 0x8006A79C | size: 0x10 */
void fn_8006A79C(void) {
}

/* 0x8006A7AC | size: 0x10 */
void fn_8006A7AC(void) {
}

/* 0x8006A7BC | size: 0xC */
void fn_8006A7BC(void) {
}

/* 0x8006A7C8 | size: 0x8 */
void fn_8006A7C8(void) {
}

/* 0x8006A7D0 | size: 0x8 */
void fn_8006A7D0(void) {
}

/* 0x8006A7D8 | size: 0x8 */
void fn_8006A7D8(void) {
}

/* 0x8006A7E0 | size: 0x8 */
void fn_8006A7E0(void) {
}

/* 0x8006A7E8 | size: 0x8 */
void fn_8006A7E8(void) {
}

/* 0x8006A7F0 | size: 0x24 */
void fn_8006A7F0(void) {
    memcpy();
}

/* 0x8006A814 | size: 0x8 */
void fn_8006A814(void) {
}

/* 0x8006A81C | size: 0x8 */
void fn_8006A81C(void) {
}

/* 0x8006A824 | size: 0x16C */
void fn_8006A824(void) {
    /* TODO: decompile (0x16C bytes) */
    fn_8012A784();
    fn_8012AA2C();
    fn_80196E10();
    fn_8006A990();
}

/* 0x8006A990 | size: 0x12C */
void fn_8006A990(void) {
    /* TODO: decompile (0x12C bytes) */
    fn_8012AC64();
    fn_8012A7B4();
    fn_8012AC08();
    fn_80077A5C();
    fn_8012086C();
    memset();
}

/* 0x8006AABC | size: 0x16C */
void fn_8006AABC(void) {
    /* TODO: decompile (0x16C bytes) */
    fn_8012AC64();
    fn_801F9CBC();
    fn_8012A130();
    fn_80196E10();
    fn_8012A7B4();
    fn_8012AC08();
    fn_80077A5C();
    fn_8012086C();
}

/* 0x8006AC28 | size: 0x44 */
void fn_8006AC28(void) {
    /* TODO: decompile (0x44 bytes) */
    memset();
}

/* 0x8006AC6C | size: 0x60 */
s32 fn_8006AC6C(void) {
    /* TODO: decompile (0x60 bytes) */
    return -1;
}

/* 0x8006ACCC | size: 0xE8 */
s32 fn_8006ACCC(void) {
    /* TODO: decompile (0xE8 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006ADB4 | size: 0x38 */
s32 fn_8006ADB4(void) {
    /* TODO: decompile (0x38 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006ADEC | size: 0x2C */
s32 fn_8006ADEC(void) {
    fn_80129280();
    return 0;
}

/* 0x8006AE18 | size: 0xD4 */
s32 fn_8006AE18(void) {
    /* TODO: decompile (0xD4 bytes) */
    fn_801906A0();
    fn_80129280();
    fn_80196E10();
    return 0;
}

/* 0x8006AEEC | size: 0x58 */
s32 fn_8006AEEC(void) {
    /* TODO: decompile (0x58 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006AF44 | size: 0x80 */
s32 fn_8006AF44(void) {
    /* TODO: decompile (0x80 bytes) */
    memcpy();
    return 0;
}

/* 0x8006AFC4 | size: 0x20 */
s32 fn_8006AFC4(void) {
    return 0;
}

/* 0x8006AFE4 | size: 0xB8 */
s32 fn_8006AFE4(void) {
    /* TODO: decompile (0xB8 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B09C | size: 0x5C */
s32 fn_8006B09C(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B0F8 | size: 0x5C */
s32 fn_8006B0F8(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B154 | size: 0x6C */
s32 fn_8006B154(void) {
    /* TODO: decompile (0x6C bytes) */
    return -1;
}

/* 0x8006B1C0 | size: 0x14 */
void fn_8006B1C0(void) {
}

/* 0x8006B1D4 | size: 0x20 */
void fn_8006B1D4(void) {
    fn_80077DB8();
}

/* 0x8006B1F4 | size: 0xB0 */
s32 fn_8006B1F4(void) {
    /* TODO: decompile (0xB0 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B2A4 | size: 0xB0 */
s32 fn_8006B2A4(void) {
    /* TODO: decompile (0xB0 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B354 | size: 0x74 */
s32 fn_8006B354(void) {
    /* TODO: decompile (0x74 bytes) */
    fn_80196E10();
    fn_80129280();
    return 0;
}

/* 0x8006B3C8 | size: 0x58 */
s32 fn_8006B3C8(void) {
    /* TODO: decompile (0x58 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B420 | size: 0x8C */
s32 fn_8006B420(void) {
    /* TODO: decompile (0x8C bytes) */
    fn_80129280();
    fn_80077E50();
    return 0;
}

/* 0x8006B4AC | size: 0x70 */
s32 fn_8006B4AC(void) {
    /* TODO: decompile (0x70 bytes) */
    fn_80196E10();
    fn_80129280();
    return 0;
}

/* 0x8006B51C | size: 0x60 */
s32 fn_8006B51C(void) {
    /* TODO: decompile (0x60 bytes) */
    fn_80129280();
    return 0;
}

/* 0x8006B57C | size: 0x2C */
s32 fn_8006B57C(void) {
    fn_80129280();
    return 0;
}

/* 0x8006B5A8 | size: 0x28 */
s32 fn_8006B5A8(void) {
    fn_80129280();
    return 0;
}

/* 0x8006B5D0 | size: 0xE4 */
s32 fn_8006B5D0(void) {
    /* TODO: decompile (0xE4 bytes) */
    fn_80129280();
    fn_8006AABC();
    memcpy();
    fn_80071644();
    return 0;
}

/* 0x8006B6B4 | size: 0x234 */
s32 fn_8006B6B4(void) {
    /* TODO: decompile (0x234 bytes, ~141 instructions) */
    memset();
    fn_80077E50();
    return 0;
}

/* 0x8006B8E8 | size: 0x8 */
void fn_8006B8E8(void) {
}

/* 0x8006B8F0 | size: 0xC */
void fn_8006B8F0(void) {
}

/* 0x8006B8FC | size: 0xC */
void fn_8006B8FC(void) {
}

/* 0x8006B908 | size: 0x28 */
void fn_8006B908(void) {
    fn_80070D84();
}

/* 0x8006B930 | size: 0x88 */
s32 fn_8006B930(void) {
    /* TODO: decompile (0x88 bytes) */
    fn_80071160();
    fn_80129280();
    fn_80071208();
    return 0;
}

/* 0x8006B9B8 | size: 0x17C */
s32 fn_8006B9B8(void) {
    /* TODO: decompile (0x17C bytes) */
    fn_80129280();
    fn_80071160();
    fn_8006A814();
    fn_80071208();
    fn_80166A28();
    return 0;
}

/* 0x8006BB34 | size: 0x4E4 */
s32 fn_8006BB34(void) {
    /* TODO: decompile (0x4E4 bytes, ~313 instructions) */
    /* Uses many saved registers */
    fn_80105624();
    fn_801022B8();
    fn_80077BD0();
    fn_80102138();
    fn_801044D0();
    fn_801040D0();
    fn_80166A28();
    fn_80102398();
    fn_80196E10();
    fn_80102F38();
    return -1;
}

/* 0x8006C018 | size: 0xC4 */
void fn_8006C018(void) {
    /* TODO: decompile (0xC4 bytes) */
    fn_80105624();
    fn_801022B8();
    fn_80102ED4();
}

/* 0x8006C0DC | size: 0x88 */
void fn_8006C0DC(void) {
    /* TODO: decompile (0x88 bytes) */
    fn_80105624();
    fn_801022B8();
    fn_80102ED4();
}

/* 0x8006C164 | size: 0x474 */
s32 fn_8006C164(void) {
    /* TODO: decompile (0x474 bytes, ~285 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_80105624();
    fn_80077BD0();
    fn_80102138();
    fn_801044D0();
    fn_801040D0();
    fn_801022B8();
    fn_80166A28();
    fn_80102F38();
    return 0;
}

/* 0x8006C5D8 | size: 0x1FC */
s32 fn_8006C5D8(void) {
    /* TODO: decompile (0x1FC bytes) */
    fn_800FE38C();
    fn_800D88DC();
    fn_800D888C();
    fn_800D6A00();
    fn_800D7820();
    fn_800D67BC();
    fn_800D61E4();
    fn_800D5BA0();
    return 0;
}

/* 0x8006C7D4 | size: 0x4EC */
s32 fn_8006C7D4(void) {
    /* TODO: decompile (0x4EC bytes, ~315 instructions) */
    /* Contains switch/jump table */
    fn_80129280();
    fn_8012AC54();
    fn_80132A38();
    fn_800FB680();
    fn_8006A7E8();
    fn_8012AA2C();
    fn_8005D858();
    fn_80071318();
    fn_8012AC3C();
    fn_800C8520();
    fn_800F9D04();
    return 0;
}

/* 0x8006CCC0 | size: 0x890 */
s32 fn_8006CCC0(void) {
    /* TODO: decompile (0x890 bytes, ~548 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_801040D0();
    fn_80196E10();
    fn_801046C8();
    fn_800FA444();
    fn_8012AC54();
    fn_80132A38();
    fn_800FB680();
    fn_8012AC08();
    fn_80077A5C();
    fn_80123FBC();
    fn_8011E8DC();
    fn_8010B718();
    return 0;
}

/* 0x8006D550 | size: 0x3F0 */
s32 fn_8006D550(void) {
    /* TODO: decompile (0x3F0 bytes, ~252 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_801091F4();
    fn_80129280();
    fn_8006B154();
    fn_8012AC54();
    fn_80132A38();
    fn_800FB680();
    fn_8012AC3C();
    fn_800C8520();
    fn_800F9D04();
    fn_8006A7E8();
    fn_8012AA2C();
    fn_8005D858();
    return 0;
}

/* 0x8006D940 | size: 0x4C */
void fn_8006D940(void) {
    /* TODO: decompile (0x4C bytes) */
    fn_801040D0();
    fn_801046C8();
}

/* 0x8006D98C | size: 0x158 */
void fn_8006D98C(void) {
    /* TODO: decompile (0x158 bytes) */
    fn_801040D0();
    fn_801046C8();
    fn_801081F8();
    fn_80070D84();
}

/* 0x8006DAE4 | size: 0x144 */
s32 fn_8006DAE4(void) {
    /* TODO: decompile (0x144 bytes) */
    fn_801040D0();
    fn_80102868();
    fn_801046C8();
    fn_800FA444();
    fn_80109220();
    fn_80070D84();
    return 0;
}

/* 0x8006DC28 | size: 0x4A4 */
s32 fn_8006DC28(void) {
    /* TODO: decompile (0x4A4 bytes, ~297 instructions) */
    /* Uses many saved registers */
    fn_801040D0();
    fn_8012AC08();
    fn_80123FBC();
    fn_8010B01C();
    fn_80076398();
    fn_80076334();
    fn_800772AC();
    fn_80076A8C();
    fn_801046C8();
    fn_8005D858();
    fn_80071318();
    fn_8011FC74();
    return 0;
}

/* 0x8006E0CC | size: 0x5C */
void fn_8006E0CC(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_8010BBB8();
    fn_8010BCE4();
    fn_800F0308();
}

/* 0x8006E128 | size: 0x38 */
s32 fn_8006E128(void) {
    /* TODO: decompile (0x38 bytes) */
    return 0;
}

/* 0x8006E160 | size: 0x28 */
void fn_8006E160(void) {
    fn_80070D84();
}

/* 0x8006E188 | size: 0x4 */
void fn_8006E188(void) {
}

/* 0x8006E18C | size: 0xCC */
void fn_8006E18C(void) {
    /* TODO: decompile (0xCC bytes) */
    fn_801046C8();
    fn_80109220();
    fn_80071160();
    fn_80107F38();
    fn_80070D84();
}

/* 0x8006E258 | size: 0xE0 */
s32 fn_8006E258(void) {
    /* TODO: decompile (0xE0 bytes) */
    fn_80129280();
    fn_8006B154();
    fn_801046C8();
    fn_80109220();
    return 0;
}

/* 0x8006E338 | size: 0x460 */
s32 fn_8006E338(void) {
    /* TODO: decompile (0x460 bytes, ~280 instructions) */
    /* Uses many saved registers */
    fn_801046C8();
    fn_80108518();
    fn_80129280();
    fn_8006B154();
    fn_8006A7E8();
    fn_8005D858();
    fn_80071318();
    fn_80196E10();
    fn_80109220();
    fn_80070D84();
    return 0;
}

/* 0x8006E798 | size: 0x20C */
s32 fn_8006E798(void) {
    /* TODO: decompile (0x20C bytes, ~131 instructions) */
    /* Uses many saved registers */
    fn_801046C8();
    fn_80108518();
    fn_80129280();
    fn_8006A7E8();
    fn_80109220();
    fn_80070D84();
    return 0;
}

/* 0x8006E9A4 | size: 0x4D8 */
s32 fn_8006E9A4(void) {
    /* TODO: decompile (0x4D8 bytes, ~310 instructions) */
    fn_801091F4();
    fn_801040D0();
    fn_800C8520();
    fn_800F9D04();
    fn_80132A38();
    fn_800FB680();
    fn_8006B3C8();
    return 0;
}

/* 0x8006EE7C | size: 0xA8 */
void fn_8006EE7C(void) {
    /* TODO: decompile (0xA8 bytes) */
    fn_80105624();
    fn_801040D0();
    fn_80102ED4();
}

/* 0x8006EF24 | size: 0xD4 */
void fn_8006EF24(void) {
    /* TODO: decompile (0xD4 bytes) */
    fn_80105624();
    fn_80102F38();
}

/* 0x8006EFF8 | size: 0x28C */
s32 fn_8006EFF8(void) {
    /* TODO: decompile (0x28C bytes, ~163 instructions) */
    /* Uses many saved registers */
    fn_801040D0();
    fn_801046C8();
    fn_80109220();
    fn_80077D88();
    fn_80142984();
    fn_801440A0();
    fn_80144088();
    fn_801081F8();
    return 0;
}

/* 0x8006F284 | size: 0x49C */
s32 fn_8006F284(void) {
    /* TODO: decompile (0x49C bytes, ~295 instructions) */
    /* Uses many saved registers */
    fn_801046C8();
    fn_80077BD0();
    fn_8005D8F8();
    fn_80102138();
    fn_801044D0();
    fn_80104CA0();
    fn_801022B8();
    fn_80109220();
    fn_8005D830();
    fn_801081F8();
    fn_801040D0();
    fn_80070D84();
    return 0;
}

/* 0x8006F720 | size: 0x4DC */
s32 fn_8006F720(void) {
    /* TODO: decompile (0x4DC bytes, ~311 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_801040D0();
    fn_801046C8();
    fn_801022B8();
    fn_80077BD0();
    fn_80109220();
    fn_8005D830();
    fn_801081F8();
    fn_80129280();
    fn_80105624();
    fn_80102138();
    fn_801044D0();
    fn_80070D84();
    return 0;
}

/* 0x8006FBFC | size: 0xFC */
void fn_8006FBFC(void) {
    /* TODO: decompile (0xFC bytes) */
    fn_801046C8();
    fn_80105624();
    fn_80070D84();
}

/* 0x8006FCF8 | size: 0x2C */
void fn_8006FCF8(void) {
    fn_80070D84();
}

/* 0x8006FD24 | size: 0x28 */
void fn_8006FD24(void) {
    fn_80070D84();
}

/* 0x8006FD4C | size: 0x28 */
void fn_8006FD4C(void) {
    fn_80070D84();
}

/* 0x8006FD74 | size: 0x28 */
void fn_8006FD74(void) {
    fn_80070D84();
}

/* 0x8006FD9C | size: 0x28 */
void fn_8006FD9C(void) {
    fn_80070D84();
}

/* 0x8006FDC4 | size: 0x28 */
void fn_8006FDC4(void) {
    fn_80070D84();
}

/* 0x8006FDEC | size: 0x28 */
void fn_8006FDEC(void) {
    fn_80070D84();
}

/* 0x8006FE14 | size: 0x28 */
void fn_8006FE14(void) {
    fn_80070D84();
}

/* 0x8006FE3C | size: 0x28 */
void fn_8006FE3C(void) {
    fn_80070D84();
}

/* 0x8006FE64 | size: 0x80 */
s32 fn_8006FE64(void) {
    /* TODO: decompile (0x80 bytes) */
    fn_80105624();
    fn_8006B3C8();
    fn_80166A28();
    fn_80102ED4();
    return 0;
}

/* 0x8006FEE4 | size: 0x390 */
s32 fn_8006FEE4(void) {
    /* TODO: decompile (0x390 bytes, ~228 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8006B3C8();
    fn_801046C8();
    fn_8005D858();
    fn_80071318();
    fn_80109220();
    fn_80108518();
    fn_80070D84();
    fn_8006B1F4();
    fn_800CE318();
    return 0;
}

/* 0x80070274 | size: 0x28 */
void fn_80070274(void) {
    fn_80070D84();
}

/* 0x8007029C | size: 0x2C */
void fn_8007029C(void) {
    fn_80070D84();
}

/* 0x800702C8 | size: 0x28 */
void fn_800702C8(void) {
    fn_80070D84();
}

/* 0x800702F0 | size: 0x28 */
void fn_800702F0(void) {
    fn_80070D84();
}

/* 0x80070318 | size: 0x110 */
void fn_80070318(void) {
    /* TODO: decompile (0x110 bytes) */
    fn_801081F8();
}

/* 0x80070428 | size: 0x7C */
s32 fn_80070428(void) {
    /* TODO: decompile (0x7C bytes) */
    fn_800FA280();
    fn_80132A38();
    fn_800FB680();
    return 0;
}

/* 0x800704A4 | size: 0x4 */
void fn_800704A4(void) {
}

/* 0x800704A8 | size: 0x4 */
void fn_800704A8(void) {
}

/* 0x800704AC | size: 0x218 */
s32 fn_800704AC(void) {
    /* TODO: decompile (0x218 bytes, ~134 instructions) */
    /* Contains switch/jump table */
    fn_8007162C();
    fn_8006B3C8();
    fn_80104704();
    fn_801046B8();
    fn_80109220();
    return 0;
}

/* 0x800706C4 | size: 0x3D8 */
s32 fn_800706C4(void) {
    /* TODO: decompile (0x3D8 bytes, ~246 instructions) */
    /* Contains switch/jump table */
    fn_801040D0();
    fn_80129280();
    fn_8006AFC4();
    fn_8006B420();
    fn_800767B8();
    fn_800FA444();
    fn_80109220();
    fn_801046C8();
    return 0;
}

/* 0x80070A9C | size: 0x2E8 */
s32 fn_80070A9C(void) {
    /* TODO: decompile (0x2E8 bytes, ~186 instructions) */
    /* Contains switch/jump table */
    fn_801040D0();
    fn_80129280();
    fn_80109220();
    return 0;
}

/* 0x80070D84 | size: 0x318 */
s32 fn_80070D84(void) {
    /* TODO: decompile (0x318 bytes, ~198 instructions) */
    /* Uses many saved registers */
    fn_801081F8();
    fn_80108518();
    return 0;
}

#pragma pop
