#include "dolphin/os/OS.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/exi/EXI.h"

/*
 * OSSram.c - SRAM (battery-backed configuration memory) access.
 *
 * The GameCube SRAM stores system configuration data (language,
 * sound mode, screen position, etc.) in 64 bytes of battery-backed
 * memory on the IPL ROM chip, accessed via EXI channel 0 device 1.
 *
 * Adapted from doldecomp/melee and zeldaret/tp matching implementations.
 *
 * Matches: 0x8009C860 - 0x8009DF3C
 *   fn_8009C860 (0x3DC) - WriteSram (large EXI transfer)
 *   fn_8009CC3C (0xFC)  - WriteSramCallback
 *   fn_8009CD38 (0x154) - ReadSram
 *   fn_8009CE8C (0x684) - __OSInitSram
 *   fn_8009D510 (0x19C) - __OSLockSram
 *   fn_8009D6AC (0x174) - __OSLockSramEx
 *   fn_8009D820 (0x58)  - UnlockSram
 *   fn_8009D878 (0x8C)  - __OSUnlockSram
 *   fn_8009D904 (0x334) - __OSUnlockSramEx
 *   fn_8009DC38 (0x304) - __OSSyncSram + helpers
 */

extern void* memcpy(void* dest, const void* src, u32 n);

/* SRAM structure (64 bytes) */
typedef struct OSSram {
    u16 checkSum;       /* 0x00 */
    u16 checkSumInv;    /* 0x02 */
    u32 ead0;           /* 0x04 */
    u32 ead1;           /* 0x08 */
    u32 counterBias;    /* 0x0C */
    s8  displayOffsetH; /* 0x10 */
    u8  ntd;            /* 0x11 */
    u8  language;       /* 0x12 */
    u8  flags;          /* 0x13 */
} OSSram;

/* Extended SRAM structure (additional 64 bytes at offset 0x40) */
typedef struct OSSramEx {
    u8 flashID[4][12];  /* 0x00 */
    u32 wirelessKbID;   /* 0x30 */
    u16 wirelessPadID[4]; /* 0x34 */
    u8  dvdErrorCode;   /* 0x3C */
    u8  _pad0;          /* 0x3D */
    u8  flashIDCheckSum[4]; /* 0x3E */
    u16 _pad1;          /* 0x42 */
} OSSramEx;

/* SRAM state flags */
#define SRAM_LOCKED     0x01
#define SRAM_LOCKED_EX  0x02
#define SRAM_NEED_WRITE 0x04
#define SRAM_WRITING    0x08

static OSSram Sram;
static OSSramEx SramEx;
static u32 SramFlags;
static u32 SramOffset;
static u32 SramSize;
static u32 SramWriteCount;

static BOOL WriteSram(u32 offset, void* data, u32 size);
static void WriteSramCallback(s32 chan, OSContext* context);
static BOOL ReadSram(u32 offset, void* dest, u32 size);

static void UnlockSram(BOOL commit, u32 lockBit);

/*
 * ReadSram - Read data from SRAM via EXI.
 * 0x8009CD38 | size: 0x154
 */
