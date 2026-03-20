#ifndef DOLPHIN_DVD_DVD_H
#define DOLPHIN_DVD_DVD_H

#include "dolphin/types.h"

typedef struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[22];
} DVDDiskID;

typedef struct DVDCommandBlock DVDCommandBlock;

typedef void (*DVDCBCallback)(s32 result, DVDCommandBlock* block);

struct DVDCommandBlock {
    DVDCommandBlock* next;
    DVDCommandBlock* prev;
    u32 command;
    s32 state;
    u32 offset;
    u32 length;
    void* addr;
    u32 currTransferSize;
    u32 transferredSize;
    DVDDiskID* id;
    DVDCBCallback callback;
    /* more fields may follow */
    u8 _padding[0x30 - 0x2C];
};

typedef struct DVDDriveInfo {
    u16 revisionLevel;
    u16 deviceCode;
    u32 releaseDate;
    u8 padding[24];
} DVDDriveInfo;

typedef struct DVDFileInfo {
    DVDCommandBlock cb;
    u32 startAddr;
    u32 length;
    DVDCBCallback callback;
} DVDFileInfo;

/* DVD functions */
void DVDInit(void);
BOOL DVDReadDiskID(DVDCommandBlock* block, DVDDiskID* diskID, DVDCBCallback callback);
BOOL DVDInquiryAsync(DVDCommandBlock* block, DVDDriveInfo* info, DVDCBCallback callback);
void DVDReset(void);
s32 DVDGetDriveStatus(void);

/* DVD internal functions */
void __DVDFSInit(void);
void __DVDClearWaitingQueue(void);
BOOL __DVDPushWaitingQueue(s32 prio, DVDCommandBlock* block);
DVDCommandBlock* __DVDPopWaitingQueue(void);
BOOL __DVDCheckWaitingQueue(void);

/* DVDLow functions */
void __DVDInitWA(void);
void DVDLowReset(void);
BOOL DVDLowStopMotor(DVDCBCallback callback);
BOOL DVDLowWaitCoverClose(DVDCBCallback callback);
void __DVDLowSetWAType(u32 type, u32 location);

/* DVDError functions */
void __DVDStoreErrorCode(u32 error);

#endif /* DOLPHIN_DVD_DVD_H */
