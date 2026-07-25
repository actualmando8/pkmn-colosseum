/**
 * @file gs_dvd.c
 * @brief DVD-adjacent MusyX sound-runtime unit (per symbols.txt this is
 * mostly _snd-prefixed MusyX audio code, NOT a "GSDVD" disc-I/O API -
 * see 2026-07-02 note below).
 *
 * Per config/GC6E01/splits.txt this unit's real range is
 * 0x80167040 - 0x80168C64. Functions with real (symbols.txt) names -
 * e.g. _sndSetSampleDataUploadCallbackFunction, _sndCheckSndWorkALL,
 * _sndSetVolumeWork, _sndStopSE, _sndStopBGM, _sndInitStack,
 * _sndSetReverbParm, _sndInitParms, _gsdvdErrorTask_801879AC,
 * _gsdvdError_MsgOpen, GSfilterCreate - confirm this is MusyX audio
 * runtime, matching the "people_field" precedent from a prior
 * campaign note (port from reference, don't brute-force). The many
 * remaining fn_XXXXXXXX gaps are genuinely un-decompiled and still
 * need real work; treat any GSDVD_* naming still present below with
 * suspicion, it has not been verified this pass.
 *
 * 2026-07-02 reconciliation: removed 7 orphan definitions whose names
 * are not present in symbols.txt and never paired in objdiff -
 * GSDVD_CheckAndClose, GSDVD_CloseHandle, GSDVD_Open, GSDVD_EmptyFunc,
 * GSDVD_ErrorStateMachine, GSDVD_ErrorCoverOpenMain, GSDVD_Init - along
 * with their unused GSDVDHandle/GSDVDEntry typedefs. Their bodies were
 * invented fiction (fake register-variable pseudo-assembly, e.g.
 * GSDVD_Init's body was actually a free-list/allocator coalescing
 * routine, unrelated to "DVD init"). This pass did not otherwise
 * change any other function or the rest of this file's speculative
 * GSDVD_* naming table below, which remains unverified.
 *
 * 2026-07-03: re-tried porting the archive bodies for fn_80167040
 * ("GSDVD_CheckAndClose", 48 bytes) and fn_80167070
 * ("GSDVD_CloseHandle", 168 bytes) under their correct current
 * fn_ names -- both compile but land at 66.7% / 76.2% fuzzy match
 * (not 100%), confirming the archive content for this file is
 * unreliable register-level fiction, not real disassembly. Reverted
 * both, no net change. The only new function landed this pass is
 * fn_80167FA4 (trivial 4-byte empty function / blr-only stub),
 * confirmed 100% via objdiff.
 */

#include "dolphin/types.h"

/* ===== External SDK / engine functions ===== */
extern void  GSlogWrite(const char* fmt, ...);         /* OSReport / GSlog */
extern void  GSresGetResource(void* ptr, u32 param);        /* resource resolution */
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
void fn_80167FA4(void);


/*
 * 2026-07-02 reconciliation: removed the fictional GSDVDHandle /
 * GSDVDEntry typedefs and the fictional bodies of GSDVD_CheckAndClose,
 * GSDVD_CloseHandle, GSDVD_Open, GSDVD_EmptyFunc and
 * GSDVD_ErrorStateMachine (orphans - none of these names are present in
 * symbols.txt, none paired in objdiff, and neither typedef was even
 * referenced by anything else in this file). See the file header for
 * the honest picture of what this unit actually is.
 */

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
void _sndCheckSndWorkALL(void) {
    extern u8 lbl_804526E0[];
    extern f32 lbl_8047D5A0;
    extern f32 lbl_8047D5A4;
    extern f32 lbl_8047D5A8;
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
    u32 tmp = 0;
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
    tmp = *(u8*)((u8*)r31 + 0x19);
    if (tmp == 0) return;
    tmp = *(u8*)((u8*)r31 + 0x1A);
    if (tmp == 0) return;
    r30 = *(u8*)((u8*)r31 + 0x18);
    r3 = 0x1;
    r29 = *(u32*)((u8*)r31 + 0x0);
    fn_800D88DC();
    r3 = 0x6;
    fn_800D888C();
    f1 = lbl_8047D5A0;
    f3 = lbl_8047D5A4;
    f2 = f1;
    f4 = lbl_8047D5A8;
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
    tmp = *(u8*)((u8*)r31 + 0x1A);
    fn_800D67BC();
    for (r31 = 0x0; (r31 & 0xFF) < r30; r31++, r29 += 0x4) {
        tmp = *(u8*)((u8*)r29 + 0x2);
        if (tmp != 0) {
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
        }
    }
    fn_800D6728();
    r3 = 0x0;
    fn_800D9ED8();

    return;
}

