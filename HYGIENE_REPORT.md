# Repo Hygiene Report

Generated: 2026-05-28  
Operator: automated cleanup pass (executor agent)

---

## Summary

| Metric | Value |
|--------|-------|
| Root scratch JSON files deleted | 27 |
| Root stray .o/.s/.obj/scratch files deleted | 14 |
| Total bytes reclaimed (deleted) | 3,658,210,638 bytes (3.407 GB) |
| Files moved to tools/attic/ | 297 |
| .gitignore patterns added | 37 new lines across 8 categories |
| Security findings | 2 secrets in working tree (never committed); 1 path reference in tracked file |

---

## Task 1 — Root-Level Scratch Files Deleted

All 41 files confirmed untracked (`git ls-files --error-unmatch` returned error for each)
before deletion.

### Scratch diff/match JSON dumps (27 files, 3,658,072,177 bytes = 3.407 GB)

| File | Size (bytes) |
|------|-------------|
| `_diff_07e4.json` | 140,994,085 |
| `_diff_78a4.json` | 138,657,059 |
| `_diff_80068738.json` | 141,874,285 |
| `_diff_DE4.json` | 138,658,717 |
| `_diff_e9c.json` | 138,661,194 |
| `_objdiff_a08.json` | 277,268,386 |
| `diff_a08.json` | 138,634,970 |
| `diff_cache_fn_80029FAC.json` | 138,635,586 |
| `diff_cache_fn_8002A0B8.json` | 138,635,586 |
| `diff_cache_fn_8002A1C4.json` | 138,635,586 |
| `diff_cache_fn_8002A2CC.json` | 138,635,586 |
| `diff_cache_fn_800F7F64.json` | 138,658,311 |
| `diff_fn_80103484.json` | 139,196,516 |
| `diff_iter1.json` | 138,654,120 |
| `diff_jobj_d5a0.json` | 139,278,224 |
| `diff_one.json` | 138,661,194 |
| `fn_8017B2CC_current_diff.json` | 139,118,111 |
| `fn_8017B2CC_diff.json` | 139,115,362 |
| `fn_8017B2CC_report.json` | 139,115,362 |
| `gfw_18de0.json` | 12,459,566 |
| `gfw_match.json` | 12,459,566 |
| `match_new.json` | 146,310,201 |
| `match_sched.json` | 146,310,157 |
| `scratch_fn_800E3604.json` | 141,187,419 |
| `tmp_diff.json` | 139,795,201 |
| `tmp_diff2.json` | 139,794,064 |
| `tmp_hsd_dobj_9905c.json` | 138,667,763 |

**Subtotal: 3,658,072,177 bytes (3,488.6 MB)**

### Root stray .o / .s / .obj / scratch files (14 files, 138,461 bytes)

| File | Size (bytes) | Type |
|------|-------------|------|
| `DVDLow.s` | 51,688 | Stray assembly dump |
| `EXI2.s` | 29,531 | Stray assembly dump |
| `EXI2.o` | 4,240 | Stray object file |
| `hsd_aobj.o` | 3,512 | Stray object file |
| `hsd_class.o` | 6,816 | Stray object file |
| `hsd_cobj.o` | 8,496 | Stray object file |
| `hsd_displayfunc.o` | 2,112 | Stray object file |
| `hsd_dobj.o` | 5,672 | Stray object file |
| `hsd_object.o` | 1,336 | Stray object file |
| `hsd_state.o` | 4,448 | Stray object file |
| `hsd_wobj.o` | 5,088 | Stray object file |
| `fn_80020EA4_ANNOTATED.s` | 10,223 | Stray annotated ASM |
| `NUL.obj` | 5,299 | Stray object file |
| `_diff2.out` | 0 | Empty diff output |
| `_objdump_input.err` | 0 | Empty error capture |
| `nul` | 0 | Windows device stub file |

**Subtotal: 138,461 bytes**

