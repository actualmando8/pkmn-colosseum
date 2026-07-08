#include "dolphin/types.h"

/*
 * memcmp - Compare two byte buffers.
 *
 * 0x800C80D0 | size: 0x4C
 */
s32 memcmp(const void* s1, const void* s2, u32 n) {
    const u8* p1;
    const u8* p2;

    for (p1 = (const u8*)s1 - 1, p2 = (const u8*)s2 - 1, n++; --n != 0;) {
        if (*++p1 != *++p2) {
            return (*p1 < *p2) ? -1 : 1;
        }
    }

    return 0;
}
