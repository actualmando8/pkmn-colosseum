#include "src/dolphin/card_dsp_private.h"

extern s32 VerifyID(CARDControl* card);
extern s32 VerifyDir(CARDControl* card, s32* checkCode);
extern s32 VerifyFAT(CARDControl* card, s32* checkCode);

s32 __CARDVerify(CARDControl* card)
{
    s32 result;
    s32 dirResult;

    result = VerifyID(card);
    if (result < 0) {
        return result;
    }
    dirResult = VerifyDir(card, NULL);
    switch (dirResult + VerifyFAT(card, NULL)) {
    case 0:
        return 0;
    case 1:
        return -6;
    default:
        return -6;
    }
}
