#include "src/dolphin/card_dsp_private.h"

extern s32 __CARDFreeBlock(s32 chan, u16 block, CARDCallback callback);
extern s32 __CARDPutControlBlock(CARDControl* card, s32 result);

void DeleteCallback(s32 chan, s32 result)
{
    CARDControl* card = &lbl_803FC620[chan];
    CARDCallback callback = card->apiCallback;

    card->apiCallback = NULL;
    if (result >= 0) {
        result = __CARDFreeBlock(chan, card->startBlock, callback);
        if (result >= 0) {
            return;
        }
    }
    __CARDPutControlBlock(card, result);
    if (callback != NULL) {
        callback(chan, result);
    }
}
