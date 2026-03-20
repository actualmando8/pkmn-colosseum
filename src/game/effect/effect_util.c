/**
 * @file effect_util.c
 * @brief Decompiled functions.
 *
 * Address range: 0x8013151C - 0x80137114
 */

#include "dolphin/types.h"

/* ===================================================================
 * Generated: 59 pattern-matched + 148 stubs
 * Range: 0x8013151C - 0x80137114
 * =================================================================== */

extern u32 lbl_803635D8;

extern u32 lbl_8047ADC8;
extern u32 lbl_8047ADCC;
extern u32 lbl_8047ADD0;
extern u32 lbl_8047ADE4;
extern u32 lbl_8047ADE8;
extern u32 lbl_8047ADEC;
extern u32 lbl_8047ADF0;
extern u32 lbl_8047ADF4;
extern u32 lbl_8047ADF8;
extern u32 lbl_8047ADFC;
extern u32 lbl_8047AE00;
extern u32 lbl_8047AE04;
extern u32 lbl_8047AE08;
extern u32 lbl_8047AE0C;
extern u32 lbl_8047AE20;
extern u32 lbl_8047AE24;
extern u32 lbl_8047AE28;
extern u32 lbl_8047AE2C;
extern u32 lbl_8047AE30;
extern u32 lbl_8047AE34;
extern u32 lbl_8047AE38;
extern u32 lbl_8047AE3C;
extern u32 lbl_8047AE40;
extern u32 lbl_8047AE5C;
extern u32 lbl_8047AE60;
extern u32 lbl_8047AE64;
extern u32 lbl_8047AE70;
extern u32 lbl_8047AE74;
extern u32 lbl_8047AE78;
extern u32 lbl_8047AE88;
extern u32 lbl_8047AE8C;
extern u32 lbl_8047AE98;
extern u32 lbl_8047AE9C;
extern u16 lbl_8047AEA2;
extern u8 lbl_8047AED0;

/* ===== Index lookup globals ===== */
extern u8 lbl_803635C0[];  /* effect table (BSS) */
extern u8 lbl_80363B88[];  /* trace fx table (BSS) */
extern u8 lbl_80363C00[];  /* trace table (BSS) */
extern u32 lbl_80478B98;  /* effect count (SDA) */
extern u32 lbl_80478BA0;  /* trace count (SDA) */

/* Forward declarations for converted functions */
void fn_80133E6C(void);



/* 0x58 | fn_8013151C | leaf_multi_output */
void fn_8013151C(u32* out1) {
    if (out1 != NULL) { *out1 = *(u32*)((u8*)lbl_803635C0 + 0); }
}

/* 0x80131574 | 20 bytes | indexed_lookup */
u8 fn_80131574(u32 idx) {
    return ((u8*)lbl_803635D8)[idx];
}

/* 0x64 | fn_80131588 | guarded_call */
u32 fn_80131588(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (1 /* guard r0 != 0 */) { return 0; }
    if (1 /* guard r3 != 0 */) { return 0; }
    fn_801666BC();
    return 1;
}

/* 0x44 | fn_801315EC | guarded_call */
u32 fn_801315EC(void) {
    if (1 /* guard r0 != 0 */) { return 0; }
    if (0 /* guard r3 == 0 */) { return 0; }
    fn_80165A20();
    return 0;
}

/* 0x80131630 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131630(void) {
    /* TODO: match -- 48 bytes at 0x80131630 */
}
#pragma pop

/* 0x80131660 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131660(void) {
    /* TODO: match -- 48 bytes at 0x80131660 */
}
#pragma pop

/* 0x80131690 | 16 bytes | set_field_return */
u32 fn_80131690(void* obj) {
    *(u8*)((u8*)obj + 0x41) = 1;
    return 0;
}

/* 0x801316A0 | 0x8 | sda_getter */
u16 fn_801316A0(void) { return lbl_8047AEA2; }

/* 0x801316A8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801316A8(void) {
    /* TODO: match -- 40 bytes at 0x801316A8 */
}
#pragma pop

/* 0x801316D0 | 0x8 | sda_getter */
u32 fn_801316D0(void) { return lbl_8047AE8C; }

