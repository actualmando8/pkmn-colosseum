# Fable 5 Type Recovery Campaign

This document is the operating guide for using Fable 5 on hard
decompilation targets. The goal is to turn wall functions into engineering
work: recover types, structs, callbacks, flags, and tables first; then use
that recovered model to match smaller functions and eventually larger ones.

The first campaign is the people / field people system because the active
tree already contains useful recovery scaffolding in `include/game/people/`.

## Non-Negotiable Rules

- Do not add, edit, stage, or commit `.inc` files.
- Do not count asm wrappers, inline asm, or included assembly as progress.
- Do not bulk-import archived campaign material into the active tree.
- Do not rename symbols, change splits, or flip object status without a
  concrete build/report reason.
- Preserve address traceability. Keep `fn_XXXXXXXX` in comments or adjacent
  declarations until the semantic name is well proven.
- Treat generated or historical `build/band_*` patches as evidence only. They
  are not accepted source of truth until revalidated against current source,
  headers, symbols, and report output.
- Before touching source, check `git status --short` and avoid unrelated dirty
  files.

## Baseline Workflow

Each Fable pass starts by refreshing local context from current repo truth:

```bash
git status --short
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
```

Use `build/GC6E01/report.json` as the active progress source. Do not use old
campaign metrics as current truth.

Useful read-only inputs:

- `config/GC6E01/symbols.txt`
- `config/GC6E01/splits.txt`
- `build/GC6E01/report.json`
- `build/GC6E01/asm/...` generated assembly
- committed headers under `include/`
- active source under `src/`
- historical `build/band_*` JSON files, only as untrusted hints
- archived material, only for comparison and attribution evidence

## Evidence Hierarchy

Use the strongest available evidence first:

1. Direct stores/loads in init, reset, copy, and allocator functions.
2. Callsite argument types and return value usage across multiple functions.
3. Strings, asserts, debug messages, and known SDK/HSD naming patterns.
4. Repeated stride/index patterns proving array element size.
5. Known external tools or community data, with the source named in a comment.
6. Historical patches or decompiler guesses, only after current evidence agrees.

Confidence levels:

- `A`: Proven by init/reset/copy plus at least one independent use.
- `B`: Proven by repeated callsites or a decisive string/assert/table.
- `C`: Likely shape/type, but semantic name is not proven.
- `D`: Placeholder only. Keep `field_XX`, `unk_XX`, or padding.

Only promote a field from `field_XX` to a semantic name at `A` or strong `B`.
For `C` and `D`, keep conservative names and record the uncertainty.

## Audit Ledger Format

Every type audit should produce a ledger before source changes. Keep it in the
Fable response or a follow-up doc section if the campaign is long-running.

Use this table format:

```markdown
| Struct | Size | Offset | Type | Name | Confidence | Evidence | Notes |
| --- | ---: | ---: | --- | --- | --- | --- | --- |
| PeopleFieldWork | 0x404 | 0x0F4 | s32 | entityID | A | fn_8014D000, fn_801557EC | -1 means free/unassigned |
```

For functions/prototypes:

```markdown
| Address | Proposed name | Prototype | Confidence | Evidence | Callers |
| --- | --- | --- | --- | --- | --- |
| fn_80144574 | peopleFieldSpawnMain | void (*)(PeopleFieldEntry*) | C | stores into PeopleFieldWork | TBD |
```

For enums/flags:

```markdown
| Name | Value/Mask | Confidence | Evidence | Meaning |
| --- | ---: | --- | --- | --- |
| PEOPLE_STATE_IDLE | 0 | B | switches on +0x54 | idle/default state |
```

## Fable Prompt: Type Audit

Use this prompt before asking Fable to match hard functions:

```text
You are auditing types, not trying to force a byte match yet.

Target:
- File:
- Function/address:
- Related headers:
- Related report unit:

Rules:
- Do not add asm, .inc files, or asm wrappers.
- Do not claim progress from wrappers or included assembly.
- Do not rename symbols or split ranges unless there is a concrete report reason.
- Preserve address traceability in comments.
- Treat build/band_* patches as untrusted hints only.

Tasks:
1. List every struct-like base pointer used by the target.
2. Build an offset table for each base pointer.
3. Identify init/reset/copy/allocation functions that prove field types.
4. Identify callbacks, function-pointer fields, enums, flags, tables, and strides.
5. Propose minimal typedef/prototype/header changes.
6. Keep unknown fields as field_XX or unk_XX unless semantic evidence is strong.
7. List smaller functions that should become easier after these types.
8. List open questions and the evidence needed to answer them.
```

