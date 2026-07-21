# Claude Handoff - GC6E01 DTK Reset + Object-Map Campaign

> Historical reset snapshot. For the current decompilation campaign restart,
> worktree reconciliation, farm refresh, strict-win audit, integration, and
> closeout procedure, use [`CAMPAIGN_OPERATIONS.md`](CAMPAIGN_OPERATIONS.md).
> Metrics below describe the 2026-06-30 reset and are not current progress.

Date: 2026-06-30  
Repo: `/Users/douglaswhittingham/pkmn-colosseum`  
Target: Pokemon Colosseum `GC6E01`

## Current Mission

This repo has been reset to the standard `encounter/dtk-template` workflow.
Treat the old decomp pipeline as archived research only. The immediate goal is
to stabilize and improve the DTK object map without inflating decompilation
progress.

The next campaign is object mapping:

1. Retire anonymous `auto_...` DTK units into named split objects.
2. Keep the code/data/function denominator stable.
3. Do not count named extracted units as decompiled source.
4. Only later convert named/source-backed units into real byte-matched C.

## Hard Rules

- Do not edit, add, stage, or commit `*.inc` files.
- Do not use asm wrappers, inline assembly, or included assembly as decomp wins.
- Do not bulk-import old campaign files into active source.
- Do not use the previous campaign's metrics as current truth.
- Do not change `splits.txt`, `symbols.txt`, or `configure.py` without rebuilding
  `build/GC6E01/report.json` and updating the freeze manifest when topology
  intentionally changes.
- Use `python3`, not `python`, on this Mac.

## Current Truth Metrics

Authoritative report:

```bash
build/GC6E01/report.json
```

Current report after the first conservative object-map reclaim:

```text
total_code:            2,495,108
matched_code:              3,496
matched_code_percent:      0.14011417

total_data:            2,196,256
matched_data:             12,364
matched_data_percent:      0.56295806

total_functions:           8,603
matched_functions:           110
matched_functions_percent:   1.2786237

total_units:                 220
auto_units:                   47
named_units:                 173
source_backed_units:         134
auto_functions:            8,206
named_no_source_functions:   262
source_backed_functions:     135
```

Interpretation:

- `matched_functions` is real source byte-match progress.
- `auto_functions` means functions still in anonymous DTK auto chunks.
- `named_no_source_functions` means object-map progress only: the range is named
  in `splits.txt`, but still comes from the extracted target object.
- `source_backed_functions` means the unit has a source path; it may or may not
  byte-match.

The buckets add up:

```text
8,206 auto
+ 262 named extracted/no source
+ 135 source-backed
= 8,603 total functions
```

## Freeze Guard

New files:

- `tools/check_object_map_freeze.py`
- `config/GC6E01/object_map.freeze.json`

The freeze locks:

- total code bytes
- total data bytes
- total function count
- unit section topology

It intentionally allows progress counters to improve. If split topology changes,
update the freeze in the same change:

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
python3 tools/check_object_map_freeze.py --update
cp build/GC6E01/report.json report.json
python3 tools/check_object_map_freeze.py
```

CI now runs the freeze check after generating the report.

## Current Uncommitted Work

As of this handoff, the worktree contains the freeze/remap campaign changes:

- `.github/workflows/build.yml`
- `README.md`
- `config/GC6E01/splits.txt`
- `report.json`
- `config/GC6E01/object_map.freeze.json`
- `tools/check_object_map_freeze.py`
- `docs/CLAUDE_HANDOFF.md`

Do not use `git add -A`. Stage specific files only.

## Remap Campaign So Far

The first conservative pass used archived `KNOWN` attribution from:

```text
archive/previous_campaign/config/GC6E01/func_tu_map.json
archive/previous_campaign/config/GC6E01/splits_refined.txt
archive/previous_campaign/config/GC6E01/symbolmap/xd_port.json
archive/previous_campaign/config/GC6E01/symbolmap/xd_port.md
```

Applied only ranges that were cleanly attributable and did not require source
linking. Examples added to `splits.txt`:

- `dolphin/os/OSContext.c`
- `dolphin/os/OSInterrupt.c`
- `dolphin/os/OSThread.c`
- `dolphin/db/DB.c`
- `dolphin/dvd/DVDLow.c`
- `dolphin/dvd/DVDError_ErrorCode2Num.c`
- `trk/TRKDispatch_range_*`
- `trk/TRKTarget_range_*`
- `trk/TRKInterrupt.c`
- `crt/printf.c`
- `crt/stdio_range_*`
- `hsd/hsd_mobj_range_*`
- `hsd/hsd_pobj_range_*`

Result:

```text
auto_units:      77 -> 47
auto_functions: 8468 -> 8206
auto_code:       2,477,064 -> 2,400,120
matched funcs:   unchanged at 110
matched code:    unchanged at 3,496
```

That is correct. Mapping progress should not inflate decomp progress.

## Remaining Object-Map Opportunity

Current remaining archive evidence inside auto chunks:

```text
Exact-source KNOWN runs still in auto:
  game:      4,742 funcs / 1,186,136 bytes
  hsd:         312 funcs /   103,452 bytes
  dolphin:     188 funcs /    38,632 bytes
  crt:          19 funcs /    10,504 bytes
  trk:           5 funcs /       924 bytes
  total:     5,266 funcs / 1,339,648 bytes

