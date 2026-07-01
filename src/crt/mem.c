#include "dolphin/types.h"

/*
 * MetroWerks CodeWarrior CRT memory functions for GameCube (PowerPC).
 *
 * These are the standard C library memory manipulation functions as
 * implemented by MetroWerks for the GameCube platform.
 */

/* MSL mem_funcs.c word-copy macros (cast-as-lvalue is a MetroWerks extension). */
#define cps ((unsigned char*)src)
#define cpd ((unsigned char*)dst)
#define lps ((unsigned long*)src)
#define lpd ((unsigned long*)dst)
#define deref_auto_inc(p) *++(p)

/* MSL mem_funcs.c word-copy helpers (memmove dispatches to these by address). */
void fn_800C8240(void* dst, const void* src, size_t n); /* __copy_longs_rev_unaligned */
void fn_800C82EC(void* dst, const void* src, size_t n); /* __copy_longs_unaligned */
void fn_800C83AC(void* dst, const void* src, size_t n); /* __copy_longs_rev_aligned */
void fn_800C8454(void* dst, const void* src, size_t n); /* __copy_longs_aligned */

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

    if (count >= 0x20) {
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
    }

    /* Byte-by-byte fill for small counts or remainder */
    if (count != 0) {
        while (count-- != 0) {
            *dst++ = (u8)v;
        }
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
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void* fn_800C8174(void* dst, const void* src, size_t n) {
    u8* csrc;
    u8* cdst;

    int reverse = (u32)src < (u32)dst;

    if (n >= 32) {
        if (((u32)dst ^ (u32)src) & 3) {
            if (!reverse) {
                fn_800C82EC(dst, src, n);
            } else {
                fn_800C8240(dst, src, n);
            }
        } else {
            if (!reverse) {
                fn_800C8454(dst, src, n);
            } else {
                fn_800C83AC(dst, src, n);
            }
        }

        return dst;
    } else {
        if (!reverse) {
            csrc = ((u8*)src) - 1;
            cdst = ((u8*)dst) - 1;
            n++;

            while (--n > 0) {
                *++cdst = *++csrc;
            }
        } else {
            csrc = (u8*)src + n;
            cdst = (u8*)dst + n;
            n++;

            while (--n > 0) {
                *--cdst = *--csrc;
            }
        }
    }

    return dst;
}


/* fn_800C8240 - 0x800C8240 | size: 0xAC */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void fn_800C8240(void* dst, const void* src, size_t n) {
    u32 i, v1, v2;
    u32 src_offset, left_shift, right_shift;

    cps = ((u8*)src) + n;
    cpd = ((u8*)dst) + n;

    i = ((u32)cpd) & 3;

    if (i) {
        n -= i;

        do
            *--cpd = *--cps;
        while (--i);
    }

    src_offset = ((u32)cps) & 3;

    left_shift = src_offset << 3;
    right_shift = 32 - left_shift;

    cps += 4 - src_offset;

    i = n >> 3;

    v1 = *--lps;

    do {
        v2 = *--lps;
        *--lpd = (v2 << left_shift) | (v1 >> right_shift);
        v1 = *--lps;
        *--lpd = (v1 << left_shift) | (v2 >> right_shift);
    } while (--i);

    if (n & 4) {
        v2 = *--lps;
        *--lpd = (v2 << left_shift) | (v1 >> right_shift);
    }

    n &= 3;

    if (n) {
        cps += src_offset;
        do
            *--cpd = *--cps;
        while (--n);
    }

    return;
}


/* fn_800C82EC - 0x800C82EC | size: 0xC0 */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void fn_800C82EC(void* dst, const void* src, size_t n) {
    u32 i, v1, v2;
    u32 src_offset, left_shift, right_shift;

    i = (-(u32)dst) & 3;

    cps = ((u8*)src) - 1;
    cpd = ((u8*)dst) - 1;

    if (i) {
        n -= i;

        do
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        while (--i);
    }

    src_offset = ((u32)(cps + 1)) & 3;

    left_shift = src_offset << 3;
    right_shift = 32 - left_shift;

    cps -= src_offset;

    lps = ((u32*)(cps + 1)) - 1;
    lpd = ((u32*)(cpd + 1)) - 1;

    i = n >> 3;

    v1 = deref_auto_inc(lps);

    do {
        v2 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
        v1 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v2 << left_shift) | (v1 >> right_shift);
    } while (--i);

    if (n & 4) {
        v2 = deref_auto_inc(lps);
        deref_auto_inc(lpd) = (v1 << left_shift) | (v2 >> right_shift);
    }

    cps = ((u8*)(lps + 1)) - 1;
    cpd = ((u8*)(lpd + 1)) - 1;

    n &= 3;

    if (n) {
        cps -= 4 - src_offset;
        do
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        while (--n);
    }

    return;
}


/* fn_800C83AC - 0x800C83AC | size: 0xA8 */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void fn_800C83AC(void* dst, const void* src, size_t n) {
    u32 i;
    u32 v1, v2;

    cpd = ((u8*)dst) + n;
    cps = ((u8*)src) + n;

    i = ((u32)cpd) & 3;

    if (i) {
        n -= i;

        do
            *--cpd = *--cps;
        while (--i);
    }

    i = n >> 5;

    if (i)
        do {
            v1 = *--lps;
            v2 = *--lps;
            *--lpd = v1;
            v1 = *--lps;
            *--lpd = v2;
            v2 = *--lps;
            *--lpd = v1;
            v1 = *--lps;
            *--lpd = v2;
            v2 = *--lps;
            *--lpd = v1;
            v1 = *--lps;
            *--lpd = v2;
            v2 = *--lps;
            *--lpd = v1;
            *--lpd = v2;
        } while (--i);

    i = (n & 31) >> 2;

    if (i)
        do
            *--lpd = *--lps;
        while (--i);

    n &= 3;

    if (n)
        do
            *--cpd = *--cps;
        while (--n);

    return;
}


/* fn_800C8454 - 0x800C8454 | size: 0xBC */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void fn_800C8454(void* dst, const void* src, size_t n) {
    u32 i;

    i = (-(u32)dst) & 3;

    cps = ((u8*)src) - 1;
    cpd = ((u8*)dst) - 1;

    if (i) {
        n -= i;

        do
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        while (--i);
    }

    lps = ((u32*)(cps + 1)) - 1;
    lpd = ((u32*)(cpd + 1)) - 1;

    i = n >> 5;

    if (i)
        do {
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
            deref_auto_inc(lpd) = deref_auto_inc(lps);
        } while (--i);

    i = (n & 31) >> 2;

    if (i)
        do
            deref_auto_inc(lpd) = deref_auto_inc(lps);
        while (--i);

    cps = ((u8*)(lps + 1)) - 1;
    cpd = ((u8*)(lpd + 1)) - 1;

    n &= 3;

    if (n)
        do
            deref_auto_inc(cpd) = deref_auto_inc(cps);
        while (--n);

    return;
}


