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
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CA7BC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    /* subi r3, r3, 0x1 */;
    /* subi r4, r4, 0x1 */;
    r6 = r5 + 0x1;
    goto L_800CA7EC;
L_800CA7CC: ;
    r0 = *(u8*)((u8*)r3 + 0x1);
    r5 = *(u8*)((u8*)r4 + 0x1);
    if ((u32)r0 == (u32)r5) goto L_800CA7E4;
    r3 = r0 - r5;
    return;
L_800CA7E4: ;
    if ((u32)r0 == (u32)0x0) goto L_800CA7F4;
L_800CA7EC: ;
    /* subic. r6, r6, 0x1 */;
    if ((u32)r0 != (u32)0x0) goto L_800CA7CC;
L_800CA7F4: ;
    r3 = 0x0;
    return;
}
#pragma pop

/* fn_800CA7FC - 0x800CA7FC | size: 0x128 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CA7FC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r5 = *(u8*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subf. r0, r0, r5 */;
    if ((s32)r0 == (s32)0) goto L_800CA814;
    r3 = r0;
    return;
L_800CA814: ;
    r0 = r4 & 0x3;
    r6 = r3 & 0x3;
    if ((u32)r0 != (u32)r6) goto L_800CA8EC;
    if ((u32)r6 == (u32)0x0) goto L_800CA880;
    if ((u32)r5 != (u32)0x0) goto L_800CA83C;
    r3 = 0x0;
    return;
L_800CA83C: ;
    r0 = 0x3 - r6;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 == (u32)0x0) goto L_800CA878;
L_800CA84C: ;
    r5 = *(u8*)((u8*)r3 + 0x1);
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* subf. r0, r0, r5 */;
    if ((u32)r0 == (u32)0x0) goto L_800CA864;
    r3 = r0;
    return;
L_800CA864: ;
    if ((u32)r5 != (u32)0x0) goto L_800CA874;
    r3 = 0x0;
    return;
L_800CA874: ;
    if (--ctr != 0) goto L_800CA84C;
L_800CA878: ;
    r3 = r3 + 0x1;
    r4 = r4 + 0x1;
L_800CA880: ;
    r7 = *(u32*)((u8*)r3 + 0x0);
    r5 = (0x8081 << 16);
    /* subi r6, r5, 0x7f80 */;
    r8 = *(u32*)((u8*)r4 + 0x0);
    /* subis r5, r7, 0x101 */;
    /* subi r0, r5, 0x101 */;
    /* and. r0, r0, r6 */;
    if ((u32)r5 != (u32)0x0) goto L_800CA8D4;
    goto L_800CA8BC;
L_800CA8A4: ;
    r7 = *(u32*)((u8*)r3 + 0x4);
    r8 = *(u32*)((u8*)r4 + 0x4);
    /* subis r5, r7, 0x101 */;
    /* subi r0, r5, 0x101 */;
    /* and. r0, r0, r6 */;
    if ((u32)r5 != (u32)0x0) goto L_800CA8D4;
L_800CA8BC: ;
    if ((u32)r7 == (u32)r8) goto L_800CA8A4;
    r3 = -0x1;
    if ((u32)r7 <= (u32)r8) return;
    r3 = 0x1;
    return;
L_800CA8D4: ;
    r5 = *(u8*)((u8*)r3 + 0x0);
    r0 = *(u8*)((u8*)r4 + 0x0);
    /* subf. r0, r0, r5 */;
    if ((u32)r7 == (u32)r8) goto L_800CA8EC;
    r3 = r0;
    return;
L_800CA8EC: ;
    if ((u32)r5 != (u32)0x0) goto L_800CA8FC;
    r3 = 0x0;
    return;
L_800CA8FC: ;
    r5 = *(u8*)((u8*)r3 + 0x1);
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* subf. r0, r0, r5 */;
    if ((u32)r5 == (u32)0x0) goto L_800CA914;
    r3 = r0;
    return;
L_800CA914: ;
    if ((u32)r5 != (u32)0x0) goto L_800CA8FC;
    r3 = 0x0;
    return;
}
#pragma pop

/* fn_800CA924 - 0x800CA924 | size: 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CA924(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    /* subi r4, r4, 0x1 */;
    /* subi r6, r3, 0x1 */;
    r5 = r5 + 0x1;
    goto L_800CA95C;
L_800CA934: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    r6 += 1; *(u8*)r6 = r0;
    if ((u32)r0 != (u32)0x0) goto L_800CA95C;
    r0 = 0x0;
    goto L_800CA950;
L_800CA94C: ;
    r6 += 1; *(u8*)r6 = r0;
L_800CA950: ;
    /* subic. r5, r5, 0x1 */;
    if ((u32)r0 != (u32)0x0) goto L_800CA94C;
    return;
L_800CA95C: ;
    /* subic. r5, r5, 0x1 */;
    if ((u32)r0 != (u32)0x0) goto L_800CA934;
    return;
}
#pragma pop

/* fn_800CA968 - 0x800CA968 | size: 0xB8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CA968(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r0 = r3 & 0x3;
    r5 = r4 & 0x3;
    r7 = r3;
    if ((u32)r0 != (u32)r5) goto L_800CA9FC;
    if ((u32)r5 == (u32)0x0) goto L_800CA9C0;
    r0 = *(u8*)((u8*)r4 + 0x0);
    *(u8*)((u8*)r7 + 0x0) = r0;
    if ((u32)r0 == (u32)0x0) return;
    r0 = 0x3 - r5;
    ctr_fn = (void(*)(void))r0;
    if ((u32)r0 == (u32)0x0) goto L_800CA9B8;
L_800CA9A4: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    r7 += 1; *(u8*)r7 = r0;
    if ((u32)r0 == (u32)0x0) return;
    if (--ctr != 0) goto L_800CA9A4;
L_800CA9B8: ;
    r7 = r7 + 0x1;
    r4 = r4 + 0x1;
L_800CA9C0: ;
    r8 = *(u32*)((u8*)r4 + 0x0);
    r5 = (0x8081 << 16);
    /* subi r6, r5, 0x7f80 */;
    /* subis r5, r8, 0x101 */;
    /* subi r0, r5, 0x101 */;
    /* and. r0, r0, r6 */;
    if ((u32)r0 != (u32)0x0) goto L_800CA9FC;
    /* subi r7, r7, 0x4 */;
L_800CA9E0: ;
    r7 += 4; *(u32*)r7 = r8;
    r8 = *(u32*)((u8*)r4 + 0x4);
    /* subis r5, r8, 0x101 */;
    /* subi r0, r5, 0x101 */;
    /* and. r0, r0, r6 */;
    if ((u32)r0 == (u32)0x0) goto L_800CA9E0;
    r7 = r7 + 0x4;
L_800CA9FC: ;
    r0 = *(u8*)((u8*)r4 + 0x0);
    *(u8*)((u8*)r7 + 0x0) = r0;
    if ((u32)r0 == (u32)0x0) return;
L_800CAA0C: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    r7 += 1; *(u8*)r7 = r0;
    if ((u32)r0 != (u32)0x0) goto L_800CAA0C;
    return;
}
#pragma pop

/* fn_800CAA3C - 0x800CAA3C | size: 0x1C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CAA3C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;

    /* subi r4, r4, 0x2 */;
    /* subi r5, r3, 0x2 */;
L_800CAA44: ;
    r0 = *(u16*)((u8*)r4 + 0x2);
    /* sthu r0, 0x2(r5) */;
    if ((u32)r0 != (u32)0x0) goto L_800CAA44;
    return;
}
#pragma pop

