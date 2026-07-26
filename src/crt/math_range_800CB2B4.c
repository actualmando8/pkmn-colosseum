/**
 * @file math_range_800CB2B4.c
 * @brief Shared libm candidate range, 0x800CB2B4 - 0x800CC754.
 */
#include "src/crt/math_range_800CAA58.c"

static const f64 fmodOne = 1.0;
static const f64 fmodZero[] = { 0.0, -0.0 };
static const f64 expOne = 1.0;
static const f64 expHalf[] = { 0.5, -0.5 };
static const f64 expHuge = 1.0e300;
static const f64 expTwoM1000 = 9.33263618503218878990e-302;
static const f64 expOverflow = 7.09782712893383973096e+02;
static const f64 expUnderflow = -7.45133219101941108420e+02;
static const f64 expLn2High[] = {
    6.93147180369123816490e-01,
    -6.93147180369123816490e-01,
};
static const f64 expLn2Low[] = {
    1.90821492927058770002e-10,
    -1.90821492927058770002e-10,
};
static const f64 expInvLn2 = 1.44269504088896338700e+00;
static const f64 expP1 = 1.66666666666666019037e-01;
static const f64 expP2 = -2.77777777770155933842e-03;
static const f64 expP3 = 6.61375632143793436117e-05;
static const f64 expP4 = -1.65339022054652515390e-06;
static const f64 expP5 = 4.13813679705723846039e-08;

f64 __ieee754_exp(f64 x)
{
    DoubleShape shape;
    DoubleShape result;
    f64 y;
    f64 high;
    f64 low;
    f64 correction;
    f64 square;
    s32 k;
    s32 sign;
    u32 hx;

    shape.value = x;
    hx = shape.parts.hi;
    sign = (hx >> 31) & 1;
    hx &= 0x7FFFFFFF;

    if (hx >= 0x40862E42) {
        if (hx >= 0x7FF00000) {
            if (((hx & 0xFFFFF) | shape.parts.lo) != 0) {
                return x + x;
            }
            return sign == 0 ? x : 0.0;
        }
        if (x > expOverflow) {
            return expHuge * expHuge;
        }
        if (x < expUnderflow) {
            return expTwoM1000 * expTwoM1000;
        }
    }

    if (hx > 0x3FD62E42) {
        if (hx < 0x3FF0A2B2) {
            high = x - expLn2High[sign];
            low = expLn2Low[sign];
            k = 1 - sign - sign;
        } else {
            k = (s32)(expInvLn2 * x + expHalf[sign]);
            square = k;
            high = x - square * expLn2High[0];
            low = square * expLn2Low[0];
        }
        x = high - low;
    } else if (hx < 0x3E300000) {
        if (expHuge + x > expOne) {
            return expOne + x;
        }
    } else {
        k = 0;
    }

    square = x * x;
    correction =
        x - square *
                (expP1 +
                 square *
                     (expP2 +
                      square *
                          (expP3 + square * (expP4 + square * expP5))));
    if (k == 0) {
        return expOne - ((x * correction) / (correction - 2.0) - x);
    }
    y = expOne - ((low - (x * correction) / (2.0 - correction)) - high);
    result.value = y;
    if (k >= -1021) {
        result.parts.hi += k << 20;
        return result.value;
    }
    result.parts.hi += (k + 1000) << 20;
    return result.value * expTwoM1000;
}

f64 __ieee754_fmod(f64 x, f64 y)
{
    DoubleShape xShape;
    DoubleShape yShape;
    s32 n;
    s32 hx;
    s32 hy;
    s32 hz;
    s32 ix;
    s32 iy;
    s32 sx;
    s32 i;
    u32 lx;
    u32 ly;
    u32 lz;

    xShape.value = x;
    yShape.value = y;
    hx = xShape.parts.hi;
    lx = xShape.parts.lo;
    hy = yShape.parts.hi;
    ly = yShape.parts.lo;
    sx = hx & 0x80000000;
    hx ^= sx;
    hy &= 0x7FFFFFFF;

    if ((hy | ly) == 0 || hx >= 0x7FF00000 ||
        (hy | ((ly | -ly) >> 31)) > 0x7FF00000)
    {
        return (x * y) / (x * y);
    }
    if (hx <= hy) {
        if (hx < hy || lx < ly) {
            return x;
        }
        if (lx == ly) {
            return fmodZero[(u32)sx >> 31];
        }
    }

    if (hx < 0x00100000) {
        if (hx == 0) {
            for (ix = -1043, i = lx; i > 0; i <<= 1) {
                ix--;
            }
        } else {
            for (ix = -1022, i = hx << 11; i > 0; i <<= 1) {
                ix--;
            }
        }
    } else {
        ix = (hx >> 20) - 1023;
    }

    if (hy < 0x00100000) {
        if (hy == 0) {
            for (iy = -1043, i = ly; i > 0; i <<= 1) {
                iy--;
            }
        } else {
            for (iy = -1022, i = hy << 11; i > 0; i <<= 1) {
                iy--;
            }
        }
    } else {
        iy = (hy >> 20) - 1023;
    }

    if (ix >= -1022) {
        hx = 0x00100000 | (hx & 0x000FFFFF);
    } else {
        n = -1022 - ix;
        if (n <= 31) {
            hx = (hx << n) | (lx >> (32 - n));
            lx <<= n;
        } else {
            hx = lx << (n - 32);
            lx = 0;
        }
    }
    if (iy >= -1022) {
        hy = 0x00100000 | (hy & 0x000FFFFF);
    } else {
        n = -1022 - iy;
        if (n <= 31) {
            hy = (hy << n) | (ly >> (32 - n));
            ly <<= n;
        } else {
            hy = ly << (n - 32);
            ly = 0;
        }
    }

    n = ix - iy;
    while (n--) {
        hz = hx - hy;
        lz = lx - ly;
        if (lx < ly) {
            hz--;
        }
        if (hz < 0) {
            hx = hx + hx + (lx >> 31);
            lx += lx;
        } else {
            if ((hz | lz) == 0) {
                return fmodZero[(u32)sx >> 31];
            }
            hx = hz + hz + (lz >> 31);
            lx = lz + lz;
        }
    }
    hz = hx - hy;
    lz = lx - ly;
    if (lx < ly) {
        hz--;
    }
    if (hz >= 0) {
        hx = hz;
        lx = lz;
    }

    if ((hx | lx) == 0) {
        return fmodZero[(u32)sx >> 31];
    }
    while (hx < 0x00100000) {
        hx = hx + hx + (lx >> 31);
        lx += lx;
        iy--;
    }
    if (iy >= -1022) {
        hx = (hx - 0x00100000) | ((iy + 1023) << 20);
        xShape.parts.hi = hx | sx;
        xShape.parts.lo = lx;
    } else {
        n = -1022 - iy;
        if (n <= 20) {
            lx = (lx >> n) | ((u32)hx << (32 - n));
            hx >>= n;
        } else if (n <= 31) {
            lx = (hx << (32 - n)) | (lx >> n);
            hx = sx;
        } else {
            lx = hx >> (n - 32);
            hx = sx;
        }
        xShape.parts.hi = hx | sx;
        xShape.parts.lo = lx;
        xShape.value *= fmodOne;
    }
    return xShape.value;
}
