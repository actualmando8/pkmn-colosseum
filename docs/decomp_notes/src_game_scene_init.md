# Decomp notes: src/game/scene_init.c

## Status snapshot
107/261 @ 100% (41.0%, as of 2026-05-14 commit a4c3074)

## Recently landed (2026-05-14, agent w10)

5 transition-block-init functions, all via the same pattern:

- **fn_800388C4** (90.6% → 100%) — `#pragma scheduling off` fixes prologue order; target saves LR (`stw r0, 0x14(r1)`) before `li` arg setup. Scheduling off prevents CW reorder.
- **fn_800373C8 / fn_80037468 / fn_80037508 / fn_800375A8** (all 90.2% → 100%) — `#pragma peephole off` eliminates spurious `clrlwi`/`extsb`; **direct `(s16)f32` cast** (not `(s16)(s32)f32`) avoids the extra `extsh r0, r0`. `sth` already truncates to 16 bits.

**Key technique discovered:** `(s16)f32_expr` (direct float-to-s16 cast)
prevents CW from emitting the extra `extsh r0, r0` that `(s16)(s32)f32_expr`
produces. Generalizes beyond this file — file under "float→short store"
pattern.

## Blocked near-misses
None identified yet.

## Untouched near-misses
Per recon 2026-05-13, 87 near-misses in this file. Top candidates:

| Function | % |
|---|---|
| fn_80040308 | 99.93 |
| fn_8004B7EC | 99.68 |
| fn_80038990 | 98.93 |
| fn_80037180 | 98.70 |
| fn_8004B598 | 98.66 |

## Session log

- **2026-05-14 (w10)** — 5 new 100%s via `#pragma peephole off` + direct `(s16)f32` cast. Pattern works for any "f32 → s16 store" function.
