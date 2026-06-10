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
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_tobj.h"
#include "hsd/hsd_robj.h"
#include "hsd/hsd_forward.h"
#include "real_content_host.h"

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

/* ========================================================================= */
/*  Animation dispatch (host) — FObj value -> JObj/TObj field                 */
/* ========================================================================= */

/*
 * VERIFIED DISPATCH MODEL (see lane verification + project memory):
 *   HSD_AObjInterpretAnim(aobj, obj, update_func) calls HSD_FObjInterpretAnimAll,
 *   which runs the per-FObj interpreter (hsd_fobj_host.c). That interpreter calls
 *   update_func(obj, fobj->obj_type, &value) for each produced value -- BUT ONLY
 *   when update_func != NULL. If NULL is passed (as the adapted src TObj/JObj C
 *   does), NO value is dispatched to any field and the animation has no effect.
 *
 *   On GC the real fn_801BBFE4/fn_801BC33C do the TObj texture-matrix build from
 *   already-updated TObj scalar fields (a separate pass over the bound TObj list);
 *   the actual FObj->field assignment is what the update_func is for. For the host
 *   we therefore PASS A REAL update_func so the FObj keyframe values flow into the
 *   live HSD_JObj / HSD_TObj SRT fields. These two dispatchers map the HSD_A_J_* /
 *   HSD_A_T_* obj_type codes (include/hsd/hsd_jobj.h, hsd_tobj.h) to the matching
 *   struct field.
 */

static void PCPort_JObjUpdateFunc(void* obj, u32 type, HSD_ObjData* val)
{
    HSD_JObj* jobj = (HSD_JObj*) obj;
    if (jobj == NULL || val == NULL) {
        return;
    }
    switch (type) {
    case HSD_A_J_ROTX: jobj->rotate_x    = val->fv; break;
    case HSD_A_J_ROTY: jobj->rotate_y    = val->fv; break;
    case HSD_A_J_ROTZ: jobj->rotate_z    = val->fv; break;
    case HSD_A_J_TRAX: jobj->translate_x = val->fv; break;
    case HSD_A_J_TRAY: jobj->translate_y = val->fv; break;
    case HSD_A_J_TRAZ: jobj->translate_z = val->fv; break;
    case HSD_A_J_SCAX: jobj->scale_x     = val->fv; break;
    case HSD_A_J_SCAY: jobj->scale_y     = val->fv; break;
    case HSD_A_J_SCAZ: jobj->scale_z     = val->fv; break;
    default:
        /* HSD_A_J_PATH / NODE / BRANCH / SETBYTE / SETFLOAT are not driven on the
         * host SRT path; ignore so an unexpected code never writes a stray field. */
        return;
    }
    HSD_JObjSetMtxDirty(jobj);
}

static void PCPort_TObjUpdateFunc(void* obj, u32 type, HSD_ObjData* val)
{
    HSD_TObj* tobj = (HSD_TObj*) obj;
    if (tobj == NULL || val == NULL) {
        return;
    }
    switch (type) {
    case HSD_A_T_TRAU: tobj->translate_x = val->fv; break;
    case HSD_A_T_TRAV: tobj->translate_y = val->fv; break;
    case HSD_A_T_SCAU: tobj->scale_x     = val->fv; break;
    case HSD_A_T_SCAV: tobj->scale_y     = val->fv; break;
    case HSD_A_T_ROTX: tobj->rotate_x    = val->fv; break;
    case HSD_A_T_ROTY: tobj->rotate_y    = val->fv; break;
    case HSD_A_T_ROTZ: tobj->rotate_z    = val->fv; break;
    case HSD_A_T_BLEND: tobj->blending   = val->fv; break;
    case HSD_A_T_TIMG: /* texture image swap from imagetbl[iv] */
        if (tobj->imagetbl != NULL && val->iv >= 0) {
            tobj->imagedesc = tobj->imagetbl[val->iv];
        }
        return; /* image swap does not dirty the tex matrix */
    case HSD_A_T_TCLT: /* TLUT swap from tluttbl[iv] */
        if (tobj->tluttbl != NULL && val->iv >= 0) {
            tobj->tlut = tobj->tluttbl[val->iv];
        }
        return;
    default:
        /* HSD_A_T_LOD_BIAS and any unknown code: no SRT effect. */
        return;
    }
    /* Any translate/rotate/scale/blend change invalidates the texture matrix. */
    tobj->flags |= TEX_MTX_DIRTY;
}

