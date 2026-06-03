# Pokémon Colosseum decompilation (GC6E01 / GPXE01)

[![Progress](https://decomp.dev/dougchansan/pkmn-colosseum.svg)](https://decomp.dev/dougchansan/pkmn-colosseum)

A work-in-progress matching decompilation of the GameCube game **Pokémon
Colosseum** (NTSC-U: `GC6E01`). The goal is byte-identical reproduction of the
original `main.dol` from C source, using the same Metrowerks CodeWarrior
compiler that Genius Sonority used in 2003.

## Status

| Metric | Value |
|---|---|
| Function match | ~46.5% (3733 / 8029 functions) |
| Byte match | ~44.0% (664 KB / 1.5 MB) |

The latest figure is regenerated on every push — see `.github/workflows/progress.yml`.

## You will need

The repository does **not** ship the game or the compiler. You must legally
obtain:

1. **An original Pokémon Colosseum disc image** (`GC6E01.iso` or `.gcm`).
   Place under `orig/GC6E01/`.
2. **Metrowerks CodeWarrior for GameCube 1.3** (`mwcceppc.exe`). The Docker
   image fetches the publicly mirrored bundle from `files.decomp.dev`. If
   you build outside Docker, drop the toolchain under
   `tools/mwcc_compiler/`.

Without both, only `compile_check.py` works — the match% step needs the
extracted target objects.

## Quick start (Docker)

```bash
docker build -t pkmn-colosseum-build .
docker run --rm -v "$PWD:/work" pkmn-colosseum-build configure.py
docker run --rm -v "$PWD:/work" pkmn-colosseum-build python3 tools/progress.py
```

See [`docker/README.md`](docker/README.md) for details.

## Quick start (Windows, native)

```powershell
# 1. Extract orig disc with dtk (not included)
# 2. Place mwcceppc.exe under tools/mwcc_compiler/GC/1.3/
python configure.py
ninja
python tools/progress.py
```

## Repository layout

```
src/            C source — match targets
include/        Headers
asm/            Unconverted assembly (shrinks over time)
config/GC6E01/  Symbol map, splits, linker script
build/          Output (gitignored)
tools/          Build, diff, and match utilities
  progress.py            Per-file match% report
  decompctx.py           Flatten includes → ctx.c for decomp.me
  weak_order_diff.py     Detect symbol-order regressions
  gen_struct_catalog.py  Refresh docs/struct_sizes.md
  compile_check.py       Compile one .c
docs/
  matching_guide.md      Compiler tips and patterns
  struct_sizes.md        Typedef catalog
  tu_split.md            Future direction
```

## Contributing

See **[CONTRIBUTING.md](CONTRIBUTING.md)** for full setup, the per-function
workflow, the objdiff GUI, and the rules that keep the match honest. In short,
to push a function from `X%` to `100%`:

1. Pick a file from `python tools/progress.py` (worst-match files printed first).
2. Run `python tools/decompctx.py src/path/to/file.c` to get a `ctx.c` you
   can paste into [decomp.me](https://decomp.me).
3. Iterate locally with `python tools/compile_check.py src/path/to/file.c`.
4. Verify with `./tools/objdiff-cli diff -1 <target.o> -2 <yours.o>`.
5. Open a PR.

Strict rules:

- **Never edit `*_fn_*.inc` files** — those are the ROM-truth bytes the
  diff measures against.
- **Never flip `#if 0` → `#if 1`** on asm-wrapper scaffolding to forge a
  match; the wrappers exist as placeholders, not as the goal state.
- Only `.c` / `.h` / config changes are accepted in match-improvement PRs.

## Inspiration

Tooling and methodology lifted from established GCN/Wii decomps:

- [`zeldaret/tp`](https://github.com/zeldaret/tp) — Twilight Princess
- [`zeldaret/tww`](https://github.com/zeldaret/tww) — Wind Waker
- [`projectPiki/pikmin2`](https://github.com/projectPiki/pikmin2) — Pikmin 2
- [`PrimeDecomp/prime`](https://github.com/PrimeDecomp/prime) — Metroid Prime
- [`doldecomp/melee`](https://github.com/doldecomp/melee) — Super Smash Bros. Melee

## License

The decompiled C source in this repository is released under the [MIT
license](LICENSE) (scope details in [NOTICE](NOTICE)). The Pokémon Colosseum
game, its assets, and the CodeWarrior toolchain are property of their
respective copyright holders — this project does not redistribute any of that
material.

## Disclaimer

This is a clean-room decompilation, produced by disassembling the user's
own legally-obtained copy of the game and rewriting the disassembly back
into C until the compiler reproduces the original bytes. No copyrighted
game data, assets, or compiler binaries are committed to this repository.
