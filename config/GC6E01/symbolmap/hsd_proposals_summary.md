# HSD Naming Proposals — Colosseum vs. Melee Alignment

Date: 2026-07-01. Read-only pass; no repo files touched.
Output: `hsd_proposals.json` (139 proposals: 119 A, 20 B). Reference corpus:
doldecomp/melee `src/sysdolphin/baselib/*` (cached under `melee_src/`).

## Method

- Anchors: existing named `HSD_*`/`*Obj*` symbols in `config/GC6E01/symbols.txt`
  within the HSD strata (0x80190E34–0x801C0F20), cross-checked at exact
  addresses via `local_symbols.json`.
- Alignment: content/semantic matching against melee source (callee shape,
  struct-field offsets, vtable-slot stores in `*InfoInit` class registration,
  distinctive constants, even a byte-identical melee copy/paste bug), with
  order+size as supporting evidence only — melee source order does NOT match
  Colosseum link order.
- Confidence A = callee-shape + semantics match a unique melee function;
  B = probable with residual ambiguity (alternative noted in evidence);
  C = skipped, not emitted.
- Every proposed name verified absent from all 14,600 existing symbol names
  (no duplicate-symbol risk), and every fn/addr pair verified to still be an
  unnamed `fn_` at that exact address.

## Counts per unit (A / B / total)

| unit | A | B | total |
|---|---|---|---|
| hsd/hsd_cobj.c | 23 | 0 | 23 |
| hsd/hsd_range_801BBAC8.c (actually melee tobj.c + util.c) | 15 | 6 | 21 |
| hsd/hsd_jobj.c | 13 | 7 | 20 |
| hsd/hsd_range_801A69C0.c (actually melee mobj.c) | 15 | 1 | 16 |
| hsd/hsd_pobj_range_801AA608.c | 12 | 0 | 12 |
| hsd/hsd_lobj.c | 7 | 1 | 8 |
| hsd/hsd_fog.c | 6 | 0 | 6 |
| hsd/hsd_mobj_range_801A86B4.c (mtx.c/objalloc.c territory) | 6 | 0 | 6 |
| hsd/hsd_class.c | 4 | 1 | 5 |
| hsd/hsd_displayfunc.c | 1 | 3 | 4 |
| hsd/hsd_dobj.c | 4 | 0 | 4 |
| hsd/hsd_texp.c | 3 | 0 | 3 |
| hsd/hsd_aobj_range_801C01C8.c | 2 | 0 | 2 |
| hsd/hsd_initialize.c | 1 | 1 | 2 |
| hsd/hsd_shadow.c | 2 | 0 | 2 |
| hsd/hsd_robj_find_by_type.c | 1 | 0 | 1 |
| hsd/hsd_robj_get_alloc_data.c | 1 | 0 | 1 |
| hsd/hsd_robj_get_alloc_data2.c | 1 | 0 | 1 |
| hsd/hsd_object.c | 1 | 0 | 1 |
| hsd/hsd_wobj.c | 1 | 0 | 1 |
| **Total** | **119** | **20** | **139** |

## 10 best samples (all confidence A)

