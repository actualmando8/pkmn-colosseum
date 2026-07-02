# symbolmap - evidence-driven naming support for GC6E01

`tools/symbolmap` is a naming-evidence pipeline for Pokemon Colosseum
(`GC6E01`). It is not the symbol map itself. The scripts in this directory mine
the DOL disassembly, compare it with Pokemon XD where available, and write
reviewable naming artifacts under:

```text
config/GC6E01/symbolmap/
```

Use this folder when you want better names for `fn_XXXXXXXX` functions, want to
confirm which translation unit a range came from, or want a safer way to decide
whether a proposed name is strong enough to adopt.

The important rule: this pipeline is advisory by default. It can generate
`// Proposed:` comments and rename maps, but the normal `run.py` pipeline does
not edit `symbols.txt`, source files, asm, or `.inc` truth files.

> **Ported 2026-07** from `archive/previous_campaign/tools/symbolmap` for the
> current dtk-template-style tree. The evidence-mining *logic* below
> (mine_xrefs / attribute_tus / propose_names / port_xd / structural_port /
> build_symbol_map) is unchanged from the archived campaign - only the
> asm-tree layout and splits-file format changed, so two small adapters were
> added: `convert_splits.py` and `salvage_xd_archive.py`. See "What changed
> in this port" below.

## Why This Helps Naming Work

The game contains debug/assert strings even though it has no normal source-level
symbol table. Those strings give strong naming evidence:

- CodeWarrior `__FILE__` literals such as `menuCardE_Matrix.c` prove the source
  file for any function that references them.
- Assert/log strings often contain the real function name, for example
  `_menuPop():stack under.` or `GSmaterialCreate: ...`.
- Pokemon XD (`GXXE01`) shares much of the same engine. If Colosseum and XD
  functions share rare string literals or identical instruction structure, XD's
  decomp name can be a high-value naming lead.

This is especially useful during decomp work because names become evidence
records rather than guesses.

## What changed in this port

The archived campaign's asm tree was flat: every dtk-emitted file used the
`auto_NN_ADDR_section.s` naming convention, and scripts globbed on that
prefix. The live tree's asm (`build/GC6E01/asm/**/*.s`) is organized per
translation unit under subdirectories (`game/`, `hsd/`, `dolphin/`, `crt/`,
...), and most files are named after their source unit, not `auto_*`. Only
the still-unattributed GAP regions keep the `auto_NN_ADDR_section.s` form.

| change | why |
|---|---|
| `mine_xrefs.py` / `structural_port.py` scan `asm_dir.rglob("*.s")` and track the active section from the `.text`/`.rodata`/`.section .sdata2, "a"` directives themselves, instead of matching filenames | strings and functions now live in named per-unit files too, not just `auto_*.s` |
| `convert_splits.py` (new) | the live tree uses dtk-template `config/GC6E01/splits.txt` (`unit.c:` blocks with `.text start:0x.. end:0x..` rows), not the archived `splits_refined.txt` three-state `KNOWN\|LIKELY\|GAP 0xSTART 0xEND src` row format `attribute_tus.py` expects. This script converts one to the other by treating dtk's own auto-named placeholder units (`*_range_ADDR.c`, `*_fn_ADDR.c`) as GAP and everything else as KNOWN. `attribute_tus.py` itself is unmodified. |
| `port_xd.py` | now degrades gracefully (skips VALIDATE/PORT with a message) instead of crashing when `--xd-symbols` doesn't exist, so `run.py` stays turnkey without live XD data |
| `salvage_xd_archive.py` (new) | no XD (GXXE01) disc/asm is available in this tree. The archived campaign's `xd_port.json` / `structural_ports.json` are still valid evidence (they're keyed by Colosseum's own, unchanged, DOL addresses) - this script re-validates every archived proposal against the *current* `symbols.txt` and classifies it `still-open` / `already-applied` / `already-conflict` / `address-not-found`. Only `still-open` entries feed back into the native `xd_port.json`/`structural_ports.json` outputs. |
| `run.py` | wires `convert_splits.py` and `salvage_xd_archive.py` into the stage order; default `--splits` now points at `config/GC6E01/splits.txt` |

`apply_names.py`, `partition_apply.py`, `apply_tu_attribution.py`,
`wire_leads.py`, `finalize_leads.py`, `regen_named_incs.py`, and
`xd_config.mine.yml` were carried over unmodified (not exercised by this
port - they operate on the same native `config/GC6E01/symbolmap/` output
files regardless of how those files were produced). `renaming_dashboard.py`
and the `.ps1` launchers were **not** ported (GUI/Windows tooling, not part
of the `run.py` advisory pipeline).

## Inputs

