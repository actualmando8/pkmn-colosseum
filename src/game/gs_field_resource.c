/**
 * @file gs_field_resource.c
 * @brief GSfield floor resource pre-load callbacks.
 *
 * Decompiled from:
 *   fn_8011432C (floorReadGFLPreFunc)
 *   fn_801143A0 (floorReadGFLPreFunc_ReturnZero)
 *   fn_801143A8 (floorReadSoundPreFunc_CheckBuffer)
 *   fn_801143EC (floorReadSoundPreFunc_Validate)
 *   fn_8011445C (floorReadSoundPreFunc_AllocBuffer)
 *   fn_801144D0 (floorReadSoundPreFunc)
 *   fn_801145C0 (floorReadParticlePreFunc_Validate)
 *   fn_80114634 (floorReadParticlePreFunc_CheckScene)
 *   fn_801146A4 (floorReadParticlePreFunc_AllocBuffer)
 *   fn_80114714 (floorReadWZXPreFunc_CheckOverlap)
 *   fn_80114760 (floorReadWZXPreFunc)
 *   fn_801147D4 (floorReadPKXPreFunc_CheckOverlap)
 *   fn_80114820 (floorReadPKXPreFunc)
 *   fn_80114878 (floorReadPKXPreFunc_AllocBuffer)
 *   fn_801148CC (floorReadTexPreFunc)
 *   fn_80114AE0 (floorReadCameraPreFunc)
 *   fn_80114CA8 (floorReadMapPreFunc)
 *
 * Each floor archive (FSYS) contains multiple resource types that need
 * to be loaded into memory before the floor becomes active. The pre-func
 * callbacks allocate memory, validate buffer sizes, and set up the
 * loading pipeline.
 *
 * Resource types handled:
 *   - GFL: Floor geometry data (ground mesh, walls)
 *   - Sound: BGM/SE wave data buffers
 *   - Particle: VFX particle system data
 *   - WZX: Walkability/collision mesh
 *   - PKX: Pokemon model data (for overworld encounters)
 *   - Texture: Shared texture packs
 *   - Camera: Pre-set camera angles/positions
 *   - Map: Minimap/area data
 *   - Script: Event script bytecode
 *   - Font: Text rendering data
 *   - Message: Localized string tables
 *   - Normal: Default/miscellaneous data
 *
 * Debug strings:
 *   "floorReadGFLPreFunc(): can't alloc %d bytes of memory"
 *   "ERROR: Over Sound Buffer! snd_res_id=%d buffer size=%d"
 *   "floorReadParticlePreFunc(): can't alloc %d bytes of memory"
 *   "floorReadWZXPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadPKXPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadTexPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadCameraPreFunc: can't alloc %d bytes of memory"
 *   "floorReadMapPreFunc: can't alloc %d bytes of memory"
 *   "floorReadScriptPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadFontPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadMsgPreFunc(): can't alloc %d bytes of memory"
 *   "floorReadNormalPreFunc(): can't alloc %d bytes of memory"
 *
 * Address range: 0x8011432C - 0x80114CA8
 */

#include "dolphin/types.h"
#include "game/world/gs_field.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* fn_800F9418(u32 size, u32 alignment,
                          u32 loadParam, u32 loadParam2,
                          void* callback);               /* GSres alloc+load */

/* ===== String constants (rodata) ===== */
extern const char lbl_80272200[]; /* "floorReadGFLPreFunc(): can't alloc %d bytes..." */
extern const char lbl_80272238[]; /* "ERROR: Over Sound Buffer! snd_res_id=%d..." */
extern const char lbl_80272270[]; /* "floorReadParticlePreFunc(): can't alloc..." */
extern const char lbl_802722B8[]; /* "floorReadWZXPreFunc(): can't alloc..." */
extern const char lbl_802722F0[]; /* "floorReadPKXPreFunc(): can't alloc..." */
extern const char lbl_8027235C[]; /* "floorReadTexPreFunc(): can't alloc..." */
extern const char lbl_80272428[]; /* "floorReadCameraPreFunc: can't alloc..." */
extern const char lbl_802724E8[]; /* "floorReadMapPreFunc: can't alloc..." */
extern const char lbl_80272520[]; /* "floorReadScriptPreFunc(): can't alloc..." */
extern const char lbl_8027255C[]; /* "floorReadFontPreFunc(): can't alloc..." */
extern const char lbl_80272594[]; /* "floorReadMsgPreFunc(): can't alloc..." */
extern const char lbl_802725CC[]; /* "floorReadNormalPreFunc(): can't alloc..." */
extern const char lbl_802722AC[]; /* "scene_data" */