LIKELY runs needing review:
  total:     1,110 funcs /   396,876 bytes
```

Recommended next remap pass:

1. Import exact `KNOWN` HSD + Dolphin + CRT/TRK runs.
2. Skip generic `src/dolphin/gx/` attribution until exact GX TU names are known.
3. Skip gap-heavy game chunks until reviewed in smaller batches.
4. Then import exact `KNOWN` game ranges in separate PR-sized groups.
5. Then use XD/string evidence to promote high-confidence symbol names in
   `symbols.txt`.

Largest remaining auto chunks:

```text
main/auto_01_801B0158_text  code=745,988 funcs=2,824
main/auto_01_800D1070_text  code=607,052 funcs=2,490
main/auto_01_800055E0_text  code=300,284 funcs=756
main/auto_01_80051710_text  code=289,232 funcs=615
main/auto_01_801653CC_text  code=274,524 funcs=794
main/auto_01_800AA4D4_text  code=52,508  funcs=166
```

Do not bulk-import those large game chunks without a per-range audit.

## Validation Commands

Use these after any mapping/source change:

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
python3 tools/check_object_map_freeze.py
python3 tools/check_object_map_freeze.py --strict-source-status
python3 tools/check_object_map_freeze.py --report report.json
python3 configure.py progress
ninja
python3 -m py_compile tools/check_object_map_freeze.py
git diff --check
```

Expected `ninja` result:

```text
build/GC6E01/main.dol: OK
```

The linker emits many floating-point setting warnings for extracted objects.
Those warnings are currently expected; the SHA check is the important gate.

## How To Audit More Candidates

Use this pattern to inspect remaining auto chunks against archive evidence:

```bash
python3 - <<'PY'
import json
from pathlib import Path
from collections import Counter

report = json.loads(Path("build/GC6E01/report.json").read_text())
ftu = json.loads(Path("archive/previous_campaign/config/GC6E01/func_tu_map.json").read_text())

funcs = []
for name, rec in ftu.items():
    try:
        addr = int(rec["addr"], 16)
        size = int(rec.get("size", "0"), 16)
    except Exception:
        continue
    funcs.append((addr, addr + size, name, rec.get("src") or "UNKNOWN", rec.get("status") or "UNKNOWN"))
funcs.sort()

for unit in report["units"]:
    if not unit.get("metadata", {}).get("auto_generated"):
        continue
    sections = unit.get("sections", [])
    if not sections or sections[0]["name"] not in [".text", ".init"]:
        continue
    start = int(sections[0]["metadata"]["virtual_address"])
    end = start + int(sections[0]["size"])
    found = [rec for a, b, _name, src, status in funcs if start <= a and b <= end for rec in [(src, status)]]
    print(unit["name"], Counter(found).most_common(8))
PY
```

For a split to be safe in this campaign:

- Every function in the proposed run should be same source/status.
- Prefer `status == "KNOWN"`.
- Avoid `src == "UNKNOWN"`.
- Avoid folder-only `src/dolphin/gx/` until refined.
- Preserve address order in `config/GC6E01/splits.txt`.
- Rebuild/report/freeze immediately after edits.

## Archived Data Use Policy

Archive data is evidence, not truth.

Good uses:

- propose split ranges
- corroborate names
- find XD/string-backed symbol names
- prioritize source-backed decomp work

Bad uses:

- restoring old metrics
- importing old source blindly
- using old wrappers/incs
- treating old object names as canonical without DTK validation

## What To Do Next

If continuing object mapping:

1. Commit the current freeze/remap batch first if it is not already committed.
2. Generate a second-batch audit for exact `KNOWN` HSD/Dolphin/CRT/TRK runs.
3. Patch `splits.txt` in small groups.
4. Rebuild `report.json`.
5. Update `object_map.freeze.json` only after successful report/build.
6. Keep `matched_code` and `matched_functions` honest; they should not change
   from mapping-only work.

If switching to decompilation:

1. Pick a source-backed unit from `configure.py`.
2. Use `objdiff`/report to find a near miss.
3. Make real C changes only.
4. Validate with the DTK report and `ninja`.