/* 0x801316D8 | 0x8 | sda_getter */
u32 fn_801316D8(void) { return lbl_8047AE9C; }

/* 0x801316E0 | 0x8 | sda_getter */
u32 fn_801316E0(void) { return lbl_8047AE98; }

/* 0x801316E8 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801316E8(void) {
    /* TODO: match -- 44 bytes at 0x801316E8 */
}
#pragma pop

/* 0x54 | fn_80131714 | generic */
u32 fn_80131714(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FA064();
    return 0;
}

/* 0x80131768 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131768(void) {
    /* TODO: match -- 44 bytes at 0x80131768 */
}
#pragma pop

/* 0x80131794 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131794(void) {
    /* TODO: match -- 52 bytes at 0x80131794 */
}
#pragma pop

/* 0x801317C8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801317C8(void) {
    /* TODO: match -- 52 bytes at 0x801317C8 */
}
#pragma pop

/* 0x801317FC | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801317FC(void) {
    /* TODO: match -- 40 bytes at 0x801317FC */
}
#pragma pop

/* 0x80131824 | 0x8 | sda_getter */
u32 fn_80131824(void) { return lbl_8047AE88; }

/* 0x8013182C | 0x208 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013182C(void) {
    /* TODO: match -- 520 bytes at 0x8013182C */
}
#pragma pop

/* 0x80131A34 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131A34(void) {
    /* TODO: match -- 52 bytes at 0x80131A34 */
}
#pragma pop

/* 0x80131A68 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131A68(void) {
    /* TODO: match -- 52 bytes at 0x80131A68 */
}
#pragma pop

/* 0x80131A9C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131A9C(void) {
    /* TODO: match -- 52 bytes at 0x80131A9C */
}
#pragma pop

/* 0x80131AD0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131AD0(void) {
    /* TODO: match -- 52 bytes at 0x80131AD0 */
}
#pragma pop

/* 0x80131B04 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131B04(void) {
    /* TODO: match -- 52 bytes at 0x80131B04 */
}
#pragma pop

/* 0x80131B38 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131B38(void) {
    /* TODO: match -- 52 bytes at 0x80131B38 */
}
#pragma pop

/* 0x80131B6C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131B6C(void) {
    /* TODO: match -- 52 bytes at 0x80131B6C */
}
#pragma pop

/* 0x80131BA0 | 0x8 | return_const */
u32 fn_80131BA0(void) { return 0; }

/* 0x80131BA8 | 0x8 | return_const */
u32 fn_80131BA8(void) { return 0; }

/* 0x80131BB0 | 0x8 | sda_getter */
u32 fn_80131BB0(void) { return lbl_8047AE40; }

/* 0x80131BB8 | 0x8 | sda_getter */
u32 fn_80131BB8(void) { return lbl_8047AE3C; }

/* 0x80131BC0 | 0x8 | sda_getter */
u32 fn_80131BC0(void) { return lbl_8047AE38; }

/* 0x80131BC8 | 0x8 | sda_getter */
u32 fn_80131BC8(void) { return lbl_8047AE34; }

/* 0x80131BD0 | 0x8 | sda_getter */
u32 fn_80131BD0(void) { return lbl_8047AE30; }

/* 0x80131BD8 | 0x8 | sda_getter */
u32 fn_80131BD8(void) { return lbl_8047AE2C; }

/* 0x80131BE0 | 0x8 | sda_getter */
u32 fn_80131BE0(void) { return lbl_8047AE28; }

/* 0x80131BE8 | 0x8 | sda_getter */
u32 fn_80131BE8(void) { return lbl_8047AE24; }

/* 0x80131BF0 | 0x8 | sda_getter */
u32 fn_80131BF0(void) { return lbl_8047AE20; }

/* 0x80131BF8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131BF8(void) {
    /* TODO: match -- 40 bytes at 0x80131BF8 */
}
#pragma pop

/* 0x80131C20 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131C20(void) {
    /* TODO: match -- 40 bytes at 0x80131C20 */
}
#pragma pop

