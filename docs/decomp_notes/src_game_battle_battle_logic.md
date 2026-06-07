# Decomp notes: src/game/battle/battle_logic.c

## Session log

- **2026-06-06 (Codex Battle Colosseum PC-port support)** — inspected
  `colosseum_battle.c`, `battle_logic.c`, `battle_waza.c`, and
  `colosseum_event.c`; focused the active-C pass on battle UI/animation state
  helpers in `battle_logic.c`. Converted `fn_801E17A8` from a semantic
  boolean-expression non-match into exact active C by spelling out the target
  false/true/test branch layout before the `fn_801E3978` placement/animation
  state call. Verified with
  `python tools/compile_check.py src/game/battle/battle_logic.c --diff --symbols fn_801E11F0 fn_801E17A8 fn_801E1D7C --timeout 180`:
  `fn_801E17A8 26/26 100.0000%`.
  Semantic/non-win measurements from the same pass:
  `fn_801E11F0 23/28 82.1429%` (cleanup loop is active C; remaining mismatch is
  global-base setup and zero-temp register scheduling),
  `fn_801E1D7C 8/42 19.0476%` in committed source; a typed semantic rewrite was
  tested locally and reached `32/42 76.1905%`, but was not committed because this
  pass is scoped to clean exact wins.
