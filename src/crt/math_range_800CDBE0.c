/**
 * @file math_range_800CDBE0.c
 * @brief MSL fdlibm cosine, floor, decomposition, and scaling routines.
 */

#include "dolphin/types.h"

typedef union DoubleShape {
    f64 value;
    struct {
        u32 hi;
        u32 lo;
    } parts;
} DoubleShape;

extern s32 __ieee754_rem_pio2(f64 x, f64* y);
extern f64 __kernel_cos(f64 x, f64 y);
extern f64 __kernel_sin(f64 x, f64 y, s32 iy);
extern f64 copysign(f64 x, f64 y);

extern const f64 lbl_8047C910;
extern const f64 lbl_8047C918;
extern const f64 lbl_8047C920;
extern const f64 lbl_8047C928;
extern const f64 lbl_8047C930;
extern const f64 lbl_8047C938;
extern const f64 lbl_8047C940;
extern const f64 lbl_8047C948;
extern const f64 lbl_8047C950;

/* Inline definition supplied by MSL's math_api.h. */
static inline s32 __fpclassifyd(f64 x)
{
    switch (((s32*)&x)[0] & 0x7ff00000) {
    case 0x7ff00000:
        if ((((s32*)&x)[0] & 0x000fffff) ||
            (((s32*)&x)[1] & 0xffffffff)) {
            return 1;
        }
        return 2;
    case 0:
        if ((((s32*)&x)[0] & 0x000fffff) ||
            (((s32*)&x)[1] & 0xffffffff)) {
            return 5;
        }
        return 3;
    }
    return 4;
}

f64 cos(f64 x)
{
    DoubleShape shape;
    f64 y[2];
    s32 ix;
    s32 n;

    shape.value = x;
    ix = shape.parts.hi & 0x7fffffff;
    if (ix <= 0x3fe921fb) {
        return __kernel_cos(x, lbl_8047C910);
    }
    if (ix >= 0x7ff00000) {
        return x - x;
    }

    n = __ieee754_rem_pio2(x, y);
    switch (n & 3) {
    case 0:
        return __kernel_cos(y[0], y[1]);
    case 1:
        return -__kernel_sin(y[0], y[1], 1);
    case 2:
        return -__kernel_cos(y[0], y[1]);
    default:
        return __kernel_sin(y[0], y[1], 1);
    }
}

f64 floor(f64 x)
{
    DoubleShape shape;
    s32 i0;
    s32 i1;
    s32 j0;
    u32 i;
    u32 j;

    shape.value = x;
    i0 = shape.parts.hi;
    i1 = shape.parts.lo;
    j0 = ((i0 >> 20) & 0x7ff) - 0x3ff;
    if (j0 < 20) {
        if (j0 < 0) {
            if (lbl_8047C918 + x > lbl_8047C920) {
                if (i0 >= 0) {
                    i0 = i1 = 0;
                } else if (((i0 & 0x7fffffff) | i1) != 0) {
                    i0 = 0xbff00000;
                    i1 = 0;
                }
            }
        } else {
            i = 0x000fffff >> j0;
            if (((i0 & i) | i1) == 0) {
                return x;
            }
            if (lbl_8047C918 + x > lbl_8047C920) {
                if (i0 < 0) {
                    i0 += 0x00100000 >> j0;
                }
                i0 &= ~i;
                i1 = 0;
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x;
        }
        return x;
    } else {
        i = 0xffffffffU >> (j0 - 20);
        if ((i1 & i) == 0) {
            return x;
        }
        if (lbl_8047C918 + x > lbl_8047C920) {
            if (i0 < 0) {
                if (j0 == 20) {
                    i0 += 1;
                } else {
                    j = i1 + (1 << (52 - j0));
                    if (j < i1) {
                        i0 += 1;
                    }
                    i1 = j;
                }
            }
            i1 &= ~i;
        }
    }

    shape.parts.hi = i0;
    shape.parts.lo = i1;
    return shape.value;
}

f64 frexp(f64 x, s32* exponent)
{
    DoubleShape shape;
    s32 hx;
    s32 ix;
    s32 lx;

    shape.value = x;
    hx = shape.parts.hi;
    ix = hx & 0x7fffffff;
    lx = shape.parts.lo;
    *exponent = 0;
    if (ix >= 0x7ff00000 || ((ix | lx) == 0)) {
        return shape.value;
    }
    if (ix < 0x00100000) {
        shape.value = x * lbl_8047C928;
        hx = shape.parts.hi;
        ix = hx & 0x7fffffff;
        *exponent = -54;
    }
    *exponent += (ix >> 20) - 1022;
    hx = (hx & 0x800fffff) | 0x3fe00000;
    shape.parts.hi = hx;
    return shape.value;
}

f64 ldexp(f64 x, s32 n)
{
    s32 k;
    s32 hx;
    s32 lx;

    if (!(__fpclassifyd(x) > 2) || lbl_8047C930 == x) {
        return x;
    }

    hx = ((s32*)&x)[0];
    lx = ((s32*)&x)[1];
    k = (hx & 0x7ff00000) >> 20;
    if (k == 0) {
        if ((lx | (hx & 0x7fffffff)) == 0) {
            return x;
        }
        x *= lbl_8047C938;
        hx = ((s32*)&x)[0];
        k = ((hx & 0x7ff00000) >> 20) - 54;
        if (n < -50000) {
            return lbl_8047C940 * x;
        }
    }
    if (k == 0x7ff) {
        return x + x;
    }
    k += n;
    if (k > 0x7fe) {
        return lbl_8047C948 * copysign(lbl_8047C948, x);
    }
    if (k > 0) {
        ((s32*)&x)[0] = (hx & 0x800fffff) | (k << 20);
        return x;
    }
    if (k <= -54) {
        if (n > 50000) {
            return lbl_8047C948 * copysign(lbl_8047C948, x);
        }
        return lbl_8047C940 * copysign(lbl_8047C940, x);
    }
    k += 54;
    ((s32*)&x)[0] = (hx & 0x800fffff) | (k << 20);
    return lbl_8047C950 * x;
}