static BOOL ReadSram(u32 offset, void* dest, u32 size) {
    BOOL result = FALSE;
    u32 cmd;

    if (!EXILock(0, 1, NULL)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    /* Send read command: address = 0x20000000 + (offset << 6) */
    cmd = 0x20000000 | (offset << 6);
    if (EXIImm(0, &cmd, 4, 1, NULL) && EXISync(0)) {
        if (EXIDma(0, dest, (s32)size, 0, NULL) && EXISync(0)) {
            result = TRUE;
        }
    }

    EXIDeselect(0);
    EXIUnlock(0);
    return result;
}

/*
 * WriteSramCallback - EXI completion callback for SRAM writes.
 * 0x8009CC3C | size: 0xFC
 */
static void WriteSramCallback(s32 chan, OSContext* context) {
    SramFlags &= ~SRAM_WRITING;
    SramWriteCount++;
    EXIDeselect(0);
    EXIUnlock(0);
}

/*
 * WriteSram - Write data to SRAM via EXI.
 * 0x8009C860 | size: 0x3DC
 *
 * Initiates an EXI write to SRAM. The write is asynchronous --
 * WriteSramCallback is called when the transfer completes.
 */
static BOOL WriteSram(u32 offset, void* data, u32 size) {
    BOOL result = FALSE;
    u32 cmd;

    if (!EXILock(0, 1, NULL)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    /* Send write command: address = 0xA0000000 + (offset << 6) */
    cmd = 0xA0000000 | (offset << 6);
    if (EXIImm(0, &cmd, 4, 1, NULL) && EXISync(0)) {
        SramFlags |= SRAM_WRITING;
        if (EXIDma(0, data, (s32)size, 1, WriteSramCallback)) {
            result = TRUE;
        } else {
            SramFlags &= ~SRAM_WRITING;
            EXIDeselect(0);
            EXIUnlock(0);
        }
    } else {
        EXIDeselect(0);
        EXIUnlock(0);
    }

    return result;
}

/*
 * __OSInitSram - Initialize SRAM subsystem.
 * 0x8009CE8C | size: 0x684
 *
 * Reads both the primary SRAM (64 bytes) and extended SRAM from the
 * IPL ROM via EXI, validates checksums, and stores the data.
 */
void __OSInitSram(void) {
    SramFlags = 0;
    SramWriteCount = 0;

    /* Read primary SRAM (64 bytes at offset 0) */
    ReadSram(0, &Sram, sizeof(Sram));

    /* Read extended SRAM (64 bytes at offset 0x40) */
    ReadSram(0x40, &SramEx, sizeof(SramEx));

    /* Validate checksums */
    {
        u16 sum = 0;
        u16 sumInv = 0xFFFF;
        u16* data = (u16*)&Sram.ead0;
        u32 i;

        for (i = 0; i < (sizeof(Sram) - 4) / 2; i++) {
            sum += data[i];
            sumInv += ~data[i];
        }

        if (sum != Sram.checkSum || sumInv != Sram.checkSumInv) {
            /* Checksum mismatch -- reinitialize SRAM */
            Sram.checkSum = sum;
            Sram.checkSumInv = sumInv;
            SramFlags |= SRAM_NEED_WRITE;
        }
    }

    /* Sync SRAM if needed */
    if (SramFlags & SRAM_NEED_WRITE) {
        WriteSram(0, &Sram, sizeof(Sram));
    }
}

/*
 * __OSLockSram - Lock and return pointer to primary SRAM.
 * 0x8009D510 | size: 0x19C
 */
void* __OSLockSram(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    /* Wait for any pending writes */
    while (SramFlags & SRAM_WRITING) {
        /* Spin */
    }

    SramFlags |= SRAM_LOCKED;
    OSRestoreInterrupts(enabled);
    return &Sram;
}

/*
 * __OSLockSramEx - Lock and return pointer to extended SRAM.
 * 0x8009D6AC | size: 0x174
 */
void* __OSLockSramEx(void) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    /* Wait for any pending writes */
    while (SramFlags & SRAM_WRITING) {
        /* Spin */
    }

    SramFlags |= SRAM_LOCKED_EX;
    OSRestoreInterrupts(enabled);
    return &SramEx;
}

/*
 * UnlockSram - Internal unlock helper.
 * 0x8009D820 | size: 0x58
 */
static void UnlockSram(BOOL commit, u32 lockBit) {
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (commit) {
        /* Recalculate checksum for primary SRAM */
        if (lockBit == SRAM_LOCKED) {
            u16 sum = 0;
            u16 sumInv = 0xFFFF;
            u16* data = (u16*)&Sram.ead0;
            u32 i;

            for (i = 0; i < (sizeof(Sram) - 4) / 2; i++) {
                sum += data[i];
                sumInv += ~data[i];
            }
            Sram.checkSum = sum;
            Sram.checkSumInv = sumInv;
        }
        SramFlags |= SRAM_NEED_WRITE;
    }

    SramFlags &= ~lockBit;
    OSRestoreInterrupts(enabled);
}

/*
 * __OSUnlockSram - Unlock primary SRAM.
 * 0x8009D878 | size: 0x8C
 */
BOOL __OSUnlockSram(BOOL commit) {
    UnlockSram(commit, SRAM_LOCKED);
    return TRUE;
}

/*
 * __OSUnlockSramEx - Unlock extended SRAM.
 * 0x8009D904 | size: 0x334
 */
BOOL __OSUnlockSramEx(BOOL commit) {
    UnlockSram(commit, SRAM_LOCKED_EX);
    return TRUE;
}

/*
 * __OSSyncSram - Flush any pending SRAM writes.
 * 0x8009DC38 | size: 0x304
 *
 * Returns TRUE if no write is in progress (SRAM is synced).
 */
BOOL __OSSyncSram(void) {
    BOOL synced;

    if (SramFlags & SRAM_NEED_WRITE) {
        SramFlags &= ~SRAM_NEED_WRITE;
        WriteSram(0, &Sram, sizeof(Sram));
    }

    synced = !(SramFlags & SRAM_WRITING);
    return synced;
}

/*
 * OSSetWirelessID - Set the wireless controller ID for a channel.
 * Part of SRAM extended functionality.
 */
void OSSetWirelessID(s32 chan, u16 id) {
    OSSramEx* sramEx;

    sramEx = (OSSramEx*)__OSLockSramEx();
    sramEx->wirelessPadID[chan] = id;
    __OSUnlockSramEx(TRUE);
}

/*
 * OSGetWirelessID - Get the wireless controller ID for a channel.
 */
u16 OSGetWirelessID(s32 chan) {
    OSSramEx* sramEx;
    u16 id;

    sramEx = (OSSramEx*)__OSLockSramEx();
    id = sramEx->wirelessPadID[chan];
    __OSUnlockSramEx(FALSE);
    return id;
}
