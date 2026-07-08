/**
 * @file GScolsys2Thru_range_80110084.c
 * @brief GScolsys2Thru -- spatial grid / "thru" collision queries.
 *
 * Third of six translation units recovered from the former
 * game/gs_field_colquery.c CodeCandidate bucket (0x8010F6A0-0x801140DC).
 * Named with a _range_ suffix per convention pending call-graph
 * confirmation of the exact XD TU boundary.
 *
 * fn_801101B4 previously carried an invented "GSfield_BuildCollisionGrid"
 * name/signature from an earlier bad campaign pass (same class of issue
 * documented in include/game/gs_colsys.h); reverted to the standard
 * fn_<addr> placeholder since no confirmed symbols.txt name exists yet.
 * GScolsys2ThruGetFixedMdlEventList similarly carried an invented "GSfield_GridLookup" name;
 * renamed to its confirmed name below.
 *
 * Address range: 0x80110084 - 0x80111B9C
 */
#include "dolphin/types.h"
#include "game/world/gs_field.h"
#include "game/gs_field_colquery_types.h"

/* 0x80110084 | 0x130 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80110084(s32* outIndex, void* data) {
#pragma optimization_level 4
    extern void* GScolsys2GetCurFloor(void);
    u8* base;
    s32 index;
    u8* entry;
    s32 i;

    base = (u8*)GScolsys2GetCurFloor();
    if (base == NULL) {
        return 1;
    }
    entry = base;
    index = 0;
    for (i = 0; i < 6; i++) {
        if ((*(u16*)(entry + 0xa10) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa24) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa38) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa4c) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa60) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa74) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa88) & 1) == 0) goto found;
        index++;
        if ((*(u16*)(entry + 0xa9c) & 1) == 0) goto found;
        entry += 0xa0;
        index++;
    }
found:
    if (index >= 0x30) {
        return 5;
    }
    {
        u8* slot = base + index * 0x14;
        *(s32*)(slot + 0xa00) = *(s32*)((u8*)data + 0x0);
        *(s32*)(slot + 0xa04) = *(s32*)((u8*)data + 0x4);
        *(f32*)(slot + 0xa08) = *(f32*)((u8*)data + 0x8);
        *(f32*)(slot + 0xa0c) = *(f32*)((u8*)data + 0xc);
        *(u16*)(slot + 0xa10) = 0;
        *(u16*)(slot + 0xa10) |= 1;
        *outIndex = index;
    }
    return 0;
}
#pragma pop

/* 0x801101B4 | 0x4E8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801101B4(void* meshData) {
    /* TODO: match -- 1256 bytes at 0x801101B4 */
}
#pragma pop

/* 0x8011069C | 0x7C8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GScolsys2ThruGetFixedMdlEventList(f32 x, f32 z, void* outTriangles) {
    /* TODO: match -- 1992 bytes at 0x8011069C */
}
#pragma pop

