#include "dolphin/types.h"

extern u8 Scb[];
extern u32 WriteSram(u8* dst, u32 addr, u32 len);

void WriteSramCallback(s32 chan, void* context) {
    u8* base = Scb;
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