## Fable Prompt: Implement Proven Types

Use this only after an audit ledger is reviewed:

```text
Implement only the proven type/prototype changes from this ledger.

Constraints:
- Keep edits scoped to headers and source needed by the audited subsystem.
- Keep field names conservative unless confidence is A or strong B.
- Preserve address comments for renamed functions.
- Do not edit .inc files.
- Do not add inline asm or wrappers.
- Do not touch unrelated dirty files.

After changes:
- Run python3 configure.py --no-progress.
- Run ninja all_source build/GC6E01/report.json.
- Run git diff --check.
- Summarize report impact and any remaining unmatched targets.
```

## Fable Prompt: Matching After Type Recovery

Use this only for small or near-miss targets:

```text
Use the recovered structs/types to match this function.

Target:
- File:
- Function/address:
- Size:
- Current fuzzy percent:

Rules:
- Prefer readable C using recovered types.
- No asm wrappers, inline asm, or .inc includes.
- Do not broaden the patch beyond this target unless a prototype/header fix is
  required by the recovered type model.
- If the match appears blocked by an unproven field or bad prototype, stop and
  return a type-audit note instead of guessing.

Validation:
- Build the smallest target available.
- Then run ninja all_source build/GC6E01/report.json.
- Report exact command failures if any.
```

## People-First Campaign

Primary files:

- `include/game/people/people.h`
- `include/game/people/people_field.h`
- `src/game/people/people_data.c`
- `src/game/people/people_field.c`

Supporting files:

- `include/game/script/script.h`
- `include/game/gs_thread.h`
- `include/game/gs_model.h`
- `include/game/world/gs_field.h`
- `config/GC6E01/symbols.txt`
- `build/GC6E01/asm/game/people/people_field.s`

Primary recovered structs to audit:

- `PeopleEntry`, size `0xDC`
- `PeopleOpenWork`
- `PeopleFieldEntry`, size `0x28`
- `PeopleFieldWork`, expected size `0x404`

First audit functions:

- `fn_80144574` / `peopleFieldSpawnMain`: large spawn/setup function.
- `fn_8014D000` / `peopleFieldSystemInit`: allocation/init evidence.
- `fn_801557EC` / `peopleFieldResetState`: reset/default field evidence.
- `fn_8015B250` / `peopleFieldScriptMain`: script state and command evidence.
- `fn_801603C0` / `peopleFieldMotionUpdate`: motion field evidence.
- `fn_80162A58` / `peopleFieldMoveApply`: position/motion arguments.
- `fn_80164DD0` / `peopleFieldAnimInterp`: animation interpolation fields.

Do not start by rewriting `fn_80144574` or `fn_8015B250`. Start by extracting
field evidence from them.

### People Pass 1: Confirm Layouts

For each people struct:

- Confirm total size from allocation stride, array indexing, or memset/copy.
- Confirm pointer fields by callsite use.
- Confirm scalar width by load/store instruction width.
- Confirm signedness only when comparisons, sign-extension, or API use proves it.
- Confirm float fields only when used by float ops or passed to float APIs.
- Split padding from unknown fields only when access boundaries prove it.

Expected outputs:

- Updated ledger for `PeopleEntry`.
- Updated ledger for `PeopleFieldEntry`.
- Updated ledger for `PeopleFieldWork`.
- List of fields whose current names are overconfident and should be demoted.
- List of fields whose semantic names are now justified.

### People Pass 2: Prototypes and Callbacks

Audit function signatures around the people system:

- Identify which functions take `PeopleEntry*`, `PeopleFieldEntry*`, or
  `PeopleFieldWork*`.
- Identify callback fields and their call signatures.
- Identify script command handlers and command byte/enums.
- Identify model, motion, floor, and script API boundaries.

Expected outputs:

- Prototype ledger.
- Callback signature ledger.
- Minimal header changes required before matching.

### People Pass 3: Tables, Flags, and Enums

Audit:

- state fields
- motion type fields
- animation bank/slot fields
- flags and bitmasks
- script command values
- array/table strides
- global pointers and counts

Use conservative enum names until strings or clear behavior justify stronger
names.

Expected outputs:

- Enum/flag ledger.
- Table/stride ledger.
- Candidate constants for headers.

