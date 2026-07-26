/** Candidate-only residual range. */
#include "src/dolphin/sdk_range_800AE3F0.c"

s32 ReadArrayUnlock(s32 chan, u32 data, void* readBuffer, s32 readLength,
                    s32 mode)
{
    CARDControl* card;
    BOOL error;
    u8 command[5];

    card = &lbl_803FC620[chan];
    if (!EXISelect(chan, 0, 4)) {
        return -3;
    }

    data &= 0xFFFFF000;
    memset(command, 0, sizeof(command));
    command[0] = 0x52;
    if (mode == 0) {
        command[1] = (data >> 29) & 3;
        command[2] = (data >> 21) & 0xFF;
        command[3] = (data >> 19) & 3;
        command[4] = (data >> 12) & 0x7F;
    } else {
        command[1] = data >> 24;
        command[2] = data >> 16;
    }

    error = FALSE;
    error |= !fn_80098368(chan, command, sizeof(command), 1);
    error |= !fn_80098368(chan, (u8*) card->workArea + sizeof(CARDID),
                          *(u32*) ((u8*) card + 0x14), 1);
    error |= !fn_80098368(chan, readBuffer, readLength, 0);
    error |= !EXIDeselect(chan);
    return error ? -3 : 0;
}
