/**
 * @file floor_character_exact_80117038.c
 * @brief Strict floor-character flag accessors, 0x80117038 - 0x80117070.
 */
#include "dolphin/types.h"

u32 floorCharacterBiosGetLoadInit(u8* ptr)
{
    return ptr != NULL ? (u32)((ptr[0] >> 6) & 1) : 0;
}

u32 floorCharacterBiosGetVisibility(u8* ptr)
{
    return ptr != NULL ? (u32)((ptr[0] >> 7) & 1) : 0;
}
