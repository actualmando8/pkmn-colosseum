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
void __copy_longs_rev_unaligned(void* dst, const void* src, size_t n); /* __copy_longs_rev_unaligned */
void __copy_longs_unaligned(void* dst, const void* src, size_t n); /* __copy_longs_unaligned */
void __copy_longs_rev_aligned(void* dst, const void* src, size_t n); /* __copy_longs_rev_aligned */
void __copy_longs_aligned(void* dst, const void* src, size_t n); /* __copy_longs_aligned */

/* memmove - 0x800C8174 | size: 0xCC */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void* memmove(void* dst, const void* src, size_t n) {
    u8* csrc;
    u8* cdst;

    int reverse = (u32)src < (u32)dst;

    if (n >= 32) {
        if (((u32)dst ^ (u32)src) & 3) {
            if (!reverse) {
                __copy_longs_unaligned(dst, src, n);
            } else {
                __copy_longs_rev_unaligned(dst, src, n);
            }
        } else {
            if (!reverse) {
                __copy_longs_aligned(dst, src, n);
            } else {
                __copy_longs_rev_aligned(dst, src, n);
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


/* __copy_longs_rev_unaligned - 0x800C8240 | size: 0xAC */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void __copy_longs_rev_unaligned(void* dst, const void* src, size_t n) {
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


/* __copy_longs_unaligned - 0x800C82EC | size: 0xC0 */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void __copy_longs_unaligned(void* dst, const void* src, size_t n) {
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


/* __copy_longs_rev_aligned - 0x800C83AC | size: 0xA8 */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void __copy_longs_rev_aligned(void* dst, const void* src, size_t n) {
    u32 i;

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
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
            *--lpd = *--lps;
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


/* __copy_longs_aligned - 0x800C8454 | size: 0xBC */
/* MSL mem.c/mem_funcs.c (zeldaret/tww); CW 2.0. Verified 100%. */
void __copy_longs_aligned(void* dst, const void* src, size_t n) {
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
