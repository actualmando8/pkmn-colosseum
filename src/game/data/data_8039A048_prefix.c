#include "dolphin/types.h"

extern u8 fn_8021B910[];

void* jumptable_8039A048[8] = {
    (void*)(fn_8021B910 + 0x104),
    (void*)(fn_8021B910 + 0xCC),
    (void*)(fn_8021B910 + 0xD4),
    (void*)(fn_8021B910 + 0xDC),
    (void*)(fn_8021B910 + 0xE4),
    (void*)(fn_8021B910 + 0xEC),
    (void*)(fn_8021B910 + 0xF4),
    (void*)(fn_8021B910 + 0xFC),
};
