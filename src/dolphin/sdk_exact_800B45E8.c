#include "src/dolphin/card_dsp_private.h"

extern s32 __CARDGetControlBlock(s32 chan, CARDControl** card);
extern s32 __CARDPutControlBlock(CARDControl* card, s32 result);

s32 CARDClose(CARDFileInfo* fileInfo)
{
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(fileInfo->chan, &card);
    if (result < 0) {
        return result;
    }
    fileInfo->chan = -1;
    return __CARDPutControlBlock(card, 0);
}

BOOL __CARDIsOpened(CARDControl* card, s32 fileNo)
{
    return FALSE;
}
