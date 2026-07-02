/**
 * @file gs_field_resource.c
 * @brief GSfield floor resource pre-load callbacks.
 *
 * Decompiled from (corrected attribution -- see notes below):
 *   floorReadGFLPreFunc      (0x8011432C, string-proven: lbl_80272200)
 *   fn_801143A0              (0x801143A0, return_const stub)
 *   fn_801143A8              (0x801143A8, NOT YET MATCHED -- see notes)
 *   fn_801143EC              (0x801143EC, matched, unproven name)
 *   floorReadParticlePreFunc (0x8011445C, string-proven: lbl_80272270)
 *   fn_801144D0              (0x801144D0, NOT YET MATCHED -- see notes)
 *   fn_801145C0              (0x801145C0, matched, unproven name -- see
 *                             two-particle-string-users anomaly below)
 *   floorReadWZXPreFunc      (0x80114634, string-proven: lbl_802722B8)
 *   floorReadPKXPreFunc      (0x801146A4, string-proven: lbl_802722F0)
 *   floorReadTexPostFunc     (0x80114714, string-proven: lbl_80272328)
 *   floorReadTexPreFunc      (0x80114760, NOT YET MATCHED -- see notes)
 *   fn_801147D4              (0x801147D4, matched, unproven name)
 *   floorReadColPreFunc      (0x80114808, string-proven: lbl_80272394)
 *   fn_8011487C              (0x8011487C, matched, unproven name)
 *   floorReadCameraPreFunc   (0x80114948, matched, previously proven)
 *   fn_801149BC              (0x801149BC, matched, unproven name)
 *   floorReadObjPreFunc      (0x80114A70, string-proven: lbl_80272460)
 *
 * ATTRIBUTION NOTE (2026-07-01 scaffold-reconciliation pass):
 *   This file previously carried a dead "orphan" block (a first-generation
 *   recovery attempt) whose function names never matched any symbols.txt
 *   entry, so it never paired in objdiff and its address attributions were
 *   wrong. It has been removed. The renames above are string-anchored: each
 *   function references a rodata string containing its own name, verified
 *   against the target DOL. This OVERRIDES any earlier attribution.
 *
 *   TWO-PARTICLE-STRING ANOMALY: both fn_8011445C and fn_801145C0 reference
 *   the "floorReadParticlePreFunc(): can't alloc..." string (lbl_80272270).
 *   Only fn_8011445C has been renamed to floorReadParticlePreFunc, since it
 *   is the only one with sufficient independent evidence; fn_801145C0 stays
 *   under its fn_ name pending further evidence (e.g. a distinguishing
 *   caller or handler-table cross-reference).
 *
 *   SOUND-FAMILY NAMES REMAIN UNPROVEN: fn_801143A8 and fn_801144D0 are
 *   believed (from the broader gs_floor_data.c / gs_floor.h documentation
 *   trail) to be sound-buffer pre-funcs, but neither has a string-proven
 *   name nor a matched body, so both are kept under fn_ names. A candidate
 *   body was harvested from the old orphan block for each, but the orphan
 *   bodies turned out to be empty "/* TODO: match *\/" placeholders with no
 *   real logic -- there was nothing to adopt. Both remain asm-only (no C
 *   stub) rather than carry a body that can never match. Likewise
 *   floorReadTexPreFunc (0x80114760, string-proven name only -- the orphan
 *   "floorReadTexPreFunc" body was also an empty placeholder) remains
 *   asm-only pending a real decompilation attempt.
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
 * Address range: 0x8011432C - 0x80114AE0 (per splits.txt; 0x80114AE0 itself
 * belongs to the next unit, field_range_80114AE0.c).
 */

#include "dolphin/types.h"
#include "game/world/gs_field.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);         /* OSReport / GSlog */
extern void* fn_800F9418(u32 size, u32 alignment,
                          u32 loadParam, u32 loadParam2,
                          void* callback);               /* GSres alloc+load */

