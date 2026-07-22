#include "game/gs_colsys.h"

extern void PSVECSubtract(const Vec3f* left, const Vec3f* right, Vec3f* result);
extern void PSVECScale(const Vec3f* vector, Vec3f* result, f32 scale);
extern void PSVECAdd(const Vec3f* left, const Vec3f* right, Vec3f* result);
extern const f32 lbl_8047CF10;

s32 GScolsys2UtilGetCpPlaneLine(Vec3f* out, f32* tOut,
                                const Vec3f* normal,
                                const Vec3f* planePoint,
                                const Vec3f* lineStart,
                                const Vec3f* lineEnd)
{
    Vec3f direction;
    f32 denominator;
    f32 t;

    PSVECSubtract(lineEnd, lineStart, &direction);
    if ((denominator = normal->x * direction.x + normal->y * direction.y
                     + normal->z * direction.z) == lbl_8047CF10) {
        return 0;
    }

    t = (normal->x * (planePoint->x - lineStart->x)
       + normal->y * (planePoint->y - lineStart->y)
       + normal->z * (planePoint->z - lineStart->z)) / denominator;
    PSVECScale(&direction, &direction, t);
    PSVECAdd(&direction, lineStart, out);
    *tOut = t;
    return 1;
}
