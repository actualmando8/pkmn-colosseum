/**
 * @file gs_range_801DE698.c
 * @brief gs-engine, 0x801DE698 - 0x801DF790.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) -- mixed-block split pass, 2026-07-01.
 * All functions asm-only until matched.
 *
 * The 10 functions below (0x801DE698-0x801DF474, the full function count
 * for this TU's declared range) previously lived, misattributed, in
 * game/battle/battle_waza.c (whose splits.txt range ends at 0x801DE698);
 * relocated here so this unit's real C source is scored where it belongs.
 */
#include "dolphin/types.h"

/**
 * fn_801DE698 - Waza stat change effect.
 * Address: 0x801DE698 | Size: 0x5CC
 */
void fn_801DE698(s32 arg0, s32 arg1) {
    /* TODO: Stat change visual effect (0x5CC bytes)
     * Displays the up/down arrow and color flash for stat changes.
     */
}

/**
 * _eyeTexAnimEnded - Waza stat change update.
 * Address: 0x801DEC64 | Size: 0x1B0
 */
void _eyeTexAnimEnded(s32 arg0, s32 arg1) {
    /* TODO: Stat change effect update (0x1B0 bytes) */
}

/**
 * fn_801DEE14 - Waza status effect visual.
 * Address: 0x801DEE14 | Size: 0xF8
 */
void fn_801DEE14(u8* obj) {
    extern void GSmodelLinkTexAnimToAnim(void* model, u32 enable);
    extern void GSmodelSetAnimIndex(void* model, u32 index);
    extern void GSmodelSetAnimType(void* model, u32 type);
    extern void GSmodelSetAnimRate(void* model, f32 rate);
    extern void GSmodelSetAnimFrame(void* model, f32 frame);
    extern void GSmodelStartAnimation(void* model);
    extern void GSmodelSetAnimEndedCallback(void* model, void* callback, void* arg);
    extern f32 lbl_8047E3C8;
    extern f32 lbl_8047E3CC;
    void* model;
    u8* entry;
    s32 count;
    u32 animIndex;

    if (obj == 0) {
        return;
    }

    model = *(void**)(obj + 0x24);
    obj[0x19] = 0;
    GSmodelLinkTexAnimToAnim(model, 1);

    entry = *(u8**)(obj + 0x2C) + 0x8D4;
    count = *(s32*)(*(u8**)(obj + 0x2C) + 0x84C);
    while (count-- > 0) {
        if (*(s32*)entry == 0) {
            animIndex = *(u32*)(entry + 4);
            goto found;
        }
        entry += 8;
    }
    animIndex = 0;

found:
    GSmodelSetAnimIndex(model, animIndex);

    if (obj[0x75] != 0) {
        GSmodelSetAnimType(model, 0);
    } else {
        GSmodelSetAnimType(model, 1);
    }
    GSmodelSetAnimRate(model, lbl_8047E3C8);
    GSmodelSetAnimFrame(model, lbl_8047E3CC);
    GSmodelStartAnimation(model);

    if (obj[0x75] != 0) {
        GSmodelSetAnimEndedCallback(model, 0, 0);
        obj[0x16] = 1;
    }
}

/**
 * fn_801DEF0C - Waza status effect update.
 * Address: 0x801DEF0C | Size: 0x164
 */
void fn_801DEF0C(void* obj, s32 arg1, s32 arg2) {
    /* TODO: Status effect visual update (0x164 bytes) */
}

/**
 * fn_801DF070 - Waza weather effect setup.
 * Address: 0x801DF070 | Size: 0xF0
 */
void fn_801DF070(u8* obj, u32 animIndex, u32 animType) {
    extern void GSmodelLinkTexAnimToAnim(void* model, u32 enable);
    extern void GSmodelSetAnimIndex(void* model, u32 index);
    extern void GSmodelSetAnimType(void* model, u32 type);
    extern void GSmodelSetAnimRate(void* model, f32 rate);
    extern void GSmodelSetAnimFrame(void* model, f32 frame);
    extern void GSmodelStartAnimation(void* model);
    void* model;

    if (obj == 0) {
        return;
    }

    if (obj[0x75] == 0) {
        switch (*(u16*)(obj + 0x32)) {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 10:
            animType = 1;
            break;
        }
    }

    if ((obj[0x18] & 8) == 8) {
        return;
    }

    model = *(void**)(obj + 0x24);
    if ((obj[0x18] & 4) != 4) {
        obj[0x19] = 0;
        GSmodelLinkTexAnimToAnim(model, 1);
    }
    GSmodelSetAnimIndex(model, animIndex);
    GSmodelSetAnimType(model, animType);
    GSmodelSetAnimRate(model, 1.0f);
    GSmodelSetAnimFrame(model, 0.0f);
    GSmodelStartAnimation(model);
}

