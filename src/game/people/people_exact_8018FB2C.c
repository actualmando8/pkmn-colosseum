#include "game/people/people.h"

struct GSmodel;

extern s32 GScolsys2HumanEnable(s32 shadowId, s32 animationId);
extern void GSmodelSetVisibility(struct GSmodel* model, u8 animationId);

void fn_8018FB2C(PeopleEntry* entry, u8 animationId)
{
    s32 shadowId;

    entry->shadowAnimId = animationId;
    shadowId = entry->shadowId;
    if (shadowId >= 0) {
        GScolsys2HumanEnable(shadowId, animationId);
    }
}

void fn_8018FB60(PeopleEntry* entry, u8 animationId)
{
    void* model;

    model = entry->modelHandle;
    if (model != 0) {
        entry->animId = animationId;
        GSmodelSetVisibility((struct GSmodel*)model, animationId);
    }
}