/* ===== String constants (rodata) ===== */
extern const char lbl_80272200[]; /* floorReadGFLPreFunc(): can't alloc... */
extern const char lbl_80272238[]; /* "ERROR: Over Sound Buffer! snd_res_id=%d..." (fn_801143A8, unproven) */
extern const char lbl_80272270[]; /* floorReadParticlePreFunc(): can't alloc... (also read by fn_801145C0, see header note) */
extern const char lbl_802722B8[]; /* floorReadWZXPreFunc(): can't alloc... */
extern const char lbl_802722F0[]; /* floorReadPKXPreFunc(): can't alloc... */
extern const char lbl_8027235C[]; /* floorReadTexPreFunc(): can't alloc... (next unit's Tex string; not yet wired to a matched body) */
extern const char lbl_80272428[]; /* floorReadCameraPreFunc: can't alloc... */
extern const char lbl_802724E8[]; /* floorReadMapPreFunc: can't alloc... (next unit) */
extern const char lbl_80272520[]; /* floorReadScriptPreFunc(): can't alloc... (next unit) */
extern const char lbl_8027255C[]; /* floorReadFontPreFunc(): can't alloc... (next unit) */
extern const char lbl_80272594[]; /* floorReadMsgPreFunc(): can't alloc... (next unit) */
extern const char lbl_802725CC[]; /* floorReadNormalPreFunc(): can't alloc... (next unit) */
extern const char lbl_802722AC[]; /* "scene_data" */
extern const char lbl_80272328[]; /* floorReadTexPostFunc(): <SJIS text> -- string-proves fn_80114714's name */
extern const char lbl_80272394[]; /* floorReadColPreFunc(): can't alloc... -- string-proves fn_80114808's name */
extern const char lbl_80272460[]; /* floorReadObjPreFunc: can't alloc... -- string-proves fn_80114A70's name */

/* ===== BSS / global state ===== */
extern u32 lbl_8047B0B0;  /* sound buffer size limit (fn_801143A8, unproven) */

/* ===== Internal callbacks referenced by pre-funcs ===== */
extern void fn_80115094(void);  /* GFL resource completion callback */
extern void fn_801150B8(void);  /* sound resource completion callback */
extern void fn_80115208(void);  /* resource completion callback (Col/...) */

extern void* fn_800F9318();        /* GSres simple alloc */
extern void* fn_800F9378();        /* GSres install loader */
extern void  fn_8010CFE4();        /* PKX overlap setup */
extern u32   fn_800EFD3C();        /* WZX overlap check */
extern void* fn_801195AC();        /* node lookup */

typedef struct HSDArchiveBuffer {
    u8 header[0x60];
    u8 payload[1];
} HSDArchiveBuffer;

/* ===================================================================
 * Range: 0x8011432C - 0x80114AE0
 * =================================================================== */