### People Pass 4: Small Matching Targets

Only after the relevant types are audited, attack small targets first.

Initial people-field candidates from the current report snapshot:

- `fn_801631AC`: 20 bytes, high fuzzy.
- `fn_801628C8`: 60 bytes, high fuzzy.
- `fn_80162FB0`: 92 bytes, high fuzzy.
- `fn_80162EB8`: 144 bytes, high fuzzy.
- `fn_80162370`: 184 bytes, high fuzzy.

Also consider `people_data.c` accessors/allocation helpers once `PeopleEntry`
and `PeopleFieldEntry` are validated, because they have many callers and help
propagate types.

Avoid these until later:

- `fn_80144574`
- `fn_801557EC`
- `fn_8015B250`
- `fn_801603C0`

Those are evidence sources first and matching targets later.

## General Hard-Function Campaign Order

After people-first has produced usable types, apply the same pattern to other
walls:

1. `people_field`: runtime NPC/work/script state.
2. `colosseum_event`: high fuzzy, many functions, likely benefits from event
   context and table recovery.
3. `pokemon` and `trainer`: high fuzzy and many data-table relationships.
4. `gs_field_world`: large but many matched functions; needs world/floor
   context structs.
5. `gs_render` and HSD render objects: type-heavy, best after HSD headers are
   audited.
6. `battle_waza` and `colosseum_battle`: defer until battle Pokemon, move,
   effect, and script data models are stronger.
7. `hsd_texp`: defer unless doing a dedicated HSD texture expression campaign.

## Source Change Rules

When implementing recovered types:

- Prefer updating existing headers over adding local duplicate typedefs.
- If a type is only proven locally, keep it local until multiple files need it.
- Keep unknown bytes explicit as padding or `field_XX`.
- Do not change struct packing assumptions without size evidence.
- Do not rename a function unless it improves callsite clarity and evidence is
  recorded.
- Keep function address comments on nontrivial renames.
- Avoid sweeping formatting or unrelated cleanup.

When matching:

- Prefer one function or a small family of repeated functions per patch.
- If a match requires a speculative field name, keep the field generic.
- If a match requires a questionable prototype, stop and audit the prototype.
- If a patch only works with inline asm or wrappers, reject it as progress.

## Validation Gates

For documentation-only changes:

```bash
git diff --check
```

For type/header/source changes:

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
git diff --check
```

For matching/source progress:

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
ninja
python3 configure.py progress
git diff --check
```

If any command cannot run, report the exact command and why.

## Acceptance Criteria

A type audit pass is successful when:

- It produces a ledger with confidence levels and evidence functions.
- It identifies at least one smaller matching target enabled by the recovered
  type information.
- It avoids speculative semantic names.
- It does not edit `.inc` files or add assembly wrappers.

A source/type implementation pass is successful when:

- The report regenerates.
- The edited types are used by real source or unlock clear follow-up targets.
- The patch is scoped to the audited subsystem.
- Existing unrelated dirty files are untouched.

A hard-function campaign is successful when:

- Raw pointer arithmetic decreases over time.
- Shared prototypes become more accurate.
- Small functions start matching before giant functions are attempted.
- The work produces reusable type knowledge even when a specific function does
  not match immediately.

## Fable Handoff Format

Each Fable pass should end with:

```markdown
## Summary
- What was audited or matched.

## Proven
- Fields, prototypes, enums, flags, or tables now proven.

## Changed Files
- Files changed, or "none" for audit-only.

## Validation
- Commands run and results.

## Next Targets
- Specific functions or structs for the next pass.

## Open Questions
- Unknowns blocking the next match.
```

## Current First Task For Fable

Start with an audit-only pass:

```text
Audit `PeopleFieldWork` in `include/game/people/people_field.h`.

Use these evidence functions:
- fn_8014D000 / peopleFieldSystemInit
- fn_801557EC / peopleFieldResetState
- fn_8015B250 / peopleFieldScriptMain
- fn_801603C0 / peopleFieldMotionUpdate

Do not edit source yet.
Return a field-offset ledger for PeopleFieldWork, identify overconfident field
names, identify fields safe to promote, and list the smallest people_field
functions that should be tried after the type audit.
```

## Subagent Audit Pilot, 2026-07-01

Four low-cost audit agents checked independent files for mechanical type-recovery
targets. Their output is audit evidence, not automatic approval. Apply only the
rows marked safe after a local review and focused build/report validation.

