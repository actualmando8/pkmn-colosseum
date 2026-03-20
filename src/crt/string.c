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
