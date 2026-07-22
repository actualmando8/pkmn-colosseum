#include "game/gs_colsys.h"

extern void PSVECSubtract(const Vec3f* a, const Vec3f* b, Vec3f* out);
extern void PSVECScale(const Vec3f* vector, Vec3f* out, f32 scale);
extern void PSVECAdd(const Vec3f* a, const Vec3f* b, Vec3f* out);
extern const f32 lbl_8047CF10;

f32 GScolsys2UtilGetCpLinePoint(Vec3f* out, Vec3f* start, Vec3f* end,
                               Vec3f* point)
{
    Vec3f direction;
    f32 lengthSquared;
    f32 t;

    PSVECSubtract(end, start, &direction);
    lengthSquared = direction.x * direction.x + direction.y * direction.y
                  + direction.z * direction.z;
    if (lbl_8047CF10 == lengthSquared) {
        out->x = start->x;
        out->y = start->y;
        out->z = start->z;
        return lbl_8047CF10;
    }

    t = (direction.x * (point->x - start->x)
       + direction.y * (point->y - start->y)
       + direction.z * (point->z - start->z)) / lengthSquared;
    PSVECScale(&direction, &direction, t);
    PSVECAdd(&direction, start, out);
    return t;
}
