/** Candidate-only residual range. */
#include "src/dolphin/sdk_range_800AE3F0.c"

void FormatCallback(s32 chan, s32 result)
{
    CARDControl* card;
    CARDCallback callback;

    card = &lbl_803FC620[chan];
    if (result >= 0) {
        ++card->formatStep;
        if (card->formatStep < 5) {
            result = fn_800AFFE0(chan,
                                card->sectorSize * card->formatStep,
                                FormatCallback);
            if (result >= 0) {
                return;
            }
        } else if (card->formatStep < 10) {
            s32 step = card->formatStep - 5;
            result = fn_800B19A4(
                chan, card->sectorSize * step, 0x2000,
                (u8*) card->workArea + 0x2000 * step, FormatCallback);
            if (result >= 0) {
                return;
            }
        } else {
            card->dirBlock = (u8*) card->workArea + 0x2000;
            memcpy(card->dirBlock, (u8*) card->workArea + 0x4000, 0x2000);
            card->fatBlock = (u8*) card->workArea + 0x6000;
            memcpy(card->fatBlock, (u8*) card->workArea + 0x8000, 0x2000);
        }
    }

    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    callback(chan, result);
}
