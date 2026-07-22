#include "game/fight_action.h"

void fightActionBiosSetDispBuff(FightAction* action, u32 index, u32 value)
{
    if (action == NULL) {
        return;
    }
    if ((u16) index >= 4) {
        return;
    }
    action->displayBuff[(u16) index] = value;
}
