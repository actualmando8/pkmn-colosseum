#include "dolphin/types.h"

extern u8 fn_8021D40C[];
extern u8 fn_8021FAD4[];
extern u8 fn_80220868[];

void* jumptable_8039A088[8] = {
    (void*)(fn_8021D40C + 0x6C),
    (void*)(fn_8021D40C + 0x78),
    (void*)(fn_8021D40C + 0x25C),
    (void*)(fn_8021D40C + 0x154),
    (void*)(fn_8021D40C + 0x194),
    (void*)(fn_8021D40C + 0x25C),
    (void*)(fn_8021D40C + 0x1CC),
    (void*)(fn_8021D40C + 0x24C),
};

void* jumptable_8039A0A8[18] = {
    (void*)(fn_8021FAD4 + 0x1C0),
    (void*)(fn_8021FAD4 + 0x290),
    (void*)(fn_8021FAD4 + 0x370),
    (void*)(fn_8021FAD4 + 0x38C),
    (void*)(fn_8021FAD4 + 0x3B4),
    (void*)(fn_8021FAD4 + 0x3DC),
    (void*)(fn_8021FAD4 + 0x3F8),
    (void*)(fn_8021FAD4 + 0x4F0),
    (void*)(fn_8021FAD4 + 0x518),
    (void*)(fn_8021FAD4 + 0x540),
    (void*)(fn_8021FAD4 + 0x55C),
    (void*)(fn_8021FAD4 + 0x5C4),
    (void*)(fn_8021FAD4 + 0x664),
    (void*)(fn_8021FAD4 + 0x6E4),
    (void*)(fn_8021FAD4 + 0x70C),
    (void*)(fn_8021FAD4 + 0xA68),
    (void*)(fn_8021FAD4 + 0xAF0),
    (void*)(fn_8021FAD4 + 0xC08),
};

void* jumptable_8039A0F0[8] = {
    (void*)(fn_80220868 + 0x244),
    (void*)(fn_80220868 + 0x20C),
    (void*)(fn_80220868 + 0x214),
    (void*)(fn_80220868 + 0x21C),
    (void*)(fn_80220868 + 0x224),
    (void*)(fn_80220868 + 0x22C),
    (void*)(fn_80220868 + 0x234),
    (void*)(fn_80220868 + 0x23C),
};

void* jumptable_8039A110[8] = {
    (void*)(fn_80220868 + 0xEC),
    (void*)(fn_80220868 + 0xB4),
    (void*)(fn_80220868 + 0xBC),
    (void*)(fn_80220868 + 0xC4),
    (void*)(fn_80220868 + 0xCC),
    (void*)(fn_80220868 + 0xD4),
    (void*)(fn_80220868 + 0xDC),
    (void*)(fn_80220868 + 0xE4),
};
