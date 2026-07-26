/**
 * @file math_range_800CD648.c
 * @brief Shared libm candidate range, 0x800CD648 - 0x800CDBB8.
 */
#include "src/crt/math_range_800CAA58.c"

static const f64 atanHigh[] = {
    4.63647609000806093515e-01,
    7.85398163397448278999e-01,
    9.82793723247329054082e-01,
    1.57079632679489655800e+00,
};
static const f64 atanLow[] = {
    2.26987774529616870924e-17,
    3.06161699786838301793e-17,
    1.39033110312309984516e-17,
    6.12323399573676603587e-17,
};
static const f64 atanCoefficients[] = {
    3.33333333333329318027e-01,
    -1.99999999998764832476e-01,
    1.42857142725034663711e-01,
    -1.11111104054623557880e-01,
    9.09088713343650656196e-02,
    -7.69187620504482999495e-02,
    6.66107313738753120669e-02,
    -5.83357013379057348645e-02,
    4.97687799461593236017e-02,
    -3.65315727442169155270e-02,
    1.62858201153657823623e-02,
};
static const f64 atanOne = 1.0;
static const f64 atanHuge = 1.0e300;
static const f64 tanCoefficients[] = {
    3.33333333333334091986e-01,
    1.33333333333201242699e-01,
    5.39682539762260521377e-02,
    2.18694882948595424599e-02,
    8.86323982359930005737e-03,
    3.59207910759131235356e-03,
    1.45620945432529025516e-03,
    5.88041240820264096874e-04,
    2.46463134818469906812e-04,
    7.81794442939557092300e-05,
    7.14072491382608190305e-05,
    -1.85586374855275456654e-05,
    2.59073051863633712884e-05,
};

f64 atan(f64 x)
{
    DoubleShape shape;
    f64 w;
    f64 s1;
    f64 s2;
    f64 z;
    s32 ix;
    s32 hx;
    s32 id;

    shape.value = x;
    hx = shape.parts.hi;
    ix = hx & 0x7FFFFFFF;
    if (ix >= 0x44100000) {
        if (ix > 0x7FF00000 ||
            (ix == 0x7FF00000 && shape.parts.lo != 0))
        {
            return x + x;
        }
        if (hx > 0) {
            return atanHigh[3] + atanLow[3];
        }
        return -atanHigh[3] - atanLow[3];
    }

    if (ix < 0x3FDC0000) {
        if (ix < 0x3E200000 && atanHuge + x > atanOne) {
            return x;
        }
        id = -1;
    } else {
        shape.parts.hi = ix;
        x = shape.value;
        if (ix < 0x3FF30000) {
            if (ix < 0x3FE60000) {
                id = 0;
                x = (2.0 * x - atanOne) / (2.0 + x);
            } else {
                id = 1;
                x = (x - atanOne) / (x + atanOne);
            }
        } else if (ix < 0x40038000) {
            id = 2;
            x = (x - 1.5) / (atanOne + 1.5 * x);
        } else {
            id = 3;
            x = -1.0 / x;
        }
    }

    z = x * x;
    w = z * z;
    s1 = z *
         (atanCoefficients[0] +
          w * (atanCoefficients[2] +
               w * (atanCoefficients[4] +
                    w * (atanCoefficients[6] +
                         w * (atanCoefficients[8] +
                              w * atanCoefficients[10])))));
    s2 = w *
         (atanCoefficients[1] +
          w * (atanCoefficients[3] +
               w * (atanCoefficients[5] +
                    w * (atanCoefficients[7] +
                         w * atanCoefficients[9]))));
    if (id < 0) {
        return x - x * (s1 + s2);
    }

    z = atanHigh[id] - ((x * (s1 + s2) - atanLow[id]) - x);
    return hx < 0 ? -z : z;
}

f64 __kernel_tan(f64 x, f64 y, s32 iy)
{
    DoubleShape shape;
    DoubleShape truncated;
    f64 z;
    f64 r;
    f64 v;
    f64 w;
    f64 s;
    f64 a;
    f64 t;
    s32 ix;
    s32 hx;

    shape.value = x;
    hx = shape.parts.hi;
    ix = hx & 0x7FFFFFFF;
    if (ix < 0x3E300000 && (s32)x == 0) {
        if (((ix | shape.parts.lo) | (iy + 1)) == 0) {
            shape.parts.hi &= 0x7FFFFFFF;
            return 1.0 / shape.value;
        }
        if (iy == 1) {
            return x;
        }
        return -1.0 / x;
    }

    if (ix >= 0x3FE59428) {
        if (hx < 0) {
            x = -x;
            y = -y;
        }
        z = 7.85398163397448278999e-01 - x;
        w = 3.06161699786838301793e-17 - y;
        x = z + w;
        y = 0.0;
    }

    z = x * x;
    w = z * z;
    r = tanCoefficients[1] +
        w * (tanCoefficients[3] +
             w * (tanCoefficients[5] +
                  w * (tanCoefficients[7] +
                       w * (tanCoefficients[9] +
                            w * tanCoefficients[11]))));
    v = z *
        (tanCoefficients[2] +
         w * (tanCoefficients[4] +
              w * (tanCoefficients[6] +
                   w * (tanCoefficients[8] +
                        w * (tanCoefficients[10] +
                             w * tanCoefficients[12])))));
    s = z * x;
    r = y + z * (s * (r + v) + y);
    r += tanCoefficients[0] * s;
    w = x + r;
    if (ix >= 0x3FE59428) {
        v = (f64)iy;
        return (f64)(1 - ((hx >> 30) & 2)) *
               (v - 2.0 * (x - (w * w / (w + v) - r)));
    }
    if (iy == 1) {
        return w;
    }

    truncated.value = w;
    truncated.parts.lo = 0;
    z = truncated.value;
    v = r - (z - x);
    t = a = -1.0 / w;
    truncated.value = t;
    truncated.parts.lo = 0;
    t = truncated.value;
    s = 1.0 + t * z;
    return t + a * (s + t * v);
}
