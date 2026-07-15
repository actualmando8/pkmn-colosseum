#include "dolphin/types.h"

/* TRKDoNotifyStopped - 0x800C0CD8 | size 0x98 | scope global */
s32 TRKDoNotifyStopped(s32 command) {
    extern s32 TRKGetFreeBuffer(s32* bufferID, s32* buffer);
    extern void TRKTargetAddStopInfo(s32 buffer);
    extern void TRKTargetAddExceptionInfo(s32 buffer);
    extern s32 TRKRequestSend(s32 buffer, s32* replyID, s32 retries, s32 timeout, s32 waitForReply);
    extern s32 TRKReleaseBuffer(s32 bufferID);
    s32 replyID;
    s32 bufferID;
    s32 buffer;
    s32 result;

    result = TRKGetFreeBuffer(&bufferID, &buffer);
    if (result == 0) {
        if (result == 0) {
            if (command == 0x90) {
                TRKTargetAddStopInfo(buffer);
            } else {
                TRKTargetAddExceptionInfo(buffer);
            }
        }
        result = TRKRequestSend(buffer, &replyID, 2, 3, 1);
        if (result == 0) {
            TRKReleaseBuffer(replyID);
        }
        TRKReleaseBuffer(bufferID);
    }
    return result;
}

/* TRK_fill_mem_800D6430 - 0x800C0DA8 | size 0xB8 | scope none (optimized memset-style fill) */
void TRK_fill_mem_800D6430(void* dest, int val, u32 count) {
    u8* dst;
    u32 v;
    u32* wp;
    u32 numBlocks;
    u32 numWords;
    u32 align;

    dst = (u8*)dest - 1;
    v = (u8)val;

    if (count >= 0x20) {
        align = ~(u32)dst & 3;
        if (align != 0) {
            count -= align;
            do {
                *++dst = (u8)v;
            } while (--align != 0);
        }

        if (v != 0) {
            v = (v << 24) | (v << 16) | (v << 8) | v;
        }

        {
            wp = (u32*)(dst - 3);
            if ((numBlocks = count >> 5) != 0) {
                do {
                    wp[1] = v;
                    wp[2] = v;
                    wp[3] = v;
                    wp[4] = v;
                    wp[5] = v;
                    wp[6] = v;
                    wp[7] = v;
                    *(wp += 8) = v;
                } while (--numBlocks != 0);
            }

            if ((numWords = (count >> 2) & 7) != 0) {
                do {
                    *(wp += 1) = v;
                } while (--numWords != 0);
            }

            dst = (u8*)wp + 3;
        }

        count = count & 3u;
    }

    if (count != 0) {
        do {
            *++dst = (u8)v;
        } while (--count != 0);
    }
}

/* TRK_flush_cache - 0x800C0D70 | size 0x38 | scope global */
void TRK_flush_cache(void* addr, u32 nBytes) {
    asm {
        lis r5, 0xffff
        ori r5, r5, 0xfff1
        and r5, r5, r3
        subf r3, r5, r3
        add r4, r4, r3
    _trk_flush_loop:
        dcbst r0, r5
        dcbf r0, r5
        sync
        icbi r0, r5
        addic r5, r5, 8
        subic. r4, r4, 8
        bge _trk_flush_loop
        isync
    }
}

/* fn_800C0E60 - 0x800C0E60 | size 0x8 | scope global */
u32 fn_800C0E60(void) {
    asm {
        mfmsr r3
    }
}

/* fn_800C0E68 - 0x800C0E68 | size 0x8 | scope global */
void fn_800C0E68(u32 msr) {
    asm {
        mtmsr r3
    }
}

/* TRK_ppc_memcpy - 0x800C0E70 | size 0x3C | scope global */
void TRK_ppc_memcpy(void* dst, const void* src, u32 count, u32 storeMSR, u32 loadMSR) {
    asm {
        mfmsr r8
        li r10, 0
    _trk_ppc_memcpy_loop:
        cmpw r10, r5
        beq _trk_ppc_memcpy_done
        mtmsr r7
        sync
        lbzx r9, r10, r4
        mtmsr r6
        sync
        stbx r9, r10, r3
        addi r10, r10, 1
        b _trk_ppc_memcpy_loop
    _trk_ppc_memcpy_done:
        mtmsr r8
        sync
    }
}
