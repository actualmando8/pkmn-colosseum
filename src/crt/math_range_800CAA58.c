/**
 * @file math_range_800CAA58.c
 * @brief libm code, 0x800CAA58 - 0x800CE79C (40 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01).
 */
#include "dolphin/types.h"

typedef struct __FILE {
    u8 pad0[4];
    union {
        u16 all;
        struct {
            u8 high;
            union {
                u8 raw;
                struct {
                    u8 top:2;
                    u8 orient:2;
                    u8 bottom:4;
                } bits;
            } low;
        } byte;
    } flags;
} __FILE;

typedef union DoubleShape {
    f64 value;
    struct {
        u32 hi;
        u32 lo;
    } parts;
} DoubleShape;

typedef union FloatShape {
    f32 value;
    u32 bits;
} FloatShape;

extern u32 OSGetConsoleType(void);
extern s32 InitializeUART(u32 baud);
extern s32 fn_800CF4EC(const void* data, u32 count);
extern u32 fn_800C3A40(u32 handle, const void* data, u32* count, void* ref);

extern f64 __ieee754_acos(f64 x);
extern f64 __ieee754_asin(f64 x);
extern f64 __ieee754_atan2(f64 y, f64 x);
extern f64 __ieee754_exp(f64 x);
extern f64 __ieee754_fmod(f64 x, f64 y);
extern f64 __ieee754_log(f64 x);
extern f64 __ieee754_pow(f64 x, f64 y);
extern s32 __ieee754_rem_pio2(f64 x, f64* y);
extern f64 __ieee754_sqrt(f64 x);
extern f64 __kernel_cos(f64 x, f64 y);
extern f64 __kernel_sin(f64 x, f64 y, s32 iy);
extern f64 __kernel_tan(f64 x, f64 y, s32 iy);
s32 __fpclassifyf(f32 x);

extern s32 lbl_8047AA18;
extern s32 lbl_8047AA10;
extern const f32 lbl_80478AC0[];

extern const f64 lbl_8047C900;
extern const f64 lbl_8047C908;
extern const f64 lbl_8047C910;
extern const f64 lbl_8047C918;
extern const f64 lbl_8047C920;
extern const f64 lbl_8047C928;
extern const f64 lbl_8047C930;
extern const f64 lbl_8047C938;
extern const f64 lbl_8047C940;
extern const f64 lbl_8047C948;
extern const f64 lbl_8047C950;
extern const f64 lbl_8047C958;
extern const f64 lbl_8047C960;
extern const f64 lbl_8047C968;
extern const f32 lbl_8047C970;
extern const f64 lbl_8047C978;
extern const f64 lbl_8047C980;
extern const f64 lbl_8047C988;

s32 fwide(__FILE* file, s32 mode) {
    u8 orient;

    if (file == NULL || (((u32)file->flags.all >> 6) & 7) == 0) {
        return 0;
    }

    orient = file->flags.byte.low.bits.orient;
    switch (orient) {
    case 0:
        if (mode > 0) {
            file->flags.byte.low.bits.orient = 2;
        } else if (mode < 0) {
            file->flags.byte.low.bits.orient = 1;
        }
        return mode;
    case 2:
        return 1;
    case 1:
        return -1;
    default:
        return (s32)file;
    }
}

s32 __write_console(u32 handle, const void* data, u32* count, void* ref) {
    s32 err;

    if ((OSGetConsoleType() & 0x20000000) == 0) {
        err = 0;
        if (lbl_8047AA18 == 0) {
            err = InitializeUART(0xE100);
            if (err == 0) {
                lbl_8047AA18 = 1;
            }
        }

        if (err != 0) {
            return 1;
        }

        if (fn_800CF4EC(data, *count) != 0) {
            *count = 0;
            return 1;
        }
    }

    fn_800C3A40(handle, data, count, ref);
    return 0;
}

f64 copysign(f64 x, f64 y) {
    DoubleShape uy;
    DoubleShape ux;

    ux.value = x;
    uy.value = y;
    ux.parts.hi = (ux.parts.hi & 0x7fffffff) | (uy.parts.hi & 0x80000000);
    return ux.value;
}

