/**
 * @file fight_range_exact_802134D4.c
 * @brief Strict turn-check callback island at 0x802134D4.
 */
#include "dolphin/types.h"

extern u8 fightOutPokemonCheckFightOut(void*);
extern void fightOutPokemonWriteJoutaiDataId(void*, u32);
extern u8 fn_802026E4(void*, u32);
extern void fightOutPokemonInitJoutaiKeep(void*);

s32 _fightSeqTurnCheckSubFightOutPokemon__FPvUsPv(
    void* ctx, u16 param2, void* param3)
{
    if (!fightOutPokemonCheckFightOut(ctx)) {
        return 1;
    }
    fightOutPokemonWriteJoutaiDataId(ctx, 0x11);
    if (fn_802026E4(ctx, 8) == 1 && fn_802026E4(ctx, 0x22) == 1) {
        fightOutPokemonInitJoutaiKeep(ctx);
    }
    return 1;
}
