#include "dolphin/exi/EXI.h"
#include "dolphin/os/OSInterrupt.h"
#include "src/dolphin/card_dsp_private.h"

extern EXICallback fn_8009870C(s32 chan, EXICallback callback);
extern void fn_80098AE8(s32 chan);

void DoUnmount(s32 chan, s32 result)
{
    CARDControl* card = &lbl_803FC620[chan];
    BOOL enabled;

    enabled = OSDisableInterrupts();
    if (card->attached != 0) {
        fn_8009870C(chan, NULL);
        fn_80098AE8(chan);
        OSCancelAlarm(&card->alarm);
        card->attached = 0;
        card->result = result;
        card->field_24 = 0;
    }
    OSRestoreInterrupts(enabled);
}
