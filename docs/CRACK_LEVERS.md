# CRACK_LEVERS — CodeWarrior reg-alloc / shape lever catalog

Read this once; the lane prompts only send the header (file + fns + tag).

## Workflow (WSL-free, never bare `bash`)
```
python tools/decomp_work/band.py init  <TAG> <FILE>
python tools/decomp_work/band.py diff   <TAG> <fn>     # see the exact instr miss
# edit tools/decomp_work/scratch/band_<TAG>.c
python tools/decomp_work/band.py check  <TAG>
python tools/decomp_work/band.py save   <TAG> <fn>     # only persists >=100%
python tools/decomp_work/kg/kg.py record-crack <fn> <lever>   # after a win
python tools/decomp_work/kg/kg.py q lever-targets <fn>        # which lever fits
```
A SAVED body must be REAL C. `asm{}` / `asm void` / `#include "*.inc"` = INSTANT
gate rejection + logged as fraud. NO vault-sync skill.

## Diagnose by miss size (`band.py diff`)
- **small (1-3 instr, reg numbering / scheduling)** → a reg-alloc lever below.
- **large (wrong control-flow shape or types, fn is <95%)** → REWORK: run
  `python tools/decomp_work/m2c_draft.py <fn> <FILE>` for a faithful draft, fix the
  shape/types to match, THEN apply levers.

## Reg-alloc levers
- **declaration ORDER** of co-surviving locals sets the r31-down register map — reorder them.
- **named TEMP** for a repeated subexpr swaps operand register order.
- **parameter-aliasing**: alias a param into a local to fix a base-pointer regswap.
- **nested call**: nest a call expr to keep its result in r3 and avoid a non-volatile spill.
- **no-temp read-modify-write**; **block-scope decl restructuring** (opt-level gated).

## Type levers
- **`int` (NOT `s32`)** enables loop-unrolling — free, zero-risk, try it.
- a **u16/u8 return retyped to u32** drops a `clrlwi` mask and cascades reg numbering.
- u8-vs-s8 / signedness at the compare site.

## Shape levers (TWW)
- constant-compare chains are a **`switch`** (sometimes needs a dead `case`/`default`).
- **avoid `goto`** (worsens O4); null-checks should **FALL THROUGH** to the null case.
- `?:` vs `if/else` codegen differs; cast-operator-KIND swap `(T*)x` vs `(T)x`.
- toggle `const` on a primitive param (omitted from the mangled name — try both).

## Pragmas / opt
- `#pragma peephole off/on`; `#pragma optimization_level N`; scheduling on/off;
  per-fn opt-level; `#pragma opt_common_subs off` (defeats cross-block CSE).

## FILE AS WALL IMMEDIATELY (not C-reachable — do not grind)
pure scheduler reorderings, `beq;b`-vs-`bne`, `@nnn`-vs-`@named` reloc,
SDA-numeric-vs-`@sda21` / conversion-literal artifacts. Report `WALL <fn> <%> <residual>`.