/* 0x80131C48 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131C48(void) {
    /* TODO: match -- 40 bytes at 0x80131C48 */
}
#pragma pop

/* 0x80131C70 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131C70(void) {
    /* TODO: match -- 40 bytes at 0x80131C70 */
}
#pragma pop

/* 0x80131C98 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131C98(void) {
    /* TODO: match -- 40 bytes at 0x80131C98 */
}
#pragma pop

/* 0x80131CC0 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131CC0(void) {
    /* TODO: match -- 40 bytes at 0x80131CC0 */
}
#pragma pop

/* 0x80131CE8 | 0x21C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131CE8(void) {
    /* TODO: match -- 540 bytes at 0x80131CE8 */
}
#pragma pop

/* 0x80131F04 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131F04(void) {
    /* TODO: match -- 152 bytes at 0x80131F04 */
}
#pragma pop

/* 0x80131F9C | 0x8 | sda_getter */
u32 fn_80131F9C(void) { return lbl_8047AE0C; }

/* 0x80131FA4 | 0x8 | sda_getter */
u32 fn_80131FA4(void) { return lbl_8047AE08; }

/* 0x80131FAC | 0x8 | sda_getter */
u32 fn_80131FAC(void) { return lbl_8047AE04; }

/* 0x80131FB4 | 0x8 | sda_getter */
u32 fn_80131FB4(void) { return lbl_8047AE00; }

/* 0x80131FBC | 0x8 | sda_getter */
u32 fn_80131FBC(void) { return lbl_8047ADFC; }

/* 0x80131FC4 | 0x8 | sda_getter */
u32 fn_80131FC4(void) { return lbl_8047ADF8; }

/* 0x80131FCC | 0x8 | sda_getter */
u32 fn_80131FCC(void) { return lbl_8047ADF4; }

/* 0x80131FD4 | 0x8 | sda_getter */
u32 fn_80131FD4(void) { return lbl_8047ADF0; }

/* 0x80131FDC | 0x8 | sda_getter */
u32 fn_80131FDC(void) { return lbl_8047ADEC; }

/* 0x80131FE4 | 0x8 | sda_getter */
u32 fn_80131FE4(void) { return lbl_8047ADE8; }

/* 0x80131FEC | 0x8 | sda_getter */
u32 fn_80131FEC(void) { return lbl_8047ADE4; }

/* 0x80131FF4 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80131FF4(void) {
    /* TODO: match -- 152 bytes at 0x80131FF4 */
}
#pragma pop

/* 0x8013208C | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013208C(void) {
    /* TODO: match -- 152 bytes at 0x8013208C */
}
#pragma pop

/* 0x80132124 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132124(void) {
    /* TODO: match -- 152 bytes at 0x80132124 */
}
#pragma pop

/* 0x801321BC | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801321BC(void) {
    /* TODO: match -- 152 bytes at 0x801321BC */
}
#pragma pop

/* 0x80132254 | 0x8 | sda_getter */
u32 fn_80132254(void) { return lbl_8047ADD0; }

/* 0x8013225C | 0x8 | sda_getter */
u32 fn_8013225C(void) { return lbl_8047ADCC; }

/* 0x80132264 | 0x8 | sda_getter */
u32 fn_80132264(void) { return lbl_8047ADC8; }

/* 0x8013226C | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013226C(void) {
    /* TODO: match -- 40 bytes at 0x8013226C */
}
#pragma pop

/* 0x80132294 | 0x8 | sda_getter */
u32 fn_80132294(void) { return lbl_8047AE78; }

/* 0x8013229C | 0x8 | sda_getter */
u32 fn_8013229C(void) { return lbl_8047AE74; }

/* 0x801322A4 | 0x8 | sda_getter */
u32 fn_801322A4(void) { return lbl_8047AE70; }

/* 0x801322AC | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801322AC(void) {
    /* TODO: match -- 52 bytes at 0x801322AC */
}
#pragma pop

/* 0x801322E0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801322E0(void) {
    /* TODO: match -- 52 bytes at 0x801322E0 */
}
#pragma pop

