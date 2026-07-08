#include "dolphin/dvd/dvd.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSCache.h"

/*
 * DVDFsExtras.c - DVD filesystem helper (tiny unit).
 *
 * Per config/GC6E01/splits.txt this unit's real address range is only
 * 0x800A7F80 - 0x800A7FE0 (0x60 bytes), i.e. __DVDDequeueWaitingQueue
 * below, which is a byte-exact match.
 *
 * 2026-07-02 reconciliation: removed the fictional definitions of
 * DVDFastOpen, DVDClose, DVDChangeDir, __DVDCheckDevice, __DVDCheckDisk,
 * __DVDFsStub and __DVDGetCoverStatus, and the fictional body of
 * __DVDPrepareReset.
 *
 * 2026-07-08 reclaim-for-linking pass: removed the remaining strays
 * (DVDConvertPathToEntrynum, DVDOpen, DVDGetCurrentDir, cb,
 * __DVDPrepareResetAsync) too. Their names DO exist in symbols.txt but
 * at addresses belonging to other splits (dolphin/dvd/dvdfs_range_800A4D28.c,
 * dolphin/dvd/sdk_range_800A7820.c and dolphin/vi/VI_range_800A8178.c),
 * none of which are decompiled - those splits are linked from their
 * extracted (byte-identical) objects, which already provide these
 * symbols. Defining them here too caused
 * 'mwldeppc.exe Linker Error: multiply-defined' when this unit was
 * flipped to Matching. This unit now defines only the one function
 * that actually lives in its address range.
 */

/* __DVDDequeueWaitingQueue - 0x800A7F80 | size: 0x60
 * Unlink a node from a doubly-linked list with interrupt protection.
 * node+0x00 = next, node+0x04 = prev.
 * Returns 1 if successfully unlinked, 0 if either link is NULL.
 */
u32 __DVDDequeueWaitingQueue(u8* node) {
    BOOL enabled;
    u32 prev;
    u32 next;

    enabled = OSDisableInterrupts();
    prev = *(u32*)(node + 0x4);
    next = *(u32*)(node + 0x0);

    if (prev == 0 || next == 0) {
        OSRestoreInterrupts(enabled);
        return 0;
    }

    *(u32*)((u8*)prev + 0x0) = next;
    *(u32*)((u8*)next + 0x4) = prev;
    OSRestoreInterrupts(enabled);
    return 1;
}

