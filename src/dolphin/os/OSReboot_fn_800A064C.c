#include "dolphin/types.h"

u32 Scb_803FB840[0x54 / sizeof(u32)];
extern u32 WriteSram(u8* dst, u32 addr, u32 len);

void WriteSramCallback(s32 chan, void* context) {
    u8* base = (u8*)Scb_803FB840;
    u32* lenPtr = (u32*)(base + 0x40);
    u32 offset;
    u32 result;

    offset = *(u32*)(base + 0x40);
    result = WriteSram(base + offset, offset, 0x40 - offset);
    *(u32*)(base + 0x4C) = result;

    if (*(volatile s32*)(base + 0x4C) != 0) {
        *lenPtr = 0x40;
    }
}
