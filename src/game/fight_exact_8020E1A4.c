/**
 * @file fight_exact_8020E1A4.c
 * @brief Strict fight type/kind data accessors, 0x8020E1A4 - 0x8020E4E8.
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

typedef struct FightTypeData {
    u8 trainerNum;
    u8 entryPokemonNum;
    u8 fightoutPokemonNum;
    u8 pad_03;
    u32 name;
} FightTypeData;

typedef struct FightKindData {
    u8 backSaveDataFlag;
    u8 doBadgeCheckFlag;
    u8 doZukanMitaFlag;
    u8 doZukanTukamaetaFlag;
    u8 useItemFlag;
    u8 callFlag;
    u8 nigeruFlag;
    u8 drawFlag;
    u8 getExpFlag;
    u8 getOkaneFlag;
    u8 okanePoolFlag;
    u8 getNekoniKobanFlag;
    u8 getFriendFlag;
    u8 getInfectPokerusFlag;
    u8 doCriticalAttackFlag;
    u8 doHizukiAiFlag;
    u8 doHizukiMiyaburiFlag;
    u8 doItemSoubiTokukoutokubouupFlag;
    u8 keikentihueruFlag;
    u8 bossFlag;
    u8 dorobouFlag;
    u8 monohiroiFlag;
    u8 darkpokemonHypermodeFlag;
    u8 pokemonStatusMenuSubbarFlag;
    u8 hostEnemyMsgFlag;
    u8 pad_19[3];
    u32 name;
} FightKindData;

extern u32* lbl_80478F00;
extern FightTypeData* lbl_80478F04;
extern u32* lbl_80478F40;
extern FightKindData* lbl_80478F44;

u8 fightTypeDataBiosGetFightoutPokemonNum(FightTypeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->fightoutPokemonNum;
}

u8 fightTypeDataBiosGetEntryPokemonNum(FightTypeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->entryPokemonNum;
}

u8 fightTypeDataBiosGetTrainerNum(FightTypeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->trainerNum;
}

u32 fightTypeDataBiosGetName(FightTypeData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->name;
}

FightTypeData* fightTypeDataBiosGetPtr(u16 index)
{
    if (index > *lbl_80478F00) {
        return NULL;
    }
    return &lbl_80478F04[index];
}

u8 fightKindDataBiosGetHostEnemyMsgFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->hostEnemyMsgFlag;
}

u32 fightKindDataBiosGetName(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->name;
}

u8 fightKindDataBiosGetPokemonStatusMenuSubbarFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->pokemonStatusMenuSubbarFlag;
}

u8 fightKindDataBiosGetDarkpokemonHypermodeFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->darkpokemonHypermodeFlag;
}

u8 fightKindDataBiosGetMonohiroiFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->monohiroiFlag;
}

u8 fightKindDataBiosGetDorobouFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->dorobouFlag;
}

u8 fightKindDataBiosGetBossFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->bossFlag;
}

u8 fightKindDataBiosGetKeikentihueruFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->keikentihueruFlag;
}

u8 fightKindDataBiosGetDoItemSoubiTokukoutokubouupFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doItemSoubiTokukoutokubouupFlag;
}

u8 fightKindDataBiosGetDoHizukiMiyaburiFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doHizukiMiyaburiFlag;
}

u8 fightKindDataBiosGetDoHizukiAiFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doHizukiAiFlag;
}

u8 fightKindDataBiosGetDoCriticalAttackFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doCriticalAttackFlag;
}

u8 fightKindDataBiosGetGetInfectPokerusFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->getInfectPokerusFlag;
}

u8 fightKindDataBiosGetGetFriendFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->getFriendFlag;
}

u8 fightKindDataBiosGetGetNekoniKobanFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->getNekoniKobanFlag;
}

u8 fightKindDataBiosGetGetOkaneFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->getOkaneFlag;
}

u8 fightKindDataBiosGetOkanePoolFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->okanePoolFlag;
}

u8 fightKindDataBiosGetGetExpFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->getExpFlag;
}

u8 fightKindDataBiosGetDrawFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->drawFlag;
}

u8 fightKindDataBiosGetNigeruFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->nigeruFlag;
}

u8 fightKindDataBiosGetCallFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->callFlag;
}

u8 fightKindDataBiosGetUseItemFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->useItemFlag;
}

u8 fightKindDataBiosGetDoZukanTukamaetaFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doZukanTukamaetaFlag;
}

u8 fightKindDataBiosGetDoZukanMitaFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doZukanMitaFlag;
}

u8 fightKindDataBiosGetDoBadgeCheckFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->doBadgeCheckFlag;
}

FightKindData* fightKindDataBiosGetPtr(u16 index)
{
    if (index > *lbl_80478F40) {
        return NULL;
    }
    return &lbl_80478F44[index];
}

u8 fightKindDataBiosGetBackSaveDataFlag(FightKindData* ptr)
{
    if (ptr == NULL) {
        return 0;
    }
    return ptr->backSaveDataFlag;
}

s32 fightAbicntFitMinMax(s32 value)
{
    if (value < 0) {
        value = 0;
    }
    if (value > 12) {
        value = 12;
    }
    return value;
}