/* ------------------------------------------------------------------------- */
/*  HSD_TObjAnim (host override)                                              */
/*                                                                           */
/*  src/hsd/hsd_tobj.c defines HSD_TObjAnim with an EMPTY body (the adapted   */
/*  src never wired the dispatch). pcport/hsd_host.c is in the BOOT link set, */
/*  compiled+linked BEFORE the generated src/hsd TUs, so /FORCE:MULTIPLE      */
/*  takes THIS definition first -- a functional host override of the empty    */
/*  game stub. It interprets the TObj's AObj keys into the TObj SRT fields    */
/*  via PCPort_TObjUpdateFunc. (Reached through HSD_MObjAnim ->               */
/*  HSD_TObjAnimAll -> HSD_TObjAnim, which the src TUs still provide.)        */
/* ------------------------------------------------------------------------- */
void HSD_TObjAnim(HSD_TObj* tobj)
{
    if (tobj == NULL || tobj->aobj == NULL) {
        return;
    }
    HSD_AObjInterpretAnim(tobj->aobj, tobj, PCPort_TObjUpdateFunc);
}

/* ------------------------------------------------------------------------- */
/*  HSD_JObjAnim (host override)                                              */
/*                                                                           */
/*  src/hsd/hsd_jobj.c HSD_JObjAnim only runs HSD_RObjAnimAll + HSD_DObjAnim  */
/*  and never interprets the joint's OWN aobj -> joint SRT animation does     */
/*  nothing. This BOOT-set override adds the missing                          */
/*  HSD_AObjInterpretAnim(jobj->aobj, jobj, PCPort_JObjUpdateFunc) before the */
/*  RObj/DObj passes. HSD_JObjAnimAll (src) recurses calling HSD_JObjAnim, so */
/*  overriding just HSD_JObjAnim covers the whole tree.                       */
/* ------------------------------------------------------------------------- */
void HSD_JObjAnim(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    if (jobj->aobj != NULL) {
        HSD_AObjInterpretAnim(jobj->aobj, jobj, PCPort_JObjUpdateFunc);
    }
    HSD_RObjAnimAll(jobj->robj);
    if (union_type_dobj(jobj)) {
        /* DObjAnimAll walks the whole DObj chain (the src HSD_JObjAnim used the
         * single-DObj HSD_DObjAnim, which would skip chained material sets). */
        HSD_DObjAnimAll(jobj->u.dobj);
    }
}

/* ------------------------------------------------------------------------- */
/*  HSD_JObjAnimAll (host override)                                           */
/*                                                                           */
/*  CRITICAL FIX: src/hsd/hsd_jobj.c ALSO defines HSD_JObjAnimAll, and its    */
/*  recursion `HSD_JObjAnimAll -> HSD_JObjAnim` binds INTRA-TU at compile     */
/*  time to that file's OWN (inert) HSD_JObjAnim -- which never interprets    */
/*  the joint's aobj. So overriding HSD_JObjAnim alone (above) is bypassed    */
/*  for the whole-tree walk: the override only intercepts CROSS-TU callers.   */
/*  This host HSD_JObjAnimAll (BOOT-linked first, so /FORCE:MULTIPLE selects  */
/*  it for cross-TU callers) recurses calling the SAME-TU host HSD_JObjAnim   */
/*  above, so the per-joint aobj IS interpreted and curr_frame advances.      */
/*  Without this the joint SRT animation never plays (curr_frame stuck at 0). */
/* ------------------------------------------------------------------------- */
void HSD_JObjAnimAll(HSD_JObj* jobj)
{
    if (jobj == NULL) {
        return;
    }
    HSD_JObjAnim(jobj);
    HSD_JObjAnimAll(jobj->child);
    HSD_JObjAnimAll(jobj->next);
}

