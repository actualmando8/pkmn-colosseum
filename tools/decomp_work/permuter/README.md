# Windows permuter farm

Native-Windows decomp-permuter farm for the Pokémon Colosseum byte-match
campaign.  Replaces the earlier WSL-based farm, which capped out around 8
effective workers (wibo + WSL2 VM + 9p filesystem overhead, plus
decomp-permuter's fork-only `-j` which cannot run multi-worker on native
Windows).  This farm sidesteps `-j` entirely: it runs N independent
single-threaded permuter processes, one per work unit, all native
(mwcceppc.exe runs directly — no wibo, no wine, no WSL).

## Measured scaling (Ryzen 9 9950X3D, 16C/32T)

The hard ceiling on this box is Windows **process-creation throughput**
(kernel + Defender image-load inspection), not CPU.  Every permuter iteration
spawns child processes; cutting spawns per iteration is what scales:

| configuration | spawns/iter | aggregate iters/s (steady) |
|---|---|---|
| compile.bat → cmd → sjiswrap → mwcc, + objdump | ~5 | ~55 flat from 8..20 workers |
| direct mwcc via `compile_cmd.json`, + objdump | 2 | ~90 at 8–16 workers |

Throughput plateaus by ~10 workers and degrades past ~16, so the farm runs
**12 workers** (breadth over 12 targets; total iteration throughput is the
same from 8 up).  Temp files are kept inside the Defender-excluded farm dir
(`%TEMP%` is redirected per worker) — candidate .c/.o scanning was a
measurable part of the old tax.  Killing worker trees must use
`taskkill /T` (`Stop-Process` on a `py`-launched process orphans the real
python worker — zombie workers silently eating cores are another way a farm
"fails to scale").

## Pieces

| file | where it runs | what it does |
|---|---|---|
| `gen_workunits.py` | Mac | builds self-contained work units from the live tree: preprocesses each queued TU with its **exact** per-unit mwcc + flags (from `build.ninja`), strips all other function bodies, assembles a single-function `target.o` from the dtk asm, and gates everything locally (parse / compile / single-symbol / finite base score) before it ships |
| `farm.py` | Windows | supervisor: keeps N permuter processes running through the queue with per-unit time budgets, writes wins to `outbox/<fn>/`, holds the machine awake while grinding, persists state across reboots |
| `poll_win.sh` | Mac | pulls `outbox/` into `build/permuter_results/win/<fn>/` (`<fn>.c` + `summary.json`); `--loop` polls every 5 min; never commits anything |

## Full-owner extraction mode (local benchmark only)

Some functions do not reproduce when their owner is reduced to one function.
For those cases, `gen_workunits.py --full-owner` provides an explicit,
fail-closed local mode. It is not a Windows farm format and `compile.bat` exits
with an error deliberately.

The mode uses an explicitly attested Clang only to resolve preprocessing
directives while retaining active MWCC pragmas. It replaces exactly one target
definition with a target-only clean seed. For `msgctrlWait`, the reviewed
`msgctrlWait-pragma-clean-v1` transform then removes the redundant
`optimization_level 4` and target-shaping `peephole off`, explicitly restores
canonical `peephole on` before the mutable definition, and retains the existing
post-target restore. The shaped owner must still reproduce the live owner
target; the clean-context target must differ from it. The honest clean baseline
is 92.5%, not the refused 99.5% shaped lane.

Both transitions run through `owner_extract.py`. The extractor rejects changes
to sibling text, named/static/data symbols, allocatable non-text sections, or
sibling relocations. Target binding/visibility and `.text` flags, address, and
alignment are immutable fingerprint inputs as well. Compiler-generated local
ASCII `@[0-9]+` names alone may renumber; they are compared by stable
section/value/type identity. It emits one PowerPC ELF function and preserves
only target-owned relocations; a relocation to a sibling or data definition is
rejected. Reported ELF hashes bind the already-audited byte snapshots rather
than reopening mutable paths.

The seed itself must retain the incumbent signature. A real Clang AST over the
authentic, sibling-pruned owner first binds `msgctrlWait` to its reviewed
statement/effect topology: exactly the incumbent two scalar locals, nested
if/else and return layout, assignment destinations, conditions, and observable
values. The only mutable search space is local naming, signed-short versus
signed-int counter storage, optional nonredundant signed-short casts, and the
same canonical direct/one-step nonzero affine leaves. Extra/empty branches,
aggregate locals, unary or nested algebra, neutral operations, reordered
comparisons, conditional expressions, cast round trips, and any changed
effect slot fail closed. A second definite-assignment/provenance/liveness pass
then rejects directives/`_Pragma`, assembly, `.inc`, `goto`/labels, every
unreviewed `__*` compiler intrinsic (the default allowlist is empty), layout
qualifiers, pointer alias tricks, save/write/restore sequences, compound
assignment, function calls, dead locals/stores, and reads before definition.
Unsupported AST/control-flow shapes never fall back to lexical acceptance.