| fn | addr | proposed | key evidence |
|---|---|---|---|
| fn_8019674C | 0x8019674C | CObjUpdateFunc | byte-identical melee copy/paste bug (cases 5/6/7 all write `.x`) |
| fn_80193D30 | 0x80193D30 | CObjRelease | line-for-line melee match; WObjUnref x2 + MtxFree + parent destroy chain |
| fn_801A6A34 | 0x801A6A34 | MObjInfoInit | vtable-slot stores at hsdMObj+0x30..0x50 pin 6 sibling fns in one shot |
| fn_801A7128 | 0x801A7128 | MObjMakeTExp | largest fn in unit (2556 B) + `make_texp` slot store |
| fn_801BBAC8 | 0x801BBAC8 | TObjInfoInit | hsdInitClassInfo("hsd_tobj") + release/amnesia slot offsets vs hsd_class.h |
| fn_8019E460 | 0x8019E460 | resolveIKJoint1 | unmistakable ~3KB two-bone IK solver, matches melee jobj.c |
| fn_801B8024 | 0x801B8024 | HSD_TExpMakeDag | exact HSD_ASSERT(0xEE) + identical two-pass DAG build vs texpdag.c |
| fn_80199264 | 0x80199264 | HSD_DObjRemoveAll | melee-exact walk; name already declared/called unresolved in hsd_jobj.c/hsd_dobj.h |
| fn_801B16C0 | 0x801B16C0 | HSD_ShadowInit | exact GXSetTexCopySrc/Dst literal-argument fingerprint |
| fn_801AA35C | 0x801AA35C | HSD_ObjAllocInit | melee objalloc.c line-for-line: assert, memset(data,0,0x2C), align math |

## Key findings / caveats for whoever applies these

1. **Unit boundaries are mislabeled in splits.txt.** `hsd_range_801BBAC8.c`
   is melee **tobj.c** (+ all 3 functions of util.c at 0x801BF098–0x801BF1F0),
   not aobj/texp. `hsd_range_801A69C0.c` is melee **mobj.c**. The
   0x801AE008–0x801B009C "pobj" ranges are actually Colosseum's **RObj**
   class (rodata strings "robj: alloc" etc.). Genuine aobj.c content only
   starts at ~0x801C0270. 0x801BF1F0–0x801C021C is Dolphin **VI driver**
   code, not HSD at all (propagation-heuristic bleed).
2. **Aspirational placeholders in src/hsd/*.c are NOT ground truth.** Many
   files contain melee-pasted function bodies under real melee names
   (`CObjRelease`, `LObjRelease`, `JObjAmnesia`, `_Early` accessors, most of
   hsd_texp.c, hsd_shadow.c) that are NOT linked at any address. Several
   proposals (7 jobj B-tier) reuse names currently squatted by such
   placeholders in the same file — the placeholders must be reconciled/removed
   before applying, or a duplicate-identifier compile error results.
3. **Stale header hints found:** hsd_dobj.c claims 0x80198F7C=HSD_DObjInit and
   hsd_fog.c claims 0x8019B7C0=HSD_FogAdjInit (both unverifiable, bodies
   absent); hsd_initialize.c claims 0x8019C690=HSD_InitAssert1 but
   src/game/gs_gfx.c evidence says it's a 2-arg VI black-screen control fn.
4. **Pre-existing anchor likely misnamed:** `HSD_LObjSetup` (0x801A4F54) is by
   signature melee's `HSD_LObjSetupInit`; the true 3-param `HSD_LObjSetup` is
   the unnamed `fn_801A6098`. Left alone (renaming anchors out of scope),
   flagged for review.
5. Filename hints on the tiny robj units were close but imprecise: melee's
   actual names are `HSD_RObjGetByType` (not FindByType) and
   `HSD_RObjGetAllocData`/`HSD_RvalueObjGetAllocData` (not "...2").
6. ~370 unnamed fns were in scope; 139 proposed. The remainder are: asm-only
   ranges with no decompiled bodies to compare (hsd_range_801920E4,
   hsd_pobj_range_801AE008/801AEBE4, most of hsd_shadow, half of jobj range),
   scaffold-stub bodies inconsistent with their declared sizes (most of
   hsd_texp.c), Colosseum-specific code with no melee counterpart (mem-callback
   plumbing in hsd_initialize.c, viewport-callback indirection in cobj, GSmem
   texture-cache glue), or genuinely ambiguous candidates (skipped as C).

## Files

- Proposals: `/Users/douglaswhittingham/.claude/jobs/76409c12/tmp/naming/hsd_proposals.json`
- Per-unit partials: `partial_*.json` (same dir)
- Melee reference cache: `melee_src/` (same dir)
- Merge/validation script: `merge.py` (same dir; re-runnable)
