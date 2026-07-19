/**
 * @file math_exact_800CE59C.c
 * @brief Exact pure-C libm wrappers and single-precision helpers.
 */
#include "crt/math.h"

typedef union FloatShape {
    f32 value;
    u32 bits;
} FloatShape;

extern f64 __ieee754_sqrt(f64 x);
extern f64 acos(f64 x);
extern f64 cos(f64 x);
extern f64 sin(f64 x);
extern f64 tan(f64 x);

extern const f64 lbl_8047C978;
extern const f64 lbl_8047C980;
extern const f64 lbl_8047C988;
extern const f32 lbl_8047C970;

f64 fabs(f64 x)
{
    return __fabs(x);
}

f32 sqrtf(f32 x)
{
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

f32 tanf(f32 x)
{
    return (f32)tan(x);
}

f32 sinf(f32 x)
{
    return (f32)sin(x);
}

f32 cosf(f32 x)
{
    return (f32)cos(x);
}

f32 acosf(f32 x)
{
    return (f32)acos(x);
}

s32 __fpclassifyf(f32 x)
{
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

f64 sqrt(f64 x)
{
    return __ieee754_sqrt(x);
}
