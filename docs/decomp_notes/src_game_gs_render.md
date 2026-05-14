# Decomp notes: src/game/gs_render.c

## Status snapshot
190/258 @ 100% (73.6%, as of 2026-05-13 commit cf60079)

## Recently landed (2026-05-13, agent w2)

Pattern A: **`*(u32*)` → `*(s32*)` for cmpwi**. 9 functions fixed by flipping the load type so CW emits a signed compare (`cmpwi`) matching the target.

- fn_800D56C0, fn_800D5724, fn_800D579C, fn_800D5A38, fn_800D5AB0, fn_800D5B28, fn_800D5BA0, fn_800D9ED8, fn_800DBAA4 — all 98% → 100%.

Other wins:

- **fn_800DB900** — same cast + separate `u32 off` to control add-operand order.
- **fn_800D7894** — explicit `cnt` var declared before `p` to fix SDA2 load order for `mtctr`.
- **fn_800E02E8 / fn_800E032C / fn_800E0370** — `u8 tmp[0x38]` → `u8 tmp[0x30]`. Smaller array forces the 0x40 frame size to match target.

## Blocked near-misses

- **fn_800D7468** @ 98.67% — `instr-scheduling-order` (backward `f2/f1` load order)
  - Symptom: target loads f2 then f1, our build loads f1 then f2.
  - Tried: b/c declaration swap (4/5 attempts).
  - Next leads: per-function `#pragma scheduling on` push/pop. Untested.
  - Last attempt: 2026-05-13 (w2)

## Untouched near-misses
~63 remain per recon. Top candidates:

| Function | % |
|---|---|
| fn_800DE128 | 99.65 |
| fn_800E02E8 | 99.65 (now landed) |
| fn_800D5724 | 98.00 (now landed) |

## Session log

- **2026-05-13 (w2)** — 14 near-misses → 100% via `s32` cmpwi cast, mtctr load-order, and frame-size shrink.
