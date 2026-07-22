#include "dolphin/os/OSInterrupt.h"
#include "src/dolphin/card_dsp_private.h"

s32 CARDCancel(CARDFileInfo* fileInfo)
{
    CARDControl* card;
    BOOL enabled;
    s32 result;

    enabled = OSDisableInterrupts();
    card = &lbl_803FC620[fileInfo->chan];
    result = 0;
    if (card->attached == 0) {
        result = -3;
    } else if (card->result == -1 && card->fileInfo == fileInfo) {
        fileInfo->length = -1;
        result = -14;
    }
    OSRestoreInterrupts(enabled);
    return result;
}
