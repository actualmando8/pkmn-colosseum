/**
 * @file gs_dvd.c
 * @brief GSDVD -- DVD/disc I/O management and error recovery.
 *
 * This module sits between the sound system (0x80166000) and the script
 * system (0x80168C64) in the link order. It manages:
 *   - Asynchronous disc read operations
 *   - DVD error state machine (cover open, read errors, disc ejection)
 *   - Sound work buffer validation
 *   - Disc status polling and recovery
 *
 * Decompiled from 56 functions in range 0x80167040 - 0x80168C64.
 *
 * Selected functions:
 *   fn_80167040 (GSDVD_CheckAndClose)
 *   fn_80167070 (GSDVD_CloseHandle)
 *   fn_80167118 (GSDVD_Open)
 *   fn_80167298 (GSDVD_Read)
 *   fn_80167318 (GSDVD_ReadAsync)
 *   fn_8016737C (GSDVD_GetReadStatus)
 *   fn_80167408 (GSDVD_CancelRead)
 *   fn_80167490 (GSDVD_WaitForRead)
 *   fn_80167508 (GSDVD_GetFileSize)
 *   fn_8016758C (GSDVD_GetFilePosition)
 *   fn_8016761C (GSDVD_Seek)
 *   fn_80167720 (GSDVD_CheckActive)
 *   fn_80167768 (GSDVD_GetDriveStatus)
 *   fn_801677BC (GSDVD_ResetStatus)
 *   fn_801677F4 (GSDVD_SetErrorCallback)
 *   fn_8016782C (GSDVD_FreeBuffer)
 *   fn_80167864 (GSDVD_AllocReadBuffer)
 *   fn_801678E4 (GSDVD_AllocWriteBuffer)
 *   fn_80167964 (GSDVD_AllocStreamBuffer)
 *   fn_801679E4 (GSDVD_GetBufferPtr)
 *   fn_80167A14 (GSDVD_GetBufferSize)
 *   fn_80167A44 (GSDVD_GetBufferState)
 *   fn_80167A6C (GSDVD_SetBufferState)
 *   fn_80167A9C (GSDVD_ValidateBuffer)
 *   fn_80167AF0 (GSDVD_FreeHandleResources)
 *   fn_80167B70 (GSDVD_GetHandleInfo)
 *   fn_80167BB0 (GSDVD_SetHandleCallback)
 *   fn_80167BE8 (GSDVD_ProcessQueue)
 *   fn_80167D30 (GSDVD_FlushQueue)
 *   fn_80167D60 (GSDVD_QueueRequest)
 *   fn_80167DC0 (GSDVD_IsQueueEmpty)
 *   fn_80167DD8 (GSDVD_GetQueueDepth)
 *   fn_80167E10 (GSDVD_SetPriority)
 *   fn_80167E34 (GSDVD_GetPriority)
 *   fn_80167E54 (GSDVD_NopStub1)
 *   fn_80167E5C (GSDVD_NopStub2)
 *   fn_80167E64 (GSDVD_ErrorCheckCoverOpen)
 *   fn_80167E98 (GSDVD_ErrorCheckReadError)
 *   fn_80167ED0 (GSDVD_ErrorRecovery)
 *   fn_80167EF8 (GSDVD_SetErrorState)
 *   fn_80167F28 (GSDVD_ClearErrorState)
 *   fn_80167FA4 (GSDVD_EmptyFunc)
 *   fn_80167FA8 (GSDVD_ErrorStateMachine)
 *   fn_801680C0 (GSDVD_PollDiscStatus)
 *   fn_80168110 (GSDVD_HandleCoverOpenWait)
 *   fn_80168164 (GSDVD_HandleCoverClosed)
 *   fn_8016819C (GSDVD_HandleDiscChange)
 *   fn_8016821C (GSDVD_HandleFatalError)
 *   fn_8016824C (GSDVD_HandleRetry)
 *   fn_80168284 (_sndCheckSndWorkALL)
 *   fn_80168408 (GSDVD_ValidateAllSndWork)
 *   fn_801684F0 (GSDVD_SndWorkCheck)
 *   fn_80168570 (GSDVD_SndWorkReset)
 *   fn_80168638 (GSDVD_ErrorCoverOpenMain)
 *   fn_80168934 (GSDVD_Init)
 *
 * Debug strings:
 *   "_sndCheckSndWorkALL:Start"
 *   "_sndCheckSndWorkALL:End"
 *   "[GSDVD_ERROR_STATE_COVEROPEN_WAIT]  status = %d"
 *
 * Code patterns:
 *   - Handle-based I/O: mulli r0, r3, 0xC (0xC-byte handle slots)
 *   - State accessed via lbl_80478FAC (sda21, handle table)
 *   - Another state via lbl_80478FB4 (sda21, 0x18-byte entries)
 *   - fn_800F9318 called for resource resolution
 *   - fn_801669E4 called for sound subsystem interaction
 *   - Extensive bit manipulation (extrwi, rlwimi) for flag packing
 *   - fn_80167FA4 is a 4-byte empty function (blr only)
 *
 * Address range: 0x80167040 - 0x80168C64 (7KB, 56 functions)
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void  fn_800F9318(void* ptr, u32 param);        /* resource resolution */
extern void  fn_801669E4(u32 a, u32 b, u32 c);         /* sound subsystem */
extern void  fn_8016A644(void* ptr);                    /* resource cleanup */
extern void  fn_80169520(void* ptr);                    /* status flag update */