| input | purpose |
|---|---|
| `build/GC6E01/asm/**/*.s` | dtk symbol-resolved Colosseum asm, recursively. Build with `python configure.py && ninja` if absent. |
| `config/GC6E01/symbols.txt` | Current project symbol names and existing `// Proposed:` comments. |
| `config/GC6E01/splits.txt` | Current address-range to source-file map (dtk-template format; auto-converted by `convert_splits.py`). |
| `archive/previous_campaign/config/GC6E01/symbolmap/{xd_port,structural_ports,xd_vocabulary}.json` | Salvaged real-XD evidence from when the previous campaign had a live XD split (no disc committed here - see below). |
| XD asm, optional | Enables stronger *live* name ports from Pokemon XD, once wired up. |
| XD `symbols.txt`, optional | Provides real XD names to port from, once wired up. |

## Outputs

Most useful generated/review files live in `config/GC6E01/symbolmap/`.

| output | what it means | how to use it |
|---|---|---|
| `strings.json` | All mined string objects from dtk asm. | Raw input for other passes. |
| `fn_strings.json` | Function to referenced-string map. (gitignored, regenerate with `run.py`) | Raw evidence for a single function's string refs. |
| `splits_compat.txt` | `convert_splits.py`'s KNOWN/GAP view of `splits.txt`. (gitignored) | Input to `attribute_tus.py`; inspect if a range's status looks wrong. |
| `tu_attribution.md` | Human-readable source-file attribution from `*.c` strings. | Check whether a range belongs to the file we think it does; also proposes TU labels for whole GAP ranges. |
| `tu_evidence.json` | Machine-readable TU attribution evidence. | Feed follow-up scripts or custom analysis. |
| `name_proposals.md` | Names mined from self-identifying log/assert strings. | Review proposed names and their exact string evidence. |
| `name_proposals_high.txt` | High-confidence self-name proposals. | Good source for `// Proposed:` comments. |
| `xd_port.md` / `xd_port.json` | XD-to-Colosseum ports/validations - live (if `--xd-asm`/`--xd-symbols` supplied) merged with salvaged archive evidence. | Stronger than local self-name when unambiguous. |
| `xd_vocabulary.json` | Names seen in XD (salvaged from the archive; address-independent). | Search when trying to match naming style. |
| `proposed_symbols.txt` | Merged high-confidence rename proposal map (XD port + string self-name only). | Main review file: `fn_OLD -> NewName // provenance`. |
| `symbols.with_proposals.txt` | Copy of `symbols.txt` with proposed renames applied. (gitignored) | Diff against `symbols.txt` before adopting. |
| `structural_ports.json` | XD ports by instruction-mnemonic fingerprint, salvaged from the archive and revalidated (`still-open` entries only). | Naming leads; verify before adopting. |
| `structural_port_salvage.md` | Full salvage breakdown (still-open / already-applied / already-conflict / not-found) with samples. | Audit trail for the structural-port salvage; also a good sanity check on archived-evidence quality. |

## Evidence Strength

Treat evidence tiers roughly like this:

| tier | signal | confidence |
|---|---|---|
| Strong | XD shared-string port, unambiguous, high score. | Often safe as a proposed name; still verify context. |
| Strong | Unique self-identifying assert/log string. | Good for `// Proposed:` and often for real rename after compile checks. |
| Medium | Structural XD port (salvaged), identical mnemonic fingerprint. | Useful lead, but verify behavior and surrounding names. |
| Medium | TU attribution from `__FILE__`. | Strong file evidence, but not usually a function name by itself. |
| Weak | Shared format string, generic prefix, or broad string fanout. | Treat as a clue only. |
| Weak | XD vocabulary-only match (name exists in XD engine but no address-level evidence tying it to a specific Colosseum function). | Style/spelling reference only. |

## Running The Pipeline

From the repository root, with the Colosseum asm already built
(`python configure.py && ninja`, or it already exists in `build/GC6E01/asm`):

```sh
python tools/symbolmap/run.py
```

`run.py` runs these stages:

1. `mine_xrefs.py` - mine strings and function string references.
2. `convert_splits.py` - adapt `splits.txt` to the row format `attribute_tus.py` expects.
3. `attribute_tus.py` - map functions/ranges to source files via `__FILE__`.
4. `propose_names.py` - mine self-identifying log/assert names.
5. `port_xd.py` - port XD names live, if `--xd-asm`/`--xd-symbols` are wired up (no-op otherwise).
6. `salvage_xd_archive.py` - revalidate the archived campaign's XD/structural evidence against current addresses (skip with `--skip-salvage`).
7. `build_symbol_map.py` - merge high-confidence evidence into review maps.

Use explicit paths if you are working from a different build/output tree:

```sh
python tools/symbolmap/run.py \
  --asm-dir build/GC6E01/asm \
  --out-dir config/GC6E01/symbolmap \
  --symbols config/GC6E01/symbols.txt \
  --splits config/GC6E01/splits.txt
```

## Enabling A Live XD Port

The XD port needs XD's `main.dol` and an XD asm split. This tree already has
a pinned dtk binary at `build/tools/dtk` (see `configure.py`, `dtk_tag =
"v1.8.3"`). From a disc image:

