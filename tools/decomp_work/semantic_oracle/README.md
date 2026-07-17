# ModernGekko semantic-oracle pilot

This pilot executes original and candidate integer-leaf PowerPC code in
Dolphin's interpreter with identical deterministic state. It is a semantic
cross-check for compile-in-the-loop work, not a replacement for byte matching,
objdiff, or the normal full-tree validation.

The dependency-free Python driver is covered by the repository's MIT license.
The source in `gpl_sidecar/` builds a separate GPL-2.0-or-later executable
against ModernGekko's RecompCore/Dolphin fork. The Python process communicates
with that executable only through versioned JSON files; it does not import or
link GPL code.

## Pinned checkout and build

`moderngekko_pins.json` records exact commits for ModernGekko, RecompCore, and
DolRecomp. The verifier checks each HEAD, origin URL, parent gitlink, all
recursive submodule states, and staged, tracked, and untracked files. Keep the
checkout pristine and build out of tree.

The CMake project repeats the literal commit checks while configuring, then
runs the full clean pin verifier immediately before and after linking. The
attested build target writes
`moderngekko-dolphin-oracle.attestation.json` beside the executable. That file
binds the binary SHA-256, `dolphin_oracle.cpp`, this sidecar's `CMakeLists.txt`,
the attestation tool, and the exact clean pinned source identity. Every result
embeds those three commits and the engine identity
`dolphin-interpreter-from-moderngekko-tree`; the Python driver rejects either
result if that provenance differs from the selected manifest. It also requires
the sidecar to report the expected 4,096-byte code sandbox.

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
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/semantic_oracle/gpl-sidecar \
  --target moderngekko-dolphin-oracle-attested
```

Do not build inside the external checkout: the verifier intentionally rejects
untracked build files.

## Authority and comparison gates

The pilot is deliberately limited to reviewed relocation-free integer/control
profiles:

- `msgctrlWait-v1`: `0x80132454`, `0x78` bytes;
- `GStextureLockImage-v2`: `0x800EF548`, `0x30` bytes.
- GC6E01 `main.dol` SHA-1:
  `870e8b9693ca780782d80f22a6a4572d8ba9458f`

Before invoking Dolphin, the driver parses the original DOL header and requires
the reference object's extracted `.text` to equal that exact DOL slice. Both
objects must have no ELF relocations, as reported by the configured
`powerpc-eabi-readelf`. Candidate text may have a different word-aligned length
within the 4,096-byte limit; the driver never pads or rewrites it. The sidecar
installs each image in—and excludes the same fixed, validated 4,096-byte
sandbox from—each whole-RAM digest, so length alone cannot create a mismatch.

The fixed seed generates 1,000 fixtures by default. `msgctrlWait-v1` starts
with ten named boundary cases; `GStextureLockImage-v2` starts with eight levels
covering 0, 1, 7, 8, 255, upper-register noise, and 16-bit reference-count
wraparound. Version-stable xorshift cases follow. Each profile declares its
register/RAM setup and watches. The comparison contract is:

- execution status and final return PC;
- requested `r3`;
- readable object and stream RAM watches;
- FNV-1a-64 over all MEM1 except the fixed 4,096-byte code sandbox.

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
  --reference-elf "$d/target.o" \
  --candidate-elf "$d/candidate.o" \
  --report-file build/semantic_oracle/GStextureLockImage-report.json
```

The default `--dol` is `orig/GC6E01/sys/main.dol`. The default PowerPC tools
are read from the campaign tool cache; override them with `--objcopy` and
`--readelf` when needed.

Requests, results, extracted text, reports, the original DOL, objects, and the
sidecar executable/attestation are generated or local-only artifacts. Keep them
under the ignored `build/` tree or outside the repository and never commit
them. The driver copies the attested executable once into its private run
directory and uses that same byte snapshot for both reference and candidate;
it hashes the snapshot after each execution. The benchmark additionally passes
its run-start binary and attestation fingerprints into every driver call and
rejects any report, engine, provenance, checkout authority, or DOL authority
that disagrees. By default, request/result files live in a private temporary
directory and are
deleted after the run. To retain them, provide all four request/result options
and place every path under `build/` or outside the repository.

## Tests and interpretation

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest -v \
  tools/decomp_work/semantic_oracle/test_driver.py
```

The tests use fake binutils, a synthetic DOL, and a fake external sidecar; they
do not need ModernGekko or extracted game files. They cover pin verification,
reference authority, deterministic profile layout, relocation rejection, safe
file round trips, immutable executable snapshots, pre/post build attestation,
variable-length sandbox normalization, engine/provenance rejection, return-PC
anchoring, register/RAM diagnostics, and whole-RAM digest mismatches.

An equal result means the two functions behaved the same over this finite
fixture set and observable contract. It is useful evidence, not a proof of
equivalence. Any candidate proposed for `src/` still needs the repository's
standard configure, build/report, full build, and progress checks.