/* ===== String constants (rodata) ===== */
extern const char lbl_80273748[]; /* "_sndCheckSndWorkALL:Start" */
extern const char lbl_80273764[]; /* "_sndCheckSndWorkALL:End" */
extern const char lbl_80273780[]; /* "[GSDVD_ERROR_STATE_COVEROPEN_WAIT]..." */

/* ===== BSS / global state ===== */
extern u8 lbl_80478FAC[];  /* DVD handle table (sda21) */
extern u8 lbl_80478FB4[];  /* DVD extended state (sda21) */

/* Forward declarations for converted functions */
s32 fn_80167E54(void);
u32 fn_80167E5C(u8* obj);


/* ===================================================================
 * DVD handle structure (0x0C bytes per slot)
 * =================================================================== */
typedef struct GSDVDHandle {
    /* 0x00 */ u8  flags;       /**< packed flag bits */
    /* 0x01 */ u8  pad[3];
    /* 0x04 */ u32 resourceId;  /**< DVD file resource ID */
    /* 0x08 */ void* buffer;    /**< read buffer pointer */
} GSDVDHandle;

/* ===================================================================
 * DVD extended entry (0x18 bytes per slot)
 * =================================================================== */
typedef struct GSDVDEntry {
    /* 0x00 */ u8  active;      /**< 1 = slot in use */
    /* 0x01 */ u8  pad[3];
    /* 0x04 */ u32 param1;
    /* 0x08 */ u32 param2;
    /* 0x0C */ void* callback;
    /* 0x10 */ u32 state;
    /* 0x14 */ u32 error;
} GSDVDEntry;

/* ==================================================================
 * fn_80167040 -- GSDVD_CheckAndClose
 *
 * Check if a DVD handle is active, and if so, close it with mode 0.
 * 48 bytes.
 *
 * From disassembly (0x80167040, 0x30 bytes):
 *   bl fn_80167720       ; GSDVD_CheckActive
 *   cmplwi r3, 0x0
 *   beq .done
 *   li r4, 0x0
 *   bl fn_80167070       ; GSDVD_CloseHandle
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSDVD_CheckAndClose(void) {
    extern void fn_80167070();
    extern void fn_80167720();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;

    fn_80167720();
    if ((u32)r3 == (u32)0x0) goto L_80167060;
    r4 = 0x0;
    fn_80167070();
L_80167060: ;
    return;
}
#pragma pop

/* ==================================================================
 * fn_80167070 -- GSDVD_CloseHandle
 *
 * Close a DVD handle by index. Frees associated resources and
 * clears the handle slot.
 * 168 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSDVD_CloseHandle(u32 handleIndex, u32 mode) {
    extern void fn_8016782C();
    extern void fn_80167AF0();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = r3 * 0xc;
    r5 = *(u32*)&lbl_80478FAC;
    r31 = r5 + r0;
    r0 = *(u8*)((u8*)r31 + 0x0);
    /* extrwi r0, r0, 1, 26 */;
    if ((u32)r0 == (u32)0x1) goto L_801670A8;
    r3 = 0x0;
    goto L_80167100;
L_801670A8: ;
    r30 = *(u32*)((u8*)r31 + 0x8);
    if ((u32)r30 != (u32)0x0) goto L_801670BC;
    r3 = 0x0;
    goto L_80167100;
L_801670BC: ;
    r0 = r4 & 0xFF;
    if ((u32)r0 != (u32)0x1) goto L_801670D4;
    r4 = 0x0;
    r5 = 0x0;
    ((void(*)(void))fn_801669E4)();
L_801670D4: ;
    r3 = r31;
    r4 = r30;
    fn_80167AF0();
    r3 = r30;
    fn_8016782C();
    r4 = 0x0;
    r3 = 0x1;
    *(u32*)((u8*)r31 + 0x8) = r4;
    r0 = *(u8*)((u8*)r31 + 0x0);
    r0 = (r0 & ~0x00000020) | (((r4 << 5) | ((u32)r4 >> 27)) & 0x00000020);
    *(u8*)((u8*)r31 + 0x0) = r0;
L_80167100: ;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* ==================================================================
 * fn_80167118 -- GSDVD_Open
 *
 * Open a new DVD file handle. Allocates a handle slot, resolves the
 * resource, and sets up the read buffer. Takes 8 parameters.
 * 384 bytes.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
