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

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800A4D28 - 0x800A4D28 | size: 0x2F4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A4D28(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A501C - 0x800A501C | size: 0xC8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A501C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A50E4 - 0x800A50E4 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A50E4(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A5108 - 0x800A5108 | size: 0x160 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A5108(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A5268 - 0x800A5268 | size: 0xC4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A5268(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A532C - 0x800A532C | size: 0xC0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A532C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A53EC - 0x800A53EC | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A53EC(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A541C - 0x800A541C | size: 0x118 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A541C(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A5534 - 0x800A5534 | size: 0x24 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A5534(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A5558 - 0x800A5558 | size: 0x98 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A5558(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A55F0 - 0x800A55F0 | size: 0x30 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A55F0(void) {
    nofralloc
    /* TODO: decompile */
    blr
}
#pragma pop

/* fn_800A5620 - 0x800A5620 | size: 0x4 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
asm void fn_800A5620(void) {
    nofralloc
    blr
}
#pragma pop

