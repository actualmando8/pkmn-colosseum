/**
 * @file gamedatasaveBios.c
 * @brief Decompiled functions.
 *
 * Address range: 0x80135B1C - 0x80135D10
 *
 * Split out of the former game/effect/effect_util.c CodeCandidate
 * bucket (0x8013151C - 0x80137114); see effect_util_types.h for
 * shared cross-TU declarations.
 */

#include "dolphin/types.h"
#include "game/effect/effect_util_types.h"


/* 0x80135B1C | 0x10 | nc_setter */
void gamedatasaveBiosSetOptionAudio(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_22 = val;
}


/* 0x80135B2C | 0x10 | nc_setter */
void gamedatasaveBiosSetOptionNoVibration(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_21 = val;
}


/* 0x80135B3C | 0x10 | nc_setter */
void gamedatasaveBiosSetFloorposindex(void* ptr, u8 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_20 = val;
}


/* 0x80135B4C | 0x10 | nc_setter */
void gamedatasaveBiosSetPrevfloorid(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_1C = val;
}


/* 0x80135B5C | 0x10 | nc_setter_f */
void gamedatasaveBiosSetPlaytime(void* ptr, f32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_18 = val;
}


/* 0x80135B6C | 0x10 | nc_setter */
void gamedatasaveBiosSetFloorid(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_14 = val;
}


/* 0x80135B7C | 0x10 | nc_setter */
void gamedatasaveBiosSetSavecount(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_08 = val;
}


/* 0x80135B8C | 0x14 */
#if 0
asm void gamedatasaveBiosSetMemcardID(void) {
#include "src/game/effect/effect_util_fn_80135B8C.inc"
}
#else
#pragma optimization_level 4
void gamedatasaveBiosSetMemcardID(void* ptr, u32 unused, u32 a, u32 b) {
    if (ptr == 0) return;
    ((EffectParamBlock*)ptr)->field_04 = b;
    ((EffectParamBlock*)ptr)->word0.word = a;
}
#endif


/* 0x80135BA0 | 0x10 | nc_setter */
void gamedatasaveBiosSetSavernd(void* ptr, u32 val) {
    if (ptr == NULL) { return; }
    ((EffectParamBlock*)ptr)->field_0C = val;
}


/* 0x80135BB0 | 24 bytes | beq_default_getter */
u8 gamedatasaveBiosGetOptionAudio(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_22;
    }
    return 0;
}


/* 0x80135BC8 | 24 bytes | beq_default_getter */
u8 gamedatasaveBiosGetOptionNoVibration(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_21;
    }
    return 0;
}


/* 0x80135BE0 | 24 bytes | beq_default_getter */
u8 gamedatasaveBiosGetFloorposindex(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_20;
    }
    return 0;
}


/* 0x80135BF8 | 24 bytes | beq_default_getter */
u32 gamedatasaveBiosGetPrevfloorid(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_1C;
    }
    return 0;
}


/* 0x80135C10 | 0x18 */
#if 0
asm f32 gamedatasaveBiosGetPlaytime(void* ptr) {
#include "src/game/effect/effect_util_fn_80135C10.inc"
}
#else
f32 gamedatasaveBiosGetPlaytime(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_18;
    }
    return lbl_8047D110;
}
#endif


/* 0x80135C28 | 24 bytes | beq_default_getter */
u32 gamedatasaveBiosGetFloorid(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_14;
    }
    return 0;
}


/* 0x80135C40 | 24 bytes | beq_default_getter */
u32 gamedatasaveBiosGetSavecount(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_08;
    }
    return 0;
}


/* 0x80135C58 | 0x20 */
#if 0
asm void gamedatasaveBiosGetMemcardID(void) {
#include "src/game/effect/effect_util_fn_80135C58.inc"
}
#else
/* Returns the r3:r4 pair (two words at *p) as a u64, or 0 if p is NULL.
 * byte-match verified: 8/8 instrs vs target; decomp.me scratch O3Iat = 100%. */
u64 gamedatasaveBiosGetMemcardID(u32* p) {
    if (p) return *(u64*)p;
    return 0;
}
#endif


/* 0x80135C78 | 24 bytes | beq_default_getter */
u32 gamedatasaveBiosGetSavernd(void* ptr) {
    if (ptr != NULL) {
        return ((EffectParamBlock*)ptr)->field_0C;
    }
    return 0;
}


/* 0x40 | gamedatasaveBiosSetPtr | generic */
void gamedatasaveBiosSetPtr(void* dst, void* src) {
    if (dst == NULL) return;
    if (src != NULL) goto _copy;
    return;
_copy: {
    f64 f1 = *(f64*)((u8*)src + 0x00);
    f64 f0 = *(f64*)((u8*)src + 0x08);
    *(f64*)((u8*)dst + 0x08) = f1;
    *(f64*)((u8*)dst + 0x10) = f0;
    f1 = *(f64*)((u8*)src + 0x10);
    f0 = *(f64*)((u8*)src + 0x18);
    *(f64*)((u8*)dst + 0x18) = f1;
    *(f64*)((u8*)dst + 0x20) = f0;
    *(f64*)((u8*)dst + 0x28) = *(f64*)((u8*)src + 0x20);
    }
}


/* 0x80135CD0 | 24 bytes | nc_addi_ptr */
void* gamedatasaveBiosGetPtr(void* ptr) {
    if (ptr == NULL) { return NULL; }
    return (u8*)ptr + 0x8;
}


/* 0x80135CE8 | 0x28 */
#if 0
asm void gamedatasaveInit(void) {
#include "src/game/effect/effect_util_fn_80135CE8.inc"
}
#else
#pragma optimization_level 4
void gamedatasaveInit(void* ptr) {
    void* sub;
    if (ptr == 0) {
        sub = 0;
    } else {
        sub = (u8*)ptr + 0x8;
    }
    if (sub == 0) return;
    *(u32*)((u8*)sub + 0x8) = 0;
}
#endif
