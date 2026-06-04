/**
 * @file hsd_host.c
 * @brief PC-port host implementations for HSD memory + display entry points.
 *
 * These four symbols have NO real decompiled C in src/hsd and would otherwise
 * be filled in by pcport_link.py's auto-stub generator (which emits a wrong-
 * signature `int sym(){return 0;}` no-op). We provide correctly-typed host
 * implementations here so the link resolves them functionally:
 *
 *   - HSD_MemAlloc / HSD_Free
 *       The HSD allocator. No decompiled C exists for these (asm-only in the
 *       original); HSD_JObjAlloc / HSD_FObjAlloc / HSD_*LoadDesc etc. all route
 *       through them. A malloc/free wrapper with the exact HSD signature is the
 *       correct host behaviour and resolves the entire alloc/free-list chain
 *       (no separate HSD_ObjAlloc/HSD_ObjFree symbols are referenced from C —
 *       they appear only in config comments, so nothing to stub for them).
 *
 *   - HSD_JObjDispAll
 *       The real implementation lives in src/hsd/hsd_jobj_display.c but ONLY as
 *       register-style asm pseudo-C (fn_801A1988 ... fn_801A3FBC); the named
 *       symbol `HSD_JObjDispAll` is never defined there, so it links unresolved.
 *       That TU is not host-portable as-is, so we provide a minimal functional
 *       host impl (scene-graph walk; actual GX emission is handled by the
 *       pcport GX shim elsewhere — display here is a safe no-op-per-node).
 *
 *   - HSD_FObjInterpretAnimAll
 *       Declared in include/hsd/hsd_fobj.h and called from src/hsd/hsd_aobj.c,
 *       but never defined in C (src/hsd/hsd_fobj.c has only load/alloc/remove).
 *       We provide a minimal, crash-safe host impl that walks the FObj list.
 *
 * NOTE on HSD_JObjDispAll signature: include/hsd/hsd_jobj.h declares the
 * canonical 4-arg form; src/hsd/hsd_texp.c re-declares a 3-arg form locally and
 * calls it with 3 args. In C there is no name mangling, so a single linker
 * symbol satisfies both. We define the canonical 4-arg header signature; the
 * 3-arg call site simply leaves `rendermode` indeterminate, which is harmless
 * for the host display path.
 *
 * PCPORT-only. Never compiled into the CodeWarrior byte-match build.
 */

#ifdef PCPORT

#include "dolphin/types.h"
#include "hsd/hsd_fobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_robj.h"
#include "hsd/hsd_forward.h"

#include <stdlib.h>
#include <string.h>

/* ========================================================================= */
/*  HSD allocator (host wrapper over malloc/free)                            */
/* ========================================================================= */

/* Signatures must match include/hsd/hsd_memory.h exactly:
 *   void* HSD_MemAlloc(s32 size);
 *   void  HSD_Free(void* ptr);
 */
void* HSD_MemAlloc(s32 size)
{
    void* ptr;

    if (size <= 0) {
        return NULL;
    }
    ptr = malloc((size_t) size);
    /* HSD callers (HSD_FObjAlloc, HSD_JObjAlloc, ...) memset after alloc, but
     * zero here too so any caller that forgets gets clean memory. */
    if (ptr != NULL) {
        memset(ptr, 0, (size_t) size);
    }
    return ptr;
}

void HSD_Free(void* ptr)
{
    if (ptr != NULL) {
        free(ptr);
    }
}

/* ========================================================================= */
/*  HSD_JObjLoadJoint — build a live HSD_JObj tree from a HSD_Joint desc      */
/* ========================================================================= */

/* The real HSD_JObjLoadJoint is asm-only in the original (no decompiled C),
 * so it would auto-stub to a NULL-returning no-op. This is a faithful host
 * functional implementation of the standard HSD joint-tree load: it allocates
 * each JObj via the real HSD_JObjAlloc, copies the joint's flags + S/R/T, loads
 * the attached display object via the GAME'S real HSD_DObjLoadDesc (which loads
 * the real MObj/PObj/TObj incl. their animation), recurses children + siblings,
 * and marks the matrix dirty. All leaf work is the game's own code; this is just
 * the recursive glue the asm function would otherwise perform. Building the real
 * HSD_JObj tree is the prerequisite for running the game's HSD_JObjAnimAll
 * (texture-matrix UV scroll = the title "sand") + its real render. */
