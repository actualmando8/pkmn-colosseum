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
