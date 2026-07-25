# Decompilation campaign operations

This is the restart, reconciliation, farm, and integration playbook for the
`GC6E01` campaign. It records the operational decision framework and commands;
it is not a transcript of any agent's private reasoning.

Read `AGENTS.md` first. Its guardrails override this document.

## Definitions that must not be mixed

- **Exact function:** the active source emits the retail function at 100%.
- **Matched progress:** exact source inside either a complete object or an
  incomplete `CodeCandidate` object.
- **Linked progress:** a complete object is admitted as `Matching` and linked
  into the rebuilt DOL. A 100% function inside an incomplete object is not yet
  linked progress.
- **Farm win:** score zero in an isolated work unit. This is only a proposal
  until the live translation unit, data, relocations, full link, and quality
  gates pass.
- **Strict campaign win:** natural, semantically faithful source that passes all
  gates without code-generation shaping.

Track exact-source and linked deltas separately. If the goal says "linked",
only functions in newly completed objects count toward it.

## Strict acceptance policy

Accept only source that is plausible program logic and preserves observable
semantics. Known SDK idioms are useful evidence, but still require the normal
build and section checks.

Reject a result when exactness depends on any of the following:

- an edited, added, staged, or committed `.inc` file;
- asm wrappers, inline asm, or included assembly, except the existing narrow
  Dolphin paired-single allowlist;
- local `optimization_level`, `optimize_for_size`, `scheduling`, `peephole`,
  `opt_propagation`, or similar compiler-control pragmas;
- dummy/self assignments, uninitialized reads, volatile added only to color
  registers, artificial structs/unions, pointer aliases, or integer aliases;
- invented inline helpers, comma expressions, labels/gotos, or temporaries used
  only to manipulate allocation or scheduling;
- changed null, range, signedness, overflow, evaluation-order, or side-effect
  behavior;
- a text-only match whose constants, jump tables, data alignment, relocations,
  or sibling functions do not pair in the live object;
- aliases, synthetic compiler data, guessed semantics, or position-dependent
  output that cannot occupy the retail section naturally.

Legacy candidate wrappers can remain research evidence, but a promoted
`Matching` source file must stand on its own without forbidden shaping.

## Resume checklist

Run from the repository root.

```bash
git status --short --branch
git fetch --prune origin
git log -5 --oneline --decorate
git worktree list --porcelain
gh pr list --state open --limit 30
```

Do not pull, rebase, delete, or stage until the main tree and every registered
worktree are understood. If `master` is clean and only behind, update it with:

```bash
git pull --ff-only origin master
```

Rebuild the authoritative report; never trust an old `report.json`.

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
python3 configure.py progress
```

Record these baseline fields before a batch:

```bash
jq '.measures | {
  matched_code, matched_functions, complete_code, complete_data,
  complete_units, total_units
}' build/GC6E01/report.json
```

Save baseline reports under ignored `build/` state when a multi-commit batch
needs an exact delta. Never commit generated reports or target objects.

## Worktree and branch reconciliation

Inventory every worktree, including detached ones:

```bash
while read -r path; do
  echo "WORKTREE $path"
  git -C "$path" status --short --branch
  git -C "$path" log -1 --oneline
  git -C "$path" diff --stat
done < <(git worktree list --porcelain | awk '$1 == "worktree" { print $2 }')
```

Classify each tree before changing it:

1. **Clean and merged/stale:** safe to remove.
2. **Committed ahead work:** inspect with `git cherry -v origin/master <branch>`
   and compare the patch against current source. Cherry-pick only surviving
   intent into a fresh integration tree.
3. **Dirty source work:** identify each touched function, rebuild its report,
   inspect semantics and sections, then bank or explicitly reject it.
4. **Generated/probe-only dirt:** remove after confirming it contains no source
   survivor. A stale `fleet/verify_fn.sh` edit is not a decompilation win.
5. **Active agent tree:** leave it alone until the owner reports or is stopped.

For a pull request that is ahead of current `master`, inspect both Git and CI:

```bash
gh pr view PR_NUMBER --json \
  state,mergeable,mergeStateStatus,headRefName,baseRefName,commits,files,statusCheckRollup
