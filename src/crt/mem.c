#include "dolphin/types.h"

/*
 * MetroWerks CodeWarrior CRT memory functions for GameCube (PowerPC).
 *
 * These are the standard C library memory manipulation functions as
 * implemented by MetroWerks for the GameCube platform.
 */

void __fill_mem(void* dest, int val, u32 count);

/*
 * memset - Fill a block of memory with a byte value.
 *
 * @param dest  Destination buffer
 * @param val   Byte value to fill with (only low 8 bits used)
 * @param count Number of bytes to fill
 * @return      The original dest pointer
 *
 * This is a wrapper around __fill_mem that preserves and returns
 * the original destination pointer per the C standard.
 */
void* memset(void* dest, int val, u32 count) {
    __fill_mem(dest, val, count);
    return dest;
}

/*
 * __fill_mem - Internal memory fill implementation.
 *
 * @param dest  Destination buffer
 * @param val   Byte value to fill with (only low 8 bits used)
 * @param count Number of bytes to fill
 *
 * Uses an optimized fill strategy:
 * - For small fills (< 32 bytes), fills byte by byte.
 * - For larger fills, first aligns the destination to a 4-byte boundary,
 *   then broadcasts the fill byte across a 32-bit word and fills in
 *   32-byte (8-word) blocks, then remaining words, then remaining bytes.
 */
void __fill_mem(void* dest, int val, u32 count) {
    u8* dst = (u8*)dest;
    u32 v = (u8)val;
    u32 wordVal;
    u32 numBlocks;
    u32 numWords;
    u32 align;
    u32 i;

    if (count < 0x20) {
        /* Small fill: byte by byte */
        goto byte_fill;
    }

    /* Align to 4-byte boundary */
    align = (-(u32)dst) & 3;
    if (align != 0) {
        count -= align;
        while (align-- != 0) {
            *dst++ = (u8)v;
        }
    }

    /* Build 32-bit fill value by replicating byte across word */
    if (v != 0) {
        wordVal = (v << 24) | (v << 16) | (v << 8) | v;
    } else {
        wordVal = 0;
    }

    /* Fill in 32-byte (8-word) blocks */
    numBlocks = count >> 5;
    {
        u32* wp = (u32*)(dst - 4);
        if (numBlocks != 0) {
            do {
                wp[1] = wordVal;
                wp[2] = wordVal;
                wp[3] = wordVal;
                wp[4] = wordVal;
                wp[5] = wordVal;
                wp[6] = wordVal;
                wp[7] = wordVal;
                *(wp += 8) = wordVal;
            } while (--numBlocks != 0);
        }

        /* Fill remaining whole words (count bits [2:4]) */
        numWords = (count & 0x1C) >> 2;
        if (numWords != 0) {
            do {
                *(wp += 1) = wordVal;
            } while (--numWords != 0);
        }

        dst = (u8*)(wp + 1);
    }

    /* Remaining bytes (count & 3) */
    count &= 3;

byte_fill:
    if (count == 0) {
        return;
    }
    while (count-- != 0) {
        *dst++ = (u8)v;
    }
}

/*
 * memcpy - Copy a block of memory, handling overlapping regions.
 *
 * @param dest  Destination buffer
 * @param src   Source buffer
 * @param count Number of bytes to copy
 * @return      The original dest pointer
 *
 * If src >= dest, copies forward (low to high address).
 * If src < dest, copies backward (high to low address) to handle
 * the case where the source and destination regions overlap.
 */
void* memcpy(void* dest, const void* src, u32 count) {
    const u8* s = (const u8*)src;
    u8* d = (u8*)dest;

    if (s >= d) {
        /* Forward copy */
        s--;
        d--;
        count++;
        while (--count != 0) {
            *++d = *++s;
        }
    } else {
        /* Backward copy (handles overlapping where dest > src) */
        s += count;
        d += count;
        count++;
        while (--count != 0) {
            *--d = *--s;
        }
    }
    return dest;
}

/*
 * __memrchr - Search for a byte value in memory, scanning backwards.
 *
 * @param src  Start of the memory region
 * @param val  Byte value to search for (only low 8 bits used)
 * @param count Number of bytes to search
 * @return     Pointer to the last occurrence of val, or NULL if not found
 */