/* 0x80132314 | 0x8 | sda_getter */
u32 fn_80132314(void) { return lbl_8047AE64; }

/* 0x8013231C | 0x8 | sda_getter */
u32 fn_8013231C(void) { return lbl_8047AE60; }

/* 0x80132324 | 0x8 | sda_getter */
u32 fn_80132324(void) { return lbl_8047AE5C; }

/* 0x8013232C | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013232C(void) {
    /* TODO: match -- 52 bytes at 0x8013232C */
}
#pragma pop

/* 0x80132360 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132360(void) {
    /* TODO: match -- 52 bytes at 0x80132360 */
}
#pragma pop

/* 0x80132394 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132394(void) {
    /* TODO: match -- 52 bytes at 0x80132394 */
}
#pragma pop

/* 0x801323C8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801323C8(void) {
    /* TODO: match -- 52 bytes at 0x801323C8 */
}
#pragma pop

/* 0x801323FC | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801323FC(void) {
    /* TODO: match -- 44 bytes at 0x801323FC */
}
#pragma pop

/* 0x80132428 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132428(void) {
    /* TODO: match -- 44 bytes at 0x80132428 */
}
#pragma pop

/* 0x78 | fn_80132454 | generic */
u32 fn_80132454(void) {
    return 0;
}

/* 0x801324CC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801324CC(void) {
    /* TODO: match -- 164 bytes at 0x801324CC */
}
#pragma pop

/* 0x54 | fn_80132570 | generic */
u32 fn_80132570(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FA160();
    return 0;
}

/* 0x68 | fn_801325C4 | two_call_arg_check */
void fn_801325C4(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    if (arg1 != 0) { return; }
    fn_800FA1BC();
    /* store u16 to offset 0x20 */
    /* store u16 to offset 0x20 */
    /* store u32 to offset 0x30 */
    fn_800FA1BC();
}

/* 0x8013262C | 16 bytes | set_field_return */
u32 fn_8013262C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 0;
    return 0;
}

/* 0x8013263C | 16 bytes | set_field_return */
u32 fn_8013263C(void* obj) {
    *(u8*)((u8*)obj + 0x4B) = 2;
    return 0;
}

/* 0x44 | fn_8013264C | generic */
u32 fn_8013264C(u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    fn_800FAA98();
    return 0;
}

/* 0x80132690 | 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132690(void) {
    /* TODO: match -- 204 bytes at 0x80132690 */
}
#pragma pop

/* 0x8013275C | 0x84 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013275C(void) {
    /* TODO: match -- 132 bytes at 0x8013275C */
}
#pragma pop

/* 0x54 | fn_801327E0 | framed_no_calls */
u32 fn_801327E0(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_8047D0E0 */
    return 0;
}

/* 0x80132834 | 0x204 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132834(void) {
    /* TODO: match -- 516 bytes at 0x80132834 */
}
#pragma pop

/* 0x80132A38 | 0x210 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132A38(void) {
    /* TODO: match -- 528 bytes at 0x80132A38 */
}
#pragma pop

/* 0x80132C48 | 36 bytes | multi_sda_store */
void fn_80132C48(void) {
    lbl_8047AE70 = 0;
    lbl_8047AE74 = 0;
    lbl_8047AE78 = 0;
    lbl_8047AE60 = 0;
    lbl_8047AE64 = 0;
    lbl_8047AE88 = 0;
    lbl_8047AE8C = 0;
}

/* 0x80132C6C | 0x310 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80132C6C(void) {
    /* TODO: match -- 784 bytes at 0x80132C6C */
}
#pragma pop

/* 0x5C | fn_80132F7C | multi_call_cond */
u32 fn_80132F7C(void) {
    if (fn_80102620() == 0) { return 171; }
    fn_80102510();
    fn_801026A4();
    return 0;
}

/* 0x78 | fn_80132FD8 | generic */
u32 fn_80132FD8(void) {
    /* refs: lbl_80272AA8 */
    fn_801E1810();
    fn_800F0308();
    fn_801E1874();
    fn_8010264C();
    fn_800C8520();
    fn_801E189C();
    return 0;
}

