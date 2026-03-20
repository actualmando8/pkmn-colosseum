/**
 * @file gs_render_util.c
 * @brief GS render utility / HSD bridge code before GSgfx.
 *
 * Contains utility functions for the rendering pipeline including
 * HSD object management, matrix/vector operations, and model
 * rendering helpers.
 *
 * Address range: 0x800D104C - 0x800D3074
 * ~40 functions
 */

#include "dolphin/types.h"

/* ===== External references ===== */
extern void fn_800DD970(const char* fmt, ...);

/* ===== Global state (SDA) ===== */
extern u8 lbl_8047AA80[];

/* ==================================================================
 * fn_800D104C
 * Address: 0x800D104C, Size: 0x24
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D104C(void) {
    /* TODO: match -- 0x24 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1070
 * Address: 0x800D1070, Size: 0x354
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1070(void) {
    /* TODO: match -- 0x354 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D13C4
 * Address: 0x800D13C4, Size: 0x4
 * ================================================================== */
void fn_800D13C4(void) {
    /* 4 bytes -- likely just blr (empty function) */
}

/* ==================================================================
 * fn_800D13C8
 * Address: 0x800D13C8, Size: 0x2AC
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D13C8(void) {
    /* TODO: match -- 0x2AC bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1674
 * Address: 0x800D1674, Size: 0xB8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1674(void) {
    /* TODO: match -- 0xB8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D172C
 * Address: 0x800D172C, Size: 0x8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D172C(void) {
    /* TODO: match -- 0x8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1734
 * Address: 0x800D1734, Size: 0x8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1734(void) {
    /* TODO: match -- 0x8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D173C
 * Address: 0x800D173C, Size: 0x5C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D173C(void) {
    /* TODO: match -- 0x5C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1798
 * Address: 0x800D1798, Size: 0xC0
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1798(void) {
    /* TODO: match -- 0xC0 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1858
 * Address: 0x800D1858, Size: 0x8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1858(void) {
    /* TODO: match -- 0x8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1860
 * Address: 0x800D1860, Size: 0x9C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1860(void) {
    /* TODO: match -- 0x9C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D18FC
 * Address: 0x800D18FC, Size: 0x88
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D18FC(void) {
    /* TODO: match -- 0x88 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1984
 * Address: 0x800D1984, Size: 0xB4
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1984(void) {
    /* TODO: match -- 0xB4 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1A38
 * Address: 0x800D1A38, Size: 0x8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1A38(void) {
    /* TODO: match -- 0x8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1A40
 * Address: 0x800D1A40, Size: 0x30
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1A40(void) {
    /* TODO: match -- 0x30 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1A70
 * Address: 0x800D1A70, Size: 0xCC
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1A70(void) {
    /* TODO: match -- 0xCC bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1B3C
 * Address: 0x800D1B3C, Size: 0x1C4
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1B3C(void) {
    /* TODO: match -- 0x1C4 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1D00
 * Address: 0x800D1D00, Size: 0x1B8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1D00(void) {
    /* TODO: match -- 0x1B8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1EB8
 * Address: 0x800D1EB8, Size: 0x4C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1EB8(void) {
    /* TODO: match -- 0x4C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1F04
 * Address: 0x800D1F04, Size: 0x54
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1F04(void) {
    /* TODO: match -- 0x54 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1F58
 * Address: 0x800D1F58, Size: 0x2C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1F58(void) {
    /* TODO: match -- 0x2C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1F84
 * Address: 0x800D1F84, Size: 0x58
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1F84(void) {
    /* TODO: match -- 0x58 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D1FDC
 * Address: 0x800D1FDC, Size: 0x60
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D1FDC(void) {
    /* TODO: match -- 0x60 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D203C
 * Address: 0x800D203C, Size: 0x40
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D203C(void) {
    /* TODO: match -- 0x40 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D207C
 * Address: 0x800D207C, Size: 0x50
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D207C(void) {
    /* TODO: match -- 0x50 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D20CC
 * Address: 0x800D20CC, Size: 0x84
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D20CC(void) {
    /* TODO: match -- 0x84 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2150
 * Address: 0x800D2150, Size: 0x78
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2150(void) {
    /* TODO: match -- 0x78 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D21C8
 * Address: 0x800D21C8, Size: 0x80
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D21C8(void) {
    /* TODO: match -- 0x80 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2248
 * Address: 0x800D2248, Size: 0x33C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2248(void) {
    /* TODO: match -- 0x33C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2584
 * Address: 0x800D2584, Size: 0x8
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2584(void) {
    /* TODO: match -- 0x8 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D258C
 * Address: 0x800D258C, Size: 0x1AC
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D258C(void) {
    /* TODO: match -- 0x1AC bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2738
 * Address: 0x800D2738, Size: 0xC4
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2738(void) {
    /* TODO: match -- 0xC4 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D27FC
 * Address: 0x800D27FC, Size: 0x1A4
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D27FC(void) {
    /* TODO: match -- 0x1A4 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D29A0
 * Address: 0x800D29A0, Size: 0x134
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D29A0(void) {
    /* TODO: match -- 0x134 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2AD4
 * Address: 0x800D2AD4, Size: 0x70
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2AD4(void) {
    /* TODO: match -- 0x70 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2B44
 * Address: 0x800D2B44, Size: 0x4C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2B44(void) {
    /* TODO: match -- 0x4C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2B90
 * Address: 0x800D2B90, Size: 0x258
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2B90(void) {
    /* TODO: match -- 0x258 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2DE8
 * Address: 0x800D2DE8, Size: 0x14C
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2DE8(void) {
    /* TODO: match -- 0x14C bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D2F34
 * Address: 0x800D2F34, Size: 0x128
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D2F34(void) {
    /* TODO: match -- 0x128 bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D305C
 * Address: 0x800D305C, Size: 0xC
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D305C(void) {
    /* TODO: match -- 0xC bytes */
}
#pragma pop

/* ==================================================================
 * fn_800D3068
 * Address: 0x800D3068, Size: 0xC
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800D3068(void) {
    /* TODO: match -- 0xC bytes */
}
#pragma pop