```sh
# 1. Convert rvz to iso if needed (Dolphin tooling, any platform build).
dolphin-tool convert -i XD.rvz -o xd.iso -f iso

# 2. Extract main.dol. Run dtk from the iso directory because drive-letter
#    colons collide with dtk's container separator on Windows.
cd <dir-with-xd.iso>
<repo>/build/tools/dtk vfs cp "xd.iso:sys/main.dol" xd_main.dol

# 3. Split with the mining config.
mkdir -p <repo>/tools/symbolmap/xd_ref/GXXE01
cp <repo>/tools/symbolmap/xd_config.mine.yml <repo>/tools/symbolmap/xd_ref/GXXE01/config.mine.yml
cp <path-to>/xd_main.dol <repo>/tools/symbolmap/xd_ref/GXXE01/orig/GXXE01/sys/main.dol
# also place XD's own config/GXXE01/symbols.txt at
#   <repo>/tools/symbolmap/xd_ref/GXXE01/symbols.txt
cd <repo>/tools/symbolmap/xd_ref/GXXE01
<repo>/build/tools/dtk dol split config.mine.yml _xdsplit

# 4. Run with XD asm enabled.
cd <repo>
python tools/symbolmap/run.py \
  --xd-asm tools/symbolmap/xd_ref/GXXE01/_xdsplit/asm \
  --xd-symbols tools/symbolmap/xd_ref/GXXE01/symbols.txt
```

Expected XD `main.dol` SHA1 (from the archived campaign's config):

```text
ff9e752ead9914af0b363ae6c831a34ccce189d2
```

`tools/symbolmap/xd_ref/` is a suggested, gitignore-friendly convention (not
committed) - point `--xd-asm`/`--xd-symbols` anywhere you like.

## Tool Index

| script | role |
|---|---|
| `run.py` | One-command driver for the standard string/XD/salvage pipeline. |
| `mine_xrefs.py` | Extracts `strings.json` and `fn_strings.json` from dtk asm (recursive, section-directive-aware). |
| `convert_splits.py` | **New.** Adapts `config/GC6E01/splits.txt` to the KNOWN/GAP row format `attribute_tus.py` expects. |
| `attribute_tus.py` | Generates TU/source-file attribution evidence. Unmodified from the archive. |
| `apply_tu_attribution.py` | Manually curated updater for a `splits_refined.txt`-style file; review before use. |
| `propose_names.py` | Mines names from self-identifying strings. Unmodified from the archive. |
| `port_xd.py` | Ports names from a *live* XD split using shared string evidence; skips gracefully without one. |
| `structural_port.py` | Ports names from a *live* XD split using identical mnemonic fingerprints (recursive glob, otherwise unmodified). |
| `salvage_xd_archive.py` | **New.** Revalidates the archived campaign's `xd_port.json`/`structural_ports.json` against current `symbols.txt` addresses when no live XD data is available. |
| `build_symbol_map.py` | Merges high-confidence proposals into `proposed_symbols.txt`. Unmodified from the archive. |
| `partition_apply.py` | Splits proposals into mechanical renames vs. wiring-needed leads. |
| `apply_names.py` | Applies an approved rename map to symbols/source. Use carefully. |
| `wire_leads.py` | Handles a narrow class of return-type-only lead wiring. |
| `finalize_leads.py` | Refreshes applied/lead records after wiring. |
| `regen_named_incs.py` | Regenerates `.inc` files for already-renamed functions. |

## Applying Names Safely

Mechanical application is intentionally separate from evidence generation.

```sh
python tools/symbolmap/partition_apply.py
python tools/symbolmap/apply_names.py \
  --map config/GC6E01/symbolmap/applied_symbols.txt \
  --symbols --source
```

Before applying:

- Read `proposed_symbols.txt` (APPLY-SAFE band: XD port / string self-name only).
- Treat `structural_ports.json` / `structural_port_salvage.md` as a REVIEW band - verify each one, don't bulk-apply.
- Diff `symbols.with_proposals.txt` against `symbols.txt`.
- Check for typed prototypes or source declarations that would conflict.
- Prefer a `// Proposed:` comment when evidence is good but typing is not done.
- Run compile checks and focused `match_test.py` after edits.

Why this is byte-safe when done correctly: the byte-match build is based on
dtk-extracted asm objects, and the DOL has no symbol table. A rename by itself is
byte-neutral. Source-level typing changes are still real decomp work and must be
verified normally.

## Regenerating `.inc` After A Rename

```sh
ninja build/GC6E01/ok
python tools/symbolmap/regen_named_incs.py --all
```

## Practical Guidance For Current Work

- Do not rename the whole project to chase one function.
- Use `symbols.txt` `// Proposed:` comments for strong local findings.
- Use `tu_attribution.md` to confirm file/range ownership, and its
  "GAP-range attribution proposals" section to find whole unattributed
  regions worth splitting into a named TU.
- Use `name_proposals.md`, `xd_port.md`, and `structural_ports.json`/
  `structural_port_salvage.md` as leads, then verify against code, offsets,
  call signatures, and objdiff/match output.

That makes this folder a research aid and audit trail for naming, not a license
to bulk-rename unknown code.
