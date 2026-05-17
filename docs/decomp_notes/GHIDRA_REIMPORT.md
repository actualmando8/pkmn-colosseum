# Ghidra re-import — findings (2026-05-17)

## What was run

`ghidra_batch_decompile.py` (reused analyzed project) → 10.6 MB fresh
decompiler C at `build/ghidra_output/raw_decompilation.c` (~381k lines,
~22k function-like defs). Then `process_ghidra_output.py` →
`ghidra_import.py` **dry-run**.

## Outcome: bulk --apply is NOT safe or valuable (do not run it)

The dry-run reported `Total imported (new): 7753 / Skipped: 650`. That is a
**broken result**, not an opportunity:

- `process_ghidra_output.py` put **7481 functions into a single
  `__unassigned__` bucket** — Ghidra has no function→TU map, so it cannot
  group them by source file. Importing these dumps thousands of stubs into
  one `__unassigned__.c` with no TU context → wrong symbol order → they
  **cannot match** and only add compile noise.
- Only ~few hundred functions map to real files, and they are concentrated
  in **dolphin/crt/trk SDK** TUs (DVD.c +36, PPCArch.c +20, OSCache.c +17,
  printf.c +9 …) that are currently empty. Those symbols are largely
  **outside the match target** (`auto_01_800055E0_text.o` is game text,
  not the SDK), so importing them risks breaking currently-clean files for
  little or no match% gain.
- Skip-detection works per-file (gs_render.c: 143 skipped, 0 imported) but
  the `__unassigned__` bulk defeats the point.

**Conclusion:** `ghidra_import.py --apply` must NOT be run until the
function→TU mapping is repaired. Forcing it in would corrupt the source
tree with ~7.5k unmappable duplicate/garbage stubs.

## What IS valuable: raw_decompilation.c as a reference

`build/ghidra_output/raw_decompilation.c` is a fresh, full Ghidra decomp
of every function. Use it as a **lookup reference**, not a bulk import:

- An agent working `fn_800XXXXX` can `grep -A60 "FUN_800xxxxx" ` (note:
  raw file uses `FUN_<lowercase-addr>`) to get fresh decompiler C for that
  one function instead of spawning a Ghidra run.
- It is gitignored (under `build/`). Regenerate any time with:
  `python tools/ghidra_batch_decompile.py` (~3 min, reuses analyzed
  project).

## Prerequisite for useful bulk import (future work)

Bulk Ghidra import only becomes valuable after a **function→TU split map**
exists (same blocker as `docs/tu_split.md`). Needed:

1. Per-address → source-file mapping (from `.file` symbols / splits /
   link order).
2. `process_ghidra_output.py` to group by that map instead of
   `__unassigned__`.
3. Then per-TU import lands functions in the correct file with correct
   symbol order, expanding the *measured* near-miss surface.

Until then: Ghidra output is a per-function reference only.