s32 GSDVD_Open(u32 slotIndex, u32 resId, void* callback, u32 param1, u32 param2, u32 param3, u32 param4, u32 param5) {
    extern u8 lbl_8047B0B8[];
    extern u8 lbl_8047B0BC[];
    extern u8 lbl_8047B0C0[];
    extern void fn_800AE78C();
    extern void fn_80159ED0();
    extern void fn_80159EF0();
    extern void fn_80167B70();
    extern void fn_80167BB0();
    extern void fn_80167E64();
    extern void fn_80167F28();
    extern void fn_80167298();
    u8 sp[0x30];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r0 = r3 * 0x18;
    /* stmw r23, 0xc(r1) */;
    r29 = r4;
    r30 = r5;
    r31 = r6;
    r23 = r7;
    r26 = r8;
    r24 = r10;
    r3 = *(u32*)&lbl_80478FB4;
    r28 = r3 + r0;
    r0 = *(u8*)((u8*)r28 + 0x1);
    if ((u32)r0 != (u32)0x1) goto L_80167160;
    r3 = 0x0;
    goto L_80167284;
L_80167160: ;
    r3 = r23;
    r4 = r9;
    ((void(*)(void))fn_800F9318)();
    /* mr. r27, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_8016717C;
    r3 = 0x0;
    goto L_80167284;
L_8016717C: ;
    r3 = r23;
    r4 = r24;
    ((void(*)(void))fn_800F9318)();
    /* mr. r25, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_80167198;
    r3 = 0x0;
    goto L_80167284;
L_80167198: ;
    r3 = r23;
    r4 = r26;
    ((void(*)(void))fn_800F9318)();
    /* mr. r24, r3 */;
    if ((u32)r0 != (u32)0x1) goto L_801671B4;
    r3 = 0x0;
    goto L_80167284;
L_801671B4: ;
    if ((u32)r29 != (u32)0x1) goto L_80167218;
    r26 = 0x0;
    fn_800AE78C();
    r5 = (u32)fn_80167298;
    r4 = r3;
    r3 = (u32)fn_80167298;
    fn_80159ED0();
    r3 = r30;
    fn_80167F28();
    *(u32*)lbl_8047B0BC = r3;
    if ((u32)r3 != (u32)0x0) goto L_801671F0;
    r3 = 0x0;
    goto L_80167284;
L_801671F0: ;
    *(u32*)lbl_8047B0B8 = r31;
    fn_800AE78C();
    fn_80167BB0();
    *(u32*)lbl_8047B0C0 = r3;
    if ((u32)r3 != (u32)0x0) goto L_80167228;
    r3 = *(u32*)lbl_8047B0BC;
    fn_80167E64();
    r3 = 0x0;
    goto L_80167284;
L_80167218: ;
    r26 = r30;
    r3 = 0x0;
    r4 = 0x0;
    fn_80159ED0();
L_80167228: ;
    r4 = *(u16*)((u8*)r28 + 0x2);
    r3 = r27;
    r5 = r26;
    r6 = r25;
    r7 = r24;
    fn_80159EF0();
    r0 = r3 & 0xFF;
    if ((u32)r3 != (u32)0x0) goto L_80167260;
    if ((u32)r29 != (u32)0x1) goto L_80167258;
    r3 = *(u32*)lbl_8047B0BC;
    fn_80167E64();
L_80167258: ;
    r3 = 0x0;
    goto L_80167284;
L_80167260: ;
    if ((u32)r29 != (u32)0x1) goto L_80167278;
    r3 = *(u32*)lbl_8047B0C0;
    fn_80167B70();
    r3 = *(u32*)lbl_8047B0BC;
    fn_80167E64();
L_80167278: ;
    r0 = 0x1;
    r3 = 0x1;
    *(u8*)((u8*)r28 + 0x1) = r0;
L_80167284: ;
    /* lmw r23, 0xc(r1) */;
    return;
}
#pragma pop

/* ==================================================================
 * fn_80167FA4 -- GSDVD_EmptyFunc
 *
 * Empty stub function. 4 bytes (just blr).
 * ================================================================== */
void GSDVD_EmptyFunc(void) {
    /* intentionally empty */
}