/*
 * 2026-07-02 reconciliation: removed the fictional bodies of
 * GSDVD_ErrorCoverOpenMain and GSDVD_Init (orphans - neither name is
 * present in symbols.txt, neither paired in objdiff). Both were
 * unreferenced anywhere else in-tree. GSDVD_Init's real target address
 * (0x80168934, per its own header comment) is symbols.txt's
 * "particleSort" (size matches exactly, 0x330 bytes) - an unrelated
 * name that doesn't belong to a DVD/sound module; renaming to it was
 * not attempted since the removed body was generic linked-list/
 * allocator code with no real connection to that function's semantics.
 */

/* ==================================================================
 * fn_80167FA4 -- ported from archive (verified 100% via objdiff).
 * Archive labeled this "GSDVD_EmptyFunc" but that name is not in
 * symbols.txt (this unit is actually MusyX audio, see file header);
 * kept the fn_ name. 4-byte empty function (just blr).
 * ================================================================== */
void fn_80167FA4(void) {
    /* intentionally empty */
}

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

/* 2026-07-24: decompiled directly from the current disassembly (not the
 * unreliable archive bodies noted above). */
extern BOOL OSDisableInterrupts(void);
extern BOOL OSRestoreInterrupts(BOOL level);
extern void *fn_80167720(void);
extern void fn_80167070(void *entry, u32 flag);
extern u8 *lbl_8047B0C4;
extern u32 lbl_8047B0C8;
extern u8 *lbl_8047B0CC;
extern u32 lbl_8047B0D0;

void fn_80167040(void)
{
    void *entry;

    entry = fn_80167720();
    if (entry != NULL) {
        fn_80167070(entry, 0);
    }
}

void fn_801677BC(u8 *flag)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    *flag = 0;
    OSRestoreInterrupts(enabled);
}

void fn_801677F4(u8 *flag)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    *flag = 0;
    OSRestoreInterrupts(enabled);
}

void fn_8016782C(u8 *flag)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    *flag = 0;
    OSRestoreInterrupts(enabled);
}

void fn_801679E4(void)
{
    u32 offset;
    u32 i;
    u8 value;

    offset = 0;
    i = 0;
    value = 0;
    while (i < lbl_8047B0C8) {
        lbl_8047B0C4[offset] = value;
        i++;
        offset += 0x78;
    }
}

void fn_80167A14(void)
{
    u32 offset;
    u32 i;
    u8 value;

    offset = 0;
    i = 0;
    value = 0;
    while (i < lbl_8047B0D0) {
        lbl_8047B0CC[offset] = value;
        i++;
        offset += 0xD0;
    }
}


extern u32 fn_800E2C04(u32 size, u32 align);
extern void* fn_800E27B0(u32 handle);
extern u32 fn_800E202C(void);
extern void fn_800E24B0(u32 handle);
extern void fn_800E209C(u32 handle);
extern s32 DVDConvertPathToEntrynum(const char* path);
extern s32 DVDGetCommandBlockStatus(const void* block);
extern s32 DVDGetDriveStatus(void);
extern void DVDClose(void* fileInfo);
extern s32 DVDRead(void* fileInfo, void* addr, s32 length, s32 offset,
                   s32 priority);
extern BOOL DVDReadAsync(void* fileInfo, void* addr, s32 length, s32 offset,
                         void (*callback)(s32 result, void* fileInfo),
                         s32 priority);
