#include "game/battle/battle_waza_types.h"

u32 fn_801D13E4(s32 idx)
{
    WazaEntry* entry;

    if (idx < 0 || (u32)idx >= *lbl_80478E98) {
        entry = NULL;
    } else {
        entry = &lbl_80478E9C[idx];
    }
    if (entry == NULL) {
        return 0xFFFF;
    }
    return entry->field_04;
}