### Not deleted — protected root JSON

These were left in place (pipeline or tooling config):
- `objdiff.json` — active pipeline config (tracked)
- `opencode.config.json` — opencode editor config (untracked but intentional)
- `opencode.json` — opencode editor config (untracked but intentional)
- `compiler_comparison_results.json` — benchmark data (14,094 bytes, untracked; added to .gitignore)

---

## Task 2 — .gitignore Additions

Added 37 new lines across 8 categories to `.gitignore` (appended after line 67):

```gitignore
# Root-level scratch diff/match JSON dumps (regenerable by pipeline tools)
/_diff*.json
/diff_*.json
/diff_cache_*.json
/tmp_diff*.json
/tmp_*.json
/match_*.json
/_objdiff_*.json
/scratch_*.json
/gfw_*.json
/fn_*_diff.json
/fn_*_current_diff.json
/fn_*_report.json
/diff_iter*.json
/diff_one.json
/diff_a08.json

# Root-level stray object/assembly files (build artifacts in wrong place)
/*.s
/*.obj
/NUL.obj
/nul

# Root-level scratch output files
/_diff*.out
/_objdump_input.err
/compiler_comparison_results.json

# Tools scratch diff/match JSON in tools/
tools/_diff_*.json
tools/_fn_*_diff.json
tools/diff_*.json

# Tools scratch text result dumps
tools/*_source.txt
tools/fn_*_source.txt
tools/col_*.txt
tools/cc_*.txt
tools/compile_*.txt
tools/match_*.txt
tools/_*.txt

# Tools scratch .c/.o experiments (codegen experiments, not source)
tools/_*.c
tools/_*.o

# Attic (moved dead code, never commit)
tools/attic/

# Secrets — additional key file patterns
deepseek*.txt
*_key.txt
*key*.txt
!requirements.txt
```

---

## Task 3 — Files Moved to tools/attic/

Total: **297 files** moved.

### Category breakdown

| Category | Count | Description |
|----------|-------|-------------|
| `_scratch_py` | 16 | `tools/_*.py` — prefixed single-experiment scripts |
| `_scratch_c` | 15 | `tools/_*.c` — prefixed codegen variant .c files |
| `_scratch_o` | 15 | `tools/_*.o` — compiled output of above .c files |
| `fix_oneoff` | 9 | `tools/fix_*.py` untracked one-off fix scripts |
| `source_txt` | 46 | `tools/fn_*_source.txt`, `fn_*_asm.txt`, `fn_*_test.txt` — per-function source captures |
| `col_txt` | 19 | `tools/col_*.txt` — colosseum symbol analysis dumps |
| `cc_txt` | 8 | `tools/cc_*.txt` — compile-check output dumps |
| `compile_txt` | 11 | `tools/compile_*.txt` — compilation result dumps |
| `match_txt` | 4 | `tools/match_*.txt` — match result dumps |
| `result_txt` | 51 | Other `*_out.txt`, `*_result.txt`, `*_results*.txt` — miscellaneous result captures |
| `diff_json` | 7 | `tools/_diff_*.json`, `tools/diff_*.json` in tools/ |
| `oneoff_py` | 94 | Untracked one-off Python scripts (analysis, wrap, scan, gen, etc.) |
| `oneoff_py` (other) | 2 | `.c` scratch: `headless_tool_shim.c`, `objdiff_cli_headless.c` |
| `oneoff_py` (bat/sh) | 7 | `.bat`/`.sh` one-off scripts: `do_compile.bat`, `do_diff.bat`, `do_match.bat`, `run_test.bat`, `run_test_fixup.bat`, `spawn_codex_pane.sh`, `spawn_codex_pane.sh` |

### convert_gotos_*.py variants

