# ModernGekko semantic-oracle pilot

This pilot executes integer-leaf PowerPC with identical deterministic state in
three lanes: original code in Dolphin's interpreter, the same original code as
DolRecomp-generated native host code, and candidate code in Dolphin's
interpreter. Native-original must agree with Dolphin before candidate feedback
is admitted. This is a semantic cross-check, not a replacement for byte
matching, objdiff, or normal full-tree validation.

The dependency-free Python driver is covered by the repository's MIT license.
The source in `gpl_sidecar/` builds separate GPL executables against
ModernGekko's pinned Dolphin and DolRecomp dependencies. The Python process
communicates with them only through versioned JSON files; it does not import or
link GPL code.

## Pinned checkout and build

`moderngekko_pins.json` records exact commits for ModernGekko, RecompCore, and
DolRecomp. The verifier checks each HEAD, origin URL, parent gitlink, all
recursive submodule states, and staged, tracked, and untracked files. Keep the
checkout pristine and build out of tree.

The CMake project repeats the literal commit checks while configuring, then
runs the full clean pin verifier before and after linking. The aggregate target
writes an attestation beside each executable. The native executable also has a
generated-source manifest binding the exact DOL, DolRecomp binary, commits,
complete generated tree, and selected chunks. Every result embeds the three
commits and an engine identity; the driver rejects drift in either engine,
binary, attestation, the generated manifest, or the 4,096-byte code sandbox.

This attestation detects accidental input drift and binds one local build; it
is not signed and does not defend against a malicious user who can replace the
verifier, binary, and attestation together.

```sh
git clone --recurse-submodules https://github.com/ExpansionPak/ModernGekko.git \
  /absolute/path/to/ModernGekko
git -C /absolute/path/to/ModernGekko checkout --detach \
  11237c119a5d8e907a20e9cae1c357df149aaa47
git -C /absolute/path/to/ModernGekko submodule sync --recursive
git -C /absolute/path/to/ModernGekko submodule update \
  --init --recursive --checkout

python3 tools/decomp_work/semantic_oracle/driver.py verify-pins \
  --checkout /absolute/path/to/ModernGekko

cmake -S tools/decomp_work/semantic_oracle/gpl_sidecar \
  -B build/semantic_oracle/gpl-sidecar -G Ninja \
  -DMODERNGEKKO_SOURCE_DIR=/absolute/path/to/ModernGekko \
  -DORACLE_DOL=/absolute/path/to/GC6E01/main.dol \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/semantic_oracle/gpl-sidecar \
  --target moderngekko-semantic-oracles-attested
```

Do not build inside the external checkout: the verifier intentionally rejects
untracked build files.

## Authority and comparison gates

The pilot is deliberately limited to reviewed relocation-free integer/control
profiles:

- `msgctrlWait-v1`: `0x80132454`, `0x78` bytes;
- `GStextureLockImage-v2`: `0x800EF548`, `0x30` bytes;
- `fn_801A6DA0-v1`: `0x801A6DA0`, `0x24` bytes.
- GC6E01 `main.dol` SHA-1:
  `870e8b9693ca780782d80f22a6a4572d8ba9458f`

Before invoking Dolphin, the driver parses the original DOL header and requires
the reference object's extracted `.text` to equal that exact DOL slice. Both
objects must have no ELF relocations, as reported by the configured
`powerpc-eabi-readelf`. Candidate text may have a different word-aligned length
within the 4,096-byte limit; the driver never pads or rewrites it. The sidecar
installs each image in—and excludes the same fixed, validated 4,096-byte
sandbox from—each whole-RAM digest, so length alone cannot create a mismatch.

The native lane executes only the pinned original full-DOL chunks. Its adjacent
manifest must identify the deterministic 157-file output tree with combined
SHA-256 `438b92e6109ac5263860dcbcee24152148c139a34c60bf17933b4253b7309048`.
Generated text is canonicalized to LF, and final build attestation recomputes
the complete tree and selected-file hashes from the sources actually compiled.
For every fixture, native status, return PC, requested GPRs, watched RAM, and
whole-MEM1 digest must equal the Dolphin original. A single disagreement aborts
before candidate execution. Native results are never used as candidate
feedback; disagreement fails closed before any candidate feedback is created.

The fixed seed generates 1,000 fixtures by default. `msgctrlWait-v1` starts
with ten named boundary cases; `GStextureLockImage-v2` starts with eight levels
covering 0, 1, 7, 8, 255, upper-register noise, and 16-bit reference-count
wraparound. Version-stable xorshift cases follow. Each profile declares its
register/RAM setup and watches. The comparison contract is:

- execution status and final return PC;
- profile-requested GPRs (`r3` where declared, none for void
  `fn_801A6DA0-v1`);
- readable object and stream RAM watches;
- FNV-1a-64 over all MEM1 except the fixed 4,096-byte code sandbox.

