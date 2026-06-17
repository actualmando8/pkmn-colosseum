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

## 2026-06-13 — two DVDLow/hsd_mobj walls (un-wrapped from codex's undocumented asm-flips)

Codex commit `2708d871` flipped four functions `#if 0`→`#if 1` (asm-wrapper) without proof or logging. Validation un-wrapped the two that had real C; both were then exhaustively tested and confirmed **genuine C-uncontrollable walls** (now Equivalents in `equivalent.txt`):

- **`fn_800A4C94`** (DVDLow.c, DVDCBCallback read-and-clear getter) — **W-operand-order**, 71.4%. Target's `0xCC006004 = 0` store emits `lis r3`(addr) **before** `li r0`(value); CW emits value-first. Proven invariant across all 7 CW versions (1.1–2.0) and ~20 C phrasings (raw cast, index, pointer var, struct field, comma, register var, reordering). Operand-order-to-fresh-register is not C-controllable.
- **`fn_801A6DDC`** (hsd_mobj.c, HSD_MObj alpha-setter) — **W-peephole branch-merge**, 77.8% (was 20%; fixed via `#pragma optimization_level 1` + guard-clause — struct offsets were already correct at 0xC/0xC). Residual: CW fuses the inner null-check `bne L; blr` into `beqlr`; the target keeps it un-merged. ~30 source structures tested across 4 CW versions — every clean variant collapses to `beqlr`; the only way to force the split injects an extra non-matching instruction (scores lower).

The other two flipped functions (`fn_801A4098`, `fn_801A4A54`) were already logged HSD walls. Takeaway: codex's *selection* of which functions to wrap was sound, but a wall **with good C is an Equivalent** (real C active), not an asm-wrapper — see policy point 3 above.

## 2026-06-16 — gs_render.c packet wrk2 (1 crack + 3 walls; + a harness fix)

Diff-triage + grind of four gs_render.c near-misses (tag `wrk2`):

- **`fn_800DCD98` — CRACKED → 100.00% (SAVED, integration-verified).** Was logged in-source
  as `WALL 95.6% bne vs beq+b`. The one-case-`switch` lever (`feedback_switch_one_case_beq_b`)
  converts CW's fused `bne skip` into the target's un-fused `beq L; b end` — `if (r0==0){...}`
  → `switch(r0){case 0: ...; break;}`. **This invalidates the prior wall note**; branch-fusion
  near-misses where the if-body is a *statement block* are now winnable with this lever.
- **`fn_800DAF60` — WALL, 98.53%** (W2 coupled loop-codegen). mtctr/bdnz free-slot scan; residual
  is the entry `ble`-vs-`beq` guard **plus** an inner branch-fusion (`bne tail; b found` vs fused
  `beq`). The switch lever is **inert here** (the case body is a bare in-loop `goto`, which CW
  re-collapses, and it forces a signed `cmpwi`); `(s32)count>0` regresses the guard. Confirms the
  prior agent's verdict + the lever boundary. → `equivalent.txt`.
- **`fn_800E0790` — WALL, 95.24%** (W2 prologue-scheduling). Inline-`__asm` GQR-register-setup
  function with two C-level `bl` calls, so CW auto-emits a prologue LR-save `stw r0,0x14(r1)`. The
  manual asm-block store is then a duplicate (95.24%); removing it lands the auto-save before the
  first asm instr (90.48%). The target interleaves `li r3,0x4` ahead of the save — unreachable
  without transcribing the whole thing as naked asm (not decompilation). Stays the inline-asm hybrid.
- **`fn_800DE128` — WALL (not a real target), 99.65%.** Active asm-wrapper (`#if 1` + `.inc`; the
  `#else` is a `/* TODO */` stub — no real C). Residual is a lost-`@sda21` reloc baked into the
  `.inc` as `subi r20, r2, 0x6c00` (vs `li r20, lbl_8047CAA0@sda21`) — the **W-SDA-WRAPPER** class.
  Nothing to grind; `sections` should have auto-skipped it (queue contamination the packet warned of).

**Harness fix (`cs_splice.py` `_active_lines`).** The save/integrate extractor couldn't disambiguate
the still-wrapped `#if 0 asm #else realC #endif` twin in this file: the whole TU is inside
`#ifdef PCPORT … #else <real game C> #endif`, and the tracker assumed every non-`#if 0` guard is
live — so it flipped the `#else` (real C) to *dead* and the live-branch filter returned nothing.
`PCPORT` is never defined in the matching build, so `#ifdef PCPORT` is now treated as dead (its
`#else` live) and `#ifndef PCPORT` as live. Without this, **every** still-wrapped near-miss in a
PCPORT-guarded TU silently fails to save/integrate. Fix verified: `fn_800DCD98` saved and re-held
100.00% through `band_integrate` (dry-run).

## 2026-06-16 — three gs_title.c "twin" position-writers (W6 conversion-literal reloc)

`fn_80024DBC` / `fn_80024F2C` / `fn_8002509C` (gs_title.c, the fade-aware ±x/±y position
writers — siblings of the already-known `fn_8002520C`) all sit at **98.75%** with an
identical two-part residual, confirmed C-uncontrollable (packet `glm1`):

1. **W6 — anonymous int→double conversion-literal reloc (the hard cap).** Each function
   does two `(f32)(s32)(s16)` conversions; CW's native conversion is **instruction-perfect**
   vs target but loads its 2^52 bias via a per-TU **anonymous** pool literal (`lfd f3, @257@sda21`)
   where the target references the shared **named** global `lfd f3, lbl_8047B8B8@sda21`. Same
   class as `fn_801327E0` (effect_util.c) and `fn_80005E00` (main.c). Manual int→double via the
   named extern `lbl_8047B8B8` was tested and **regressed to 89.50%** (emits `fsub`+`frsp` with
   extra stack traffic instead of the fused `fsubs`) — independently reproducing the
   "manual union makes it worse" result already logged for `fn_801327E0`. No C form yields
   both the perfect instruction sequence **and** the named symbol.
2. **W2 — branch-fusion shape (secondary, 1 instr).** The list-walk index match emits a fused
   `beq LAB` where target keeps the un-fused `bne incr; b LAB`. Tested an `if(!=){++}else{goto}`
   inversion — CW canonicalizes straight back to the fused `beq`, no movement. Moot anyway,
   since (1) caps below 100% regardless.

All three have real, correct, active C → added to `equivalent.txt`. Hard-skip.

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

### W3 — stmw / lmw threshold mismatch  *(**PARTIALLY DISSOLVED 2026-06-10: per-TU `-O4,s`**)*
> **UPDATE 2026-06-10:** the pokemon.c campaign discovered the target's stmw-at-2-3-regs idiom
> comes from `-O4,s` (size), not our default `-O4,p`. The fix is a PER-TU override in
> compile_config.json, NOT the file-global -use_lmw_stmw flag tested below. Swept across 10
> W3-heavy TUs: **KEPT on trainer.c (+8 byte-exact), colosseum_event.c (+9), battle_main.c (+6),
> colosseum_script.c (+1) — +24 fns, 0 lost-100s** (commits bb015a65 369089e0 e70d8996 4d496c44).
> REJECTED on colosseum_battle/battle_waza/gs_title/gs_event_exec/menu_middle/battle_logic
> (lost-100s or no byte-exact gain — those TUs genuinely are `-O4,p`). W3 rows for the cracked
> fns (fn_801FE468, fn_801FA8CC, fn_80209380, fn_80202998, fn_802077D4, fn_80204854, fn_80214864
> tail, fn_801FA634, battle_main quintuplet+1) are SUPERSEDED — now 100%, parent-verified.
> Before filing any new W3: test the TU's flag first.

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
  `-O3,p/-O4,s` were inert. ~~**W3 is a genuine wall, not a flag artifact.** Do not re-test.~~
