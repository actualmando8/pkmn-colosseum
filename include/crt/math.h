#ifndef CRT_MATH_H
#define CRT_MATH_H

#include "dolphin/types.h"

f64 atan2(f64 y, f64 x);
f64 __frsqrte(f64 value);

/* NaN constant owned by the CRT math data range. */
extern const f32 lbl_80478AC0[];

#endif /* CRT_MATH_H */