/* ==================================================================
 * fn_80167FA8 -- GSDVD_ErrorStateMachine
 *
 * Main error state machine for DVD error recovery.
 * 280 bytes. Handles states:
 *   - Cover open wait (logs "[GSDVD_ERROR_STATE_COVEROPEN_WAIT]")
 *   - Cover closed
 *   - Disc change
 *   - Fatal error
 *   - Retry
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSDVD_ErrorStateMachine(void) {
    extern u8 lbl_80478C24[];
    extern u8 lbl_80478C28[];
    extern u8 lbl_8047B0F4[];
    extern u8 lbl_8047B0F8[];
    extern u8 lbl_8047D578[];
    extern u8 lbl_8047D594[];
    extern void fn_800057A0();
    extern void fn_800057A8();
    extern void fn_800A7BCC();
    extern void fn_800A82FC();
    extern void fn_800FE834();
    extern void fn_8016821C();
    extern void fn_8016824C();
    extern void fn_80167BE8();
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;

    r0 = r3;
    r3 = r0 * 0x44;
    *(u32*)lbl_8047B0F8 = r0;
    fn_8016824C();
    *(u32*)lbl_8047B0F4 = r3;
    if ((u32)r3 != (u32)0x0) goto L_80167FD8;
    r3 = 0x0;
    goto L_801680B0;
L_80167FD8: ;
    fn_8016821C();
    DVDInit();
    fn_800057A8();
    if ((s32)r3 == (s32)0x4) goto L_8016808C;
    fn_800057A0();
    if ((s32)r3 == (s32)0x1) goto L_80168020;
    if ((s32)r3 >= (s32)0x1) goto L_80168008;
    if ((s32)r3 >= (s32)0x0) goto L_80168014;
    goto L_80168034;
L_80168008: ;
    if ((s32)r3 >= (s32)0x3) goto L_80168034;
    goto L_8016802C;
L_80168014: ;
    r0 = (u32)lbl_8047D578;
    *(u32*)lbl_80478C24 = r0;
    goto L_80168034;
L_80168020: ;
    r0 = (u32)lbl_8047D594;
    *(u32*)lbl_80478C24 = r0;
    goto L_80168034;
L_8016802C: ;
    r0 = (u32)lbl_8047D594;
    *(u32*)lbl_80478C24 = r0;
L_80168034: ;
    fn_800A7BCC();
    r4 = *(u32*)lbl_80478C24;
    r0 = 0x0;
    r4 = *(u8*)((u8*)r4 + 0x0);
    *(u8*)((u8*)r3 + 0x0) = r4;
    r4 = *(u32*)lbl_80478C24;
    r4 = *(u8*)((u8*)r4 + 0x1);
    *(u8*)((u8*)r3 + 0x1) = r4;
    r4 = *(u32*)lbl_80478C24;
    r4 = *(u8*)((u8*)r4 + 0x2);
    *(u8*)((u8*)r3 + 0x2) = r4;
    r4 = *(u32*)lbl_80478C24;
    r4 = *(u8*)((u8*)r4 + 0x3);
    *(u8*)((u8*)r3 + 0x3) = r4;
    r4 = *(u32*)lbl_80478C28;
    r4 = *(u8*)((u8*)r4 + 0x0);
    *(u8*)((u8*)r3 + 0x4) = r4;
    r4 = *(u32*)lbl_80478C28;
    r4 = *(u8*)((u8*)r4 + 0x1);
    *(u8*)((u8*)r3 + 0x5) = r4;
    *(u8*)((u8*)r3 + 0x6) = r0;
    *(u8*)((u8*)r3 + 0x7) = r0;
L_8016808C: ;
    r3 = 0x1;
    fn_800A82FC();
    r4 = (u32)fn_80167BE8;
    r3 = 0x1;
    r6 = (u32)fn_80167BE8;
    r5 = 0x0;
    r4 = 0x13;
    fn_800FE834();
    r3 = 0x1;
L_801680B0: ;
    return;
}
#pragma pop

/* ==================================================================
 * fn_80168284 -- _sndCheckSndWorkALL
 *
 * Validate all sound work buffers. Logs start/end markers.
 * 388 bytes.
 *
 * From disassembly references:
 *   lbl_80273748: "_sndCheckSndWorkALL:Start"
 *   lbl_80273764: "_sndCheckSndWorkALL:End"
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void _sndCheckSndWorkALL(void) {
    extern u8 lbl_804526E0[];
    extern u8 lbl_8047D5A0[];
    extern u8 lbl_8047D5A4[];
    extern u8 lbl_8047D5A8[];
    extern void fn_800D5A38();
    extern void fn_800D5FA4();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800D9B58();
    extern void fn_800D9ED8();
    extern void fn_800DA028();
    extern void fn_800DA100();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    r3 = (u32)lbl_804526E0;
    r31 = (u32)lbl_804526E0;
    r0 = *(u8*)((u8*)r31 + 0x19);
    if ((u32)r0 == (u32)0x0) goto L_801683EC;
    r0 = *(u8*)((u8*)r31 + 0x1A);
    if ((u32)r0 == (u32)0x0) goto L_801683EC;
    r30 = *(u8*)((u8*)r31 + 0x18);
    r3 = 0x1;
    r29 = *(u32*)((u8*)r31 + 0x0);
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    f1 = *(f32*)lbl_8047D5A0;
    f3 = *(f32*)lbl_8047D5A4;
    f2 = f1;
    f4 = *(f32*)lbl_8047D5A8;
    fn_800D9B58();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x0;
    r4 = 0x7;
    r5 = 0x0;
    r6 = 0x0;
    r7 = 0x7;
    r8 = 0x0;
    fn_800DA100();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x1;
    fn_800D9ED8();
    r3 = 0x0;
    fn_800DA028();
    r3 = (u32)lbl_804526E0;
    r3 = (u32)lbl_804526E0;
    r3 = *(u32*)((u8*)r3 + 0x20);
    fn_800D7820();
    r3 = 0x6;
    fn_800D6A00();
    r0 = *(u8*)((u8*)r31 + 0x1A);
    /* clrlslwi r3, r0, 18, 2 */;
    fn_800D67BC();
    r31 = 0x0;
    goto L_801683D4;