All 11 `convert_gotos*.py` variants (`convert_gotos.py`, `_aggressive`, `_dowhile`, `_manual`,
`_perfunc`, `_safe`, `_safe2`, `_switch`, `_v2`, `_v2_perfunc`, `_v3`) are **tracked by git**
and therefore were **left in place**. They cannot be moved without a `git mv` or `git rm`,
which is out of scope for this pass. Note for future cleanup: evaluate whether all variants
are still needed or if most can be archived via a tracked commit.

---

## Uncertain — Left in Place

The following untracked files were **left in tools/** because their purpose suggests
potential ongoing use. Review before the next cleanup pass:

| File | Reason left |
|------|-------------|
| `tools/verify_gate.py` | Described as "tamper-evident merge gate for fraud detection" — may be part of CI |
| `tools/test_verify_gate.py` | Test harness for verify_gate.py |
| `tools/symdb.py` | "Project-wide symbol/type/signature database" — may feed pipeline |
| `tools/convert_stubs.py` | ASM wrapper converter — may still be in use |
| `tools/convert_stubs_general.py` | General-purpose stub converter |
| `tools/convert_to_asm_wrappers.py` | ASM wrapper conversion |
| `tools/convert_waza_stubs.py` | Waza-specific stub converter |
| `tools/convert_waza_stubs_v2.py` | v2 of waza stub converter |
| `tools/debug_fixup.py` | Debug fixup utility |
| `tools/debug_fixup_result.txt` | Output of debug_fixup.py |
| `tools/conflicts_out.txt` | Conflict analysis output — may be recent |
| `tools/headless_subprocess.py` | ACTIVE PIPELINE — explicitly protected, left in place |

---

## Task 4 — Security Findings

Full details in `docs/SECURITY_TODO.md`. Summary:

### Finding 1: OpenRouter + DeepSeek keys in `openrouterkey.txt`

- **Location:** `openrouterkey.txt` (repo root)
- **Git status:** UNTRACKED — covered by `.gitignore` line 6. Never committed to history (confirmed via `git log --all -- openrouterkey.txt`).
- **Exposed keys:**
  - OpenRouter: `REDACTED_ROTATED_OPENROUTER_KEY`
  - DeepSeek: `REDACTED_ROTATED_DEEPSEEK_KEY`
- **Risk:** Keys are on disk. No git purge needed, but keys should be rotated as a precaution (file could be accidentally shared or pushed if `.gitignore` is bypassed).
- **Action:** Rotate both keys. See `docs/SECURITY_TODO.md` for exact steps.

### Finding 2: HF token path reference in tracked file

- **Location:** `tools/decomp_work/rl/train_sft.py` line 11 (TRACKED)
- **Content:** `HF_TOKEN=$(cat /storage/finetune/llm4decompile/hf_token)` — absolute path to token file on a remote machine.
- **Risk:** Low — path reference only, no literal token value committed. Token file security depends on permissions on the training server.
- **Action:** Ensure `/storage/finetune/llm4decompile/hf_token` is mode 600 on the training machine.

### Finding 3: `.gitignore` coverage

`openrouterkey.txt` was already covered by the pre-existing `.gitignore` pattern at line 6.
New patterns added in this pass additionally cover `*_key.txt`, `deepseek*.txt`, and `*key*.txt`
to catch future key files with similar naming.

---

## Protected Files — Confirmed Untouched

The following active pipeline files were verified to remain in place:

- `tools/progress.py`
- `tools/measure_cache.py`
- `tools/decomp_scheduler.py`
- `tools/decomp_agent.py`
- `tools/compile_check.py`
- `tools/match_test.py`
- `tools/quick_diff.py`
- `tools/assign_work.py`
- `tools/merge_results.py`
- `tools/headless_subprocess.py`
- `tools/configure*.py` / `configure.py`
- `objdiff.json` / `tools/objdiff.json`
- All `*.exe` tools
- All content in `src/`, `include/`, `config/`, `asm/`, `crt/`

No tracked files were deleted or moved. No `git rm`, `git mv`, `git commit`, or
history-rewriting commands were run.
