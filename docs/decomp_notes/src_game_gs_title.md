# Decomp notes: src/game/gs_title.c

## Status snapshot
~42/65 @ 100% (rough, re-measure with `tools/objdiff-cli.exe`)

## Blocked near-misses

All remaining near-misses (>= 90%) are blocked by these deep compiler-level
issues. **Do not re-attempt these unless you bring a new technique.**

- **fn_80024DBC / fn_80024F2C / fn_80024F9C / fn_8002520C / fn_80024308** —
  `anonymous-sda21`
  - Symptom: CW emits `lis r3, @255@sda21@ha; addi r3, r3, @255@sda21@l` for
    the float-bias constant. Target uses a named label
    (e.g. `lbl_8047B8D0@sda21@ha`). objdiff sees this as a symbol mismatch
    that no source change can fix.
  - Tried: every pragma combo, lifting the constant into a global, casting
    rearrangement — all attempts produce the same anonymous name.
  - Next leads: linker-level remapping, custom mwcc patch, or moving the
    .sdata2 entry into a different `.c`/`.h` to coincidentally change
    objdiff's anonymous numbering. Untested.
  - Last attempt: 2026-05-13 (w9 burned ~20 attempts here)

- **fn_80025490 / fn_8002537C / fn_80024A2C** — `peephole-bne-b`
  - Symptom: target was compiled with the backend peephole optimizer active
    (`beq @far` → `bne @near; b @far`). Our build uses global
    `-opt nopeephole`, so CW never produces `bne; b` pairs.
  - Tried: condition inversion at C level, function-local
    `#pragma push/pop peephole on`, all unsuccessful (file-wide flag
    overrides).
  - Next leads: split the file into two TUs so just these fns build with
    peephole on. Untested.
  - Last attempt: 2026-05-13 (w9)

- **fn_800218BC / fn_80024CDC / fn_80024308** — `instr-scheduling-order`
  - Symptom: CW's scheduler hoists `lfs/lfd` loads earlier than target.
    Target has `lfs f31; li r31, 0x0`, we get them swapped.
  - Tried: 5+ scheduling/peephole pragma combos.
  - Next leads: per-function scheduling pragma push/pop. Untested in
    isolation (was combined with other changes that may have masked the
    effect).
  - Last attempt: 2026-05-13 (w9)

- **fn_80025F84 / fn_80025A80 / fn_80024A2C** — `reg-alloc-permutation`
  - Symptom: target uses r28/r29/r30 where we use r29/r30/r31 (same count,
    shifted by 1).
  - Tried: declaration order reordering (does not affect CW allocation at
    -O4 for this fn).
  - Next leads: `-O2` + carefully-chosen variable order. Untested for these
    specific fns; this technique worked elsewhere (see
    `feedback_or_operand_and_optlevel.md`).
  - Last attempt: 2026-05-13 (w9)

## Matched
Run `tools/objdiff-cli.exe` for the up-to-date list.

## Session log

- **2026-05-13 (w9)** — explored all 12 near-misses, no progress. Identified
  4 distinct blocker classes (above). Source reverted to baseline.