/* 0x80110E64 | 0x60C */
#pragma push
#pragma optimizewithasm off
s32 GScolsys2ThruGetMdlEventList(GScolsys2Vec3* point, GScolsys2Vec3* dirVec, f32 radius,
                GScolsys2TriangleList* triList, void* mtxInv, void* mtxFwd,
                GSfieldQueryTriangle* outTris) {
    GScolsys2Triangle* tri;
    GSfieldQueryTriangle* out;
    s32 outCount;
    s32 triIdx;
    GSfieldQueryTriangle* scan;
    s32 scanIdx;
    s32 vertIdx;
    GScolsys2Vec3* vdst;
    GScolsys2Vec3* vsrc;
    s32 hit;
    f32 radiusSq;
    GSfieldEdgeMasks edgeMasksA;
    GSfieldEdgeMasks edgeMasksB;
    GScolsys2Vec3 planePoint;
    GScolsys2Vec3 cp;
    GScolsys2Vec3 lineCp;
    GScolsys2Vec3 verts[3];
    f32 lineT;
    u16 flags;

    radiusSq = radius * radius;
    tri = triList->triangles;
    out = outTris;
    outCount = 0;
    triIdx = 0;
    while ((u32)triIdx < triList->count && outCount < 4) {
        scan = outTris;
        for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
            if (tri->id == scan->id) {
                break;
            }
        }
        if (scanIdx >= outCount) {
            PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
            if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                vdst = verts;
                vsrc = tri->verts;
                vertIdx = 0;
                do {
                    PSMTXMultVec(mtxInv, vsrc, vdst);
                    vertIdx++;
                    vsrc++;
                    vdst++;
                } while (vertIdx < 3);
                if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                    hit = 0;
                } else {
                    GScolsy2UtilGetCpPlanePoint(&cp, &planePoint, verts, point);
                    if (PSVECSquareDistance(&cp, point) >= radiusSq) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(&cp, verts, &planePoint) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                }
                if (hit != 0) {
                    out->id = tri->id;
                    out->verts[0] = verts[0];
                    out->verts[1] = verts[1];
                    out->verts[2] = verts[2];
                    out->normal = planePoint;
                    outCount++;
                    out++;
                }
            }
        }
        triIdx++;
        tri++;
    }

    tri = triList->triangles;
    triIdx = 0;
    out = outTris + outCount;
    while ((u32)triIdx < triList->count && outCount < 4) {
        if ((tri->flags & 7) != 0) {
            scan = outTris;
            for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
                if (tri->id == scan->id) {
                    break;
                }
            }
            if (scanIdx >= outCount) {
                PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
                if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                    vdst = verts;
                    vsrc = tri->verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        vertIdx++;
                        vsrc++;
                        vdst++;
                    } while (vertIdx < 3);
                    edgeMasksA = lbl_8047CF48;
                    flags = tri->flags;
                    if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                        hit = 0;
                    } else {
                        vsrc = verts;
                        hit = 0;
                        for (vertIdx = 0; vertIdx < 3; vertIdx++, vsrc++) {
                            if ((flags & edgeMasksA.values[vertIdx]) != 0) {
                                s32 next = vertIdx + 1;
                                if (next >= 3) {
                                    next = 0;
                                }
                                lineT = GScolsys2UtilGetCpLinePoint(&lineCp, vsrc, &verts[next], point);
                                if (lineT >= 0.0f && lineT <= 1.0f
                                    && PSVECSquareDistance(&lineCp, point) < radiusSq) {
                                    hit = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (hit != 0) {
                        out->id = tri->id;
                        out->verts[0] = verts[0];
                        out->verts[1] = verts[1];
                        out->verts[2] = verts[2];
                        out->normal = planePoint;
                        outCount++;
                        out++;
                    }
                }
            }
        }
        triIdx++;
        tri++;
    }

    tri = triList->triangles;
    triIdx = 0;
    out = outTris + outCount;
    while ((u32)triIdx < triList->count && outCount < 4) {
        if ((tri->flags & 7) != 0) {
            scan = outTris;
            for (scanIdx = 0; scanIdx < outCount; scanIdx++, scan++) {
                if (tri->id == scan->id) {
                    break;
                }
            }
            if (scanIdx >= outCount) {
                PSMTXMultVec(mtxFwd, &tri->normal, &planePoint);
                if (PSVECDotProduct(&planePoint, dirVec) < 0.0f) {
                    vdst = verts;
                    vsrc = tri->verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, vsrc, vdst);
                        vertIdx++;
                        vsrc++;
                        vdst++;
                    } while (vertIdx < 3);
                    edgeMasksB = lbl_8047CF50;
                    flags = tri->flags;
                    if (GScolsy2UtilGetSidePlanePoint(&planePoint, verts, point) < 0.0f) {
                        hit = 0;
                    } else {
                        vsrc = verts;
                        hit = 0;
                        for (vertIdx = 0; vertIdx < 3; vertIdx++, vsrc++) {
                            s32 next = vertIdx + 2;
                            if (next >= 3) {
                                next -= 3;
                            }
                            if ((flags & edgeMasksB.values[vertIdx]) != 0 && (flags & edgeMasksB.values[next]) != 0) {
                                if (PSVECSquareDistance(vsrc, point) < radiusSq) {
                                    hit = 1;
                                    break;
                                }
                            }
                        }
                    }
                    if (hit != 0) {
                        out->id = tri->id;
                        out->verts[0] = verts[0];
                        out->verts[1] = verts[1];
                        out->verts[2] = verts[2];
                        out->normal = planePoint;
                        outCount++;
                        out++;
                    }
                }
            }
        }
        triIdx++;
        tri++;
    }
    return outCount;
}
#pragma pop