/* 0x80133050 | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133050(void) {
    /* TODO: match -- 60 bytes at 0x80133050 */
}
#pragma pop

/* 0x8013308C | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013308C(void) {
    /* TODO: match -- 60 bytes at 0x8013308C */
}
#pragma pop

/* 0x801330C8 | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801330C8(void) {
    /* TODO: match -- 336 bytes at 0x801330C8 */
}
#pragma pop

/* 0x80133218 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133218(void) {
    /* TODO: match -- 56 bytes at 0x80133218 */
}
#pragma pop

/* 0x80133250 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133250(void) {
    /* TODO: match -- 44 bytes at 0x80133250 */
}
#pragma pop

/* 0x5C | fn_8013327C | multi_call_cond */
u32 fn_8013327C(void) {
    if (fn_800E0E14() != 0) { return 1; }
    fn_800DD970("");
    fn_800DD970("");
    return 0;
}

/* 0x801332D8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801332D8(void) {
    /* TODO: match -- 40 bytes at 0x801332D8 */
}
#pragma pop

/* 0x80133300 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133300(void) {
    /* TODO: match -- 40 bytes at 0x80133300 */
}
#pragma pop

/* 0x80133328 | 36 bytes | call_return_const2 */
u32 fn_80133328(void) {
    fn_801D216C();
    return 0;
}

/* 0x60 | fn_8013334C | generic */
u32 fn_8013334C(void) {
    fn_801D1CC4();
    fn_801D1D58();
    fn_801D268C();
    fn_8010264C();
    return 0;
}

/* 0x801333AC | 0xA4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801333AC(void) {
    /* TODO: match -- 164 bytes at 0x801333AC */
}
#pragma pop

/* 0x58 | fn_80133450 | call_sequence */
void fn_80133450(void) {
    fn_801026A4();
    fn_80102510();
}

/* 0x801334A8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801334A8(void) {
    /* TODO: match -- 52 bytes at 0x801334A8 */
}
#pragma pop

/* 0x801334DC | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801334DC(void) {
    /* TODO: match -- 52 bytes at 0x801334DC */
}
#pragma pop

/* 0x5C | fn_80133510 | multi_call_cond */
u32 fn_80133510(void) {
    if (fn_80102620() == 0) { return 7; }
    fn_80102510();
    fn_801026A4();
    return 0;
}

/* 0x68 | fn_8013356C | generic */
u32 fn_8013356C(void) {
    fn_8012F11C();
    fn_8012F150();
    fn_8012F1FC();
    return 0;
}

/* 0x5C | fn_801335D4 | multi_call_cond */
u32 fn_801335D4(void) {
    if (fn_80102620() == 0) { return 4; }
    fn_80102510();
    fn_801026A4();
    return 0;
}

/* 0x80133630 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133630(void) {
    /* TODO: match -- 52 bytes at 0x80133630 */
}
#pragma pop

/* 0x80133664 | 0x13C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133664(void) {
    /* TODO: match -- 316 bytes at 0x80133664 */
}
#pragma pop

/* 0x801337A0 | 0x8 | sda_getter */
u8 fn_801337A0(void) { return lbl_8047AED0; }

/* 0x801337A8 | 0x8 | sda_setter */
void fn_801337A8(u8 val) { lbl_8047AED0 = val; }

/* 0x801337B0 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801337B0(void) {
    /* TODO: match -- 52 bytes at 0x801337B0 */
}
#pragma pop

/* 0x801337E4 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801337E4(void) {
    /* TODO: match -- 44 bytes at 0x801337E4 */
}
#pragma pop

/* 0x80133810 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133810(void) {
    /* TODO: match -- 148 bytes at 0x80133810 */
}
#pragma pop

/* 0x801338A4 | 0x2AC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801338A4(void) {
    /* TODO: match -- 684 bytes at 0x801338A4 */
}
#pragma pop

/* 0x80133B50 | 0x94 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133B50(void) {
    /* TODO: match -- 148 bytes at 0x80133B50 */
}
#pragma pop

/* 0x58 | fn_80133BE4 | generic */
u32 fn_80133BE4(void) {
    /* refs: lbl_80478F8C */
    fn_80133E6C();
    return 0;
}

