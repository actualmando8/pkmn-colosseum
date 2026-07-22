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

u32 fightEncountDataBiosGetGSInputDevice(FightEncountData* data, u8 slot)
{
    FightEncountTrainerSlot* trainer;

    if (data == NULL) {
        trainer = NULL;
    } else if (slot >= 4) {
        trainer = NULL;
    } else {
        trainer = &data->trainerSlots[slot];
    }
    if (trainer == NULL) {
        return 0;
    }
    return trainer->gsInputDevice;
}

u16 fightEncountDataBiosGetFightTrainerDataId(FightEncountData* data,
                                               u8 slot)
{
    FightEncountTrainerSlot* trainer;

    if (data == NULL) {
        trainer = NULL;
    } else if (slot >= 4) {
        trainer = NULL;
    } else {
        trainer = &data->trainerSlots[slot];
    }
    if (trainer == NULL) {
        return 0;
    }
    return trainer->fightTrainerDataId;
}
