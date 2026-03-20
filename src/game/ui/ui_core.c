/**
 * @file ui_core.c
 * @brief UI Core state machine and dispatch (0x80059BDC-0x80069A60)
 *
 * Address range: 0x80059BDC - 0x80069A60
 * Total functions: 101
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8001DA60();
extern void fn_8001E074();
extern void fn_8001E58C();
extern void fn_8002D91C();
extern void fn_800347B8();
extern void fn_800347C4();
extern void fn_800347E8();
extern void fn_8003480C();
extern void fn_80069C0C();
extern void fn_8006A76C();
extern void fn_8006A79C();
extern void fn_8006A7AC();
extern void fn_8006A7BC();
extern void fn_8006A7C8();
extern void fn_8006A7D0();
extern void fn_8006A7E0();
extern void fn_8006A7E8();
extern void fn_8006A7F0();
extern void fn_8006A814();
extern void fn_8006A81C();
extern void fn_8006A824();
extern void fn_8006AC28();
extern void fn_8006AC6C();
extern void fn_8006ACCC();
extern void fn_8006ADB4();
extern void fn_8006ADEC();
extern void fn_8006AF44();
extern void fn_8006AFC4();
extern void fn_8006AFE4();
extern void fn_8006B09C();
extern void fn_8006B0F8();
extern void fn_8006B1D4();
extern void fn_8006B1F4();
extern void fn_8006B2A4();
extern void fn_8006B354();
extern void fn_8006B3C8();
extern void fn_8006B420();
extern void fn_8006B4AC();
extern void fn_8006B51C();
extern void fn_8006B5D0();
extern void fn_8006B8E8();
extern void fn_8006B8F0();
extern void fn_8006B8FC();
extern void fn_8006E0CC();
extern void fn_80071160();
extern void fn_80071344();
extern void fn_80071398();
extern void fn_800714C8();
extern void fn_800715BC();
extern void fn_8007162C();
extern void fn_80076054();
extern void fn_800776E4();
extern void fn_80077E80();
extern void fn_80077EA4();
extern void fn_800849B4();
extern void fn_800886D0();
extern void fn_80088964();
extern void fn_800889A4();
extern void fn_800889E4();
extern void fn_80088C60();
/* ... and 173 more external functions */
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);

/* ===== SDA globals ===== */
extern u8 lbl_80478848;
extern u8 lbl_80478900;
extern u8 lbl_80478908;
extern u8 lbl_80478910;
extern u8 lbl_80478918;
extern u8 lbl_8047891A;
extern u8 lbl_8047891C;
extern u8 lbl_80478920;
extern u8 lbl_80478922;
extern u8 lbl_80478968;
extern u8 lbl_80478BD8;
extern u8 lbl_80478E90;
extern u8 lbl_80478E94;
extern u8 lbl_8047A5A0;
extern u8 lbl_8047A5A8;
extern u8 lbl_8047A5B0;
extern u8 lbl_8047A5B4;
extern u8 lbl_8047A5B8;
extern u8 lbl_8047A5BC;
extern u8 lbl_8047A5C0;
extern u8 lbl_8047A5C4;
extern u8 lbl_8047A5C8;
extern u8 lbl_8047A5D0;
extern u8 lbl_8047BF18;
extern u8 lbl_8047BF1C;
extern u8 lbl_8047BF20;
extern u8 lbl_8047BF24;
extern u8 lbl_8047BF28;
extern u8 lbl_8047BF30;
extern u8 lbl_8047BF34;
extern u8 lbl_8047BF38;
extern u8 lbl_8047BF3C;
extern u8 lbl_8047BF40;
extern u8 lbl_8047BF48;
extern u8 lbl_8047BF50;
extern u8 lbl_8047BF54;
extern u8 lbl_8047BF58;
extern u8 lbl_8047BF5C;
extern u8 lbl_8047BF60;
extern u8 lbl_8047BF64;
extern u8 lbl_8047BF68;
extern u8 lbl_8047BF6C;
extern u8 lbl_8047BF70;
extern u8 lbl_8047BF74;
extern u8 lbl_8047BF78;
extern u8 lbl_8047BF7C;
extern u8 lbl_8047BF80;
extern u8 lbl_8047BF88;
extern u8 lbl_8047BF90;
extern u8 lbl_8047BF94;
extern u8 lbl_8047BF98;
extern u8 lbl_8047BF9C;
extern u8 lbl_8047BFA0;
extern u8 lbl_8047BFA4;
extern u8 lbl_8047BFA8;
extern u8 lbl_8047BFAC;
extern u8 lbl_8047BFB0;
extern u8 lbl_8047BFB4;
extern u8 lbl_8047BFB8;
extern u8 lbl_8047BFBC;
extern u8 lbl_8047BFC0;
extern u8 lbl_8047BFC4;
extern u8 lbl_8047BFC8;
extern u8 lbl_8047BFCC;
extern u8 lbl_8047BFD0;
extern u8 lbl_8047BFD4;
extern u8 lbl_8047BFD8;
extern u8 lbl_8047BFE0;
extern u8 lbl_8047BFE8;
extern u8 lbl_8047BFEC;
extern u8 lbl_8047BFF0;
extern u8 lbl_8047BFF8;
extern u8 lbl_8047BFFC;
extern u8 lbl_8047C000;
extern u8 lbl_8047C004;
extern u8 lbl_8047C008;
extern u8 lbl_8047C00C;
extern u8 lbl_8047C010;
extern u8 lbl_8047C014;
extern u8 lbl_8047C018;
extern u8 lbl_8047C020;

