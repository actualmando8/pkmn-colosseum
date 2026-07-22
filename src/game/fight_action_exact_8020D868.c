#include "game/colosseum.h"
#include "game/fight_action.h"

void fightActionBiosSetMotoFightActionDataPtr(FightAction* action, void* motoData)
{
    if (action == NULL) {
        return;
    }
    action->motoActionData = motoData;
}

void fightActionBiosSetBuffDataId(FightAction* action, u32 buffDataId)
{
    if (action == NULL) {
        return;
    }
    action->buffDataId = buffDataId;
}

void fightActionBiosSetBuffDataPtr(FightAction* action, void* buffData)
{
    if (action == NULL) {
        return;
    }
    action->buffData = buffData;
}

void fightActionBiosSetActorFightTargetPtr(FightAction* action, void* actorTarget)
{
    if (action == NULL) {
        return;
    }
    action->actorTarget = actorTarget;
}

void fightActionBiosSetFightActionDataPtr(FightAction* action, FightActionData* data)
{
    if (action == NULL) {
        return;
    }
    action->data = data;
}

void fightActionBiosSetBuff(FightAction* action, u32 buff)
{
    if (action == NULL) {
        return;
    }
    action->buff = buff;
}

void fightActionBiosSetKind(FightAction* action, u32 kind)
{
    if (action == NULL) {
        return;
    }
    action->kind = kind;
}

u32 fightActionBiosGetBuffDataId(FightAction* action)
{
    if (action == NULL) {
        return 0;
    }
    return action->buffDataId;
}

void* fightActionBiosGetBuffDataPtr(FightAction* action)
{
    if (action == NULL) {
        return NULL;
    }
    return action->buffData;
}

void* fightActionBiosGetActorFightTargetPtr(FightAction* action)
{
    if (action == NULL) {
        return NULL;
    }
    return action->actorTarget;
}

FightActionData* fightActionBiosGetFightActionDataPtr(FightAction* action)
{
    if (action == NULL) {
        return NULL;
    }
    return action->data;
}

u32 fightActionBiosGetBuff(FightAction* action)
{
    if (action == NULL) {
        return 0;
    }
    return action->buff;
}

u16 fightActionBiosGetKind(FightAction* action)
{
    if (action == NULL) {
        return 0;
    }
    return action->kind;
}