/**
 * fn_801DF160 - Waza weather effect update.
 * Address: 0x801DF160 | Size: 0x70
 */
s32 fn_801DF160(u8* obj) {
    u8* base;
    u8* table;
    s32* entry;
    s32 count;

    base = *(u8**)(obj + 0x2C);
    table = base;
    if ((obj[0x18] & 2) == 2 && *(u16*)(obj + 0x14) > 0x10) {
        table = base + 0xD40;
        if (*(volatile s32*)(table + 0x94) == 1) {
            table = base;
        }
    }

    count = *(s32*)(table + 4);
    entry = (s32*)(table + 0x8C);
    while (count-- > 0) {
        if (entry[0] == 0) {
            return entry[1];
        }
        entry += 2;
    }
    return 0;
}

/**
 * fn_801DF1D0 - Waza weather effect render.
 * Address: 0x801DF1D0 | Size: 0x16C
 */
void fn_801DF1D0(void* obj) {
    /* TODO: Weather effect render (0x16C bytes) */
}

/**
 * fn_801DF33C - Waza weather effect clear.
 * Address: 0x801DF33C | Size: 0x98
 */
void fn_801DF33C(u8* obj) {
    extern void GSmodelLinkTexAnimToAnim(void* model, u32 enable);
    extern void GSmodelSetTexAnimIndex(void* model, u32 index);
    extern void GSmodelSetTexAnimRate(void* model, f32 rate);
    extern void GSmodelSetTexAnimType(void* model, u32 type);
    extern void GSmodelSetTexAnimFrame(void* model, f32 frame);
    extern void GSmodelStartTexAnimation(void* model);
    void* model = *(void**)(obj + 0x24);

    if (*(s16*)(obj + 0x1E) >= 0 && obj[0x19] == 6) {
        obj[0x19] = 2;
        GSmodelLinkTexAnimToAnim(model, 0);
        GSmodelSetTexAnimIndex(model, *(s16*)(obj + 0x1E));
        GSmodelSetTexAnimRate(model, 1.0f);
        GSmodelSetTexAnimType(model, 0);
        GSmodelSetTexAnimFrame(model, 0.0f);
        GSmodelStartTexAnimation(model);
    }
}

/**
 * fn_801DF3D4 - Waza weather get type.
 * Address: 0x801DF3D4 | Size: 0xA0
 */
void fn_801DF3D4(u8* obj) {
    extern void GSmodelLinkTexAnimToAnim(void* model, u32 enable);
    extern void GSmodelSetTexAnimIndex(void* model, u32 index);
    extern void GSmodelSetTexAnimRate(void* model, f32 rate);
    extern void GSmodelSetTexAnimType(void* model, u32 type);
    extern void GSmodelSetTexAnimFrame(void* model, f32 frame);
    extern void GSmodelStartTexAnimation(void* model);
    void* model = *(void**)(obj + 0x24);

    if (*(s16*)(obj + 0x1A) >= 0 && obj[0x19] != 6 && obj[0x19] != 1) {
        obj[0x19] = 1;
        GSmodelLinkTexAnimToAnim(model, 0);
        GSmodelSetTexAnimIndex(model, *(s16*)(obj + 0x1A));
        GSmodelSetTexAnimRate(model, 1.0f);
        GSmodelSetTexAnimType(model, 0);
        GSmodelSetTexAnimFrame(model, 0.0f);
        GSmodelStartTexAnimation(model);
    }
}

/**
 * fn_801DF474 - Waza ability effect handler.
 * Address: 0x801DF474 | Size: 0x31C
 */
void fn_801DF474(s32 slot, s32 abilityID) {
    /* TODO: Ability effect handler (0x31C bytes)
     * Handles visual effects for ability activations
     * (Intimidate, Levitate, etc.).
     */
}
