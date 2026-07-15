/**
 * @file hsd_archive.h
 * @brief HSD Archive - .dat archive header / relocation / symbol tables.
 *
 * Adapted from doldecomp/melee src/sysdolphin/baselib/archive.h
 */
#ifndef HSD_ARCHIVE_H
#define HSD_ARCHIVE_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"

#define HSD_ARCHIVE_DONT_FREE 1

typedef struct HSD_ArchiveHeader {
    u32 file_size; /* 0x00 */
    u32 data_size; /* 0x04 */
    u32 nb_reloc;  /* 0x08 */
    u32 nb_public; /* 0x0C */
    u32 nb_extern; /* 0x10 */
    u8 version[4]; /* 0x14 */
    u32 pad[2];    /* 0x18 */
} HSD_ArchiveHeader;

typedef struct HSD_ArchiveRelocationInfo {
    u32 offset; /* 0x00 */
} HSD_ArchiveRelocationInfo;

typedef struct HSD_ArchivePublicInfo {
    u32 offset; /* 0x00 */
    u32 symbol; /* 0x04 */
} HSD_ArchivePublicInfo;

typedef struct HSD_ArchiveExternInfo {
    u32 offset; /* 0x00 */
    u32 symbol; /* 0x04 */
} HSD_ArchiveExternInfo;

struct HSD_Archive {
    HSD_ArchiveHeader header;              /* 0x00 */
    u8* data;                              /* 0x20 */
    HSD_ArchiveRelocationInfo* reloc_info; /* 0x24 */
    HSD_ArchivePublicInfo* public_info;    /* 0x28 */
    HSD_ArchiveExternInfo* extern_info;    /* 0x2C */
    char* symbols;                         /* 0x30 */
    HSD_Archive* next;                     /* 0x34 */
    char* name;                            /* 0x38 */
    u32 flags;                             /* 0x3C */
    void* top_ptr;                         /* 0x40 */
};

s32 HSD_ArchiveParse(HSD_Archive* archive, u8* src, u32 file_size);
void* HSD_ArchiveGetPublicAddress(HSD_Archive* archive, const char* symbols);

#endif /* HSD_ARCHIVE_H */
