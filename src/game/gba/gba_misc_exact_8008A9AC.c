#include "dolphin/types.h"

s32 gbaCommandEntryPokemon(u32 value, u8* digits)
{
    digits[0] = value & 0xF;
    digits[1] = (value >> 4) & 0xF;
    digits[2] = (value >> 8) & 0xF;
    digits[3] = (value >> 12) & 0xF;
    digits[4] = (value >> 16) & 0xF;
    digits[5] = (value >> 20) & 0xF;
    return 0;
}