/* 0x8011432C | 0x74 | floorReadGFLPreFunc (string-proven) */
#pragma push
#pragma peephole off
void* floorReadGFLPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;

    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode, (void*)fn_80115094);
    if (buf == (void*)0) {
        fn_800DD970(lbl_80272200, dataSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x801143A0 | 0x8 | return_const */
u32 fn_801143A0(void) { return 0; }

/* 0x801143A8 | 0x44 | UNMATCHED -- sound buffer size check (unproven name).
 * ADOPTION ATTEMPT: harvested the old orphan "floorReadSoundPreFunc_CheckBuffer"
 * body for this address; it was only an empty TODO placeholder. Built and
 * confirmed via objdiff at 5.9% fuzzy match (i.e. effectively no match --
 * there was no real logic to adopt). No signature/callee-extern tweak can
 * fix an empty body, so the adoption was reverted. Left asm-only. */

/* 0x801143EC | 0x70 */
#pragma push
#pragma peephole off
void* fn_801143EC(u32 resId, u32 param) {
    void* result = fn_800F9318(resId, (param & 0x7FFF0000) | 0x400);
    void* node = fn_801195AC(result);
    if (node != (void*)0) {
        fn_800F9378(node, resId, param, (void*)fn_801150B8);
    }
    return result;
}
#pragma peephole on
#pragma pop

/* 0x8011445C | 0x74 | floorReadParticlePreFunc (string-proven) */
#pragma push
#pragma peephole off
void* floorReadParticlePreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;

    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, (loadMode & 0x7FFF0000) | 0x400, (void*)0);
    if (buf == (void*)0) {
        fn_800DD970(lbl_80272270, alignedSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x801144D0 | 0xF0 | UNMATCHED -- sound pre-alloc (unproven name).
 * ADOPTION ATTEMPT: harvested the old orphan "floorReadSoundPreFunc" body
 * for this address; it was only an empty TODO placeholder. Built and
 * confirmed via objdiff at 1.7% fuzzy match (i.e. effectively no match --
 * there was no real logic to adopt). No signature/callee-extern tweak can
 * fix an empty body, so the adoption was reverted. Left asm-only. */

/* 0x801145C0 | 0x74 | matched, unproven name.
 * NOTE: also references lbl_80272270 ("floorReadParticlePreFunc(): can't
 * alloc..."), same as floorReadParticlePreFunc above. Two functions read
 * this string; only 0x8011445C has independent evidence for the rename,
 * so this one keeps its fn_ name pending further evidence. */
#pragma push
#pragma peephole off
void* fn_801145C0(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;

    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, (loadMode & 0x7FFF0000) | 0x400, (void*)0);
    if (buf == (void*)0) {
        fn_800DD970(lbl_80272270, alignedSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x80114634 | 0x70 | floorReadWZXPreFunc (string-proven) */
#pragma push
#pragma peephole off
void* floorReadWZXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode, (void*)0);
    if (buf == (void*)0) {
        fn_800DD970(lbl_802722B8, alignedSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x801146A4 | 0x70 | floorReadPKXPreFunc (string-proven) */
#pragma push
#pragma peephole off
void* floorReadPKXPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode, (void*)0);
    if (buf == (void*)0) {
        fn_800DD970(lbl_802722F0, alignedSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x80114714 | 0x4C | floorReadTexPostFunc (string-proven: lbl_80272328 is
 * "floorReadTexPostFunc(): <SJIS text>") */
#pragma push
#pragma peephole off
void* floorReadTexPostFunc(void) {
    void* result = fn_800F9318();
    if (fn_800EFD3C(result) == 0) {
        fn_800DD970(lbl_80272328);
    }
    return result;
}
#pragma peephole on
#pragma pop

/* 0x80114760 | 0x74 | floorReadTexPreFunc -- UNMATCHED (string-proven name
 * only). ADOPTION ATTEMPT: harvested the old orphan "floorReadTexPreFunc"
 * body for this address; it was only an empty TODO placeholder. Built and
 * confirmed via objdiff at 3.4% fuzzy match (i.e. effectively no match --
 * there was no real logic to adopt). No signature/callee-extern tweak can
 * fix an empty body, so the adoption was reverted. Left asm-only. */

/* 0x801147D4 | 0x34 | matched, unproven name */
#pragma push
#pragma peephole off
void* fn_801147D4(void) {
    void* result = fn_800F9318();
    fn_8010CFE4();
    return result;
}
#pragma peephole on
#pragma pop

/* 0x80114808 | 0x74 | floorReadColPreFunc (string-proven: lbl_80272394) */
#pragma push
#pragma peephole off
void* floorReadColPreFunc(u32 resId, u32 loadMode, u32 dataSize) {
    void* buf;
    u32 alignedSize;

    alignedSize = (dataSize + 0x1F) & ~0x1F;
    buf = fn_800F9418(alignedSize, 0x20, resId, loadMode, (void*)fn_80115208);
    if (buf == (void*)0) {
        fn_800DD970(lbl_80272394, alignedSize);
    }
    return buf;
}
#pragma peephole on
#pragma pop

/* 0x8011487C | 0xCC | matched, unproven name */
#pragma push
#pragma peephole off
void* fn_8011487C(u32 resId, u32 loadMode, u32 dataSize) {
#pragma optimization_level 4
    extern void HSD_ArchiveParse(void* archive, void* buf, u32 size);
    extern void* HSD_ArchiveGetPublicAddress(void* archive, const char* sym);
    extern void* fn_800D27FC(void* model);
    extern void fn_800F9378(void* entry, u32 resId, u32 loadMode, void* callback);
    extern u32 fn_801150DC(void);
    const u8* strings;
    HSDArchiveBuffer* archive;
    void* pub;
    void* entry;
    u32 flags;

    flags = loadMode & 0x7FFF0000;
    strings = (const u8*)lbl_80272200;
    archive = fn_800F9318(resId, flags | 0x400);
    HSD_ArchiveParse(archive, archive->payload, dataSize);
    pub = HSD_ArchiveGetPublicAddress(archive, (const char*)(strings + 0xAC));
    if (pub == (void*)0) {
        fn_800DD970((const char*)(strings + 0x1CC));
    } else {
        entry = fn_800D27FC(*(void**)((u8*)pub + 4));
        if (entry == (void*)0) {
            fn_800DD970((const char*)(strings + 0x200));
        } else {
            fn_800F9378(entry, resId, loadMode, (void*)fn_801150DC);
        }
    }
    return pub;
}
#pragma peephole on
#pragma pop

/* 0x80114948 | 0x74 | floorReadCameraPreFunc (previously proven, matched) */
#pragma push
/* Matches the map pre-load wrapper, but forces the camera load flag. */
#pragma peephole off
void* floorReadCameraPreFunc(void* owner, u32 param, u32 alloc_size) {
    u32 total = ((alloc_size + 0x1F) & ~0x1F) + 0x60;
    HSDArchiveBuffer* archive = fn_800F9418(total, 0x20, (u32)owner, (param & 0x7FFF0000) | 0x400, (void*)0);
    if (archive == (void*)0) {
        fn_800DD970(lbl_80272428, total);
        return (void*)0;
    }
    return archive->payload;
}
#pragma pop

/* 0x801149BC | 0xB4 | matched, unproven name */
#pragma push
#pragma peephole off
void* fn_801149BC(u32 resId, u32 loadMode, u32 dataSize) {
    extern void* fn_800F9318(void);
    extern void HSD_ArchiveParse(void* archive, void* buf, u32 size);
    extern void* HSD_ArchiveGetPublicAddress(void* archive, const char* sym);
    extern void fn_800F9378(void* entry, u32 resId, u32 flags, u32 cb);
    HSDArchiveBuffer* archive;
    void* pub;
    void* entry;
    u32 flags;
    u32 offset;
    u32 counter;

    counter = 0;
    archive = fn_800F9318();
    HSD_ArchiveParse(archive, archive->payload, dataSize);
    pub = HSD_ArchiveGetPublicAddress(archive, lbl_802722AC);
    if (pub == (void*)0) {
        return (void*)0;
    }
    flags = (loadMode & 0x7FFF0000) | 0x1000;
    if (*(u32*)pub != 0) {
        offset = 0;
        while ((entry = *(void**)(*(u8**)pub + offset)) != 0) {
            fn_800F9378(entry, resId, flags | counter, 0);
            offset += 4;
            counter++;
        }
    }
    return pub;
}
#pragma peephole on
#pragma pop

/* 0x80114A70 | 0x70 | floorReadObjPreFunc (string-proven: lbl_80272460) */
#pragma push
#pragma peephole off
void* floorReadObjPreFunc(void* owner, u32 param, u32 alloc_size) {
    u32 total = ((alloc_size + 0x1F) & ~0x1F) + 0x60;
    HSDArchiveBuffer* archive = fn_800F9418(total, 0x20, (u32)owner, param, (void*)0);
    if (archive == (void*)0) {
        fn_800DD970(lbl_80272460, total);
        return (void*)0;
    }
    return archive->payload;
}
#pragma peephole on
#pragma pop

/* NOTE: 0x80114AE0 belongs to the NEXT unit (field_range_80114AE0.c) per
 * splits.txt (this unit ends at 0x80114AE0, exclusive). A definition for
 * it was previously (incorrectly) included here; a cross-unit definition
 * is an invariant hazard, so it has been removed. It remains asm-only in
 * its own unit. */