L_80168370: ;
    r0 = *(u8*)((u8*)r29 + 0x2);
    if ((u32)r0 == (u32)0x0) goto L_801683CC;
    r3 = 0x0;
    fn_800D5FA4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5A38();
    r3 = 0x1;
    fn_800D5FA4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5A38();
    r3 = 0x2;
    fn_800D5FA4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5A38();
    r3 = 0x3;
    fn_800D5FA4();
    r4 = r31;
    r3 = 0x0;
    fn_800D5A38();
L_801683CC: ;
    r31 = r31 + 0x1;
    r29 = r29 + 0x4;
L_801683D4: ;
    r0 = r31 & 0xFF;
    if ((u32)r0 < (u32)r30) goto L_80168370;
    fn_800D6728();
    r3 = 0x0;
    fn_800D9ED8();
L_801683EC: ;
    r31 = *(u32*)(sp + 0x1C);
    r30 = *(u32*)(sp + 0x18);
    r29 = *(u32*)(sp + 0x14);
    return;
}
#pragma pop

/* ==================================================================
 * fn_80168638 -- GSDVD_ErrorCoverOpenMain
 *
 * Main handler for the DVD cover-open error state.
 * 764 bytes -- manages the recovery sequence when the disc lid
 * is opened during gameplay.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSDVD_ErrorCoverOpenMain(void) {
    extern u8 lbl_804526E0[];
    extern u8 lbl_8047B100[];
    extern void fn_800D75F4();
    extern void fn_800D7868();
    extern void fn_800D7894();
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    extern void fn_800FE834();
    extern void fn_80168284();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r4 = (u32)lbl_804526E0;
    r5 = 0x24;
    /* stmw r27, 0xc(r1) */;
    r30 = r3;
    r3 = (u32)lbl_804526E0;
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    /* clrlslwi r27, r30, 24, 2 */;
    r3 = r27;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 == (s32)0) goto L_80168920;
    r4 = (u32)lbl_804526E0;
    r28 = (u32)lbl_804526E0;
    *(u16*)((u8*)r28 + 0x1C) = r3;
    fn_800E27B0();
    r4 = (u32)lbl_804526E0;
    r5 = r27;
    r6 = (u32)lbl_804526E0;
    r4 = 0x0;
    *(u32*)((u8*)r6 + 0x0) = r3;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r27;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    if ((s32)r0 != (s32)0) goto L_8016871C;
    r28 = *(u16*)((u8*)r28 + 0x1C);
    if ((u32)r28 == (u32)0x0) goto L_801686C8;
    r3 = r28;
    fn_800E24B0();
    r3 = r28;
    fn_800E209C();
L_801686C8: ;
    r3 = (u32)lbl_804526E0;
    r3 = (u32)lbl_804526E0;
    r27 = *(u16*)((u8*)r3 + 0x1E);
    if ((u32)r27 == (u32)0x0) goto L_801686EC;
    r3 = r27;
    fn_800E24B0();
    r3 = r27;
    fn_800E209C();
L_801686EC: ;
    r3 = (u32)lbl_804526E0;
    r3 = (u32)lbl_804526E0;
    r3 = *(u32*)((u8*)r3 + 0x20);
    if ((u32)r3 == (u32)0x0) goto L_80168704;
    fn_800D75F4();
L_80168704: ;
    r3 = (u32)lbl_804526E0;
    r4 = 0x0;
    r3 = (u32)lbl_804526E0;
    r5 = 0x24;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_80168920;
L_8016871C: ;
    r4 = (u32)lbl_804526E0;
    r29 = (u32)lbl_804526E0;
    *(u16*)((u8*)r29 + 0x1E) = r3;
    fn_800E27B0();
    r4 = (u32)lbl_804526E0;
    r5 = r27;
    r31 = (u32)lbl_804526E0;
    r4 = 0x0;
    *(u32*)((u8*)r31 + 0x4) = r3;
    memset((void*)r3, (int)r4, (u32)r5);
    r3 = r27;
    fn_800E3534();
    r0 = r3 & 0xFFFF;
    if ((u32)r3 != (u32)0x0) goto L_801687BC;
    r28 = *(u16*)((u8*)r28 + 0x1C);
    if ((u32)r28 == (u32)0x0) goto L_80168770;
    r3 = r28;
    fn_800E24B0();
    r3 = r28;
    fn_800E209C();
