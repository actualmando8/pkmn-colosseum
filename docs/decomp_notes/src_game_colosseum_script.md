# Decomp notes: src/game/colosseum_script.c

## Status snapshot
Large PLAIN-C (Ghidra-import) TU, CW 1.3 -O4,p. Most near-misses are W1
saved-band register tie-breaks. fn_80216650 / fn_8022E6F0 cracked 2026-06-09
(block-scope lever). fn_80219270 cracked 2026-06-10 (u32-param + per-cmp u16 cast).

## Recently landed
- **fn_80219270** — 96.94 -> **100%** (2026-06-10, commit c1e62c6a). Declared the
  param `u32` (was `u16`) and applied `(u16)` casts only on the equality
  comparisons, leaving `(u16)(r3 - 0xa5U)` to read the raw param. This keeps the
  raw value in r3 for the `subi r0,r3,0xa5` while the masked compare-value goes to
  r4 (`clrlwi r4,r3,16`) — the target's register split. Also De-Morgan-inverted the
  condition (`return 1` first, `return 0` fall-through) to match the target's
  `li r3,0x1; blr; li r3,0x0; blr` epilogue order. **LESSON: a `u16` param forces
  CW to clrlwi-into-the-same-reg, destroying the raw value an unmasked sub needs;
  promoting to `u32` + casting at compare sites recovers the target's two-register
  split.**

## Blocked near-misses

- **fn_80222ADC** @ 99.63% — W1 saved-band tie-break (subi/extsh. home register)
  - Symptom: target `subi r4,r3,0x1; extsh. r0,r4` (subtract result lives in r4,
    test extends into scratch r0); CW emits `subi r0,r3,0x1; extsh. r4,r0`
    (subtract scratch r0, extends into r4). `short sVar4 = (uVar3 & 0xff) - 1`.
  - Tried: `(u8)uVar3` cast (inert), `s32 sVar4` + `(s16)` casts at uses (97.0 ✗).
  - Class: W1 saved-band tie-break, opt4-locked. Only 2 instrs differ.
  - Last attempt: 2026-06-10 by executor

- **fn_8021C0F4** @ 97.44% — signed-div-by-2 idiom mismatch
  - Symptom: target `srawi r0,r3,1; addze r0,r0` (2-instr round-toward-zero /2);
    `iVar3 = -(uVar2 >> 1)` gives only `srawi` (floor, 1 instr, 97.44); `uVar2 / 2`
    gives CW's 3-instr `srwi r0,r3,31; add r0,r0,r3; srawi r0,r0,1` (92.18).
  - Root cause: CW 1.3 -O4 lowers signed `/2` to the 3-instr add-then-shift form;
    the target was built with a compiler that uses the compact `srawi/addze`. No C
    spelling or `optimization_level 2` reaches the 2-instr idiom.
  - Tried: `>>1` (97.44, baseline kept), `/2` (92.18), split `q=x/2;q=-q` (92.18),
    inlined `(int)call()/2` (92.18), `#pragma optimization_level 2` (92.18).
  - Class: codegen-idiom wall (div lowering). Baseline `>>1` retained at 97.44.
  - Last attempt: 2026-06-10 by executor

- **fn_80228DAC** @ 99.18% — W1 saved-band rename + W2 prologue mr-through-r0
  - Symptom: pervasive r26/r27/r28/r29 renaming across the body, plus prologue call
    results homed via `mr r0,r3; mr rN,r0` (extra hop) where target does `mr rN,r3`
    directly (the following `li r3,K` clobbers r3 before CW schedules the save), and
    one `clrlwi r26,r3,16` direct vs `mr r29,r3; clrlwi r0,r29,16`.
  - Class: W1 saved-band tie-break + W2 scheduler. 2860-byte fn, ~25 reg-name diffs.
  - Last attempt: 2026-06-10 by executor (diagnosed, not perturbed — breadth-first)

## Pre-existing filed walls (from WALLS.md, do not re-attack)
fn_8023CE60 (98.26, W1 twin of fn_8023CFDC/fn_8023D158), fn_80214B68 (98.95,
W1 volatile 2-coloring), fn_8021F1CC (96.88, W2 switch-dispatch).

## Session log

### 2026-06-10 — executor (Opus)
- fn_80219270 -> 100% (u32-param + per-cmp u16 cast + De Morgan inversion). Committed c1e62c6a.
- fn_80222ADC, fn_8021C0F4, fn_80228DAC triaged as walls (see above).
- Helper scripts added (untracked): tools/decomp_work/measure_fn.py, diff_fn.py.
