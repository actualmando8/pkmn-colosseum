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

### Last reconciled snapshot — 2026-07-21

```text
Goal baseline: 6f25dc2c (PR #381)
Current master: 4b68d012 (PR #387 merged)
Campaign head: PR #388 (integration/fifteenth-refill-20260721)
Open PRs: #388 only
Exact-source delta from goal baseline: +7 functions / +3,036 code bytes
PR #388 delta from master: +4 exact functions / +1,796 matched code;
  +30 linked functions / +7 units / +5,664 linked code / +0 linked data
Newly linked function delta from goal baseline: +41
Newly linked unit/code/data delta from goal baseline: +13 units / +7,480 code / +177 data
Head report: 6,275 / 8,603 matched functions; 889,532 matched code bytes
Head linked: 698 / 1,301 units; 573,640 complete code bytes
README/report: synced on the final 30-function fifteenth-refill head
Retail SHA: 870e8b9693ca780782d80f22a6a4572d8ba9458f
3090: pre-merge current-master queue has 543 entries and 21 workers; no pending wins;
  refresh to the merged #388 head after integration
Windows: pre-merge disjoint batch has 49 active-valid units and 12 workers; no
  pending wins; refresh to the merged #388 head after integration
Active worktrees: fifteenth integration plus validated next-batch MusyX/effect
  scouts and clean/rejected audit trees queued for post-merge reconciliation
Banked next batch: 8a31a866 (MusyX dataInsertKeymap/dataInsertLayer, +2 functions /
  +1,080 linked code) and b050e547 (effect teardown callback, +1 / +96)
Rejected: fight_timer (private conversion constants break canonical relocations);
  THP decoder (293 relocation mismatches, extra data, and asm outside allowlist);
  pokemonGetDp (private conversion constant); fight residuals that require register,
  inline, pragma, jump-table, or volatile-only shaping
Next unit closers: integrate the three banked MusyX/effect functions, then audit
  strict CObjLoad and HSD JObj islands against full relocation/data ownership
```
