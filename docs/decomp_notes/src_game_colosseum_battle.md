# Decomp notes: src/game/colosseum_battle.c

## Status snapshot
199/646 @ 100% (30.8%, as of 2026-05-13)

## ⚠️ File-level diagnosis (2026-05-13, w5 attempt)

Agent w5 stalled trying to find 95-99.99% near-misses and reported:

> **The 199 matched are all simple/short fns. The unmatched 447 are complex
> asm-style fns sitting in the 35-60% band — there are essentially no
> 95-99% near-misses in this file.**

This means **the easy-win sweep strategy will not work here.** The file
needs:

1. **Full decomp passes** on the asm-style fns to lift them from 35-60%
   to 90%+ before any near-miss work
2. Or **TU split** so the .o becomes smaller targets — but we don't have
   evidence the original was multi-TU

## Recently landed (prior session, x8 wave 2)

5 commits cherry-picked, 175 → 199:

- fn_8025DD14 (if-inversion)
- fn_802612D0 (base-pointer array pattern)
- fn_8025E534 (97.9% array + casts, partial)
- fn_80261D8C (80.5% variadic, partial)
- fn_8025F2FC (85.2% revert goto, partial)
- + 8 type-fix wins

## Untouched
Most remaining unmatched fns. Use `python tools/objdiff-cli.exe diff -1 <target> -2 <obj>` to see per-fn match; focus on fns where match% is >70% before attempting near-miss work.

## Session log

- **2026-06-06 (Codex battle-flow grind)** — `fn_8025F2FC` converted from
  83.3333% to **100.0000%** active C by expressing the original shared
  completion branch for request slots where `entry[5] != 0 || entry[6] != 4`.
  Verified with `python tools/compile_check.py src/game/colosseum_battle.c --diff --symbols fn_8025F2FC fn_8025F514 --timeout 180`:
  `fn_8025F2FC 21/21 instructions, 0 mismatches, no active asm wrapper`.
- **2026-06-06 (Codex battle-flow grind)** — `fn_8025F514` converted from
  60.0000% to **100.0000%** active C by adding a local `#pragma scheduling off`
  around the battle request-stop flag setter. Verified with
  `python tools/compile_check.py src/game/colosseum_battle.c --diff --symbols fn_8025F514 fn_8025F584 fn_8025F648 --timeout 180`:
  `fn_8025F514 4/4 instructions, 0 mismatches, no active asm wrapper`.
- **2026-05-13 (w5)** — stalled, no salvageable commits. Identified the
  "no actual near-misses" problem above. Recommend full-decomp wave on the
  35-60% cluster, not easy-win sweeps.
- **2026-05-13 (x8 wave 2)** — 175 → 199 via 5-commit cherry-pick chain.