/* ===== BSS / global state ===== */
extern u32 lbl_8047B0B0;  /* sound buffer size limit */

/* ===== Internal callbacks referenced by pre-funcs ===== */
extern void fn_80115094(void);  /* GFL resource completion callback */

/* Forward declarations for converted functions */
u32 fn_801143A0(void);
void fn_801143EC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
void fn_80114714(void);
void fn_801147D4(void);
void fn_80114AE0(void);
void* fn_8011432C(void* owner, u32 param, u32 alloc_size);
void* fn_8011445C(void* owner, u32 param, u32 alloc_size);
void* fn_801145C0(void* owner, u32 param, u32 alloc_size);
void* fn_80114634(void* owner, u32 param, u32 alloc_size);
void* fn_801146A4(void* owner, u32 param, u32 alloc_size);


/* ==================================================================
 * fn_8011432C -- floorReadGFLPreFunc
 *
 * Allocate memory for a GFL floor geometry resource.
 * Rounds size up to 32-byte alignment, calls fn_800F9418 to allocate,
 * and installs fn_80115094 as the completion callback.
 *
 * If allocation fails, logs an error via fn_800DD970 with the
 * "floorReadGFLPreFunc(): can't alloc" string.
 *
 * From disassembly (0x8011432C, 0x74 bytes):
 *   - addi r0, r30, 0x1f      ; round up to 32
 *   - clrrwi r3, r0, 5        ; mask low 5 bits
 *   - li r4, 0x20             ; alignment = 32
 *   - bl fn_800F9418           ; allocate
 *   - cmplwi r31, 0x0         ; check NULL
 *   - bne .success
 *   - lis r3, lbl_80272200@ha ; error string
 * ================================================================== */
void* floorReadGFLPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;
    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode,
    (void*)fn_80115094);
    if (buf == (void*)0) {
    fn_800DD970(lbl_80272200, dataSize);
    }
    return buf;
}

/* ==================================================================
 * fn_801143A0 -- floorReadGFLPreFunc_ReturnZero
 *
 * Stub that returns 0 (NULL). Used as a no-op pre-func for
 * resource types that don't need pre-allocation.
 *
 * From disassembly (0x801143A0, 0x8 bytes):
 *   li r3, 0x0
 *   blr
 * ================================================================== */
void* floorReadGFLPreFunc_ReturnZero(void) {
    return (void*)0;
}

/* ==================================================================
 * fn_801143A8 -- floorReadSoundPreFunc_CheckBuffer
 *
 * Check if the requested sound buffer size exceeds the limit.
 * If so, log an error. Otherwise allocate and pass to the loader.
 *
 * From disassembly (0x801143A8, 0x44 bytes):
 *   - addi r0, r5, 0x1f      ; round up
 *   - clrrwi r0, r0, 5       ; align
 *   - lwz r5, lbl_8047B0B0   ; buffer limit
 *   - cmplw r0, r5           ; check overflow
 *   - ble .ok
 *   - lis r3, lbl_80272238@ha ; "ERROR: Over Sound Buffer!"
 * ================================================================== */
void* floorReadSoundPreFunc_CheckBuffer(u32 resId, u32 loadMode, u32 dataSize) {
    extern u8 lbl_8047B0B4[];
    u8 sp[0x10];
    u32 tmp = 0;

    tmp = dataSize + 0x1f;
    dataSize = *(u32*)&lbl_8047B0B0;
    /* clrrwi tmp, tmp, 5 */;
    if (tmp > dataSize) {
        resId = (u32)&lbl_80272238;
        resId = (u32)&lbl_80272238;
        ((void(*)(void))fn_800DD970)();
    }
    resId = *(u32*)lbl_8047B0B4;
    return;
}

/* ==================================================================
 * fn_801144D0 -- floorReadSoundPreFunc
 *
 * Full sound buffer pre-allocation function. 0xF0 bytes.
 * Validates the sound resource ID, checks buffer capacity,
 * allocates aligned memory, and sets up the sound loading state.
 * ================================================================== */
void* floorReadSoundPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    extern void fn_800E3C08();
    extern void fn_800F9318();
    extern void fn_800F9378();
    extern void fn_80113F48();
    extern void fn_80115BD8();
    extern void fn_801195AC();
    extern void fn_80191ECC();
    extern void fn_801150B8();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r6 = 0;
    u32 r31 = 0;

    resId = resId;
    loadMode = loadMode;
    tmp = loadMode & 0x7FFF0000;
    r31 = 0x0;
    loadMode = tmp | 0x400;
    fn_800F9318();
    resId = resId;
    fn_801195AC();
    resId = resId;
    if (resId == 0) { resId = resId; return; }
    dataSize = (u32)fn_801150B8;
    loadMode = resId;
    r6 = (u32)fn_801150B8;
    dataSize = loadMode;
    fn_800F9378();
    fn_80115BD8();
    resId = resId;
    fn_80113F48();
    loadMode = *(u32*)((u8*)resId + 0x8);
    fn_800F9318();
    loadMode = (u32)&lbl_802722AC;
    loadMode = (u32)&lbl_802722AC;
    fn_80191ECC();
    resId = resId;
    if (resId == 0) {
        resId = 0x0;
        return;
    }
    tmp = *(u32*)((u8*)resId + 0x0);
    if (tmp == 0) { resId = resId; return; }
    tmp = *(u32*)((u8*)resId + 0x8);
    loadMode = 0x0;
    tmp = tmp & 0x7FFF0000;
    resId = tmp | 0x1000;
    while (1) {
        resId = *(u32*)((u8*)resId + 0x0);
        tmp = *(u32*)(resId + loadMode);
        if (tmp == 0) break;
        fn_80113F48();
        loadMode = resId | r31;
        fn_800F9318();
        if (resId != 0) {
            loadMode = resId;
            fn_800E3C08();
        }
        loadMode = loadMode + 0x4;
        r31 = r31 + 0x1;
    }

    resId = resId;

    return;
}

/* ==================================================================
 * fn_80114760 -- floorReadParticlePreFunc
 *
 * Allocate memory for particle effect data within a floor archive.
 * Checks "scene_data" label for the particle system's scene context.
 *
 * From disassembly references lbl_80272270 for error logging.
 * ================================================================== */
void* floorReadParticlePreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    extern void fn_8011522C();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r30 = 0;

    tmp = dataSize + 0x1f;
    r6 = (u32)fn_8011522C;
    /* clrrwi r30, tmp, 5 */;
    dataSize = resId;
    r7 = (u32)fn_8011522C;
    r6 = loadMode;
    resId = r30;
    loadMode = 0x20;
    ((void(*)(void))fn_800F9418)();
    resId = resId;
    if (resId == 0) {
        resId = (u32)&lbl_8027235C;
        loadMode = r30;
        resId = (u32)&lbl_8027235C;
        ((void(*)(void))fn_800DD970)();
    }
    resId = resId;
    return;
}

/* ==================================================================
 * fn_80114820 -- floorReadWZXPreFunc
 *
 * Allocate memory for WZX collision mesh data.
 * References lbl_802722B8 for error logging.
 * ================================================================== */
void* floorReadWZXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}

/* ==================================================================
 * fn_80114878 -- floorReadPKXPreFunc
 *
 * Allocate memory for PKX (Pokemon model) data.
 * References lbl_802722F0 for error logging.
 * ================================================================== */
void* floorReadPKXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}

/* ==================================================================
 * fn_801148CC -- floorReadTexPreFunc
 *
 * Allocate memory for shared texture data.
 * References lbl_8027235C for error logging.
 * ================================================================== */
void* floorReadTexPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}

/* ==================================================================
 * fn_80114AE0 -- floorReadCameraPreFunc
 *
 * Allocate memory for pre-set camera data.
 * References lbl_80272428 for error logging.
 * ================================================================== */
void* floorReadCameraPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    /* TODO: match */
}

/* ===================================================================
 * AUTO-GENERATED accessor functions
 * Generated by tools/gen_accessors.py
 * 1 functions matched
 * =================================================================== */

/* Address: 0x801143A0 | Size: 0x8 | Pattern: return_constant */
u32 fn_801143A0(void) { return 0; }

/* ===================================================================
 * Generated: 0 pattern-matched + 14 stubs
 * Range: 0x8011432C - 0x80114CA8
 * =================================================================== */

