#include "dolphin/types.h"

typedef struct ColosseumBattleTimerState {
    u8 done;
    u8 forceDone;
    u8 padding_02[2];
    f32 elapsed;
    f32 limit;
    u32 thread;
} ColosseumBattleTimerState;

extern f32 lbl_8047E6D8;
extern const f32 lbl_8047E6DC;
extern u32 fn_800D3088(void);
extern void _threadSwitch(void);

void fightTimerThreadFunc(ColosseumBattleTimerState* timer)
{
    timer->elapsed = lbl_8047E6D8;
    while (timer->elapsed < lbl_8047E6DC * timer->limit) {
        _threadSwitch();
        timer->elapsed = (f32)fn_800D3088() + timer->elapsed;
    }
    timer->done = 1;
    for (;;) {
        _threadSwitch();
    }
}
