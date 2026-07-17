# GPL semantic-oracle sidecars

This directory builds two separate executables from the exact ModernGekko
dependency tree. Both stay outside the game build and communicate with the MIT
campaign tooling only through versioned JSON files:

- `moderngekko-dolphin-oracle` executes reference and candidate PPC with
  Dolphin's interpreter.
- `moderngekko-native-oracle` executes the original retail function through
  DolRecomp-generated host code and the matching standalone `dr_cpu` runtime.

The Python driver first requires native-original output to equal Dolphin's
original output for every fixture. Only then does it compare the candidate with
the Dolphin reference. Native execution is therefore a qualification lane, not
a matching authority; objdiff and the canonical full-DOL build remain the
acceptance authorities.

Build against an initialized, exact-pinned checkout:

```sh
cmake -S tools/decomp_work/semantic_oracle/gpl_sidecar \
  -B build/semantic_oracle/gpl-sidecar -G Ninja \
  -DMODERNGEKKO_SOURCE_DIR=/absolute/path/to/ModernGekko \
  -DORACLE_DOL=/absolute/path/to/GC6E01/main.dol \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/semantic_oracle/gpl-sidecar \
  --target moderngekko-semantic-oracles-attested
```

CMake refuses dirty or wrongly pinned ModernGekko, RecompCore, and DolRecomp
checkouts and refuses an in-repository build outside `build/`. The native build
regenerates the exact GC6E01 output, canonicalizes generated text to LF, verifies
zero drift in the DOL/tool/pins, and compiles only the two reviewed chunks
containing `msgctrlWait` and `GStextureLockImage`. Generated C and all binaries
remain in the ignored build directory.

Each executable receives an adjacent `.attestation.json`. The native executable
also receives `.generated-manifest.json`, binding the exact DOL hashes, all
three commits, the local DolRecomp binary hash, 157 generated files, and selected
chunk hashes. Finalization independently rehashes the complete generated source
root, verifies the actual DolRecomp executable hash, and probes the native
binary's embedded pins/tree identity before binding the manifest and executable.
The driver fingerprints and snapshots both executables at run start.

The pinned checkout's full RecompCore module-template path is not used. At this
revision its generated include/runtime CPU layouts diverge, its recorded
RecompCore provenance is stale, and emitted chunks do not consistently charge
`downcount`. The selected standalone DolRecomp ABI is internally consistent and
is admitted only after deterministic parity with Dolphin.

`dolphin_oracle.cpp` is GPL-2.0-or-later; `native_oracle.cpp` links the
GPL-3.0-or-later DolRecomp runtime and is marked accordingly. Upstream license
notices remain in the external checkout; `COPYING.GPL3` accompanies the native
sidecar source. This is a technical distribution boundary, not legal advice.

Do not commit generated recompilation C, game objects, extracted assets,
compiler binaries, or either executable.