gh pr diff PR_NUMBER --name-only
gh pr checks PR_NUMBER
```

Rebase or replay only if the source remains strict and current. A PR that needs
a forbidden pragma or regresses the report is closed, not merged.

After work is merged or explicitly rejected:

```bash
git worktree remove /absolute/path              # clean tree
git worktree remove --force /absolute/path      # only documented reject/banked work
git worktree prune
git branch --merged master
```

Delete only campaign branches whose commits are reachable from `master` or
whose rejected patch has been documented. Never bulk-delete unknown branches.

## Candidate selection: prefer object closers

High fuzzy percentage alone is not the best payoff. Prefer objects with many
exact functions and one to four residual functions, because closing the
residuals links the entire object.

This report query lists small residual sets with their unit payoff:

```bash
jq -r '
  [.units[] | select(.functions != null) |
    {unit:.name,
     total:(.functions|length),
     exact:([.functions[]|select(.fuzzy_match_percent==100)]|length),
     residual:([.functions[]|select(.fuzzy_match_percent!=100)]|
       map({name,size:(.size|tonumber),pct:(.fuzzy_match_percent//0)}))} |
    .n=(.residual|length) |
    select(.n>0 and .n<=4 and .exact>=3)] |
  sort_by(-.exact,-.total)[] |
  [.exact,.total,.n,.unit,
   ([.residual[]|(.name+":"+(.pct|tostring)+"/"+(.size|tostring))]|join(","))] |
  @tsv
' build/GC6E01/report.json
```

For a single candidate, use the verifier that explicitly rebuilds the report:

```bash
fleet/verify_fn.sh FUNCTION 100
```

Important: a bare `ninja` does not necessarily regenerate
`build/GC6E01/report.json`. This previously caused stale percentages and wasted
probes. Always request the report target explicitly when scoring source.

For manual/model work, give one function or one small residual set per isolated
worktree. The task should state the retail size, current score, unit payoff,
strict rejection rules, and required commit/report handoff.

## Scout/agent handoff contract

Each scout should:

1. create a worker tree from current `origin/master` with
   `tools/setup_worker_worktree.py`;
2. touch only its assigned source/header scope;
3. compile in the loop against the exact MWCC flags;
4. compare text, data, relocations, and sibling functions;
5. run the smallest full-tree gates that prove the result;
6. commit only strict survivors on its own branch;
7. report commit hash, functions, retail bytes, unit-link payoff, validation,
   and precise blockers for rejects;
8. never push, merge, or edit the main worktree unless explicitly assigned.

Model output, including frontier/Kimi/ModernGekko-assisted output, is a source
proposal. It receives exactly the same semantic and compile-in-the-loop audit
as a human or permuter proposal.

## Farm reconciliation

Windows is the sole active farm. Poll it before restarting or replacing its
queue so results are not stranded:

```bash
tools/decomp_work/permuter/poll_win.sh
tools/decomp_work/permuter/poll_win.sh --status
```

For every `WIN` or `WIN?`:

1. identify its function, unit, source baseline, timestamp, and machine;
2. determine whether it is already exact on current `master`;
3. diff the candidate against live source, not an archived baseline;
4. reject shaping or semantic drift even when isolated score is zero;
5. apply survivors in an isolated current-master worktree;
6. rebuild the live report and inspect sibling/data effects;
7. keep the candidate provisional until the full DOL and quality gates pass.

Windows `outbox/` is cumulative. A pull reporting many wins can contain old
artifacts; compare against current source and timestamps before claiming a new
result. Do not clear outboxes or terminal state until reconciliation is done.

The retired accelerator farm must not be contacted, restarted, or recreated
from this repository. Its former activity remains only as historical evidence
in the handoff ledger and benchmark records below.

### Keep farm tools isolated from validation builds

Farm generation, reconciliation, and integration builds must never share a
writable `build/tools/` or `build/compilers/` directory. In particular, do not
symlink a live farm's tool directories into another worktree. A generator can
download or replace a tool while a verifier is reading it, producing a result
that is neither reproducible nor attributable to one toolchain snapshot.

Use this sequence whenever farm state and source work overlap:

1. stop the queue generator or give it a private copied tool/compiler tree;
2. record the tool paths, SHA-256 values, sizes, and mtimes used by the audit;
3. run reconciliation and the full validation from a worktree-local build;
4. verify those fingerprints again before restarting the supervisor;
5. if any fingerprint or mtime moved, discard the affected result and rebuild;
6. never run two `configure.py`/Ninja jobs in the same worktree concurrently;
7. after an interrupted local build, finish all surviving processes, run
   `ninja -t recompact`, regenerate deliberately, and repeat the required gate.

The post-PR #422 rotation caught a shared-tool race fail-closed before any
candidate was admitted. The operational fix is private tool directories (or a
single explicit mutex), not accepting a later successful retry without
provenance.

### Refresh the Windows farm

Generate fidelity-gated work units from a current-report queue. Exclude names
already exact on the integration head and targets already being integrated; a
70–100% near-match band is a useful starting point before ranking by object
closure payoff.

```bash
python3 tools/decomp_work/permuter/gen_workunits.py \
  --queue build/PURPOSE_BUILT_WINDOWS_QUEUE.tsv \
  --outdir build/permuter_workunits/WINDOWS_BATCH
```

Only entries marked `ok` in `manifest.json` are shippable. The generator
already checks parseability, exact compiler/flags, a single target symbol,
isolated/full-TU fidelity, and a finite nonzero base score.

Stop and pull first. Copy complete unit directories, then copy
`manifest.json` last; the running supervisor treats the manifest as the atomic
queue switch.

```bash
OUT=build/permuter_workunits/WINDOWS_BATCH
WINROOT=C:/Users/douglaswhittingham/gamecube-decomp/pkmn-permuter

tools/decomp_work/permuter/poll_win.sh
ssh win "schtasks /end /tn PkmnPermuterFarm" || true
tar --exclude='./manifest.json' -cf - -C "$OUT" . | \
  ssh win "tar -xf - -C $WINROOT/units"
scp "$OUT/manifest.json" "win:$WINROOT/units/manifest.json"
ssh win "schtasks /run /tn PkmnPermuterFarm"
tools/decomp_work/permuter/poll_win.sh --status
```

Healthy Windows status is `alive: true`, 12 workers, and the new manifest's
unit count. The persisted state is pruned against the new manifest at startup.

## Integration and validation

Create a fresh integration worktree from current remote master:

```bash
python3 tools/setup_worker_worktree.py /tmp/pkmn-integration-BATCH \
  --branch integration/BATCH --base origin/master
```

Cherry-pick only audited commits. Resolve overlaps from semantic intent and
current retail output; never accept a candidate merely because it was ahead in
an old branch.

Run, in order:

```bash
python3 configure.py --no-progress
ninja all_source build/GC6E01/report.json
ninja
python3 configure.py progress
python3 tools/check_object_map_freeze.py
python3 tools/update_readme_progress.py
python3 tools/update_readme_progress.py --check
python3 .github/scripts/test_quality_scan.py
python3 .github/scripts/quality_scan.py BASE_COMMIT HEAD
git diff --check
```

`ninja` must finish with the retail SHA-1 required by
`config/GC6E01/build.sha1`. If a full command cannot run, report the exact
command and failure; do not substitute a weaker claim.

Compare the saved base and head reports with the regression guard when a batch
contains multiple units:

```bash
python3 .github/scripts/check_regression.py \
  build/BASE_report.json build/GC6E01/report.json
```

Review the final diff explicitly. Stage source, headers, config status, README,
and intentional tooling only. Never use `git add -A` in a mixed worktree, and
never stage generated build products or `.inc` files.

## Publish, merge, and closeout

Push the focused integration branch, open a PR with exact function/byte/unit
deltas and validation, and wait for Build, Quality Lint, and Regression Guard.
Do not use admin overrides.

```bash
git push -u origin integration/BATCH
gh pr create --base master --head integration/BATCH --title "..." --body-file /tmp/pr-body.md
gh pr checks PR_NUMBER --watch
gh pr merge PR_NUMBER --merge --delete-branch
```

For a rejected PR:

```bash
gh pr close PR_NUMBER --delete-branch --comment "precise rejection reason"
```

After merge:

1. fast-forward local `master` and rebuild its report;
2. verify README metrics against that report and decomp.dev's PR report;
3. reconcile newly exact names out of the Windows queue and manifest;
4. pull any results produced during CI;
5. remove merged/rejected worktrees and prune registrations;
6. delete only merged/rejected branches;
7. refill the Windows farm with fresh targets that do not overlap integration;
8. update the handoff ledger below.

## Common failure modes

- **Stale score:** `ninja` ran, but the report target did not. Rebuild the
  report explicitly.
- **False farm zero:** isolated source differs from the live TU, or the result
  relies on shaping. Re-run the live full-TU and semantic gates.
- **Text exact, unit not linkable:** data/jump-table alignment or relocations
  differ. Inspect every emitted section.
- **Function exact, linked delta zero:** its `CodeCandidate` object still has
  residual functions or data. Do not report it as linked.
- **Residual range becomes an auto object:** every split range must keep an
  explicit `configure.py` object, including candidate-only prefixes/suffixes.
  Otherwise totals may stay constant while source-backed/category topology
  silently loses the residual functions.
- **Windows pull appears to find many new wins:** cumulative outbox contains
  history. Deduplicate against current master and timestamps.
- **Dirty detached tree:** inspect and bank/reject its diff before `--force`.
- **README/decomp.dev disagree:** rebuild the canonical report, sync README,
  and compare the PR base/head reported by CI.

## Handoff ledger

Update this compact block after each merged batch or farm rotation.

```text
Baseline commit:
Current master:
Open PRs:
Exact-source delta:
Newly linked function delta:
Newly linked unit/code/data delta:
README/report status:
Retail SHA status:
Windows workers / queue / wins pending:
Active worktrees and owners:
Banked commits not merged:
Rejected candidates and one-line reasons:
Next three unit closers:
```

### Batch snapshot — 2026-07-21 (PR #394)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: fd1ae92a (PR #393 merged)
Batch PR: #394, `campaign/next-batch-20260721`
Exact-source delta from goal baseline: +33 functions / +12,836 code bytes
PR #388 batch: +30 linked functions / +7 linked units / +5,664 linked code bytes
PR #389 batch: +2 exact functions / +1,080 matched code;
  +3 linked functions / +3 units / +1,176 linked code / +0 linked data
PR #391 batch: +12 exact functions / +5,700 matched code;
  +30 linked functions / +5 units / +7,616 linked code / +0 linked data
PR #394 batch: +12 exact functions / +3,020 matched code;
  +34 linked functions / +8 units / +6,804 linked code / +0 linked data
Newly linked function delta from goal baseline: +108
Newly linked unit/code/data delta from goal baseline: +29 units / +23,076 code / +177 data
Head report: 6,301 / 8,603 matched functions; 899,332 matched code bytes
Head linked: 714 / 1,303 units; 589,236 complete code bytes;
  752,761 complete data bytes
README/report: synced on PR #394; decomp.dev pending PR report
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: 535-entry 80%-plus queue across 210 units; 21 workers/permuters/
  timeouts/claims; no new recoverable wins; terminal state retained
Windows: 74 active-valid 70%-to-80% units, 12 workers, zero live wins/bad;
  persisted state pruned to the new manifest
Active campaign worktrees at snapshot: PR #394 integration; SDK 800A7820
  island audit; OSMemory closer; completed GX/gamedata/fdlibm trees pending
  post-merge cleanup
Banked commits not merged: SDK 800A7820 and OSMemory audits still in flight
Integrated: PR #388 (30-function linked batch) and PR #389 (MusyX table
  insertions plus effect teardown callback), and PR #391 (30 linked functions)
Rejected: fight_timer (private conversion constants break canonical relocations);
  THP decoder (293 relocation mismatches, extra data, and asm outside allowlist);
  CObjLoad (exact local literal cannot satisfy existing global HSD references
  without duplicated data); HSD JObj islands (current exacts require impossible
  branches, volatile rereads, gotos, or local optimization pragmas);
  CObj 80194400 (same local/global D970 pool conflict); CObj 80194DA4
  (remaining differences are FPR coloring and natural variants did not close);
  hsd_class (initializer is 27.375%; exact siblings rely on local pragmas or a
  volatile codegen reread);
  HSD_MObjReqAnimByFlags (goto/dummy-label zero changes null behavior);
  pokemonGetDp (private conversion constant); fightSideGetStatus (text exact,
  but linked retail SHA fails from relocation mismatch); fight residuals that
  require register, inline, pragma, jump-table, or volatile-only shaping
Next unit closers: strict SDK 800A7820 exact islands, Dolphin OSMemory, then
  fresh non-overlapping 3090/Windows targets after PR #394 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #395)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 079a4f4a (PR #394 merged)
Batch PR: #395, `campaign/post394-batch-20260721`
Exact-source delta from goal baseline: +36 functions / +14,412 code bytes
PR #395 batch: +3 exact functions / +1,576 matched code;
  +33 linked functions / +10 units / +4,568 linked code / +0 linked data
Newly linked function delta from goal baseline: +141
Newly linked unit/code/data delta from goal baseline: +39 units / +27,644 code / +177 data
Head report: 6,304 / 8,603 matched functions; 900,908 matched code bytes
Head linked: 724 / 1,318 units; 593,804 complete code bytes;
  752,761 complete data bytes
README/report: synced on PR #395; decomp.dev confirmed at merge 53492f21
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: 535-entry current queue; 21 workers/permuters/timeouts/claims;
  zero new recoverable wins; one stale terminal WIN? retained
Windows: 58 active-valid post-#394 units; 12 workers; zero live wins;
  one bad unit retained for the next reconciliation pass
Active campaign worktrees at snapshot: PR #395 integration and the banked
  fight_trainer_ai2 closer; completed audit trees pending post-merge cleanup
Banked commits not merged: dae86488, strict fight_trainer_ai2 split
  (+9 linked functions / +3 units / +1,132 linked code)
Rejected: DVDCheckDisk (unaligned jump-table padding); DVDCancelAsync
  (dead-local frame shaping); OSCache privileged/pragma functions; OSMemory
  privileged assembly tail; DBGWrite/DBGRead (unused volatile scratch shaping);
  three fight-timer conversion functions (private unowned sdata2 constants)
Next unit closers: banked fight_trainer_ai2 (+9), then fresh strict islands
  from current-master reports while both farms continue their refreshed queues
```

### Batch snapshot — 2026-07-21 (PR #396)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 53492f21 (PR #395 merged)
Batch PR: #396, `campaign/post395-batch-20260721`
Exact-source delta from goal baseline: +36 functions / +14,412 code bytes
This batch: +0 exact functions / +0 matched code;
  +30 linked functions / +7 units / +2,644 linked code / +0 linked data
Newly linked function delta from goal baseline: +171
Newly linked unit/code/data delta from goal baseline: +46 units / +30,288 code / +177 data
Head report: 6,304 / 8,603 matched functions; 900,908 matched code bytes
Head linked: 731 / 1,328 units; 596,448 complete code bytes;
  752,761 complete data bytes
README/report/decomp.dev: synced on PR #396 at merge `d856c1a5`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: 535-entry current queue; 21 workers/permuters/timeouts/claims;
  zero new recoverable wins; one stale terminal WIN? retained
Windows: 58 active-valid post-#395 units; 12 workers; zero live wins;
  one bad unit retained; manifest and supervisor restarted atomically
Integrated: 9 fight_trainer_ai2 functions, 2 input functions, 16 Pokemon
  accessors, and 3 MusyX input helpers
Banked not merged: b7e47fee (+16 fight-range links) and 6c7ffef0
  (+21 MusyX links on top of the integrated 99cd073b prerequisite)
Rejected: SDK 80098108 exacts (local pragmas/asm/shaping); remaining input
  exacts (gotos or local pragmas); pokemonEvolutionAll (dont_inline pragma);
  MusyX _GetInputValue (goto-dependent); inpTranslateExCtrl and inpGetExCtrl
  (private duplicate jump tables break the retail SHA)
Next unit closers: banked fight-range 16, banked MusyX 21, then fresh farm
  wins or a strict small-residual object closer
```

### Batch snapshot — 2026-07-21 (PR #397)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: d856c1a5 (PR #396 merged)
Batch PR: #397, `campaign/post396-batch-20260721`
Exact-source delta from goal baseline: +36 functions / +14,412 code bytes
This batch: +0 exact functions / +0 matched code;
  +37 linked functions / +5 units / +4,020 linked code / +0 linked data
Newly linked function delta from goal baseline: +208
Newly linked unit/code/data delta from goal baseline: +51 units / +34,308 code / +177 data
Head report: 6,304 / 8,603 matched functions; 900,908 matched code bytes
Head linked: 736 / 1,337 units; 600,468 complete code bytes;
  752,761 complete data bytes
README/report/decomp.dev: synced on PR #397 at merge `34442c32`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#397 queue; 538 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#397 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated bank: 16 strict fight-sequence/status functions and 21 strict
  MusyX MIDI/input-getter functions
Active campaign worktrees: item-data integration plus isolated game and HSD
  closer scouts; Dolphin survivor banked for the next integration
Next unit closers: 37 strict item-data accessors plus 6 strict Dolphin
  allocator/DVD functions, then survivors from the game and HSD scouts
```

### Batch snapshot — 2026-07-21 (PR #398)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 34442c32 (PR #397 merged)
Batch PR: #398, `campaign/people-item-post397-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +3 exact functions / +512 matched code / +12 matched data;
  +43 linked functions / +4 units / +2,168 linked code / +12 linked data
Newly linked function delta from goal baseline: +251
Newly linked unit/code/data delta from goal baseline: +55 units / +36,476 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 740 / 1,341 units; 602,636 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #398 at merge `5445b846`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#397 queue; 538 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#397 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated: 37 item-data accessors plus 6 canonical Dolphin OS allocator/DVD
  functions; the suspicious exportable check and residual suffix remain candidates
Rejected: game closer probes requiring codegen shaping (width-copy relocation,
  scheduling/peephole controls, cast-hoist suppression, or artificial iterators)
Next unit closers: strict HSD scout survivor if any, then current-report object
  closers and non-overlapping farm results after PR #398 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #399)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 5445b846 (PR #398 merged)
Batch PR: #399, `campaign/post398-next-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +42 linked functions / +6 units / +768 linked code / +0 linked data
Newly linked function delta from goal baseline: +293
Newly linked unit/code/data delta from goal baseline: +61 units / +37,244 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 746 / 1,351 units; 603,404 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #399 at merge `64c0ee9c`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#398 queue; 537 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#398 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated: 31 strict msgctrl helpers, 10 strict HSD light-state helpers,
  and the strict HSD RObj value loader
Rejected: 7-function debug callback island was text-exact but dead-stripped at
  link, shrinking .text by 812 bytes and failing the retail SHA
Banked not merged: four-function MusyX voice-start island still in final gates
Next unit closers: banked MusyX island, then fresh strict current-report islands
  and non-overlapping farm results after PR #399 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #400)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 64c0ee9c (PR #399 merged)
Batch PR: #400, `campaign/post399-batch30-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +37 linked functions / +2 units / +1,476 linked code / +0 linked data
Newly linked function delta from goal baseline: +330
Newly linked unit/code/data delta from goal baseline: +63 units / +38,720 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 748 / 1,354 units; 604,880 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #400 at merge `3f0d86b6`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#399 queue; 537 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#399 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated: 33 strict fight type/kind accessors plus four strict MusyX
  last-started voice helpers
Rejected: the apparent 31-function msgctrl tail batch was pragma-dependent;
  only strict natural-C islands may be promoted
Next unit closers: fresh current-report exact islands and preserved nonexact farm
  artifacts after PR #400 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #401)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 3f0d86b6 (PR #400 merged)
Batch PR: #401, `campaign/post400-batch33-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +33 linked functions / +2 units / +416 linked code / +0 linked data
Newly linked function delta from goal baseline: +363
Newly linked unit/code/data delta from goal baseline: +65 units / +39,136 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 750 / 1,356 units; 605,296 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #401 at merge `2dfedf82`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#400 queue; 537 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#400 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated: 20 strict fight-action dispatchers plus 13 strict fight-action
  BIOS setters/getters; the middle range remains an explicit candidate
Next unit closers: separately banked strict HSD islands plus fresh current-report
  closers and non-overlapping farm results after PR #401 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #402)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 2dfedf82 (PR #401 merged)
Batch PR: #402, `campaign/post401-batch30-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +30 linked functions / +9 units / +2,428 linked code / +0 linked data
Newly linked function delta from goal baseline: +393
Newly linked unit/code/data delta from goal baseline: +74 units / +41,564 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 759 / 1,373 units; 607,724 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #402 at merge `ec1874d3`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#401 queue; 537 entries, 21 workers, zero new wins;
  one stale terminal WIN? retained
Windows: healthy post-#401 manifest; 72 fidelity-valid units, 12 workers,
  zero new wins
Integrated: 12 strict HSD state/AObj functions plus 18 strict fight-Pokemon
  accessors; every split residual remains an explicit candidate
Rejected: pragma/codegen-shaped HSD islands, incorrect private jump-table
  relocations, volatile-dependent setters, and Ghidra-shaped
  fightOutPokemonIsHinsi
Next unit closers: current-report object closers and non-overlapping farm results
  after PR #402 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #403)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: ec1874d3 (PR #402 merged)
Batch PR: #403, `campaign/post402-batch30-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +30 linked functions / +6 units / +1,148 linked code / +0 linked data
Newly linked function delta from goal baseline: +423
Newly linked unit/code/data delta from goal baseline: +80 units / +42,712 code / +189 data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
Head linked: 765 / 1,384 units; 608,872 complete code bytes;
  752,773 complete data bytes
README/report/decomp.dev: synced on PR #403 at merge `f1c251bd`
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#402 queue; 537 entries, 21 workers, zero new wins;
  13 units retargeted and the historical WIN? retained
Windows: healthy post-#402 manifest; 75 fidelity-valid units, 12 workers,
  zero new wins; five exact targets retired, five fresh targets added, and
  orphaned prior-run permuters removed without discarding outputs
Integrated: 26 strict fight encounter/effect accessors plus four canonical
  Dolphin SRAM lock/unlock entry points; every residual remains a candidate
Rejected: four pragma-dependent fight accessors; MusyX candidates with broken
  cross-split sdata/relocations; SRAM sound/progressive accessors that lose
  exactness without inventing inline helpers
Next unit closers: fresh current-report natural-C islands and non-overlapping
  farm results after PR #403 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #404)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: f1c251bd (PR #403 merged)
Batch PR: #404, `campaign/post403-batch30-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 exact functions / +0 matched code/data;
  +30 linked functions / +5 complete units / +9 total-unit topology /
  +728 linked code / +0 linked data
Goal cumulative from PR #381: +453 linked functions / +85 complete units /
  +43,440 linked code / +189 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes
  (unchanged)
Head linked: 4,221 functions; 770 / 1,393 units;
  609,600 complete code bytes; 752,773 complete data bytes
README/report/decomp.dev: synced on PR #404 at merge `f3cf812a`;
  decomp.dev reports 24.43% linked / 609.60 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#404 queue on `f3cf812a`;
  537 entries, 21 workers and 21 active claims, zero new wins;
  historical terminal WIN? retained
Windows: healthy post-#404 manifest on `f3cf812a`;
  74 fidelity-valid units, 12 workers and 12 active units, zero new wins;
  historical score-15 partial retained
Integrated: 30 strict item-parameter helpers; residual ranges remain
  explicit candidates
Next unit closers: strict current-report islands and non-overlapping farm results
  after PR #404 reconciliation
```

### Batch snapshot — 2026-07-21 (PR #405)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: f3cf812a (PR #404 merged)
Batch PR: #405, `campaign/post404-batch30-20260721`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code / +4 matched data;
  +30 linked functions / +11 complete units / +19 total-unit topology /
  +1,492 linked code / +4 linked data
Goal cumulative from PR #381: +483 linked functions / +96 complete units /
  +44,932 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,251 functions; 781 / 1,412 units;
  611,092 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced on PR #405 at merge `244e9d7d`;
  decomp.dev reports 24.49% linked / 611.09 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#405 queue on `244e9d7d`;
  537 entries, 21 workers and 21 active claims, zero new wins;
  `__OSStopAudioSystem` retargeted and historical evidence retained
Windows: healthy post-#405 manifest on `244e9d7d`;
  74 fidelity-valid units, 12 workers and 12 active units, zero new wins;
  historical score-15 partial retained
Integrated: 11 strict SDK/runtime helpers, five strict HSD helpers,
  11 strict floor-character helpers, and three strict battle-camera helpers;
  residual ranges remain explicit candidates
Next unit closers: strict current-report game/HSD islands while both farms
  continue their reconciled queues
```

### Batch snapshot — 2026-07-21 (PR #406)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 244e9d7d (PR #405 merged)
Batch PR: #406, `integration/pr406-exact30`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +10 complete units / +14 total-unit topology /
  +1,660 linked code / +0 linked data
Goal cumulative from PR #381: +513 linked functions / +106 complete units /
  +46,592 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,281 functions; 791 / 1,426 units;
  612,752 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced on PR #406 at merge `ce1c97e5`;
  decomp.dev reports 24.56% linked / 612.75 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#406 queue on `ce1c97e5`; 537 entries, 21 workers and
  21 active claims, zero new recoverable wins
Windows: healthy post-#406 manifest on `ce1c97e5`; 74 fidelity-valid units,
  12 workers and 12 active units, zero new recoverable wins
Integrated: six GS texture getters, four RNG helpers, five battle
  resource helpers, three FObj helpers, three ID helpers, one render-pass
  getter, three ImageDesc helpers, three model-bound helpers, and two AObj
  callback setters; every residual range remains an explicit candidate
Rejected: TObj 0x801BBDDC five-function probe required `-O1` and emitted a
  private jump table/relocation that broke the retail SHA
Independent audits: PASS for all 30 functions, emitted sections, normalized
  relocations, semantics, residual coverage, and canonical type cleanup
Next unit closers: the strict Toolentry/field/battle exact-30 island prepared
  for PR #407 after post-#406 farm reconciliation
```

### Batch snapshot — 2026-07-21 (PR #407)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: ce1c97e5 (PR #406 merged)
Batch PR: #407, `integration/pr407-exact30`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +10 complete units / +18 total-unit topology /
  +1,944 linked code / +0 linked data
Goal cumulative from PR #381: +543 linked functions / +116 complete units /
  +48,536 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,311 functions; 801 / 1,444 units;
  614,696 complete code bytes; 752,777 complete data bytes
README/report: synced at merge `7e9f6187`; decomp.dev closeout verification
  remains part of the post-merge reconciliation
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: healthy post-#406 queue on `ce1c97e5`; 537 entries, 21 workers and
  21 active claims; queue/metadata SHA-256 snapshots preserved; zero overlap
  with this batch and zero new recoverable wins
Windows: healthy post-#406 manifest on `ce1c97e5`; 74 fidelity-valid units,
  12 workers and 12 active units; zero overlap and zero new recoverable wins
Integrated: 23 strict Toolentry helpers, five strict field/nursery
  helpers, and two strict battle-camera helpers; all eight residual ranges remain
  explicit CodeCandidate units
Rejected farm artifacts: dataInsertLayer used artificial pointer aliases and
  lacked live-tree validation; fn_8004E8E0 was 99.91071% and used artificial
  aliases/control flow; Windows produced no new zero-score result
Independent audit: PASS for exactly 30 functions / 1,944 bytes, ten text-only
  Matching units, all 59 normalized relocations, semantics, contiguous
  residual coverage, canonical flags, quality policy, and retail SHA
Semantic corrections: replaced implicit return-register chaining with typed
  value flow, corrected callback and state/accessor types, and preserved the
  12-byte visibility array with target-evidenced extern small-data placement
Next action: complete post-merge farm reconciliation while the ranked PR #408
  exact-30 batch proceeds in an isolated current-master integration tree
```

### Batch snapshot — 2026-07-21 (PR #408)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 7e9f6187 (PR #407 merged)
Batch PR: #408, `integration/pr408-exact30`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +10 complete units / +18 total-unit topology /
  +2,276 linked code / +0 linked data
Goal cumulative from PR #381: +573 linked functions / +126 complete units /
  +50,812 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,341 functions; 811 / 1,462 units;
  616,972 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `08f50d3a`; decomp.dev reports
  24.73% linked / 616.97 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
Farm status: post-#407 reconciliation/remirror is tracked independently; no
  farm result is claimed by this source-linking batch
Integrated: seven canonical Dolphin DVD/VI functions, 16 GS VTR/save
  state functions, three battle party/status functions, and four model/HSD JObj
  traversal/matrix helpers
Residual policy: callback `cb`, `fn_800A8894`, `fn_801E1300`, and every other
  prefix/interstitial/suffix range remain explicit CodeCandidate units;
  `VISetNextFrameBuffer`, `VISetBlack`, and `fn_801B06DC` stay outside this
  batch and are not counted as linked progress
Validation: all 30 selected functions / 2,276 bytes are raw 100%; ten
  text-only Matching units emit retail text and all 120 normalized relocations
  under the canonical compilers and flags; full retail SHA passes
Independent audit: PASS for exact composition, semantic/type integrity,
  residual coverage, canonical flags, report/freeze deltas, and retail SHA
Next action: continue post-merge farm reconciliation while the PR #409
  exact-30 batch proceeds in an isolated current-master integration tree
```

### Batch snapshot — 2026-07-21 (PR #409)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 08f50d3a (PR #408 merged)
Batch PR: #409, `integration/pr409-exact30`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +10 complete units / +20 total-unit topology /
  +1,992 linked code / +0 linked data
Goal cumulative from PR #381: +603 linked functions / +136 complete units /
  +52,804 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,371 functions; 821 / 1,482 units;
  618,964 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `f9f06071`; decomp.dev reports
  24.81% linked / 618.96 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: reconciled post-#408 snapshots
  `post408-pre-08f50d3a-20260722T053816Z` and
  `post408-live-08f50d3a-20260722T054610Z`; 537 queue records; queue SHA-256
  `07aea0db351b74a7d9c99748f53b0528ef79ff0d2dca577c49276b36cec9bdc3`;
  zero overlap with this batch
Windows: reconciled 143-record manifest with 74 fidelity-valid units;
  manifest SHA-256
  `670a7bc1f1709521fd9593424f48fed64f12ae54e395c2d2e34bfb79f7e331ca`;
  zero overlap with this batch
Integrated: four HSD MObj helpers, 13 mail helpers, one waza-viewer
  updater, four waza-sequence helpers, and eight fight-action accessors
Residual policy: all 15 prefix/interstitial/suffix ranges remain explicit
  CodeCandidate units; no farm artifact is claimed by this source-linking batch
Validation: all 30 selected functions / 1,992 bytes are raw 100%; ten
  text-only Matching units emit retail text and all 59 normalized relocations
  under the canonical compilers and flags; full retail SHA passes
Type recovery: synchronized the mail/waza/fight cross-TU structures and
  callbacks; recovered explicit particle visibility/scaling parameters and
  pointer-valued model/particle fields without changing retail code
Independent audit: PASS for exact composition, relocation parity, residual
  coverage, semantic/type integrity, zero regression, and retail SHA
Next action: continue the Windows-only farm reconciliation while the PR #410
  exact-30 batch proceeds in an isolated current-master integration tree
```

### Batch snapshot — 2026-07-21 (PR #410)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: f9f06071 (PR #409 merged)
Batch PR: #410, `integration/pr410-exact30`
Exact-source delta from goal baseline: +39 functions / +14,924 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +4 complete units / +8 total-unit topology /
  +2,376 linked code / +0 linked data
Goal cumulative from PR #381: +633 linked functions / +140 complete units /
  +55,180 linked code / +193 linked data
Head report: 6,307 / 8,603 matched functions; 901,420 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,401 functions; 825 / 1,490 units;
  621,340 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `1a93c215`; decomp.dev reports
  24.90% linked / 621.34 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: disabled and removed from active operation; remote campaign checkout,
  processes, cron entries, and services are absent; no PR #410 work used it
Windows: sole active farm; post-merge reconciliation produced a 142-record
  manifest with 73 fidelity-valid units, SHA-256
  `80015b753741e0c382594e4304c2dbfe0ccaecbb3549d13100e5eb5bf5aa3cc0`,
  and 12 / 12 live workers; 48 orphan duplicate permuters and one terminal
  Windows-incompatible camera unit were removed
Integrated: 15 particle-generator state helpers, ten light animation
  state helpers, and five model/material shadow-state helpers
Residual policy: all prefix/interstitial/suffix ranges remain explicit
  CodeCandidate units; no farm artifact is claimed by this source-linking batch
Validation: all 30 selected functions / 2,376 bytes are raw 100%; four
  text-only Matching units emit retail text and all 69 normalized relocations
  under the canonical compilers and flags; full retail SHA passes
Context fidelity: the never-linked GSlight suffix candidate retains the exact
  island as compile-only same-TU context so MWCC preserves the pre-split natural
  inline decision in `GSlightLoad`; only the standalone exact island is linked
Independent audit: PASS for exact composition, relocation parity, residual
  coverage, semantic/type integrity, zero regression, and retail SHA
Next action: integrate the two strict Windows recoveries with the ranked
  SDK/light complement in PR #411 while the cleaned Windows-only farm runs
```

### Batch snapshot — 2026-07-21 (PR #411)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 1a93c215 (PR #410 merged)
Batch PR: #411, `integration/pr411-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +2 matched functions / +340 matched code / +0 matched data;
  +30 linked functions / +15 complete units / +27 total-unit topology /
  +3,660 linked code / +0 linked data
Goal cumulative from PR #381: +663 linked functions / +155 complete units /
  +58,840 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,431 functions; 840 / 1,517 units;
  625,000 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `c4ab1d16`; decomp.dev reports
  25.05% linked / 625.00 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: disabled and removed from active operation; remote campaign checkout,
  processes, cron entries, and services remain absent; PR #411 did not use it
Windows: sole active farm; reconciled on `1a93c215` with 142 records, 73
  fidelity-valid units, 12 / 12 live workers, zero bad units, and manifest
  SHA-256 `80015b753741e0c382594e4304c2dbfe0ccaecbb3549d13100e5eb5bf5aa3cc0`;
  the two previously banked strict recoveries are integrated in this batch
Integrated: 24 canonical Dolphin DSP/CARD functions, two typed camera
  vector helpers, two typed GSlight vector setters, GX pixel-mode
  synchronization, and the GS layer vertex setup loop
Residual policy: all 20 affected prefix/interstitial/suffix ranges remain
  explicit CodeCandidate units; the alias-dependent CARD functions and the
  pragma-dependent neighboring layer helper remain unlinked and are not counted
Validation: all 30 selected functions / 3,660 bytes are raw 100%; fifteen
  text-only Matching units emit retail text and all 96 normalized relocations
  under the canonical compilers and flags; full retail SHA passes
Type/provenance recovery: canonical Dolphin SDK sources support all 24 SDK
  bodies; exact sources use explicit `GSRenderVec3*`, `Vec*`, `OSContext*`,
  `EXICallback`, `DVDDiskID*`, and `CARDCallback` types instead of
  hidden-register or K&R argument dependencies
Regression: all 8,603 per-function scores compared with `1a93c215`; zero
  regressions and two new byte-exact matches
Independent audit: PASS for exact composition, relocation parity, residual
  coverage, semantic/type integrity, zero regression, and retail SHA
Next action: continue the Windows-only farm while the ranked PR #412 batch
  proceeds in an isolated current-master integration tree
```

### Merged batch snapshot — 2026-07-21 (PR #412)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: c4ab1d16 (PR #411 merged)
Merged batch: PR #412, source `2b057335`, merge `c7701bef`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +8 complete units / +13 total-unit topology /
  +1,384 linked code / +0 linked data
Goal cumulative from PR #381: +693 linked functions / +163 complete units /
  +60,224 linked code / +193 linked data
Merged report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Merged linked: 4,461 functions; 848 / 1,530 units;
  626,384 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `c7701bef`; decomp.dev reports
  25.10% linked / 626.38 kB complete code
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
Retired 3090 farm: remote checkout, processes, cron entries, and services
  remain absent; this follow-up removes its dedicated tracked launch, queue,
  harvest, and poll tooling while preserving historical campaign evidence
Windows: sole active farm; post-#411 rotation on `c4ab1d16` has a 140-record
  manifest with 71 fidelity-valid units, zero exact overlap, 12 / 12 workers,
  zero bad units, and SHA-256
  `4f87af1c09ac3ab6609b54c036700b027498c7cbc09a58566a94e280e6e40a5a`
Integrated: four typed GS camera accessors, three typed window-work
  accessors, three field-camera state helpers, 17 People/model helpers, and
  three weather-flow functions
Residual policy: all ten affected prefix/interstitial/suffix ranges remain
  explicit CodeCandidate units; `peopleTestFlags`, `peopleGetPosition`,
  `fn_8018FCE0`, `windowGetAllocPtr`, and pragma-dependent `windowAllocMemory`
  remain unlinked and are not counted
Validation: all 30 selected functions / 1,384 bytes are raw 100%; eight
  text-only Matching units emit retail text and all 52 normalized relocations
  under the canonical compilers and flags
Type recovery: corrected `PeopleEntry` byte fields at 0x94-0x97; the model
  vector wrappers now use explicit `GSmodel*` / `GSvec*` arguments, including
  the previously hidden second output argument to `fn_8018FC2C`; field-camera,
  GS camera, window-work, weather callback, and allocator APIs are explicit
Regression: all 8,603 per-function scores compared with `c4ab1d16`; zero
  regressions and no score-only progress claimed
Independent audit: PASS from two independent reviews; exact composition,
  emitted text, all 52 relocations, prototypes, topology, and policy checks agree
Next action: merge this tracked-tooling cleanup, rotate the Windows-only farm
  to `c7701bef`, and integrate the next exact-30 batch
```

### Farm cleanup snapshot — 2026-07-21 (PR #413)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before cleanup: c7701bef (PR #412 merged)
Merged cleanup: PR #413, source `5503da7f`, merge `ec451007`
Decompilation delta: none; report and linked metrics remain the PR #412 values
Tracked cleanup: removed all 28 files under the retired 3090 farm directory
  plus its queue generator, unit-flag extractor, and poll script; retained only
  historical ledgers, benchmark evidence, and archived results
Local cleanup: removed the active ignored 3090 queue, manifest, metadata, and
  bytecode caches after reconciliation; no recoverable source was discarded
Remote 3090: campaign checkout, processes, cron entries, and services remain
  absent; do not contact, restart, or redeploy this machine
Windows: sole active farm; current-report refill contains 138 records, 69
  fidelity-valid runnable units across 42 objects, and 69 documented skips;
  manifest SHA-256
  `59c84e331b7ce4a00b742e97314affd002bda096ea258d10ee92e6f7bd8150b4`;
  one supervisor and 12 / 12 workers are live with zero wins or bad units at
  the handoff boundary
Quality: generator tests now exercise only the live Windows generator; source,
  config, object topology, progress counters, and retail output are unchanged
Next action: integrate the independently preflighted PR #414 exact-30 batch,
  then reconcile and rotate the Windows queue from the merged head
```

### Merged batch snapshot — 2026-07-21 (PR #414)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: ec451007 (PR #413 merged)
Merged batch: PR #414, source `96fca6dd`, merge `99ab9e5b`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +12 complete units / +20 total-unit topology /
  +1,916 linked code / +0 linked data
Goal cumulative from PR #381: +723 linked functions / +175 complete units /
  +62,140 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,491 functions; 860 / 1,550 units;
  628,300 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `99ab9e5b`; decomp.dev reports
  25.18% linked / 628.30 kB complete code
Retired 3090 farm: no tooling, processes, checkout, cron, or services were
  recreated or contacted; all model and permutation work remains Windows/local
Windows: sole active farm; the post-#413 138-record manifest remains isolated
  from this batch, with 69 runnable units and 12 / 12 live workers
Integrated: one Dolphin OSLink wrapper, four MetroTRK dispatch/init helpers,
  two MetroTRK communication/circle-buffer helpers, the canonical MSL string
  writer, five GS thread access/block helpers, one collision plane-line query,
  one flag clear routine, and 15 typed People info-table helpers
Reconciliation rejects: `GSscratchFree` required a one-use artificial inline
  helper; three printf-family callers depended on a TU-local `__pformatter`;
  `fn_8000D290` and `OSLinkFixed` were report-exact but dead-stripped from the
  linked image. None is linked or counted. Live natural replacements were
  selected and the retail-link gate now passes.
Residual policy: all nine affected original ranges remain contiguous explicit
  CodeCandidate partitions; no missing range became an auto object
Validation: all 30 functions / 1,916 bytes are raw 100%; twelve Matching units
  emit target-identical text and all 57 normalized relocations with correct
  symbol ownership; every selected symbol survives in main.elf at retail VA
Type recovery: `PeopleInfoBiosEntry` is an evidenced 0x2C serialized record;
  byte 0x09 is stored raw and explicitly sign-extended by its getter. Collision
  vector, TRK result/buffer/callback, OS module, and GS thread types are explicit
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, liveness, text, relocation
  ownership, residual coverage, semantic/type fidelity, policy, and full link
Next action: reconcile/rotate Windows, then replay the linker-live PR #415
  exact-30 batch from the merged head
```

### Merged batch snapshot — 2026-07-21 (PR #415)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 99ab9e5b (PR #414 merged)
Merged batch: PR #415, source `6ed99f1b`, merge `76ecf7c4`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +15 complete units / +30 total-unit topology /
  +480 linked code / +0 linked data
Goal cumulative from PR #381: +753 linked functions / +190 complete units /
  +62,620 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,521 functions; 875 / 1,580 units;
  628,780 complete code bytes; 752,777 complete data bytes
README/report/decomp.dev: synced at merge `76ecf7c4`; decomp.dev reports
  25.20% linked / 628.78 kB complete code
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole active permutation farm
Windows: post-#414 reconciliation found no recoverable survivors among seven
  strict rejects; the fresh queue has 138 records, 69 runnable units across 42
  objects, and 69 documented skips, with manifest SHA-256
  `dcaa80696c3cf87ee4368c4f9fb34f11d27a3bb5ba56a7888139c75de9160e52`;
  one supervisor and 12 / 12 workers are live with zero wins or bad units
Integrated: 21 typed FSYS slot/state helpers, seven typed material helpers,
  the six-nibble GBA entry-Pokemon decoder, and one NPC model-ID setter
Dead-strip gate: `fn_80097FCC` and `fn_80097FF8` were report-exact but absent
  from the final linked image, so live replacements were selected instead
Residual policy: four affected source ranges remain gapless and overlap-free as
  15 Matching units plus 19 explicit non-auto CodeCandidate units
Validation: all 30 functions / 480 bytes are raw 100%; all 15 Matching units
  emit target-identical text with target/source relocation totals of zero; each
  selected symbol survives in main.elf at its retail VA and has a genuine
  incoming relocation from an active linker input; `FORCEACTIVE` remains empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, source/target text, liveness,
  semantic and ABI fidelity, residual coverage, policy, and full retail link
Next action: rotate the Windows-only queue from the merged head, then integrate
  the next vetted exact-30 batch
```

### Batch snapshot — 2026-07-21 (PR #416)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 76ecf7c4 (PR #415 merged)
Batch PR: #416, `integration/pr416-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +19 complete units / +34 total-unit topology /
  +1,268 linked code / +0 linked data
Goal cumulative from PR #381: +783 linked functions / +209 complete units /
  +63,888 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,551 functions; 894 / 1,614 units;
  630,048 complete code bytes; 752,777 complete data bytes
README/report: synced at the PR head at 25.25% linked / 630.05 kB complete
  code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole active permutation farm
Windows: post-#415 reconciliation found 32 already-exact artifacts, the same
  seven strict rejects, and zero recoverable survivors; the fresh queue has 138
  records, 69 runnable units across 42 objects, and 69 documented skips, with
  manifest SHA-256
  `019ff556c2328192d60c2d17b0ecc8a2fd2c9bd6a70e759ef71abdba2aae2277`;
  one supervisor and 12 / 12 workers are live with zero wins or bad units
Integrated: two texture lifetime helpers, two event-state helpers, seven typed
  render-camera/state helpers, three typed HSD shadow helpers, three debug-menu
  state helpers, six typed GBA globals, five field-camera state helpers, one
  model-shadow buffer release, and one camera-state size query
ABI rejection gate: `GStextureSetFilter` and `GStextureSetWrap` were report
  exact but depended on zero-argument inherited-register callsites; two natural,
  live event helpers replaced them byte-for-byte. Pragma-, volatile-, raw-alias-,
  and asm-wrapper-dependent alternates remain unlinked and are not counted.
Type recovery: viewing-rectangle, shadow-owner/data, GS render-camera/state,
  AObj, texture-handle, model-shadow, and field-resource layouts are explicit;
  the HSD shadow enable byte and active-camera pointer ABI match their callers
Residual policy: nine affected original objects become 19 Matching units plus
  24 explicit non-auto CodeCandidate ranges; coverage is gapless and disjoint
Validation: all 30 functions / 1,268 bytes are raw 100%; all 19 Matching units
  emit target-identical text and exactly 53 normalized target/source relocations;
  every selected symbol survives in main.elf at its retail VA and size and has
  a genuine incoming relocation from an active linker input; `FORCEACTIVE` is
  empty. The two `fn_800D2B44` `.sdata2` declarations are narrowly retained as
  canonical storage-class metadata required for the retail SDA21 relocations.
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, text and relocation parity,
  liveness, ABI/type fidelity, residual topology, strict-policy compliance,
  zero regression, and the full retail link
Next action: publish and merge PR #416 after final review, rotate Windows from
  the merged head, then integrate the next vetted exact-30 batch
```

### Batch snapshot — 2026-07-22 (PR #418)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: e6987c21 (PR #417 merged; no decomp delta)
Batch branch: `integration/pr418-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +13 complete units / +25 total-unit topology /
  +2,008 linked code / +0 linked data
Goal cumulative from PR #381: +813 linked functions / +222 complete units /
  +65,896 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,581 functions; 907 / 1,639 units;
  632,056 complete code bytes; 752,777 complete data bytes
README/report: synced at the PR head at 25.33% linked / 632.06 kB complete
  code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole active permutation farm
Windows: post-#416 reconciliation found 32 already-exact artifacts, the same
  seven strict rejects, and zero recoverable survivors; four topology rechecks
  remain full-TU-fidelity invalid. The fresh queue has 138 records across 70
  objects, 69 runnable units across 42 objects, and 69 documented skips (67
  full-TU-fidelity, two parser), with manifest SHA-256
  `49c4713b83038612ae368f2d400d87f523acca54d70ff8cb65352593af563cf5`;
  one supervisor (PID 57200) and 12 / 12 workers are live at round three with
  zero wins or bad units
Integrated: 22 typed camera state, animation, movement, and public-vector
  helpers; seven typed particle AppSRT lifetime/attachment helpers; and the
  linker-live `_psListGetNext` accessor
Relocation rejection gate: `psAddGeneratorAppSRT` emitted target-identical text
  but failed strict parity for two of 14 private-versus-owned relocations; the
  replacement `_psListGetNext` has zero relocations and three genuine incoming
  references. The rejected function remains unlinked and is not counted.
Type recovery: the 0xFC saved camera-state prefix, 0xB4 generator state, and
  0x74 AppSRT layouts are canonical shared definitions; local duplicates were
  removed from camera and particle units, and public camera-vector APIs use
  explicit `GSSceneVec3*` arguments
Residual policy: the two affected original ranges become 13 Matching units
  plus 14 explicit non-auto CodeCandidate ranges; coverage is gapless and
  disjoint
Validation: all 30 functions / 2,008 bytes are raw 100%; all 13 Matching units
  emit target-identical text and exactly 85 normalized target/source
  relocations; every selected symbol survives in main.elf at its retail VA and
  size and has a genuine incoming relocation from an active linker input;
  `FORCEACTIVE` is empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, text and relocation parity,
  liveness, caller ABI/type fidelity, residual topology, strict-policy
  compliance, zero regression, and the full retail link
Next action: publish and merge PR #418 after final review, rotate Windows from
  the merged head, then integrate the next vetted exact-30 batch
```

### Batch snapshot — 2026-07-22 (PR #419)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 0b79a8af (PR #418 merged)
Batch branch: `integration/pr419-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +15 complete units / +29 total-unit topology /
  +2,920 linked code / +0 linked data
Goal cumulative from PR #381: +843 linked functions / +237 complete units /
  +68,816 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,611 functions; 922 / 1,668 units;
  634,976 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.45% linked / 634.98 kB
  complete code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole active permutation farm
Windows: post-#418 reconciliation classified all 39 artifacts as 32 already
  exact plus the same seven strict rejects, with zero recoverable survivors;
  136 transients were preserved (minimum score 105, none at or below 25). The
  final overlap-free queue has 129 records across 68 objects, 60 runnable
  units across 40 objects, and 69 skips (67 full-TU-fidelity, two parser),
  with queue SHA-256
  `a29e3e199380cdf80c3ecf0de432ce5b864f5a9dd41d2afc931f272d7ed97bd0`
  and manifest SHA-256
  `ffd1525548fd474c00b79cf99abe034465bb2deffa239e4421771bcbb2b54b86`;
  supervisor PID 37836 and 12 / 12 parented workers are live at round three
  with 60 units and zero wins or bad units. Evidence:
  `reconciliation_post418_pr419final_0b79a8af.json`,
  `post418-pr419final-pre-0b79a8af-20260722T124401Z` and
  `post418-pr419final-live-0b79a8af-20260722T124700Z`; all PR #419 functions
  are excluded through their five owning source units. Nine stale directories
  from those owners were archived remotely after their candidates were
  preserved.
Integrated: 13 natural typed GX helpers, 12 MusyX DSP-control helpers, four
  typed HSD helpers, and the zero-relocation particle byte-stream time decoder
Type recovery: a shared GX context preserves the retail field offsets and
  confines volatility to the 0xCC008000 FIFO and 0xCC000000 CP MMIO; the
  MusyX DSPvoice, DSPstudioinfo, parameter block, and studio-input layouts are
  consolidated from the v2.0.0 ABI (`DSPvoice` 0xF4, `DSPstudioinfo` 0xBC);
  HSD class/string declarations use their authentic array/object types
Strict rejection gate: `GXSetBlendMode` and `GXSetDstAlpha` were byte-exact
  only with direct `__rlwimi` instruction shaping. Natural mask and bitfield
  forms were not exact, so both remain residual and are not counted. The
  report-100 `fn_800BD454` also remains residual because its normalized
  relocation owner differs from retail.
Residual policy: five affected original ranges become 15 Matching units plus
  19 explicit non-auto CodeCandidate ranges; coverage is gapless and disjoint
Validation: all 30 functions / 2,920 bytes are raw 100%; all 15 Matching units
  emit target-identical text and exactly 85 normalized target/source
  relocations; every selected symbol survives in main.elf at its retail VA and
  size, with 154 genuine incoming relocations in aggregate; `FORCEACTIVE` is
  empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, text and relocation parity,
  liveness, ABI/type fidelity, residual topology, strict-policy compliance,
  farm-owner exclusion, zero regression, and the full retail link
Next action: publish and merge PR #419 after final review, rotate Windows from
  the merged head, then integrate the next vetted exact-30 batch
```

### Batch snapshot — 2026-07-22 (PR #420)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 89e2aa8c (PR #419 merged)
Batch branch: `integration/pr420-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +17 complete units / +30 total-unit topology /
  +1,304 linked code / +0 linked data
Goal cumulative from PR #381: +873 linked functions / +254 complete units /
  +70,120 linked code / +193 linked data
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,641 functions; 939 / 1,698 units;
  636,280 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.50% linked / 636.28 kB
  complete code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: no queue, process, or remote state was touched by this integration
  worktree; reconciliation and rotation remain a separate campaign operation
Integrated: three NPC event-queue/callback helpers; seven GBA status, counter,
  bit-extraction, and ready-state helpers; eight graphics state/frame helpers;
  seven MusyX DSP, AI, IRQ, allocator, and reverb helpers; four PC-box capacity
  and status helpers; and the mailbox receive-order lookup
Type recovery: the NPC callback context is an evidenced 13-pointer queue with
  its cursor at 0x34; the GBA retry storage is the six-u16 0xC-byte SDA object;
  graphics state 0x0C is a render-target pointer with 0xFEFEFEFE as its empty
  sentinel, not a packed clear color; PC-box capacities preserve their u16/s8
  caller ABI; and the 0x1E0 `SND_AUX_REVERBHI` layout and update routine follow
  the canonical MusyX runtime types and field order
Residual policy: the six affected original ranges become 17 Matching units
  plus 19 explicit non-auto CodeCandidate ranges; coverage is gapless and
  disjoint
Validation: all 30 functions / 1,304 bytes are raw 100%; all 17 Matching units
  emit target-identical text and all 47 normalized target/source relocations;
  every selected symbol survives in main.elf at its retail VA and size, with
  613 genuine incoming relocations in aggregate; `FORCEACTIVE` remains empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS after replacing two scalar-pointer GBA aliases with
  the evidenced six-u16 small-data array and replacing raw graphics offsets
  with typed `xfbCount`, `xfbIndex`, and signed `mode` field access
Next action: independently audit, publish, and merge PR #420; then reconcile
  and rotate Windows from the merged head before selecting exact-30 #421
```

### Batch snapshot — 2026-07-22 (PR #421)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 1e9fbb29 (PR #420 merged)
Batch branch: `integration/pr421-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +14 complete units / +0 total-unit topology /
  +2,620 linked code / +0 linked data
Goal cumulative from PR #381: +903 linked functions / +268 complete units /
  +72,740 linked code / +193 linked data; 97 linked functions remain in the
  current +1,000 campaign
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,671 functions; 953 / 1,698 units;
  638,900 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.61% linked / 638.90 kB
  complete code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: post-#420 reconciliation classified all 39 cumulative artifacts as
  32 already exact plus the same seven strict rejects, with zero recoverable
  survivors. All stale farm processes were stopped before rotation. The fresh,
  owner-disjoint queue has 60 runnable units across 44 objects, queue SHA-256
  `7cc0baca6f575aabfa01068d030929b749a97e18fa5dc47dde191cb20227789e`
  and manifest SHA-256
  `f9faa1a6daf4811ce41a99217e0f5d9fce7ca5da80c06bf4bdb61175e0b905b5`;
  supervisor PID 61800 and 12 / 12 workers were live at round three with zero
  wins or bad units at the rotation checkpoint. None of the 30 functions in
  this batch overlaps that queue.
Kimi: the full-owner compile-loop trial remains fail-closed behind an
  adversarial source-policy audit; no Moonshot request, token, or spend is
  permitted before explicit PASS. The API remains at zero requests and zero
  spend at this snapshot.
Integrated: 18 typed camera render, state, movement, and vector helpers; one
  material-alpha helper; canonical SDK `__CARDSetDiskID`; two NPC
  flag/model-vector helpers; one fight-action display-buffer setter; four
  encounter trainer-slot accessors; and two tool-entry Pokémon pointers plus
  one battle counter getter
Type recovery: the camera floor table, 0xFC camera state, render camera/vector,
  encounter trainer slot, fight action, material, NPC model-vector return, and
  tool-entry battle-state layouts preserve their caller ABI and evidenced field
  offsets. The camera target initializer is a typed `GSSceneVec3` rodata prefix,
  and the SDK disk ID uses the existing `CARDControl` / `DVDDiskID` ABI.
  `fn_8025DBB0` names offset 0x14 as `count_14`, consistently with the
  neighboring exact unit; the historical `peopleGetPosition` symbol name is
  retained for address traceability while its pointer-return ABI and rotation
  accessor behavior are explicit.
Residual policy: 14 existing CodeCandidate ranges become one-for-one Matching
  units; total topology is unchanged and all neighboring residual coverage
  remains gapless and disjoint
Validation: all 30 functions / 2,620 bytes are raw 100%; all 14 Matching units
  emit target-identical text and exactly 118 normalized target/source
  relocations; every selected symbol survives in main.elf at its retail VA and
  size, with 185 genuine incoming relocations in aggregate and none at zero;
  `FORCEACTIVE` remains empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, direct text and relocation
  parity, typed semantics and ABI, linker liveness, residual topology, strict
  policy, farm-owner exclusion, zero regression, and the full retail link
Next action: obtain independent PASS, publish and merge PR #421, reconcile and
  rotate Windows from the merged head, then integrate exact-30 #422
```

### Batch snapshot — 2026-07-22 (PR #422)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 3acb75f1 (PR #421 merged)
Batch branch: `integration/pr422-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +19 complete units / +8 total-unit topology /
  +2,204 linked code / +0 linked data
Goal cumulative from PR #381: +933 linked functions / +287 complete units /
  +74,944 linked code / +193 linked data; 67 linked functions remain in the
  current +1,000 campaign
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes
Head linked: 4,701 functions; 972 / 1,706 units;
  641,104 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.69% linked / 641.10 kB
  complete code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: post-#421 reconciliation classified all 39 preserved artifacts as
  32 already exact plus seven strict rejects, with zero recoverable survivors.
  The old Windows processes were stopped before rotation. The fresh,
  owner-disjoint queue has 60 functions across 45 objects, queue SHA-256
  `5a6d5579e68725bcf1975f17cda7918184200f4a05767ef1b9acd97ea9af0a4f`
  and manifest SHA-256
  `9ee1e38ed3c84f701403180720d5993dfee0d4b713d3d367042f3fff782f1b9e`;
  supervisor PID 53508 and 12 / 12 workers were live at round three with zero
  wins or bad units at the rotation checkpoint. None of this batch's 30
  functions overlaps that queue.
Kimi K3: the full-owner source guard passed all 67 tests, an independent audit
  of 60 rejected mutations and nine accepted variants, semantic64, and the
  final 1,000 / 1,000 ModernGekko differential fixtures before any API use.
  The bounded pilot made exactly three Moonshot requests, all served by
  `kimi-k3`: 65,217 prompt tokens (38,912 cached), 17,985 completion tokens,
  and 83,202 total. The API returned no billed-dollar field; the official-rate
  pre-tax list-cost estimate is $0.3603636. The guard rejected two noncanonical
  outputs before MWCC and the third response ended at its length limit without
  a function, yielding zero compiled candidates and zero bankable source wins.
  The audited guard/tooling is retained on its isolated tooling branch for a
  separate tooling-only review; no model response or generated report enters
  this source batch.
Integrated: 16 typed item-parameter flag accessors; canonical SDK
  `CARDGetSerialNo`; cosine-table initialization and lookup; two texture-state
  setters; window allocation access; closest-point line math; particle
  generator SRT allocation; camera animation synchronization; a fight-action
  setup helper; the battle-timer thread; and three MusyX hardware wrappers
Type recovery: the item bitfields use the shared `ItemParamData` layout; the
  CARD function reuses the canonical private SDK control block and ID layout;
  camera uses the evidenced 0xFC state and emits only its retail function; and
  window, texture, particle, fight-action, timer, and vector types preserve
  caller ABI and corroborated field offsets.
Residual/data policy: the window and MusyX ranges remain contiguous and
  disjoint after splitting. Compiler-owned conversion constants at 0x8047CB40
  and 0x8047E6E0 are paired with their authentic text owners, while new suffix
  units preserve every trailing byte. `pokemonGetDp` was rejected because its
  shared 0x8047D008 symbol could not be linked naturally and remains
  `CodeCandidate`.
Validation: all 30 functions / 2,204 bytes are raw 100%; all 17 selected text
  units have equal target/source sizes and all 61 normalized relocations pair.
  Every symbol survives in main.elf at its retail VA and size, with 145 genuine
  incoming relocations in aggregate and none at zero; `FORCEACTIVE` remains
  empty. The camera object contains no dead private sibling.
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, direct text and relocation
  parity, typed semantics and ABI, linker liveness, residual/data topology,
  strict policy, Windows owner exclusion, zero regression, and the full retail
  link
Next action: publish and merge PR #422, reconcile and rotate Windows from the
  merged head, bank the audited Kimi guard separately, then integrate exact-30
  #423 toward the remaining 67 functions
```

### Batch snapshot — 2026-07-22 (PR #424)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 7c01bc79 (PR #423 merged)
Batch branch: `integration/pr424-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +25 complete units / +44 total-unit topology /
  +1,756 linked code / +0 linked data
Goal cumulative from PR #381: +963 linked functions / +312 complete units /
  +76,700 linked code / +193 linked data; 37 linked functions remain in the
  current +1,000 campaign
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes; fuzzy progress 57.809658%
Head linked: 4,731 functions; 997 / 1,750 units;
  642,860 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.76% linked / 642.86 kB
  complete code; decomp.dev publication is checked again after merge
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: post-#422 reconciliation classified all 39 preserved artifacts as
  32 already exact plus the same seven strict rejects, with zero recoverable
  survivors. The old supervisor and 12 orphan workers were stopped before
  rotation. The fresh owner-disjoint queue has 60 functions across 46 objects,
  queue SHA-256
  `188f40770a2411d3a75b8e330bf5817d0defe19bab8365a37dc72b78d9c5fcd2`
  and manifest SHA-256
  `8c85f5d40c7b310cbca7083501c79b7e607bbe9de2be6d967034efb0d097f8ae`;
  supervisor PID 15728 and 12 / 12 workers were live at the rotation checkpoint
  with zero wins or bad units. None of this batch, the raw-100 pool, the Kimi
  target, or historical owners overlaps that queue. The queue is rotated again
  only after this batch merges.
Kimi K3: the PR #423 full-owner guard is merged. One bounded compile-loop
  request tested `GSmaterialCreate` at a 97.386% baseline using 7,644 prompt
  and 8,192 completion tokens (15,836 total). The completion budget was spent
  almost entirely on reasoning and yielded no compilable primary proposal, no
  score lift, and no bankable source. The trial stopped after that zero-lift
  round; no model response enters this batch.
Integrated: two people field lookups and three friendship-item accessors;
  three fade hooks; three window accessors; four field script helpers; two
  fight-sequence flags; memo count; two mail helpers; camera target refresh;
  two GBA conversion helpers; one MusyX private-ID lookup; the waza viewer and
  window-message helpers; two model-shadow flag helpers; and two collision
  object-enable helpers
Residual/data policy: 25 Matching units and their explicit CodeCandidate
  neighbors preserve gapless, disjoint text ownership. The unrelated four-byte
  GBA `.sdata2` residual remains with its original prefix owner rather than
  being absorbed into either exact unit.
Validation: all 30 functions / 1,756 bytes are raw 100%; all 25 Matching units
  emit target-identical text and all 58 normalized target/source relocations;
  every selected symbol is linker-live at its retail address and size, with
  579 genuine incoming references in aggregate and none at zero; `FORCEACTIVE`
  remains empty
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, direct text and 58 / 58
  relocation parity, typed semantics and ABI, linker liveness, residual/data
  topology, strict source policy, Windows owner exclusion, zero regression,
  and the full retail link
Next action: publish and merge PR #424, reconcile and rotate Windows from the
  merged head, then integrate the next exact-30 batch toward the remaining 37
  functions
```

### Batch snapshot — 2026-07-22 (PR #425)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: b57f183f (PR #424 merged)
Batch branch: `integration/pr425-exact30`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +30 linked functions / +23 complete units / +40 total-unit topology /
  +1,600 linked code / +0 linked data
Goal cumulative from PR #381: +993 linked functions / +335 complete units /
  +78,300 linked code / +193 linked data; seven linked functions remain in
  the current +1,000 campaign
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes; fuzzy progress 57.809658%
Head linked: 4,761 functions; 1,020 / 1,790 units;
  644,460 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.83% linked / 644.46 kB
  complete code; PR #424's decomp.dev publication was confirmed at exact
  merge b57f183f before this batch
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: post-#424 reconciliation found the same 39 cumulative artifacts:
  32 already exact plus seven strict rejects and zero recoverable/new source.
  The stale supervisor and 12 orphan workers were stopped before rotation.
  Of 113 native-smoked fresh candidates, 102 passed and 11 were rejected; the
  final 60 functions across 60 owners then passed a second 60 / 60 native
  smoke. Atomic manifest-last deployment verified all 420 contract files.
  Queue SHA-256 is
  `b7f9409798d02b8282fac2ebb383e5380f449cd3edc22cc492ab776bec400333`
  and manifest SHA-256 is
  `51fd5da9ff74aa78b782fae9be117d5671347727ef3195ae32cdfcee9d0b47f7`.
  Supervisor PID 54284 and 12 / 12 parented workers were live with zero
  orphans, wins, or bad units at the restart checkpoint. The queue has no
  function or owner overlap with this batch, PR #424, the exact/raw-100 pool,
  historical Windows deployments, or the Kimi reservation.
Kimi K3: the audited full-owner guard remains available through the owner-only
  Moonshot key path. No additional request was made in this batch after the
  prior `GSmaterialCreate` zero-lift round; no model output enters source.
Integrated: three GBA command/table helpers; four fight condition/weather
  helpers; five field resource/memcard helpers; four Gapp breakpoint/state
  helpers; one menu error accessor; two camera state helpers; three DVD
  wrappers; and eight MusyX allocator, ARAM, AI, and interrupt helpers
Type recovery: GBA tables use evidenced typed arrays; fight helpers use typed
  weather/move-query prototypes and a compiler-owned jump table; menu, camera,
  field, and memcard accessors name corroborated fields; Gapp stores its
  breakpoint callback as a function pointer; and the MusyX ARAM queue and
  allocator-pair layouts preserve the observed ABI. The sole volatile access
  is the authentic ARAM completion-count wait: an asynchronous callback
  decrements the byte while the foreground thread spins.
Residual/data policy: 22 Matching text units and explicit CodeCandidate
  neighbors preserve gapless, disjoint ownership. The fight switch's 32-byte
  jump table remains paired with its authentic text owner; adjacent prefix and
  suffix data remain target-identical. The canonical GX distance-attenuation
  candidate was rejected because its text-only match required private
  `.sdata2` ownership; linker-live `menuGetLastError` replaced it naturally.
Validation: all 30 functions / 1,600 bytes are raw 100%; all 22 Matching text
  units emit target-identical code. All 58 text and eight fight jump-table
  data relocations pair after address normalization, for 66 / 66 total.
  Every selected symbol survives in main.elf at its retail VA and size, with
  255 genuine incoming active-input relocations in aggregate and none at zero;
  `FORCEACTIVE` remains empty.
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, 58 text plus eight data
  relocation pairs, typed semantics and ABI, authentic asynchronous volatile
  use, linker liveness, residual/data topology, strict source policy, Windows
  owner exclusion, zero regression, and the full retail link
Next action: publish and merge PR #425, reconcile and rotate Windows from the
  merged head, then link the final seven functions in the current +1,000
  campaign
```

### Batch snapshot — 2026-07-22 (PR #426)

```text
Goal baseline: 6f25dc2c (PR #381)
Current master before batch: 67c5c73f (PR #425 merged)
Batch branch: `integration/pr426-exact7`
Exact-source delta from goal baseline: +41 functions / +15,264 code bytes
This batch: +0 matched functions / +0 matched code/data;
  +7 linked functions / +6 complete units / +11 total-unit topology /
  +296 linked code / +0 linked data
Goal cumulative from PR #381: +1,000 linked functions / +341 complete units /
  +78,596 linked code / +193 linked data; the campaign milestone is complete
Head report: 6,309 / 8,603 matched functions; 901,760 matched code bytes;
  2,136,513 matched data bytes; fuzzy progress 57.809658%
Head linked: 4,768 functions; 1,026 / 1,801 units;
  644,756 complete code bytes; 752,777 complete data bytes
README/report: synced at the branch head at 25.84% linked / 644.76 kB
  complete code; PR #425's decomp.dev publication was confirmed at exact
  merge 67c5c73f before this batch
Retired 3090 farm: remains absent and was neither contacted nor recreated;
  Windows remains the sole authorized permutation farm
Windows: post-#425 reconciliation found the same 39 cumulative artifacts:
  32 already exact plus seven strict rejects and zero recoverable/new source.
  The prior supervisor and all 12 workers were stopped without orphans, and
  transient outputs were preserved in the post-#425 archive. A private build
  generated 69 fidelity-valid functions across 46 owners; all 69 passed native
  smoke. The final queue chose the best base-score function from each owner,
  then 14 next-best residuals for 60 functions across 46 owners; all 60 passed
  a second native smoke. Atomic manifest-last deployment verified 420 / 420
  contract files. Queue SHA-256 is
  `7a8fa3aa16b7f9c275967a58aa77aed1e5d1f97ad266aad5dd2c162044898471`,
  manifest SHA-256 is
  `a3b1dc530628cf77ccc208804b8301531375d4091ebc6486ec71cc96d062789c`,
  and final smoke-results SHA-256 is
  `8d0301a41b3ada14677f8138c4f8dfc05467fa8bf84723ad99c8320e0eae1652`.
  Supervisor PID 60548 and 12 / 12 parented workers were live with zero
  orphans, wins, or bad units at the restart checkpoint. All exact/raw-100,
  historical, Kimi, PR #425, and PR #426 function and owner overlaps are zero.
Kimi K3: the audited full-owner guard remains available through the owner-only
  Moonshot key path. No additional request was made while closing the exact
  +1,000 milestone; the next fuzzy campaign will use a fresh compile-loop
  target rather than repeat the zero-lift `GSmaterialCreate` trial.
Integrated: a constant event callback and event-state output copier; two typed
  summary alpha callbacks; the window sequence movement check; one FSYS state
  wrapper; and the canonical HSD vertex-descriptor clear wrapper
Type recovery: the summary callbacks use the evidenced page context and draw
  item color field. The window checker uses tagged window/sprite layouts, a
  typed `WinSeqCommand` pointer, and the existing `enabled` state name rather
  than raw offsets or a guessed flag meaning.
Residual/data policy: six Matching text islands and explicit CodeCandidate
  neighbors preserve gapless, disjoint ownership; no data range moves.
Validation: all seven functions / 296 bytes are raw 100%; all six Matching
  units emit target-identical text and all seven normalized relocations pair.
  Every selected symbol survives in main.elf at its retail VA and size, with
  13 genuine incoming active-input relocations in aggregate and none at zero;
  `FORCEACTIVE` remains empty.
Regression/link: all 8,603 function scores have zero regressions; full retail
  SHA-1 passes at `870e8b9693ca780782d80f22a6a4572d8ba9458f`
Independent audit: PASS for exact composition, typed semantics and ABI, direct
  text and 7 / 7 relocation parity, linker liveness, residual topology, strict
  source policy, Windows owner exclusion, zero regression, and the retail link
Next action: publish and merge PR #426, confirm decomp.dev at the exact merge,
  then start the 100% fuzzy campaign with a fresh model-assisted compile-loop
  target while the Windows-only farm continues its non-overlapping queue
```

### Fuzzy campaign restart snapshot — 2026-07-25

```text
Master baseline: dae9b326; 57.809658% fuzzy, 901,760 matched code bytes,
  6,309 / 8,603 exact functions, 1,026 / 1,801 linked units
Recovered Claude worktree:
  `.claude/worktrees/decompilation-linking-fuzzy-match-274746`
Recovered handoff head: ac0125b6; 60.544765% fuzzy, 928,372 matched code
  bytes, 6,378 exact functions, 1,026 / 1,804 linked units
Reconciled source head: 697dd7b3 on
  `claude/decompilation-linking-fuzzy-match-274746`
Current report: 60.900250% fuzzy, 931,092 matched code bytes,
  6,384 / 8,603 exact functions, 1,026 / 1,804 linked units
Delta from handoff: +0.355485pp fuzzy, +2,720 matched code bytes,
  +6 exact functions; linked code/data and complete-unit counts unchanged
Delta from master: +3.090592pp fuzzy, +29,332 matched code bytes,
  +75 exact functions; linked code/data and complete-unit counts unchanged

Reconciliation: all Claude scratch lanes, five abandoned agent worktrees, six
  older frontier worktrees, and the dirty fuzzy-batch tree were inspected.
  Existing frontier commits were already ancestors of the recovered head.
  Accepted abandoned survivors were the canonical SI BIOS range, HSD TObj
  reference bodies, fdlibm `__ieee754_sqrt`, and the typed THP audio-buffer
  consumer. Rejected work included the unfinished fdlibm exp body, raw-offset
  THP scratch and its pragma variant, a regressing pointer-walk experiment,
  and a particle candidate with an inverted condition and uninitialized read.

Standards re-audit: prior campaign exclusions are hypotheses, not permanent
  bans. Readable inline helpers, canonical gotos, SDK hardware unions and
  volatility, and legacy MSL/fdlibm word access are admissible when semantics
  and target/compiler/reference evidence agree. Dummy/self assignments,
  uninitialized reads, fake volatility, fabricated constants or layouts,
  semantic changes, `.inc` source, and asm wrappers remain rejected.
  Canonical `GetFontSize`/`ReadFont` helpers replaced an unsupported
  block-scope-extern coercion and made OSLoadFont exact. Unsupported volatile
  qualifiers were removed from VI's ordinary software shadow arrays with no
  score change; real MMIO, interrupt-shared state, and the SDK delay counter
  remain volatile.

New exact source: canonical OSLoadFont (+820 bytes), fdlibm
  `__ieee754_sqrt` (+548), SITransfer (+364), SIGetTypeAsync (+316),
  `__timesdec` (+632), and GXSetClipMode (+40). New partial source includes
  SI transfer/type internals, HSD TObj loading/matrix/animation bodies,
  THP audio consumption at 98.63636%, GXSetGPMetric at 95.76792%, and
  `__two_exp_800DCCB8` at 99.91561%.

HSD bytecode sqrt: the old 96.27564% bespoke volatile-union shape was rejected
  as unsupported. The reference-authentic MSL inline sqrt/classifier is
  96.11863%; canonical extern-inline spelling, constants, expression ordering,
  and meaningful locals produce no lift. The nine-byte-equivalent handoff
  regression is retained for source fidelity; only the artificial volatile
  shape recovered it.

Compiler provenance: `sdk_range_800C5458.c` now uses canonical MSL GC/2.5
  with read-only string pooling. The remaining `__two_exp` mismatch is a
  leading max-double pool string with no semantic consumer in this source
  subset; no unused constant was added to force it.

Candidate topology: ff62ca1e and ac0125b6 remain explicitly disclosed,
  score-driven CodeCandidate splits, not evidence of original TU boundaries.
  They add three report units while preserving 8,603 functions, 2,495,108
  code bytes, and 2,196,100 data bytes. Do not promote them or count them as
  linked progress without independent boundary evidence.

Validation: configure, all_source/report, progress, full retail link and SHA-1
  `870e8b9693ca780782d80f22a6a4572d8ba9458f` pass. Master-to-head regression
  guard reports zero regressions and +75 exact functions. Object-map freeze,
  24 quality-scan tests, source quality scan, README check, and diff check
  pass. No `.inc`, asm-wrapper, generated-product, or invented-data changes
  were accepted.

Next action: continue byte-weighted source work from this head. Prefer
  reference-backed SDK/HSD/MSL ports and large game/menu/people functions;
  leave the documented walls and register-permutation residue alone.
```

### CARDCheck exact range — 2026-07-25

```text
Baseline: 9c641dc2; 60.900250% fuzzy, 931,092 matched code bytes,
  6,384 / 8,603 exact functions, 1,026 / 1,804 linked units
Current report: 60.992270% fuzzy, 933,388 matched code bytes,
  6,388 / 8,603 exact functions, 1,027 / 1,804 linked units
Delta: +0.092020pp fuzzy, +2,296 matched and linked code bytes,
  +4 exact functions, +1 linked unit

Integrated: canonical Dolphin SDK CARDCheck implementations for
  `__CARDCheckSum`, `VerifyID`, `VerifyDir`, and `VerifyFAT`
Evidence: the GC/1.2.5n retail disassembly agrees with the CARDCheck source in
  the local Mario Strikers, Mario Party 4, Prime, and Melee corpora. Struct
  offsets, SRAM flash-ID validation, checksum ranges, directory/FAT selection,
  and return codes were checked against the target before integration.
Inlining: `__CARDCheckSum` is ordinary readable C. Its same-TU definition is
  authentically auto-inlined into the three verification functions; no asm,
  artificial volatile state, dummy assignment, or optimization pragma is used.
Compiler detail: the canonical signed `int` checksum length is required for
  the target's signed divide-by-two sequence. The prior unsigned declaration
  was corrected to the reference signature.
Topology: `sdk_range_800B2070` is promoted from CodeCandidate to Matching only
  after all four functions reached raw 100%; the range is now a complete linked
  unit with target-identical 2,296-byte text.

Next action: continue byte-weighted reference-backed SDK/HSD work; the adjacent
  CARDCreate/CARDRead residual at 0x800B4644 is a promising canonical target.
```