| Area | Target | Action | Confidence | Notes |
| --- | --- | --- | --- | --- |
| `gs_field_resource.c` | archive buffers with payload at `+0x60` | Applied locally as `HSDArchiveBuffer` with `payload` field | B | Removes repeated archive payload pointer arithmetic; touched functions remained 100% matched. |
| `people_data.c` | `PeopleFieldEntry` table, stride `0x28` | Safe next | B | Existing typedef and comments already prove the shape; use typed indexing and `sizeof(PeopleFieldEntry)` where raw slot math remains. |
| `people_data.c` | `ItemParamConvertEntry`, stride `0x10` | Already typed | B | No source change needed from this audit. |
| `gs_party_access.c` | local evolution call payload | Safe next | B | Name the anonymous local struct and fields while preserving layout. |
| `gs_party_access.c` | scene key/value table copies | Safe next | B | Use pair structs for the `lbl_80266700` and `lbl_802666E0` stack copies; names should stay generic key/value unless stronger evidence appears. |
| `effect_util.c` | `gsEffectGlobals.instanceTable` access | Safe next | B | Header already defines `GSEffectGlobals` and `GSEffectInstance`; replace hand-walked `0x34` instance math in `fn_8013151C`. |
| `effect_util.c` | small trace/link tables | Queue after callsite review | C | Shapes are plausible, but field names should stay generic (`links`, `kind`, `value0`) until more consumers are checked. |
| `gs_field_resource.c` | `scene_data` root walk | Defer | C | Needs a second pass to prove whether the entries share one layout. |
| `people_data.c` | unknown `0x10` and `0x20` stride tables | Defer | D | Keep raw wrappers or byte arrays until field accesses prove meaning. |
| `effect_util.c` | scene slot / entry blocks | Defer | D | Nested `0x24a4` and `0x138` structures are high leverage but too broad for a mechanical patch. |

Pilot rule: subagents may collect candidate structs, strides, and evidence in
parallel, but a main pass must still review names, apply the patch, and validate
with the standard report before committing.

## Subagent Extrapolation Batch 1, 2026-07-01

Applied after local review:

- `effect_util.c`: `fn_8013151C` now uses `GSEffectGlobals` and
  `GSEffectInstance` for the effect instance table instead of raw `0x34` slot
  math. Focused match stayed `95.0%`.
- `people_data.c`: `peopleFieldGetByIndex` now returns from a typed
  `PeopleFieldEntry[]` global instead of `slot * 0x28` byte indexing. Unit
  metrics stayed unchanged.
- `gs_party_access.c`: `testEvolution__Fv` now names its call payload
  `GSpartyEvolutionArgs`; `fn_8000CF68` now uses conservative `key`/`value`
  pairs for the scene resource lookup. Focused matches stayed `95.84%` and
  `100.0%`.

Rejected from this batch:

- `fn_8000D05C` pair-struct rewrite. It made the C more readable but changed
  instruction ordering, so keep the raw `u32 table[8]` form until a better
  matching-preserving structure is found.

## Subagent Extrapolation Batch 2, 2026-07-01

Applied after local review:

- `effect_util.c`: added conservative local row types for the linked status
  table and trace lookup tables: `EffectLinkedStatusRow`,
  `EffectTraceFxEntry`, and `EffectTraceEntry`.
- Converted `fn_80135F58`, `fn_80136368`, `fn_801363A8`, `fn_801363E8`,
  `fn_80136428`, and `fn_80136468` from byte-stride pointer math to typed
  table indexing and field access.

Validation result:

- All six focused objdiff checks stayed `100.0%`.
- `main/game/effect/effect_util` unit metrics stayed unchanged.

## Subagent Extrapolation Batch 3, 2026-07-01

Applied after local review:

- `effect_util.c`: introduced `EffectParamBlock` with conservative `field_XX`
  names for the compact parameter block used by the `fn_80135A30` through
  `fn_80135C78` accessor cluster.
- Replaced raw offset reads/writes at `0x00..0x03`, `0x08`, `0x0C`, `0x14`,
  `0x18`, `0x1C`, and `0x20..0x22` with field access. The first word remains a
  union because some functions treat it as individual bytes and another stores a
  whole word.

Validation result:

- Focused objdiff for the exact-match setters/getters stayed `100.0%`.
- Lower-match default getters stayed at their previous report percentages.
- `main/game/effect/effect_util` unit metrics stayed unchanged.
