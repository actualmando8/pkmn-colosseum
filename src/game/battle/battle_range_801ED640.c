/**
 * @file battle_range_801ED640.c
 * @brief battle-domain (direct calls into battle_*.c), 0x801ED640 - 0x801EF02C.
 *
 * Boundary evidence-verified from asm (sdata clusters, callee families,
 * static linkage, call chains) — mixed-block split pass, 2026-07-01.
 * Small accessors/helpers are decompiled below; larger render/runtime helpers
 * remain to be matched.
 */
#include "dolphin/types.h"

#pragma use_lmw_stmw on

typedef struct BattleRangeVec {
    f32 x;
    f32 y;
    f32 z;
} BattleRangeVec;

typedef struct BattleRangeDef {
    u8 type;
    u8 pad01;
    u16 field02;
    u16 field04;
    u16 field06;
    u16 field08;
    u16 runtimeSlot;
    u16 flag0C;
    u16 flag0E;
    u16 pad10;
    u16 flag12;
    u16 flag14;
    u16 flag16;
    u16 pad18;
    u16 pad1A;
    u16 flag1C;
    u16 actionListId;
    u16 pad20;
    u16 pad22;
    u16 indexedFlags[10];
} BattleRangeDef;

typedef struct BattleRangeIndexedEntry {
    u8 type;
    u8 variant;
    u8 pad02[2];
    u32 value;
} BattleRangeIndexedEntry;

typedef struct BattleRangeIndexedHeader {
    u32 count;
} BattleRangeIndexedHeader;

extern u8 lbl_8047B5C0;
extern u8 lbl_8047B5C1;

extern BattleRangeVec lbl_80375230;
extern u16 lbl_80375240[];
extern BattleRangeDef* lbl_80478F6C;
extern BattleRangeIndexedHeader* lbl_80478F78;
extern BattleRangeIndexedEntry* lbl_80478F7C;

extern void GSvecCopy(void* dst, void* src);
extern u32 GSgappCreate(s32 state, u8 priority, u32 param, void* func);
extern u8 fn_801902E0(u16 flag);
extern u8 fn_801906A0(u16 flag);
extern void _flagSet(u16 flag, s32 value);

void fn_801ED780(void);

void fn_801ED640(u8 value)
{
    lbl_8047B5C1 = value;
}

void fn_801ED648(BattleRangeVec* value)
{
    GSvecCopy(&lbl_80375230, value);
}

void fn_801ED674(void)
{
    lbl_8047B5C0 = 0;
}

void fn_801ED740(void)
{
    lbl_8047B5C0 = 0;
    lbl_8047B5C1 = 0;
    GSgappCreate(1, 0xF0, 0xA, fn_801ED780);
}

u32 fn_801EE034(BattleRangeIndexedEntry* entry)
{
    if (entry == NULL) {
        return 0;
    }
    return entry->value;
}

u8 fn_801EE04C(BattleRangeIndexedEntry* entry)
{
    if (entry == NULL) {
        return 0;
    }
    return entry->variant;
}

u8 fn_801EE064(BattleRangeIndexedEntry* entry)
{
    if (entry == NULL) {
        return 0;
    }
    return entry->type;
}

BattleRangeIndexedEntry* fn_801EE07C(u32 index)
{
    u32 id = index & 0xFFFF;
    BattleRangeIndexedHeader* header = lbl_80478F78;

    if (id >= header->count) {
        return lbl_80478F7C;
    }
    return &lbl_80478F7C[id];
}

#pragma peephole off
u16 fn_801EE0A8(u32 idx)
{
    extern u16* lbl_80478F74;
    u8 i;

    i = idx;
    return lbl_80478F74[i];
}
#pragma peephole on

u16 fn_801EE440(u16 index)
{
    if (index > 0x30) {
        return 0;
    }
    return lbl_80375240[index];
}

u16 fn_801EE468(void)
{
    return 0x30;
}

s32 fn_801EE614(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (def->flag1C != 0) {
            return (s8)fn_801906A0(def->flag1C);
        }
        return 0;
    }
    return 0;
}

s32 fn_801EE824(u16 id, u16 index)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        u16 flag = def->indexedFlags[index];

        if (flag != 0) {
            return (s8)fn_801906A0(flag);
        }
        return 0;
    }
    return 0;
}

void fn_801EE894(u16 id, u16 index, s8 value)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        u16 flag = def->indexedFlags[index];

        if (flag != 0) {
            _flagSet(flag, value);
        }
    }
}

u8 fn_801EE8F4(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (def->flag16 != 0) {
            return fn_801902E0(def->flag16);
        }
        return 0;
    }
    return 0;
}

u8 fn_801EEAD0(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (def->flag12 != 0) {
            return fn_801902E0(def->flag12);
        }
        return 0;
    }
    return 0;
}

u8 fn_801EEC74(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (def->flag14 != 0) {
            return fn_801902E0(def->flag14);
        }
        return 0;
    }
    return 0;
}

void fn_801EECD8(u16 id, u8 value)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL && def->flag14 != 0) {
        _flagSet(def->flag14, value);
    }
}

void fn_801EED30(u16 id, u8 value)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL && def->flag0E != 0) {
        _flagSet(def->flag0E, value);
    }
}

u8 fn_801EED88(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (def->flag0C != 0) {
            return fn_801902E0(def->flag0C);
        }
        return 0;
    }
    return 0;
}

void fn_801EEDEC(u16 id, u8 value)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL && def->flag0C != 0) {
        _flagSet(def->flag0C, value);
    }
}

u8 fn_801EEE44(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    return def->type;
}

u16 fn_801EEF08(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        return def->field08;
    }
    return 0;
}

u16 fn_801EEFAC(u16 id, s32 side)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        if (side == 0) {
            return def->field04;
        }
        return def->field06;
    }
    return 0;
}

u16 fn_801EEFF4(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        return def->field02;
    }
    return 0;
}
