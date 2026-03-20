/**
 * @file gs_dvd.h
 * @brief GSDVD -- DVD/disc I/O management and error recovery.
 *
 * Manages asynchronous disc operations, error handling (cover open,
 * read errors), and sound work buffer validation.
 *
 * Handle table stored in BSS at lbl_80478FAC (0x0C bytes per handle).
 * Extended state at lbl_80478FB4 (0x18 bytes per entry).
 *
 * Debug strings:
 *   "_sndCheckSndWorkALL:Start"
 *   "_sndCheckSndWorkALL:End"
 *   "[GSDVD_ERROR_STATE_COVEROPEN_WAIT]  status = %d"
 *
 * Address range: 0x80167040 - 0x80168C64 (7KB, 56 functions)
 */
#ifndef GS_DVD_H
#define GS_DVD_H

#include "dolphin/types.h"

/* ===================================================================
 * Constants
 * =================================================================== */

#define GSDVD_HANDLE_SIZE       0x0C
#define GSDVD_ENTRY_SIZE        0x18

/* DVD error states */
#define GSDVD_ERROR_NONE            0
#define GSDVD_ERROR_COVEROPEN_WAIT  1
#define GSDVD_ERROR_COVER_CLOSED    2
#define GSDVD_ERROR_DISC_CHANGE     3
#define GSDVD_ERROR_FATAL           4
#define GSDVD_ERROR_RETRY           5

/* ===================================================================
 * Structures
 * =================================================================== */

typedef struct GSDVDHandle {
    /* 0x00 */ u8  flags;
    /* 0x01 */ u8  pad[3];
    /* 0x04 */ u32 resourceId;
    /* 0x08 */ void* buffer;
} GSDVDHandle;

typedef struct GSDVDEntry {
    /* 0x00 */ u8  active;
    /* 0x01 */ u8  pad[3];
    /* 0x04 */ u32 param1;
    /* 0x08 */ u32 param2;
    /* 0x0C */ void* callback;
    /* 0x10 */ u32 state;
    /* 0x14 */ u32 error;
} GSDVDEntry;

/* ===================================================================
 * Public API
 * =================================================================== */

/** fn_80167040 */ void GSDVD_CheckAndClose(void);
/** fn_80167070 */ s32  GSDVD_CloseHandle(u32 handleIndex, u32 mode);
/** fn_80167118 */ s32  GSDVD_Open(u32 slotIndex, u32 resId, void* callback,
                                    u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
/** fn_80167FA4 */ void GSDVD_EmptyFunc(void);
/** fn_80167FA8 */ void GSDVD_ErrorStateMachine(void);
/** fn_80168284 */ void _sndCheckSndWorkALL(void);
/** fn_80168638 */ void GSDVD_ErrorCoverOpenMain(void);
/** fn_80168934 */ void GSDVD_Init(void);

#endif /* GS_DVD_H */
