<!-- WALLS.md — the genuine-wall ledger. Maintained by hand; read by humans + agents. -->
# Decompilation Walls Ledger

A **wall** is a function whose residual mismatch is **not controllable from C** — no
declaration order, cast, pragma, compiler version, or expression rewrite reachable
from our toolchain closes it. Mature matching-decomp projects (TWW, Pikmin2, Prime,
Melee) do not crack these; they **wrap them as asm or ship them as "Equivalent"
C and move on.** This file is how we *stop re-grinding* them.

**Policy**
1. A residual is logged here **only after** it is confirmed C-uncontrollable — i.e. it
   falls into one of the wall **classes** below, or has been explicitly tested
   (allocator-inversion / version-sweep / pragma-sweep) and did not move.
2. Each confirmed wall is logged **once**, by class, then **hard-skipped** — do not
   spend agent budget re-attacking a logged wall unless a *new lever* appears
   (e.g. the `-use_lmw_stmw` flag sweep, task #19, may dissolve the **stmw** class).
3. A wall function that already has **real, correct C** is also an **Equivalent**
   function — add it to `tools/decomp_work/equivalent.txt` so it counts toward the
   C-converted axis and is excluded from byte-verify nagging. A wall with no good C
   stays an asm-wrapper (ROM-only).

**The two-axis model this serves** — see `tools/decomp_work/progress2.py --measure`:
- **C-converted %** (decompilation headline) = real C written, byte-match or not.
- **ROM-reproducible %** (the old objdiff number) = main.dol byte-identical
  (real-C-match **+ asm/split wrappers** — the wrappers inflate it).
- **Byte-exact-C %** (strict) = real C that byte-matches.
Walls live in the gap between C-converted and byte-exact-C. Logging them keeps the
gap *honest and intentional* instead of an open backlog we keep re-attacking.

<!-- SNAPSHOT:auto — regenerate with: python tools/decomp_work/progress2.py --measure -->
## Snapshot — 2026-05-31 (regenerate: `python tools/decomp_work/progress2.py --measure`)

Fresh clean `ninja` base build; denominator = objdiff `fn_` universe (8287),
the same one `tools/progress.py` reports.

| Axis | Count | % | What it means |
|---|---|---|---|
| **1. C-converted** (headline) | 6372 / 8287 | **76.89%** | real C written, byte-match or not |
| **2. ROM-reproducible** (official) | 4598 / 8287 | **55.48%** | main.dol byte-identical (real-C-match **+ 1227 asm/split wrappers**) |
| **3. Byte-exact C** (strict) | 3329 / 8287 | **40.17%** | real C that actually byte-matches |

- **Equivalent pool** (real C, not byte-exact) = **3043** functions — the gap between
  axis 1 and axis 3. Confirmed walls go in `tools/decomp_work/equivalent.txt` (4 so far).
- **ROM-only inflation** = **1227** asm/split wrappers counted in axis 2 but not decompiled
  (the gap between axis 2 and axis 3). The headline "55%" is mostly these + the byte-exact C.
- Source-scan total (all `.c`, incl. named library fns not in the `fn_` universe):
  **6897 REAL_C / 815 ASM-wrapper / 580 STUB** of 8292 → **83.18%** C-converted across
  *all* source functions. (The 76.89% above is the subset visible to objdiff's `fn_` set.)

**Takeaway:** we have **decompiled ~77–83%** to readable C; the ROM rebuilds byte-identical
at **55.5%** (inflated by ~1,200 wrappers); only **~40%** is strictly byte-exact C. Peers
headline the first number. The ~3,000-function Equivalent pool is where the remaining
byte-exact work — and the genuine walls below — live.

> Scanner caveat: the active-branch classifier is a heuristic (handles the
> `#if 0 asm / #else C` + Ghidra brace-on-next-line patterns). Axis 2 is exact
> (objdiff-counted); axes 1/3 carry small classification noise on unusual files.

---

## Wall classes (the reusable taxonomy)

Each class is a codegen behaviour we have proven the C cannot steer. Recognise the
class from the objdiff residual, log the function, skip it.

### W1 — Data-flow-locked register allocation
The survivor-register coloring is fixed by **data-flow shape**, not by statement
order, so the allocator-inversion lever (reorder first-definitions to match the
target's ascending saved-reg band — see `tools/decomp_work/ra/ALLOCATOR_MODEL.md`)
**cannot** reach it. Locked when the cycled values are **parameter-derived AND
loop-carried**, or when one value has the **longest forward reach** and CW lifts it
to r31 regardless of where you define it. Inversion cracks only *independent* call-
result survivors (e.g. fn_8021A984, which we won); it fails on these.
- *Failed levers:* first-def reorder, decl reorder, type changes, version sweep.
- *Triage:* `walltriage.py` reg_remap shows a pure register *permutation* among
  values that are param-fed or live across a back-edge. → log W1.

### W2 — Instruction-scheduling residual
Same registers, **reordered instructions**. The source declaration order drives
*both* the register map *and* the load/issue order, and the target wants the opposite
issue order for the same coloring — so any reorder that fixes the schedule breaks the
coloring and vice-versa. `#pragma scheduling on/off` flips the whole TU, not the one
spot. Not C-controllable.
- *Failed levers:* scheduling pragma (whole-file only), peephole pragma, reorder.
- *Triage:* residual instructions are a *permutation of identical opcodes/operands*
  (no register or immediate differs). → log W2.

### W3 — stmw / lmw threshold mismatch  *(flag lever TESTED — does NOT dissolve it)*
With `-use_lmw_stmw on` (our current flag) CW emits `stmw/lmw` only when the saved
band is **≥ 5** registers; for ≤ 4 it emits individual `stw/lwz`. When the target
saved 2–4 regs but our C forces a ≥5 band (or vice-versa) the whole prologue/epilogue
diverges. Band size = number of values live across a call, only weakly C-steerable.
- *`-use_lmw_stmw off` TESTED (2026-05-31, `tools/decomp_work/flag_sweep.py`) — REJECTED,
  net-DESTRUCTIVE.* colosseum_battle.c: **0 new 100%** (199→199), 10 improved / 27 regressed.
  colosseum_event.c: **LOST 10 matches** (227→**217**), 8 improved / 31 regressed.
  Reasons: (a) the flag is file-GLOBAL — it strips stmw from the *many* functions that
  already match (target uses stmw) to chase the *few* with a threshold mismatch; (b) it's
  the **wrong direction** — the target was built with a compiler whose stmw threshold is
  ~2-3 saved regs, so it wants *more* stmw than CW 1.3 emits (≥5), and `off` removes stmw
  entirely. No CW flag *lowers* the threshold. `-inline {off,noauto,deferred,auto}` and
  `-O3,p/-O4,s` were inert. **W3 is a genuine wall, not a flag artifact.** Do not re-test.
- *Triage:* residual concentrated in `stmw/lmw` vs `stw…/lwz…` in prologue/epilogue.

### W4 — carry-vs-sign boolean materialize
Lowering of `(x != 0)` / boolean-of-int: target uses the carry idiom
(`subic.`/`subfe`) where CW 1.3 emits the sign idiom (`neg`/`or`/`srwi`) — or the
reverse. This is a **TU-wide compiler-version idiom**, not selectable per-expression.
- *Failed levers:* every C spelling of the boolean, version sweep within our set.
- *Triage:* residual is a fixed `subic/subfe` ↔ `neg/or/srwi` swap around a compare.

### W5 — commutative fresh-register add
`saved_var OP call()` forces CW to put the call result in a **fresh** register, but
the target reuses the saved var's register (in-place accumulate). **Winnable only**
when the saved var is genuinely live afterward — then `x += fn()` matches. When the
saved var is consumed (dead after), no spelling reproduces the reuse.
- *Related winnable cousin:* a `crset cr1eq` vs `crclr` before a varargs call means a
  dropped **float** vararg — that one is fixable (restore the arg). Don't confuse it
  with W5. See `feedback_crset_dropped_float_vararg`.
- *Triage:* residual is which register holds a commutative add operand, saved var dead.

### W6 — relocation / jumptable / call-target NAME mismatch
Instructions are **byte-identical**; the only diff is a jumptable, relocation, or
**`bl` call-target** **symbol name** (e.g. `jumptable_803634A8` vs CW's anonymous label,
or `bl fn_80053110` vs `bl dbgMenuFightWazaEditSub` — same address, different name).
Not reachable from C — the fix is objdiff per-unit `symbol_mappings` (a config feature)
or symbols/splits edits, which are **forbidden-edit** truth files here.
- *Failed levers:* none from C; this is a scoring artifact, not a codegen miss.
- *Triage:* objdiff shows 100%-identical opcodes (or same-address call targets),
  mismatch flagged only on a reloc/call target name. Candidate for the `symbol_mappings`
  recovery pass. Note: `progress.py` / `measure_cache` invoke `objdiff-cli diff` without
  the `-p` project flag, so `symbol_mappings` only affect per-unit diffs in `objdiff.json`
  and the decomp.dev interactive GUI, not per-function `compile_check.py` diffs.

---

## Confirmed individual walls

match% is the authoritative objdiff value (compiled with the real ninja `GC/1.3`
flags — **not** `compile_check -s`, which uses wrong fallback flags and under-reports).
All four below have correct C and are in `tools/decomp_work/equivalent.txt`.

| Function | File | Class | match% | Equivalent? | Evidence |
|---|---|---|---|---|---|
| `fn_80038380` | scene_init.c | W2 instruction scheduling (stb/cmpwi reorder) | 89.88% | no (C active) | 4 real mismatches: instruction scheduling moves `stb` earlier and `cmpwi` later; 8 name-only symbol diffs. Sibling of fn_8003907C (identical pattern). |
| `fn_8003907C` | scene_init.c | W2 instruction scheduling (stb/cmpwi reorder) | 89.88% | no (C active) | Same W2 pattern as fn_80038380. |
| `fn_80039004` | scene_init.c | W2 `extsb.` vs `extsb`+`cmpwi` codegen idiom + W1 regalloc (`lfsx` indexed vs `lfs` offset, `mulli r6` vs `mulli r0`) | 89.17% | no (C active) | `extsb.` combines sign-extend+condition test — CW emits separate `extsb`+`cmpwi`. `lfsx f0,r5,r0` vs `lfs f0,0(r5)` is regalloc-driven addressing mode. |
| `fn_801A4A54` | hsd_lobj.c | W2 branch inversion (`ble` vs `bgt`) + early-return emission (`li r3,0; blr` vs fall-through) | 88.21% | no (C active) | Target inverts branch condition and emits early-return block; CW uses opposite branch direction. |
| `fn_800E3604` | gs_material.c | W1 data-flow-locked reg-alloc (live-range coalescing) | 99.37% | yes | inversion-tested 7 experiments, reg assignment invariant to source order (slotMatch is param-derived + loop-carried → lifted to r31 by data-flow). `feedback_objdiff_orientation_and_mwcc_harness` |
| `fn_80216650` | colosseum_script.c | W1 data-flow-locked reg-alloc | 99.14% | yes | inversion-tested, locked (`wf_invert`) |
| `fn_8022E6F0` | colosseum_script.c | W1 data-flow-locked reg-alloc | 99.50% | yes | inversion-tested, locked (`wf_invert`) |
| `fn_8023CE60` | colosseum_script.c | W1 param landed one saved-reg off (data-flow-locked) | 98.26% | yes | inversion-tested, locked (`wf_invert`) |
| `fn_8019C128` | hsd_fog.c | W1 data-flow-locked reg-alloc (in CW's `%101` division-strength-reduction: magic const lands in r0 not r6) | 99.71% | no (asm-wrapper; C staged) | permuter best score 10 (~thousands of iters); 12 source forms swept (modulo local, inline, comma-op `(0,expr)`, hand u64 magic) all emit `mulhwu r6,r0,r4`; r0-vs-r6 is internal to the division-lowering pass |
| `fn_80191788` | hsd_wobj.c | W2 instruction-codegen residual (negated null-check tail-merge: target `bne .L; blr` vs CW's merged `beqlr`) | ~94% | no (asm-wrapper; C staged) | body byte-EXACT via struct-copy `*(V3*)&dst=*(V3*)src`; permuter 840→300, never broke the merge; goto/flat-return/`&&`/peephole-off all stay merged |
| `fn_8011BA0C` | gs_field_world.c | W6 relocation/jumptable NAME mismatch (external `jumptable_8035C260` unreproducible standalone) + range-check reg/prologue | ~91% | no (asm-wrapper; C staged) | dense `switch` (12 cases) matches; isolated compile emits internal `@18` label not the named external jumptable; permuter best 380, harness-unwinnable |
| `fn_800A43D4` | DVDLow.c | W1 reg-alloc + frame/prologue (target's late-`stwu` 8-byte frame + `r0`-reuse for constants vs CW's `stwu`-first 16-byte frame, base reg r9 vs r10) + dependency on asm-wrapper `fn_800A46EC` (its `void(void)` sig blocks a clean prototyped `bl`) | 78% | no (asm-wrapper; C correct) | body logic byte-correct (`clrrwi.`+branches via volatile WSL read, direct `bl` via block-scope shadow decl); residual is pervasive reg-assignment + frame-layout, not C-controllable. Candidate's "100%" was a standalone hallucination. GC/1.2.5n. Revisit only after `fn_800A46EC` is decompiled with its real prototype. |
| `fn_800AB4FC` | VIFull.c | W1 `addi rD,rS,0` param-move idiom + `stw-lr`-pre-`stwu` spill-interleaved prologue (same class as OSMemory, `feedback_osmemory_msgqueue_addi_move_wall`) | 83% (GC/1.3, exact size 184) | no (asm-wrapper; C correct) | SI-poll loop logic byte-correct; target emits `addi r30,r3,0`/`addi r3,r29,0` where CW emits `mr`, plus a late-`stwu` 0x28 frame vs CW's `stwu`-first 0x20 — both reg-alloc-internal. GC/1.2.5n gives 88% but at WRONG size 200 (the brief's "100% at 1.2.5n" was a standalone hallucination). All CW versions swept on this idiom for OSMemory; hard wall. |
| `fn_801327E0` | effect_util.c | float-conversion-in-TU (CW's auto `(f32)(s32)` int→float emits a per-TU **anonymous** 2^52 magic-double pool constant; target references the shared **named** `lbl_8047D0E0(r2)` sdata2 global) | 57% (GC/1.3, exact size 84) | no (asm-wrapper; C correct) | `*(f32*)(p+0xC)=*(f32*)(p+4)` + `field10 += field64*(f32)((u8)p[0x23]+(s8)p[0x42])` is logically byte-correct; auto-conversion's magic-build matches but its constant is anonymous (objdiff name-mismatch under `calculatePoolRelocations=false`) + scheduling of the magic-build differs. Manual union conversion via `lbl_8047D0E0` made it WORSE (50%, size 88). The brief's flagged "float-conversion fails in-TU" class, confirmed. |
| `fn_801CA728` | battle_scene.c | float-conversion-in-TU — SAME class as `fn_801327E0` (anonymous 2^52 magic-double pool constant vs target's shared named `(r2)` sdata2 global) | 93.79% (GC/1.3, exact size 132) | no (asm-wrapper; C correct) | `base + (s32)((f32)param * fn_8025D0A8())` with the int→float + `fctiwz` is byte-identical EXCEPT the single `lfd f2` magic-constant reloc (mine anonymous `@263`, target named `-21888(r2)`); diff persists even with `calculatePoolRelocations=true` (genuine, not a pool-scoring artifact). Manual union conversion via `lbl_8047D0E0` made it WORSE (88%, size 136). Block-scope shadow decls (`s32 fn_8006ADEC()`, `f32 fn_8025D0A8()`) resolve the void-return caller-decl conflicts cleanly. |
| `fn_800F7DE4` / `fn_800F7E40` / `fn_800F7E9C` (family) | input.c | W1 reg-alloc: **`lwzu`-in-place-increment vs separate-sentinel-register are mutually exclusive in CW 1.3.** The 4-way pad-by-id lookup (`lbl_80401C10`, stride 0x6c) needs the early-return-of-pointer form to emit the target's `lwzu r0,0x6c(r5)` in-place walk + un-merged `bne next; b end` (achieved via a `static inline` helper that returns `pad` directly). But that form makes CW fold the not-found sentinel to `li r5,0` at the miss, where the **target keeps `li r6,0` hoisted early + `mr r5,r6`**. Any form that keeps the sentinel in a separate live register (result-var, comma-`else if`) instead emits absolute-offset `lwz`+`addi` (loses `lwzu`). | ~92% (objdiff reports **0.0%** — alignment artifact: the single early `li r6,0` displaces the LCS) | no (asm-active; correct C staged + shared `PADInput_FindPad` helper) | swept ≥10 source forms: early-return / goto-hit / result-var+goto / nul-as-param-with-second-use / `static const` sentinel / named local / peephole off (breaks `lwzu`) / opt-level 0–4. `lwzu`+folded-`li r5,0` is locked; separate-`r6` only with absolute loads. Sentinel hoist-vs-fold is internal to the allocator. The float-storing cousins `fn_800F7C8C`/`fn_800F7D38` add the `fn_801327E0` float-conversion wall on top. |

---

## How to use this ledger
- **Before** attacking a near-miss: check it isn't already logged here. If it is, skip.
- **After** confirming a new wall (class match, or a tested-and-didn't-move residual):
  add a row above, and if it has good C, add the name to
  `tools/decomp_work/equivalent.txt`.
- **When a new lever lands** (e.g. `-use_lmw_stmw off` clears W3): re-open *that class*
  only, re-test its logged members, and graduate any that now match.