/* ------------------------------------------------------------------------- */
/*  HSD_FObjReqAnimAll (host override)                                        */
/*                                                                           */
/*  The adapted src/hsd/hsd_fobj.c HSD_FObjReqAnimAll sets f->flags = 0,      */
/*  which is the interpreter's "idle" state -> the animation never starts.    */
/*  The real GC fn sets the low nibble to 2 (load-next-packet). This BOOT-set */
/*  override does the correct thing so HSD_JObjReqAnimAll alone is enough to  */
/*  arm playback (it routes through HSD_AObjReqAnim -> HSD_FObjReqAnimAll).    */
/* ------------------------------------------------------------------------- */
#ifndef PCPORT_USE_SRC_FOBJ_REQ_ANIM_ALL
void HSD_FObjReqAnimAll(HSD_FObj* fobj, f32 startframe)
{
    HSD_FObj* f;
    for (f = fobj; f != NULL; f = f->next) {
        f->ad = f->ad_head;
        f->time = startframe;
        f->flags = (u8) ((f->flags & 0xF0) | 2); /* load-next-packet start state */
        f->nb_pack = 0;
        /* Reset segment state so the interpreter re-reads fterm from the stream
         * on the next call.  Without this, re-arm after loop-rewind leaves fterm
         * at the last-cycle value, causing state-4 to sample the terminal segment
         * control points for the entire next cycle instead of starting from
         * segment 0.  The initial load has fterm=0 (from HSD_FObjAlloc memset),
         * which produces the correct "fall through to state-3 immediately" path;
         * re-arm must replicate that condition. */
        f->fterm = 0;
        f->p0    = 0.0f;
        f->p1    = 0.0f;
        f->d0    = 0.0f;
        f->d1    = 0.0f;
    }
}
#endif

/* ------------------------------------------------------------------------- */
/*  PCPort_HSDStartAnimAll — belt-and-suspenders FObj kickoff                 */
/*                                                                           */
/*  After HSD_JObjAddAnimAll builds the live AObj/FObj tree, walk it and put  */
/*  every FObj chain into the start state. HSD_JObjReqAnimAll already routes  */
/*  through the overridden HSD_FObjReqAnimAll above, but this also covers any */
/*  AObj reached only via the DObj/MObj material path and is safe to call     */
/*  redundantly (idempotent: it just (re)arms the state machine at frame 0).  */
/* ------------------------------------------------------------------------- */
static void PCPort_StartAObj(HSD_AObj* aobj)
{
    if (aobj != NULL && aobj->fobj != NULL) {
        PCPort_FObjStartAnim(aobj->fobj, 0.0f);
    }
}

void PCPort_HSDStartAnimAll(HSD_JObj* root)
{
    HSD_JObj* j;
    for (j = root; j != NULL; j = j->next) {
        PCPort_StartAObj(j->aobj);
        if (union_type_dobj(j)) {
            HSD_DObj* d;
            for (d = j->u.dobj; d != NULL; d = d->next) {
                HSD_MObj* m = d->mobj;
                if (m != NULL) {
                    HSD_TObj* t;
                    PCPort_StartAObj(m->aobj);
                    for (t = m->tobj; t != NULL; t = t->next) {
                        PCPort_StartAObj(t->aobj);
                    }
                }
            }
        }
        if (j->child != NULL) {
            PCPort_HSDStartAnimAll(j->child);
        }
    }
}

#endif /* PCPORT */