/* 0x80133C3C | 0x1E0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133C3C(void) {
    /* TODO: match -- 480 bytes at 0x80133C3C */
}
#pragma pop

/* 0x50 | fn_80133E1C | call_then_multi_check */
u32 fn_80133E1C(void) {
    fn_80133E6C();
    /* multi-branch on result */
    return 0;
}

/* 0x80133E6C | 0x2F8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80133E6C(void) {
    /* TODO: match -- 760 bytes at 0x80133E6C */
}
#pragma pop

/* 0x80134164 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134164(void) {
    /* TODO: match -- 196 bytes at 0x80134164 */
}
#pragma pop

/* 0x80134228 | 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134228(void) {
    /* TODO: match -- 48 bytes at 0x80134228 */
}
#pragma pop

/* 0x80134258 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134258(void) {
    /* TODO: match -- 28 bytes at 0x80134258 */
}
#pragma pop

/* 0x44 | fn_80134274 | call_then_multi_check */
u32 fn_80134274(void) {
    u32 result = fn_800057A8();
    /* multi-branch on result */
    return result;
}

/* 0x4C | fn_801342B8 | framed_no_calls */
u32 fn_801342B8(u32 arg1, u32 arg2) {
    /* data manipulation using lbl_80478F8C */
    return 0;
}

/* 0x80134304 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134304(void) {
    /* TODO: match -- 56 bytes at 0x80134304 */
}
#pragma pop

/* 0x8013433C | 0xE4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013433C(void) {
    /* TODO: match -- 228 bytes at 0x8013433C */
}
#pragma pop

/* 0x80134420 | 0x164 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134420(void) {
    /* TODO: match -- 356 bytes at 0x80134420 */
}
#pragma pop

/* 0x80134584 | 0xF8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134584(void) {
    /* TODO: match -- 248 bytes at 0x80134584 */
}
#pragma pop

/* 0x8013467C | 0xEC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013467C(void) {
    /* TODO: match -- 236 bytes at 0x8013467C */
}
#pragma pop

/* 0x68 | fn_80134768 | generic */
u32 fn_80134768(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80129280();
    return 0;
}

/* 0x801347D0 | 0x8 | return_const */
u32 fn_801347D0(void) { return 235; }

/* 0x801347D8 | 0x8 | return_const */
u32 fn_801347D8(void) { return 30; }

/* 0x801347E0 | 0x8 | return_const */
u32 fn_801347E0(void) { return 3; }

/* 0x801347E8 | 0x104 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801347E8(void) {
    /* TODO: match -- 260 bytes at 0x801347E8 */
}
#pragma pop

/* 0x801348EC | 0xF0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801348EC(void) {
    /* TODO: match -- 240 bytes at 0x801348EC */
}
#pragma pop

/* 0x801349DC | 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801349DC(void) {
    /* TODO: match -- 188 bytes at 0x801349DC */
}
#pragma pop

/* 0x60 | fn_80134A98 | generic */
u32 fn_80134A98(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_80129280();
    return 0;
}

/* 0x80134AF8 | 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134AF8(void) {
    /* TODO: match -- 200 bytes at 0x80134AF8 */
}
#pragma pop

/* 0x80134BC0 | 0x250 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134BC0(void) {
    /* TODO: match -- 592 bytes at 0x80134BC0 */
}
#pragma pop

/* 0x80134E10 | 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134E10(void) {
    /* TODO: match -- 224 bytes at 0x80134E10 */
}
#pragma pop

/* 0x80134EF0 | 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134EF0(void) {
    /* TODO: match -- 152 bytes at 0x80134EF0 */
}
#pragma pop

/* 0x80134F88 | 0x9C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80134F88(void) {
    /* TODO: match -- 156 bytes at 0x80134F88 */
}
#pragma pop

/* 0x80135024 | 0x4 | void_stub */
void fn_80135024(void) {
}

/* 0x80135028 | 0x8 | return_const */
u32 fn_80135028(void) { return 0; }

