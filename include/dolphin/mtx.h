/**
 * @file mtx.h
 * @brief Dolphin SDK matrix/vector types.
 *
 * The SDK's Mtx library types, as used by the HSD (sysdolphin) middleware.
 * Having a real Vec type lets HSD code assign vectors by value
 * (`wobj->pos = *position;`) the way the original sources did, instead of
 * copying through `u32*` casts.
 */
#ifndef DOLPHIN_MTX_H
#define DOLPHIN_MTX_H

#include "dolphin/types.h"

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec, Vec3;

typedef f32 Mtx[3][4];
typedef f32 (*MtxPtr)[4];

void PSMTXMultVec(const Mtx m, const Vec* src, Vec* dst);

#endif /* DOLPHIN_MTX_H */
