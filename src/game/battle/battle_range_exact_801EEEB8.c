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
extern u8* fn_801EF1E4(u32 data);

u16 fn_801EEEB8(u16 id)
{
    u8* data = fn_801EF1E4(0);

    if (data != NULL) {
        return *(u16*)(data + id * 0x18 + 0x4A6);
    }
    return 0;
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

u32 fn_801EEF40(u16 id)
{
    BattleRangeDef* def = &lbl_80478F6C[id];
    u16 slot;
    u8* data;

    if (id == 0 || id > 0x60) {
        def = NULL;
    }
    if (def != NULL) {
        slot = def->runtimeSlot;
        data = fn_801EF1E4(0);
        return *(u32*)(data + slot * 0xC + 0xC);
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