/* 0x80111470 | 0x1CC */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GScolsys2ThruGetEventList(void) {
    /* TODO: match -- 460 bytes at 0x80111470 */
}
#pragma pop

/* 0x8011163C | 0x228 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GScolsys2ThruGetEventID(void) {
    /* TODO: match -- 552 bytes at 0x8011163C */
}
#pragma pop

/* 0x80111864 | 0x338 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 fn_80111864(void* a, void* b, void* c) {
#pragma optimization_level 4
    extern f32 PSVECDotProduct(void* a, void* b);
    extern s32 GScolsys2UtilGetCpPlaneLine(void* a, void* b, void* c, void* d, void* e, void* f);
    extern s32 GScolsy2UtilChkInTri(void* a, void* b, void* c);
    extern f32 lbl_8047CF60;
    extern f32 lbl_8047CF64;
    u8* wzx;
    u8* region;
    u8* triList;
    u8* tri;
    u8* tempWrite;
    u8* tempRead;
    u8* out;
    u8* scan;
    s32 outCount;
    s32 outOffset;
    s32 regionIdx;
    s32 tempCount;
    s32 triIdx;
    s32 scanIdx;
    s32 vertIdx;
    s32 visible;
    s32 hit;
    f32 resultT;
    u8 temp[0xD0];
    f32 mtxInv[12];
    f32 mtxFwd[12];
    f32 verts[9];
    f32 dirVec[3];
    f32 planePoint[3];
    f32 hitPoint[3];

    outCount = 0;
    outOffset = 0;
    wzx = (u8*)fn_8010CBC0();
    PSVECSubtract(b, a, dirVec);
    region = *(u8**)wzx;
    regionIdx = 0;
    while ((u32)regionIdx < *(u32*)(wzx + 4)) {
        GScolsys2GetObjEnable(regionIdx, &visible);
        if (visible != 0) {
            triList = *(u8**)(region + 0x30);
            if (triList != NULL) {
                fn_8010CA30(mtxInv, regionIdx);
                fn_8010C8D0(mtxFwd, regionIdx);
                tempWrite = temp;
                tempCount = 0;
                tri = *(u8**)triList;
                triIdx = 0;
                while ((u32)triIdx < *(u32*)(triList + 4)) {
                    scan = temp;
                    scanIdx = 0;
                    if (tempCount > 0) {
                        do {
                            if (*(u16*)(tri + 0x30) == *(u16*)(scan + 0x30)) {
                                break;
                            }
                            scan += 0x34;
                            scanIdx++;
                        } while (scanIdx < tempCount);
                    }
                    if (scanIdx < tempCount) {
                        goto next_triangle;
                    }
                    PSMTXMultVec(mtxFwd, tri + 0x24, planePoint);
                    if (PSVECDotProduct(planePoint, dirVec) >= lbl_8047CF60) {
                        goto next_triangle;
                    }
                    scan = (u8*)verts;
                    vertIdx = 0;
                    do {
                        PSMTXMultVec(mtxInv, tri + (vertIdx * 0xC), scan);
                        vertIdx++;
                        scan += 0xC;
                    } while (vertIdx < 3);
                    if (GScolsys2UtilGetCpPlaneLine(hitPoint, &resultT, planePoint, verts, a, b) == 0) {
                        hit = 0;
                    } else if ((resultT < lbl_8047CF60) || (resultT > lbl_8047CF64)) {
                        hit = 0;
                    } else if (GScolsy2UtilChkInTri(hitPoint, verts, planePoint) == 0) {
                        hit = 0;
                    } else {
                        hit = 1;
                    }
                    if (hit != 0) {
                        *(u32*)(tempWrite + 0x00) = *(u32*)((u8*)verts + 0x00);
                        *(u32*)(tempWrite + 0x04) = *(u32*)((u8*)verts + 0x04);
                        *(u32*)(tempWrite + 0x08) = *(u32*)((u8*)verts + 0x08);
                        *(u32*)(tempWrite + 0x0C) = *(u32*)((u8*)verts + 0x0C);
                        *(u32*)(tempWrite + 0x10) = *(u32*)((u8*)verts + 0x10);
                        *(u32*)(tempWrite + 0x14) = *(u32*)((u8*)verts + 0x14);
                        *(u32*)(tempWrite + 0x18) = *(u32*)((u8*)verts + 0x18);
                        *(u32*)(tempWrite + 0x1C) = *(u32*)((u8*)verts + 0x1C);
                        *(u32*)(tempWrite + 0x20) = *(u32*)((u8*)verts + 0x20);
                        *(u32*)(tempWrite + 0x24) = *(u32*)((u8*)planePoint + 0x00);
                        *(u32*)(tempWrite + 0x28) = *(u32*)((u8*)planePoint + 0x04);
                        *(u32*)(tempWrite + 0x2C) = *(u32*)((u8*)planePoint + 0x08);
                        *(u16*)(tempWrite + 0x30) = *(u16*)(tri + 0x30);
                        tempWrite += 0x34;
                        tempCount++;
                    }
                next_triangle:
                    triIdx++;
                    tri += 0x34;
                    if (tempCount >= 4) {
                        break;
                    }
                }
                tempRead = temp;
                triIdx = 0;
                while (triIdx < tempCount) {
                    scan = (u8*)c;
                    scanIdx = 0;
                    if (outCount > 0) {
                        do {
                            if (*(u16*)(scan + 0x30) == *(u16*)(tempRead + 0x30)) {
                                break;
                            }
                            scan += 0x34;
                            scanIdx++;
                        } while (scanIdx < outCount);
                    }
                    if (scanIdx < outCount) {
                        goto next_temp;
                    }
                    out = (u8*)c + outOffset;
                    *(u32*)(out + 0x00) = *(u32*)(tempRead + 0x00);
                    *(u32*)(out + 0x04) = *(u32*)(tempRead + 0x04);
                    *(u32*)(out + 0x08) = *(u32*)(tempRead + 0x08);
                    *(u32*)(out + 0x0C) = *(u32*)(tempRead + 0x0C);
                    *(u32*)(out + 0x10) = *(u32*)(tempRead + 0x10);
                    *(u32*)(out + 0x14) = *(u32*)(tempRead + 0x14);
                    *(u32*)(out + 0x18) = *(u32*)(tempRead + 0x18);
                    *(u32*)(out + 0x1C) = *(u32*)(tempRead + 0x1C);
                    *(u32*)(out + 0x20) = *(u32*)(tempRead + 0x20);
                    *(u32*)(out + 0x24) = *(u32*)(tempRead + 0x24);
                    *(u32*)(out + 0x28) = *(u32*)(tempRead + 0x28);
                    *(u32*)(out + 0x2C) = *(u32*)(tempRead + 0x2C);
                    *(u32*)(out + 0x30) = *(u32*)(tempRead + 0x30);
                    outCount++;
                    outOffset += 0x34;
                next_temp:
                    tempRead += 0x34;
                    triIdx++;
                    if (outCount >= 4) {
                        break;
                    }
                }
            }
        }
        regionIdx++;
        region += 0x40;
        if (outCount >= 4) {
            break;
        }
    }
    return outCount;
}
#pragma pop
