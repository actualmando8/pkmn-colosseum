# ModernGekko evaluation — 2026-07-16

## Decision

Use ModernGekko's pinned dependency tree only through separately licensed,
external semantic-testing sidecars. The completed pilot executes original and
candidate PPC with Dolphin's interpreter, plus the original through exact
DolRecomp-generated native chunks. Native-original must agree with Dolphin
fixture-for-fixture before candidate feedback is admitted. This helps reject
behaviorally wrong decompilation candidates, but cannot recover matching C or
replace MWCC, dtk, objdiff, or the full-DOL SHA check.

Do not vendor the upstream trees or link their GPL code into the game build or
MIT Python tooling. The small sidecar source/build definition is isolated under
`tools/decomp_work/semantic_oracle/gpl_sidecar`, is separately licensed, and
produces only ignored build output. This is a technical boundary, not legal
advice.

## What the repository does

The audit pinned [ModernGekko commit `11237c1`](https://github.com/ExpansionPak/ModernGekko/commit/11237c119a5d8e907a20e9cae1c357df149aaa47).
Its recursive dependency pins are
[RecompCore `1873066`](https://github.com/ExpansionPak/RecompCore/commit/1873066167f3d03b39771b547f280d2b970427b6) and
[DolRecomp `a2b02e5`](https://github.com/ExpansionPak/DolRecomp/commit/a2b02e5a515fc8971cc551ad51c9e26a9815daad).
Its port tool reads an extracted `sys/main.dol`, invokes DolRecomp, builds the
emitted host C into a shared library, and loads it in a Dolphin-derived runtime.
The relevant orchestration is in
[`moderngekko_port.cpp`](https://github.com/ExpansionPak/ModernGekko/blob/11237c119a5d8e907a20e9cae1c357df149aaa47/tools/moderngekko_port.cpp#L170-L346).

DolRecomp emits low-level `CPUState` instruction state machines in fixed-size
chunks. Those chunks preserve register and memory effects, but they do not
recover source function boundaries, types, structures, loops, expressions,
object splits, or Metrowerks scheduling and register allocation. Consequently,
generated host C must never be counted as decompilation progress.

The useful interface is the guest-state ABI: GPRs, floating-point and
paired-single state, PC/LR/CTR, CR/XER/FPSCR, RAM pointers, and dispatch hooks.
See [`module_abi.h`](https://github.com/ExpansionPak/ModernGekko/blob/11237c119a5d8e907a20e9cae1c357df149aaa47/include/moderngekko/module_abi.h)
and [`cpu_state.h`](https://github.com/ExpansionPak/ModernGekko/blob/11237c119a5d8e907a20e9cae1c357df149aaa47/include/moderngekko/cpu_state.h).

## GC6E01 probe

The exact pinned DolRecomp revision built successfully and was run against the
project's legally supplied original `main.dol`:

| Check | Result |
|---|---:|
| DolRecomp tests | 10 / 10 passed |
| ModernGekko runtime-disabled tests | 31 / 31 passed |
| Instruction words decoded | 625,816 |
| Unknown opcodes | 0 |
| Generated chunks / files | 154 / 157 |
| Generated size | 142 MiB |
| Code-generation wall time | 0.74 s |
| Peak RSS | about 35.7 MiB |
| Possible runtime code-patching ranges | 105 |
| DOL entry point | `0x80003154` |

The original and reconstructed DOL SHA-1 both equaled
`870e8b9693ca780782d80f22a6a4572d8ba9458f`. Zero unknown opcodes establishes
decoder coverage, not semantic correctness.

The native build canonicalized DolRecomp text output to LF and reproducibly
generated 157 files with combined SHA-256
`438b92e6109ac5263860dcbcee24152148c139a34c60bf17933b4253b7309048`,
then rehashed the complete generated tree at final attestation and compiled only
the two reviewed full-DOL chunks. Native-original and Dolphin-original agreed on
all 1,000 deterministic fixtures for both `GStextureLockImage-v2` and
`msgctrlWait-v1`. A known wrong texture candidate still produced 132
candidate-vs-reference mismatches over 64 fixtures while native qualification
remained at zero. The 1,000-fixture `msgctrlWait-v1` corpus SHA-256 is
`690b1b4851d861bd4d8503d5f18e0b3bff147d4d42320722ae38933a23c09350`.

## Semantic-feedback model probe

A separate Kimi K3 probe connected the sidecar to the exact-MWCC/objdiff model
loop for the integer/pointer leaf `GStextureLockImage`. The run is retained at
`build/model_benchmark/20260717T012212.377061Z_moonshot_kimi-k3`.
It predates the final native-original qualification requirement, so it is
evidence for the behavioral-feedback path rather than a current-protocol model
benchmark.

| Stage | Objdiff match | Dolphin-interpreter observations |
|---|---:|---:|
| Existing source baseline | 91.000000% | 132 mismatches / 64 fixtures |
| K3 round 1 | 99.750000% | 132 mismatches / 64 fixtures |
| K3 round 2 | 100.000000% | 0 mismatches / 64 fixtures |

Round one shows why the two authorities are complementary: its instruction
match improved substantially while its wrong return and memory effects
remained. After receiving the bounded behavioral differences, round two
compiled to the exact 48-byte retail text. The candidate and retail `.text`
SHA-256 values are both
`c16b4571e5b98b5da3dd3f66e8e36f653820a5c4e6dee8b4dde8e67739d60a91`.
A current-protocol balanced replay over 1,000 deterministic fixtures returned
zero native-vs-Dolphin and zero exact-candidate mismatches; its report is
`build/semantic_oracle/native-integrated-GStextureLockImage-1000.report.json`.

The API rounds took 99.746 and 101.054 seconds and consumed 14,946 total tokens
(9,670 prompt, including 3,072 cached, and 5,276 completion). At Moonshot's
published K3 rates used for this evaluation, the estimated cost is about
$0.100, treating reasoning tokens as output tokens.

The model expressed the recovered offsets with aliasing casts because the
supplied structure declaration was stale. The source-authentic landing instead
corrects `GStextureHandle` and uses normal `mipData` and `refCount` member
access. The source-authentic form subsequently passed the source-tree objdiff
report at 100.0%, adding one matched function and 48 matched code bytes, and the
normal full build passed the canonical DOL SHA-1 check. Those remain the
acceptance authorities. The surrounding `gs_texture` translation unit is still
partial and is not newly linkable.

This is one function with no repeated independent trials. It validates the
feedback plumbing and gives one positive convergence example; it does not
establish general K3 performance or superiority over another model.

## Correctness risk

ModernGekko is young: at the audited revision it had no releases or CI workflow
and only a small commit history. Its
[`PROVENANCE.md`](https://github.com/ExpansionPak/ModernGekko/blob/11237c119a5d8e907a20e9cae1c357df149aaa47/PROVENANCE.md)
also calls out the generated origin of much of the initial code.

More importantly, the pinned DolRecomp lineage has known emitter problems.
[DolRecomp issue #8](https://github.com/ExpansionPak/DolRecomp/issues/8) reports
84 defects found through differential testing, including floating-point,
paired-single, FPSCR, load/store, lazy-FP, and cycle-accounting behavior.
[PR #9](https://github.com/ExpansionPak/DolRecomp/pull/9) adds differential
tests and documents remaining expected failures. ModernGekko does not yet pin
all later native-execution fixes.

Start with integer/bit/control-flow leaf functions. The current implementation
uses Dolphin's interpreter as the candidate gate and exact DolRecomp chunks as
a native-original qualification lane. The pinned full RecompCore module path is
not used: its emitted include/runtime CPU layouts diverge, its recorded
RecompCore identity is stale, and generated chunks do not consistently charge
`downcount`. The selected standalone DolRecomp CPU ABI is internally
consistent, but every enrolled function still has to prove parity with Dolphin;
absence of a built-in lockstep failure is not evidence.

The first two profiles are also deliberately no-stack leaves. Whole-RAM
comparison will reject an otherwise equivalent candidate that writes a legal
stack spill slot, even if it restores `r1`; stack-using profiles need an
explicit scratch-window contract before admission.

## Implemented sidecar protocol

The useful comparison has two independent execution paths:

```text
original DOL -> Dolphin interpreter --------+
                                             | native qualification (must equal)
original DOL -> DolRecomp native chunk ------+

original DOL -> Dolphin interpreter --------+
                                             | candidate diagnostics
candidate C -> exact MWCC -> Dolphin --------+
                    |
                    +-> objdiff + full DOL remain matching authorities
```

Time-box the pilot to five to ten small integer leaves:

1. Install exact ModernGekko/RecompCore/DolRecomp commits outside the repo.
2. Seed registers and RAM with deterministic edge cases plus randomized cases.
3. Prove the oracle rejects deliberately wrong candidates.
4. Compare original and candidate return registers and touched memory over at
   least 1,000 fixtures per function.
5. Feed only compact state mismatches, target assembly, compiler diagnostics,
   and objdiff feedback into the model loop.
6. Retain each native profile only while it agrees with Dolphin; measure model
   convergence separately on held-out functions.

Candidate C is compiled by the exact project MWCC into relocation-free PPC and
placed in a bounded synthetic MEM1 code sandbox. Executing candidate host C
directly would introduce 32-bit big-endian guest versus 64-bit little-endian
host pointer and layout hazards. Relocation-bearing functions remain ineligible
until a relocation-aware loader is implemented.

No ModernGekko-generated code, game assets, objects, or build products belong
in version control.
