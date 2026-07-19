#include "game/battle/battle_waza_types.h"

extern u8 GSmodelIsRootNullAdded(s32 model);
extern void GSmodelGetRootPosition(s32 model, void* position);
extern void GSmodelAddNull(
    s32 model, const void* position, s32 arg2, s32 arg3);
extern void fn_801DEF0C(void* effect, s32 arg1, s32 arg2);

void fn_801DCF00(WazaEffect* effect)
{
    u8 flags = effect->flags;

    if ((flags & 2) != 2) {
        effect->flags = flags | 2;
        if (GSmodelIsRootNullAdded(effect->handle) != 0) {
            GSmodelGetRootPosition(effect->handle, (u8*)effect + 0x5c);
        } else {
            GSmodelAddNull(effect->handle, (u8*)effect + 0x5c, 0, 0);
        }
        fn_801DEF0C(effect, 1, 0);
    }
}
