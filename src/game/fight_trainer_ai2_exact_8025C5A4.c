/**
 * @file fight_trainer_ai2_exact_8025C5A4.c
 * @brief Strict exact prefix of fightTrainerAi2.cpp (0x8025C5A4-0x8025C6BC).
 */

#include "game/colosseum.h"

typedef u32 (*FightFloorCallback)(void*, u32, void*);

typedef struct HorobinoutaContext {
    void* trainer;
    u32 count;
} HorobinoutaContext;

extern u16 wazaGetStatus(void*, u32, u32, u32);
extern u32 fightFloorLoopValidFightOutPokemon(u32, FightFloorCallback, void*, u32);
extern u32 _fightTrainerAiCheckHorobinoutaSub(void*, u32, void*);

s32 fightTrainerAiCheckJoutaiKieWazaHitWazaDataId(
    void* trainer,
    u32 conditionId,
    u32 moveId,
    u32 unused)
{
    u32 value = wazaGetStatus(0, moveId, 9, 0) & 0xFFFF;

    if ((u16)conditionId == 0x1F &&
        (value == 0x92 || value == 0x95 || value == 0x98 || value == 0xCF)) {
        return 1;
    }
    if ((u16)conditionId == 0x20 && value == 0x93) {
        return 1;
    }
    if ((u16)conditionId == 0x21 &&
        ((u16)moveId == 0x39 || (u16)moveId == 0xFA)) {
        return 1;
    }
    if (value == 0x5E) {
        return 1;
    }
    return 0;
}

u32 fightTrainerAiCheckHorobinouta(void* trainer, u32 unused1, u32 unused2)
{
    HorobinoutaContext context;

    context.trainer = trainer;
    context.count = 0;
    fightFloorLoopValidFightOutPokemon(
        0, _fightTrainerAiCheckHorobinoutaSub, &context, 0);
    return context.count & 0xFFFF;
}
