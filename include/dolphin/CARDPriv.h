#ifndef DOLPHIN_CARD_PRIV_H
#define DOLPHIN_CARD_PRIV_H

#include "dolphin/os/OSAlarm.h"
#include "dolphin/types.h"

typedef void (*CARDCallback)(s32 chan, s32 result);

struct CARDFileInfo;

typedef struct CARDControl {
    /* 0x000 */ s32 attached;
    /* 0x004 */ s32 result;
    /* 0x008 */ u16 size;
    /* 0x00A */ u16 pageSize;
    /* 0x00C */ u32 sectorSize;
    /* 0x010 */ u16 cBlock;
    /* 0x012 */ u8 padding_012[0x12];
    /* 0x024 */ s32 field_024;
    /* 0x028 */ s32 formatStep;
    /* 0x02C */ u32 scramble;
    /* 0x030 */ u8 task[0x50];
    /* 0x080 */ void* workArea;
    /* 0x084 */ void* dirBlock;
    /* 0x088 */ void* fatBlock;
    /* 0x08C */ u8 padding_08C[0x08];
    /* 0x094 */ u8 command[5];
    /* 0x099 */ u8 padding_099[0x07];
    /* 0x0A0 */ s32 commandLength;
    /* 0x0A4 */ s32 field_0A4;
    /* 0x0A8 */ s32 field_0A8;
    /* 0x0AC */ s32 repeat;
    /* 0x0B0 */ u32 address;
    /* 0x0B4 */ u8* buffer;
    /* 0x0B8 */ u32 transferred;
    /* 0x0BC */ u16 freeBlock;
    /* 0x0BE */ u16 startBlock;
    /* 0x0C0 */ struct CARDFileInfo* fileInfo;
    /* 0x0C4 */ CARDCallback extCallback;
    /* 0x0C8 */ CARDCallback txCallback;
    /* 0x0CC */ CARDCallback callback;
    /* 0x0D0 */ CARDCallback apiCallback;
    /* 0x0D4 */ CARDCallback xferCallback;
    /* 0x0D8 */ CARDCallback updateCallback;
    /* 0x0DC */ CARDCallback unlockCallback;
    /* 0x0E0 */ OSAlarm alarm;
    /* 0x108 */ u8 padding_108[0x04];
    /* 0x10C */ void* diskId;
} CARDControl;

typedef struct CARDID {
    u8 serial[32];
    u16 deviceID;
    u16 size;
    u16 encode;
    u8 padding[470];
    u16 checkSum;
    u16 checkSumInv;
} CARDID;

s32 __CARDGetControlBlock(s32 chan, CARDControl** card);
s32 __CARDPutControlBlock(CARDControl* card, s32 result);

#endif
