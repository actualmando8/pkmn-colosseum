#include "src/dolphin/card_dsp_private.h"

extern s32 CARDCheckExAsync(s32 chan, s32* xferBytes, CARDCallback callback);

s32 CARDCheckAsync(s32 chan, CARDCallback callback)
{
    s32 xferBytes;

    return CARDCheckExAsync(chan, &xferBytes, callback);
}

BOOL IsCard(u32 id)
{
    s32 sectorSize;
    u32 cardSize;

    if ((id & 0xffff0000) != 0) {
        if (id != 0x80000004 || lbl_80478A58 == 0xffff) {
            return FALSE;
        }
    }
    if ((id & 3) != 0) {
        return FALSE;
    }
    cardSize = id & 0xfc;
    switch (cardSize) {
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
        break;
    default:
        return FALSE;
    }
    sectorSize = lbl_80312960[(id >> 11) & 7];
    if (sectorSize == 0) {
        return FALSE;
    }
    if (((cardSize << 17) & 0x1ffe0000) / sectorSize < 8) {
        return FALSE;
    }
    return TRUE;
}
