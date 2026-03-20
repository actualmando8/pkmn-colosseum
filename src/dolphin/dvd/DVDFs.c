#include "dolphin/dvd/dvd.h"

/*
 * DVDFs.c - DVD filesystem initialization.
 *
 * Reads the FST (File System Table) pointer from the disc header
 * and sets up the string table pointer and entry count.
 *
 * Matches: 0x800A4CF0 | size: 0x38
 */

/* Boot info / disc header at 0x80000000 */
#define BOOT_INFO       ((u32*)0x80000000)

/* SDA-relative globals */
static u32* BootInfo;           /* 0x8047A7C8 */
static u32* FstStart;           /* 0x8047A7CC */
static u32* FstStringStart;     /* 0x8047A7D0 */
static u32  MaxEntryNum;        /* 0x8047A7D4 */

/*
 * __DVDFSInit - Initialize the DVD filesystem
 * 0x800A4CF0 | size: 0x38
 *
 * Reads the FST location from offset 0x38 in the boot info,
 * then extracts the root entry count and string table offset.
 *
 * FST entry format (12 bytes each):
 *   word 0: flags/name_offset
 *   word 1: file_offset or parent_dir
 *   word 2: file_length or num_entries (for root: total entries)
 */
void __DVDFSInit(void) {
    BootInfo = BOOT_INFO;

    /* FST start address is at offset 0x38 in boot info */
    FstStart = (u32*)BootInfo[0x38 / 4];

    if (FstStart == NULL) {
        return;
    }

    /* Root entry's third word contains the total number of entries */
    MaxEntryNum = FstStart[2];

    /* String table immediately follows the FST entries */
    FstStringStart = (u32*)((u8*)FstStart + MaxEntryNum * 12);
}