L_80168770: ;
    r27 = *(u16*)((u8*)r29 + 0x1E);
    if ((u32)r27 == (u32)0x0) goto L_8016878C;
    r3 = r27;
    fn_800E24B0();
    r3 = r27;
    fn_800E209C();
L_8016878C: ;
    r3 = (u32)lbl_804526E0;
    r3 = (u32)lbl_804526E0;
    r3 = *(u32*)((u8*)r3 + 0x20);
    if ((u32)r3 == (u32)0x0) goto L_801687A4;
    fn_800D75F4();
L_801687A4: ;
    r3 = (u32)lbl_804526E0;
    r4 = 0x0;
    r3 = (u32)lbl_804526E0;
    r5 = 0x24;
    memset((void*)r3, (int)r4, (u32)r5);
    goto L_80168920;
L_801687BC: ;
    r3 = (u32)lbl_804526E0;
    r4 = 0x0;
    r29 = (u32)lbl_804526E0;
    r3 = 0x1e0;
    r0 = 0x280;
    *(u16*)((u8*)r29 + 0xA) = r4;
    r27 = r29 + 0x8;
    *(u16*)((u8*)r29 + 0x8) = r4;
    *(u16*)((u8*)r29 + 0xC) = r4;
    *(u16*)((u8*)r29 + 0xE) = r3;
    *(u16*)((u8*)r29 + 0x10) = r0;
    *(u16*)((u8*)r29 + 0x12) = r3;
    *(u16*)((u8*)r29 + 0x14) = r0;
    *(u16*)((u8*)r29 + 0x16) = r4;
    *(u8*)((u8*)r29 + 0x18) = r30;
    fn_800D7894();
    /* mr. r28, r3 */;
    if ((u32)r3 == (u32)0x0) goto L_8016884C;
    r9 = r27;
    r4 = 0x1;
    r5 = 0x1;
    r6 = 0x0;
    r7 = 0x2;
    r8 = 0x0;
    r10 = 0x4;
    fn_800D7868();
    r9 = *(u32*)((u8*)r31 + 0x4);
    r3 = r28;
    r4 = 0x4;
    r5 = 0x1;
    r6 = 0x6;
    r7 = 0xa;
    r8 = 0x0;
    r10 = 0x4;
    fn_800D7868();
    goto L_80168850;
L_8016884C: ;
    r28 = 0x0;
L_80168850: ;
    r3 = (u32)lbl_804526E0;
    r0 = r30 & 0xFF;
    r3 = (u32)lbl_804526E0;
    r5 = 0x0;
    *(u32*)((u8*)r3 + 0x20) = r28;
    r4 = *(u32*)((u8*)r3 + 0x0);
    goto L_80168878;
L_8016886C: ;
    *(u8*)((u8*)r4 + 0x0) = r5;
    r5 = r5 + 0x1;
    r4 = r4 + 0x4;
L_80168878: ;
    r3 = r5 & 0xFF;
    if ((u32)r3 < (u32)r0) goto L_8016886C;
    r3 = (u32)lbl_804526E0;
    r4 = *(u8*)((u8*)r29 + 0x18);
    r6 = (u32)lbl_804526E0;
    r0 = *(u8*)((u8*)r6 + 0x19);
    if ((u32)r0 >= (u32)r4) goto L_80168900;
    r7 = *(u32*)((u8*)r6 + 0x0);
    r3 = 0x0;
    goto L_801688F4;
L_801688A8: ;
    r0 = *(u8*)((u8*)r7 + 0x1);
    if ((u32)r0 != (u32)0x0) goto L_801688EC;
    r5 = *(u32*)((u8*)r31 + 0x4);
    /* clrlslwi r4, r3, 24, 2 */;
    r3 = 0x0;
    r0 = 0x1;
    r4 = r5 + r4;
    *(u8*)((u8*)r4 + 0x0) = r3;
    *(u8*)((u8*)r4 + 0x1) = r3;
    *(u8*)((u8*)r4 + 0x2) = r3;
    *(u8*)((u8*)r4 + 0x3) = r3;
    *(u8*)((u8*)r7 + 0x1) = r0;
    r3 = *(u8*)((u8*)r6 + 0x19);
    r0 = r3 + 0x1;
    *(u8*)((u8*)r6 + 0x19) = r0;
    goto L_80168904;
L_801688EC: ;
    r3 = r3 + 0x1;
    r7 = r7 + 0x4;
L_801688F4: ;
    r0 = r3 & 0xFF;
    if ((u32)r0 < (u32)r4) goto L_801688A8;
L_80168900: ;
    r7 = 0x0;
L_80168904: ;
    r3 = (u32)fn_80168284;
    *(u32*)lbl_8047B100 = r7;
    r6 = (u32)fn_80168284;
    r4 = 0xfc;
    r3 = 0x1;
    r5 = 0x0;
    fn_800FE834();
