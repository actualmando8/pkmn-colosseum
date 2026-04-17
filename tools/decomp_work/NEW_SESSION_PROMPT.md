# New Session: Apply gs_title.c Pipeline to Other Source Files

You are taking over a Pokémon Colosseum (GPXE01) byte-match decompilation project.
The previous session pushed `src/game/gs_title.c` from ~66% avg to **92.23% avg**
across 15 functions using a Ghidra → CW pipeline. **gs_title.c is now considered
done** (the remaining 1-8% gaps are CW 1.3 codegen ceilings — see "DONE" section).

Your job: **apply the same pipeline to other src/game/*.c files**, leveraging
multiple models in parallel (especially the free/local ones) to maximize
throughput.

---

## Project orientation

- **Repo root:** `C:/Users/douglaswhittingham/pkmn-colosseum`
- **Compiler:** `tools/mwcc_compiler/GC/1.3/mwcceppc.exe` (CW 1.3) is the default for game/. Per-file overrides in `config/GC6E01/compile_config.json`.
- **Build target:** `build/GC6E01/obj/auto_01_800055E0_text.o` is the byte-truth.
- **Match tool:** `python3 tools/match_scan.py fn_XXXXXXXX [...]`
- **Per-file scan:** `python3 tools/scan_all_files.py` returns 0% for every file — that's expected (objdiff-cli's section match needs symbol-set parity which game .o files don't have vs the full target.o). Use the **asm-active count proxy** below (counts `#if 1` + `asm void fn_` patterns) instead — that's the real metric.
- **Compile + diff:** `python3 tools/compile_check.py src/game/<file>.c`
- **Full diff:** `./tools/objdiff-cli.exe diff -1 build/GC6E01/obj/auto_01_800055E0_text.o -2 build/GC6E01/base/game/<file>.o -o - --format json -c ppc.calculatePoolRelocations=false fn_XXXXXXXX`

## Pipeline (READ THIS FIRST)

The full Ghidra→CW recipe is in **`tools/decomp_work/CLAUDE.md`** with 7 documented quirks. Critical excerpts:

1. **Quirk 1:** `switch (x) { case 1: ...; case 0: default: ...; }` — the redundant `case 0:` produces the dead `cmpwi r0, 0` CW emits.
2. **Quirk 2:** declare local as `s32 unaff_rN;` and apply `(s16)` cast at USE site for explicit `extsh` before xor (not at assignment).
3. **Quirk 3:** `f32 local[4]` array (not separate `f32 a; f32 b;`) eliminates spurious `f31` save spill (~+5% per function).
4. **Quirk 4:** `((u32)*ptr >> N) & 1` produces target's `extrwi`; `(s8)*ptr < 0` produces wrong `extsb+cmpwi`.
5. **Quirk 5:** Nested call expressions (`fn_X(fn_Y(...))`) keep the intermediate pointer in r3 (volatile), avoiding non-volatile spill.
6. **Quirk 6 (NEW THIS SESSION):** `*(volatile u8*)ptr` cast prevents CW from caching byte loads in r4 across multiple bit tests in linked-list walks (+1-2% per walk).
7. **Quirk 7:** Nested `!=` chain with comma operator: `if (a != X0 && (iv=1, a != X1) && (iv=2, a != X2) && ...) iv = N;` — CW emits the progressive `li rN, i` sequence target uses for table-of-IDs lookups.

Plus discovered after CLAUDE.md was last updated:
- **`(s16)` outer cast** on f32 expression assigned to `*(s16*)` memory: avoids extra `extsh` before `sth` that the more obvious `(s32)` cast emits (+1-2%).
- **goto LAB_XXX inside for/while loop** instead of `break`: emits target's `bne+b` pair instead of `beq` (+2-5%).
- **Duplicate fn calls in if/else branches** (e.g., `if (cond) { ...; fn(x); } else { ...; fn(x); }` not `if(cond) ...else...; fn(x);`) — matches target's per-branch register-allocation choices (+1-2%).

## Available agents/models

Run **multiple in parallel via tmux**. Use `tools/decomp_work/tmux_control/control.sh` for Codex/GLM panes; spawn additional agents via `Task` tool for Claude executors.

### Tier 1 (perfect 13/13 on benchmark — use first):
- **codestral:22b** (local ollama, free, slowest at ~30s/fn but most accurate)
- **deepseek-coder-v2:16b** (local ollama, free)
- **kimi-k2-turbo-preview** (Moonshot, paid but fast)
- **qwen2.5-coder:7b** (local ollama, free, FAST)

### Tier 2 (10/13 — viable for simpler functions):
- **deepseek-r1:14b** (local, free)
- **qwen2.5-coder:32b** (local, free, slow)
- **qwen3:14b** (local, free)

### Tier 3 (uncertain):
- **opencode/nemotron-3-super-free** (6/13, free)
- **moonshot/kimi-latest** (8/13, paid)
- **GLM-5.1** in opencode pane (good for review/iteration)
- **Codex GPT-5.4** in tmux pane 0:0.4 (great for cross-checking)

### AVOID:
- opencode/gpt-5-nano (0/13)
- opencode/minimax-m2.5-free (0/13)
- ollama-proxmox/gemma3:4b (7/13, slow + mediocre)

### Run benchmarks via:
```bash
python3 tools/decomp_work/benchmark/bench_opencode.py <model_name>
```

## Workflow per file

For each target `src/game/<name>.c`:

1. **Confirm it's a candidate** (run `python3 tools/scan_all_files.py` and pick from the bottom of the list — lowest match% has the most potential gain).

2. **Generate Ghidra decomps** for any unmatched functions in that file (most should already exist in `tools/decomp_work/ghidra_out/`). If missing, run:
   ```bash
   bash tools/ghidra/run_headless.sh
   # then export specific addresses via tools/ghidra/scripts/ExportDecomp.java
   ```

3. **Spawn parallel agents** (1 per function, distribute across the model tiers above). Each gets:
   - The function's address + size
   - Ghidra C output
   - Target asm inc file (`src/game/<name>_fn_XXXXXXXX.inc`)
   - The 7 CW quirks from `tools/decomp_work/CLAUDE.md`
   - Constraint: don't touch other functions, must keep compile green, must not regress.

4. **Verify each result** by patching into the .c file, running `python3 tools/compile_check.py src/game/<name>.c` then `python3 tools/match_scan.py fn_XXXXXXXX`. If the new code regresses, revert.

5. **Commit per-function** with message `<name>: fn_XXXXXXXX X.X%->Y.Y% via <approach>`.

6. **Stop pushing a function once at ≥99% OR after 5+ source variants fail**. The remaining 1-2% is usually CW reg-alloc that source can't fix.

## Don't touch (DONE):

- **`src/game/gs_title.c`** — 92.23% avg, 15 functions, ceiling reached. The doc blocks at the top of the file explain every remaining diff. Touching it risks regression for no gain.
- The 7 named quirks in `tools/decomp_work/CLAUDE.md` are reference; do not edit unless adding a NEW discovered quirk.

## File state at session handoff

- Avg match across 15 prior-unmatched gs_title functions: **92.23%**
- Latest commit: `git log --oneline -1`
- Pending items in `tools/decomp_work/relay/` are review artifacts; safe to ignore unless cross-referencing.
- `.gitattributes` was set with `core.autocrlf=input` — file editing should be stable now (no more "file modified since read" stalls).

## Top targets (ranked by asm-active function count)

Run this anytime to refresh the list:
```bash
python3 -c "
import os, re
counts = []
for f in sorted(os.listdir('src/game')):
    if not f.endswith('.c'): continue
    with open(f'src/game/{f}', 'r', errors='ignore') as fp:
        c = fp.read()
    asm_active = len(re.findall(r'#if 1\s*\nasm void fn_', c))
    if asm_active > 0:
        counts.append((asm_active, os.path.getsize(f'src/game/{f}'), f))
counts.sort(reverse=True)
for a, s, f in counts[:25]:
    print(f'{a:4d} asm-active  {s:7d}b  {f}')
"
```

Current ranking (snapshot at handoff):
```
asm-active  size      file                       difficulty (heuristic)
   94        712 KB   gs_field_world.c           HUGE — multi-week push
   71        153 KB   gs_thread.c                HIGH-DENSITY (best ROI per file?)
   58        192 KB   scene_init.c
   48        121 KB   gs_render.c                FP/GX-heavy, like gs_title
   46         60 KB   gs_worldmap.c              MID-SIZE, good first target
   28         56 KB   gs_npc_interact.c          MID-SIZE, good first target
   25         14 KB   gs_event_exec.c            SMALL, fast wins
   23         15 KB   gs_pokemon_summary.c       SMALL, fast wins
   18         69 KB   gs_npc_event.c
   11        207 KB   gs_material.c              big file, only 11 left
    7         34 KB   gs_texture.c               SMALL, fast wins
    6        162 KB   gs_pcbox.c
    6         26 KB   gs_scene.c
    4         36 KB   gs_party_access.c
    3         64 KB   gs_task.c
    3         20 KB   movie.c
    3         20 KB   gs_gfx.c
```

**Recommended first targets** (small → large):
1. `gs_event_exec.c` (25 asm, 14KB) — quick wins to validate pipeline
2. `gs_pokemon_summary.c` (23 asm, 15KB) — small, easy
3. `gs_texture.c` (7 asm, 34KB) — texture fns likely simple HSD wrappers
4. `gs_worldmap.c` (46 asm, 60KB) — first medium-effort file
5. `gs_thread.c` (71 asm, 153KB) — high density once ramped up

**Avoid for first session:**
- `gs_field_world.c` (94 asm, 712KB) — too big for one session
- `colosseum_event.c` / `colosseum_battle.c` / `colosseum_script.c` — game-script files, rarely byte-match without huge investment

## Tmux setup

The decomp tmux session has these panes:
- pane `0:0.0` — status bar (don't touch)
- pane `0:0.1` — main agent (you/Claude Code)
- pane `0:0.3` — GLM-5.1 (opencode) — good for reviews
- pane `0:0.4` — Codex GPT-5.4 — good for first drafts

To send a prompt: `tmux send-keys -t %7 "your prompt" Enter` for GLM, `%8` for Codex.
**IMPORTANT:** send the full prompt as a single send-keys call to avoid the "Please" truncation bug.

## Success criterion for the new session

Push avg match% across 1-3 additional game files from <X>% to ≥80%, with
zero regression on previously-matched functions (including all of
gs_title.c's 15 documented near-misses).
