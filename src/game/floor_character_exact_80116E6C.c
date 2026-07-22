/**
 * @file floor_character_exact_80116E6C.c
 * @brief Strict floor-character BIOS accessors, 0x80116E6C - 0x80116F68.
 */
#include "dolphin/types.h"

void floorCharacterBiosSetVisibility(u8* ptr, u8 val)
{
    u8 tmp;

    if (ptr == NULL) {
        return;
    }
    tmp = ptr[0];
    tmp = (u8)(((val & 1) << 7) | (tmp & ~0x80));
    ptr[0] = tmp;
}

void floorCharacterBiosSetPos(u8* dst, f32* src)
{
    if (dst == NULL) {
        return;
    }
    if (src == NULL) {
        return;
    }
    *(f32*)&dst[0x18] = src[0];
    *(f32*)&dst[0x1C] = src[1];
    *(f32*)&dst[0x20] = src[2];
}

u32 floorCharacterBiosGetTalkSctID(void* ptr)
{
    return ptr != NULL ? *(u32*)((u8*)ptr + 0x14) : 0;
}

u32 floorCharacterBiosGetMoveSctID(void* ptr)
{
    return ptr != NULL ? *(u32*)((u8*)ptr + 0x10) : 0;
}

u32 floorCharacterBiosGetNameID(void* ptr)
{
    return ptr != NULL ? *(u16*)((u8*)ptr + 0x8) : 0;
}

u32 floorCharacterBiosGetTalkWallThrough(u8* ptr)
{
    return ptr != NULL ? (u32)((ptr[0] >> 4) & 1) : 0;
}

u32 floorCharacterBiosGetTalkEndType(u8* ptr)
{
    return ptr != NULL ? (u32)((ptr[1] >> 4) & 3) : 0;
}

u32 floorCharacterBiosGetTalkStartType(u8* ptr)
{
    return ptr != NULL ? (u32)((ptr[1] >> 6) & 3) : 0;
}

u32 floorCharacterBiosGetMoveType(u8* ptr)
{
    return ptr != NULL ? (u32)(ptr[0] & 7) : 0;
}
