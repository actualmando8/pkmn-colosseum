# gs_material.c decomp session log

## Session: May 17 2026

### Functions matched to 100%

#### fn_800E8FE8 (99.44% → 100%)
Branch condition inversion: `if (val & 3)` → `if (!(val & 3))`

#### fn_800E3B08 (99.23% → 100%)
sdata2 label names: replaced `gsMatPoolCount` → `lbl_8047AB78`, `gsMatPool` → `lbl_8047AB74` (raw pointer used as u32)

#### fn_800E8F74 (98.33% → 100%)
sdata2 label: replaced named `gsMatDistThresholdSq` → `lbl_8047AB88`

#### fn_800E4D3C (95.86% → 100%)
- sdata2 label: replaced named pool vars with `lbl_8047AB74`, `lbl_8047AB78`, `lbl_8047AB70`
- Extra `mr r3, r0`: cast `(u32)fn_800E27B0((u32)handle)` forces explicit register move

#### fn_800EC990 (89.47% → 100%)
CSE-defeating reload pattern: explicit `flags = *(u32*)p` after each store to force memory reloads that match target's load-all-then-store pattern

#### fn_800E3760 (99.93% → 100%)
Shadow check scope: moved `if (*(u32*)entry & 0x200000) fn_80190E60(...)` INSIDE the `if (r31 != NULL)` block so the null path skips it (matching target's beq to epilogue)

#### fn_800EC9DC (99.87% → 100%)
Control flow restructure: moved secondary flags checks (`if !(flags & 0x2000)`, `if !(flags & 0x8)`) INSIDE the outer `if (*(u32*)entry & 0x4)` block; target's first beq branches to epilogue when outer bit clear

#### fn_800E7290 (94.74% → 99.69%)
- Added `#pragma push; #pragma scheduling on` to enable CW instruction scheduling
- Used temp var `t` to force load-before-store ordering of the 0x38/0x3c pair
- Remaining 2 diffs: stw r3 vs stw r0 store-order (scheduler picks opposite order); stuck

#### fn_800E8EFC (91.85% → 94.07%)
- Changed loop variable `s32 i` → `u32 i` to generate `cmplwi` (unsigned) instead of `cmpwi` (signed)
- Remaining 3 diffs: prologue address computation `addi r30, r3, @l` vs `addi r0; mr r30, r0`; stuck

#### fn_800EA820 (98.18% → 98.63%)
- Swapped local declarations in inner block: `s32 r3 = 0; u32 f2 = ...` (r3 first, f2 second) to fix r3/r4 register swap
- Enlarged buf: `u8 buf[48]` instead of `u8 buf[12]` to match target's -0x50 frame
- Remaining 3 diffs: `beq X; bne Y` vs `bne Z` code ordering for null check; stuck

### Functions NOT changed (baseline maintained)
- fn_800E5550/638C/65CC/68D8 (98.92%): reg-alloc permutation in first loop (r29/r30 swap for count/counter); scheduling issue
- fn_800E5790 (91.02%): same reg-alloc + initialization order issue  
- fn_800E3604 (99.37%): reg-alloc permutation (r29/r30/r31 for slotMatch/offset/mobj-inner)
- fn_800E8FA0 (94.17%): peephole-bgtlr (`if (h > 0x1e0) return` → bgtlr vs ble+blr)
- fn_800ED4D4: anonymous-sda21 BLOCKED

### Anti-fraud verification
- `git diff --name-only`: only `src/game/gs_material.c`
- No `*_fn_*.inc` files modified
- No `#if 0` → `#if 1` flips
