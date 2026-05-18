# Decomp notes: src/game/colosseum_event.c

## Status snapshot
5/5 matched this session (2026-05-17); 22 functions remain in near-miss band (88-99%)

## Blocked near-misses

- **fn_80206C94** @ 92.94% — stmw-emission
  - Symptom: target uses stmw/lmw; TU emits stw/lwz
  - Next leads: TU split
  - Last attempt: 2026-05-17 by executor

- **fn_802099AC** @ 89.88% — stmw-emission
  - Symptom: target uses stmw/lmw; TU emits stw/lwz
  - Next leads: TU split
  - Last attempt: 2026-05-17 by executor

- **fn_80206AEC** @ 89.42% — stmw-emission
  - Symptom: target uses stmw/lmw; TU emits stw/lwz
  - Next leads: TU split
  - Last attempt: 2026-05-17 by executor

## Blocked near-misses (new, from this session)

- **fn_8020DF10, fn_8020DF50, fn_8020E020, fn_8020E068** @ 96.25% each — clrlslwi-cse
  - Symptom: target emits `clrlslwi r4, r4, 24, 3` (mask+shift on param reg); decomp emits `slwi r4, r0, 3` (reuses r0 from earlier clrlwi for comparison)
  - Root cause: CW CSE's `clrlwi r0, r4, 24` from the `slot >= 4` check and reuses r0 for the multiply. Target recomputes from r4.
  - Tried: no `(u32)` cast, explicit `(u8)` cast, optimization_level 2/3, restructured condition
  - Next leads: possibly `-opt nopeephole` or different if-structure to kill r0 liveness before multiply
  - Last attempt: 2026-05-17 by executor

- **fn_8020E204, fn_8020E488** @ 93.64% — clrlslwi-cse (u16 variant)
  - Symptom: target emits `clrlslwi r0, r3, 16, N`; decomp emits `slwi r0, r5, N`
  - Root cause: CW CSE's `clrlwi r5, r3, 16` from the `index > *count` comparison and reuses r5 for the shift.
  - Tried: inline deref, different cast forms
  - Next leads: change comparison to not produce r5 clrlwi result (e.g. use u32 param type)
  - Last attempt: 2026-05-17 by executor

- **fn_8020E614** @ 88.64% — booleanize-idiom
  - Symptom: target uses `neg r3, r3; subic r0, r3, 0x1; subfe r3, r0, r3` (3 instrs); decomp uses `neg r0, r3; or r0, r0, r3; srwi r3, r0, 31` (3 instrs)
  - Root cause: CW emits different booleanize idiom for `!= 0` check
  - Tried: `s32` vs `u32` return type, `!!`, local variable
  - Next leads: unclear — may require different compiler version or flag
  - Last attempt: 2026-05-17 by executor

- **fn_80209CB4** @ 93.09% — mr.-cw-signed + booleanize-idiom
  - Symptom: `mr.`/`cmpwi` pattern (signed, needs peephole) + `subic/subfe` vs `neg/or/srwi` booleanize
  - Tried: not yet attempted (Ghidra stub structure complicates fix)
  - Next leads: convert from Ghidra stub to proper function signature, then apply peephole + fix condition
  - Last attempt: 2026-05-17 by executor

## Recently landed

- **fn_802078F0** — `#pragma peephole on` + `if ((ctx = r3) != NULL)` → `mr. r31, r3` (2026-05-17)
- **fn_80209D90** — `#pragma peephole on` + `if ((ctx = r3) == NULL)` → `mr. r31, r3` (2026-05-17)
- **fn_8020A478** — `#pragma peephole on` + `if ((ctx = r3) != NULL)` → `mr. r31, r3` (2026-05-17)
- **fn_8020FC70** — `#pragma peephole on` + `mode != 0` condition + inline `fn_801F0204` call in arg (2026-05-17)
- **fn_80211040** — same pattern as fn_8020FC70 (2026-05-17)

## Matched (100%)
(From this session — move to matched list after confirming stable)
fn_802078F0, fn_80209D90, fn_8020A478, fn_8020FC70, fn_80211040

## Session log

### 2026-05-17 — executor (Sonnet 4.6)

**Goal:** push HINT near-misses in 88-99% band to 100%.

**Technique discoveries:**

1. **mr./clrlwi. pattern** (`peephole-combine`): Functions with `if (ctx == NULL) return` where ctx is saved to a callee-save register needed `#pragma peephole on` + assignment-in-condition form `if ((ctx = r3) == NULL)` to get CW to emit `mr. r31, r3` instead of `mr r31, r3; cmplwi r31, 0`. Without the pragma, CW 1.3 at -O4 does NOT emit `mr.` even with the combined form.

2. **clrlwi. pattern** for u16: same peephole fix, but condition needs to be `mode != 0` (not `mode != 1`) to get `clrlwi. r0, r30, 16; beq` — the target branches when mode==0, not mode==1. Original condition was wrong semantically.

3. **inline-fn-arg reg-alloc**: When `tablePtr = fn_801F0204(...)` is stored in a local and then passed as 5th arg, CW goes through r0. Inlining as `fn_802085C4(p3, p1, 2, 0, fn_801F0204(fn_801F0234(0x12)))` avoids the r0 intermediate and matches `mr r7, r3` directly.

**Unresolved patterns:**

- `clrlslwi r4, r4, 24, 3` vs `slwi r4, r0, 3`: CW CSE's the u8/u16 mask from an earlier comparison into a scratch register and reuses it for the multiply. No source change found to prevent this. Possibly needs `-opt nopeephole` global flag.
- `neg+subic+subfe` vs `neg+or+srwi`: different booleanize idiom. Unknown trigger.

**Functions moved to 100%:** 5 (fn_802078F0, fn_80209D90, fn_8020A478, fn_8020FC70, fn_80211040)
**Functions attempted but not improved:** fn_8020E614, fn_80209CB4, fn_8020DF10, fn_8020DF50, fn_8020E020, fn_8020E068, fn_8020E204, fn_8020E488

## Untouched near-misses
(Pull from `python tools/near_miss_report.py` for full list)
fn_80210998 (96.44%), fn_803103E8 (95.70%), fn_80211810 (95.00%), fn_80210D04 (93.96%), fn_8020ECE0 (91.36%), fn_8020B330 (89.68%)
