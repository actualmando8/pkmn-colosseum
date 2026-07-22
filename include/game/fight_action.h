#ifndef GAME_FIGHT_ACTION_H
#define GAME_FIGHT_ACTION_H

#include "dolphin/types.h"

typedef u32 (*FightActionCallback)(void* action);

u32 fightActionFlowNullFunc(void* action);
u32 fightActionFlowKaijou(void* action);
u32 fightActionFlowKaisiPre(void* action);
u32 fightActionFlowKaisiNyuujouTrainer(void* action);
u32 fightActionFlowKaisiNyuujouPokemon(void* action);
u32 fightActionFlowKaisiPost(void* action);
u32 fightActionFlowTenkouInit(void* action);
u32 fightActionFlowAllFightTrainerSelectFightAction(void* action);
u32 fightActionFlowFightNigeru(void* action);
u32 fightActionFlowFightOutPokemonIrekae(void* action);
u32 fightActionFlowFightTrainerCall(void* action);
u32 fightActionFlowAllFightOutPokemonDoFightAction(void* action);
u32 fightActionFlowWazaKiaipantiPre(void* action);
u32 fightActionFlowOneTurnPost(void* action);
u32 fightActionFlowSyuuryouPre(void* action);
u32 fightActionFlowSyuuryou(void* action);
u32 fightActionFlowSyuuryouPost(void* action);
u32 fightActionFlowHeijou(void* action);
u32 fightActionFlowFightTrainerUseItem(void* action);
u32 fightActionFlowFightOutPokemonOutWaza(void* action);

u32 fightActionDispNullFunc(void* action);
u32 fightActionDispKaijou(void* action);
u32 fightActionDispKaisiPre(void* action);
u32 fightActionDispKaisiNyuujouTrainer(void* action);
u32 fightActionDispKaisiNyuujouPokemon(void* action);
u32 fightActionDispKaisiPost(void* action);
u32 fightActionDispTenkouInit(void* action);
u32 fightActionDispAllFightTrainerSelectFightAction(void* action);
u32 fightActionDispFightNigeru(void* action);
u32 fightActionDispFightOutPokemonIrekae(void* action);
u32 fightActionDispFightTrainerCall(void* action);
u32 fightActionDispAllFightOutPokemonDoFightAction(void* action);
u32 fightActionDispWazaKiaipantiPre(void* action);
u32 fightActionDispOneTurnPost(void* action);
u32 fightActionDispSyuuryouPre(void* action);
u32 fightActionDispSyuuryou(void* action);
u32 fightActionDispSyuuryouPost(void* action);
u32 fightActionDispHeijou(void* action);
u32 fightActionDispFightTrainerUseItem(void* action);
u32 fightActionDispFightOutPokemonOutWaza(void* action);

typedef struct FightActionKindData {
    /* 0x00 */ u8 rawPriority;
    /* 0x01 */ u8 pad_01[3];
    /* 0x04 */ FightActionCallback flow;
    /* 0x08 */ FightActionCallback display;
} FightActionKindData;

extern FightActionKindData lbl_80375BB8[20];

typedef struct FightActionData {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u8 pad_02[2];
    /* 0x04 */ u32 buff;
} FightActionData;

typedef struct FightAction {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u8 pad_02[2];
    /* 0x04 */ u32 buff;
    /* 0x08 */ FightActionData* data;
    /* 0x0C */ void* buffData;
    /* 0x10 */ u32 buffDataId;
    /* 0x14 */ void* actorTarget;
    /* 0x18 */ void* motoActionData;
    /* 0x1C */ s32 fifoIndex;
    /* 0x20 */ u32 displayBuff[4];
} FightAction;

void fightActionBiosSetFifoBanme(FightAction* action, s32 fifoIndex);
void fightActionBiosSetMotoFightActionDataPtr(FightAction* action, void* motoData);
void fightActionBiosSetBuffDataId(FightAction* action, u32 buffDataId);
void fightActionBiosSetBuffDataPtr(FightAction* action, void* buffData);
void fightActionBiosSetActorFightTargetPtr(FightAction* action, void* actorTarget);
void fightActionBiosSetFightActionDataPtr(FightAction* action, FightActionData* data);
void fightActionBiosSetBuff(FightAction* action, u32 buff);
void fightActionBiosSetKind(FightAction* action, u32 kind);
void fightActionBiosSetDispBuff(FightAction* action, u32 index, u32 value);
FightActionCallback fightActionKindDataBiosGetDispFuncPtr(FightActionKindData* data);
FightActionCallback fightActionKindDataBiosGetFlowFuncPtr(FightActionKindData* data);
s32 fightActionKindDataBiosGetPri(FightActionKindData* data);
FightActionKindData* fightActionKindDataBiosGetPtr(u16 index);
u32 fightActionDataBiosGetBuff(FightActionData* data);
u16 fightActionDataBiosGetKind(FightActionData* data);
FightActionData* fightActionBiosGetFightActionDataPtr(FightAction* action);
void* fightActionBiosGetActorFightTargetPtr(FightAction* action);
void* fightActionBiosGetBuffDataPtr(FightAction* action);
u32 fightActionBiosGetBuffDataId(FightAction* action);
u32 fightActionBiosGetBuff(FightAction* action);
u16 fightActionBiosGetKind(FightAction* action);
u16 fightActionGetKindDataId(void* action);
s32 fightActionGetPri(void* action);
u32 fightActionCheckValid(void* action);
s32 fightActionCreate(FightAction* action, void* motoAction, void* actorTarget,
                      u32 kind, u32 buff, FightActionData* actionData);
u32 fightActionFlowFifo(void* action);
void* fightOutPokemonCreateFightActionAttackWaza(
    void* ctx, void* motoAction, u32 kind, u32 buff,
    FightActionData* actionData, u32 moveId, u32 targetId, u32 slot, u8 mode);

#endif