L_80168920: ;
    /* lmw r27, 0xc(r1) */;
    return;
}
#pragma pop

/* ==================================================================
 * fn_80168934 -- GSDVD_Init
 *
 * Initialize the DVD subsystem. Largest function in this module
 * at 816 bytes. Sets up handle table, error callbacks, and
 * initial disc state.
 * ================================================================== */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void GSDVD_Init(void) {
    extern u8 lbl_80452708[];
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;
    u32 ctr = 0;

    r6 = (u32)lbl_80452708;
    /* stmw r26, 0x88(r1) */;
    r31 = (u32)lbl_80452708;
    r30 = r3 << 2;
    r26 = r4;
    r3 = r31 + 0x40;
    r29 = r31 + 0x80;
    r27 = r5;
    r0 = *(u32*)(r3 + r30);
    r28 = *(u32*)(r29 + r30);
    if ((s32)r0 != (s32)0x0) goto L_801689BC;
    if ((u32)r28 != (u32)0x0) goto L_8016898C;
    r0 = 0x0;
    r3 = 0x0;
    *(u32*)((u8*)r26 + 0x0) = r0;
    *(u32*)((u8*)r27 + 0x0) = r0;
    goto L_80168C50;
L_8016898C: ;
    r0 = *(u32*)((u8*)r28 + 0x4);
    r0 = r0 & 0x00000008;
    if ((u32)r28 == (u32)0x0) goto L_801689A0;
    *(u32*)((u8*)r26 + 0x0) = r28;
    goto L_801689A8;
L_801689A0: ;
    r0 = 0x0;
    *(u32*)((u8*)r26 + 0x0) = r0;
L_801689A8: ;
    r4 = r31 + 0x0;
    r3 = r28;
    r0 = *(u32*)(r4 + r30);
    *(u32*)((u8*)r27 + 0x0) = r0;
    goto L_80168C50;
L_801689BC: ;
    r0 = 0x0;
    *(u32*)(r3 + r30) = r0;
    if ((u32)r28 != (u32)0x0) goto L_801689E4;
    r4 = r31 + 0x0;
    r3 = 0x0;
    *(u32*)(r4 + r30) = r0;
    *(u32*)((u8*)r26 + 0x0) = r0;
    *(u32*)((u8*)r27 + 0x0) = r0;
    goto L_80168C50;
L_801689E4: ;
    r3 = r1 + 0x8;
    r4 = 0x0;
    r5 = 0x80;
    memset((void*)r3, (int)r4, (u32)r5);
    r5 = *(u32*)((u8*)r28 + 0x4);
    r4 = r1 + 0x8;
    r3 = (0xe00 << 16);
    /* extrwi r0, r5, 1, 28 */;
    /* extrwi r6, r5, 3, 4 */;
    r0 = r0 ^ 0x1;
    r5 = r3 + 0x8;
    r0 = r0 << 3;
    r7 = r6 + r0;
    r0 = r7 << 3;
    *(u32*)(r4 + r0) = r28;
    r6 = *(u32*)((u8*)r28 + 0x0);
    goto L_80168A90;
L_80168A28: ;
    r3 = *(u32*)((u8*)r28 + 0x4);
    r0 = *(u32*)((u8*)r6 + 0x4);
    r0 = r3 ^ r0;
    /* and. r0, r0, r5 */;
    if ((u32)r28 == (u32)0x0) goto L_80168A88;
    r3 = r7 << 3;
    r4 = r1 + 0x8;
    r0 = r3 + 0x4;
    *(u32*)(r4 + r0) = r28;
    r3 = *(u32*)((u8*)r6 + 0x4);
    /* extrwi r0, r3, 1, 28 */;
    /* extrwi r3, r3, 3, 4 */;
    r0 = r0 ^ 0x1;
    r0 = r0 << 3;
    r7 = r3 + r0;
    r0 = r7 << 3;
    r3 = r4 + r0;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 != (u32)0x0) goto L_80168A80;
    *(u32*)((u8*)r3 + 0x0) = r6;
    goto L_80168A88;
L_80168A80: ;
    r3 = *(u32*)((u8*)r3 + 0x4);
    *(u32*)((u8*)r3 + 0x0) = r6;
L_80168A88: ;
    r28 = r6;
    r6 = *(u32*)((u8*)r6 + 0x0);
L_80168A90: ;
    if ((u32)r6 != (u32)0x0) goto L_80168A28;
    r4 = r7 << 3;
    r3 = r1 + 0xc;
    r0 = 0x2;
    *(u32*)(r3 + r4) = r28;
    r3 = r1 + 0x8;
    r7 = 0x0;
    r5 = 0x0;
    r8 = 0x0;
    r6 = 0x0;
    r4 = 0x0;
    ctr_fn = (void(*)(void))r0;
L_80168AC4: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_80168AE8;
    if ((u32)r5 != (u32)0x0) goto L_80168AE0;
    r5 = r0;
    goto L_80168AE4;
