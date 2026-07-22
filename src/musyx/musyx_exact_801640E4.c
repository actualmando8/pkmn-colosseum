#include "dolphin/ai/AI.h"
#include "dolphin/types.h"

extern u32 lbl_8047B09C;
extern void fn_80164400(u32 address);

u32 salExitAi(void)
{
    AIRegisterDMACallback(0);
    AIStopDMA();
    fn_80164400(lbl_8047B09C);
    return 1;
}
