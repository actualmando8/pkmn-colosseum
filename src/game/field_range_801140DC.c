/**
 * @file field_range_801140DC.c
 * @brief field code, 0x801140DC - 0x8011432C (8 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 *
 * Boundary bug fix: the 8 functions below were physically sitting in
 * game/gs_field_colquery.c -- past that unit's own splits.txt range
 * (0x8010F6A0-0x801140DC) -- while this unit's declared range already
 * covers them exactly (8 fns, matching the count in this file's own
 * header above). Relocated here during the gs_field_colquery.c 6-way
 * split so this unit scores real progress instead of 0%.
 */
#include "dolphin/types.h"

extern u8 lbl_80408378[];

/* 0x801140DC | 0x90 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void fn_801140DC(u32 material, u32 palette, u8 slot) {
#pragma optimization_level 4
    extern u32 fn_80128E24(void);
    extern void* fn_80128E04(void);
    extern void gamedatasaveSetStatus(void*, u32, u32);
    void* obj;

    if (fn_80128E24() != 0) {
        obj = fn_80128E04();
        if (obj != 0) {
            gamedatasaveSetStatus(obj, 5, material);
            gamedatasaveSetStatus(obj, 7, palette);
            gamedatasaveSetStatus(obj, 8, slot);
        }
    }
}
#pragma peephole on
#pragma pop

/* 0x8011416C | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011416C(u32 material, u32 palette, u8 materialSlot) {
#pragma optimization_level 4
    u8* state = lbl_80408378;

    *(u32*)(state + 0x48) = material;
    *(u32*)(state + 0x4C) = palette;
    *(u8*)(state + 0x50) = materialSlot;
    *(u8*)(state + 0x51) = 1;
}
#pragma pop

/* 0x8011418C | 0x4C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_8011418C(u32* texture, u32* palette, u8* textureSlot) {
#pragma optimization_level 4
    if (texture != NULL) {
        *texture = *(u32*)(lbl_80408378 + 0x3C);
    }
    if (palette != NULL) {
        *palette = *(u32*)(lbl_80408378 + 0x40);
    }
    if (textureSlot != NULL) {
        *textureSlot = *(u8*)(lbl_80408378 + 0x44);
    }
}
#pragma pop

/* 0x801141D8 | 0x20 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_801141D8(u32 texture, u32 palette, u8 textureSlot) {
#pragma optimization_level 4
    u8* state = lbl_80408378;

    *(u32*)(state + 0x3C) = texture;
    *(u32*)(state + 0x40) = palette;
    *(u8*)(state + 0x44) = textureSlot;
    *(u8*)(state + 0x51) = 0;
}
#pragma pop

/* 0x801141F8 | 0x5C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void EvlogSet__FScUl(void) {
    /* TODO: match -- 92 bytes at 0x801141F8 */
}
#pragma pop

/* 0x80114254 | 0x60 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
s32 fn_80114254(s32 param1, s32 param2, s32 param3) {
#pragma optimization_level 4
    extern s32 GSresGetResource(s32);
    extern void fn_8017F484(s32, s32, s32);
    s32 result = GSresGetResource(param1);

    fn_8017F484(param1, param2, param3);
    return result;
}
#pragma peephole on
#pragma pop

/* 0x801142B4 | 0x44 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void* fn_801142B4(void* group, void* model, u32 size) {
#pragma optimization_level 4
    extern void* GSresAllocResourceAlign(u32 size, u32 alignment, void* group, void* model, u32 flags);
    void* result;

    result = GSresAllocResourceAlign((size + 0x1F) & ~0x1F, 0x20, group, model, 0);
    if (result == NULL) {
        result = NULL;
    }
    return result;
}
#pragma peephole on
#pragma pop

/* 0x801142F8 | 0x34 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#pragma peephole off
void* floorReadGFLPostFunc(u32 group, u32 modelId) {
#pragma optimization_level 4
    extern void* GSresGetResource(u32, u32);
    extern void fn_801ED680(void*);
    void* model;

    model = GSresGetResource(group, modelId);
    fn_801ED680(model);
    return model;
}
#pragma peephole on
#pragma pop
