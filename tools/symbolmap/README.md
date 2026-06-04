# symbolmap — string-driven symbol map for GC6E01

Builds a function symbol map for Pokémon Colosseum from the **debug strings**
already present in `main.dol`, plus the sister game's decomp. Three sources of
evidence, all derived deterministically from dtk's symbol-resolved disassembly:

| stage | evidence | output |
|---|---|---|
| **1 — TU attribution** | CodeWarrior `__FILE__` literals (`cobj.c`, `menuCB_Battle.c`, …). Every function referencing a `*.c` literal was compiled from that file. | `tu_attribution.md`, `tu_evidence.json` |
| **2 — name proposals** | functions that log their own name (`GSmaterialCreate: …`, `_menuPop():…`, `GCN_Mem_Alloc.c : InitDefaultHeap`). | `name_proposals.md/.json`, `name_proposals_high.txt` |
| **3 — XD port** | Pokémon **XD** (GXXE01) runs the same engine and its decomp names ~17k functions. Match Colosseum↔XD functions by shared string literals and copy the real name. | `xd_port.md/.json`, `xd_port_apply.txt`, `xd_vocabulary.json` |
| **merge** | reconcile, prefer XD > self-name, unique names only | `proposed_symbols.txt`, `symbols.with_proposals.txt` |

## Why not Ghidra?

Ghidra works, but its saved project here is empty (analysis lives only in the
open GUI session), and a headless re-analysis of a 3.7 MB dol is slow and not
reproducible. dtk's disassembly already resolves every reference to a symbol —
including the small-data-area (`r2`/`r13`, `@sda21`) references that are exactly
why Ghidra needs full auto-analysis. So this pipeline reads dtk asm instead:
deterministic, reproducible, and the same data Ghidra would compute. Ghidra
remains useful for *interactive* exploration and its decompiler output
(`tools/ghidra/run_export.sh`); this just doesn't depend on it.

## Run

```sh
# from repo root, after the Colosseum asm exists (configure.py && ninja)
python tools/symbolmap/run.py                      # stages 1, 2 (+3 if XD asm)
python tools/symbolmap/run.py --xd-asm <xd>/asm    # all stages incl. XD port
```

### Enabling the XD port

Needs XD's `main.dol`. From a disc image (rvz/iso):

```sh
# 1. rvz -> iso (if needed)
dolphin-2603a-x64/Dolphin-x64/DolphinTool.exe convert -i XD.rvz -o xd.iso -f iso
# 2. pull main.dol (run dtk from the iso's dir; drive-letter ':' collides with
#    dtk's container ':' separator, so use a relative path)
cd <dir-with-xd.iso> && tools/dtk.exe vfs cp "xd.iso:sys/main.dol" xd_main.dol
# 3. split with the mining config (template: tools/symbolmap/xd_config.mine.yml;
#    no `splits:` -> emits asm for all funcs, named from XD's symbols.txt).
#    Run from the xd-decomp root, forward-slash outdir (dtk drive-letter quirk).
cd tools/decomp_work/refs/xd-decomp
cp <…>/xd_main.dol orig/GXXE01/sys/main.dol
cp ../../../symbolmap/xd_config.mine.yml config/GXXE01/config.mine.yml
../../../dtk.exe dol split --no-update config/GXXE01/config.mine.yml _xdsplit
# 4. point the pipeline at it
python tools/symbolmap/run.py --xd-asm tools/decomp_work/refs/xd-decomp/_xdsplit/asm
```

XD's extracted `main.dol` SHA1 must be `ff9e752ead9914af0b363ae6c831a34ccce189d2`.

## Adopting proposals — safety

Everything here is **advisory**. The pipeline never edits `symbols.txt`, the
asm, or any `.inc` truth file — adopting a name cannot affect byte-match%, and a
wrong name cannot forge a match. To adopt:

- Review `proposed_symbols.txt` (one `fn_OLD -> NEW // provenance` per line).
- `symbols.with_proposals.txt` is `symbols.txt` with the renames applied, for a
  `diff` against the real file before you copy any line over.
- XD-port names (score ≥ 2, unambiguous) are the most trustworthy; string
  self-names can be a shared prefix rather than a unique name (flagged).

## Applying the names

The proposals were applied to the project (this is what's committed alongside
the tooling):

```sh
python tools/symbolmap/partition_apply.py     # split proposals -> applied vs leads
python tools/symbolmap/apply_names.py --map config/GC6E01/symbolmap/applied_symbols.txt --symbols --source
```

`partition_apply.py` splits the 54 merged proposals into:

- **`applied_symbols.txt` (34)** — renamed in **both** `symbols.{txt,build.txt}`
  and `src/**/*.{c,h}`. Safe because the name has no pre-existing typed
  prototype to clash with.
- **`leads_needs_wiring.md` (20)** — names that ALREADY exist as a typed
  prototype in the headers (XD *and* the headers agree — doubly confirmed), so
  the asm-wrapper's `(void)` signature and untyped `lbl_` globals conflict.
  Left as `fn_` (annotated in `symbols.txt`); each needs per-function typing to
  wire up. The split is **compile-verified**: `compile_sweep.ps1` confirmed
  zero rename-induced compile regressions across every touched TU vs. the `fn_`
  baseline, and `dtk` re-link reproduced `main.dol` byte-identical.

Why this is byte-safe: `config.libs = []` (configure.py), so the byte-match
build links dtk-extracted asm objects carved from the DOL — the C is not
compiled into it, and the DOL has no symbol table. Renaming is byte-neutral;
`build/GC6E01/ok` (SHA-1 gate) still passes.

### Wiring leads (partial)

Some leads are tractable: a function whose only blocker is a return-type
mismatch (header `u16 NewName(void)` vs wrapper `void NewName(void)`) wires by
renaming + matching the return type. `wire_leads.py` does the return-type fix
(byte-neutral — the asm body is verbatim `.inc`); `finalize_leads.py` then moves
the wired names into `applied_symbols.txt` and refreshes `leads_needs_wiring.md`
+ the `symbols.txt` annotations. 9 of the original 20 leads were wired this way
(the 8 `effect_visual` `u16` effect functions + `_sndCheckSndWorkALL`), each
compile-verified with byte-match preserved. The remaining 11 need typed-arg
signatures + call-site casts / global typing (genuine per-function decomp).

### Regenerating `.inc` after a rename

The stock `convert_to_asm_wrappers.py` / `regen_incs.py` only recognise `fn_`
wrappers. For renamed functions use the name-aware regen (so objdiff works
without committing the gitignored `.inc`):

```sh
ninja build/GC6E01/ok                                  # produce renamed build/GC6E01/asm
python tools/symbolmap/regen_named_incs.py --all       # regenerate every .inc by name
```

## Regenerating

All `*.json`/`*.md` outputs are deterministic functions of the dtk asm. The
large intermediates (`fn_strings.json`, `symbols.with_proposals.txt`) are
gitignored; re-run `run.py` to recreate them.
