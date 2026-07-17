# GPL semantic-oracle sidecar

This directory contains source for a separate executable that links the
GPL-2.0-or-later RecompCore/Dolphin fork pinned by ModernGekko. Build products
must stay under the ignored `build/` tree (or another external cache), and the
executable must only be invoked out of process by the campaign tooling.

The source is not part of the game build. It accepts deterministic PPC code,
register state, and RAM patches in a versioned JSON request, runs the code by
calling Dolphin's interpreter directly, and returns requested GPR/RAM
observations plus bounded whole-MEM1 diagnostics. This first lane does not run
ModernGekko/DolRecomp native generated chunks; its precise role is a Dolphin
interpreter oracle built from ModernGekko's exact reviewed dependency tree.

Build against an already initialized, exact-pinned checkout:

```sh
cmake -S tools/decomp_work/semantic_oracle/gpl_sidecar \
  -B build/semantic_oracle/gpl-sidecar -G Ninja \
  -DMODERNGEKKO_SOURCE_DIR=/absolute/path/to/ModernGekko \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/semantic_oracle/gpl-sidecar \
  --target moderngekko-dolphin-oracle-attested
```

CMake refuses the build unless ModernGekko, RecompCore, and DolRecomp are at
the reviewed commits and the complete recursive checkout is clean. The
attested target repeats that verification before and after linking, then writes
an adjacent `.attestation.json` binding the binary hash, this directory's C++
and CMake inputs, the verifier, and the pinned checkout. The executable embeds
the reviewed commits in every result; the MIT-side Python driver verifies the
attestation and executes one private binary snapshot for both sides of each
comparison.

The files in this directory are GPL-2.0-or-later as marked; see `COPYING`.
The linked upstream aggregate documents GPLv3 compatibility and carries its
own license notices. This is a technical distribution boundary, not legal
advice.

Do not copy ModernGekko, generated recompilation C, game objects, extracted
assets, compiler binaries, or the resulting executable into version control.
