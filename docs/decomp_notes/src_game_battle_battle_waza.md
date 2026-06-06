# Decomp notes: src/game/battle/battle_waza.c

## Session log

- **2026-06-06 (Codex battle-flow grind)** — corrected the mislabeled waza
  accessor block at `0x801D1470`-`0x801D16F0`. Verified with
  `python tools/compile_check.py src/game/battle/battle_waza.c --diff --symbols fn_801D1470 fn_801D1650 fn_801D167C fn_801D16C4 fn_801D16F0 fn_801D1A44 fn_801D1A88 fn_801D1ACC fn_801D1B10 fn_801D1B4C --timeout 180`.
  Results after correction:
  `fn_801D1470 3/3 100.0000%`,
  `fn_801D1650 11/11 100.0000%`,
  `fn_801D16C4 11/11 100.0000%`,
  `fn_801D16F0 17/17 100.0000%`,
  `fn_801D1A44 17/17 100.0000%`,
  `fn_801D1A88 17/17 100.0000%`,
  `fn_801D1ACC 17/17 100.0000%`,
  `fn_801D1B10 15/15 100.0000%`,
  `fn_801D1B4C 11/11 100.0000%`.
  `fn_801D167C` is real active C and behaviorally corrected but remains at
  `17/19 89.4737%` due only to `li r4, 0xa` scheduling before/after LR save.
