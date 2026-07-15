/**
 * @file gs_range_8003686C.c
 * @brief Post-movie boot fragment, 0x8003686C - 0x80037158.
 *
 * Called from movie.c right after menuOpen(0x85, ...); spawns
 * _menuSoundReadWaveThread and runs filesystem init (_fsysInitTOC).
 * Identified during the PDA decomposition (2026-07-03); the range
 * name stays honest until the TU identity is proven.
 */
#include "dolphin/types.h"

#pragma push
#pragma peephole off
s32 fn_8003708C(void)
{
    extern s32 _fsysInitTOC(u32 numSlots, u32 param2, u32 param3, u32 param4);
    extern u32 GSgappCreate(s32 state, u8 priority, void* param, void* func);
    extern void fn_8017AAA4(void);
    extern u32 lbl_8047A464;

    _fsysInitTOC(0x40, 0, 0, 0);
    GSgappCreate(1, 0x14, 0, fn_8017AAA4);
    lbl_8047A464 = 1;
    return 0;
}
#pragma pop

#pragma push
#pragma peephole off
s32 _menuSoundReadWaveThread__FPv(u32* arg)
{
    extern void fn_801655D4(u32 index);
    extern u32 lbl_8047A460;

    if (arg[0] != 0) {
        fn_801655D4(arg[0]);
    }
    if (arg[1] != 0) {
        fn_801655D4(arg[1]);
    }
    if (arg[2] != 0) {
        fn_801655D4(arg[2]);
    }
    if (arg[3] != 0) {
        fn_801655D4(arg[3]);
    }
    lbl_8047A460--;
    return 0;
}
#pragma pop