/* ===== Rodata / data labels ===== */
extern u8 jumptable_802E62B0[];
extern u8 jumptable_802ED9B8[];
extern u8 jumptable_802EDB7C[];
extern u8 lbl_80267840[];
extern u8 lbl_802678D8[];
extern u8 lbl_80267A20[];
extern u8 lbl_80267A80[];
extern u8 lbl_80267A98[];
extern u8 lbl_80267AB0[];
extern u8 lbl_80267AF8[];
extern u8 lbl_80267B88[];
extern u8 lbl_802E2DB8[];
extern u8 lbl_802E6428[];
extern u8 lbl_802E7CE8[];
extern u8 lbl_802ED958[];
extern u8 lbl_802ED978[];
extern u8 lbl_802ED9A0[];
extern u8 lbl_802ED9F0[];
extern u8 lbl_802ED9FC[];
extern u8 lbl_802EDA20[];
extern u8 lbl_802EDB40[];
extern u8 lbl_802EDB64[];
extern u8 lbl_802EF0A8[];
extern u8 lbl_80314E08[];
extern u8 lbl_80314F98[];
extern u8 lbl_803A9A60[];
extern u8 lbl_803A9E40[];
extern u8 lbl_803A9EA0[];
extern u8 lbl_803A9F08[];

/* ===== Forward declarations ===== */
s32 fn_80059BDC(void);
s32 fn_8005CCD0(void);
s32 fn_8005CD88(void);
s32 fn_8005CEE8(void);
s32 fn_8005CF2C(void);
s32 fn_8005D094(void);
s32 fn_8005D0B8(void);
s32 fn_8005D0DC(void);
s32 fn_8005D130(void);
s32 fn_8005D184(void);
s32 fn_8005D26C(void);
s32 fn_8005D2E8(void);
s32 fn_8005D3D0(void);
s32 fn_8005D4AC(void);
s32 fn_8005D4F4(void);
s32 fn_8005D53C(void);
s32 fn_8005D584(void);
s32 fn_8005D5CC(void);
s32 fn_8005D6A8(void);
s32 fn_8005D738(void);
void fn_8005D798(void);
s32 fn_8005D7F8(void);
s32 fn_8005D830(void);
s32 fn_8005D858(void);
s32 fn_8005D880(void);
s32 fn_8005D8B8(void);
s32 fn_8005D8F8(void);
s32 fn_8005D934(void);
s32 fn_8005D95C(void);
s32 fn_8005D9AC(void);
s32 fn_8005D9E4(void);
s32 fn_8005DA18(void);
s32 fn_8005DA48(void);
s32 fn_8005DBC4(void);
s32 fn_8005DC24(void);
s32 fn_8005DCC4(void);
s32 fn_8005DEE8(void);
s32 fn_8005DFC8(void);
void fn_8005E690(void);
void fn_8005E730(void);
s32 fn_8005E750(void);
void fn_8005E7F0(void);
s32 fn_8005FFE4(void);
s32 fn_80060434(void);
s32 fn_800608C4(void);
void fn_800609B4(void);
void fn_80060A28(void);
s32 fn_80060D70(void);
void fn_80060EF4(void);
void fn_80061018(void);
s32 fn_80061028(void);
s32 fn_8006106C(void);
s32 fn_80061240(void);
s32 fn_80061454(void);
s32 fn_800615F4(void);
s32 fn_800617E0(void);
s32 fn_80061A2C(void);
void fn_80061B74(void);
s32 fn_80061BBC(void);
void fn_80061D34(void);
s32 fn_80061F6C(void);
s32 fn_80062284(void);
void fn_80062334(void);
s32 fn_800626CC(void);
void fn_80062834(void);
s32 fn_80062948(void);
s32 fn_80062AB4(void);
s32 fn_80063060(void);
s32 fn_800637B0(void);
s32 fn_800638F4(void);
s32 fn_80063AD4(void);
void fn_80063D10(void);
s32 fn_80063D14(void);
s32 fn_80064378(void);
s32 fn_800643D4(void);
void fn_80065628(void);
void fn_80065730(void);
void fn_80065838(void);
void fn_80065940(void);
s32 fn_80065A48(void);
s32 fn_800676EC(void);
s32 fn_800679C0(void);
s32 fn_800681B4(void);
s32 fn_80068418(void);
s32 fn_80068738(void);
s32 fn_80068794(void);
s32 fn_800688C4(void);
void fn_800689FC(void);
s32 fn_80068BB0(void);
s32 fn_80068DBC(void);
void fn_80068F84(void);
void fn_80069048(void);
s32 fn_8006905C(void);
s32 fn_80069220(void);
s32 fn_800693A4(void);
s32 fn_80069504(void);
s32 fn_80069664(void);
s32 fn_800697C4(void);
void fn_800697F4(void);
void fn_80069944(void);
s32 fn_80069A08(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x80059BDC | size: 0x30F4 */
s32 fn_80059BDC(void) {
    /* TODO: decompile (0x30F4 bytes, ~3133 instructions) */
    /* Contains switch/jump table */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_80129280();
    fn_8007162C();
    fn_800FF540();
    fn_80196E10();
    fn_801906A0();
    fn_801026A4();
    fn_80071344();
    fn_8006A7E0();
    fn_8006B4AC();
    fn_8006AFC4();
    fn_80106D3C();
    fn_801069FC();
    return 0;
}

/* 0x8005CCD0 | size: 0xB8 */
s32 fn_8005CCD0(void) {
    /* TODO: decompile (0xB8 bytes) */
    fn_801C40F0();
    fn_8006B8E8();
    fn_80129280();
    fn_8019075C();
    fn_80102510();
    fn_8025CD64();
    fn_801CB9D8();
    fn_800E202C();
    return 0;
}

/* 0x8005CD88 | size: 0x160 */
s32 fn_8005CD88(void) {
    /* TODO: decompile (0x160 bytes) */
    fn_80165A20();
    fn_8019075C();
    fn_80196E10();
    fn_800E2C04();
    fn_800E27B0();
    fn_80113F48();
    fn_801CBA0C();
    fn_800F9318();
    return 0;
}

/* 0x8005CEE8 | size: 0x44 */
s32 fn_8005CEE8(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_8006B8F0();
    fn_80129280();
    fn_800FF58C();
    return 0;
}

/* 0x8005CF2C | size: 0x168 */
s32 fn_8005CF2C(void) {
    /* TODO: decompile (0x168 bytes) */
    fn_801026A4();
    fn_8006B420();
    fn_80102868();
    fn_80076054();
    fn_80196E10();
    fn_80166A28();
    fn_80106D3C();
    fn_80102568();
    return 0;
}

/* 0x8005D094 | size: 0x24 */
s32 fn_8005D094(void) {
    fn_800347B8();
    return -1;
}

/* 0x8005D0B8 | size: 0x24 */
s32 fn_8005D0B8(void) {
    fn_800347C4();
    return 0;
}

/* 0x8005D0DC | size: 0x54 */
s32 fn_8005D0DC(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_8010264C();
    fn_80102510();
    fn_800347E8();
    return 0;
}

/* 0x8005D130 | size: 0x54 */
s32 fn_8005D130(void) {
    /* TODO: decompile (0x54 bytes) */
    fn_8010264C();
    fn_80102510();
    fn_8003480C();
    return 0;
}

/* 0x8005D184 | size: 0xE8 */
s32 fn_8005D184(void) {
    /* TODO: decompile (0xE8 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D26C | size: 0x7C */
s32 fn_8005D26C(void) {
    /* TODO: decompile (0x7C bytes) */
    fn_8010264C();
    fn_80102510();
    fn_80102428();
    return 0;
}

/* 0x8005D2E8 | size: 0xE8 */
s32 fn_8005D2E8(void) {
    /* TODO: decompile (0xE8 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D3D0 | size: 0xDC */
s32 fn_8005D3D0(void) {
    /* TODO: decompile (0xDC bytes) */
    fn_801046B8();
    fn_801026A4();
    fn_80102510();
    fn_80102428();
    return 0;
}

/* 0x8005D4AC | size: 0x48 */
s32 fn_8005D4AC(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D4F4 | size: 0x48 */
s32 fn_8005D4F4(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D53C | size: 0x48 */
s32 fn_8005D53C(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D584 | size: 0x48 */
s32 fn_8005D584(void) {
    /* TODO: decompile (0x48 bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D5CC | size: 0xDC */
s32 fn_8005D5CC(void) {
    /* TODO: decompile (0xDC bytes) */
    fn_80109220();
    return 0;
}

/* 0x8005D6A8 | size: 0x90 */
s32 fn_8005D6A8(void) {
    /* TODO: decompile (0x90 bytes) */
    fn_80105624();
    fn_80166A50();
    fn_80102ED4();
    return 0;
}

/* 0x8005D738 | size: 0x60 */
s32 fn_8005D738(void) {
    /* TODO: decompile (0x60 bytes) */
    fn_8010264C();
    fn_80102510();
    fn_80102428();
    return 0;
}

/* 0x8005D798 | size: 0x60 */
void fn_8005D798(void) {
    /* TODO: decompile (0x60 bytes) */
}

/* 0x8005D7F8 | size: 0x38 */
s32 fn_8005D7F8(void) {
    /* TODO: decompile (0x38 bytes) */
    return 0;
}

/* 0x8005D830 | size: 0x28 */
s32 fn_8005D830(void) {
    return 0;
}

/* 0x8005D858 | size: 0x28 */
s32 fn_8005D858(void) {
    return 0;
}

/* 0x8005D880 | size: 0x38 */
s32 fn_8005D880(void) {
    /* TODO: decompile (0x38 bytes) */
    return 0;
}

/* 0x8005D8B8 | size: 0x40 */
s32 fn_8005D8B8(void) {
    /* TODO: decompile (0x40 bytes) */
    return 0;
}

/* 0x8005D8F8 | size: 0x3C */
s32 fn_8005D8F8(void) {
    /* TODO: decompile (0x3C bytes) */
    return 0;
}

/* 0x8005D934 | size: 0x28 */
s32 fn_8005D934(void) {
    return 0;
}

/* 0x8005D95C | size: 0x50 */
s32 fn_8005D95C(void) {
    /* TODO: decompile (0x50 bytes) */
    return 0;
}

/* 0x8005D9AC | size: 0x38 */
s32 fn_8005D9AC(void) {
    /* TODO: decompile (0x38 bytes) */
    return 0;
}

/* 0x8005D9E4 | size: 0x34 */
s32 fn_8005D9E4(void) {
    /* TODO: decompile (0x34 bytes) */
    return 0;
}

/* 0x8005DA18 | size: 0x30 */
s32 fn_8005DA18(void) {
    return 0;
}

/* 0x8005DA48 | size: 0x17C */
s32 fn_8005DA48(void) {
    /* TODO: decompile (0x17C bytes) */
    fn_80105624();
    fn_801040D0();
    fn_801040B8();
    fn_800DA1E8();
    fn_8001E58C();
    fn_800DD384();
    fn_800DD270();
    fn_800FAEF8();
    return 0;
}

/* 0x8005DBC4 | size: 0x60 */
s32 fn_8005DBC4(void) {
    /* TODO: decompile (0x60 bytes) */
    fn_80102620();
    fn_80102510();
    fn_801026A4();
    return 0;
}

/* 0x8005DC24 | size: 0xA0 */
s32 fn_8005DC24(void) {
    /* TODO: decompile (0xA0 bytes) */
    fn_80102620();
    fn_800FF56C();
    fn_80117AD4();
    fn_801176C8();
    fn_801026A4();
    fn_80102868();
    return 0;
}

/* 0x8005DCC4 | size: 0x224 */
s32 fn_8005DCC4(void) {
    /* TODO: decompile (0x224 bytes, ~137 instructions) */
    fn_80105624();
    return 0;
}

/* 0x8005DEE8 | size: 0xE0 */
s32 fn_8005DEE8(void) {
    /* TODO: decompile (0xE0 bytes) */
    fn_8010264C();
    fn_80142984();
    fn_80102568();
    fn_80129A78();
    fn_80102510();
    fn_80102428();
    return 0;
}

/* 0x8005DFC8 | size: 0x6C8 */
s32 fn_8005DFC8(void) {
    /* TODO: decompile (0x6C8 bytes, ~434 instructions) */
    /* Uses floating point */
    fn_800D37CC();
    fn_800D3088();
    fn_80069048();
    fn_8025DA88();
    fn_8017B000();
    fn_80060A28();
    fn_801666BC();
    fn_801080CC();
    return 0;
}

/* 0x8005E690 | size: 0xA0 */
void fn_8005E690(void) {
    /* TODO: decompile (0xA0 bytes) */
    fn_80102ED4();
}

/* 0x8005E730 | size: 0x20 */
void fn_8005E730(void) {
    fn_8005DFC8();
}

/* 0x8005E750 | size: 0xA0 */
s32 fn_8005E750(void) {
    /* TODO: decompile (0xA0 bytes) */
    fn_80061F6C();
    fn_80103CC0();
    fn_8010264C();
    fn_80102568();
    return 0;
}

/* 0x8005E7F0 | size: 0x17F4 */
void fn_8005E7F0(void) {
    /* TODO: decompile (0x17F4 bytes, ~1533 instructions) */
    fn_800608C4();
    fn_8006106C();
    fn_80061A2C();
    fn_80061BBC();
    fn_80061B74();
    fn_80060D70();
    fn_80060EF4();
    fn_800617E0();
    fn_800615F4();
    fn_80061454();
    fn_800609B4();
    fn_80060434();
}

/* 0x8005FFE4 | size: 0x450 */
s32 fn_8005FFE4(void) {
    /* TODO: decompile (0x450 bytes, ~276 instructions) */
    fn_8025DA88();
    fn_8025D89C();
    fn_8025D808();
    return 0;
}

/* 0x80060434 | size: 0x490 */
s32 fn_80060434(void) {
    /* TODO: decompile (0x490 bytes, ~292 instructions) */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_8025D9A8();
    fn_8025DAD0();
    fn_80132A38();
    fn_800FA444();
    fn_800FB680();
    return 0;
}

/* 0x800608C4 | size: 0xF0 */
s32 fn_800608C4(void) {
    /* TODO: decompile (0xF0 bytes) */
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

/* 0x800609B4 | size: 0x74 */
void fn_800609B4(void) {
    /* TODO: decompile (0x74 bytes) */
    fn_800FE6D0();
    fn_800FE4D4();
}

/* 0x80060A28 | size: 0x348 */
void fn_80060A28(void) {
    /* TODO: decompile (0x348 bytes, ~210 instructions) */
}

/* 0x80060D70 | size: 0x184 */
s32 fn_80060D70(void) {
    /* TODO: decompile (0x184 bytes) */
    fn_801EF634();
    return 0;
}

/* 0x80060EF4 | size: 0x124 */
void fn_80060EF4(void) {
    /* TODO: decompile (0x124 bytes) */
    fn_8025D9A8();
}

/* 0x80061018 | size: 0x10 */
void fn_80061018(void) {
}

/* 0x80061028 | size: 0x44 */
s32 fn_80061028(void) {
    /* TODO: decompile (0x44 bytes) */
    fn_80102568();
    return 0;
}

/* 0x8006106C | size: 0x1D4 */
s32 fn_8006106C(void) {
    /* TODO: decompile (0x1D4 bytes) */
    fn_80061D34();
    fn_800FE6D0();
    fn_800FE4D4();
    fn_80069A08();
    fn_801040F0();
    return 0;
}

/* 0x80061240 | size: 0x214 */
s32 fn_80061240(void) {
    /* TODO: decompile (0x214 bytes, ~133 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_800D88DC();
    fn_800D888C();
    fn_800D7820();
    fn_800D6A00();
    fn_800D67BC();
    fn_800D61E4();
    fn_800D5BA0();
    fn_800D6728();
    return 0;
}

/* 0x80061454 | size: 0x1A0 */
s32 fn_80061454(void) {
    /* TODO: decompile (0x1A0 bytes) */
    fn_8025DA88();
    fn_800FE6D0();
    fn_800FE4D4();
    fn_800D88DC();
    fn_800D888C();
    fn_800D6A00();
    fn_800D7820();
    fn_800D85D4();
    return 0;
}

/* 0x800615F4 | size: 0x1EC */
s32 fn_800615F4(void) {
    /* TODO: decompile (0x1EC bytes) */
    fn_8025D9CC();
    fn_8025DA88();
    fn_800FE6D0();
    fn_800FE4D4();
    fn_8025DA18();
    fn_80132A38();
    fn_800FBB34();
    fn_800FB680();
    return 0;
}

/* 0x800617E0 | size: 0x24C */
s32 fn_800617E0(void) {
    /* TODO: decompile (0x24C bytes, ~147 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_800FE6D0();
    fn_800FE4D4();
    fn_8025D914();
    fn_8012AC54();
    fn_800FA280();
    fn_8025D9CC();
    fn_80132A38();
    fn_800FBB34();
    fn_800FB680();
    fn_8025DA18();
    return 0;
}

/* 0x80061A2C | size: 0x148 */
s32 fn_80061A2C(void) {
    /* TODO: decompile (0x148 bytes) */
    fn_80061D34();
    fn_800FE6D0();
    fn_800FE4D4();
    fn_801040F0();
    fn_80061240();
    return 0;
}

/* 0x80061B74 | size: 0x48 */
void fn_80061B74(void) {
    /* TODO: decompile (0x48 bytes) */
}

/* 0x80061BBC | size: 0x178 */
s32 fn_80061BBC(void) {
    /* TODO: decompile (0x178 bytes) */
    fn_800FE6D0();
    fn_800FE4D4();
    fn_80061D34();
    return 0;
}

/* 0x80061D34 | size: 0x238 */
void fn_80061D34(void) {
    /* TODO: decompile (0x238 bytes, ~142 instructions) */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_8025D89C();
    fn_8025D808();
}

/* 0x80061F6C | size: 0x318 */
s32 fn_80061F6C(void) {
    /* TODO: decompile (0x318 bytes, ~198 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_8025D914();
    fn_8012AC54();
    fn_800F9E70();
    fn_800FA280();
    fn_8025D28C();
    fn_801FCCC4();
    fn_801FCC64();
    fn_801FBD58();
    fn_801FBD28();
    fn_8025DBB0();
    fn_8005FFE4();
    return 0;
}

/* 0x80062284 | size: 0xB0 */
s32 fn_80062284(void) {
    /* TODO: decompile (0xB0 bytes) */
    fn_8025D808();
    fn_8025D938();
    fn_80123FBC();
    fn_8012640C();
    return 0;
}

/* 0x80062334 | size: 0x398 */
void fn_80062334(void) {
    /* TODO: decompile (0x398 bytes, ~230 instructions) */
    /* Uses many saved registers */
    fn_8025DA88();
}

/* 0x800626CC | size: 0x168 */
s32 fn_800626CC(void) {
    /* TODO: decompile (0x168 bytes) */
    fn_800F92D4();
    fn_8025DA88();
    fn_8025D364();
    fn_8025D2D4();
    fn_8017B000();
    return 0;
}

/* 0x80062834 | size: 0x114 */
void fn_80062834(void) {
    /* TODO: decompile (0x114 bytes) */
    fn_8025DA88();
    fn_8017B1CC();
    fn_800F915C();
    fn_800F9210();
}

/* 0x80062948 | size: 0x16C */
s32 fn_80062948(void) {
    /* TODO: decompile (0x16C bytes) */
    fn_80061F6C();
    fn_8025D9A8();
    fn_8025DA88();
    fn_80063060();
    fn_8010264C();
    fn_8025D788();
    fn_80102568();
    fn_80062AB4();
    return 0;
}

/* 0x80062AB4 | size: 0x5AC */
s32 fn_80062AB4(void) {
    /* TODO: decompile (0x5AC bytes, ~363 instructions) */
    /* Uses many saved registers */
    fn_801EF634();
    fn_80103CC0();
    fn_8010264C();
    fn_8025DA88();
    fn_8025D9F0();
    fn_8025DA3C();
    fn_8025D9A8();
    fn_8025D2B0();
    fn_8008ABA0();
    fn_80132A38();
    fn_80106D3C();
    fn_801069FC();
    return 0;
}

/* 0x80063060 | size: 0x750 */
s32 fn_80063060(void) {
    /* TODO: decompile (0x750 bytes, ~468 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_801EF634();
    fn_8025D9A8();
    fn_8025DBB0();
    fn_80129280();
    fn_8012A80C();
    fn_8012A7C4();
    fn_8010264C();
    fn_8025D164();
    fn_8006ADB4();
    fn_800637B0();
    fn_8006ADEC();
    fn_801906A0();
    return 0;
}

/* 0x800637B0 | size: 0x144 */
s32 fn_800637B0(void) {
    /* TODO: decompile (0x144 bytes) */
    fn_8025DA88();
    fn_8025DAAC();
    fn_8006B1F4();
    fn_8006B2A4();
    fn_8006B3C8();
    fn_8006B354();
    return 0;
}

/* 0x800638F4 | size: 0x1E0 */
s32 fn_800638F4(void) {
    /* TODO: decompile (0x1E0 bytes) */
    fn_80063AD4();
    fn_800FB680();
    return 0;
}

/* 0x80063AD4 | size: 0x23C */
s32 fn_80063AD4(void) {
    /* TODO: decompile (0x23C bytes, ~143 instructions) */
    /* Uses floating point */
    fn_800D88DC();
    fn_800D888C();
    fn_800D6A00();
    fn_800D7820();
    fn_800D67BC();
    fn_800D61E4();
    fn_800D5BA0();
    fn_800D6728();
    fn_800FE38C();
    fn_800D5648();
    fn_800FE35C();
    return 0;
}

/* 0x80063D10 | size: 0x4 */
void fn_80063D10(void) {
}

/* 0x80063D14 | size: 0x664 */
s32 fn_80063D14(void) {
    /* TODO: decompile (0x664 bytes, ~409 instructions) */
    /* Uses many saved registers */
    fn_80165A20();
    memcpy();
    fn_8025DA88();
    fn_8025DA3C();
    fn_8006B09C();
    fn_8006A814();
    fn_8006B0F8();
    fn_8008AB4C();
    fn_8006B1D4();
    fn_8025D89C();
    fn_8008AB20();
    fn_8025D744();
    return 0;
}

/* 0x80064378 | size: 0x5C */
s32 fn_80064378(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_800FB680();
    fn_80063AD4();
    return 0;
}

/* 0x800643D4 | size: 0x1254 */
s32 fn_800643D4(void) {
    /* TODO: decompile (0x1254 bytes, ~1173 instructions) */
    fn_8025D970();
    fn_8011F4F0();
    fn_800FA280();
    fn_80132A38();
    fn_800FB680();
    fn_8001DA60();
    fn_8012640C();
    fn_8011F188();
    fn_80123CD4();
    fn_8011BEB4();
    fn_800FBB34();
    fn_8011CA34();
    return 0;
}

/* 0x80065628 | size: 0x108 */
void fn_80065628(void) {
    /* TODO: decompile (0x108 bytes) */
    fn_8025DA88();
    fn_80065A48();
    fn_8025DA18();
}

/* 0x80065730 | size: 0x108 */
void fn_80065730(void) {
    /* TODO: decompile (0x108 bytes) */
    fn_8025DA88();
    fn_80065A48();
    fn_8025DA18();
}

/* 0x80065838 | size: 0x108 */
void fn_80065838(void) {
    /* TODO: decompile (0x108 bytes) */
    fn_8025DA88();
    fn_80065A48();
    fn_8025DA18();
}

/* 0x80065940 | size: 0x108 */
void fn_80065940(void) {
    /* TODO: decompile (0x108 bytes) */
    fn_8025DA88();
    fn_80065A48();
    fn_8025DA18();
}

/* 0x80065A48 | size: 0x1CA4 */
s32 fn_80065A48(void) {
    /* TODO: decompile (0x1CA4 bytes, ~1833 instructions) */
    /* Contains switch/jump table */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_8010B9E8();
    fn_80068DBC();
    fn_80068BB0();
    fn_800689FC();
    fn_8006B1D4();
    fn_8025D89C();
    fn_80068794();
    fn_800688C4();
    fn_8025D9F0();
    return 0;
}

/* 0x800676EC | size: 0x2D4 */
s32 fn_800676EC(void) {
    /* TODO: decompile (0x2D4 bytes, ~181 instructions) */
    fn_800F7BC4();
    fn_8025D2B0();
    fn_8025D89C();
    fn_800679C0();
    fn_8006905C();
    fn_80102ED4();
    return 0;
}

/* 0x800679C0 | size: 0x7F4 */
s32 fn_800679C0(void) {
    /* TODO: decompile (0x7F4 bytes, ~509 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_8025DA3C();
    fn_8025DA88();
    fn_8025D9F0();
    fn_8025D9CC();
    fn_8025D2B0();
    fn_8006AFE4();
    fn_8008ABA0();
    fn_800681B4();
    fn_800F7AF0();
    fn_800F7BC4();
    fn_8025DA18();
    fn_8025D560();
    return 0;
}

/* 0x800681B4 | size: 0x264 */
s32 fn_800681B4(void) {
    /* TODO: decompile (0x264 bytes, ~153 instructions) */
    /* Uses floating point */
    fn_8025DA18();
    fn_8008A9E4();
    fn_8025D560();
    fn_8025D644();
    fn_80166AB8();
    fn_8025D584();
    fn_8025D808();
    fn_8008A9AC();
    fn_8025D5E0();
    return 0;
}

/* 0x80068418 | size: 0x320 */
s32 fn_80068418(void) {
    /* TODO: decompile (0x320 bytes, ~200 instructions) */
    /* Uses floating point */
    /* Uses many saved registers */
    fn_800F7A08();
    fn_800F7A7C();
    fn_800CE2D8();
    fn_800F7BC4();
    fn_800D3088();
    return 0;
}

/* 0x80068738 | size: 0x5C */
s32 fn_80068738(void) {
    /* TODO: decompile (0x5C bytes) */
    fn_80105624();
    fn_80068418();
    return 0;
}

/* 0x80068794 | size: 0x130 */
s32 fn_80068794(void) {
    /* TODO: decompile (0x130 bytes) */
    fn_8025D560();
    fn_8025DA88();
    return 0;
}

/* 0x800688C4 | size: 0x138 */
s32 fn_800688C4(void) {
    /* TODO: decompile (0x138 bytes) */
    fn_8025DA88();
    fn_8025D914();
    fn_8012AC54();
    fn_800FA280();
    fn_8025DA18();
    fn_80132A38();
    fn_8025D9CC();
    fn_800FB680();
    return 0;
}

/* 0x800689FC | size: 0x1B4 */
void fn_800689FC(void) {
    /* TODO: decompile (0x1B4 bytes) */
    fn_8025DA88();
    fn_8025D970();
    fn_801230E0();
}

/* 0x80068BB0 | size: 0x20C */
s32 fn_80068BB0(void) {
    /* TODO: decompile (0x20C bytes, ~131 instructions) */
    /* Uses many saved registers */
    fn_8025DA88();
    fn_8025D970();
    fn_80123FBC();
    fn_8012640C();
    fn_80132A38();
    fn_800FB680();
    return 0;
}

/* 0x80068DBC | size: 0x1C8 */
s32 fn_80068DBC(void) {
    /* TODO: decompile (0x1C8 bytes) */
    fn_8025DA88();
    fn_8025D970();
    fn_8011F4F0();
    fn_800FA280();
    fn_80132A38();
    fn_800FB680();
    return 0;
}

/* 0x80068F84 | size: 0xC4 */
void fn_80068F84(void) {
    /* TODO: decompile (0xC4 bytes) */
}

/* 0x80069048 | size: 0x14 */
void fn_80069048(void) {
}

/* 0x8006905C | size: 0x1C4 */
s32 fn_8006905C(void) {
    /* TODO: decompile (0x1C4 bytes) */
    fn_8025D9CC();
    fn_8025DA88();
    fn_8006B1D4();
    fn_8025D89C();
    fn_8025D560();
    return 0;
}

/* 0x80069220 | size: 0x184 */
s32 fn_80069220(void) {
    /* TODO: decompile (0x184 bytes) */
    fn_800D37CC();
    fn_800D3088();
    return 0;
}

/* 0x800693A4 | size: 0x160 */
s32 fn_800693A4(void) {
    /* TODO: decompile (0x160 bytes) */
    fn_800D37CC();
    fn_800D3088();
    return 0;
}

/* 0x80069504 | size: 0x160 */
s32 fn_80069504(void) {
    /* TODO: decompile (0x160 bytes) */
    fn_800D37CC();
    fn_800D3088();
    return 0;
}

/* 0x80069664 | size: 0x160 */
s32 fn_80069664(void) {
    /* TODO: decompile (0x160 bytes) */
    fn_800D37CC();
    fn_800D3088();
    return 0;
}

/* 0x800697C4 | size: 0x30 */
s32 fn_800697C4(void) {
    fn_8010B01C();
    return 0;
}

/* 0x800697F4 | size: 0x150 */
void fn_800697F4(void) {
    /* TODO: decompile (0x150 bytes) */
    fn_8025DA3C();
    fn_8025DA88();
    fn_80061018();
    fn_8025D970();
    fn_8025D808();
    fn_8025D938();
    fn_80123FBC();
    fn_8010BBB8();
}

/* 0x80069944 | size: 0xC4 */
void fn_80069944(void) {
    /* TODO: decompile (0xC4 bytes) */
}

/* 0x80069A08 | size: 0x58 */
s32 fn_80069A08(void) {
    /* TODO: decompile (0x58 bytes) */
    fn_8010B9E8();
    return 0;
}

#pragma pop
