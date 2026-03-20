/**
 * @file menu_rule.c
 * @brief Menu rule handlers (0x800767B8-0x80077A5C)
 *
 * Address range: 0x800767B8 - 0x80077A5C
 * Total functions: 6
 */

#include "dolphin/types.h"

/* ===== External function declarations ===== */
extern void fn_8006B420();
extern void fn_80076398();
extern void fn_8011E8DC();
extern void fn_8011F1A0();
extern void fn_8011F4A8();
extern void fn_8011F5C8();
extern void fn_80123FBC();
extern void fn_8012640C();
extern void fn_8012AC08();
extern void fn_80142984();
extern void fn_80196E10();

/* ===== SDA globals ===== */
extern u8 lbl_80478928;

/* ===== Rodata / data labels ===== */
extern u8 lbl_80268A48[];
extern u8 lbl_80268A58[];
extern u8 lbl_802EE458[];

/* ===== Forward declarations ===== */
s32 fn_800767B8(void);
s32 fn_80076A8C(void);
s32 fn_80076F2C(void);
s32 fn_800772AC(void);
s32 fn_800774D4(void);
s32 fn_800776E4(void);

/* ===== Function implementations ===== */

#pragma push
#pragma force_active on

/* 0x800767B8 | size: 0x2D4 */
s32 fn_800767B8(void) {
    /* TODO: decompile (0x2D4 bytes, ~181 instructions) */
    /* Uses many saved registers */
    fn_80076F2C();
    fn_8012AC08();
    fn_80123FBC();
    fn_8012640C();
    fn_8011F4A8();
    fn_8011F1A0();
    fn_8006B420();
    fn_80142984();
    fn_80196E10();
    return 0;
}

/* 0x80076A8C | size: 0x4A0 */
s32 fn_80076A8C(void) {
    /* TODO: decompile (0x4A0 bytes, ~296 instructions) */
    /* Uses many saved registers */
    fn_80076F2C();
    fn_8012640C();
    fn_8011E8DC();
    fn_80123FBC();
    fn_8012AC08();
    fn_8011F5C8();
    fn_8011F1A0();
    return 0;
}

/* 0x80076F2C | size: 0x380 */
s32 fn_80076F2C(void) {
    /* TODO: decompile (0x380 bytes, ~224 instructions) */
    /* Uses many saved registers */
    fn_8012AC08();
    fn_8012640C();
    fn_8011E8DC();
    fn_80123FBC();
    fn_8011F4A8();
    fn_8011F1A0();
    fn_8011F5C8();
    return 0;
}

/* 0x800772AC | size: 0x228 */
s32 fn_800772AC(void) {
    /* TODO: decompile (0x228 bytes, ~138 instructions) */
    /* Uses many saved registers */
    fn_8012640C();
    fn_8011F4A8();
    fn_8011F1A0();
    fn_8006B420();
    fn_80142984();
    fn_80196E10();
    return 0;
}

/* 0x800774D4 | size: 0x210 */
s32 fn_800774D4(void) {
    /* TODO: decompile (0x210 bytes, ~132 instructions) */
    fn_8012640C();
    fn_8011F4A8();
    fn_8011F1A0();
    fn_8006B420();
    fn_80142984();
    fn_80196E10();
    return 0;
}

/* 0x800776E4 | size: 0x378 */
s32 fn_800776E4(void) {
    /* TODO: decompile (0x378 bytes, ~222 instructions) */
    /* Uses many saved registers */
    fn_8012AC08();
    fn_80076398();
    fn_8006B420();
    fn_80076F2C();
    fn_80123FBC();
    fn_8012640C();
    fn_8011F4A8();
    fn_8011F1A0();
    fn_80142984();
    fn_80196E10();
    return 0;
}

#pragma pop
