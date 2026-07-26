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

static inline u32 cardExnorDone(u32 data, u32 shift)
{
    u32 feedback;
    u32 work;
    u32 i;

    work = data;
    for (i = 0; i < shift; i++) {
        feedback = ~(work ^ (work << 7) ^ (work << 15) ^ (work << 23));
        work = (work << 1) | ((feedback >> 30) & 2);
    }
    return work;
}

void DoneCallback(void* task)
{
    u8 readBuffer[64];
    u32 data;
    s32 dummy;
    s32 readLength;
    u32 shift;
    u8 status;
    u32 work;
    u32 feedback;
    u32 answer;
    s32 chan;
    CARDControl* card;
    s32 result;
    CARDDecParam* parameter;
    u8* input;
    u8* output;

    for (chan = 0; chan < 2; ++chan) {
        card = &lbl_803FC620[chan];
        if ((DSPTaskInfo*) card->task == task) {
            break;
        }
    }

    parameter = (CARDDecParam*) card->workArea;
    input = (u8*) parameter + sizeof(CARDDecParam);
    input = (u8*) (((u32) input + 31) & ~31);
    output = input + 32;
    answer = *(u32*) output;

    dummy = DummyLen();
    readLength = dummy;
    data = (answer ^ card->scramble) & 0xFFFF0000;
    if (ReadArrayUnlock(chan, data, readBuffer, readLength, 1) < 0) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, -3);
        return;
    }

    shift = (dummy + 4 + *(u32*) ((u8*) card + 0x14)) * 8 + 1;
    work = cardExnorDone(card->scramble, shift);
    feedback = ~(work ^ (work << 7) ^ (work << 15) ^ (work << 23));
    card->scramble = work | ((feedback >> 31) & 1);

    dummy = DummyLen();
    readLength = dummy;
    data = ((answer << 16) ^ card->scramble) & 0xFFFF0000;
    if (ReadArrayUnlock(chan, data, readBuffer, readLength, 1) < 0) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, -3);
        return;
    }

    result = fn_800AF660(chan, &status);
    if (!fn_80098944(chan)) {
        EXIUnlock(chan);
        __CARDMountCallback(chan, -3);
        return;
    }

    if (result == 0 && !(status & 0x40)) {
        EXIUnlock(chan);
        result = -5;
    }
    __CARDMountCallback(chan, result);
}