/* 0x74 | fn_8011432C | alloc_wrapper */
void* fn_8011432C(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x70 | fn_801143EC | multi_call_guarded */
void fn_801143EC(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    fn_800F9318();
    if (fn_801195AC() == 0) { return; }
    fn_800F9378();
}

/* 0x74 | fn_8011445C | alloc_wrapper */
void* fn_8011445C(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x74 | fn_801145C0 | alloc_wrapper */
void* fn_801145C0(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x70 | fn_80114634 | alloc_wrapper */
void* fn_80114634(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x70 | fn_801146A4 | alloc_wrapper */
void* fn_801146A4(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x4C | fn_80114714 | multi_call_guarded */
void fn_80114714(void) {
    fn_800F9318();
    { fn_800EFD3C(); return; }
    fn_800DD970("");
}

/* 0x801147D4 | 0x34 */
void fn_801147D4(void) {
    extern void fn_800F9318();
    extern void fn_8010CFE4();
    u8 sp[0x10];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r31 = 0;

    fn_800F9318();
    r31 = r3;
    fn_8010CFE4();
    r3 = r31;
    return;
}

/* 0x74 | fn_80114808 | alloc_wrapper */
void* fn_80114808(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return mem;
}

/* 0x8011487C | 0xCC */
/*
 * gsFieldResourceLoad - Load a field resource by ID.
 *
 * Allocates a resource slot, initializes it, parses the data, and
 * registers a completion callback. Reports errors via fn_800DD970.
 *
 * 0x8011487C | size: 0xA8
 */
void* fn_8011487C(u32 owner, u32 resFlags, u32 allocSize) {
    extern void* fn_800F9318(u32 flags);
    extern void fn_800F9378(void* res, u32 owner, u32 flags, void* callback);
    extern void* fn_80191ECC(void* data, const char* name);
    extern void fn_80191F64(u32 allocSize, void* buf);
    extern void fn_801150DC(void);
    extern void* fn_800D27FC(void* data);
    char* strings = (char*)&lbl_80272200;
    void* res;
    void* parsed;
    u32 masked;

    masked = (resFlags & 0x7FFF0000) | 0x400;
    res = fn_800F9318(masked);
    fn_80191F64(allocSize, (void*)((u8*)res + 0x60));
    parsed = fn_80191ECC(res, strings + 0xAC);

    if (parsed == NULL) {
        fn_800DD970(strings + 0x1CC);
        return parsed;
    }

    if (fn_800D27FC((void*) *(u32*)((u8*)parsed + 0x04)) == NULL) {
        fn_800DD970(strings + 0x200);
        return parsed;
    }

    fn_800F9378(res, owner, resFlags, (void*)fn_801150DC);
    return parsed;
}

/* 0x74 | fn_80114948 | alloc_wrapper */
void* fn_80114948(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned + 0x60, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return (u8*)mem + 0x60;
}

/*
 * gsFieldResourceLoadMulti - Load a multi-part field resource.
 *
 * Allocates a resource slot, parses the data using a different string
 * key (lbl_802722AC), then iterates through a pointer table at offset
 * 0x00 of the parsed result, registering each sub-resource.
 *
 * 0x801149BC | size: 0xB4
 */
void* fn_801149BC(u32 owner, u32 resFlags, u32 allocSize) {
    extern void* fn_800F9318(u32 flags);
    extern void fn_800F9378(void* res, u32 owner, u32 flags, void* callback);
    extern void* fn_80191ECC(void* data, const char* name);
    extern void fn_80191F64(u32 size, void* buf);
    extern const char lbl_802722AC[];
    void* res;
    void* parsed;
    u32* ptrTable;
    u32 idx;
    u32 offset;
    u32 maskedFlags;

    res = fn_800F9318(0);
    fn_80191F64(allocSize, (void*)((u8*)res + 0x60));
    parsed = fn_80191ECC(res, lbl_802722AC);

    if (parsed == NULL) {
        return NULL;
    }

    maskedFlags = (resFlags & 0x7FFF0000) | 0x1000;
    ptrTable = *(u32**)((u8*)parsed + 0x00);
    if (ptrTable != NULL) {
        idx = 0;
        offset = 0;
        while (*(u32*)((u8*)ptrTable + offset) != 0) {
            fn_800F9378((void*) *(u32*)((u8*)ptrTable + offset),
                        owner, maskedFlags | idx, NULL);
            offset += 4;
            idx++;
        }
    }

    return parsed;
}

/* 0x70 | fn_80114A70 | alloc_wrapper */
void* fn_80114A70(void* owner, u32 param, u32 alloc_size) {
    u32 aligned = (alloc_size + 0x1F) & ~0x1F;
    void* mem = (void*)fn_800F9418(aligned + 0x60, 0x20, (u32)owner, (u32)param, 0);
    if (mem == NULL) {
        fn_800DD970("");
        return NULL;
    }
    return (u8*)mem + 0x60;
}

/* 0x80114AE0 | 0x1C8 */
void fn_80114AE0(void) {
    extern void fn_800D27FC();
    extern void fn_800DCE4C();
    extern void fn_800F9318();
    extern void fn_800F9378();
    extern void fn_801134E4();
    extern void fn_80115584();
    extern void fn_80115BD8();
    extern void fn_80191ECC();
    extern void fn_80191F64();
    extern void fn_801150DC();
    extern void fn_80115100();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r29 = r3;
    r30 = r4;
    r25 = r5;
    r5 = (u32)&lbl_80272200;
    r24 = 0x0;
    r27 = (u32)&lbl_80272200;
    r31 = 0x0;
    fn_800F9318();
    r26 = r3;
    r5 = r25;
    r4 = r26 + 0x60;
    fn_80191F64();
    r3 = r26;
    r4 = r27 + 0xac;
    fn_80191ECC();
    r26 = r3;
    if (r26 == 0) {
        r3 = 0x0;
        return;
    }
    fn_80115BD8();
    r4 = r30;
    fn_80115584();
    tmp = *(u32*)((u8*)r26 + 0x0);
    if (tmp != 0) {
        tmp = r30 & 0x7FFF0000;
        r25 = 0x0;
        r28 = tmp | 0x1000;
        while (1) {
            r3 = *(u32*)((u8*)r26 + 0x0);
            r3 = *(u32*)(r3 + r25);
            if (r3 == 0) break;
            r4 = r29;
            r5 = r28 | r24;
            r6 = 0x0;
            fn_800F9378();
            r25 = r25 + 0x4;
            r24 = r24 + 0x1;
        }
    }
    tmp = *(u32*)((u8*)r26 + 0x8);
    if (tmp != 0) {
        tmp = r30 & 0x7FFF0000;
        r24 = 0x0;
        r25 = tmp | 0x1600;
        r3 = (u32)fn_80115100;
        r28 = (u32)fn_80115100;
        while (1) {
            r3 = *(u32*)((u8*)r26 + 0x8);
            r3 = *(u32*)(r3 + r24);
            if (r3 == 0) break;
            fn_800DCE4C();
            if (r3 == 0) {
                r4 = r31;
                r3 = r27 + 0x298;
                ((void(*)(void))fn_800DD970)();
                r3 = r27 + 0x2d0;
                ((void(*)(void))fn_800DD970)();
            } else {
                r4 = r29;
                r6 = r28;
                r5 = r25 | r31;
                fn_800F9378();
            }
            r24 = r24 + 0x4;
            r31 = r31 + 0x1;
        }
    }
    r24 = r30 & 0x7FFF0000;
    r25 = r24 | 0x1800;
    if (r26 == 0) {
        r3 = r27 + 0x1cc;
        ((void(*)(void))fn_800DD970)();
    } else {
        r3 = *(u32*)((u8*)r26 + 0x4);
        fn_800D27FC();
        if (r3 == 0) {
            r3 = r27 + 0x200;
            ((void(*)(void))fn_800DD970)();
        } else {
            r5 = (u32)fn_801150DC;
            r4 = r29;
            r6 = (u32)fn_801150DC;
            r5 = r25;
            fn_800F9378();
        }
    }
    r3 = *(u32*)((u8*)r26 + 0xC);
    if (r3 != 0) {
        r3 = *(u32*)((u8*)r3 + 0x0);
        if (r3 != 0) {
            r4 = r29;
            r5 = r24 | 0x1a00;
            r6 = 0x0;
            fn_800F9378();
    }
    }
    r3 = r29;
    r4 = r30;
    fn_801134E4();
    r3 = r26;

    return;
}
