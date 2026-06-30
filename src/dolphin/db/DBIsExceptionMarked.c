#include "dolphin/types.h"

extern u32* __DBInterface;

s32 __DBIsExceptionMarked(u8 exceptionType) {
    u32 mask = 1 << ((u32)exceptionType & 0xFF);
    return (s32)(__DBInterface[1] & mask);
}
