# Contributing to the Pokémon Colosseum decompilation

Thanks for helping! This is a **matching** decompilation: the goal is C source
that the original Metrowerks CodeWarrior 1.3 compiler turns back into
**byte-identical** `main.dol` code. A function only counts when it matches the
original bytes **100.0000%**.

This guide covers the setup, the per-function workflow, and the rules that keep
the build honest. The [README](README.md) has the high-level overview; the
[matching guide](docs/matching_guide.md) has the deep CodeWarrior tricks.

## What you need (not shipped here)

You must legally obtain these yourself — the repo ships neither:

1. **An original Pokémon Colosseum disc** (NTSC-U `GC6E01`). Extract it with
   [dtk](https://github.com/encounter/decomp-toolkit) and place the files under
   `orig/GC6E01/`.
2. **Metrowerks CodeWarrior for GameCube 1.3** (`mwcceppc.exe`), under
   `tools/mwcc_compiler/GC/1.3/`. The Docker image fetches the publicly
   mirrored bundle from `files.decomp.dev`.

Without both, you can still run `tools/compile_check.py` (does the C compile?)
but not the match% step (which needs the ROM-extracted target objects).

## Setup

```bash
# Windows, native
python configure.py        # generates build.ninja (objdiff.json is tracked)
ninja                      # builds base objects

# or Docker — see docker/README.md
docker build -t pkmn-colosseum-build .
docker run --rm -v "$PWD:/work" pkmn-colosseum-build configure.py
```

## The per-function workflow

1. **Pick a target.** `python tools/progress.py` lists files worst-match first.
   Open one and find a function still wrapped in an `asm { #include ... }`
   block — that's an unconverted target.
2. **Get context.** `python tools/decompctx.py src/path/to/file.c` flattens the
   includes into a `ctx.c` you can paste into [decomp.me](https://decomp.me).
3. **Write the C**, then **compile**: `python tools/compile_check.py src/path/to/file.c`.
4. **Diff against the original**:
   ```bash
   ./tools/objdiff-cli.exe diff -1 <target.o> -2 <yours.o>
   ```
   or open the [objdiff GUI](https://github.com/encounter/objdiff) and point it
   at this repo — it reads the tracked `objdiff.json` and shows a live
   side-by-side diff as you edit.
5. **Iterate to 100%.** Near-misses are usually register allocation or
   instruction order — see `docs/matching_guide.md` and the CodeWarrior quirks
   notes for the levers (declaration order, casts, pragmas).
6. **Open a focused PR** (one purpose per PR).

## Strict rules — read before your first PR

These keep the match measurement trustworthy. PRs that break them will be
rejected:

- **Never edit `*_fn_*.inc` files.** Those are the ROM-truth bytes the diff
  measures *against*. Editing them forges a match.
- **Never flip `#if 0` → `#if 1`** on asm-wrapper scaffolding to "activate" the
  assembly. The wrappers are placeholders to be *replaced* by matching C, not
  the goal state.
- **Never edit** the symbol map, `splits.txt`, linker script, or `objdiff.json`
  unit mappings to make a function appear matched.
- **A function counts only at 100.0000%.** "99.8%, basically done" is not a
  match — always re-measure with `objdiff-cli` / `tools/progress.py` and report
  the real number.
- **Match PRs touch only `.c` / `.h` / build-config files.** A compiler-version
  change (`config/.../compile_config.json`) is fine if it's reviewed and the
  byte-match build still reproduces `main.dol` identically.

## PR checklist

- [ ] One logical change (one function, or one closely-related set).
- [ ] `python tools/compile_check.py <file>` is green.
- [ ] The improved function(s) measure **100.0000%** (paste the `objdiff-cli`
      output in the PR).
- [ ] No `*_fn_*.inc`, symbol, split, or linker changes.
- [ ] Descriptive title, e.g. `gs_title: match fn_80024CDC (cursor anim)`.

## Progress tracking

Match progress is published to **[decomp.dev](https://decomp.dev)** — see the
badge in the README. The `progress.yml` workflow regenerates the report on each
push to `master`.

Questions? Open an issue, or join the broader GameCube/Wii decomp community on
the [decomp.dev Discord](https://decomp.dev).
