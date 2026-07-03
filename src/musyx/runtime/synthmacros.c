/**
 * @file synthmacros.c
 * @brief MusyX runtime macro interpreter (musyx/runtime/synthmacros.c),
 * 0x801525E4 - 0x80157280.
 *
 * Split out of the misnamed people_field.c unit (2026-07-02). Reference:
 * AxioDL/musyx `musyx/runtime/synthmacros.c`. Boundary evidence: mcmdWait
 * confirmed at 0x801525E4 (simindex seq=0.989 vs MP4/Strikers matched
 * copies); macInit (0x80157218 + 0x68) is reference synthmacros.c's last
 * function and ends exactly at vidInit (0x80157280), reference
 * synthvoice.c's first. The mcmd motion-setter family below
 * (fn_80153FEC .. fn_80154A14) inlines the same-TU static
 * MotionSetterCommon; all other functions asm-only until matched.
 */
#include "dolphin/types.h"

extern void inpAddCtrl(void* dst, u32 lowByte, s32 value, u32 repeat, u32 hasUpperByte);
static void MotionSetterCommon(u8* ctx, u32* cmd, u64 initMask, u32 dataOffset, u32 doneMask) {
    u32 comb;
    s32 scale;
    u8 upperByte;

    if (!(*(u64*)(ctx + 0x114) & initMask)) {
        comb = 0;
        *(u64*)(ctx + 0x114) |= initMask;
    } else {
        comb = cmd[1] & 0xFF;
    }
    scale = ((s16)(cmd[0] >> 16) << 16) / 100;
    if (scale < 0) {
        scale -= ((s8)(cmd[1] >> 0x10) << 8) / 100;
    } else {
        scale += ((s8)(cmd[1] >> 0x10) << 8) / 100;
    }
    upperByte = (cmd[1] >> 8) & 0xFF;
    inpAddCtrl(ctx + dataOffset, (cmd[0] >> 8) & 0xFF, scale, comb, upperByte != 0);
    *(u32*)(ctx + 0x214) |= doneMask;
}
#define PF_DEFINE_MOTION_SETTER(name, initMask, dataOffset, doneMask) \
void name(u8* ctx, u32* cmd) { MotionSetterCommon(ctx, cmd, (initMask), (dataOffset), (doneMask)); }
#if 0
asm void fn_80153FEC(void) {
#include "src/game/people/people_field_fn_80153FEC.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80153FEC, 0x00100000ULL, 0x23C, 0x0002u)
#endif
#if 0
asm void fn_801540F0(void) {
#include "src/game/people/people_field_fn_801540F0.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801540F0, 0x00200000ULL, 0x284, 0x0008u)
#endif
#if 0
asm void fn_801541F4(void) {
#include "src/game/people/people_field_fn_801541F4.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801541F4, 0x00400000ULL, 0x2CC, 0x0020u)
#endif
#if 0
asm void fn_801542F8(void) {
#include "src/game/people/people_field_fn_801542F8.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801542F8, 0x02000000ULL, 0x2F0, 0x0040u)
#endif
#if 0
asm void fn_801543FC(void) {
#include "src/game/people/people_field_fn_801543FC.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_801543FC, 0x01000000ULL, 0x314, 0x0080u)
#endif
#if 0
asm void fn_80154500(void) {
#include "src/game/people/people_field_fn_80154500.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154500, 0x00800000ULL, 0x35C, 0x0200u)
#endif
#if 0
asm void fn_80154604(void) {
#include "src/game/people/people_field_fn_80154604.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154604, 0x20000000ULL, 0x338, 0x0100u)
#endif
#if 0
asm void fn_80154708(void) {
#include "src/game/people/people_field_fn_80154708.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154708, 0x40000000ULL, 0x380, 0x0400u)
#endif
#if 0
asm void fn_8015480C(void) {
#include "src/game/people/people_field_fn_8015480C.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_8015480C, 0x80000000ULL, 0x3A4, 0x0800u)
#endif
#if 0
asm void fn_80154910(void) {
#include "src/game/people/people_field_fn_80154910.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154910, 0x04000000ULL, 0x260, 0x0004u)
#endif
#if 0
asm void fn_80154A14(void) {
#include "src/game/people/people_field_fn_80154A14.inc"
}
#else
PF_DEFINE_MOTION_SETTER(fn_80154A14, 0x08000000ULL, 0x2A8, 0x0010u)
#endif
#undef PF_DEFINE_MOTION_SETTER
