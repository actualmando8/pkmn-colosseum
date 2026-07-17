# Compile-loop model benchmark

This benchmark measures whether a model can improve real GC6E01 C candidates
under the exact Metrowerks compiler and objdiff feedback used by the project.
It is a benchmark and scratch generator, not a progress counter: no result is
accepted into `src/` until it passes the normal full-tree validation.

## Inputs and isolation

- Work units come from `tools/decomp_work/permuter/gen_workunits.py`. Its
  fidelity gate proves that isolated `base.c` emits the same target function as
  the full translation unit before the unit is admitted.
- Each model run copies only six allowlisted, hashed work-unit inputs below the
  gitignored `build/model_benchmark/` tree. Run directories are mode `0700` and
  files are mode `0600` (`compile.sh` remains executable).
- The runner rejects `.inc`, `#include`, and inline assembly in both incumbents
  and model responses. The Dolphin SDK paired-single exception is deliberately
  outside this benchmark until it has explicit allowlist metadata.
- Credentials are read in-process from an owner-only `name: value` file. Never
  put an API key on the command line or in an environment variable.

The checked-in suites are pinned to source commit `d9b908ee`. The general pilot
uses eight work units; `suite_semantic_pilot.json` adds the fidelity-gated,
relocation-free `GStextureLockImage` behavioral pilot. Each target has a
SHA-256 over its six admitted files. The runner also verifies
function/fidelity/baseline metadata, recompiles the expected baseline, and
records hashes for the suite, runner, compiler, wrappers, objdump, and objdiff.
Regenerate a suite deliberately after an active source/config change.

## Local validation

```sh
python3 -m unittest tools/decomp_work/benchmark/test_compile_loop.py
python3 tools/decomp_work/benchmark/compile_loop.py score \
  build/permuter_workunits/win-fifty-20260716-next/fn_80034708
```

Render the initial user prompt without making an API call. The system prompt
and provider request parameters are added only by `run`:

```sh
python3 tools/decomp_work/benchmark/compile_loop.py prepare \
  build/permuter_workunits/win-fifty-20260716-next/fn_80034708 \
  -o build/model_benchmark/fn_80034708.prompt.txt
```

## Benchmark conditions

Run single-shot and closed-loop as separate conditions. Use the same pinned
suite, prompt, output cap, and attempt count for every model. Provider-required
sampling differences must be reported; Kimi K3 currently requires maximum
reasoning, temperature 1.0, and full reasoning-history replay, which this runner
preserves.

Example after installing a fresh credential under the `kimi` label:

```sh
# Single shot
python3 tools/decomp_work/benchmark/compile_loop.py run \
  --provider moonshot --model kimi-k3 --rounds 1

# Three-round compile-feedback condition
python3 tools/decomp_work/benchmark/compile_loop.py run \
  --provider moonshot --model kimi-k3 --rounds 3
```

Recommended pilot comparators are Kimi K2.7 Code and the currently available
DeepSeek/GLM coding models. Frontier products accessed through different
agentic harnesses should be labeled separately; their tool autonomy is not an
apples-to-apples API comparison.

Primary metrics count only functions returned in final content. For a separate
diagnostic, `--allow-reasoning-salvage` compiles C found in a reasoning channel,
but excludes it from primary scores, incumbents, feedback, and termination.

## Behavioral-feedback condition

The semantic pilot runs exact-MWCC candidate PPC in the pinned external
Dolphin-interpreter sidecar documented in
[`../semantic_oracle/README.md`](../semantic_oracle/README.md). Generate its
ignored work unit, then provide the clean external checkout and built sidecar
at runtime. Build the sidecar's `moderngekko-dolphin-oracle-attested` target
first; the runner requires its adjacent `.attestation.json`:

```sh
python3 tools/decomp_work/permuter/gen_workunits.py \
  --queue build/permuter_queue_win.tsv \
  --outdir build/permuter_workunits/semantic-target-scan \
  --only GStextureLockImage --force

python3 tools/decomp_work/benchmark/compile_loop.py run \
  --provider moonshot --model kimi-k3 --rounds 3 \
  --suite tools/decomp_work/benchmark/suite_semantic_pilot.json \
  --semantic-checkout /absolute/path/to/ModernGekko \
  --semantic-sidecar \
    build/semantic_oracle/gpl-sidecar/moderngekko-dolphin-oracle
```

The benchmark condition uses 64 balanced deterministic fixtures per round for
latency. The standalone oracle defaults to 1,000 fixtures for the acceptance
gate. Behavioral pass/fail is persisted and included in prompts as a separate,
bounded diagnostic channel. It never changes the objdiff percentage, incumbent
ranking, exact flag, or normal termination; an objdiff-exact/semantic-fail
combination is treated as a harness invariant failure. Any exact isolated lead
still requires the normal full-DOL validation below.

The run configuration fingerprints the sidecar binary and build attestation
once before model work starts. Those exact hashes are passed into every oracle
invocation and must be repeated in each report, along with the expected engine,
pinned provenance, clean-checkout report, and authoritative DOL slice. The
driver itself snapshots the binary once per comparison so reference and
candidate cannot execute different rebuilt pathnames.

The current summary retains exact matches, compile-at-1, best match lift,
tokens, API latency, rounds, served model ID, and failure status. Full link,
full-build regression, and calculated cost are acceptance/future-protocol
metrics, not claims made automatically by this runner. A failed target aborts
the fixed-policy run without retry and is recorded atomically in
`run_status.json`.

`--timeout` is a socket timeout plus an approximate wall budget checked between
stream events; it is not a hard process deadline if a socket stalls near the
limit. Farm supervisors should enforce their own outer wall timeout.

Raw reasoning is replayed in memory when a provider requires complete history,
but is never persisted. Each round stores a SHA-256 of its exact outbound
payload so the otherwise-private history can be distinguished across runs.
For a publishable result, expand to at least 30 stratified held-out functions
and three independent attempts.

Pilot measurements and limitations are recorded in
[`RESULTS_2026-07-16.md`](RESULTS_2026-07-16.md).

## Accepting a candidate

An exact isolated result is only a lead. Reapply the C function in a disposable
worktree, then run:

```sh
python configure.py --no-progress
ninja all_source build/GC6E01/report.json
ninja
python configure.py progress
```

Only source-authentic C that passes those checks can count as campaign progress.
