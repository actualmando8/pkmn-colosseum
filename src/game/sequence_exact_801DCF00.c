#include "game/battle/battle_waza_types.h"

extern void fn_801DEF0C(void* effect, s32 arg1, s32 arg2);

void fn_801DCF00(WazaEffect* effect)
{
    u8 flags = effect->flags;

    if ((flags & 2) != 2) {
        effect->flags = flags | 2;
        if ((u8)GSmodelIsRootNullAdded(effect->model) != 0) {
            GSmodelGetRootPosition(
                effect->model, (GSvec*)((u8*)effect + 0x5c));
        } else {
            GSmodelAddNull(effect->model,
                           (GSvec*)((u8*)effect + 0x5c), NULL, NULL);
        }
        fn_801DEF0C(effect, 1, 0);
    }
}
