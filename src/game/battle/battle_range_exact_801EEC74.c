#include "dolphin/types.h"

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

extern BattleRangeDef* lbl_80478F6C;
extern u8 fn_801902E0(u16 flag);
extern void _flagSet(u16 flag, s32 value);

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