- **W3 DISSOLVED 2026-06-10 (pokemon.c campaign).** The 2026-05-31 sweep result was wrong:
  **`-O4,s` (optimize-for-size) emits `stmw` at 2-3 saved regs**, exactly the target idiom.
  Verified on a minimal repro (same fn: `,p` → `stw/stw`, `,s`/plain `-O4` → `stmw r30`,
  identical objects for `-O4` and `-O4,s`) AND TU-wide: switching `src/game/pokemon.c` to
  `-O4,s` in `config/GC6E01/compile_config.json` took 84→86 byte-exact, **22 improved,
  0 regressed**. Per-function form: `#pragma push` / `#pragma optimize_for_size on` /
  `#pragma pop` flips it under a `,p` TU. Compiler version is NOT the axis (1.1→2.7
  identical at `,p`). → Re-test colosseum_script/battle/event with a per-TU `-O4,s`
  sweep gated on 0 regressions.
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
| `fn_80029FAC` | gs_worldmap.c | W2 instruction scheduling (slwi vs early sda21 lwz issue order) | 97.01% | no (C active) | Gekko list-scheduler always issues the ready SDA load before the ready shift — exhaustively swept 2026-06-10 (volatile cast, subscript inversion, statement reorders, scheduling 601/603/604/750/off, peephole on) all inert or worse. Sibling `fn_8002A0B8` same wall. → equivalent.txt |
| `fn_80038380` | scene_init.c | W2 instruction scheduling (stb/cmpwi reorder) | 89.88% | no (C active) | 4 real mismatches: instruction scheduling moves `stb` earlier and `cmpwi` later; 8 name-only symbol diffs. Sibling of fn_8003907C (identical pattern). |
| `fn_8003907C` | scene_init.c | W2 instruction scheduling (stb/cmpwi reorder) | 89.88% | no (C active) | Same W2 pattern as fn_80038380. |
| `fn_80039004` | scene_init.c | W2 `extsb.` vs `extsb`+`cmpwi` codegen idiom + W1 regalloc (`lfsx` indexed vs `lfs` offset, `mulli r6` vs `mulli r0`) | 89.17% | no (C active) | `extsb.` combines sign-extend+condition test — CW emits separate `extsb`+`cmpwi`. `lfsx f0,r5,r0` vs `lfs f0,0(r5)` is regalloc-driven addressing mode. |
| `fn_801A4A54` | hsd_lobj.c | switch-dispatch register choice (`lwzx r0`/`mtctr r0` vs target's `r3`) + anonymous jumptable symbol (`@369` vs `jumptable_8036CA64`, W6-adjacent) | 99.29% (was 88.21 — dropped redundant range guard, commit e6016880) | no (C active) | 2026-06-10 parent-verified. 9 switch variants swept; default-first regressed to 98.75. Dispatch reg + jumptable name not C-controllable. |
| `fn_801A4098` | hsd_lobj.c | W1 reg-rename bijection — scratch reg in final vtable-dispatch block (target `r4`, ours `r3`; `param` genuinely dead there so CW reuses r3) | 98.75% | no (C active) | 2026-06-10: inline single-expression 94.38 ✗, hoist+reuse lbl pointer 74.38 ✗, pragma removal inert. |
| `fn_801A40F8` | hsd_lobj.c | structural — method dispatch off by +4 (target release@0x30/destroy@0x34 vs ours @0x34/@0x38) + `lbl_8036CA20` vs `&hsdLObj` dispatch base. **Header hypothesis TESTED AND REJECTED 2026-06-10**: u16 nb_exist/nb_peak/nb_alloc (Melee layout, head 0x2C→0x28) swept across all 22 HSD TUs → 10 fns REGRESSED from 100 (their lwz/stw counter accesses match target), only 2 marginal ups. Colosseum HSD genuinely uses u32 counters. | 93.28% | no (C active) | The +4 shift in THIS fn has a different cause (possibly a different vtable struct or an extra leading slot in just this dispatch path). Do NOT re-test the u16 header change. |
| `fn_801A620C` | hsd_lobj.c | W2 address-CSE — CW CSEs the `lis @ha` address reg for repeated non-sda2 absolute-label loads (one `lis`+`lfsu`) where target recomputes `lis` per comparison; `volatile` reloads the VALUE but cannot defeat address-register CSE | 91.97% | no (C active) | 2026-06-10: 4 variants (xyz-volatile 90.96, xy-only 90.96, non-volatile 89.21) all regressed. New W2 sub-class: address-CSE on absolute labels. |
| `fn_80194C2C` | hsd_cobj.c | **W-SDA-WRAPPER** — asm-wrapper residual is lost `lbl_80478AC8@ha/@l` reloc (`.inc` baked raw `lis 0x8048`/`lfs -30008`). C path reimplemented (epsilon-vec near-zero check) but `optimization_level 0` spills the 2 params live across the trailing `fn_800A3ADC` call to r31/r30 → 60% vs wrapper 99.2 (objdiff). | ~99.2 (wrapper) / 60 (C) | no (asm-wrapper, best avail) | 2026-06-10: volatile/const-f32/negated-or/opt0-4 all produce the same param-spill. Wrapper is best. See docs/decomp_notes/src_hsd_sda_wrapper_loss.md. |
| `fn_801A4344` | hsd_lobj.c | **W-SDA-WRAPPER** — wrapper residual = lost `lbl_8047DBC0/C4@sda21` assert-string relocs (`.inc` `la sym(r2)` resolves to offset 0 even with externs declared). C reconstruction (class-info list builder + bctrl @0x3c dispatch) hit only 55% — CW reorders the if/else and allocates r29/r30/r31 differently. | 96.8 (wrapper) / 55 (C) | no (asm-wrapper, best avail) | 2026-06-10: 5 C variants (scalar-char vs u8[] decls, condition inversion). Wrapper best. See decomp note. |
| W-SDA-WRAPPER (class) | hsd_cobj/dobj/mobj/fog/lobj | asm-wrapper near-miss whose ONLY residual is a lost `@sda21` (or `@ha/@l`) relocation baked as a raw `addi rN, r2/r13, imm` in the `.inc`. **Typed externs do NOT fix these** (unlike the symbolic-`bl` fn_801A0FBC case) — the inline assembler can't synthesize an SDA reloc from a raw imm, even with the label declared. C reimplementation regresses (functions also carry psq/cntlzw/fctiwz-magic/bctrl). FAST-SKIP. | — | — | Affects fn_80193D30 (94.1), fn_801A4440 (99.8), fn_801A4D20 (98.6), fn_8019BD18 (99.7), fn_8019BB78 (92.3), fn_801A7128 (99.9). Leave as wrappers. 2026-06-10. |
| `fn_800E8FA0` | gs_material.c | W-peephole: `bgtlr` (fused) vs target's `ble @L; blr` (unfused) on last conditional return before stores; CW fuses ALL conditional-return paths | 88.9% | yes | 2026-06-10: original `if (h > 0x1e0) return`, inverted `if (h <= 0x1e0) { stores }`, explicit `goto`, `#pragma peephole off` (70%) — all fuse or break. → equivalent.txt |
| `fn_80005E00` | main.c | W1 volatile-FPR coloring rotation (target cap=f0/conv-chain=f1/magic2=f2 vs ours chain=f0/magic2=f1/cap=f2 in the vol-ramp clamp) + W6-adjacent measurement artifact: CW's anonymous f64 int->float conversion literals (`@140`/`@142`) vs target's named `lbl_8047B6A8`/`lbl_8047B6B0` relocs — artifact reproduces on already-matched repo code (gs_pcbox fn_8001BAC4); cap ~99.87 even if the rotation cracked | 99.39% | yes | 2026-06-10 decomp-main-c workflow: TWO independent agents, ~32 materially different variants (ternary/arg-anchoring, comma-eval flips, static-helper inline proof, volatile pins, opt pragmas, decl orders). New mechanisms learned: CW volatile-FPR web numbering is position-independent; binary-op operand eval order is complexity-driven (comma-wrapping the left operand of `/` flips to left-first); divisor must stay an inline compiler temp to get vol=f30/den=f31. Working levers retained in C: right-operand-first `&` call eval, peephole off, flat mixer if/else. -> equivalent.txt |
| `fn_800E8EFC` | gs_material.c | W2 prologue instruction scheduling (`addi r0,r3,@l; mr r30,r0` vs target's direct `addi r30,r3,@l`); registers correct (r31=zero,r30=slot,r29=i) | 92.9% | yes | 2026-06-10: `u32 zero=0` (no change), `#pragma peephole off` (54.5%), `#pragma scheduling off` (46.9%). Prologue issue-order not C-controllable. → equivalent.txt |
| `fn_800E3604` | gs_material.c | W1 data-flow-locked reg-alloc (live-range coalescing) | 99.37% | yes | inversion-tested 7 experiments, reg assignment invariant to source order (slotMatch is param-derived + loop-carried → lifted to r31 by data-flow). `feedback_objdiff_orientation_and_mwcc_harness`. RE-CONFIRMED 2026-06-09 vs the block-scope lever (6 variants incl. full nested-class mirror — all kept or worsened it). |
| ~~`fn_80216650`~~ | colosseum_script.c | **CRACKED 2026-06-09** (was: W1 data-flow-locked) | **100%** | — | block-scope declaration restructuring (inner-block decls demote toward band bottom; reverse decl order within block) — commit a68f62be. New lever dissolves part of the W1 class; re-triage other W1 entries against it. |
| ~~`fn_8022E6F0`~~ | colosseum_script.c | **CRACKED 2026-06-09** (was: W1 data-flow-locked) | **100%** | — | same block-scope lever, declaration moves only — commit 7132a808 |
| `fn_8023CE60` | colosseum_script.c | W1 param landed one saved-reg off (data-flow-locked) | 98.26% | yes | inversion-tested, locked (`wf_invert`); RE-CONFIRMED 2026-06-09 vs the block-scope lever too (~12-shape sweep + minimal repro temp/probe1.c: li-0 accumulator pinned to r31 in every legal shape). Twins fn_8023CFDC/fn_8023D158 share the defect. |
| `fn_8019C128` | hsd_fog.c | W1 data-flow-locked reg-alloc (in CW's `%101` division-strength-reduction: magic const lands in r0 not r6) | 99.71% | no (asm-wrapper; C staged) | permuter best score 10 (~thousands of iters); 12 source forms swept (modulo local, inline, comma-op `(0,expr)`, hand u64 magic) all emit `mulhwu r6,r0,r4`; r0-vs-r6 is internal to the division-lowering pass |
| `fn_80191788` | hsd_wobj.c | W2 instruction-codegen residual (negated null-check tail-merge: target `bne .L; blr` vs CW's merged `beqlr`) | ~94% | no (asm-wrapper; C staged) | body byte-EXACT via struct-copy `*(V3*)&dst=*(V3*)src`; permuter 840→300, never broke the merge; goto/flat-return/`&&`/peephole-off all stay merged |
| `fn_8011BA0C` | gs_field_world.c | W6 relocation/jumptable NAME mismatch (external `jumptable_8035C260` unreproducible standalone) + range-check reg/prologue | ~91% | no (asm-wrapper; C staged) | dense `switch` (12 cases) matches; isolated compile emits internal `@18` label not the named external jumptable; permuter best 380, harness-unwinnable |
| `fn_80129280` | gs_field_world.c | W6 jumptable NAME only (`@6029` vs `jumptable_803634A8`) | 99.85% | no (C active) | ONLY residual is the compiler-local jumptable symbol; code + table shape match, bytes link identical. Annealer's isolated frame scores it 0 (banked as its "first win" 2026-06-09 — integration revealed the artifact). → equivalent.txt |
| `fn_80132A38` | effect_util.c | W6 jumptable NAME only (`@559` vs `jumptable_80363630`) | 99.92% | no (C active) | same class; the long-trapped effect_util near-miss, finally measurable after the cascade repair. Codex handoff 2026-06-09. → equivalent.txt |
| `fn_80065A48` | ui_core.c | W6 jumptable NAME only (`@2187` vs `jumptable_802EDB7C`) | 99.99% | no (C active) | 7.3KB 182-entry UI command dispatcher fully decompiled (msg-as-first-local lever fixed the whole register band; family pragma set opt4+sched+nopeephole). Only residual is the compiler-local jumptable symbol; bytes link identical. 2026-06-10. -> equivalent.txt |
| `fn_800A43D4` | DVDLow.c | W1 reg-alloc + frame/prologue (target's late-`stwu` 8-byte frame + `r0`-reuse for constants vs CW's `stwu`-first 16-byte frame, base reg r9 vs r10) + dependency on asm-wrapper `fn_800A46EC` (its `void(void)` sig blocks a clean prototyped `bl`) | 78% | no (asm-wrapper; C correct) | body logic byte-correct (`clrrwi.`+branches via volatile WSL read, direct `bl` via block-scope shadow decl); residual is pervasive reg-assignment + frame-layout, not C-controllable. Candidate's "100%" was a standalone hallucination. GC/1.2.5n. Revisit only after `fn_800A46EC` is decompiled with its real prototype. |
| `fn_800AB4FC` | VIFull.c | W1 `addi rD,rS,0` param-move idiom + `stw-lr`-pre-`stwu` spill-interleaved prologue (same class as OSMemory, `feedback_osmemory_msgqueue_addi_move_wall`) | 83% (GC/1.3, exact size 184) | no (asm-wrapper; C correct) | SI-poll loop logic byte-correct; target emits `addi r30,r3,0`/`addi r3,r29,0` where CW emits `mr`, plus a late-`stwu` 0x28 frame vs CW's `stwu`-first 0x20 — both reg-alloc-internal. GC/1.2.5n gives 88% but at WRONG size 200 (the brief's "100% at 1.2.5n" was a standalone hallucination). All CW versions swept on this idiom for OSMemory; hard wall. |
| `fn_801327E0` | effect_util.c | float-conversion-in-TU (CW's auto `(f32)(s32)` int→float emits a per-TU **anonymous** 2^52 magic-double pool constant; target references the shared **named** `lbl_8047D0E0(r2)` sdata2 global) | 57% (GC/1.3, exact size 84) | no (asm-wrapper; C correct) | `*(f32*)(p+0xC)=*(f32*)(p+4)` + `field10 += field64*(f32)((u8)p[0x23]+(s8)p[0x42])` is logically byte-correct; auto-conversion's magic-build matches but its constant is anonymous (objdiff name-mismatch under `calculatePoolRelocations=false`) + scheduling of the magic-build differs. Manual union conversion via `lbl_8047D0E0` made it WORSE (50%, size 88). The brief's flagged "float-conversion fails in-TU" class, confirmed. |
| `fn_801CA728` | battle_scene.c | float-conversion-in-TU — SAME class as `fn_801327E0` (anonymous 2^52 magic-double pool constant vs target's shared named `(r2)` sdata2 global) | 93.79% (GC/1.3, exact size 132) | no (asm-wrapper; C correct) | `base + (s32)((f32)param * fn_8025D0A8())` with the int→float + `fctiwz` is byte-identical EXCEPT the single `lfd f2` magic-constant reloc (mine anonymous `@263`, target named `-21888(r2)`); diff persists even with `calculatePoolRelocations=true` (genuine, not a pool-scoring artifact). Manual union conversion via `lbl_8047D0E0` made it WORSE (88%, size 136). Block-scope shadow decls (`s32 fn_8006ADEC()`, `f32 fn_8025D0A8()`) resolve the void-return caller-decl conflicts cleanly. |
| `fn_80162FB0` | people_field.c | float-conversion-in-TU, same anonymous conversion-literal class as `fn_801327E0`: readable C emits CW's auto u32-to-f32 magic double as local `@405`, while target references named `lbl_8047D4E8@sda21`; fpr names rotate around that constant | 78.3% by `match_scan_file.py` / 98.48% by single-symbol diff with pool relocation suppression | yes | 2026-06-16 Codex pass: active asm wrapper replaced with typed C using `PeopleFieldMoveScale->divisor`, no raw base+offset arithmetic. Manual named-bias union conversions via `lbl_8047D4E8` were materially worse (44.0% and 51.9% by `match_scan_file.py`) and introduced less-portable type-punning. Leave readable C active; do not re-grind without a new conversion-literal lever. -> equivalent.txt |
| `fn_800F7DE4` / `fn_800F7E40` / `fn_800F7E9C` (family) | input.c | W1 reg-alloc: **`lwzu`-in-place-increment vs separate-sentinel-register are mutually exclusive in CW 1.3.** The 4-way pad-by-id lookup (`lbl_80401C10`, stride 0x6c) needs the early-return-of-pointer form to emit the target's `lwzu r0,0x6c(r5)` in-place walk + un-merged `bne next; b end` (achieved via a `static inline` helper that returns `pad` directly). But that form makes CW fold the not-found sentinel to `li r5,0` at the miss, where the **target keeps `li r6,0` hoisted early + `mr r5,r6`**. Any form that keeps the sentinel in a separate live register (result-var, comma-`else if`) instead emits absolute-offset `lwz`+`addi` (loses `lwzu`). | ~92% (objdiff reports **0.0%** — alignment artifact: the single early `li r6,0` displaces the LCS) | no (asm-active; correct C staged + shared `PADInput_FindPad` helper) | swept ≥10 source forms: early-return / goto-hit / result-var+goto / nul-as-param-with-second-use / `static const` sentinel / named local / peephole off (breaks `lwzu`) / opt-level 0–4. `lwzu`+folded-`li r5,0` is locked; separate-`r6` only with absolute loads. Sentinel hoist-vs-fold is internal to the allocator. The float-storing cousins `fn_800F7C8C`/`fn_800F7D38` add the `fn_801327E0` float-conversion wall on top. |
| `fn_800EC4D0` | gs_material.c | W2 branch-reorder + REG-IMM offset swap (then/else block order in `if (flag) {out0=a0; out1=a0+cc} else {out0=a0+a0; out1=a0_const}`) | 84.19% (objdiff 68.97%) | no (C active) | Target emits then-block first (`lfs f0, 0xa0(r3)`), CW emits else-block first (`lfs f0, 0xcc(r3)`). The offset swap (`0xa0↔0xcc`) is a W2 structural reorder. Also has `clrlwi.` vs `clrlwi` idiom (dot-record in target, separate test in base). |
| `fn_800EC208` | gs_material.c | W2 branch inversion (`cmpwi r0, 0x1` first vs `cmpwi r0, 0x0` first in if/else chain) | 79.21% (objdiff 75.61%) | no (C active) | Target tests `mode==1` first with compound branch (`beq .L1; bge .Lelse; cmpwi 0x0`), CW tests `mode==0` first (`cmpwi 0x0; bne .Lcheck1; ...`). Not C-controllable: compiler chooses branch order. |
| `fn_8020B330` | colosseum_event.c | W1 saved-band tie-break (oracle PURE_RENAME r23↔r26, dist=1) **locked at opt4** — copy-propagation dissolves every artificial web edit | 99.70% | no (C active) | 2026-06-10: 7 variants ALL inert or worse — fn-scope decl swap, single-var inner block, both-vars inner block (both orders), dead init, u16→u32 retype (98.58 ✗), web-split tmp (99.51 ✗). The block-scope lever's proof commits ran with web splits that survive only at lower opt levels (b323f29b had `optimization_level 1`); at this TU's opt4 + `peephole on`, CW canonicalizes them away before allocation. uVar3/iVar7 swap is allocator-internal tie-break. |
| `fn_8020EED4` | colosseum_event.c | W1 saved-band tie-break (oracle PURE_RENAME r28↔r29 = p7-param vs uVar1) + clrlwi home-vs-arg materialization order | 99.53% | no (C active) | 2026-06-10: comma-op `(0,limit)` inert; param-split `p7c = p7` copy fully coalesced back at opt4 (diff byte-identical). Same opt4 copy-prop lock as fn_8020B330. |
| `fn_800FE38C` | gs_thread.c | W1 volatile tie-break (`bx` reuses ax's freed r3 vs target r5) + anonymous f64 pool constant `@652` vs named `lbl_8047CD50` (the 0x43300000… int→double bias; same class as fn_801327E0) | 99.82% (was 99.09 — clamp-cluster regs fixed by REVERSED decl order `cy2,cx2,cy1,cx1`, commit 49f0ac3e) | no (C active) | 2026-06-10 parent-verified. Operand swaps, in-place `+=`, decl reversal on ax/bx all inert or worse (98.84–99.69). NOTE: reversed-decl-order DID work here — TU is at pragma opt_level 2/3, confirming the lever's opt-level gate. |
| `fn_80214B68` | colosseum_script.c | W1 volatile 2-coloring tie-break (r4↔r5: lbl_8047B610 value vs lbl_80478D78+3 store-base; target gives the longer-lived value r4) | 98.95% | no (C active) | 2026-06-10: 8 variants (decl reorder, `1+pc`, pointer local, volatile store, `[3]` index, statement reorder 88.68 ✗, non-volatile read 88.68 ✗) all inert or worse. |
| `fn_801096AC` | gs_model.c | W1 FP tie-break (f0↔f3 on the two sda2 loads; SAME load order both sides, only assignment differs) | 98.0% | no (C active) | 2026-06-10: 7 variants (decl swap, inline read, eq-bool-first, block-scope demote, extern const f32, volatile read, delayed assignment) all copy-propped away at opt4. |
| `fn_8020341C` | colosseum_event.c | W1 saved-vs-temp tie-break (target homes null-guard ptr to r29/r28 immediately; CW keeps it in r3 then `mr`) | 99.62% | no (C active) | 2026-06-10 ANNEAL sweep: ternary, `!ccData`, peephole off, opt_propagation off all inert. Body byte-identical, 4-instr residual. |
| `fn_80209380` / `fn_80202998` / `fn_80209960` / `fn_801FE468` | colosseum_event.c ×3, trainer.c | W3 stmw threshold (body byte-identical; 2–3 saved regs → target stmw/lmw, CW individual stw/lwz) | 89.85 / 87.57 / 74.74 / 92.46 | no (C active) | 2026-06-10 ANNEAL sweep — joins the filed W3 class, no re-test. |
| `fn_802062A8` / `fn_80207C24` / `fn_80209484` / `fn_80209FAC` | colosseum_event.c | W2 scheduler prologue-slot interleave (target hoists free `li rN` call-arg constants into prologue store gaps; CW list-scheduler groups them at the call) | 80.67 / 77.78 / 77.78 / 76.0 | no (C active) | 2026-06-10: scheduling already on; not per-spot controllable. New W2 sub-pattern, common in this TU. |
| `fn_801F9034` | trainer.c | W4 bool-materialize (`return val==0` → CW `cntlzw;srwi` vs target `cntlzw;extrwi;cmplwi;bne` branch form) | 84.44% | no (C active) | 2026-06-10: ternary and if/else both peephole back to srwi. |
| `fn_8021F1CC` | colosseum_script.c | W2 switch-dispatch idiom (target's 3-branch `beq;bge;b` with redundant `bge`; CW 1-case switch gives `beq;b` only) | 96.88% (was 90.16, commit f33ddfbe) | no (C active) | 2026-06-10: one-case switch + inlined call-arg got +6.7; the redundant `bge` is dispatch-internal. |
| `fn_800F7068` | gs_thread.c | mtctr/bdnz selection wall (early `goto found` exit prevents CW choosing the CTR counter; target uses mtctr/bdnz, CW subic./bne) | 95.25% | no (C active) | 2026-06-10: every for-loop restructure regressed to ~75%. |
| `fn_8002AE9C` | gs_worldmap.c | W2 CSE/load-grouping at opt4 (target computes base+off as BOTH an lbzx index and a separate add; CW CSEs to one precomputed pointer) | 90.39% | no (C active) | 2026-06-10: store-sequence matched but not the CSE split; opt 1–4 sweep + array-restate all regressed. |
| `fn_80131F04` (+4 siblings fn_80131FF4/fn_8013208C/fn_80132124/fn_801321BC) | effect_util.c | peephole-off scratch-spill (split-decl reaches 97.24 but leaves a 2-instr r0-scratch hop unbreakable under the required peephole off) | 96.45 each | no (C active) | 2026-06-10: +0.79 best, under commit bar; reverted to baseline. |
| `fn_80133810` | effect_util.c | W4 bool-materialize (target `neg/or/srwi.` nonzero idiom vs CW `clrlwi/cmplwi`) | 92.84% | no (C active) | 2026-06-10: `!=0`, `&0xFF` variants identical. |
| `fn_80008868` | gs_task.c | W6 sda21 symbol naming (`lbl_80478838@sda21` vs raw `0x0(r13)`) | 98.54% | no (asm-wrapper active) | 2026-06-10: not C-addressable; #else C is an untested pseudo-register transliteration. |
| `fn_80040308` / `fn_8004B7EC` | scene_init.c | **W-SDA-RELOC-C** — target `li rX, lbl@sda21` vs CW `addi rX, r13/r2, 0`: when the symbol resolves to sda-base+0, CW emits the base-reg form with no reloc; not C-controllable | 99.93 / 99.68 | no (C active) | Phase V sweep 2026-06-10. New C-side cousin of W-SDA-WRAPPER. |
| `fn_80128E38` / `fn_8011F260` | gs_field_world.c | W-SDA-RELOC-C (4 sites in fn_8011F260) | 99.60 / 98.11 | no (C active) | same class, same sweep. |
| `fn_8004C4A4` / `fn_80038990` / `fn_80043728` | scene_init.c | W6 anonymous int→float bias constant (@NNN) + FP-reg tie-break (f29/f30 locked) | 99.85 / 98.93 / 98.27 | no (C active) | Phase V sweep 2026-06-10. |
| `fn_80053C00` / `fn_80129E20` | scene_init.c / gs_field_world.c | W1 mask-into-scratch reg tie-break (r0 vs r3) | 99.70 / 96.64 | no (C active) | resists all structural levers; Phase V sweep 2026-06-10. |
| `fn_80037180` / `fn_80117D14` | scene_init.c / gs_field_world.c | **branch-fusion wall** — CW collapses target's `bne body; b exit` two-branch form into single `beq exit` | 98.70 / 98.46 | no (C active) | peephole-off / switch / invert / return-early all failed. Phase V 2026-06-10. Note: the one-case-switch lever produces `beq;b` for FORWARD dispatch but cannot stop the fusion here. |
| `fn_8004B598` | scene_init.c | **aliasing-CSE wall** — target reloads `[0x44]` index after the aliasing `arr[idx]=` store; CW keeps it cached | 98.66% | no (C active) | volatile cast had no effect (it reloads the VALUE but the index web stays cached). Phase V 2026-06-10. |
| `fn_80057DE8` | scene_init.c | `(u8)x==0` lowering — CW fuses to `clrlwi.` record form vs target's separate `clrlwi`+`cmplwi` (inverse of the usual peephole-off case — here peephole off did NOT split it) | 95.23% | no (C active) | Phase V 2026-06-10. |
| `fn_8004DC18` | scene_init.c | W2 scheduler `li r0,0` prologue-slot hoist | 95.24% | no (C active) | known class. |
| `fn_8012F150` / `fn_801171C8` | gs_field_world.c | W1 FP-register permutation tie-break | 99.07 / 98.56 | no (C active) | Phase V 2026-06-10. |
| `fn_8012A1A4` | gs_field_world.c | W5 saved-first commutative OR canonicalization + scheduling | 94.88% | no (C active) | Phase V 2026-06-10. |
| `fn_8011CBC8` | gs_field_world.c | W1 register-routing tie-break (idx r3 vs r5 → clrlslwi vs slwi) | 93.64% | no (C active) | Phase V 2026-06-10. |
| fn_8011A280 quintuplet (+fn_8011A570/9EC/B50/FCC) + fn_8011B788/fn_8011A3E4 group | gs_field_world.c | base/idx register-coalescing + r3→r0→rN double-mr scheduling | 98.6 / 98.5 band | no (C active) | improved via authoritative-symbol renames; residual not C-controllable at opt4. Phase V 2026-06-10. |
| `fn_8021C0F4` / `fn_8021C308` / `fn_80212D6C` / `fn_8020DAD0` | colosseum_script.c ×3, colosseum_event.c | **DIV-IDIOM wall** (new class): CW 1.3 -O4 reciprocal-multiplies constant `/N`,`%N` and 3-instr `/2`; target uses real `divw`/`mullw` and 2-instr `srawi;addze` | 97.44 / 92.96 / 90.9 / 95.0 | no (C active) | Wave1-A 2026-06-10: not reachable at this opt level; cousin of the fn_8019C128 div-magic wall. |
| `fn_80222ADC` / `fn_80228DAC` / `fn_8020E7AC` / `fn_8020E95C` / `fn_80227C40` / `fn_8023F8C0` | colosseum_script/event | W1 saved-band tie-break (fn_8023F8C0 = void-pseudo-register; real signature regresses) | 99.63 / 99.18 / 96.44 / 89.6 / 88.3 / 88.4 | no (C active) | Wave1-A sweep 2026-06-10. |
| `fn_8020BFA0` / `fn_80211A78` / `fn_802077D4` | colosseum_event.c | W2 call-arg slot interleave / branch-shape / W3 stmw (body matches) | 95.15 / 98.45 / 87.9 | no (C active) | Wave1-A sweep 2026-06-10. |
| `fn_800E4170` | gs_material.c | W1 register-recycling (linear-scan slot reuse): 3rd section `lwz r30, 0x8(r30)` vs target `lwz r29, 0x8(r30)` — CW recycles freed entry-reg (r30) for dobj in section 3 since entry dies at that exact load; target's compiler did not recycle | 94.3% (8 instr off) | yes (C active, correct) | 2026-06-10: tested goto-restructure, inlined flags (no `flags0` var), outer-scope `void* r29` decl — all inert. CW linear-scan recycles the last-freed register (r30) for dobj3. Target preserved r30=entry+r29=dobj throughout — internal allocator decision, not C-controllable at opt4. |
| `fn_800218BC` | gs_title.c | W2 instruction scheduling (`lfs f31, lbl_8047B8A0` before `li r31, 0x0` at loop-init block `@L_80021948`; CW issues the SDA load first, target issues the zero-init first; same registers, same instructions, 1 swap pair = 2 mismatches) | 98.35% | yes (C active, correct) | 2026-06-10: the two instructions are data-independent; no C restructuring, pragma, or expression form can control SDA-load-before-zero-init vs zero-init-before-SDA-load issue order in CW's scheduler at opt4. → equivalent.txt |
| `fn_800E59C8` | gs_material.c | W2 store scheduling (`stb r3, 0x2→0x1→0x0` vs `0x0→0x1→0x2`) + `clrlwi.` vs `clrlwi` idiom | 80.88% (objdiff 76.09%) | no (C active) | Store order of `color[2]→[1]→[0]` vs `color[0]→[1]→[2]` is a compiler scheduling decision. The `clrlwi.` combining test+branch is a W2/W4 idiom. |
| `fn_800E5550` / `fn_800E638C` / `fn_800E65CC` / `fn_800E68D8` | gs_material.c | W1 data-flow-locked reg-alloc (`ptr` lands in r29 ours vs r31 target in first loop; CW first-def ascending assigns `ptr`=first-def → r29=lowest-open; target source declared idx/cnt/ptr order → ptr→r31) | 99.32% each (parent re-measured 2026-06-10; agent reported 89.8 from a stale mid-experiment state) | yes (C active, correct) | 2026-06-10: pointer-walk pattern `*ptr; idx++; ptr++` is correct C (improved from 88.1% in prior session); the 6-instruction residual is a pure register permutation in the first loop (ptr r29↔r31, idx r31↔r29) — allocator-internal, not C-controllable at opt4. → equivalent.txt |
| ~~`fn_800E4598`~~ | gs_material.c | **CRACKED 2026-06-10** (was: W1+W2 — a MISDIAGNOSIS: the C had dropped the inlined fn_8019D620 precondition idiom) | **100%** | — | restored 0x25d null-assert + `(flags & 0x2000000)` guard, then inner-block `f2` re-read split flipped the volatile coloring — commit 16ef3799. Parent-verified. |
| ~~`fn_800E5B68`~~ | gs_material.c | **CRACKED 2026-06-10** (was: W1+W2 "3-way rotation" — dissolved once access pattern was right) | **100%** | — | pointer-walk (`*arr; arr++`) instead of `arr[i]` + `i` declared first (→r31) — commit b678957d. Parent-verified. |
| ~~`fn_800E60F0`~~ | gs_material.c | **CRACKED 2026-06-10** (was: W1+W2+REG-IMM) | **100%** | — | direct call via local `extern` decl (cast-to-fnptr emitted lis/mtctr/bctrl), explicit if/else for `(x&4)?1:0` (ternary peepholes to `extrwi`), pointer-walk + flat decl order + late `r31 = 0` init — commit 21e4572c. Parent-verified. **LESSON: 77–84% "W1+W2" verdicts on C-active fns often hide WRONG C (dropped idioms, indirect calls, array-vs-pointer-walk) masquerading as reg permutation — exhaust structural shape before filing W1.** |
| `fn_800DF854` | gs_render.c | W2 register-coalescing + scheduling (3-instr prologue: target schedules `li r5,0` before `stw lbl_…` and materializes the store byte via `mr r4,r5` constant-coalesce from off's initial 0 vs our `li r4,0`) | 86.33% (was 81.67 — loop body now byte-exact, commit b2713fa9) | no (C active) | 2026-06-10: `(u16)handle` cast + `for(off=0,i=0;…;i++,off+=K)` pointer-arith cracked the loop. Residual inert vs block-scope, `val=off` chain/comma-init, peephole off, scheduling on, opt_propagation off. Sibling fn_800D7B80 has the IDENTICAL stuck residual. |
| `fn_800DD0B8` | gs_render.c | W2 register-coalescing + scheduling — same 3-instr prologue residual as fn_800DF854 | 85.36% (was 77.14, commit 7256a034) | no (C active) | same crack + same wall as fn_800DF854. Parent-verified 2026-06-10. |
| `fn_800FC244` | gs_thread.c | W2+type mismatch (void vs s32 return — target uses `beqlr` to return pointer when found, CW generates `bne+li r3,0+blr` for void) | 81.67% | no (C active, TODO annotated) | Target returns `(s32)p` on found-path via `beqlr`; current C declares `void` so CW returns early without value. Changing return type to `s32` may improve match but cannot verify due to compile_check.py failure on ASM-heavy files. **Potential fix: change `void fn_800FC244(u32*)` → `s32 fn_800FC244(u32*)` with `return (s32)p` on found and `return 0` on append.** |
| ~~people_field family + gs_task cluster (24 fns)~~ | people_field.c, gs_task.c | **CRACKED 2026-06-10** symbol-name reloc artifact (target reloc names from `symbols.build.txt`: `_GetInputValue`, `_dbgMenuFight*`, `waza*`, `Reverb*`, `snd*`, `sal*` etc. vs our `fn_<addr>`) | **100%** each | — | commits acc96b75 / 8ba96f85 / 9a7c7835. Byte-identical C; only the `bl/lis+addi` reloc symbol name differed. Renamed C identifiers (externs + calls + same-TU defs) to the symbols.build.txt names, each verified at its exact address. **LESSON: a 99.x C-active near-miss whose sole objdiff DIFF is a `bl <name>`/`lis name@ha` reloc is a NAMING artifact — rename the C identifier, do NOT file W1.** GOTCHA: stale K&R structural externs (`extern void Name();`) may already exist and collide on return type — delete the redundant K&R decl. |
| `fn_800FE4D4` | gs_thread.c | W1 f64-pool + FP tie-break — SAME class as filed `fn_800FE38C` (anonymous `@652@sda21` vs named `lbl_8047CD50` int→double bias) + f28/f29/f30/f31 assignment permutation on the four `fdivs`/`fmuls` temps | 98.0% | no (C active) | 2026-06-10: identical wall to fn_800FE38C; the pool constant is toolchain-locked and the FP reg band is allocator-internal. Skip. |
| `fn_800F7434` | gs_thread.c | reg-alloc r30↔r31 swap pervading the whole fn (target homes the va_list scratch ptr to r30 + the entry ptr to r31; CW gives the opposite) + `cmpwi 0x26`(signed, pre-mask) vs CW `cmplwi 0x26`(after clrlslwi) + `add r3,r30,r0;lwz 0x6c(r3)` pointer-form vs CW `addi;lwzx` | 95.2% | no (C active) | 2026-06-10: large varargs dispatch loop; the r30/r31 home assignment is the dominant residual and is allocator-internal at the function's `register void* listPtr` hint. Not C-flippable without regressing the body. |
| `fn_801FA634` | trainer.c | W4 bool-materialize (`subic./subfe` carry idiom vs CW `neg/or/srwi`) | 94.6% | no (C active) | 2026-06-10: filed W4 fast-skip class (`return x==0`/`!=0` lowering). |
| `fn_801FA8CC` / `fn_801F923C` | trainer.c | W3 stmw threshold (`stmw r28/r29` vs individual `stw`) + `fn_801F1460`→`fightActionInit` name (fn_801F923C also has W4 `neg/or/srwi`) | 91.3 / 86.7% | no (C active) | 2026-06-10: body diffs dominated by stmw prologue wall; the name reloc alone cannot reach 100 under the stmw wall. Filed W3. |
| trainer.c `fn_801F9130`/`fn_801F9600`/`fn_80201340`/`fn_801FBA24`/`fn_801FB974`/`fn_801FEC10`; gs_thread.c `fn_800F9AEC`/`fn_800F6BC4`/`fn_800F67C8`/`fn_800FE834`/`fn_800F694C`/`fn_800FA280` | trainer.c, gs_thread.c | structural multi-diff (9–29 mismatches each; no name component) — W2 scheduling / W3 stmw / reg-alloc permutation mix | 85.5–93.6% | no (C active) | 2026-06-10 breadth scan: all dominated by compiler-internal scheduling/reg-alloc, no single C-controllable lever surfaced. Deferred to per-fn structural decomp. |

---

## 75–85% range scan summary (2026-06-05)

All 122 functions in the 75–85% objdiff range (from `walltriage_out.json`) were classified:

| Category | Count | Wall class |
|---|---|---|
| C-ACTIVE (`#if 0` asm / `#else` active C) | 19 | W1/W2 (regalloc + structural scheduling) |
| PLAIN-C (Ghidra-import C stubs, no asm wrapper) | 102 | W1/W2 (colosseum_script.c bulk) |
| NOT_FOUND | 1 | — |

**Key finding:** Every single 75–85% C-ACTIVE target is blocked by W1 (data-flow-locked register allocation) and/or W2 (instruction scheduling). No new fixable patterns were found beyond those already logged.

**Exceptions worth noting:**
- `fn_800FC244` (81.67%) has a **type mismatch** (void vs s32) that is potentially fixable — the target uses `beqlr` to return the found-pointer, but the C declaration is `void`. Fixing the return type could improve match.
- `fn_800EBEEC` (80.46%, 80 mismatches) is the largest C-ACTIVE function with a `stmw r23→r22` register-band shift — pure W1.
- All colosseum_script.c functions (92 in this range) are PLAIN-C with heavy W1/W2 — no `#if 0` wrappers to toggle.

---

## 2026-06-10 — pokemon.c campaign walls (84→136+/185 byte-exact)

W3 DISSOLVED for this TU (see W3 section: `-O4,s`). Walls logged from the campaign:

| function | best% | class | residual |
|---|---|---|---|
| fightSideGetStatus | 99.82 | W6 | anonymous vs `jumptable_8037564C` (instructions byte-exact; in equivalent.txt) |
| fn_801F54A4 (PokemonGet) | 98.8 | W1+W6 | pkm/a16 r30↔r31 coloring pair (8+ shape variants swept) + jumptable name; real C in #else branch, wrapper stays |
| fn_801F2F3C / 3178 / 3074 / 32EC / 3430 | 97–99 | W1 | param-vs-loop-counter pair swaps (param gets high reg in ours, low in target) |
| fn_801F2020 / 221C / 2434 / 2350 / 1C18 / 1DBC | 86–99.7 | W1 | r29/r30/r31 pair/cycle swaps |
| fn_801F4220 / 4354 / 47B4 / 75F8 | 84–98.6 | new: **unfolded-branch** | target keeps `bne+b` / `blt+b` two-branch where CW folds to single inverted branch; goto/else-empty/one-case-switch/pragma all folded back |
| fn_801F7090 / 7174 / 6B54 / 6F38 / 6FD4 / 61EC | 90–98.9 | W1 | loop-var band shifts; 61EC: 15-reg fn puts params at TOP regs |
| fn_801F4718 / 4860 | 98.5 | new: **zero-init li-vs-mr** | target `li rA,0; mr rB,rA` (copy of zero) vs CW two `li`; `i = n` source form did not reproduce |

The W1 entries are candidates if a per-TU allocator lever ever lands; the
unfolded-branch and li-vs-mr classes look like a *different original compiler
build/patch-level* fingerprint — flag/version sweeps were inert.

## 2026-06-13 — colosseum_script.c ballistic batch (146→205 byte-exact, +59)

44-agent fleet via the `cs_band.py` private-scratch harness (each agent on its own
copy + object → no collision on the shared base; `cs_band.py save` only persists
true 100.00%; `cs_integrate.py` re-compiles + re-measures independently). 59 wins
integrated, 0 regressions. The near-misses below were pushed high but hit residual
walls — recorded for re-attack when a reg-coalescing/scheduler lever lands.

### HIGH-VALUE re-attack: the 0x8023Dxxx–0x8023Exxx family (×10) — crack once → +10
| fns | now | class | note |
|---|---|---|---|
| fn_8023D94C / DB98 / DDE4 / E030 / E27C / E4C8 / E714 / E960 / EBAC / EDF8 | 95.20 (was 49.22) | new: **deferred-write-through-r0** | 10 byte-identical fns. Each: result-commit deferred via `mr r0,r3; mr r3,rParam; mr r30,r0` at 4 call sites (target wants `mr r30,r3; mr r3,rParam`) + a count/base reg swap. CW -O4,s scheduler/allocator artifact; same source for all 10. |

### Confirmed reg-alloc / scheduler walls this batch (not C-controllable at -O4,s)
| fn | now | note |
|---|---|---|
| fn_80219838 | 99.73 | coalescer picks first-arg reg (target) vs last-arg reg (ours) for a value dying at a call; both args die simultaneously |
| fn_80238E30 | 99.73 | two near-identical blocks color shared vars to opposite non-volatile regs (per-block allocation) + inverted return branch |
| fn_8021847C | 99.71 | target allocates an 8th preserved reg (stmw r24); CW coalesces into 7 |
| fn_80227C40 | 99.68 | accumulator coalesces into r31 vs target r27 (two simultaneously-freed regs) |
| fn_80212D6C | 99.56 | defensive `mr r25,r26` relocation; target keeps the reg throughout |
| fn_80229C90 | 98.81 | 3-cycle r27/r28/r30 permutation |
| fn_80216410 | 98.69 | scheduler stages the call return through r0 |
| fn_8023CE60 / fn_8023CFDC | 98.26 | saved-register band off-by-one (twins) [re-confirmed] |
| fn_80228DAC | 99.18 | 2268-byte: pervasive r26–r29 reassignment [re-confirmed] |
| fn_80214B68 | 98.95 | sdata2 2-coloring tie-break [re-confirmed] |
| fn_8021C308 | 98.32 | r24–r29 reshuffle + one extsh. control miss [re-confirmed] |
| fn_80222ADC | 95.00 | scheduler: `mr r3,r31` call-arg before `extsh r7` [re-confirmed] |
| fn_80222EF0 | 93.30 | incoming param r3 must claim r31 throughout; CW won't pin it |
| fn_8023D158 / fn_8023F8C0 | 16.66 / 90.72 | void-pseudo-register-param idiom (3–4 uninit pseudo-reg params on a `void()` sig) |

WINNING levers this batch (folded into the agent cheat-sheet): **narrow-at-def** (declare
a local `u32` while its source `fn` extern returns `u8`/`u16` → CW masks the call
result at assignment `clrlwi rN,r3,24/16`, not at the compare); **real-signature**
(replace `void(void)`+uninit pseudo-reg locals with real `(u32 r3,u32 r4,…)` params —
cracked the fn_80236Exx ×5 family + several 0x238xxx fns); **direct sda21 lbl**
(use `lbl_8047B610` as a scalar value, `lbl=*(u32*)(lbl+1); lbl+=5`, not a
double-deref'd pointer → `@sda21` reloc); **u8 not s8** for unsigned compares
(`clrlwi/cmplwi` vs `extsb/cmpwi`); **raw-addr→array-extern** for `@ha/@l` relocs.

## 2026-06-13 — colosseum_script.c wave 2 (205→228 byte-exact, +23)

Generic wave fleet (cs_wave_workflow.js + cs_make_wave.py). Wins resumed from
wave-1 hints. New confirmed reg-coalescing / scheduler-tie walls (all reached
high % but the residual is rN-only, not C-shape-controllable at -O4,s):

| fn | reached | note |
|---|---|---|
| fn_80219354 | 99.73 | divw/mullw/subf/lwzx modulo-index: auStack base wants r5, CW gives r3 (shifts dividend/divisor) |
| fn_8023CB60 | 99.56 | r27/r28 coalescing swap (divw-result vs return accumulator, reuse different freed slots) |
| fn_80238060 | 99.49 | saved-band off-by-one: target uVar6→r31 / masked-r5→r29, CW reverses |
| fn_80229934 | 99.23 | 8-deep non-volatile rotation in else-loop (loop-varying want high regs, invariants low) |
| fn_80213A78 | 99.16 | target saves 10 callee-saved (stmw r22), CW 9 (stmw r23); entangled with a u16 arg mask |
| fn_80219964 / fn_80219B2C | 98.77 | uVar2 spanning-temp vs cond-tested var get opposite regs (both die at same call); cascades |
| fn_80236D60 | 98.67 | uVar1-into-callee-saved-across-call: CW routes via r0 (mr r0,r3;li r3,0;mr r31,r0) vs target direct |
| fn_802389D4 | 98.08 | narrow-at-def mask lands r0 vs target r3 + u8 loop-counter double-mask |

The 0x8023Dxxx–Exxx family (×10) re-confirmed walled at ~95% (deferred-write-through-r0).
Accumulated wall set: build/cs_walls.json (43 fns, auto-excluded by cs_make_wave.py).

## 2026-06-14 — colosseum_script.c wave 3 (228→270 byte-exact, +42)

Full-quota window: all 49 agents returned. Cracked many low-tier fns outright
(several 0%/<20% → 100%) via real-signature + narrow-at-def + direct-sda21 +
data-symbol relocs. +33 new reg-coalescing/scheduler walls logged (now 110 total
in build/cs_walls.json, auto-excluded by cs_make_wave.py).

NEW recurring wall sub-pattern — **loop-counter reload register choice**: at a
loop-back/merge block the target reloads a label byte (e.g. lbl_8047B648) into
**r6**, CW uses **r3**, because r3 is reserved for an upcoming call-arg `mr r3,rN`.
Seen on fn_80216F50 (99.8), fn_8021CF3C (99.81), fn_8021D090 (99.9), fn_80223E40
(99.73). Operand swap / u8 casts / for-loop / temp-local all inert. Not C-controllable.
Other ties: fn_8022DCB8 (99.99, switch body-layout vs dispatch-displacement — can't
satisfy both), fn_80232024 (99.83, extsb into fresh reg vs reuse), fn_80220B8C (99.9,
var-pair first-use coloring).

GATE CATCH: fn_80235B04 saved 100% in its band but only 90.41% bodies-only —
its win needed an out-of-body pragma; dropped from the commit (hint stored for retry).

## 2026-06-14 — colosseum_script.c campaign CONVERGED (146 → 277 byte-exact, +131)

Six waves over ~24h (commits ce8219f7 / 5a540e1d / 2931b04c / dcd7793e / b80d4eeb,
+ wave-1 partials). **277/460 byte-exact (60.2%), 0 regressions across the campaign.**
Wave 6 returned **0 saved wins** from 49 functions — every residual is a CW -O4,s
register-allocation / scheduler / coalescing tie (94–99.3%, agents confirmed
"not C-controllable"): callee-saved bank rotations, r29↔r30/r27↔r28 pair swaps,
frame-size off-by-16, dead-branch the target emits, value-range mask elimination,
loop-counter-reload r6-vs-r3. **177 walls** accumulated in build/cs_walls.json.

OPEN (not walls — future targeted/structural passes):
- 8 sub-90% non-wall fns left unattempted-to-completion (large, structural):
  fn_80213E94, fn_80220868, fn_802226A4, fn_80222BD8, fn_80226914, fn_802317E4,
  fn_80232110, fn_80233DB0.
- FILE-SCOPE-PRAGMA batch: a `#pragma optimization_level` region (~src line 21555)
  compiled at the wrong level vs target; fixing the region could crack
  fn_80218B6C / fn_8022FE80 / fn_8022FF90 / fn_80218A6C at once (needs a careful
  multi-function pass + full re-measure — out of scope for the bodies-only loop).
- The fn_8023D94C..EDF8 family (×10) still ~95% (deferred-write-through-r0).

## How to use this ledger
- **Before** attacking a near-miss: check it isn't already logged here. If it is, skip.
- **After** confirming a new wall (class match, or a tested-and-didn't-move residual):
  add a row above, and if it has good C, add the name to
  `tools/decomp_work/equivalent.txt`.
- **When a new lever lands** (e.g. `-use_lmw_stmw off` clears W3): re-open *that class*
  only, re-test its logged members, and graduate any that now match.


## 2026-06-15 - bulk walltriage batch: 41 W1 wall CANDIDATES classified (3 promoted to registry, 36 staged, 2 already registered)

Automated classification via `tools/decomp_work/ra/walltriage.py --min-mm 1` over the 95-99.99%
near-miss pool (171 of 267 triaged; 96 BLOCKED in non-compiling TUs, 2 synthetic units). Each fn's
residual is a **pure physical-register permutation** (W1 class) - the reg-remap is captured in
`tools/decomp_work/_triage_b1..4.json`. Spot-checked fn_800A541C / fn_80053C00 / fn_800FE38C
(only r4<->r3 / r0<->r3 / r5<->r3 differ).

**Conservative promotion (per the line-241 lesson):** reg-permutation verdicts CAN hide wrong-C
(several previously-filed W1+W2 walls were CRACKED to 100% once wrong-C was fixed). So only the
**3 fns already test-swept elsewhere in this ledger** (fn_8020B330, fn_8020341C, fn_8020EED4 - each
"variants inert at opt4") were promoted to `tools/decomp_work/equivalent.txt` (they were missing
from the registry). The other **36 are staged in `tools/decomp_work/_wall_candidates.md`** -
promote one ONLY after a structural-decomp exhaust (C-rewrite attempts) confirms it does not move,
matching this file's tested-and-didn't-move policy. 5 are highest-confidence (99%+, 1 reg-pair).

> NOTE: this is operational bookkeeping (fleet routing + honest staging), NOT a headline gain.
> progress2.py computes the axes purely from the objdiff byte-match join. Headline gains come from
> the **92 WINNABLE** near-misses (real structural diffs) in `tools/decomp_work/_winnable_queue.md`,
> esp. the 5 gs_field_world.c sudoku-siblings (fn_8011A280/A570/A9EC/AB50/AFCC @98.6%, struct=1).

| file | count | functions |
|---|---|---|
| gs_worldmap.c | 6 | fn_800265C0 fn_80026600 fn_80026640 fn_80026680 fn_800266C0 fn_80026700 |
| colosseum_event.c | 5 | fn_8020B330 fn_8020341C fn_8020EED4 fn_802032E4 fn_80209960 |
| effect_util.c | 4 | fn_80135F90 fn_80135FF8 fn_80136024 fn_80135FBC |
| scene_init.c | 3 | fn_80053C00 fn_80038990 fn_80038138 |
| gs_model.c | 3 | fn_80103484 fn_80101A70 fn_801096AC |
| gs_title.c | 3 | fn_80024CDC fn_80024308 fn_80025F84 |
| people_field.c | 3 | fn_80162370 fn_801628C8 fn_801631AC |
| gs_pokemon_summary.c | 2 | fn_80016618 fn_80015050 |
| gs_field_world.c | 2 | fn_8012F150 fn_801171C8 |
| DVDFs.c | 1 | fn_800A541C |
| gs_thread.c | 1 | fn_800FE38C |
| script_callback.c | 1 | fn_80053778 |
| gs_render.c | 1 | fn_800DB758 |
| main.c | 1 | fn_80005E00 |
| gs_event_exec.c | 1 | fn_800138B4 |
| colosseum_script.c | 1 | fn_80214B68 |
| ui_core.c | 1 | fn_8005D8F8 |
| gba_misc.c | 1 | fn_80089D30 |
| battle_waza.c | 1 | fn_801DD0C8 |

_Full per-fn match% + reg-remap evidence: `tools/decomp_work/_triage_consolidated.json`._
