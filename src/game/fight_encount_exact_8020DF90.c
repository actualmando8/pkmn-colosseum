/**
 * @file fight_encount_exact_8020DF90.c
 * @brief Strict encounter field accessors, 0x8020DF90-0x8020E020.
 */

#include "game/colosseum.h"

typedef struct FightEncountTrainerSlot {
    u16 fightTrainerDataId;
    u16 pad_02;
    u32 gsInputDevice;
} FightEncountTrainerSlot;

typedef struct FightEncountData {
    u8 fightKind;
    u8 trainer;
    u8 zenmetuFlag;
    u8 pad_03;
    u16 fightFloorDataId;
    u16 pad_06;
    u32 fightName;
    u32 bgmSndId;
    u32 wipeId;
    u32 syoukaiWzxDataId;
    FightEncountTrainerSlot trainerSlots[4];
} FightEncountData;

void fightEncountDataBiosSetFightFloorDataId(FightEncountData* ptr, u16 val)
{
    if (ptr == NULL) {
        return;
    }
    ptr->fightFloorDataId = val;
}

void fightEncountDataBiosSetTrainer(FightEncountData* ptr, u8 val)
{
    if (ptr == NULL) {
        return;
    }
    ptr->trainer = val;
}

void fightEncountDataBiosSetFightKind(FightEncountData* ptr, u8 val)
{
    if (ptr == NULL) {
        return;
    }
    ptr->fightKind = val;
}

u32 fightEncountDataBiosGetSyoukaiWzxDataId(FightEncountData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->syoukaiWzxDataId;
}

u8 fightEncountDataBiosGetZenmetuFlag(FightEncountData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->zenmetuFlag;
}

u32 fightEncountDataBiosGetWipeId(FightEncountData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->wipeId;
}

u32 fightEncountDataBiosGetBgmSndId(FightEncountData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->bgmSndId;
}
