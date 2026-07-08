/**
 * @file GScolsys2Sun_range_80111C24.c
 * @brief GScolsys2Sun (+ neighbor overflow) -- boundary/region ray tests.
 *
 * Fifth of six translation units recovered from the former
 * game/gs_field_colquery.c CodeCandidate bucket (0x8010F6A0-0x801140DC).
 * Only the first function (GScolsys2Sun) is anchor-confirmed; the
 * remaining four have no confirmed match and may include
 * Colosseum-only additions or GScolsys2Check remainder overflow, so
 * this unit keeps the _range_ fallback name pending call-graph
 * confirmation of the exact XD TU boundary.
 *
 * Address range: 0x80111C24 - 0x80112380
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
#include "game/gs_field_colquery_types.h"

/* 0x80111C24 | 0x1D4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
s32 GScolsys2Sun(void* origin, void* dir) {
#pragma optimization_level 4
    extern f32 PSVECDistance(void* a, void* b);
    extern s32 GScolsys2UtilGetCpPlaneLine(void* a, void* b, void* c, void* d, void* e, void* f);
    extern s32 GScolsy2UtilChkInTri(void* a, void* b, void* c);
    extern f32 lbl_8047CF68;
    extern f32 lbl_8047CF6C;
    GSFieldWzxData* wzx;
    u32 regIdx;
    GSFieldWzxTriangleList* triList;
    GSFieldWzxRegion* region;
    s32 k;
    u32 vertIdx;
    GSFieldWzxCompactTriangle* tri;
    GSFieldVec3f* vdst;
    GSFieldVec3f* vsrc;
    s32 visFlag;
    f32 resultT;
    GSFieldVec3f dirVec;
    GSFieldVec3f pt;
    GSFieldVec3f out;
    f32 mtxFwd[12];
    f32 mtxInv[12];
    GSFieldVec3f verts[3];
    s32 found;
    s32 hit;

    if (fn_8010CBC0() == NULL) {
        return 0;
    }
    if (PSVECDistance(dir, origin) <= lbl_8047CF68) {
        return 0;
    }
    wzx = (GSFieldWzxData*)fn_8010CBC0();
    PSVECSubtract(dir, origin, &dirVec);
    region = wzx->regions;
    regIdx = 0;
    while (regIdx < wzx->regionCount) {
        GScolsys2GetObjEnable(regIdx, &visFlag);
        if (visFlag != 0) {
            triList = region->boundaryTriangles;
            if (triList != NULL) {
                fn_8010CA30(mtxInv, regIdx);
                fn_8010C8D0(mtxFwd, regIdx);
                tri = (GSFieldWzxCompactTriangle*)triList->triangles;
                vertIdx = 0;
                while (vertIdx < triList->triangleCount) {
                    PSMTXMultVec(mtxFwd, &tri->normal, &pt);
                    vsrc = tri->vertices;
                    vdst = verts;
                    k = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        k++;
                        vsrc++;
                        vdst++;
                    } while (k < 3);
                    if (GScolsys2UtilGetCpPlaneLine(&out, &resultT, &pt, verts, origin, dir) == 0) {
                        hit = 0;
                    } else if (resultT < lbl_8047CF68 || resultT > lbl_8047CF6C) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(&out, verts, &pt) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                    if (hit != 0) {
                        found = 1;
                        goto inner_done;
                    }
                    vertIdx++;
                    tri++;
                }
                found = 0;
            inner_done:
                if (found != 0) {
                    return 1;
                }
            }
        }
        regIdx++;
        region++;
    }
    return 0;
}
#pragma peephole on
#pragma pop

/* 0x80111DF8 | 0x134 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111DF8(void) {
    /* TODO: match -- 308 bytes at 0x80111DF8 */
}
#pragma pop

/* 0x80111F2C | 0x150 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80111F2C(void) {
    /* TODO: match -- 336 bytes at 0x80111F2C */
}
#pragma pop

/* 0x8011207C | 0x1E4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011207C(void) {
    /* TODO: match -- 484 bytes at 0x8011207C */
}
#pragma pop

/* 0x80112260 | 0x120 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_80112260(void) {
    /* TODO: match -- 288 bytes at 0x80112260 */
}
#pragma pop
