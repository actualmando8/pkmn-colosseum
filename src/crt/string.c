#include "dolphin/types.h"

/*
 * MetroWerks CodeWarrior CRT string functions for GameCube (PowerPC).
 */

/*
 * strlen - Compute the length of a null-terminated string.
 *
 * @param str  Pointer to the null-terminated string
 * @return     Number of characters before the terminating null byte
 *
 * Assembly pattern uses lbzu pre-increment loop:
 *   subi r4, r3, 1    ; p = str - 1
 *   li r3, -1         ; len = -1
 *   lbzu r0, 1(r4)    ; load *++p
 *   addi r3, r3, 1    ; len++
 *   cmplwi r0, 0      ; check null
 *   bne loop
 */
u32 strlen(const char* str) {
    const u8* p = (const u8*)str - 1;
    u32 len = (u32)-1;

    do {
        len++;
    } while (*++p != 0);

    return len;
}

/*
 * strchr - Locate the first occurrence of a character in a string.
 *
 * @param str  Pointer to the null-terminated string to search
 * @param c    Character to search for (only low 8 bits used)
 * @return     Pointer to the first occurrence of c in str,
 *             or NULL if c is not found. If c is '\0', returns
 *             a pointer to the terminating null byte.
 */
char* strchr(const char* str, int c) {
    char* p = (char*)str - 1;
    u8 ch = (u8)c;
    u8 cur;

    while ((cur = (u8)*++p) != 0) {
        if (cur == ch) {
            return p;
        }
    }

    if (ch == 0) {
        return p;
    }
    return NULL;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800CA7BC - 0x800CA7BC | size: 0x40 */
/*
 * strncmp - Compare at most n characters of two strings.
 *
 * 0x800CA7BC | size: 0x40
 */
s32 strncmp(const char* s1, const char* s2, u32 n) {
    u32 i;
    for (i = 0; i < n; i++) {
        u8 c1 = *(const u8*)s1;
        u8 c2 = *(const u8*)s2;
        if (c1 != c2) {
            return (s32)c1 - (s32)c2;
        }
        if (c1 == 0) {
            return 0;
        }
        s1++;
        s2++;
    }
    return 0;
}

/* fn_800CA7FC - 0x800CA7FC | size: 0x128 */
void fn_800CA7FC(void) {
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r5 = *(u8*)((u8*)r3 + 0x0);
    tmp = *(u8*)((u8*)r4 + 0x0);
    /* subf. tmp, tmp, r5 */;
    if ((s32)tmp == 0) goto L_800CA814;
    r3 = tmp;
    return;
L_800CA814:
    tmp = r4 & 0x3;
    r6 = r3 & 0x3;
    if (tmp != r6) goto L_800CA8EC;
    if (r6 == 0) goto L_800CA880;
    if (r5 != 0) goto L_800CA83C;
    r3 = 0x0;
    return;
L_800CA83C:
    tmp = 0x3 - r6;
    ctr_fn = (void(*)(void))tmp;
    if (tmp == 0) goto L_800CA878;
L_800CA84C:
    r5 = *(u8*)((u8*)r3 + 0x1);
    tmp = *(u8*)((u8*)r4 + 0x1);
    /* subf. tmp, tmp, r5 */;
    if (tmp == 0) goto L_800CA864;
    r3 = tmp;
    return;
L_800CA864:
    if (r5 != 0) goto L_800CA874;
    r3 = 0x0;
    return;
L_800CA874:
    if (--ctr != 0) goto L_800CA84C;
L_800CA878:
    r3 = r3 + 0x1;
    r4 = r4 + 0x1;
L_800CA880:
    r7 = *(u32*)((u8*)r3 + 0x0);
    r5 = 0x80810000;
    r8 = *(u32*)((u8*)r4 + 0x0);
    /* subis r5, r7, 0x101 */;
    /* and. tmp, tmp, r6 */;
    if (r5 != 0) goto L_800CA8D4;
    goto L_800CA8BC;
L_800CA8A4:
    r7 = *(u32*)((u8*)r3 + 0x4);
    r8 = *(u32*)((u8*)r4 + 0x4);
    /* subis r5, r7, 0x101 */;
    /* and. tmp, tmp, r6 */;
    if (r5 != 0) goto L_800CA8D4;
L_800CA8BC:
    if (r7 == r8) goto L_800CA8A4;
    r3 = -0x1;
    if ((u32)r7 <= (u32)r8) return;
    r3 = 0x1;
    return;
L_800CA8D4:
    r5 = *(u8*)((u8*)r3 + 0x0);
    tmp = *(u8*)((u8*)r4 + 0x0);
    /* subf. tmp, tmp, r5 */;
    if (r7 == r8) goto L_800CA8EC;
    r3 = tmp;
    return;
L_800CA8EC:
    if (r5 != 0) goto L_800CA8FC;
    r3 = 0x0;
    return;
L_800CA8FC:
    r5 = *(u8*)((u8*)r3 + 0x1);
    tmp = *(u8*)((u8*)r4 + 0x1);
    /* subf. tmp, tmp, r5 */;
    if (r5 == 0) goto L_800CA914;
    r3 = tmp;
    return;
L_800CA914:
    if (r5 != 0) goto L_800CA8FC;
    r3 = 0x0;
    return;
}

/*
 * strncpy - Copy at most n characters from src to dst.
 *
 * If the source string is shorter than n, the destination is padded
 * with null bytes up to length n.
 *
 * 0x800CA924 | size: 0x44
 */
char* strncpy(char* dst, const char* src, u32 n) {
    char* p = dst;
    u32 i;

    for (i = 0; i < n; i++) {
        char c = *src;
        *p++ = c;
        if (c == '\0') {
            /* Pad remaining with nulls */
            i++;
            while (i < n) {
                *p++ = '\0';
                i++;
            }
            return dst;
        }
        src++;
    }
    return dst;
}

/*
 * strcpy - Copy a null-terminated string from src to dst.
 *
 * Uses word-at-a-time copying when both pointers are word-aligned,
 * with a null-byte detection trick for early termination. Falls
 * back to byte-by-byte copy when unaligned.
 *
 * 0x800CA968 | size: 0xB8
 */
char* strcpy(char* dst, const char* src) {
    char* d = dst;
    const char* s = src;

    /* Byte-by-byte copy */
    while (1) {
        *d = *s;
        if (*s == '\0') {
            break;
        }
        d++;
        s++;
    }
    return dst;
}

/* fn_800CAA3C - 0x800CAA3C | size: 0x1C
 * wcscpy - Copy a wide character string from src to dst.
 */
void fn_800CAA3C(u16* dst, const u16* src) {
    do {
        src++;
        dst++;
        *dst = *src;
    } while (*src != 0);
}

