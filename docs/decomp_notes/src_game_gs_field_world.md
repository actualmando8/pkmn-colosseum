# Decomp notes: src/game/gs_field_world.c

## Status snapshot
649/735 @ 100% (88.3%, as of 2026-05-13 commit 51e49ec)

## Recently landed (2026-05-13, agent w1)

- **fn_8011E1D4 / E21C / E264 / E2DC / E324** — 99.72% → 100%. Change `u16 idx` → `s32 idx`. Forces CW to emit `clrlslwi r4, r4, 16, N` using the original param register instead of zero-extending through a temp in r0.
- **fn_801231A4** — 99.24% → 100%. `threshold <= (u8)count` → `(u16)threshold <= (u16)(u8)count`. Forces unsigned `cmplw` instead of signed `cmpw`.
- **fn_80129718** — 98.75% → 100%. Cast `fn_80142368(...)` return to `(u32)` before `!= 0`. Forces `cmplwi` (unsigned) instead of `cmpwi` (signed).
- **fn_80129514** — 98.68% → 100%. `u16 arg2, u16 arg3` → `s32 arg2, s32 arg3`. Fixes register allocation for `clrlslwi`.

Technique pattern: **demoting `u16` params to `s32`** keeps the value in the original param register through `clrlslwi`, avoiding an extra `clrlwi`/`mr` pair.

## Untouched near-misses
86 near-misses remain (per recon 2026-05-13). Top candidates:

| Function | % |
|---|---|
| fn_80129280 | 99.85 |
| fn_80128E38 | 99.60 |
| fn_801294C4 | 99.50 |

Run `tools/objdiff-cli.exe` for the up-to-date list.

## Blocked near-misses
None identified yet — explore freely.

## Session log

- **2026-05-13 (w1)** — 8 near-misses pushed to 100% via `u16→s32` and unsigned-cast techniques.
