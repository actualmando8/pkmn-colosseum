#include "game/battle/battle_waza_types.h"

u32 mailGetContents(s32 idx)
{
    WazaEntry* entry;
    if (idx < 0 || (u32)idx >= *lbl_80478E98) {
        entry = NULL;
    } else {
        entry = &lbl_80478E9C[idx];
    }
    if (entry == NULL) return 0;
    return entry->field_14;
}

u32 mailGetSenderName(s32 idx)
{
    WazaEntry* entry;
    if (idx < 0 || (u32)idx >= *lbl_80478E98) {
        entry = NULL;
    } else {
        entry = &lbl_80478E9C[idx];
    }
    if (entry == NULL) return 0;
    return entry->field_0C;
}

u32 mailGetSubject(s32 idx)
{
    WazaEntry* entry;
    if (idx < 0 || (u32)idx >= *lbl_80478E98) {
        entry = NULL;
    } else {
        entry = &lbl_80478E9C[idx];
    }
    if (entry == NULL) return 0;
    return entry->field_10;
}
