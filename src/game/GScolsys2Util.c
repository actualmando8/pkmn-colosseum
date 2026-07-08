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
#include "game/world/gs_field.h"
#include "game/gs_field_colquery_types.h"

/* 0x8010F6A0 | 0x7C */
void GScolsy2UtilGetPointExtentionLine(void* arg0, void* arg1, void* arg2, f32 t) {
    f32 v[3];

    PSVECSubtract(arg2, arg1, v);
    PSVECScale(v, v, t / PSVECMag(v));
    PSVECAdd(v, arg1, arg0);
}

/* 0x8010F71C | 0x338 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GScolsy2UtilChkInTri(void* point, void* verts, void* normal) {
    /* TODO: match -- 824 bytes at 0x8010F71C */
}
#pragma pop

/* 0x8010FA54 | 0xA0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GScolsy2UtilGetCpPlanePoint(void* out, void* normal, void* verts, void* point) {
    /* TODO: match -- 160 bytes at 0x8010FA54 */
}
#pragma pop
