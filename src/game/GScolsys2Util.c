/**
 * @file GScolsys2Util.c
 * @brief GScolsys2Util -- small collision-query leaf helpers.
 *
 * First of six translation units recovered from the former
 * game/gs_field_colquery.c CodeCandidate bucket (0x8010F6A0-0x801140DC).
 * This unit covers the leaf point/plane utility helpers used by the
 * larger GScolsys2Human/Thru/Sun query functions in the sibling units.
 *
 * Address range: 0x8010F6A0 - 0x8010FAF4
 */
#include "dolphin/types.h"
#include "game/gs_colsys.h"
#include "game/world/gs_field.h"

/* 0x8010F6A0 | 0x7C */
void GScolsy2UtilGetPointExtentionLine(void* arg0, void* arg1, void* arg2, f32 t) {
    f32 v[3];
    extern void PSVECSubtract(void*, void*, void*);
    extern f32 PSVECMag(void*);
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);

    PSVECSubtract(arg2, arg1, v);
    PSVECScale(v, v, t / PSVECMag(v));
    PSVECAdd(v, arg1, arg0);
}

/* 0x8010F71C | 0x338 */
s32 GScolsy2UtilChkInTri(void* pointArg, void* vertsArg, void* normalArg)
{
    extern const f32 lbl_8047CF10;
    extern const f32 lbl_8047CF14;
    extern const f32 lbl_8047CF18;
    Vec3f* point = pointArg;
    Vec3f* verts = vertsArg;
    Vec3f* normal = normalArg;
    f32* pointValues = (f32*)point;
    f32 projected[3][2];
    f32 absX;
    f32 absY;
    f32 absZ;
    f32 orientation;
    f32 minU;
    f32 minV;
    f32 maxU;
    f32 maxV;
    s32 u;
    s32 v;
    s32 i;

    absX = normal->x > lbl_8047CF10 ? normal->x : -normal->x;
    absY = normal->y > lbl_8047CF10 ? normal->y : -normal->y;
    absZ = normal->z > lbl_8047CF10 ? normal->z : -normal->z;

    if (absX < absY) {
        if (absY < absZ) {
            orientation = normal->z;
            u = 0;
            v = 1;
        } else {
            orientation = normal->y;
            u = 2;
            v = 0;
        }
    } else if (absX > absZ) {
        orientation = -normal->x;
        u = 2;
        v = 1;
    } else {
        orientation = normal->z;
        u = 0;
        v = 1;
    }

    if (orientation < lbl_8047CF10) {
        for (i = 0; i < 3; i++) {
            projected[i][0] = ((f32*)&verts[i])[u];
            projected[i][1] = ((f32*)&verts[i])[v];
        }
    } else {
        for (i = 0; i < 3; i++) {
            projected[i][0] = ((f32*)&verts[2 - i])[u];
            projected[i][1] = ((f32*)&verts[2 - i])[v];
        }
    }

    minU = minV = lbl_8047CF14;
    maxU = maxV = lbl_8047CF18;
    for (i = 0; i < 3; i++) {
        if (minU > projected[i][0]) {
            minU = projected[i][0];
        }
        if (minV > projected[i][1]) {
            minV = projected[i][1];
        }
        if (maxU < projected[i][0]) {
            maxU = projected[i][0];
        }
        if (maxV < projected[i][1]) {
            maxV = projected[i][1];
        }
    }

    if (minU > pointValues[u] || minV > pointValues[v] ||
        maxU < pointValues[u] || maxV < pointValues[v]) {
        return 0;
    }

    for (i = 0; i < 3; i++) {
        s32 next = i + 1;
        f32 cross;
        if (next >= 3) {
            next = 0;
        }
        cross = (projected[next][1] - projected[i][1]) *
                    (pointValues[u] - projected[i][0]) -
                (projected[next][0] - projected[i][0]) *
                    (pointValues[v] - projected[i][1]);
        if (cross > lbl_8047CF10) {
            return 0;
        }
    }
    return 1;
}

/* 0x8010FA54 | 0xA0 */
void GScolsy2UtilGetCpPlanePoint(Vec3f* out, Vec3f* normal, Vec3f* verts, Vec3f* point) {
    f32 scale;
    extern void PSVECScale(void*, void*, f32);
    extern void PSVECAdd(void*, void*, void*);

    scale = (normal->x * (verts->x - point->x)
           + normal->y * (verts->y - point->y)
           + normal->z * (verts->z - point->z))
          / (normal->x * normal->x
           + normal->y * normal->y
           + normal->z * normal->z);
    PSVECScale(normal, out, scale);
    PSVECAdd(out, point, out);
}