/* 0x80135030 | 0x138 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135030(void) {
    /* TODO: match -- 312 bytes at 0x80135030 */
}
#pragma pop

/* 0x80135168 | 0x124 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135168(void) {
    /* TODO: match -- 292 bytes at 0x80135168 */
}
#pragma pop

/* 0x8013528C | 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013528C(void) {
    /* TODO: match -- 172 bytes at 0x8013528C */
}
#pragma pop

/* 0x80135338 | 0x88 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135338(void) {
    /* TODO: match -- 136 bytes at 0x80135338 */
}
#pragma pop

/* 0x801353C0 | 0x170 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801353C0(void) {
    /* TODO: match -- 368 bytes at 0x801353C0 */
}
#pragma pop

/* 0x80135530 | 0x1D8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135530(void) {
    /* TODO: match -- 472 bytes at 0x80135530 */
}
#pragma pop

/* 0x80135708 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135708(void) {
    /* TODO: match -- 308 bytes at 0x80135708 */
}
#pragma pop

/* 0x8013583C | 0xFC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013583C(void) {
    /* TODO: match -- 252 bytes at 0x8013583C */
}
#pragma pop

/* 0x80135938 | 0xF8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135938(void) {
    /* TODO: match -- 248 bytes at 0x80135938 */
}
#pragma pop

/* 0x80135A30 | 0x10 | nc_setter */
void fn_80135A30(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x3) = val;
}

/* 0x80135A40 | 0x10 | nc_setter */
void fn_80135A40(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x2) = val;
}

/* 0x80135A50 | 0x10 | nc_setter */
void fn_80135A50(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x1) = val;
}

/* 0x80135A60 | 0x10 | nc_setter */
void fn_80135A60(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x0) = val;
}

/* 0x80135A70 | 0x18 | nc_getter */
u8 fn_80135A70(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x3);
}

/* 0x80135A88 | 0x18 | nc_getter */
u8 fn_80135A88(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x2);
}

/* 0x80135AA0 | 0x18 | nc_getter */
u8 fn_80135AA0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x1);
}

/* 0x80135AB8 | 0x18 | nc_getter */
u8 fn_80135AB8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x0);
}

/* 0x80135AD0 | 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135AD0(void) {
    /* TODO: match -- 28 bytes at 0x80135AD0 */
}
#pragma pop

/* 0x80135AEC | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135AEC(void) {
    /* TODO: match -- 32 bytes at 0x80135AEC */
}
#pragma pop

/* 0x80135B0C | 16 bytes | nc_bnelr */
u32 fn_80135B0C(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}

/* 0x80135B1C | 0x10 | nc_setter */
void fn_80135B1C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x22) = val;
}

/* 0x80135B2C | 0x10 | nc_setter */
void fn_80135B2C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x21) = val;
}

/* 0x80135B3C | 0x10 | nc_setter */
void fn_80135B3C(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)((u8*)ptr + 0x20) = val;
}

/* 0x80135B4C | 0x10 | nc_setter */
void fn_80135B4C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x1C) = val;
}

/* 0x80135B5C | 0x10 | nc_setter_f */
void fn_80135B5C(void* ptr, f32 val) {
    if (ptr == NULL) { return; }
    *(f32*)((u8*)ptr + 0x18) = val;
}

/* 0x80135B6C | 0x10 | nc_setter */
void fn_80135B6C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x14) = val;
}

/* 0x80135B7C | 0x10 | nc_setter */
void fn_80135B7C(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0x8) = val;
}

/* 0x80135B8C | 0x14 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135B8C(void) {
    /* TODO: match -- 20 bytes at 0x80135B8C */
}
#pragma pop

/* 0x80135BA0 | 0x10 | nc_setter */
void fn_80135BA0(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)((u8*)ptr + 0xC) = val;
}

/* 0x80135BB0 | 24 bytes | beq_default_getter */
u8 fn_80135BB0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x22);
}

/* 0x80135BC8 | 24 bytes | beq_default_getter */
u8 fn_80135BC8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x21);
}

/* 0x80135BE0 | 24 bytes | beq_default_getter */
u8 fn_80135BE0(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)((u8*)ptr + 0x20);
}

