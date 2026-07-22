#include "dolphin/types.h"

typedef struct BattleGridTransitionState {
    u8 mode;
    u8 pending;
    u16 arg;
    f32 startValue;
    f32 endValue;
    void* callbackArg;
    void* texture;
    f32 value;
    f32 timer;
} BattleGridTransitionState;

extern BattleGridTransitionState lbl_80466E30;

void fadeSetFunctionOnly(s32 callbackArg)
{
    lbl_80466E30.callbackArg = (void*)callbackArg;
}
