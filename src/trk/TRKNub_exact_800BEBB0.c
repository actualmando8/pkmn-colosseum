/**
 * @file TRKNub_exact_800BEBB0.c
 * @brief Exact pure-C MetroTRK byte append helper.
 */
#include "dolphin/types.h"

s32 TRKAppendBuffer_ui8(u8* buffer, u8* source, s32 count)
{
    u8 byte;
    s32 i = 0;
    s32 error = 0;

    while (error == 0 && i < count) {
        u32 position = *(u32*)(buffer + 0xC);
        byte = *source;
        if (position >= 0x880) {
            position = 0x301;
        } else {
            *(u32*)(buffer + 0xC) = position + 1;
            *(buffer + 0x10 + position) = byte;
            position = 0;
            *(u32*)(buffer + 0x8) = *(u32*)(buffer + 0x8) + 1;
        }
        error = position;
        i = i + 1;
        source = source + 1;
    }

    return error;
}