void* __memrchr(const void* src, int val, u32 count) {
    u8 v = (u8)val;
    const u8* p = (const u8*)src + count;

    count++;
    while (--count != 0) {
        if (*--p == v) {
            return (void*)p;
        }
    }
    return NULL;
}

/*
 * memchr - Search for a byte value in memory, scanning forwards.
 *
 * @param src   Start of the memory region
 * @param val   Byte value to search for (only low 8 bits used)
 * @param count Number of bytes to search
 * @return      Pointer to the first occurrence of val, or NULL if not found
 */
void* memchr(const void* src, int val, u32 count) {
    u8 v = (u8)val;
    const u8* p = (const u8*)src - 1;

    count++;
    while (--count != 0) {
        if (*++p == v) {
            return (void*)p;
        }
    }
    return NULL;
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C8174 - 0x800C8174 | size: 0xCC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8174(void) {
    extern void fn_800C8240();
    extern void fn_800C82EC();
    extern void fn_800C83AC();
    extern void fn_800C8454();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r31 = r3;
    r6 = r31 ^ r4;
    r0 = __cntlzw(r6);
    r0 = r31 << r0;
    r7 = (u32)r0 >> 31;
    if ((u32)r5 < (u32)0x20) goto L_800C81DC;
    r0 = r6 & 0x3;
    if ((u32)r5 == (u32)0x20) goto L_800C81C0;
    if ((s32)r7 != (s32)0x0) goto L_800C81B8;
    fn_800C82EC();
    goto L_800C81D4;
L_800C81B8: ;
    fn_800C8240();
    goto L_800C81D4;
L_800C81C0: ;
    if ((s32)r7 != (s32)0x0) goto L_800C81D0;
    fn_800C8454();
    goto L_800C81D4;
L_800C81D0: ;
    fn_800C83AC();
L_800C81D4: ;
    r3 = r31;
    goto L_800C822C;
L_800C81DC: ;
    if ((s32)r7 != (s32)0x0) goto L_800C8208;
    /* subi r3, r4, 0x1 */;
    /* subi r4, r31, 0x1 */;
    r5 = r5 + 0x1;
    goto L_800C81FC;
L_800C81F4: ;
    r0 = *(u8*)((u8*)r3 + 0x1);
    r4 += 1; *(u8*)r4 = r0;
L_800C81FC: ;
    /* subic. r5, r5, 0x1 */;
    if ((s32)r7 != (s32)0x0) goto L_800C81F4;
    goto L_800C8228;
L_800C8208: ;
    r3 = r4 + r5;
    r4 = r31 + r5;
    r5 = r5 + 0x1;
    goto L_800C8220;
L_800C8218: ;
    r0 = *(u8*)((u8*)r3 + (-1));
    r4 += -1; *(u8*)r4 = r0;
L_800C8220: ;
    /* subic. r5, r5, 0x1 */;
    if ((s32)r7 != (s32)0x0) goto L_800C8218;
L_800C8228: ;
    r3 = r31;
L_800C822C: ;
    r31 = *(u32*)(sp + 0xC);
    return;
}
#pragma pop

/* fn_800C8240 - 0x800C8240 | size: 0xAC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8240(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r11 = r3 + r5;
    r10 = r4 + r5;
    r3 = r11 & 0x3;
    if ((s32)r0 == (s32)0) goto L_800C8264;
    r5 = r5 - r3;
L_800C8254: ;
    r0 = *(u8*)((u8*)r10 + (-1));
    /* subic. r3, r3, 0x1 */;
    r11 += -1; *(u8*)r11 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8254;
L_800C8264: ;
    /* clrlslwi r8, r10, 30, 3 */;
    r7 = r10 & 0x3;
    r9 = 0x20 - r8;
    r6 = (u32)r5 >> 3;
    r0 = 0x4 - r7;
    r10 = r10 + r0;
    r4 = *(u32*)((u8*)r10 + (-4));
L_800C8280: ;
    r0 = *(u32*)((u8*)r10 + (-4));
    r3 = (u32)r4 >> r9;
    /* subic. r6, r6, 0x1 */;
    r4 = r0 << r8;
    r0 = (u32)r0 >> r9;
    r3 = r4 | r3;
    *(u32*)((u8*)r11 + (-4)) = r3;
    r4 = *(u32*)((u8*)r10 + (-8));
    r3 = r4 << r8;
    r0 = r3 | r0;
    r11 += -8; *(u32*)r11 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8280;
    r0 = r5 & 0x00000004;
    if ((s32)r0 == (s32)0) goto L_800C82CC;
    r3 = *(u32*)((u8*)r10 + (-4));
    r0 = (u32)r4 >> r9;
    r3 = r3 << r8;
    r0 = r3 | r0;
    r11 += -4; *(u32*)r11 = r0;
L_800C82CC: ;
    r5 = r5 & 0x3;
    if ((s32)r0 == (s32)0) return;
    r10 = r10 + r7;
L_800C82D8: ;
    r0 = *(u8*)((u8*)r10 + (-1));
    /* subic. r5, r5, 0x1 */;
    r11 += -1; *(u8*)r11 = r0;
    if ((s32)r0 != (s32)0) goto L_800C82D8;
    return;
}
#pragma pop

