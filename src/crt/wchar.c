#include "dolphin/types.h"

/*
 * wchar.c - Wide character conversion functions.
 *
 * Provides wcstombs for the MetroWerks CRT. This is used by
 * the printf formatter when handling wide string conversions.
 */

typedef unsigned short wchar_t;

/*
 * wcstombs - Convert a wide character string to multibyte.
 *
 * Converts at most n bytes worth of wide characters from src
 * to multibyte characters in dst. If dst is NULL, returns
 * the number of bytes that would be needed.
 *
 * On GameCube, wide characters are simply truncated to their
 * low byte since the locale is always "C" (ASCII).
 *
 * Size: 0x118 bytes at 0x800C7FB8.
 */
u32 wcstombs(char* dst, const wchar_t* src, u32 n) {
    u32 count = 0;
    const wchar_t* p = src;

    if (dst == NULL) {
        /* Just count the needed bytes */
        while (*p != 0) {
            count++;
            p++;
        }
        return count;
    }

    while (n > 0 && *p != 0) {
        *dst = (char)(*p & 0xFF);
        dst++;
        p++;
        count++;
        n--;
    }

    if (n > 0) {
        *dst = '\0';
    }

    return count;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C80D0 - 0x800C80D0 | size: 0x4C */
void fn_800C80D0(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r4 = r5 + 0x1;
    goto L_800C810C;
L_800C80E0:
    r3 = *(u8*)((u8*)r6 + 0x1);
    tmp = *(u8*)((u8*)r7 + 0x1);
    if (r3 == tmp) goto L_800C810C;
    r4 = *(u8*)((u8*)r6 + 0x0);
    r3 = 0x1;
    tmp = *(u8*)((u8*)r7 + 0x0);
    if ((u32)r4 >= (u32)tmp) return;
    r3 = -0x1;
    return;
L_800C810C:
    /* subic. r4, r4, 0x1 */;
    if (r4 != tmp) goto L_800C80E0;
    r3 = 0x0;
    return;
}

