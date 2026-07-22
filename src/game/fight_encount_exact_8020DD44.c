/**
 * @file fight_encount_exact_8020DD44.c
 * @brief Strict encounter audio, wipe, and basic field helpers,
 *        0x8020DD44-0x8020DF10.
 */

#include "game/colosseum.h"

typedef struct FightEncountWipeData {
    u8 snapshotUse;
    u8 pad_01;
    u16 wipeEffectSndId;
    f32 wipeEffectTime;
    u32 wipeFunction;
} FightEncountWipeData;

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

extern FightEncountWipeData fight_encount_wipe_data[];

u32 fightEncountGetEnvSndDataId(u16 encountDataIndex)
{
    extern s32 fightFloorGetStatus(u8*, u32, u32, u32);
    extern u32 fightEncountDataBiosGetFightFloorDataId(FightEncountData* ptr);
    extern FightEncountData* fightEncountDataBiosGetPtr(u16 index);
    FightEncountData* encountData;
    u32 fightFloorDataId;

    encountData = fightEncountDataBiosGetPtr(encountDataIndex);
    fightFloorDataId = fightEncountDataBiosGetFightFloorDataId(encountData);
    return fightFloorGetStatus(NULL, fightFloorDataId, 0x7, 0);
}

u32 fightEncountGetBgmSndDataId(u16 encountDataIndex)
{
    extern s32 fightFloorGetStatus(u8*, u32, u32, u32);
    extern u32 fightTrainerGetStatus(u32, u16, u32, u16);
    extern u32 fightTrainerKindDataBiosGetBgmSndId(u8* trainerKindData);
    extern u8* fightTrainerKindDataBiosGetPtr(u16);
    extern u32 fightEncountDataBiosGetBgmSndId(FightEncountData* ptr);
    extern u16 fightEncountDataBiosGetFightTrainerDataId(FightEncountData* base, u8 slot);
    extern u32 fightEncountDataBiosGetFightFloorDataId(FightEncountData* ptr);
    extern FightEncountData* fightEncountDataBiosGetPtr(u16 index);
    FightEncountData* encountData;
    u32 bgmSndId;
    u32 fightFloorDataId;
    u16 fightTrainerDataId;
    u16 trainerStatus;
    u16 i;

    encountData = fightEncountDataBiosGetPtr(encountDataIndex);
    bgmSndId = fightEncountDataBiosGetBgmSndId(encountData);
    if (bgmSndId != 0) {
        return bgmSndId;
    }
    fightFloorDataId = fightEncountDataBiosGetFightFloorDataId(fightEncountDataBiosGetPtr(encountDataIndex));
    bgmSndId = fightFloorGetStatus(0, fightFloorDataId, 6, 0);
    if (bgmSndId != 0) {
        return bgmSndId;
    }
    for (i = 0; i < 4; i++) {
        fightTrainerDataId = fightEncountDataBiosGetFightTrainerDataId(encountData, (u8)i);
        if (fightTrainerDataId != 0) {
            trainerStatus = fightTrainerGetStatus(0, fightTrainerDataId, 4, 0);
            if (trainerStatus != 0) {
                bgmSndId = fightTrainerKindDataBiosGetBgmSndId(
                    fightTrainerKindDataBiosGetPtr(trainerStatus));
                if (bgmSndId != 0) {
                    return bgmSndId;
                }
            }
        }
    }
    return 1;
}

u16 fightEncountDataBiosGetWipeEffectSndID(FightEncountWipeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->wipeEffectSndId;
}

f32 fightEncountDataBiosGetWipeEffectTime(FightEncountWipeData* ptr)
{
    extern f32 lbl_8047E530;

    if (ptr == NULL) {
        return lbl_8047E530;
    }
    return ptr->wipeEffectTime;
}

u8 fightEncountDataBiosGetWipeSnapshotUse(FightEncountWipeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->snapshotUse;
}

u32 fightEncountDataBiosGetWipeFunction(FightEncountWipeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->wipeFunction;
}

FightEncountWipeData* fightEncountWipeDataBiosGetPtr(u32 index)
{
    extern u32 lbl_80478D20;

    if (index >= lbl_80478D20) {
        return NULL;
    }
    return &fight_encount_wipe_data[index];
}

u32 fightEncountDataBiosGetFightName(FightEncountData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->fightName;
}

void fightEncountDataBiosSetSyoukaiWzxDataId(FightEncountData* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    ptr->syoukaiWzxDataId = val;
}

void fightEncountDataBiosSetBgmSndId(FightEncountData* ptr, u32 val)
{
    if (ptr == NULL) {
        return;
    }
    ptr->bgmSndId = val;
}
