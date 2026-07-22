#include "dolphin/CARDPriv.h"

s32 CARDGetSerialNo(s32 chan, u64* serialNo)
{
    CARDControl* card;
    CARDID* id;
    s32 result;
    u64 code;
    s32 i;

    if (chan < 0 || chan >= 2) {
        return -128;
    }
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }

    id = card->workArea;
    code = 0;
    for (i = 0; i < sizeof(id->serial) / sizeof(u64); i++) {
        code ^= *(u64*)&id->serial[sizeof(u64) * i];
    }
    *serialNo = code;
    return __CARDPutControlBlock(card, 0);
}
