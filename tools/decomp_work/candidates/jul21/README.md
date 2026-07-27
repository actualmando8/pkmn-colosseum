# jul21 research candidates — gs_render_util composed improvement

Verified on scout trees at base 4b68d012. No commits made under strict rules
(no function reached 100.0); these are measured, reproducible source forms for
integration to adopt.

## Composed unit-level change for src/game/gs_render_util.c

Two independent, stacking, strict-legal edits:

1. **`extern f32 lbl_80478ACC;` -> `extern f32 lbl_80478ACC[];`** (uses become
   `lbl_80478ACC[0]`). Forces the absolute `lis/@l` addressing the target uses
   (the existing source comment already documented this behavior). Effect,
   measured TU-wide, zero regressions:
   - fn_800D1B3C  89.16 -> 91.50
   - fn_800D1D00  89.87 -> 92.28
   - fn_800D258C  90.52 -> 93.00
   - _cameraLoadCameraMatrix 78.60 -> 79.22 (alone)
2. **`cameraLoadCameraMatrix_9727.c`** — replacement body for
   `_cameraLoadCameraMatrix__FP9_GScamera12GSgfxLayerID`. Produced by the
   regenerate-then-close pipeline (kimi-k3 78.60->94.37, fable audit + surgical
   close ->97.27): authentic block-emission order (ortho arm fall-through),
   corrected decl-slot order, target float-abs ternary arm order. Audited clean
   of shaping constructs; semantics credible; offsets consistent with
   GSRenderCamera field map.

**Composed** (edit 1 + edit 2 with its lbl uses in array form):
`_cameraLoadCameraMatrix` = **98.309**, siblings keep their gains, nothing
regresses. Composed-form residual on camera: FPR f0/f1 web (known coloring
wall class, ~15 rows) + fn-specific scheduling.

`fn_8023C530_below_baseline_ref.c` is a regeneration reference form that scored
BELOW the existing source (94.18 vs 99.61) — kept only as a record of what the
regeneration lane produced; do not adopt.

Provenance/method details: session scout branches scout/fable-camera-20260721,
scout/claude-sdarelo-20260721; measurements via objdiff-cli against unit
main/game/gs_render_util.
