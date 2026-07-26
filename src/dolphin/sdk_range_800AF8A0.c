/**
 * @file sdk_range_800AF8A0.c
 * @brief Candidate Dolphin SDK range, 0x800AF8A0 - 0x800B1788.
 */

#include "src/dolphin/sdk_range_800AE3F0.c"

s32 __CARDReadSegment(s32 chan, CARDCallback callback)
{
    CARDControl* card;
    s32 result;

    card = &lbl_803FC620[chan];
    card->cmd[0] = 0x52;
    card->cmd[1] = (card->addr >> 17) & 0x7F;
    card->cmd[2] = (card->addr >> 9) & 0xFF;
    card->cmd[3] = (card->addr >> 7) & 3;
    card->cmd[4] = card->addr & 0x7F;
    card->cmdLen = 5;
    card->field_A4 = 0;
    card->field_A8 = 0;

    result = fn_800AFBDC(chan, NULL, callback);
    if (result == -1) {
        result = 0;
    } else if (result >= 0) {
        if (!fn_80098368(chan, card->cmd, card->cmdLen, 1) ||
            !fn_80098368(chan, (u8*) card->workArea + sizeof(CARDID),
                          *(u32*) ((u8*) card + 0x14), 1) ||
            !EXIDma(chan, card->buffer, 0x200, card->field_A4,
                    (EXICallback)__CARDTxHandler))
        {
            card->callback_CC = NULL;
            EXIDeselect(chan);
            EXIUnlock(chan);
            result = -3;
        } else {
            result = 0;
        }
    }
    return result;
}
