#include "trk/trk.h"

extern TRKResult TRKGetFreeBuffer(s32* bufferID, TRKBuffer** buffer);
extern void TRKTargetAddStopInfo(TRKBuffer* buffer);
extern void TRKTargetAddExceptionInfo(TRKBuffer* buffer);
extern TRKResult TRKRequestSend(TRKBuffer* buffer, s32* replyID,
                                s32 retries, s32 timeout, BOOL waitForReply);

TRKResult TRKDoNotifyStopped(s32 command)
{
    s32 replyID;
    s32 bufferID;
    TRKBuffer* buffer;
    TRKResult result;

    result = TRKGetFreeBuffer(&bufferID, &buffer);
    if (result == kTRKSuccess) {
        if (result == kTRKSuccess) {
            if (command == 0x90) {
                TRKTargetAddStopInfo(buffer);
            } else {
                TRKTargetAddExceptionInfo(buffer);
            }
        }
        result = TRKRequestSend(buffer, &replyID, 2, 3, TRUE);
        if (result == kTRKSuccess) {
            TRKReleaseBuffer(replyID);
        }
        TRKReleaseBuffer(bufferID);
    }
    return result;
}