void fn_80168164(u8* flag);

typedef struct GSDVDWork {
    u8 active;
    u8 started;
    u8 drawing;
    u8 _pad03;
    u8 fileInfo[0x3C];
    void (*callback)(s32 result, struct GSDVDWork* work);
} GSDVDWork;

extern GSDVDWork* lbl_8047B0F4;
extern u32 lbl_8047B0F8;
extern u8 lbl_804526E0[];
extern void GXDrawDone(void);
extern void fn_800B856C(void);

GSDVDWork* _info2work(void* fileInfo);

void _AsyncCallback(s32 result, void* fileInfo)
{
    GSDVDWork* work;

    work = _info2work(fileInfo);
    if (work != NULL && work->callback != NULL) {
        work->callback(result, work);
    }
}

GSDVDWork* _info2work(void* fileInfo)
{
    GSDVDWork* base;
    GSDVDWork* work;
    u32 i;

    base = lbl_8047B0F4;
    work = base;
    for (i = 0; i < lbl_8047B0F8; i++, work++) {
        if (work->active != 0 && work->fileInfo == fileInfo) {
            return &base[i];
        }
    }
    return NULL;
}

void fn_80167B70(void)
{
    u32 handle = fn_800E202C();

    if ((u16)handle != 0) {
        fn_800E24B0(handle);
        fn_800E209C(handle);
    }
}

void* fn_80167BB0(u32 size)
{
    u32 handle = fn_800E2C04(size, 0x20);

    if ((u16)handle != 0) {
        return fn_800E27B0(handle);
    }
    return 0;
}

s32 fn_80167E10(u8* handle)
{
    return DVDGetCommandBlockStatus(handle + 4);
}

s32 fn_80167E34(void)
{
    return DVDGetDriveStatus();
}

void fn_80167E64(u8* file)
{
    fn_80168164(file);
    DVDClose(file + 4);
}

u32 fn_80167EF8(const char* path)
{
    return DVDConvertPathToEntrynum(path) != -1;
}

u8 fn_80167E98(GSDVDWork* work, void* addr, s32 length, s32 offset,
                void (*callback)(s32 result, GSDVDWork* work))
{
    work->callback = callback;
    return DVDReadAsync(work->fileInfo, addr, length, offset, _AsyncCallback,
                        2);
}

s32 fn_80167ED0(GSDVDWork* work, void* addr, s32 length, s32 offset)
{
    return DVDRead(work->fileInfo, addr, length, offset, 2);
}

void fn_80168164(u8* flag)
{
    BOOL enabled;

    enabled = OSDisableInterrupts();
    *flag = 0;
    OSRestoreInterrupts(enabled);
}

GSDVDWork* fn_8016819C(void)
{
    GSDVDWork* base;
    GSDVDWork* work;
    GSDVDWork* result;
    BOOL enabled;
    u32 i;

    enabled = OSDisableInterrupts();
    result = NULL;
    base = lbl_8047B0F4;
    work = base;
    for (i = 0; i < lbl_8047B0F8; i++, work++) {
        if (work->active != 1) {
            base[i].active = 1;
            result = &lbl_8047B0F4[i];
            break;
        }
    }
    OSRestoreInterrupts(enabled);
    return result;
}

void fn_8016821C(void)
{
    u32 i;

    i = 0;
    while (i < lbl_8047B0F8) {
        lbl_8047B0F4[i].active = 0;
        i++;
    }
}

void* fn_8016824C(u32 size)
{
    u32 handle = fn_800E2C04(size, 0x20);

    if ((u16)handle != 0) {
        return fn_800E27B0(handle);
    }
    return 0;
}

void fn_801684F0(GSDVDWork* work)
{
    if (work->started != 0) {
        work->started = 0;
        if (work->drawing != 0) {
            GXDrawDone();
            fn_800B856C();
            work->drawing = 0;
            lbl_804526E0[0x1A]--;
        }
        lbl_804526E0[0x19]--;
    }
}