Example (all named queue/seed/output files are ignored build artifacts):

```sh
python3 tools/decomp_work/permuter/gen_workunits.py \
  --queue build/k3_msgctrlWait_owner_queue.tsv \
  --outdir build/permuter_workunits/k3-full-owner-msgctrl-clean-20260722 \
  --only msgctrlWait \
  --full-owner \
  --owner-target-source build/k3_msgctrlWait_clean_seed.c \
  --force
```

Schema-2 admission separately binds the retail scorer target, live/shaped
target, and pragma-clean/extracted target fingerprints. `meta.json` also binds
the sanitizer/version and transitive inputs, clean seed, context transform,
candidate policy, generator/source guard, extractor, exact compiler/wrappers,
and both sibling audits. The executable `compile.sh` is an isolated Python
wrapper: it verifies every tool hash, snapshots base and candidate bytes once,
guards exact context equality, and compiles only those immutable snapshots
under a clean environment. Benchmark hashing remains the same six-file
contract: `base.c`, `compile.sh`, `compile_cmd.json`, `meta.json`,
`settings.toml`, and `target.o`. The fidelity label is
`full-owner-clean-context-extracted-v2`; it never claims equality with the
live/shaped target.

Windows layout: `C:\Users\douglaswhittingham\gamecube-decomp\pkmn-permuter\`
(`decomp-permuter/` patched checkout, `tools/` mwcc + sjiswrap + ppc objdump,
`units/`, `state/`, `outbox/`, `logs/`).

## Permuter patches (applied to the shipped checkout)

1. `src/main.py` — use `compile.bat` instead of `compile.sh` when
   `os.name == "nt"`, and skip the POSIX exec-bit check there.
2. `src/preprocess.py` — fall back to reading `base.c` verbatim when no `cpp`
   binary exists (units ship fully preprocessed, so cpp is redundant).
3. `toml.py` shim (stdlib `tomllib`) so no pip installs are needed.

## Operating it

```bash
# Mac: regenerate units after queue/source changes, then re-ship units/
python3 tools/decomp_work/permuter/gen_workunits.py

# Mac: pull results
tools/decomp_work/permuter/poll_win.sh          # once
tools/decomp_work/permuter/poll_win.sh --loop   # every 5 min
tools/decomp_work/permuter/poll_win.sh --status

# Windows (over ssh): farm status / control
ssh win "schtasks /query /tn PkmnPermuterFarm"
ssh win "schtasks /run /tn PkmnPermuterFarm"
ssh win "schtasks /end /tn PkmnPermuterFarm"
ssh win "type C:\Users\douglaswhittingham\gamecube-decomp\pkmn-permuter\state\status.json"
```

The farm runs as scheduled task `PkmnPermuterFarm` (S4U, at-startup trigger,
no time limit, absolute path to
`...\AppData\Local\Programs\Python\Python312\python.exe` — `py` is not on the
task context's PATH), so it survives ssh disconnects and reboots.
Re-register/reconfigure with `register_task.ps1 -Workers N -Budget S` in the
farm dir.  While it runs it
calls `SetThreadExecutionState(ES_SYSTEM_REQUIRED)` so the box will not sleep;
when it exits, normal sleep policy resumes.  `powercfg /change
standby-timeout-ac 0` was also set (AC sleep disabled) — revert with
`powercfg /change standby-timeout-ac 30` if undesired.

A Windows Defender exclusion covers the farm directory (mwcc writes thousands
of short-lived .o files; real-time scanning would tax every compile).

## Win criteria

A **win** is permuter score 0 with `--stack-diffs` (strict stack offsets):
the candidate compiles with the unit's exact compiler + flags and its
disassembly matches the dtk-split target function modulo standard
normalizations (sda21 base-register rewriting, anonymous local symbol names).
Wins land in `build/permuter_results/win/<fn>/` and must be validated against
the full build by a human before landing — the farm never touches `src/`.

Near-misses (score ≤ 25) are banked under `nearwins/` for review.