The completed native qualification replay passed all 1,000 balanced
`GStextureLockImage-v2` fixtures with zero native-vs-Dolphin mismatches, then
rejected the known wrong candidate with 132 observable mismatches over 64
fixtures. A fresh 1,000-fixture `msgctrlWait-v1` replay also returned zero
native-vs-Dolphin and zero authoritative-candidate mismatches. Its deterministic
corpus SHA-256 is
`690b1b4851d861bd4d8503d5f18e0b3bff147d4d42320722ae38933a23c09350`.

`fn_801A6DA0-v1` covers null `mobj`, null `tobj`, and valid insertion with
both null and non-null old list heads. It watches complete `HSD_MObj` and
`HSD_TObj` regions, including both `+0x08` pointer fields, and intentionally
declares no GPR equality contract for this void function. Its seed
`0x6c6f7373` 1,000-fixture corpus has SHA-256
`45f612466208aef474c6f2c89b3a5a1bbe9009dd886e20d97ea70768cc3858bc`.
The current baseline passed with zero native-vs-Dolphin and zero candidate
mismatches. The archived known-bad partial was rejected with 1,248 semantic
mismatches, including null-`tobj` alerts and incorrect `tobj+0x08` state.

Digest mismatches include the first bounded before/after RAM change from each
run. The sidecar also limits its diagnostic change list to 32 spans and 512
bytes. Volatile scratch registers, CR, XER, CTR, and LR are intentionally not
equality gates.

The current profiles are no-stack leaves. A semantically equivalent candidate
that spills to a legal stack scratch slot will change the whole-RAM digest and
false-fail even if it restores `r1`; do not enroll stack-writing candidates
until a profile declares and separately validates an excluded scratch window.

## Run the pilot

Generate the fidelity-gated work unit with the project's existing work-unit
tooling, compile its current C, then compare the authoritative target object
with that candidate:

```sh
python3 tools/decomp_work/permuter/gen_workunits.py \
  --queue build/permuter_queue_win.tsv \
  --outdir build/permuter_workunits/semantic-target-scan \
  --only GStextureLockImage --force

d=build/permuter_workunits/semantic-target-scan/GStextureLockImage
"$d/compile.sh" "$d/base.c" unused "$d/candidate.o"

python3 tools/decomp_work/semantic_oracle/driver.py run \
  --profile GStextureLockImage-v2 \
  --checkout /absolute/path/to/ModernGekko \
  --sidecar build/semantic_oracle/gpl-sidecar/moderngekko-dolphin-oracle \
  --native-sidecar build/semantic_oracle/gpl-sidecar/moderngekko-native-oracle \
  --reference-elf "$d/target.o" \
  --candidate-elf "$d/candidate.o" \
  --report-file build/semantic_oracle/GStextureLockImage-report.json
```

The default `--dol` is `orig/GC6E01/sys/main.dol`. The default PowerPC tools
are read from the campaign tool cache; override them with `--objcopy` and
`--readelf` when needed.

`msgctrlWait` is context-sensitive under isolated compilation. Generate its
ignored benchmark unit with the explicit full-owner command documented in
[`../permuter/README.md`](../permuter/README.md). Its `compile.sh` exact-MWCC
compiles the complete pragma-clean owner from an immutable guarded snapshot,
rejects sibling drift, and emits the single relocation-free candidate ELF
expected by `msgctrlWait-v1`. The semantic reference remains the retail target
and authoritative original-DOL slice; the refused 99.5% shaped target is never
used as the candidate baseline. The benchmark runner binds that semantic
profile directly to `msgctrlWait`; this remains a scratch feedback gate, not
campaign acceptance.

Requests, results, extracted text, reports, the original DOL, objects, generated
native C, and both executable/attestation sets are local-only artifacts. Keep
them under the ignored `build/` tree or outside the repository and never commit
them. The driver snapshots both attested executables in its private run
directory and hashes each snapshot after execution. The benchmark additionally
passes its run-start binary, attestation, and generated-manifest fingerprints
into every driver call and
rejects any report, engine, provenance, checkout authority, or DOL authority
that disagrees. By default, request/result files live in a private temporary
directory and are deleted after the run. The four Dolphin request/result files
can be retained by providing all four path options under `build/` or outside the
repository; native qualification request/result files always remain private
temporaries.

## Tests and interpretation

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest -v \
  tools/decomp_work/semantic_oracle/test_driver.py
```

The tests use fake binutils, a synthetic DOL, and fake external results; they
do not need ModernGekko or extracted game files. They cover pin verification,
reference authority, deterministic profile layout, relocation rejection, safe
file round trips, immutable executable snapshots, dual build attestations,
generated-manifest binding, variable-length sandbox normalization,
engine/provenance rejection, return-PC anchoring, register/RAM diagnostics, and
whole-RAM digest mismatches.

An equal result means the two functions behaved the same over this finite
fixture set and observable contract. It is useful evidence, not a proof of
equivalence. Any candidate proposed for `src/` still needs the repository's
standard configure, build/report, full build, and progress checks.