/* fn_800C82EC - 0x800C82EC | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C82EC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;

    r0 = -r3;
    /* subi r8, r4, 0x1 */;
    r6 = r0 & 0x3;
    /* subi r3, r3, 0x1 */;
    if ((s32)r0 == (s32)0) goto L_800C8314;
    r5 = r5 - r6;
L_800C8304: ;
    r0 = *(u8*)((u8*)r8 + 0x1);
    /* subic. r6, r6, 0x1 */;
    r3 += 1; *(u8*)r3 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8304;
L_800C8314: ;
    r0 = r8 + 0x1;
    /* subi r6, r3, 0x3 */;
    r9 = r0 & 0x3;
    r7 = (u32)r5 >> 3;
    r8 = r8 - r9;
    /* clrlslwi r10, r0, 30, 3 */;
    r4 = *(u32*)((u8*)r8 + 0x1);
    r11 = 0x20 - r10;
L_800C8334: ;
    r3 = *(u32*)((u8*)r8 + 0x4);
    r4 = r4 << r10;
    /* subic. r7, r7, 0x1 */;
    r0 = (u32)r3 >> r11;
    r3 = r3 << r10;
    r0 = r4 | r0;
    *(u32*)((u8*)r6 + 0x4) = r0;
    r4 = *(u32*)((u8*)r8 + 0x8);
    r0 = (u32)r4 >> r11;
    r0 = r3 | r0;
    r6 += 8; *(u32*)r6 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8334;
    r0 = r5 & 0x00000004;
    if ((s32)r0 == (s32)0) goto L_800C8380;
    r0 = *(u32*)((u8*)r8 + 0x4);
    r3 = r4 << r10;
    r0 = (u32)r0 >> r11;
    r0 = r3 | r0;
    r6 += 4; *(u32*)r6 = r0;
L_800C8380: ;
    r5 = r5 & 0x3;
    r4 = r8 + 0x3;
    r3 = r6 + 0x3;
    if ((s32)r0 == (s32)0) return;
    r0 = 0x4 - r9;
    r4 = r4 - r0;
L_800C8398: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* subic. r5, r5, 0x1 */;
    r3 += 1; *(u8*)r3 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8398;
    return;
}
#pragma pop

/* fn_800C83AC - 0x800C83AC | size: 0xA8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C83AC(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r7 = r3 + r5;
    r6 = r4 + r5;
    r3 = r7 & 0x3;
    if ((s32)r0 == (s32)0) goto L_800C83D0;
    r5 = r5 - r3;
L_800C83C0: ;
    r0 = *(u8*)((u8*)r6 + (-1));
    /* subic. r3, r3, 0x1 */;
    r7 += -1; *(u8*)r7 = r0;
    if ((s32)r0 != (s32)0) goto L_800C83C0;
L_800C83D0: ;
    /* srwi. r4, r5, 5 */;
    if ((s32)r0 == (s32)0) goto L_800C8420;
