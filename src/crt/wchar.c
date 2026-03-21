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

/*
 * wcsncmp - Compare at most n wide characters.
 *
 * Compares wide character strings byte-by-byte (since GC locale
 * truncates to single bytes). Returns 1 if s1 > s2, -1 if s1 < s2,
 * 0 if equal up to n characters.
 *
 * 0x800C80D0 | size: 0x4C
 */
s32 fn_800C80D0(const wchar_t* s1, const wchar_t* s2, u32 n) {
    u32 i;
    for (i = 0; i < n; i++) {
        u8 c1 = (u8)(s1[i] & 0xFF);
        u8 c2 = (u8)(s2[i] & 0xFF);
        if (c1 != c2) {
            if (c1 >= c2) { return 1; }
            return -1;
        }
    }
    return 0;
}

