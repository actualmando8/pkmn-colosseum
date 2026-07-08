/**
 * @file gamedataBios.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80135A30 - 0x80135B1C
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80135A30 | 0x10 | nc_setter */
void gamedataAttestBiosSetLangareaId(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_03 = val;
}


/* 0x80135A40 | 0x10 | nc_setter */
void gamedataAttestBiosSetAreaId(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_02 = val;
}


/* 0x80135A50 | 0x10 | nc_setter */
void gamedataAttestBiosSetGenId(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_01 = val;
}


/* 0x80135A60 | 0x10 | nc_setter */
void gamedataAttestBiosSetVerId(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->word0.byte.field_00 = val;
}


/* 0x80135A70 | 0x18 | nc_getter */
u8 gamedataAttestBiosGetLangareaId(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_03;
}


/* 0x80135A88 | 0x18 | nc_getter */
u8 gamedataAttestBiosGetAreaId(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_02;
}


/* 0x80135AA0 | 0x18 | nc_getter */
u8 gamedataAttestBiosGetGenId(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_01;
}


/* 0x80135AB8 | 0x18 | nc_getter */
u8 gamedataAttestBiosGetVerId(void* ptr) {
    if (ptr == NULL) { return 0; }
    return ((EffectParamBlock*)ptr)->word0.byte.field_00;
}


/* 0x80135AD0 | 0x1C */
#if 0
asm void gamedataAttestBiosCopy(void) {
#include "src/game/effect/effect_util_fn_80135AD0.inc"
}
#else
#pragma optimization_level 4
void gamedataAttestBiosCopy(void* dst, void* src) {
    if (dst == 0) return;
    if (src == 0) return;
    *(u32*)dst = *(u32*)src;
}
#endif


/* 0x80135AEC | 0x20 */
/* Copy the first u32 from src to dst, if both are non-NULL. */
void gamedataBiosSetGamedataAtttestPtr(u32* dst, u32* src) {
    if (dst == NULL || src == NULL) {
        return;
    }
    *dst = *src;
}


/* 0x80135B0C | 16 bytes | nc_bnelr */
u32 gamedataBiosGetGamedataAtttestPtr(void* ptr) {
    if (ptr != NULL) { return (u32)ptr; }
    return 0;
}