L_800C83D8: ;
    r3 = *(u32*)((u8*)r6 + (-4));
    /* subic. r4, r4, 0x1 */;
    r0 = *(u32*)((u8*)r6 + (-8));
    *(u32*)((u8*)r7 + (-4)) = r3;
    r3 = *(u32*)((u8*)r6 + (-12));
    *(u32*)((u8*)r7 + (-8)) = r0;
    r0 = *(u32*)((u8*)r6 + (-16));
    *(u32*)((u8*)r7 + (-12)) = r3;
    r3 = *(u32*)((u8*)r6 + (-20));
    *(u32*)((u8*)r7 + (-16)) = r0;
    r0 = *(u32*)((u8*)r6 + (-24));
    *(u32*)((u8*)r7 + (-20)) = r3;
    r3 = *(u32*)((u8*)r6 + (-28));
    *(u32*)((u8*)r7 + (-24)) = r0;
    r0 = *(u32*)((u8*)r6 + (-32));
    *(u32*)((u8*)r7 + (-28)) = r3;
    r7 += -32; *(u32*)r7 = r0;
    if ((s32)r0 != (s32)0) goto L_800C83D8;
L_800C8420: ;
    /* extrwi. r3, r5, 3, 27 */;
    if ((s32)r0 == (s32)0) goto L_800C8438;
L_800C8428: ;
    r0 = *(u32*)((u8*)r6 + (-4));
    /* subic. r3, r3, 0x1 */;
    r7 += -4; *(u32*)r7 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8428;
L_800C8438: ;
    r5 = r5 & 0x3;
    if ((s32)r0 == (s32)0) return;
L_800C8440: ;
    r0 = *(u8*)((u8*)r6 + (-1));
    /* subic. r5, r5, 0x1 */;
    r7 += -1; *(u8*)r7 = r0;
    if ((s32)r0 != (s32)0) goto L_800C8440;
    return;
}
#pragma pop

/* fn_800C8454 - 0x800C8454 | size: 0xBC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8454(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;

    r0 = -r3;
    /* subi r4, r4, 0x1 */;
    r6 = r0 & 0x3;
    /* subi r3, r3, 0x1 */;
    if ((s32)r0 == (s32)0) goto L_800C847C;
    r5 = r5 - r6;
L_800C846C: ;
    r0 = *(u8*)((u8*)r4 + 0x1);
    /* subic. r6, r6, 0x1 */;
    r3 += 1; *(u8*)r3 = r0;
    if ((s32)r0 != (s32)0) goto L_800C846C;
L_800C847C: ;
    /* srwi. r6, r5, 5 */;
    /* subi r7, r4, 0x3 */;
    /* subi r4, r3, 0x3 */;
    if ((s32)r0 == (s32)0) goto L_800C84D4;
L_800C848C: ;
    r3 = *(u32*)((u8*)r7 + 0x4);
    /* subic. r6, r6, 0x1 */;
    r0 = *(u32*)((u8*)r7 + 0x8);
    *(u32*)((u8*)r4 + 0x4) = r3;
    r3 = *(u32*)((u8*)r7 + 0xC);
    *(u32*)((u8*)r4 + 0x8) = r0;
    r0 = *(u32*)((u8*)r7 + 0x10);
    *(u32*)((u8*)r4 + 0xC) = r3;
    r3 = *(u32*)((u8*)r7 + 0x14);
    *(u32*)((u8*)r4 + 0x10) = r0;
    r0 = *(u32*)((u8*)r7 + 0x18);
    *(u32*)((u8*)r4 + 0x14) = r3;
    r3 = *(u32*)((u8*)r7 + 0x1C);
    *(u32*)((u8*)r4 + 0x18) = r0;
    r0 = *(u32*)((u8*)r7 + 0x20);
    *(u32*)((u8*)r4 + 0x1C) = r3;
    r4 += 32; *(u32*)r4 = r0;
    if ((s32)r0 != (s32)0) goto L_800C848C;
L_800C84D4: ;
    /* extrwi. r3, r5, 3, 27 */;
    if ((s32)r0 == (s32)0) goto L_800C84EC;
L_800C84DC: ;
    r0 = *(u32*)((u8*)r7 + 0x4);
    /* subic. r3, r3, 0x1 */;
    r4 += 4; *(u32*)r4 = r0;
    if ((s32)r0 != (s32)0) goto L_800C84DC;
L_800C84EC: ;
    r5 = r5 & 0x3;
    r6 = r7 + 0x3;
    r3 = r4 + 0x3;
    if ((s32)r0 == (s32)0) return;
L_800C84FC: ;
    r0 = *(u8*)((u8*)r6 + 0x1);
    /* subic. r5, r5, 0x1 */;
    r3 += 1; *(u8*)r3 = r0;
    if ((s32)r0 != (s32)0) goto L_800C84FC;
    return;
}
#pragma pop