L_80168AE0: ;
    *(u32*)((u8*)r7 + 0x0) = r0;
L_80168AE4: ;
    r7 = *(u32*)((u8*)r3 + 0x4);
L_80168AE8: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_80168B0C;
    if ((u32)r5 != (u32)0x0) goto L_80168B04;
    r5 = r0;
    goto L_80168B08;
L_80168B04: ;
    *(u32*)((u8*)r7 + 0x0) = r0;
L_80168B08: ;
    r7 = *(u32*)((u8*)r3 + 0xC);
L_80168B0C: ;
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_80168B30;
    if ((u32)r5 != (u32)0x0) goto L_80168B28;
    r5 = r0;
    goto L_80168B2C;
L_80168B28: ;
    *(u32*)((u8*)r7 + 0x0) = r0;
L_80168B2C: ;
    r7 = *(u32*)((u8*)r3 + 0x14);
L_80168B30: ;
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80168B54;
    if ((u32)r5 != (u32)0x0) goto L_80168B4C;
    r5 = r0;
    goto L_80168B50;
L_80168B4C: ;
    *(u32*)((u8*)r7 + 0x0) = r0;
L_80168B50: ;
    r7 = *(u32*)((u8*)r3 + 0x1C);
L_80168B54: ;
    r3 = r3 + 0x20;
    r4 = r4 + 0x3;
    if (--ctr != 0) goto L_80168AC4;
    r0 = 0x2;
    r3 = r1 + 0x48;
    r4 = 0x8;
    ctr_fn = (void(*)(void))r0;
L_80168B70: ;
    r0 = *(u32*)((u8*)r3 + 0x0);
    if ((u32)r0 == (u32)0x0) goto L_80168B94;
    if ((u32)r6 != (u32)0x0) goto L_80168B8C;
    r6 = r0;
    goto L_80168B90;
L_80168B8C: ;
    *(u32*)((u8*)r8 + 0x0) = r0;
L_80168B90: ;
    r8 = *(u32*)((u8*)r3 + 0x4);
L_80168B94: ;
    r0 = *(u32*)((u8*)r3 + 0x8);
    if ((u32)r0 == (u32)0x0) goto L_80168BB8;
    if ((u32)r6 != (u32)0x0) goto L_80168BB0;
    r6 = r0;
    goto L_80168BB4;
L_80168BB0: ;
    *(u32*)((u8*)r8 + 0x0) = r0;
L_80168BB4: ;
    r8 = *(u32*)((u8*)r3 + 0xC);
L_80168BB8: ;
    r0 = *(u32*)((u8*)r3 + 0x10);
    if ((u32)r0 == (u32)0x0) goto L_80168BDC;
    if ((u32)r6 != (u32)0x0) goto L_80168BD4;
    r6 = r0;
    goto L_80168BD8;
L_80168BD4: ;
    *(u32*)((u8*)r8 + 0x0) = r0;
L_80168BD8: ;
    r8 = *(u32*)((u8*)r3 + 0x14);
L_80168BDC: ;
    r0 = *(u32*)((u8*)r3 + 0x18);
    if ((u32)r0 == (u32)0x0) goto L_80168C00;
    if ((u32)r6 != (u32)0x0) goto L_80168BF8;
    r6 = r0;
    goto L_80168BFC;
L_80168BF8: ;
    *(u32*)((u8*)r8 + 0x0) = r0;
L_80168BFC: ;
    r8 = *(u32*)((u8*)r3 + 0x1C);
L_80168C00: ;
    r3 = r3 + 0x20;
    r4 = r4 + 0x3;
    if (--ctr != 0) goto L_80168B70;
    r3 = 0x0;
    if ((u32)r7 == (u32)0x0) goto L_80168C20;
    r3 = r5;
    *(u32*)((u8*)r7 + 0x0) = r6;
L_80168C20: ;
    if ((u32)r8 == (u32)0x0) goto L_80168C3C;
    if ((u32)r3 != (u32)0x0) goto L_80168C34;
    r3 = r6;
L_80168C34: ;
    r0 = 0x0;
    *(u32*)((u8*)r8 + 0x0) = r0;
L_80168C3C: ;
    r4 = r31 + 0x0;
    *(u32*)(r29 + r30) = r3;
    *(u32*)(r4 + r30) = r6;
    *(u32*)((u8*)r26 + 0x0) = r5;
    *(u32*)((u8*)r27 + 0x0) = r6;
L_80168C50: ;
    /* lmw r26, 0x88(r1) */;
    return;
}
#pragma pop

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 2 functions matched
 * =================================================================== */

/* Address: 0x80167E54 | Size: 0x8 | Pattern: return_constant */
s32 fn_80167E54(void) { return -1; }

/* Address: 0x80167E5C | Size: 0x8 | Pattern: simple_getter */
u32 fn_80167E5C(u8* obj) {
    return *(u32*)((u8*)obj + 0x38);
}