/* 0x80135BF8 | 24 bytes | beq_default_getter */
u32 fn_80135BF8(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x1C);
}

/* 0x80135C10 | 0x18 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135C10(void) {
    /* TODO: match -- 24 bytes at 0x80135C10 */
}
#pragma pop

/* 0x80135C28 | 24 bytes | beq_default_getter */
u32 fn_80135C28(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x14);
}

/* 0x80135C40 | 24 bytes | beq_default_getter */
u32 fn_80135C40(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0x8);
}

/* 0x80135C58 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135C58(void) {
    /* TODO: match -- 32 bytes at 0x80135C58 */
}
#pragma pop

/* 0x80135C78 | 24 bytes | beq_default_getter */
u32 fn_80135C78(void* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)((u8*)ptr + 0xC);
}

/* 0x40 | fn_80135C90 | generic */
void fn_80135C90(u32 arg1, u32 arg2) {

}

/* 0x80135CD0 | 24 bytes | nc_addi_ptr */
void* fn_80135CD0(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}

/* 0x80135CE8 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135CE8(void) {
    /* TODO: match -- 40 bytes at 0x80135CE8 */
}
#pragma pop

/* 0x80135D10 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135D10(void) {
    /* TODO: match -- 308 bytes at 0x80135D10 */
}
#pragma pop

/* 0x80135E44 | 0x114 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135E44(void) {
    /* TODO: match -- 276 bytes at 0x80135E44 */
}
#pragma pop

/* 0x80135F58 | 0x38 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135F58(void) {
    /* TODO: match -- 56 bytes at 0x80135F58 */
}
#pragma pop

/* 0x80135F90 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135F90(void) {
    /* TODO: match -- 44 bytes at 0x80135F90 */
}
#pragma pop

/* 0x80135FBC | 0x3C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135FBC(void) {
    /* TODO: match -- 60 bytes at 0x80135FBC */
}
#pragma pop

/* 0x80135FF8 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80135FF8(void) {
    /* TODO: match -- 44 bytes at 0x80135FF8 */
}
#pragma pop

/* 0x80136024 | 0x2C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80136024(void) {
    /* TODO: match -- 44 bytes at 0x80136024 */
}
#pragma pop

/* 0x80136050 | 0x28 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80136050(void) {
    /* TODO: match -- 40 bytes at 0x80136050 */
}
#pragma pop

/* 0x80136078 | 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80136078(void) {
    /* TODO: match -- 196 bytes at 0x80136078 */
}
#pragma pop

/* 0x8013613C | 0x22C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8013613C(void) {
    /* TODO: match -- 556 bytes at 0x8013613C */
}
#pragma pop

/* 0x40 | fn_80136368 | index_lookup */
u32 fn_80136368(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B98) { return 0; }
    entry = (u8*)lbl_80363B88 + idx * 0x18;
    if (entry == NULL) { return 0; }
    return *(u32*)((u8*)entry + 0x8);
}

/* 0x40 | fn_801363A8 | index_lookup */
u8 fn_801363A8(u16 idx) {
    void* entry;
    if (idx >= lbl_80478B98) { return 0; }
    entry = (u8*)lbl_80363B88 + idx * 0x18;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x0);
}

/* 0x40 | fn_801363E8 | index_lookup */
u16 fn_801363E8(u16 idx) {
    void* entry;
    if (idx >= lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0x4);
}

/* 0x40 | fn_80136428 | index_lookup */
u8 fn_80136428(u16 idx) {
    void* entry;
    if (idx >= lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u8*)((u8*)entry + 0x0);
}

/* 0x40 | fn_80136468 | index_lookup */
u16 fn_80136468(u16 idx) {
    void* entry;
    if (idx >= lbl_80478BA0) { return 0; }
    entry = (u8*)lbl_80363C00 + idx * 0xC;
    if (entry == NULL) { return 0; }
    return *(u16*)((u8*)entry + 0x2);
}

/* 0x801364A8 | 0xC6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801364A8(void) {
    /* TODO: match -- 3180 bytes at 0x801364A8 */
}
#pragma pop
