# CodeWarrior matching levers — GC6E01

Upload this alongside a sandbox kit, or paste it into a ChatGPT Project's
instructions. Every entry below was **measured in this repo**, not inferred.

Read the diff shape first, then pick the lever. Most near-misses are codegen
steering, not a logic error — if the opcodes are right and only registers or
ordering differ, do not rewrite the algorithm.

## Ground rules (violating these makes a "match" worthless)

- **C89.** All declarations precede all statements in a block.
- **No live inline asm, no `.inc` include, no asm wrapper.** Rejected on sight
  by `verify.py`. Note the codebase *does* keep original asm in `#if 0 / #else`
  blocks — that is dead reference material and is fine; adding **live** asm is not.
- Types `u8/s8/u16/s16/u32/s32/f32/f64`. `cmpwi` signed, `cmplwi` unsigned.
- Float constants come from named `extern f32 lbl_XXXX;`, never literals.
- Change only the one `.c`. Flag changes go in that unit's `extra_cflags`.

## Flag levers (checked FIRST — invisible to source rewriting)

| Symptom in diff | Lever |
|---|---|
| target `stmw`/`lmw`, we emit paired `stw`/`lwz` | **add `-O4,s`**. `-use_lmw_stmw on` is NOT enough at 2–3 registers. This alone took fn_801EEE6C 74.74% → **100%** |
| whole-function shape off, opcodes right | `#pragma optimization_level 0..4` sweep, keep best |
| `fmadds`/`fnmsubs` missing | `-fp_contract on` (usually already set) |
| stray codegen changes after an earlier function | an un-popped `#pragma` leaked — always `#pragma push` / `#pragma pop` |

Do **not** try to fix a diff by changing `mw_version` based on the target
object's `.comment` version — target objects all report `2.3.0.1` as a dtk
artifact. fn_801EEE6C matches byte-exactly against a "2.3.0.1" target using a
compiler emitting 2.4.2.1.

## Source-shape levers

| Symptom | Lever |
|---|---|
| branch polarity inverted (`beq` vs `bne`) | invert the C test; early-return the negated case: `if (x != K) return 0; call();` |
| `clrlwi` present/absent at a callsite | declare a **local** `extern void fn(..., u8);` inside the caller — a file-scope prototype or an explicit `(u8)` cast both produce different code |
| mask via `& 0xFF` vs `(u8)` cast | on an `s32`, `x & 0xFF` gives a bare `clrlwi` with no tail mask |
| extra `mr` round-trip | drop a redundant `(u16)`/`(u8)` cast on an already-correctly-typed return |
| wrong register assignment (rN vs rM), same opcodes | reorder **declarations**; the later-declared co-surviving local often takes the lower register |
| `mtctr`/`bdnz` loop missing | inline the trip count into the `for` init, no separate local, no outer `if` guard |
| two loads where we emit one (CSE too eager) | read through `*(volatile T*)&x` to force a re-read |
| redundant counter kept in a `do/while` | keep the count in both branches so CW cannot fold it |

## Known walls — do not spend time here

- **`addi rD,rS,0` vs `mr`** (zero-offset pointer arithmetic). On `gdev_cc_write`
  every form folds: `&p[0]`, `(char*)((u32)p+0)`, `&struct->member0`,
  `#pragma peephole off`, and all opt levels ≥2. Wall at 98.75%, 1 instruction.
- **Anonymous `@NNN@sda21` conv-literals.** When the target references a *named*
  `lbl_XXXX@sda21` and we emit `@32@sda21`, the float pool entry is
  position-dependent; this typically caps a function just under 100%.
- **FPR web / f0-f1-f2 permutations** accompanying the above — usually the same
  root cause, not separately steerable.
- **Register permutation across 8+ non-volatiles** — no reliable source lever.

## Workflow

1. `gen_brief.py --source X --symbol Y` — brief with diff + matching levers.
2. `make_sandbox.py --source X --symbol Y` — tarball for an offline model to
   iterate against ground truth (`./try.sh` prints match % and every delta).
3. `verify.py --source X --symbol Y --candidate new.c --link` — the real gate.

**objdiff 100% is necessary, not sufficient.** The authoritative check is
`main.dol` matching `config/GC6E01/build.sha1`. Units can score 100% and still
fail to link (shared-master shims → `multiply-defined`) or link and change the
DOL. Always finish with `--link`.