#pragma dont_inline on
f64 cos(f64 x) {
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

f64 ceil(f64 x) {
    DoubleShape shape;
    s32 hi;
    u32 lo;
    s32 j0;
    s32 i;
    u32 ui;
    u32 old;

    shape.value = x;
    hi = shape.parts.hi;
    lo = shape.parts.lo;
    j0 = ((hi >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20) {
        if (j0 < 0) {
            if (lbl_8047C900 + x > lbl_8047C908) {
                if (hi < 0) {
                    hi = 0x80000000;
                    lo = 0;
                } else if ((hi | lo) != 0) {
                    hi = 0x3ff00000;
                    lo = 0;
                }
            }
        } else {
            i = 0x000fffff >> j0;
            if (((hi & i) | lo) == 0) {
                return x;
            }
            if (lbl_8047C900 + x > lbl_8047C908) {
                if (hi > 0) {
                    hi += 0x00100000 >> j0;
                }
                hi &= ~i;
                lo = 0;
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x;
        }
        return x;
    } else {
        ui = 0xffffffffU >> (j0 - 20);
        if ((lo & ui) == 0) {
            return x;
        }
        if (lbl_8047C900 + x > lbl_8047C908) {
            if (hi > 0) {
                if (j0 == 20) {
                    hi += 1;
                } else {
                    i = 1 << (52 - j0);
                    old = lo;
                    lo += i;
                    if (lo < old) {
                        hi += 1;
                    }
                }
            }
            lo &= ~ui;
        }
    }

    shape.parts.hi = hi;
    shape.parts.lo = lo;
    return shape.value;
}

f64 floor(f64 x) {
    DoubleShape shape;
    s32 hi;
    u32 lo;
    s32 j0;
    s32 i;
    u32 ui;
    u32 old;

    shape.value = x;
    hi = shape.parts.hi;
    lo = shape.parts.lo;
    j0 = ((hi >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20) {
        if (j0 < 0) {
            if (lbl_8047C918 + x > lbl_8047C920) {
                if (hi >= 0) {
                    hi = 0;
                    lo = 0;
                } else if (((hi & 0x7fffffff) | lo) != 0) {
                    hi = 0xbff00000;
                    lo = 0;
                }
            }
        } else {
            i = 0x000fffff >> j0;
            if (((hi & i) | lo) == 0) {
                return x;
            }
            if (lbl_8047C918 + x > lbl_8047C920) {
                if (hi < 0) {
                    hi += 0x00100000 >> j0;
                }
                hi &= ~i;
                lo = 0;
            }
        }
    } else if (j0 > 51) {
        if (j0 == 0x400) {
            return x + x;
        }
        return x;
    } else {
        ui = 0xffffffffU >> (j0 - 20);
        if ((lo & ui) == 0) {
            return x;
        }
        if (lbl_8047C918 + x > lbl_8047C920) {
            if (hi < 0) {
                if (j0 == 20) {
                    hi += 1;
                } else {
                    i = 1 << (52 - j0);
                    old = lo;
                    lo += i;
                    if (lo < old) {
                        hi += 1;
                    }
                }
            }
            lo &= ~ui;
        }
    }

    shape.parts.hi = hi;
    shape.parts.lo = lo;
    return shape.value;
}

f64 frexp(f64 x, s32* exponent) {
    DoubleShape shape;
    s32 hi;
    s32 ix;

    shape.value = x;
    hi = shape.parts.hi;
    *exponent = 0;
    ix = hi & 0x7fffffff;
    if (ix >= 0x7ff00000 || ((ix | shape.parts.lo) == 0)) {
        return x;
    }
    if (ix < 0x00100000) {
        *exponent = -54;
        shape.value = x * lbl_8047C928;
        hi = shape.parts.hi;
        ix = hi & 0x7fffffff;
    }
    shape.parts.hi = (hi & 0x800fffff) | 0x3fe00000;
    *exponent += (ix >> 20) - 0x3fe;
    return shape.value;
}

f64 ldexp(f64 x, s32 n) {
    DoubleShape shape;
    s32 hi;
    u32 lo;
    s32 k;
    s32 fpclass;

    shape.value = x;
    hi = shape.parts.hi;
    lo = shape.parts.lo;
    k = hi & 0x7ff00000;

    if (k == 0x7ff00000) {
        if ((hi & 0x000fffff) != 0 || lo != 0) {
            fpclass = 1;
        } else {
            fpclass = 2;
        }
    } else if (k == 0) {
        if ((hi & 0x000fffff) != 0 || lo != 0) {
            fpclass = 5;
        } else {
            fpclass = 3;
        }
    } else {
        fpclass = 4;
    }

    if (fpclass <= 2 || x == lbl_8047C930) {
        return x;
    }

    hi = shape.parts.hi;
    lo = shape.parts.lo;
    k = (hi >> 20) & 0x7ff;

    if (k == 0) {
        if (((hi & 0x7fffffff) | lo) == 0) {
            return x;
        }
        x *= lbl_8047C938;
        shape.value = x;
        hi = shape.parts.hi;
        k = ((hi >> 20) & 0x7ff) - 54;
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
        shape.parts.hi = (hi & 0x800fffff) | (k << 20);
        return shape.value;
    }
    if (k <= -54) {
        if (n > 50000) {
            return lbl_8047C948 * copysign(lbl_8047C948, x);
        }
        return lbl_8047C940 * copysign(lbl_8047C940, x);
    }

    k += 54;
    shape.parts.hi = (hi & 0x800fffff) | (k << 20);
    return lbl_8047C950 * shape.value;
}

f64 modf(f64 x, f64* integral) {
    DoubleShape shape;
    DoubleShape intpart;
    s32 hi;
    u32 lo;
    s32 j0;
    s32 i;
    u32 ui;

    shape.value = x;
    hi = shape.parts.hi;
    lo = shape.parts.lo;
    j0 = ((hi >> 20) & 0x7ff) - 0x3ff;

    if (j0 < 20) {
        if (j0 < 0) {
            intpart.parts.hi = hi & 0x80000000;
            intpart.parts.lo = 0;
            *integral = intpart.value;
            return x;
        }
        i = 0x000fffff >> j0;
        if (((hi & i) | lo) == 0) {
            intpart.parts.hi = hi & 0x80000000;
            intpart.parts.lo = 0;
            *integral = x;
            return intpart.value;
        }
        intpart.parts.hi = hi & ~i;
        intpart.parts.lo = 0;
        *integral = intpart.value;
        return x - intpart.value;
    }

    if (j0 > 51) {
        intpart.parts.hi = hi & 0x80000000;
        intpart.parts.lo = 0;
        *integral = x;
        return intpart.value;
    }

    ui = 0xffffffffU >> (j0 - 20);
    if ((lo & ui) == 0) {
        intpart.parts.hi = hi & 0x80000000;
        intpart.parts.lo = 0;
        *integral = x;
        return intpart.value;
    }

    intpart.parts.hi = hi;
    intpart.parts.lo = lo & ~ui;
    *integral = intpart.value;
    return x - intpart.value;
}

f64 sin(f64 x) {
    DoubleShape shape;
    f64 y[2];
    s32 ix;
    s32 n;

    shape.value = x;
    ix = shape.parts.hi & 0x7fffffff;
    if (ix <= 0x3fe921fb) {
        return __kernel_sin(x, lbl_8047C958, 0);
    }
    if (ix >= 0x7ff00000) {
        return x - x;
    }

    n = __ieee754_rem_pio2(x, y);
    switch (n & 3) {
    case 0:
        return __kernel_sin(y[0], y[1], 1);
    case 1:
        return __kernel_cos(y[0], y[1]);
    case 2:
        return -__kernel_sin(y[0], y[1], 1);
    default:
        return -__kernel_cos(y[0], y[1]);
    }
}

f64 tan(f64 x) {
    DoubleShape shape;
    f64 y[2];
    s32 ix;
    s32 n;

    shape.value = x;
    ix = shape.parts.hi & 0x7fffffff;
    if (ix <= 0x3fe921fb) {
        return __kernel_tan(x, lbl_8047C960, 1);
    }
    if (ix >= 0x7ff00000) {
        return x - x;
    }

    n = __ieee754_rem_pio2(x, y);
    return __kernel_tan(y[0], y[1], 1 - ((n & 1) << 1));
}
#pragma dont_inline reset

f64 acos(f64 x) {
    return __ieee754_acos(x);
}

f64 asin(f64 x) {
    return __ieee754_asin(x);
}

f64 atan2(f64 y, f64 x) {
    return __ieee754_atan2(y, x);
}

f64 exp(f64 x) {
    return __ieee754_exp(x);
}

f64 fmod(f64 x, f64 y) {
    return __ieee754_fmod(x, y);
}

f64 log(f64 x) {
    return __ieee754_log(x);
}

f64 pow(f64 x, f64 y) {
    return __ieee754_pow(x, y);
}

f64 fabs(f64 x) {
    return __fabs(x);
}

f32 sqrtf(f32 x) {
    FloatShape shape;
    f64 y;
    u32 bits;
    s32 exp;
    s32 fpclass;

    if (x > lbl_8047C970) {
        y = __frsqrte(x);
        y = lbl_8047C978 * y * (lbl_8047C980 - x * (y * y));
        y = lbl_8047C978 * y * (lbl_8047C980 - x * (y * y));
        y = lbl_8047C978 * y * (lbl_8047C980 - x * (y * y));
        return (f32)(x * y);
    }

    if ((f64)x < lbl_8047C988) {
        return lbl_80478AC0[0];
    }

    shape.value = x;
    bits = shape.bits;
    exp = bits & 0x7f800000;
    switch (exp) {
    case 0x7f800000:
        if ((bits & 0x007fffff) != 0) {
            fpclass = 1;
        } else {
            fpclass = 2;
        }
        break;
    case 0:
        if ((bits & 0x007fffff) != 0) {
            fpclass = 5;
        } else {
            fpclass = 3;
        }
        break;
    default:
        fpclass = 4;
        break;
    }

    if (fpclass == 1) {
        return lbl_80478AC0[0];
    }

    return x;
}

f32 tanf(f32 x) {
    return (f32)tan(x);
}

f32 sinf(f32 x) {
    return (f32)sin(x);
}

f32 cosf(f32 x) {
    return (f32)cos(x);
}

f32 acosf(f32 x) {
    return (f32)acos(x);
}

s32 __fpclassifyf(f32 x) {
    FloatShape shape;
    u32 bits;
    s32 exp;
    u32 frac;

    shape.value = x;
    bits = shape.bits;
    exp = bits & 0x7f800000;
    switch (exp) {
    case 0x7f800000:
        frac = bits & 0x007fffff;
        return ((s32)(-frac | frac) >> 31) + 2;
    case 0:
        return (bits & 0x007fffff) ? 5 : 3;
    default:
        return 4;
    }
}

f64 sqrt(f64 x) {
    return __ieee754_sqrt(x);
}
