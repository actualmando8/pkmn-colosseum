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

Poll before restarting or replacing queues so results are not stranded:

```bash
tools/decomp_work/permuter/poll_3090.sh
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

### Refresh the 3090 farm

The normal near-match band is 80–100%. Generate the queue and exact compile
manifest from the current report:

```bash
ninja all_source build/GC6E01/report.json
python3 tools/decomp_work/permuter/gen_queue_3090.py --min-pct 80
python3 tools/decomp_work/permuter/extract_unit_flags.py
```

Ship a faithful mirror of source, headers, config, live base objects, target
objects, and asm to the user-owned 3090 box. These generated/extracted files
are farm inputs only and must never be staged.

```bash
HOST=douglaswhittingham@192.168.50.101
BASE=/storage/finetune/pkmn-colosseum-2026

ssh "$HOST" "bash $BASE/farm/stop_farm.sh --requeue"
rsync -az --delete src/ "$HOST:$BASE/repo/src/"
rsync -az --delete include/ "$HOST:$BASE/repo/include/"
rsync -az --delete config/ "$HOST:$BASE/repo/config/"
rsync -az --delete build/GC6E01/src/ "$HOST:$BASE/repo/build/GC6E01/src/"
rsync -az --delete build/GC6E01/obj/ "$HOST:$BASE/repo/build/GC6E01/obj/"
rsync -az --delete build/GC6E01/asm/ "$HOST:$BASE/repo/build/GC6E01/asm/"
rsync -az configure.py build.ninja objdiff.json README.md "$HOST:$BASE/repo/"
rsync -az build/permuter_queue_3090.tsv build/permuter_units_3090.json \
  "$HOST:$BASE/repo/build/"
rsync -az tools/decomp_work/permuter/3090/ "$HOST:$BASE/farm/"

ssh "$HOST" \
  "cp $BASE/repo/build/permuter_queue_3090.tsv $BASE/farm/queue.tsv && \
   WORKERS=21 BUDGET=10800 bash $BASE/farm/launch_farm.sh"
tools/decomp_work/permuter/poll_3090.sh
```

Keep existing terminal state during an ordinary refresh: old targets remain
terminal and only newly added queue entries run. Archive/reset state only for
an intentional second-round campaign, and say so in the handoff.

Healthy 3090 status is 21 workers, 21 permuters, 21 timeouts, and 21 active
claims. Zero workers with a nonempty queue usually means every queued name is
terminal; expand/refill the queue instead of repeatedly relaunching it.

### Refresh the Windows farm

Generate fidelity-gated work units from a queue. Use a band disjoint from the
3090 when possible; after the 3090 takes 80–100%, Windows can explore 70–80%.

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
3. reconcile newly exact names out of both farm queues/manifests;
4. pull any results produced during CI;
5. remove merged/rejected worktrees and prune registrations;
6. delete only merged/rejected branches;
7. refill both farms with new, preferably non-overlapping targets;
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
- **3090 relaunch exits immediately:** queue entries are terminal. Add new
  targets; do not loop the launcher.
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
3090 workers / queue / wins pending:
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
README/report: synced on PR #403; decomp.dev pending merge ingestion
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
