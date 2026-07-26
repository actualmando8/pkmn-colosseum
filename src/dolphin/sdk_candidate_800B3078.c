/** Candidate-only residual range. */
#include "src/dolphin/sdk_range_800AE3F0.c"

s32 CARDProbeEx(s32 chan, s32* memSize, s32* sectorSize)
{
    extern u32 fn_800993A8(s32 chan);
    CARDControl* card;
    BOOL enabled;
    s32 result;
    s32 probe;
    u32 id;

    if (chan < 0 || chan >= 2) {
        return -128;
    }
    if (*(volatile u8*) 0x800030E3 & 0x80) {
        return -3;
    }

    card = &lbl_803FC620[chan];
    enabled = OSDisableInterrupts();
    probe = EXIProbeEx(chan);
    if (probe == -1) {
        result = -3;
    } else if (probe == 0) {
        result = -1;
    } else if (card->attached) {
        if (card->field_24 < 1) {
            result = -1;
        } else {
            if (memSize != NULL) {
                *memSize = card->size;
            }
            if (sectorSize != NULL) {
                *sectorSize = card->sectorSize;
            }
            result = 0;
        }
    } else if (fn_800993A8(chan) & 8) {
        result = -2;
    } else if (!fn_80099400(chan, 0, &id)) {
        result = -1;
    } else if (IsCard(id)) {
        if (memSize != NULL) {
            *memSize = id & 0xFC;
        }
        if (sectorSize != NULL) {
            *sectorSize = lbl_80312960[(id & 0x3800) >> 11];
        }
        result = 0;
    } else {
        result = -2;
    }
    OSRestoreInterrupts(enabled);
    return result;
}

void __CARDMountCallback(s32 chan, s32 result)
{
    CARDControl* card;
    CARDCallback callback;

    card = &lbl_803FC620[chan];
    switch (result) {
    case 0:
        if (++card->field_24 < 7) {
            result = fn_800B31F4(chan);
            if (result >= 0) {
                return;
            }
        } else {
            result = __CARDVerify(card);
        }
        break;
    case 1:
        card->unlockCallback = __CARDMountCallback;
        if (!EXILock(chan, 0, __CARDUnlockedHandler)) {
            return;
        }
        card->unlockCallback = NULL;
        result = fn_800B31F4(chan);
        if (result >= 0) {
            return;
        }
        break;
    case -5:
    case -3:
        DoUnmount(chan, result);
        break;
    }

    callback = card->apiCallback;
    card->apiCallback = NULL;
    __CARDPutControlBlock(card, result);
    callback(chan, result);
}

s32 CARDMountAsync(s32 chan, void* workArea, CARDCallback detachCallback,
                   CARDCallback attachCallback)
{
    extern u32 fn_800993A8(s32 chan);
    extern BOOL fn_80098790(s32 chan, EXICallback callback);
    CARDControl* card;
    BOOL enabled;

    if (chan < 0 || chan >= 2) {
        return -128;
    }
    if (*(volatile u8*) 0x800030E3 & 0x80) {
        return -3;
    }

    card = &lbl_803FC620[chan];
    enabled = OSDisableInterrupts();
    if (card->result == -1) {
        OSRestoreInterrupts(enabled);
        return -1;
    }

    if (!card->attached && (fn_800993A8(chan) & 8)) {
        OSRestoreInterrupts(enabled);
        return -2;
    }

    card->result = -1;
    card->workArea = workArea;
    card->extCallback = detachCallback;
    card->apiCallback =
        attachCallback != NULL ? attachCallback : __CARDDefaultApiCallback;
    card->callback_CC = NULL;

    if (!card->attached &&
        !fn_80098790(chan, (EXICallback) __CARDExtHandler))
    {
        card->result = -3;
        OSRestoreInterrupts(enabled);
        return -3;
    }

    card->field_24 = 0;
    card->attached = TRUE;
    fn_8009870C(chan, NULL);
    OSCancelAlarm(&card->alarm);
    card->dirBlock = NULL;
    card->fatBlock = NULL;
    OSRestoreInterrupts(enabled);

    card->unlockCallback = __CARDMountCallback;
    if (!EXILock(chan, 0, __CARDUnlockedHandler)) {
        return 0;
    }

    card->unlockCallback = NULL;
    return fn_800B31F4(chan);
}