HSD_JObj* HSD_JObjLoadJoint(HSD_Joint* joint)
{
    HSD_JObj* jobj;
    HSD_JObj* c;

    if (joint == NULL) {
        return NULL;
    }

    jobj = HSD_JObjAlloc();
    if (jobj == NULL) {
        return NULL;
    }

    jobj->flags = joint->flags;
    jobj->rotate_x = joint->rotation_x;
    jobj->rotate_y = joint->rotation_y;
    jobj->rotate_z = joint->rotation_z;
    jobj->scale_x = joint->scale_x;
    jobj->scale_y = joint->scale_y;
    jobj->scale_z = joint->scale_z;
    jobj->translate_x = joint->position_x;
    jobj->translate_y = joint->position_y;
    jobj->translate_z = joint->position_z;

    if (union_type_dobj(joint) && joint->u.dobjdesc != NULL) {
        jobj->u.dobj = HSD_DObjLoadDesc(joint->u.dobjdesc);
    }
    if (joint->robjdesc != NULL) {
        jobj->robj = HSD_RObjLoadDesc(joint->robjdesc);
    }

    /* Children (with their full sibling chain), then set their parent. */
    jobj->child = HSD_JObjLoadJoint(joint->child);
    for (c = jobj->child; c != NULL; c = c->next) {
        c->parent = jobj;
    }
    /* Siblings of this joint (parent set by the caller's child loop). */
    jobj->next = HSD_JObjLoadJoint(joint->next);

    HSD_JObjSetMtxDirty(jobj);
    return jobj;
}

/* ========================================================================= */
/*  HSD_JObjDispAll — scene-graph display entry (host)                       */
/* ========================================================================= */

/* Canonical signature from include/hsd/hsd_jobj.h:
 *   void HSD_JObjDispAll(HSD_JObj* jobj, f32 vmtx[3][4], u32 flags,
 *                        u32 rendermode);
 *
 * Walks the joint hierarchy (child + sibling chain) so reference/visibility
 * traversal behaves; actual primitive emission is performed by the pcport GX
 * shim path, so per-node display is a safe no-op here. Hidden joints and their
 * subtrees are skipped, matching JOBJ_HIDDEN semantics. */
void HSD_JObjDispAll(HSD_JObj* jobj, f32 vmtx[3][4], u32 flags, u32 rendermode)
{
    HSD_JObj* j;

    (void) vmtx;
    (void) flags;
    (void) rendermode;

    for (j = jobj; j != NULL; j = j->next) {
        if (j->flags & JOBJ_HIDDEN) {
            continue;
        }
        /* Per-node display (GX emission) is handled by the pcport GX shim.
         * Recurse into children to keep the full hierarchy traversed. */
        if (j->child != NULL) {
            HSD_JObjDispAll(j->child, vmtx, flags, rendermode);
        }
    }
}

/* ========================================================================= */
/*  HSD_FObjInterpretAnimAll — animation interpretation (host)              */
/* ========================================================================= */

/* Canonical signature from include/hsd/hsd_fobj.h:
 *   void HSD_FObjInterpretAnimAll(void* fobj, void* obj,
 *                                 HSD_ObjUpdateFunc obj_update, f32 rate);
 *
 * Faithful host decompilation of fn_80199A88: walk the FObj list and run the
 * per-FObj keyframe interpreter (HSD_FObjInterpretAnim, fn_80199AF8) on each.
 * The interpreter + FObjLoadData + cubic-Hermite eval live in hsd_fobj_host.c
 * (decompiled from the original GC asm; see that file). */
void HSD_FObjInterpretAnimAll(void* fobj, void* obj,
                              HSD_ObjUpdateFunc obj_update, f32 rate)
{
    HSD_FObj* f;

    for (f = (HSD_FObj*) fobj; f != NULL; f = f->next) {
        HSD_FObjInterpretAnim(f, obj, obj_update, rate);
    }
}

#endif /* PCPORT */
