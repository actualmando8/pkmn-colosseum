#include "dolphin/types.h"

extern u32 ARQGetChunkSize(void);
extern u32 lbl_8047B06C;
extern u32 lbl_8047B070;

void aramSetUploadCallback(u8* buffer, u32 size)
{
    u32 aligned;
    u32 chunkSize;

    if (buffer != 0) {
        aligned = (size + 0x1F) & ~0x1Fu;
        chunkSize = ARQGetChunkSize();
        lbl_8047B06C = aligned < chunkSize ? chunkSize : aligned;
    }
    lbl_8047B070 = (u32)buffer;
}
